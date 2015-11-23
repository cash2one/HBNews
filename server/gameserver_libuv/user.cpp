#include "user.h"
#include "libuv_helper.h"

#include "task_mgr.h"
#include "burst_mgr.h"
#include "user_mgr.h"

#include "client_for_dbs.h"
#include "item_mgr.h"
//#include "item.h"
#include "hero_mgr.h"
#include "sys_setting_mgr.h"
#include "vip_mgr.h"
#include "user_shop_strategy.h"
#include "forbid_word_mgr.h"
#include "server_for_user.h"
UINT32 CHeroTeam::UpdateTeam( UINT8 iTeamIdx, UINT32 (&heros)[MAXNUM_HEROINTEAM] )
{
	if(iTeamIdx >= MAXNUM_TEAM){
		return 1;
	}

	memcpy(m_Teams[iTeamIdx], heros, MAXNUM_HEROINTEAM);
	return 0;
}

//bool CUser::Send( const char* pBuf, UINT32 len, uv_write_cb cb )
//{
//	if(!m_connect || !cb){
//		return false;
//	}
//
//	uv_write_t* write_req = (uv_write_t*)req_alloc();
//	int r = uv_write(write_req, m_connect, &pBuf, 1, cb);
//	ASSERT(r == 0);
//}

bool CUser::IntertwineConnect( uv_stream_t* connect )
{
	printf("!!!! user[%u] 链接[%p]准备绑定  !!!\n", UserID(), connect);
	if(m_connect != NULL){
		printf("!!!! user[%u] 已经绑定链接[%p]绑定 现在绑定[%p] !!!\n", UserID(), m_connect, connect);
	}
	
	CHECK_NULL_RETURN_FALSE(connect);
	m_connect = connect;
	////connect->data = this;//不直接存放指针 而是存放user_ID

	//stConnectUserData* pUserData = (stConnectUserData*)connect->data;
	//ASSERT(pUserData);
	//pUserData->iUserID = UserID();

	return true;
}

bool CUser::UnIntertwineConnect( uv_stream_t* connect )
{
	printf("!!!! user[%u] 当前链接[%p] 需要解绑[%p]  !!!\n", UserID(), m_connect, connect);
	ASSERT(m_connect == connect);
	CHECK_NULL_RETURN_FALSE(m_connect);
	CHECK_NULL_RETURN_FALSE(m_connect == connect);
	////connect->data = this;//
	//stConnectUserData* pUserData = (stConnectUserData*)m_connect->data;
	//ASSERT(pUserData);
	//pUserData->iUserID = NULL;
	m_connect = NULL;
	return true;
}

bool CUser::Send( UINT32 msg_id, google::protobuf::Message* pMessage )
{
	return CServerForUser::GetInstance()->send(m_connect, msg_id, pMessage);
}

void CUser::SendNtf(GS_C_NTF& ntf){
	if (ntf.is_mail_ntf_arr_init())
	{
		Send(e_msg_gs_c_mail_ntf, ntf.mutable_mail_ntf_arr());
	}
	if (ntf.is_mission_ntf_arr_init())
	{
		Send(e_msg_gs_c_mission_ntf, ntf.mutable_mis_ntf_arr());
	}
	if (ntf.is_marquee_ntf_init())
	{
		Send(e_msg_gs_c_marquee_ntf, ntf.mutable_marquee_ntf());
	}
	if (ntf.is_element_ntf_init())
	{
		Send(e_msg_gs_c_element_ntf, ntf.mutable_element_ntf());
	}
	if (ntf.is_friend_ntf_init())
	{
		Send(e_msg_gs_c_user_friend_ntf, ntf.mutable_friend_ntf());
	}
	if (ntf.is_chat_ntf_init())
	{
		Send(e_msg_gs_c_chat_ntf, ntf.mutable_chat_ntf());
	}
}

void CUser::ArenaLeave(bool bUserIsOnline/* = false*/){
	//未结束的竞技场判负！
	c_gs_arena_end_challenge_req req;
	req.set_is_win(false);

	gs_dbs_user_info_op_req* db_req = new gs_dbs_user_info_op_req;
	db_req->set_user_id(UserID());

	gs_c_arena_end_challenge_res res;
	GS_C_NTF ntf;
	gs_c_arena_challenge_ntf attacker_ntf;
	gs_c_arena_challenge_ntf defender_ntf;
	bool r = ArenaEndChallenge(req, *db_req, res, ntf, attacker_ntf, defender_ntf);

	if (bUserIsOnline){
		Send(e_msg_gs_c_arena_end_challenge_res, &res);
	}
	CServerForUser::GetInstance()->SendDBReq(db_req);

	//if (mail_ntf.mails_size() > 0){
	//	send(stream, e_msg_gs_c_mail_ntf, &mail_ntf);
	//}
	//如果对手在线就给对手发日志
	UINT32 iDefenderUserID = 0;
	if(attacker_ntf.IsInitialized()){
		ASSERT(defender_ntf.IsInitialized());
		iDefenderUserID = attacker_ntf.ele().match_user_id();
	}

	if (iDefenderUserID && defender_ntf.IsInitialized()){
		CUser* pDefender = CUserMgr::GetInstance()->GetUserByUserID(iDefenderUserID);

		if (pDefender){
			uv_stream_t* con = pDefender->GetConnect();
			ASSERT(con);
			CServerForUser::GetInstance()->send(con, e_msg_gs_c_arena_challenge_ntf, &defender_ntf);
		}
		//ASSERT(pDefender);
	}
}

//玩家一段时间内 不响应时调用， 并不一定只是在下线时调用
bool CUser::OnLeave()
{
	printf("CUser::OnLeave 玩家[%u]相关数据清理和对象析构 \n", UserID());
	//未结束的竞技场判负！
	
	ArenaLeave();

	//清理user_mgr的map
	bool r = CUserMgr::GetInstance()->RemoveUser(UserID());
	ASSERT(r);
	//删除自己
	delete this;
	return r;
}

bool CUser::Fill_gs_c_user_info_res( gs_c_user_info_res* res, gs_dbs_user_info_op_req &db_req)
{
	ASSERT(m_db_user_info);
	ASSERT(res);

	//const ::db_user_info& user_info = m_db_data_res->user_info();
	//db_user_info* user_info = m_db_user_info->mutable_user_info();
	//ASSERT(user_info);

	//填充玩家基本信息
	user_base_info* base_user_info = res->mutable_base_info();
	base_user_info->set_user_id(m_db_user_info->mutable_user_base_info()->user_id());                       
	base_user_info->set_name(m_db_user_info->mutable_user_base_info()->name());                     
	base_user_info->set_level(m_db_user_info->mutable_user_base_info()->level());                     
	base_user_info->set_exp(m_db_user_info->mutable_user_base_info()->exp());                 
	base_user_info->set_vip(m_db_user_info->mutable_user_base_info()->vip());                
	base_user_info->set_diamond(m_db_user_info->mutable_user_base_info()->diamond());                    
	base_user_info->set_gold(m_db_user_info->mutable_user_base_info()->gold());  
	
	base_user_info->set_buy_used_times_gold(GetEventTodayValue(EEventType_Buy_Gold, EEventSubType_None));
	base_user_info->set_buy_used_times_health(GetEventTodayValue(EEventType_Buy_Health, EEventSubType_None));
	base_user_info->set_buy_used_times_skill_point(GetEventTodayValue(EEventType_Buy_Skill_Point, EEventSubType_None));
	

	//作处理 再给客户端
	//base_user_info->set_health_cur(m_db_user_info->mutable_user_base_info()->health_cur());        
	//base_user_info->set_health_last_update_timestamp(m_db_user_info->mutable_user_base_info()->health_last_update_timestamp());
	base_user_info->set_health_cur(GetHealth());//更新了时间
	base_user_info->set_health_last_update_timestamp(_get_health_last_update_time());

	base_user_info->set_skill_point(GetSkillPoint());//重新计算了技能点和更新时间
	base_user_info->set_skill_point_last_update_timestamp(_get_skill_point_last_update_time());

	base_user_info->set_invoke_gold_free_left_num(GetInvokeGoldFreeLeftNum());	//计算当前还剩下多少次
	base_user_info->set_invoke_gold_free_update_timestamp(_get_invoke_gold_free_update_timestamp());		//计算cd刷新的时间 ： 初始化的时候和满到不满的这个时间
	base_user_info->set_invoke_diamond_free_left_num(GetInvokeDiamondFreeLeftNum());	//计算当前还剩下多少次
	base_user_info->set_invoke_diamond_free_update_timestamp(_get_invoke_diamond_free_update_timestamp());	//计算cd刷新的时间 ： 初始化的时候和满到不满的这个时间
	base_user_info->set_head_id(m_db_user_info->mutable_user_base_info()->head_id());
	base_user_info->set_head_frame_id(m_db_user_info->mutable_user_base_info()->head_frame_id());
	base_user_info->set_friend_point(m_db_user_info->mutable_user_base_info()->friend_point());
	base_user_info->set_signature(m_db_user_info->mutable_user_base_info()->signature());
	base_user_info->set_fight_power(m_db_user_info->mutable_user_base_info()->fight_power());
	base_user_info->set_crusade_point(m_db_user_info->mutable_user_base_info()->crusade_point());
	//填充子任务信息
	//db_user_sub_task_array* sub_tasks = m_db_data_res->mutable_user_info()->mutable_sub_tasks();
	db_user_sub_task_array* sub_tasks = m_db_user_info->mutable_sub_tasks();
	for (::google::protobuf::RepeatedPtrField<db_user_sub_task>::iterator it = sub_tasks->mutable_items()->begin(); it != sub_tasks->mutable_items()->end() ; ++it){
		user_sub_task* pItem = res->mutable_sub_tasks()->add_items();

		//pItem->set_sub_task_id(CClientID::GetInstance()->GetClientID(EELEMENTTYPE_CHILD_MIS, it->sub_task_id()));
		pItem->set_sub_task_id(it->sub_task_id());
		pItem->set_star(it->star());
		pItem->set_last_enter_time(it->last_enter_timestamp());
		pItem->set_last_day_enter_num(it->last_day_enter_num());
		pItem->set_total_enter_num(it->total_enter_num());

		pItem->set_last_reset_time(it->last_reset_time());
		pItem->set_last_day_reset_num(it->last_day_reset_num());

		//pItem->set_has_got_item1(it->has_got_item1());
		//pItem->set_has_got_item2(it->has_got_item2());
		//pItem->set_has_got_item3(it->has_got_item3());
		//pItem->set_has_got_item4(it->has_got_item4());
	}
	
	//邮件
	for (::google::protobuf::RepeatedPtrField< ::db_user_mail >::iterator  iter = m_db_user_info->mutable_mails_received()->mutable_mails()->begin(); iter != m_db_user_info->mutable_mails_received()->mutable_mails()->end(); ++iter){
		user_mail* pResMail = res->add_mail_elements();
		pResMail->set_mail_id(iter->mail_id());
		pResMail->set_send_time(iter->send_time());
		pResMail->set_type(iter->type());
		if (iter->has_content()){
			pResMail->set_content(iter->content());
		}
		pResMail->set_status(iter->status());
		if(iter->attachs_size()){
			::google::protobuf::RepeatedPtrField< ::db_mail_attach >::iterator itDBEle = iter->mutable_attachs()->begin();
			for (; itDBEle != iter->mutable_attachs()->end(); ++itDBEle){
				struct_game_element* pResEle = pResMail->add_elements();
				pResEle->CopyFrom(*itDBEle->mutable_element());
			}
		}
	}
	
	//填充英雄信息
	db_user_hero_array* heros = m_db_user_info->mutable_heros();
	for (::google::protobuf::RepeatedPtrField<db_user_hero>::iterator it = heros->mutable_items()->begin(); it != heros->mutable_items()->end() ; ++it){
		user_hero* pHero = res->mutable_heros()->add_items();

		pHero->set_hero_id(it->hero_id());
		pHero->set_star(it->star());
		pHero->set_quality(it->quality());
		pHero->set_level(it->level());
		pHero->set_cur_exp(it->cur_exp());
		pHero->set_got_timestamp(it->got_timestamp());
	
		//设置装备信息
		for (::google::protobuf::RepeatedPtrField<db_user_hero_equip>::iterator it_in = it->mutable_equips()->begin(); it_in != it->mutable_equips()->end() ; ++it_in){
			user_hero_equip* pEquip = pHero->add_equips();
			pEquip->set_equip_id(it_in->equip_id());
			pEquip->set_level(it_in->level());
		}
		
		//设置技能信息
		for (::google::protobuf::RepeatedPtrField<db_user_hero_skill>::iterator it_in = it->mutable_skills()->begin(); it_in != it->mutable_skills()->end() ; ++it_in){
			user_hero_skill* pSkill = pHero->add_skills();
			pSkill->set_type((user_hero_skill::e_skill_type)it_in->type());
			pSkill->set_level(it_in->level());
		}
	}
	
	//道具
	for (::google::protobuf::RepeatedPtrField< ::struct_game_element >::iterator iterItem = m_db_user_info->mutable_items()->mutable_items()->begin(); iterItem != m_db_user_info->mutable_items()->mutable_items()->end(); ++iterItem){
		struct_game_element* pItem = res->add_item_elements();
		pItem->CopyFrom(*iterItem);
	}
	
	//商店信息
	//for( ::google::protobuf::RepeatedPtrField< ::db_user_shop >::const_iterator it = m_db_user_info->shops().items().begin() ; it != m_db_user_info->shops().items().end() ; ++it){
	//	user_shop* pShop = res->add_shops();
	//	ASSERT(pShop);

	//	//pShop->CopyFrom(*it);//不是一个类型 不能copy！

	//	pShop->set_shop_type(it->shop_type());
	//	pShop->set_last_auto_refresh_time(it->last_auto_refresh_time());
	//	pShop->set_last_manual_refresh_time(it->last_manual_refresh_time());
	//	pShop->set_last_day_manual_refresh_num(it->last_day_manual_refresh_num());
	//	pShop->set_random_open_start_timestamp(it->random_open_start_timestamp());
	//	pShop->set_random_open_end_timestamp(it->random_open_end_timestamp());

	//	for (::google::protobuf::RepeatedPtrField< ::db_user_shop_item >::const_iterator itItem = it->items().begin(); itItem != it->items().end(); ++itItem){
	//		user_shop_item* pItem = pShop->add_items();
	//		ASSERT(pItem);
	//		pItem->set_pos(itItem->pos());
	//		pItem->set_element_type(itItem->element_type());
	//		pItem->set_element_id(itItem->element_id());
	//		pItem->set_element_num(itItem->element_num());
	//		pItem->set_consume_type(itItem->consume_type());
	//		pItem->set_consume_id(itItem->consume_id());
	//		pItem->set_consume_num(itItem->consume_num());
	//		pItem->set_flag(itItem->flag());
	//		pItem->set_is_sold(itItem->is_sold());
	//	}
	//}

	//任务信息
	ASSERT(m_userMissionStrategy);
	m_userMissionStrategy->GetMissonInfo(*res->mutable_missions());

	//重用现有模块 填充商店信息
	ASSERT(m_shopStrategy);
	e_gsc_errorcode r = m_shopStrategy->GetShopInfo(c_gs_shop_info_req::ALL, user_shop::ESHOPTYPE_MIN, db_req, *res->mutable_shops());
	res->set_error_code(r);
	ASSERT(r == e_gsc_errorcode_success);

	//竞技场信息
	bool ret = m_arenaStrategy->GetArenaInfo(*res);
	ASSERT(ret);
	
	//签到信息
	ret = m_usersignstrategy->GetSignInfo(*res->mutable_sign());
	ASSERT(ret);
	
	//新手引导信息
	res->mutable_guide_ids()->CopyFrom(*m_db_user_info->mutable_guide()->mutable_guide_ids());

	//好友信息
	m_userFriendStrategy->GetFriendInfo(res->mutable_friend_info());

	m_userCrusadeStrategy->GetCrusadeInfo(*res);
	return r == e_gsc_errorcode_success;
}

bool CUser::StartFight( UINT32 iSubTaskID )
{
	//清理
	m_start_sub_task_res.Clear();
	m_start_sub_task_res.set_error_code(e_gsc_errorcode_min);

	if (!CanStartFight(iSubTaskID))
	{
		m_start_sub_task_res.set_error_code(e_gsc_errorcode_cannot_start_sub_task);
		return false;
	}

	//算消耗
	stSubTask* pSubTask = CTaskMgr::GetInstance()->GetSubTaskBySubTaskID(iSubTaskID);
	CHECK_NULL_RETURN_FALSE(pSubTask);

	std::vector<stGameElement> bursts;
	bool r = CBurstMgr::GetInstance()->GetBurst(pSubTask->iBurstPacketID, bursts);
	ASSERT(r);
	if (!r)
	{
		m_start_sub_task_res.set_error_code(e_gsc_errorcode_cannot_start_sub_task);
		return false;
	}

	if (pSubTask->iExtraPacketID && IsConditionsSatisfy(pSubTask->ExtraPacketCondition, UserID())){
		r = CBurstMgr::GetInstance()->GetBurst(pSubTask->iExtraPacketID, bursts);
		ASSERT(r);
		if (!r){
			m_start_sub_task_res.set_error_code(e_gsc_errorcode_cannot_start_sub_task);
			return false;
		}
	}

	bool isFull2NotFull = false;
	// 玩家 gs内存扣体力
	//r = DecreaseHealth(pSubTask->iSpendHealth, &isFull2NotFull);
	//先扣进场体力！ 胜利了再扣除剩下的！
	//UINT32 spend_health = SUBTASK_START_SPEND_HEALTH;
	r = DecreaseHealth(SUBTASK_START_SPEND_HEALTH, &isFull2NotFull);
	ASSERT(r);//CanStartFight中做过判定

	if (!r)
	{
		m_start_sub_task_res.set_error_code(e_gsc_errorcode_cannot_start_sub_task);
		return false;
	}

	m_start_sub_task_res.set_error_code(e_gsc_errorcode_success);
	m_start_sub_task_res.set_sub_task_id(iSubTaskID);
	m_start_sub_task_res.set_enter_time((UINT32)Now());

	//m_start_sub_task_res.set_spend_health(pSubTask->iSpendHealth);
	struct_game_element* ele = m_start_sub_task_res.add_consume();
	ele->set_type(struct_game_element_EELEMENTTYPE_EELEMENTTYPE_HEALTH);
	ele->set_id(0);
	if (isFull2NotFull)
	{
		ele->set_sub_id(_get_health_last_update_time());//假如客户端本地体力满（自动回复最大值）的 在扣去服务器发的体力 不满之后 就使用这里发的最后更新时间为倒计时起点
	}
	else{
		ele->set_sub_id(0);
	}
	ele->set_num_type(struct_game_element::DECREASE);
	ele->set_num(SUBTASK_START_SPEND_HEALTH);


	//m_start_sub_task_res.set_health_last_update_timestamp(_get_health_last_update_time());//移到 体力 element的sub_id上。 假如客户端本地体力满（自动回复最大值）的 在扣去服务器发的体力 不满之后 就使用这里发的最后更新时间为倒计时起点

	//db更新玩家基本信息
	gs_dbs_user_info_op_req* db_req = new gs_dbs_user_info_op_req;
	db_req->set_user_id(UserID());
		//修改db基本信息
	gs_dbs_user_info_op_req_struct_op_base_info* base_info = db_req->mutable_base_info();
	base_info->set_op_type(gs_dbs_user_info_op_req::Update);
	base_info->set_health_cur(_get_health());
	base_info->set_health_last_update_timestamp(_get_health_last_update_time());
		//修改c 和db的道具信息
	for (std::vector<stGameElement>::iterator it = bursts.begin() ; it != bursts.end() ; ++it)
	{
		struct_game_element* ele = m_start_sub_task_res.add_output();
		ele->set_type((struct_game_element_EELEMENTTYPE)it->eType);
		ele->set_id(it->iID);
		ele->set_sub_id(it->iSubID);
		ele->set_num_type(struct_game_element::INCREASE);
		ele->set_num(it->iNum);

		//！道具不在这里加 而是在end的时候加！
		//struct_game_element* db_ele = db_req->add_items();
		//db_ele->CopyFrom(*ele);
	}

	bool ret = CClientForDBS::GetInstance()->SendAsync(CClientForDBSAsync::EASyncOP_User_Info_OP_Req, UserID(), db_req);
	if (!ret)
	{
		delete db_req;
	}

	return true;
}



bool CUser::UpdateGameElement(const struct_game_element& ele, gs_dbs_user_info_op_req& db_req, ::google::protobuf::RepeatedPtrField<struct_game_element>& c_rep_game_ele, GS_C_NTF& gs_c_ntf, ::google::protobuf::RepeatedPtrField<user_hero>* c_heros/* = NULL*/)
{
	//检查db请求中是否已添加，如果已添加直接报错
	::google::protobuf::RepeatedPtrField< ::struct_game_element >::iterator iter = db_req.mutable_items()->begin();
	for (; iter != db_req.mutable_items()->end(); ++iter)
	{
		if (&*iter == &ele)
		{
			ASSERT(false && "gs_dbs_user_info_op_req 结构中已添加相同struct_game_element");
		}
	}

	switch (ele.type())
	{
		case struct_game_element::EELEMENTTYPE_ITEM :
			{
				switch(ele.num_type())
				{
					case struct_game_element::TOTAL:
					{
						_update_element_item_total(ele, &db_req, &c_rep_game_ele);
					}
					break;
					case struct_game_element::INCREASE:
					{
						_update_element_item_increase(ele, &db_req, &c_rep_game_ele);
					}
					break;
					case struct_game_element::DECREASE:
					{
						ASSERT(_update_element_item_decrease(ele, &db_req, &c_rep_game_ele) && "UpdateGameElement decrease 不存在或者数量不足");
					}
					break;
					default:
						ASSERT(false);
						break;
				}
			}
			break;
		case struct_game_element::EELEMENTTYPE_EXP :
			{
				switch(ele.num_type())
				{
				case struct_game_element::INCREASE:
					{
						if (ele.id() == 1)//玩家经验
						{
							IncreaseExp(ele.num(), db_req, c_rep_game_ele, gs_c_ntf);
						}
						else if (ele.id() == 2)//英雄经验
						{
							//ASSERT(false && "暂时不支持英雄经验");
							//队伍信息 gs_c_start_sub_task_req的时候进行更新 并且在给予英雄经验的时候 也是据此 假如EELEMENTTYPE_HERO_EXP上面没有指定hero_id就是根据出阵的英雄队伍的英雄来！
							
							if (ele.sub_id() != 0)//此时是给特定英雄的
							{
								UINT32 hero_id = ele.sub_id();
								UINT32 add_exp = ele.num();
								m_heroStrategy->IncreaseExp(hero_id, add_exp, GetLevel(), *(db_req.mutable_heros_base()), c_rep_game_ele);
							}
							else{//=0 不是给特定英雄 则是给出战英雄的
								std::set<UINT32> heros;
								m_hero_team.GetCurTeamHero(heros);
								ASSERT(!heros.empty());
								if (!heros.empty())
								{
									for (std::set<UINT32>::iterator it = heros.begin(); it != heros.end(); ++it)
									{
										UINT32 hero_id = *it;
										UINT32 add_exp = ele.num();
										m_heroStrategy->IncreaseExp(hero_id, add_exp, GetLevel(), *(db_req.mutable_heros_base()), c_rep_game_ele);
									}
								}
							}
						}
						else{
							ASSERT(false && "不支持的经验类型");
						}
					}
					break;
				//case struct_game_element::DECREASE:
				//	{
				//		_set_exp(_get_exp() - ele.num());
				//		db_req.mutable_base_info()->set_op_type(gs_dbs_user_info_op_req::Update);
				//		db_req.mutable_base_info()->set_exp(_get_exp());
				//		struct_game_element* pGameEle = c_rep_game_ele.Add();
				//		pGameEle->CopyFrom(ele);
				//	}
				//	break;
				default:
					ASSERT(false);
					break;
				}
			}
			break;
		case struct_game_element::EELEMENTTYPE_GOLD:
			{
				switch(ele.num_type())
				{
				case struct_game_element::INCREASE:
					{
						_set_gold(ele.num() + _get_gold());
						db_req.mutable_base_info()->set_op_type(gs_dbs_user_info_op_req::Update);
						db_req.mutable_base_info()->set_gold(_get_gold());
						struct_game_element* pGameEle = c_rep_game_ele.Add();
						pGameEle->CopyFrom(ele);
					}
					break;
				case struct_game_element::DECREASE:
					{
						_set_gold(_get_gold() - ele.num());
						db_req.mutable_base_info()->set_op_type(gs_dbs_user_info_op_req::Update);
						db_req.mutable_base_info()->set_gold(_get_gold());
						struct_game_element* pGameEle = c_rep_game_ele.Add();
						pGameEle->CopyFrom(ele);
					}
					break;
				default:
					ASSERT(false);
					break;
				}
			}
			break;
		case struct_game_element::EELEMENTTYPE_DIAMOND:
			{
				switch(ele.num_type())
				{
				case struct_game_element::INCREASE:
					{
						_set_diamond(ele.num() + _get_diamond());
						db_req.mutable_base_info()->set_op_type(gs_dbs_user_info_op_req::Update);
						db_req.mutable_base_info()->set_diamond(_get_diamond());
						struct_game_element* pGameEle = c_rep_game_ele.Add();
						pGameEle->CopyFrom(ele);
					}
					break;
				case struct_game_element::DECREASE:
					{
						_set_diamond(_get_diamond() - ele.num());
						db_req.mutable_base_info()->set_op_type(gs_dbs_user_info_op_req::Update);
						db_req.mutable_base_info()->set_diamond(_get_diamond());
						struct_game_element* pGameEle = c_rep_game_ele.Add();
						pGameEle->CopyFrom(ele);
					}break;
				default:
					ASSERT(false);
					break;
				}
			}break;
		case struct_game_element::EELEMENTTYPE_HEALTH:
			{
				switch (ele.num_type())
				{
					case struct_game_element::INCREASE:
						{
							IncreaseHealth(ele.num());
							db_req.mutable_base_info()->set_op_type(gs_dbs_user_info_op_req::Update);
							db_req.mutable_base_info()->set_health_cur(GetHealth());
							struct_game_element* pGameEle = c_rep_game_ele.Add();
							pGameEle->set_num(GetHealth());
							pGameEle->CopyFrom(ele);
						}
						break;
					case struct_game_element::DECREASE:
						{
							bool isFull2NotFull;
							DecreaseHealth(ele.num(), &isFull2NotFull);
							db_req.mutable_base_info()->set_op_type(gs_dbs_user_info_op_req::Update);
							db_req.mutable_base_info()->set_health_cur(GetHealth());
							struct_game_element* pGameEle = c_rep_game_ele.Add();
							pGameEle->set_num(GetHealth());
							pGameEle->CopyFrom(ele);
						}
						break;
					default:
						ASSERT(false);
						break;
				}
			}
			break;
		case struct_game_element::EELEMENTTYPE_HERO:
			{
				ASSERT(ele.num_type() == struct_game_element::INCREASE);
				if( GetHero(ele.id()) ){//英雄已经存在

					CHeroCfg* hero_cfg = CHeroMgr::GetInstance()->GetHeroByHeroID(ele.id());
					ASSERT(hero_cfg);

					//获取转化的碎片数！
					stHeroStarCfg* need = CHeroMgr::GetInstance()->GetStarNeed(hero_cfg->iStar);
					ASSERT(need);

					CItemCfg* item_cfg = CItemMgr::GetInstance()->GetFragmentItemByHeroID(ele.id());
					ASSERT(item_cfg);
					
					struct_game_element fragment;
					//todo 物品ID
					fragment.set_type(struct_game_element::EELEMENTTYPE_ITEM);
					fragment.set_id(item_cfg->m_iID);
					fragment.set_sub_id(1);
					fragment.set_num_type(struct_game_element::INCREASE);
					fragment.set_num(need->iToFragmentNum);

					bool r = UpdateGameElement(fragment, db_req, c_rep_game_ele, gs_c_ntf, c_heros);
					ASSERT(r);
				}
				else{//不存在这个英雄 则新创建！
					gs_dbs_user_info_op_req_struct_new_hero* hero_op = db_req.add_new_heros();
					//hero_op->set_op_type(gs_dbs_user_info_op_req::Add);

					//填充db的信息
					db_user_hero* db_hero = hero_op->mutable_hero();

					//填充c的信息
					ASSERT(c_heros);
					user_hero*  c_hero = c_heros->Add();

					bool r = CHeroMgr::GetInstance()->NewHero(ele.id(), db_hero, c_hero);
					ASSERT(r);

					//修改gs上玩家信息
					r = AddNewHero(*db_hero, gs_c_ntf, db_req);
					ASSERT(r);
				}
			}
			break;
		case struct_game_element::EELEMENTTYPE_ARENA_POINT://竞技场积分
			{
				//必要要有竞技场信息 而竞技场信息是在startchallenge的时候新建的！
				ASSERT(m_arenaStrategy);
				if(!m_arenaStrategy->HasArenaData()){
					printf("！！！ user[%u] 没有竞技场信息 但是要领竞技场积分 这里不给！ \n\n");
					break;
				}

				switch(ele.num_type())
				{
				case struct_game_element::INCREASE:
					{
						ArenaAddScore(ele.num());

						db_req.mutable_op_arena()->set_op_type(gs_dbs_user_info_op_req::Update);
						db_req.mutable_op_arena()->set_score(ArenaGetScore());

						struct_game_element* pGameEle = c_rep_game_ele.Add();
						pGameEle->CopyFrom(ele);
					}
					break;
				case struct_game_element::DECREASE:
					{
						bool r = ArenaDecreaseScore(ele.num());
						if (!r){
							ASSERT(false);
							return false;
						}

						db_req.mutable_op_arena()->set_op_type(gs_dbs_user_info_op_req::Update);
						db_req.mutable_op_arena()->set_score(ArenaGetScore());

						struct_game_element* pGameEle = c_rep_game_ele.Add();
						pGameEle->CopyFrom(ele);
					}break;
				default:
					ASSERT(false);
					break;
				}
			}
			break;
		case struct_game_element::EELEMENTTYPE_FRIEND_POINT:
			{
				switch(ele.num_type())
				{
				case struct_game_element::INCREASE:
					{
						_set_friend_point(ele.num() + _get_friend_point());
						db_req.mutable_base_info()->set_op_type(gs_dbs_user_info_op_req::Update);
						db_req.mutable_base_info()->set_friend_point(_get_friend_point());
						struct_game_element* pGameEle = c_rep_game_ele.Add();
						pGameEle->CopyFrom(ele);
					}
					break;
				case struct_game_element::DECREASE:
					{
						_set_friend_point(_get_friend_point() - ele.num());
						db_req.mutable_base_info()->set_op_type(gs_dbs_user_info_op_req::Update);
						db_req.mutable_base_info()->set_friend_point(_get_friend_point());
						struct_game_element* pGameEle = c_rep_game_ele.Add();
						pGameEle->CopyFrom(ele);
					}
					break;
				default:
					ASSERT(false);
					break;
				}
			}
			break;
		case struct_game_element::EELEMENTTYPE_CRUSADE_POINT:
			{
				switch(ele.num_type())
				{
				case struct_game_element::INCREASE:
					{
						_set_crusade_point(ele.num() + _get_crusade_point());
						db_req.mutable_base_info()->set_op_type(gs_dbs_user_info_op_req::Update);
						db_req.mutable_base_info()->set_crusade_point(_get_crusade_point());
						struct_game_element* pGameEle = c_rep_game_ele.Add();
						pGameEle->CopyFrom(ele);
					}
					break;
				case struct_game_element::DECREASE:
					{
						_set_crusade_point(_get_crusade_point() - ele.num());
						db_req.mutable_base_info()->set_op_type(gs_dbs_user_info_op_req::Update);
						db_req.mutable_base_info()->set_crusade_point(_get_crusade_point());
						struct_game_element* pGameEle = c_rep_game_ele.Add();
						pGameEle->CopyFrom(ele);
					}
					break;
				default:
					ASSERT(false);
					break;
				}
			}
			break;
		default:
			ASSERT(false && "不支持该类型添加，如果需要添加");
			break;
	}
	return true;
}

//bool CUser::_update_game_element(std::vector<stGameElement>& eles)
//{
//	for (std::vector<stGameElement>::iterator it = eles.begin() ; it != eles.end(); ++it)
//	{
//		//stGameElement& ele = *it;
//		struct_game_element ele = *it;
//		switch (ele.type())
//		{
//		case struct_game_element::EELEMENTTYPE_ITEM :
//			{
//				switch(ele.num_type())
//				{
//				case struct_game_element::TOTAL:
//					{
//						_update_element_item_total(&ele, NULL, NULL, NULL);
//					}
//					break;
//				case struct_game_element::INCREASE:
//					{
//						_update_element_item_increase(&ele, NULL, NULL, NULL);
//					}
//					break;
//				case struct_game_element::DECREASE:
//					{
//						_update_element_item_decrease(&ele, NULL, NULL);
//					}
//					break;
//				default:
//					ASSERT(false);
//					break;
//				}
//			}
//			break;
//		case struct_game_element::EELEMENTTYPE_EXP :
//			{
//				switch(ele.num_type())
//				{
//				case struct_game_element::INCREASE:
//					{
//						_set_exp(ele.num() + _get_exp());	
//					}
//					break;
//				//case struct_game_element::DECREASE:
//				//	{
//				//		_set_exp(_get_exp() - ele.num());
//				//	}
//				//	break;
//				default:
//					ASSERT(false);
//					break;
//				}
//			}
//			break;
//		case struct_game_element::EELEMENTTYPE_GOLD:
//			{
//				switch(ele.num_type())
//				{
//				case struct_game_element::INCREASE:
//					{
//						_set_gold(ele.num() + _get_gold());
//					}
//					break;
//				case struct_game_element::DECREASE:
//					{
//						_set_gold(_get_gold() - ele.num());
//					}
//					break;
//				default:
//					ASSERT(false);
//					break;
//				}
//			}
//			break;
//		case struct_game_element::EELEMENTTYPE_DIAMOND:
//			{
//				switch(ele.num_type())
//				{
//				case struct_game_element::INCREASE:
//					{
//						_set_diamond(ele.num() + _get_diamond());
//					}
//					break;
//				case struct_game_element::DECREASE:
//					{
//						_set_diamond(_get_diamond() - ele.num());
//					}break;
//				default:
//					ASSERT(false);
//					break;
//				}
//			}break;
//		case struct_game_element::EELEMENTTYPE_HERO:
//			{
//				ASSERT(ele.num_type() == struct_game_element::INCREASE);
//				if( GetHero(ele.id()) ){//英雄已经存在
//
//					CHeroCfg* hero_cfg = CHeroMgr::GetInstance()->GetHeroByHeroID(ele.id());
//					ASSERT(hero_cfg);
//
//					//获取转化的碎片数！
//					stHeroStarCfg* need = CHeroMgr::GetInstance()->GetStarNeed(hero_cfg->iStar);
//					ASSERT(need);
//
//					CItemCfg* item_cfg = CItemMgr::GetInstance()->GetFragmentItemByHeroID(ele.id());
//					ASSERT(item_cfg);
//
//					//struct_game_element fragment;
//					////todo 物品ID
//					//fragment.set_type(struct_game_element::EELEMENTTYPE_ITEM);
//					//fragment.set_id(item_cfg->m_iID);
//					//fragment.set_sub_id(1);
//					//fragment.set_num_type(struct_game_element::INCREASE);
//					//fragment.set_num(need->iToFragmentNum);
//
//					std::vector<stGameElement> new_eles;
//					new_eles.push_back(stGameElement(EELEMENTTYPE_ITEM, item_cfg->m_iID, 0, stGameElement::INCREASE, need->iToFragmentNum));
//
//					bool r = _update_game_element(new_eles);
//					ASSERT(r);
//				}
//				else{//不存在这个英雄 则新创建！
//					//填充db的信息
//					db_user_hero new_hero;
//					bool r = CHeroMgr::GetInstance()->NewHero(ele.id(), &new_hero, NULL);
//					ASSERT(r);
//					//修改gs上玩家信息
//					r = AddNewHero(new_hero);
//					ASSERT(r);
//				}
//			}
//			break;
//		default:
//			ASSERT(false && "不支持该类型添加，如果需要添加");
//			break;
//		}
//	}
//	
//	return true;
//}

bool CUser::UpdateGameElement(const ::google::protobuf::RepeatedPtrField<struct_game_element>& rep_ele, gs_dbs_user_info_op_req& db_req, ::google::protobuf::RepeatedPtrField<struct_game_element>& c_rep_game_ele, GS_C_NTF& gs_c_ntf, ::google::protobuf::RepeatedPtrField<user_hero>* c_heros/* = NULL*/)
{
	::google::protobuf::RepeatedPtrField<struct_game_element>::const_iterator iter = rep_ele.begin();
	for (; iter != rep_ele.end(); ++iter)
	{
		bool r = UpdateGameElement(*iter, db_req, c_rep_game_ele, gs_c_ntf, c_heros);
		ASSERT(r);
	}
	return true;
}

bool CUser::UpdateGameElement(const stGameElement& ele, gs_dbs_user_info_op_req& db_req, ::google::protobuf::RepeatedPtrField<struct_game_element>& c_rep_game_ele, GS_C_NTF& gs_c_ntf, ::google::protobuf::RepeatedPtrField<user_hero>* c_heros/* = NULL*/)
{
	struct_game_element gameEle;
	bool r = ele.Fill(gameEle);
	ASSERT(r);
	if (!r){
		return false;
	}

	r = UpdateGameElement(gameEle, db_req, c_rep_game_ele, gs_c_ntf, c_heros);
	ASSERT(r);
	return r;
}

bool CUser::UpdateGameElement(const std::vector<stGameElement*> &eles, gs_dbs_user_info_op_req& db_req, ::google::protobuf::RepeatedPtrField<struct_game_element>& c_rep_game_ele, GS_C_NTF& gs_c_ntf, ::google::protobuf::RepeatedPtrField<user_hero>* c_heros/* = NULL*/)
{
	for (size_t i = 0; i < eles.size(); ++i)
	{
		bool r = UpdateGameElement(*eles[i], db_req, c_rep_game_ele, gs_c_ntf, c_heros);
		ASSERT(r);
		if (!r){
			return false;
		}
	}
	return true;
}

bool CUser::UpdateGameElement(const std::vector<stGameElement> &eles, gs_dbs_user_info_op_req& db_req, ::google::protobuf::RepeatedPtrField<struct_game_element>& c_rep_game_ele, GS_C_NTF& gs_c_ntf, ::google::protobuf::RepeatedPtrField<user_hero>* c_heros/* = NULL*/)
{
	for (size_t i = 0; i < eles.size(); ++i)
	{
		bool r = UpdateGameElement(eles[i], db_req, c_rep_game_ele, gs_c_ntf, c_heros);
		ASSERT(r);
	}
	return true;
}

UINT32 CUser::GetMaxHealth()
{
	return CUserMgr::GetInstance()->GetMaxHealth(_get_level());
}

UINT32 CUser::GetHealth()
{
	//todo 根据时间戳 更新当前体力！
	UINT32 now = (UINT32)(Now());
	UINT32 last_time = _get_health_last_update_time();
	UINT32 seconds = diffTime(now, last_time);
	UINT32 last_health = _get_health();
	UINT32 max_health = GetMaxHealth();//第一重限制
	UINT32 max_health_pro = CUserMgr::GetMaxHealthPro();//第二重限制
	if (seconds >= HEALTH_RECOVER_SECONDS && max_health > last_health)//没有突破上限 并且可回复时
	{
		UINT32 add_health = seconds/HEALTH_RECOVER_SECONDS;
		//UINT32 left = seconds - add_health*HEALTH_RECOVER_SECONDS;

		int should_add_health = min(add_health, max_health- last_health);//时间恢复的影响 受到第一个上限控制！
		should_add_health = max(0, should_add_health);
		UINT32 cur_health = last_health + should_add_health;

		_set_health_last_update_time(last_time + add_health*HEALTH_RECOVER_SECONDS);
		_set_health(cur_health);

		return cur_health;
	}
	else{
		//ASSERT(last_health <= max_health_pro);
		//_set_health(max_health_pro);//只修正gs 不改db也没事 总有改的时候 如果一直不减体力 下次再来也会修复
		return last_health;
	}
}

UINT32 CUser::IncreaseHealth( UINT32 iHealth )
{
	UINT32 max_health_pro = CUserMgr::GetMaxHealthPro();
	UINT32 last_health = _get_health();
	if ( last_health >= max_health_pro){//超过购买上限
		return last_health;
	}

	UINT32 now = (UINT32)(Now());
	UINT32 seconds = diffTime(now, _get_health_last_update_time());
	
	UINT32 last_time = _get_health_last_update_time();
	UINT32 max_health = GetMaxHealth();//第一重限制
	UINT32 cur_health = 0;
	if(seconds >= HEALTH_RECOVER_SECONDS && max_health > last_health)//没有突破上限 并且可回复时
	{
		UINT32 add_health = seconds/HEALTH_RECOVER_SECONDS;
		//UINT32 left = seconds - add_health*HEALTH_RECOVER_SECONDS;

		int should_add_health = min(add_health, max_health- last_health);//时间恢复的影响 受到第一个上限控制！
		should_add_health = max(0, should_add_health);
		cur_health = last_health + should_add_health;

		_set_health_last_update_time(last_time + add_health*HEALTH_RECOVER_SECONDS);
		//_set_health(cur_health);

		//再计算外部恢复的影响 受到第二个上限的控制
		//cur_health = min(cur_health + iHealth, max_health_pro);
		if ( cur_health < max_health_pro){//不超过购买上限
			cur_health = cur_health + iHealth;//允许超过第二重上限
		}
	}
	else{
		//cur_health = min(last_health + iHealth, max_health_pro);
		cur_health = last_health + iHealth;//last_health 此时《= 第二重上限， 之和允许超过第二重上限
		//不需要修改更新时间
	}

	_set_health(cur_health);
	return cur_health;
}

bool CUser::DecreaseHealth( UINT32 iHealth, bool* isFull2NotFull)
{
	UINT32 now = (UINT32)(Now());
	UINT32 seconds = diffTime(now, _get_health_last_update_time());
	UINT32 last_health = _get_health();
	UINT32 last_time = _get_health_last_update_time();
	UINT32 max_health = GetMaxHealth();//第一重限制
	UINT32 max_health_pro = CUserMgr::GetMaxHealthPro();

	if(seconds >= HEALTH_RECOVER_SECONDS && max_health > last_health)//没有突破上限 并且可回复时
	{

		UINT32 add_health = seconds/HEALTH_RECOVER_SECONDS;
		//UINT32 left = seconds - add_health*HEALTH_RECOVER_SECONDS;

		int should_add_health = min(add_health, max_health- last_health);//时间恢复的影响 受到第一个上限控制！
		should_add_health = max(0, should_add_health);
		UINT32 cur_health = last_health + should_add_health;

		_set_health_last_update_time(last_time + add_health*HEALTH_RECOVER_SECONDS);
		_set_health(cur_health);

		//再处理扣体力
		if (cur_health >= iHealth)
		{
			//假如从自动回复max到小于自动回复max 则客户端开启倒计时！
			//往后 有修改体力的操作 ，都加一个这样的字段：
			//只要是 从满体力值（自动回复可达的最大值）--> 到非满体力 这种情况时
			//你就用这个时间戳用来启动倒计时

			//其他情况 包括拉取玩家初始信息时 如果体力是满的 这个字段就没用
			//你不用更改定时器
			if (cur_health >= max_health && (cur_health - iHealth) < max_health)
			{
				_set_health_last_update_time(now);
				*isFull2NotFull = true;
			}

			cur_health -= iHealth;
			_set_health(cur_health);
			return true;
		}
		else
		{
			return false;
		}
	}
	else{
		UINT32 cur_health = last_health;
		//再处理扣体力
		if (cur_health >= iHealth)
		{
			//假如从自动回复max到小于自动回复max 则客户端开启倒计时！并且时间要给db 如果不给 db一直是古老的数据 再上线会加很多
			if (cur_health >= max_health && (cur_health - iHealth) < max_health)
			{
				_set_health_last_update_time(now);
				*isFull2NotFull = true;
			}

			cur_health -= iHealth;
			_set_health(cur_health);
			return true;
		}
		else
		{
			return false;
		}
	}
}

bool CUser::IsHealthFull()
{
	UINT32 iHealthLimit = 0;
	ASSERT(CSysSettingMgr::GetInstance()->GetIntValue("Health_Limit", iHealthLimit));
	UINT32 iHealth = GetHealth();
	return iHealth >= iHealthLimit;
}

UINT32 CUser::GetMaxSkillPoint(UINT32 iVipLevel)
{
	//return CUserMgr::GetInstance()->GetMaxSkillPoint(iVipLevel);
	return CVipMgr::GetInstance()->GetMaxSkillPoint(iVipLevel);
}

UINT32 CUser::GetSkillPoint()
{
	//todo 根据时间戳 更新当前体力！
	UINT32 now = (UINT32)(Now());
	UINT32 last_value = _get_skill_point();
	UINT32 last_time = _get_skill_point_last_update_time();
	UINT32 seconds = diffTime(now, last_time);
	
	UINT32 max_value = GetMaxSkillPoint(GetVipLevel());//限制
	if (seconds >= SKILLPOINT_RECOVER_SECONDS && max_value > last_value)
	{
		UINT32 add_value = seconds/SKILLPOINT_RECOVER_SECONDS;

		int should_add_value = min(add_value, max_value- last_value);//时间恢复的影响 受到上限控制！
		should_add_value = max(0, should_add_value);
		UINT32 cur_value = last_value + should_add_value;

		_set_skill_point_last_update_time(last_time + add_value*SKILLPOINT_RECOVER_SECONDS);
		_set_skill_point(cur_value);

		return cur_value;
	}
	else{
		return last_value;
	}
}

UINT32 CUser::IncreaseSkillPoint( UINT32 iSkillPoint )
{
	UINT32 last_value = _get_skill_point();
	UINT32 max_value = GetMaxSkillPoint(GetVipLevel());//限制
	if (last_value >= max_value)
	{
		return last_value;
	}

	UINT32 now = (UINT32)(Now());
	UINT32 seconds = diffTime(now, _get_skill_point_last_update_time());
	UINT32 last_time = _get_skill_point_last_update_time();
	if(seconds >= SKILLPOINT_RECOVER_SECONDS  /*&& max_value > last_value 前面已作判断 除非也像体力有两种上限*/ ){

		UINT32 add_value = seconds/SKILLPOINT_RECOVER_SECONDS;

		int should_add_value = min(add_value, max_value- last_value);//时间恢复的影响 受到上限控制！
		should_add_value = max(0, should_add_value);
		UINT32 cur_value = last_value + should_add_value;

		_set_skill_point_last_update_time(last_time + add_value*SKILLPOINT_RECOVER_SECONDS);
		_set_skill_point(cur_value);

		//再计算外部恢复的影响
		//cur_value = min(cur_value + iSkillPoint, max_value);
		cur_value = cur_value + iSkillPoint;
		_set_skill_point(cur_value);

		return cur_value;
	}
	else{
		//UINT32 cur_value = min(last_value + iSkillPoint, max_value);
		UINT32 cur_value = last_value + iSkillPoint;

		//不需要修改更新时间
		_set_skill_point(cur_value);
		return cur_value;
	}
}

bool CUser::DecreaseSkillPoint( UINT32 iSkillPoint, bool* isFull2NotFull)
{
	UINT32 now = (UINT32)(Now());
	UINT32 seconds = diffTime(now, _get_skill_point_last_update_time());
	UINT32 last_value = _get_skill_point();
	UINT32 last_time = _get_skill_point_last_update_time();
	UINT32 max_value = GetMaxSkillPoint(GetVipLevel());//限制
	
	if(seconds >= SKILLPOINT_RECOVER_SECONDS  && max_value > last_value){

		UINT32 add_value = seconds/SKILLPOINT_RECOVER_SECONDS;
		
		int should_add_value = min(add_value, max_value- last_value);//时间恢复的影响 受到第一个上限控制！
		should_add_value = max(0, should_add_value);
		UINT32 cur_value = last_value + should_add_value;

		_set_skill_point_last_update_time(last_time + add_value*SKILLPOINT_RECOVER_SECONDS);
		_set_skill_point(cur_value);

		//再处理扣值
		if (cur_value >= iSkillPoint)
		{
			//假如从自动回复max到小于自动回复max 则客户端开启倒计时！
			//往后 有修改的操作 ，都加一个这样的字段：
			//只要是 从满值（自动回复可达的最大值）--> 到非满值 这种情况时
			//你就用这个时间戳用来启动倒计时

			//其他情况 包括拉取玩家初始信息时 如果值是满的 这个字段就没用
			//不用更改定时器
			if (cur_value >= max_value && (cur_value - iSkillPoint) < max_value)
			{
				_set_skill_point_last_update_time(now);
				*isFull2NotFull = true;
			}

			cur_value -= iSkillPoint;
			_set_skill_point(cur_value);
			return true;
		}
		else
		{
			return false;
		}
	}
	else{
		UINT32 cur_value = last_value;
		//再处理扣值
		if (cur_value >= iSkillPoint)
		{
			//假如从自动回复max到小于自动回复max 则客户端开启倒计时！
			if (cur_value >= max_value && (cur_value - iSkillPoint) < max_value)
			{
				_set_skill_point_last_update_time(now);
				*isFull2NotFull = true;
			}

			cur_value -= iSkillPoint;
			_set_skill_point(cur_value);
			return true;
		}
		else
		{
			return false;
		}
	}
}

bool CUser::isFirstInvokeGoldN()
{
	ASSERT(m_db_user_info && m_db_user_info->has_user_base_info());
	return m_db_user_info->mutable_user_base_info()->invoke_gold_n_total_num() == 0;
}

bool CUser::isFirstInvokeDiamondN()
{
	ASSERT(m_db_user_info && m_db_user_info->has_user_base_info());
	return m_db_user_info->mutable_user_base_info()->invoke_diamond_n_total_num() == 0;
}

UINT32 CUser::GetInvokeGoldFreeLeftNum()
{
	//todo 根据时间戳 更新当前体力！
	UINT32 now = (UINT32)(Now());
	UINT32 last_time = _get_invoke_gold_free_update_timestamp();

	//。一日可以免费抽取5次（00:00刷新次数）。
	if (!isSameDay(now, last_time, CSysSettingMgr::GetInstance()->Global_Reset_Time))
	{
		UINT32 max_value = 0;
		bool r = CSysSettingMgr::GetInstance()->GetIntValue("Invoke_Gold_Free_Per_Day", max_value);
		ASSERT(r);

		//_set_invoke_gold_free_update_timestamp(now);//可以设置为0或者保留这个时间 在使用的时候可以判断是否在cd
		_set_invoke_gold_free_left_num(max_value);
	}

	return _get_invoke_gold_free_left_num();
}

bool CUser::DecreaseInvokeGoldFreeLeftNum( UINT32 iPoint )
{
	UINT32 now = (UINT32)(Now());
	UINT32 last_time = _get_invoke_gold_free_update_timestamp();//上一次的使用时间
	
	//。一日可以免费抽取5次（00:00刷新次数）。
	if (!isSameDay(now, last_time, CSysSettingMgr::GetInstance()->Global_Reset_Time)){
		UINT32 max_value = 0;
		bool r = CSysSettingMgr::GetInstance()->GetIntValue("Invoke_Gold_Free_Per_Day", max_value);
		ASSERT(r);
		//_set_invoke_gold_free_update_timestamp(now);//可以设置为0或者保留这个时间 在使用的时候可以判断是否在cd
		_set_invoke_gold_free_left_num(max_value);
	}

	UINT32 cur_value = _get_invoke_gold_free_left_num();
	if(cur_value < iPoint){// 没有剩余可使用次数 或者不在cd时间内
		return false;
	}

	//点数够
	UINT32 seconds = diffTime(now, last_time);
	UINT32 recover_time = -1;
	bool r = CSysSettingMgr::GetInstance()->GetIntValue("Invoke_Gold_Free_CD", recover_time);
	ASSERT(r);

	if(seconds >= recover_time){//意义已经变了 不是回复 而是计算是否允许减少 是否在cd时间！
		//再处理扣值
		cur_value -= iPoint;
		_set_invoke_gold_free_left_num(cur_value);
		_set_invoke_gold_free_update_timestamp(now);
		return true;
	}
	else{
		return false;
	}
}

UINT32 CUser::GetInvokeDiamondFreeLeftNum()
{
	//todo 根据时间戳 更新当前体力！
	UINT32 now = (UINT32)(Now());
	UINT32 last_time = _get_invoke_diamond_free_update_timestamp();
	UINT32 last_value = _get_invoke_diamond_free_left_num();
	UINT32 seconds = diffTime(now, last_time);

	UINT32 max_value = 0;
	bool r = CSysSettingMgr::GetInstance()->GetIntValue("Invoke_Diamond_Free_Max_num", max_value);
	ASSERT(r);

	UINT32 recover_time = -1;
	r = CSysSettingMgr::GetInstance()->GetIntValue("Invoke_Diamond_Free_CD", recover_time);
	ASSERT(r);

	if (seconds >= recover_time)
	{
		UINT32 add_value = seconds/recover_time;

		int should_add_value = min(add_value, max_value- last_value);//时间恢复的影响 受到上限控制！
		should_add_value = max(0, should_add_value);
		UINT32 cur_value = last_value + should_add_value;

		_set_invoke_diamond_free_update_timestamp(last_time + add_value*recover_time);
		_set_invoke_diamond_free_left_num(cur_value);

		return cur_value;
	}
	else{
		return last_value;
	}
}

bool CUser::DecreaseInvokeDiamondFreeLeftNum( UINT32 iPoint, bool* isFull2NotFull )
{
	UINT32 now = (UINT32)(Now());
	UINT32 last_time = _get_invoke_diamond_free_update_timestamp();
	UINT32 last_value = _get_invoke_diamond_free_left_num();
	UINT32 seconds = diffTime(now, last_time);
	UINT32 max_value = 0;
	bool r = CSysSettingMgr::GetInstance()->GetIntValue("Invoke_Diamond_Free_Max_num", max_value);
	ASSERT(r);

	UINT32 recover_time = -1;
	r = CSysSettingMgr::GetInstance()->GetIntValue("Invoke_Diamond_Free_CD", recover_time);
	ASSERT(r);
	if(seconds >= recover_time){

		UINT32 add_value = seconds/recover_time;

		int should_add_value = min(add_value, max_value- last_value);//时间恢复的影响 受到第一个上限控制！
		should_add_value = max(0, should_add_value);
		UINT32 cur_value = last_value + should_add_value;

		_set_invoke_diamond_free_update_timestamp(last_time + add_value*recover_time);
		_set_invoke_diamond_free_left_num(cur_value);

		//再处理扣值
		if (cur_value >= iPoint)
		{
			//假如从自动回复max到小于自动回复max 则客户端开启倒计时！
			//往后 有修改的操作 ，都加一个这样的字段：
			//只要是 从满值（自动回复可达的最大值）--> 到非满值 这种情况时
			//你就用这个时间戳用来启动倒计时

			//其他情况 包括拉取玩家初始信息时 如果值是满的 这个字段就没用
			//不用更改定时器
			if (cur_value >= max_value && (cur_value - iPoint) < max_value)
			{
				_set_invoke_diamond_free_update_timestamp(now);//这个是重置时间 不是操作时间 所以不要更新
				*isFull2NotFull = true;
			}

			cur_value -= iPoint;
			_set_invoke_diamond_free_left_num(cur_value);
			return true;
		}
		else
		{
			return false;
		}
	}
	else{
		UINT32 cur_value = last_value;
		//再处理扣值
		if (cur_value >= iPoint)
		{
			//假如从自动回复max到小于自动回复max 则客户端开启倒计时！
			if (cur_value >= max_value && (cur_value - iPoint) < max_value)
			{
				_set_invoke_diamond_free_update_timestamp(now);//这个是重置时间 不是操作时间 所以不要更新
				*isFull2NotFull = true;
			}

			cur_value -= iPoint;
			_set_invoke_diamond_free_left_num(cur_value);
			return true;
		}
		else
		{
			return false;
		}
	}
}

bool CUser::IsConsumeEnough(EELEMENTTYPE eType, UINT32 iID, UINT32 iNum){
	switch (eType)
	{
	case EELEMENTTYPE_ITEM:
		if (!IsItemEnough(iID, iNum))
		{
			return false;
		}
		break;
	case EELEMENTTYPE_GOLD:
		if (GetGold() < iNum)
		{
			return false;
		}
		break;
	case EELEMENTTYPE_DIAMOND:
		if (GetDiamond() < iNum)
		{
			return false;
		}
		break;
	case EELEMENTTYPE_ARENA_POINT:
		if(ArenaGetScore() < iNum){
			return false;
		}
		break;
	case EELEMENTTYPE_FRIEND_POINT:
		if(GetFriendPoint() < iNum){
			return false;
		}
		break;
		//todo add other type
	default:
		ASSERT(false && "需要扩展CUser::IsConsumeEnough类型判断！");
	}
	
	return true;
}


bool CUser::IsConsumeEnough( std::vector<stGameElement>& vecEles )
{
	std::vector<stGameElement>::iterator it = vecEles.begin();
	for (; it != vecEles.end(); ++it)
	{
		if(!IsConsumeEnough(it->eType, it->iID, it->iNum)){
			return false;
		}
	}
	return true;
}

bool CUser::IsConsumeEnough( std::vector<stGameElement*>& vecEles )
{
	std::vector<stGameElement*>::iterator it = vecEles.begin();
	for (; it != vecEles.end(); ++it)
	{
		stGameElement* ele = *it;
		ASSERT(ele);
		CHECK_NULL_RETURN_FALSE(ele);
		if(!IsConsumeEnough(ele->eType, ele->iID, ele->iNum)){
			return false;
		}
	}
	return true;
}

bool CUser::AddNewHero( const db_user_hero& hero , GS_C_NTF& gs_c_ntf, gs_dbs_user_info_op_req& db_req)
{
	ASSERT(m_heroStrategy);

	//复制一个出来！
	db_user_hero* new_hero = m_db_user_info->mutable_heros()->add_items();
	new_hero->CopyFrom(hero);

	bool r = m_heroStrategy->Add(new_hero);
	ASSERT(r);
	//假如添加失败了 怎么处理？
	if (!r)
	{
		//去掉这个数据
		m_db_user_info->mutable_heros()->mutable_items()->RemoveLast();
		return false;
	}
	TriggerMission(EEventType_Hero_Collect, EEventSubType_None, db_req, gs_c_ntf);
	return true;
}

void CUser::_update_element_item_total(const struct_game_element& ele, gs_dbs_user_info_op_req* db_req, ::google::protobuf::RepeatedPtrField<struct_game_element>* p_c_rep_game_ele)
{
	ASSERT(db_req && p_c_rep_game_ele);
	UINT32 iAddNum  = 0;
	m_pItemBag->SetItem(ele, db_req->mutable_items(), p_c_rep_game_ele, iAddNum);
	if(m_pItemBag->CheckNeedSendMail(ele.id()))
	{
		if (iAddNum < ele.num())
		{//邮件发送
			struct_game_element gameEle;
			gameEle.CopyFrom(ele);
			gameEle.set_num(ele.num() - iAddNum);
			gameEle.set_num_type(struct_game_element::INCREASE);
			_add_item_to_mail(gameEle, db_req, p_c_rep_game_ele);
		}
	}
}

void CUser::_update_element_item_increase(const struct_game_element& ele, gs_dbs_user_info_op_req* pDBReq, ::google::protobuf::RepeatedPtrField<struct_game_element>* p_c_rep_game_ele)
{	
	ASSERT(pDBReq && p_c_rep_game_ele);
	UINT32 iAddNum = 0;
	m_pItemBag->AddItem(ele, pDBReq->mutable_items(), p_c_rep_game_ele, iAddNum);
	if(m_pItemBag->CheckNeedSendMail(ele.id()))
	{
		if (iAddNum < ele.num())
		{//邮件发送
			struct_game_element gameEle;
			gameEle.CopyFrom(ele);
			gameEle.set_num(ele.num() - iAddNum);

			_add_item_to_mail(gameEle, pDBReq, p_c_rep_game_ele);
		}
	}
}

bool CUser::_update_element_item_decrease(const struct_game_element& ele, gs_dbs_user_info_op_req* pDBReq, ::google::protobuf::RepeatedPtrField<struct_game_element>* p_c_rep_game_ele)
{
	return m_pItemBag->DelItem(ele, pDBReq->mutable_items(), p_c_rep_game_ele);
}

UINT32 CUser::GetInvokeGoldFreeUpdateTime()
{
	return _get_invoke_gold_free_update_timestamp();
}

UINT32 CUser::GetInvokeDiamondFreeUpdateTime()
{
	return _get_invoke_diamond_free_update_timestamp();
}


void CUser::_add_item_to_mail(struct_game_element& gameEle, gs_dbs_user_info_op_req* db_req, ::google::protobuf::RepeatedPtrField<struct_game_element>* p_c_rep_game_ele)
{
	ASSERT(db_req && p_c_rep_game_ele);
	db_user_mail* pDBMail = NULL;
	bool bExist = false;
	if (db_req->has_new_mail())
	{
		bExist =  m_userMailStrategy->GetExistMailWithAttach(db_req->mutable_new_mail(), pDBMail);
	}

	if (bExist)
	{
		ASSERT(pDBMail);
		m_userMailStrategy->AddAttachTo(gameEle, pDBMail);
	}
	else
	{
		stGameElement stgameEle;
		stgameEle.CopyFrom(gameEle);
		CreateMail(MAIL_TYPE_OVERLAP_ITEMS, *db_req, NULL, NULL, &stgameEle);
	}
}

//UINT32 CUser::IncreaseExp( const UINT32 add_exp, UINT32& up_level_num, UINT32& new_cur_exp )
//{
//	//是否升级
//	UINT32 cur_level = GetLevel();
//	UINT32 cur_exp = GetExp();
//	UINT32 actual_add_exp = CUserMgr::GetInstance()->IncreaseExp(cur_level, cur_exp, add_exp, up_level_num, new_cur_exp);
//	if (actual_add_exp)//一定有某些改变的
//	{
//		if (up_level_num)
//		{
//			_set_level(cur_level + up_level_num);
//
//			if (cur_exp != new_cur_exp)//等级变了 可能当前经验值不变 gs内部可以不修改 但是外部还是要告诉客户端这个操做 并且元素的sub_id附上增量
//			{
//				_set_exp(new_cur_exp);
//			}
//		}
//		else{//等级不变的话 那经验值一定变了
//			ASSERT(cur_exp != new_cur_exp);
//			_set_exp(new_cur_exp);
//		}
//	}
//	return actual_add_exp;
//}

UINT32 CUser::IncreaseExp( const UINT32 add_exp, gs_dbs_user_info_op_req& db_req, ::google::protobuf::RepeatedPtrField<struct_game_element>& c_rep_game_ele, GS_C_NTF& gs_c_ntf)
{
	ASSERT(add_exp);/* && db_req && c_rep_game_ele*/
	UINT32 up_level_num = 0;
	UINT32 new_cur_exp = 0;
	UINT32 last_exp = GetExp();
	UINT32 last_level = GetLevel();
	UINT32 actual_add_exp = CUserMgr::GetInstance()->IncreaseExp(GetLevel(), last_exp, add_exp, up_level_num, new_cur_exp);
	if (actual_add_exp)
	{
		if (up_level_num)
		{
			_set_level(last_level + up_level_num);

			UINT32 health_prize = CUserMgr::GetInstance()->GetHealthPrizeOfLevelUp(last_level, last_level + up_level_num);
			UINT32 cur_health = IncreaseHealth(health_prize);

			db_req.mutable_base_info()->set_op_type(gs_dbs_user_info_op_req::Update);
			db_req.mutable_base_info()->set_level(last_level + up_level_num);
			db_req.mutable_base_info()->set_health_cur(cur_health);

			struct_game_element* c_ele = c_rep_game_ele.Add();
			c_ele->set_type(struct_game_element::EELEMENTTYPE_HEALTH);
			c_ele->set_id(0);
			c_ele->set_sub_id(0);
			c_ele->set_num_type(struct_game_element::TOTAL);
			c_ele->set_num(cur_health);

			c_ele = c_rep_game_ele.Add();
			c_ele->set_type(struct_game_element::EELEMENTTYPE_PLAYER_LEVEL);
			c_ele->set_id(0);
			c_ele->set_sub_id(0);
			c_ele->set_num_type(struct_game_element::TOTAL);
			c_ele->set_num(GetLevel());

			if (last_exp != new_cur_exp)//等级变了 可能当前经验值不变 就不需要更新
			{
				_set_exp(new_cur_exp);
				db_req.mutable_base_info()->set_exp(new_cur_exp);
			}

			//等级变了 但当前经验值不变 也要告诉客户端 因为需要total元素的sub_id来给出经验增量！
			c_ele = c_rep_game_ele.Add();
			c_ele->set_type(struct_game_element::EELEMENTTYPE_PLAYER_EXP);
			c_ele->set_id(0);
			c_ele->set_sub_id(actual_add_exp);
			c_ele->set_num_type(struct_game_element::TOTAL);
			c_ele->set_num(new_cur_exp);

			//触发任务
			TriggerMission(EEventType_Level_Up, EEventSubType_Player_Level, db_req, gs_c_ntf, up_level_num);
			TriggerUserStatistics(EEventType_Level_Up, db_req, 0);
			TriggerGuidMail(last_level, GetLevel(), db_req);
		}
		else{//等级不变的话 那经验值一定变了
			ASSERT(last_exp != new_cur_exp);

			_set_exp(new_cur_exp);

			db_req.mutable_base_info()->set_op_type(gs_dbs_user_info_op_req::Update);
			db_req.mutable_base_info()->set_exp(new_cur_exp);

			struct_game_element* c_ele = c_rep_game_ele.Add();
			c_ele->set_type(struct_game_element::EELEMENTTYPE_PLAYER_EXP);
			c_ele->set_id(0);
			c_ele->set_sub_id(actual_add_exp);
			c_ele->set_num_type(struct_game_element::TOTAL);
			c_ele->set_num(new_cur_exp);
		}
	}

	return actual_add_exp;
}

void CUser::DelOutOfDateMail( gs_dbs_user_info_op_req* db_req, GS_C_NTF& ntf)
{
	std::vector<struct_game_element> vecEle;
	m_userMailStrategy->DelOutOfDateMail(db_req->mutable_op_mails(), ntf.mutable_mail_ntf_arr(), vecEle);
	//将附件信息加给玩家
	for (size_t i = 0; i < vecEle.size(); ++i)
	{
		switch (vecEle[i].type())
		{
		case struct_game_element::EELEMENTTYPE_ITEM:
			{
				UINT32 iActualAddNum = 0;
				m_pItemBag->AddItem(vecEle[i], db_req->mutable_items(), ntf.mutable_mail_ntf_arr()->mutable_eles(), iActualAddNum);
			}
			break;
		case struct_game_element::EELEMENTTYPE_EXP:
		case struct_game_element::EELEMENTTYPE_GOLD:
		case struct_game_element::EELEMENTTYPE_DIAMOND:
			UpdateGameElement(vecEle[i], *db_req, *ntf.mutable_mail_ntf_arr()->mutable_eles(), ntf);
			break;
		default:
			ASSERT(false && "不支持类型");
			break;
		}
		
	}
}

void CUser::SetHeadOrHeadFrame(HEAD_CHANGE_TYPE change_type, UINT32 iTargetID, gs_dbs_user_info_op_req& db_req, gs_c_head_or_frame_change_res& res)
{
	if(change_type == HEAD)
	{
		//检测是否有该英雄id
		if(!GetHero(iTargetID))
		{
			res.set_error_code(e_gsc_errorcode_unknown_error);
			return;
		}
		db_req.mutable_base_info()->set_head_id(iTargetID);
	}
	else if(change_type == HEAD_FRAME)
	{
		//检测是否有该头像框即道具
		if(!GetItem(iTargetID))
		{
			res.set_error_code(e_gsc_errorcode_unknown_error);
			return;
		}
		db_req.mutable_base_info()->set_head_frame_id(iTargetID);
	}
	db_req.mutable_base_info()->set_op_type(gs_dbs_user_info_op_req::Update);
	res.set_error_code(e_gsc_errorcode_success);
	res.set_change_type(change_type);
	res.set_target_id(iTargetID);
	return;
}

void CUser::EditPlayerInfo(const c_gs_edit_player_info_req& req, gs_dbs_user_info_op_req& db_req, gs_dbs_edit_name_req& name_req, gs_c_edit_player_info_res& res){
	
	if (req.has_name())
	{
		//检测名字不为空
		if(req.name().empty())
		{
			res.set_error_code(e_gsc_errorcode_unknown_error);
			return;
		}

		//检测名字长度
		if(req.name().length() > MAXLEN_USERNAME){
			res.set_error_code(e_gsc_errorcode_edit_player_info_name_too_long);
			return;
		}
		//是否有敏感词汇
		bool r = CForbidWordMgr::GetInstance()->IsForbidWord(req.name());
		if (r)
		{
			res.set_error_code(e_gsc_errorcode_edit_player_info_name_forbid_word);
			return;
		}
	}
	
	if (req.has_head_id())
	{
		//检测是否有该英雄id
		if(!GetHero(req.head_id()))
		{
			res.set_error_code(e_gsc_errorcode_unknown_error);
			return;
		}
	}

	if (req.has_head_frame_id())
	{
		//检测是否有该头像框即道具
		if(!GetItem(req.head_frame_id()))
		{
			res.set_error_code(e_gsc_errorcode_unknown_error);
			return;
		}
		
	}
	
	if (req.has_signature())
	{
		//检测名字不为空
		if(req.signature().empty())
		{
			res.set_error_code(e_gsc_errorcode_unknown_error);
			return;
		}
		
		UINT32 Max_Length_Signature = 0;
		bool r = CSysSettingMgr::GetInstance()->GetIntValue("Max_Length_Signature", Max_Length_Signature);
		ASSERT(Max_Length_Signature && r);
		//检测名字长度
		if(req.signature().length() > Max_Length_Signature){
			res.set_error_code(e_gsc_errorcode_signature_too_long);
			return;
		}
		//是否有敏感词汇
		r = CForbidWordMgr::GetInstance()->IsForbidWord(req.signature());
		if (r)
		{
			res.set_error_code(e_gsc_errorcode_signature_has_forbid_word);
			return;
		}
	}

	if (req.has_name())
	{
		name_req.set_user_id(UserID());
		name_req.set_name(req.name());
	}
	
	if (req.has_signature())
	{
		db_req.mutable_base_info()->set_op_type(gs_dbs_user_info_op_req::Update);
		db_req.mutable_base_info()->set_signature(req.signature());
		res.set_signature(req.signature());
	}

	if (req.has_head_id())
	{
		db_req.mutable_base_info()->set_op_type(gs_dbs_user_info_op_req::Update);
		db_req.mutable_base_info()->set_head_id(req.head_id());
		res.set_head_id(req.head_id());
	}

	
	if (req.has_head_frame_id())
	{
		db_req.mutable_base_info()->set_op_type(gs_dbs_user_info_op_req::Update);
		db_req.mutable_base_info()->set_head_frame_id(req.head_frame_id());
		res.set_head_frame_id(req.head_frame_id());
	}
	
	res.set_error_code(e_gsc_errorcode_success);
}
e_gsc_errorcode CUser::ResetSubTaskEnterNum( UINT32 cfg_buy_num_instance, UINT32 sub_task_id, gs_c_buy_res &c_res, gs_dbs_user_info_op_req& db_req, UINT32& rCostDiamond )
{
	UINT32 now = Now();

	db_user_sub_task* pSubTask = GetSubTask(sub_task_id);
	//ASSERT(pSubTask);
	if (!pSubTask){
		return e_gsc_errorcode_unknown_error;
	}

	stSubTask* pSubTaskCfg = CTaskMgr::GetInstance()->GetSubTaskBySubTaskID(sub_task_id);
	if (!pSubTaskCfg){
		return e_gsc_errorcode_unknown_error;
	}

	CTask* pTaskCfg = CTaskMgr::GetInstance()->GetTaskBySubTaskID(sub_task_id);
	if (!pTaskCfg){
		return e_gsc_errorcode_unknown_error;
	}

	if (pTaskCfg->eType != CTask::ETASKTYPE_BRANCH){
		return e_gsc_errorcode_sub_task_not_branch;
	}

	/*
	if (pSubTaskCfg->iTotalEnterNumPerDay == 0){
		return e_gsc_errorcode_sub_task_no_enter_num_limit;
	}

	UINT32 enter_num = 0;
	if (isSameDay(now, pSubTask->last_enter_timestamp(), CSysSettingMgr::GetInstance()->Global_Reset_Time) ){
		enter_num = pSubTask->last_day_enter_num();
	}
	else{
		enter_num = 0;
	}

	ASSERT(enter_num <= pSubTaskCfg->iTotalEnterNumPerDay);//
	if (enter_num < cfg_buy_num_instance){
		return e_gsc_errorcode_sub_task_has_left_enter_num;
	}
	*/

	UINT32 left_enter_num = GetSubTaskLeftEnterNum(sub_task_id);
	if (left_enter_num > 0){
		return e_gsc_errorcode_sub_task_has_left_enter_num;
	}
	
	UINT32 reseted_num = 0;
	
	if (isSameDay(now, pSubTask->last_reset_time(), CSysSettingMgr::GetInstance()->Global_Reset_Time) ){
		reseted_num = pSubTask->last_day_reset_num();
	}
	else{
		reseted_num = 0;
	}

	//判断
	//ASSERT(reseted_num <= cfg_buy_num_instance);
	if(cfg_buy_num_instance <= reseted_num ){
		return e_gsc_errorcode_not_enough_times;
	}

	UINT32 iDiamond = GetDiamond();
	UINT32 iCostDiamond = CVipMgr::GetInstance()->GetCostDiamond(EBUY_BRANCHTASK_RESET, reseted_num + 1);
	if(iDiamond < iCostDiamond){
		return e_gsc_errorcode_not_enough_diamond;
	}
	rCostDiamond = iCostDiamond;
	//gs更新钻石 db和c_res在此函数调用方做了
	bool r  = DecreaseDiamond(iCostDiamond);
	ASSERT(r);
	if (!r){
		return e_gsc_errorcode_not_enough_diamond;
	}

	//gs更新进入次数和刷新次数
	gs_dbs_user_info_op_req::struct_op_sub_task* db_sub_task = db_req.mutable_sub_task();
	if (isSameDay(now, pSubTask->last_reset_time(), CSysSettingMgr::GetInstance()->Global_Reset_Time) ){
		pSubTask->set_last_day_reset_num(pSubTask->last_day_reset_num() + 1);
	}
	else{
		pSubTask->set_last_day_reset_num(1);
	}
	pSubTask->set_last_reset_time(now);
	pSubTask->set_last_day_enter_num(0);
	//pSubTask->set_last_enter_timestamp(0);//这个是不需要设置的

	//db全量更新 进入次数和刷新次数
	db_sub_task->set_op_type(gs_dbs_user_info_op_req::Add_Or_Update);
	db_sub_task->set_sub_task_id(pSubTask->sub_task_id());
	db_sub_task->set_star(pSubTask->star());
	db_sub_task->set_last_enter_timestamp(pSubTask->last_enter_timestamp());
	db_sub_task->set_last_day_enter_num(pSubTask->last_day_enter_num());
	db_sub_task->set_total_enter_num(pSubTask->total_enter_num());
	db_sub_task->set_last_reset_time(pSubTask->last_reset_time());
	db_sub_task->set_last_day_reset_num(pSubTask->last_day_reset_num());

	//c res
	c_res.set_buy_type(struct_game_element::EELEMENTTYPE_SUB_TASK_ENTER_NUM);

	struct_game_element* c_consume = c_res.add_consume();
	c_consume->set_type(struct_game_element::EELEMENTTYPE_SUB_TASK_RESET_NUM);
	c_consume->set_id(0);
	c_consume->set_sub_id(pSubTask->sub_task_id());
	c_consume->set_num_type(struct_game_element::TOTAL);
	c_consume->set_num(pSubTask->last_day_reset_num());//

	struct_game_element* c_output = c_res.mutable_output();
	c_output->set_type(struct_game_element::EELEMENTTYPE_SUB_TASK_ENTER_NUM);
	c_output->set_id(0);
	c_output->set_sub_id(pSubTask->sub_task_id());
	c_output->set_num_type(struct_game_element::TOTAL);
	c_output->set_num(pSubTask->last_day_enter_num());//实际为0了

	return e_gsc_errorcode_success;
}

void CUser::TriggerUserStatistics( EEventType eEventType, gs_dbs_user_info_op_req& db_req, UINT32 iCurID)
{
	gs_dbs_user_info_op_req::struct_new_user_statis* user_statis = db_req.mutable_new_user_statis(); 

	user_statis->set_level(GetLevel());
	user_statis->set_vip(GetVipLevel());
	user_statis->set_occur_time(Now());
	user_statis->set_event_type(eEventType);
	user_statis->set_cur_id(iCurID);
}

void CUser::SearchUser( c_gs_search_user_req& req, gs_c_search_user_res& res )
{
	if (!req.has_user_id() && !req.has_name())
	{
		res.set_error_code(e_gsc_errorcode_unknown_error);
		return;
	}
	CUser* pUser = NULL;
	if (req.has_user_id())
	{
		pUser = CUserMgr::GetInstance()->GetUserByUserID(req.user_id());
	}
	else{
		if (req.has_name())
		{
			pUser = CUserMgr::GetInstance()->GetUserByName(req.name());
		}
	}

	if (pUser)
	{
		res.set_error_code(e_gsc_errorcode_success);

		res.mutable_base_info()->set_user_id(pUser->UserID());
		res.mutable_base_info()->set_head_id(pUser->GetHeadID());		
		res.mutable_base_info()->set_head_frame_id(pUser->GetHeadFrameID());	
		res.mutable_base_info()->set_name(pUser->Name());		
		res.mutable_base_info()->set_level(pUser->GetLevel());		
		res.mutable_base_info()->set_vip(pUser->GetVipLevel());
		res.mutable_base_info()->set_signature(pUser->GetSignature());
		CArenaContestant* pArena = CArena::GetInstance()->GetArenaContestantByUserID(pUser->UserID());
		if(pArena){
			res.mutable_base_info()->set_arena_rank(pArena->m_rank);
		}else{
			res.mutable_base_info()->set_arena_rank(0);
		}

		res.mutable_base_info()->set_last_login_time(0);

		SearchUserRet(*res.mutable_base_info());
		return;
	}
}

friend_base_info CUser::GetBaseInfo()
{
	friend_base_info info;
	info.set_user_id(UserID());
	info.set_head_id(GetHeadID());		
	info.set_head_frame_id(GetHeadFrameID());	
	info.set_name(Name());		
	info.set_level(GetLevel());		
	info.set_vip(GetVipLevel());
	info.set_signature(GetSignature());
	CArenaContestant* pArena = CArena::GetInstance()->GetArenaContestantByUserID(UserID());
	if(pArena){
		info.set_arena_rank(pArena->m_rank);
	}else{
		info.set_arena_rank(0);
	}

	info.set_last_login_time(0);
	return info;
}
//bool CUser::GiveInitPlayerBelongings()
//{
//	bool r = _update_game_element(CUserMgr::GetInstance()->GetInitPlayerBelongings());
//	ASSERT(r);
//	return r;
//}


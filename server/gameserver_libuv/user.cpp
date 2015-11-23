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
	printf("!!!! user[%u] ����[%p]׼����  !!!\n", UserID(), connect);
	if(m_connect != NULL){
		printf("!!!! user[%u] �Ѿ�������[%p]�� ���ڰ�[%p] !!!\n", UserID(), m_connect, connect);
	}
	
	CHECK_NULL_RETURN_FALSE(connect);
	m_connect = connect;
	////connect->data = this;//��ֱ�Ӵ��ָ�� ���Ǵ��user_ID

	//stConnectUserData* pUserData = (stConnectUserData*)connect->data;
	//ASSERT(pUserData);
	//pUserData->iUserID = UserID();

	return true;
}

bool CUser::UnIntertwineConnect( uv_stream_t* connect )
{
	printf("!!!! user[%u] ��ǰ����[%p] ��Ҫ���[%p]  !!!\n", UserID(), m_connect, connect);
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
	//δ�����ľ������и���
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
	//����������߾͸����ַ���־
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

//���һ��ʱ���� ����Ӧʱ���ã� ����һ��ֻ��������ʱ����
bool CUser::OnLeave()
{
	printf("CUser::OnLeave ���[%u]�����������Ͷ������� \n", UserID());
	//δ�����ľ������и���
	
	ArenaLeave();

	//����user_mgr��map
	bool r = CUserMgr::GetInstance()->RemoveUser(UserID());
	ASSERT(r);
	//ɾ���Լ�
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

	//�����һ�����Ϣ
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
	

	//������ �ٸ��ͻ���
	//base_user_info->set_health_cur(m_db_user_info->mutable_user_base_info()->health_cur());        
	//base_user_info->set_health_last_update_timestamp(m_db_user_info->mutable_user_base_info()->health_last_update_timestamp());
	base_user_info->set_health_cur(GetHealth());//������ʱ��
	base_user_info->set_health_last_update_timestamp(_get_health_last_update_time());

	base_user_info->set_skill_point(GetSkillPoint());//���¼����˼��ܵ�͸���ʱ��
	base_user_info->set_skill_point_last_update_timestamp(_get_skill_point_last_update_time());

	base_user_info->set_invoke_gold_free_left_num(GetInvokeGoldFreeLeftNum());	//���㵱ǰ��ʣ�¶��ٴ�
	base_user_info->set_invoke_gold_free_update_timestamp(_get_invoke_gold_free_update_timestamp());		//����cdˢ�µ�ʱ�� �� ��ʼ����ʱ����������������ʱ��
	base_user_info->set_invoke_diamond_free_left_num(GetInvokeDiamondFreeLeftNum());	//���㵱ǰ��ʣ�¶��ٴ�
	base_user_info->set_invoke_diamond_free_update_timestamp(_get_invoke_diamond_free_update_timestamp());	//����cdˢ�µ�ʱ�� �� ��ʼ����ʱ����������������ʱ��
	base_user_info->set_head_id(m_db_user_info->mutable_user_base_info()->head_id());
	base_user_info->set_head_frame_id(m_db_user_info->mutable_user_base_info()->head_frame_id());
	base_user_info->set_friend_point(m_db_user_info->mutable_user_base_info()->friend_point());
	base_user_info->set_signature(m_db_user_info->mutable_user_base_info()->signature());
	base_user_info->set_fight_power(m_db_user_info->mutable_user_base_info()->fight_power());
	base_user_info->set_crusade_point(m_db_user_info->mutable_user_base_info()->crusade_point());
	//�����������Ϣ
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
	
	//�ʼ�
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
	
	//���Ӣ����Ϣ
	db_user_hero_array* heros = m_db_user_info->mutable_heros();
	for (::google::protobuf::RepeatedPtrField<db_user_hero>::iterator it = heros->mutable_items()->begin(); it != heros->mutable_items()->end() ; ++it){
		user_hero* pHero = res->mutable_heros()->add_items();

		pHero->set_hero_id(it->hero_id());
		pHero->set_star(it->star());
		pHero->set_quality(it->quality());
		pHero->set_level(it->level());
		pHero->set_cur_exp(it->cur_exp());
		pHero->set_got_timestamp(it->got_timestamp());
	
		//����װ����Ϣ
		for (::google::protobuf::RepeatedPtrField<db_user_hero_equip>::iterator it_in = it->mutable_equips()->begin(); it_in != it->mutable_equips()->end() ; ++it_in){
			user_hero_equip* pEquip = pHero->add_equips();
			pEquip->set_equip_id(it_in->equip_id());
			pEquip->set_level(it_in->level());
		}
		
		//���ü�����Ϣ
		for (::google::protobuf::RepeatedPtrField<db_user_hero_skill>::iterator it_in = it->mutable_skills()->begin(); it_in != it->mutable_skills()->end() ; ++it_in){
			user_hero_skill* pSkill = pHero->add_skills();
			pSkill->set_type((user_hero_skill::e_skill_type)it_in->type());
			pSkill->set_level(it_in->level());
		}
	}
	
	//����
	for (::google::protobuf::RepeatedPtrField< ::struct_game_element >::iterator iterItem = m_db_user_info->mutable_items()->mutable_items()->begin(); iterItem != m_db_user_info->mutable_items()->mutable_items()->end(); ++iterItem){
		struct_game_element* pItem = res->add_item_elements();
		pItem->CopyFrom(*iterItem);
	}
	
	//�̵���Ϣ
	//for( ::google::protobuf::RepeatedPtrField< ::db_user_shop >::const_iterator it = m_db_user_info->shops().items().begin() ; it != m_db_user_info->shops().items().end() ; ++it){
	//	user_shop* pShop = res->add_shops();
	//	ASSERT(pShop);

	//	//pShop->CopyFrom(*it);//����һ������ ����copy��

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

	//������Ϣ
	ASSERT(m_userMissionStrategy);
	m_userMissionStrategy->GetMissonInfo(*res->mutable_missions());

	//��������ģ�� ����̵���Ϣ
	ASSERT(m_shopStrategy);
	e_gsc_errorcode r = m_shopStrategy->GetShopInfo(c_gs_shop_info_req::ALL, user_shop::ESHOPTYPE_MIN, db_req, *res->mutable_shops());
	res->set_error_code(r);
	ASSERT(r == e_gsc_errorcode_success);

	//��������Ϣ
	bool ret = m_arenaStrategy->GetArenaInfo(*res);
	ASSERT(ret);
	
	//ǩ����Ϣ
	ret = m_usersignstrategy->GetSignInfo(*res->mutable_sign());
	ASSERT(ret);
	
	//����������Ϣ
	res->mutable_guide_ids()->CopyFrom(*m_db_user_info->mutable_guide()->mutable_guide_ids());

	//������Ϣ
	m_userFriendStrategy->GetFriendInfo(res->mutable_friend_info());

	m_userCrusadeStrategy->GetCrusadeInfo(*res);
	return r == e_gsc_errorcode_success;
}

bool CUser::StartFight( UINT32 iSubTaskID )
{
	//����
	m_start_sub_task_res.Clear();
	m_start_sub_task_res.set_error_code(e_gsc_errorcode_min);

	if (!CanStartFight(iSubTaskID))
	{
		m_start_sub_task_res.set_error_code(e_gsc_errorcode_cannot_start_sub_task);
		return false;
	}

	//������
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
	// ��� gs�ڴ������
	//r = DecreaseHealth(pSubTask->iSpendHealth, &isFull2NotFull);
	//�ȿ۽��������� ʤ�����ٿ۳�ʣ�µģ�
	//UINT32 spend_health = SUBTASK_START_SPEND_HEALTH;
	r = DecreaseHealth(SUBTASK_START_SPEND_HEALTH, &isFull2NotFull);
	ASSERT(r);//CanStartFight�������ж�

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
		ele->set_sub_id(_get_health_last_update_time());//����ͻ��˱������������Զ��ظ����ֵ���� �ڿ�ȥ�������������� ����֮�� ��ʹ�����﷢��������ʱ��Ϊ����ʱ���
	}
	else{
		ele->set_sub_id(0);
	}
	ele->set_num_type(struct_game_element::DECREASE);
	ele->set_num(SUBTASK_START_SPEND_HEALTH);


	//m_start_sub_task_res.set_health_last_update_timestamp(_get_health_last_update_time());//�Ƶ� ���� element��sub_id�ϡ� ����ͻ��˱������������Զ��ظ����ֵ���� �ڿ�ȥ�������������� ����֮�� ��ʹ�����﷢��������ʱ��Ϊ����ʱ���

	//db������һ�����Ϣ
	gs_dbs_user_info_op_req* db_req = new gs_dbs_user_info_op_req;
	db_req->set_user_id(UserID());
		//�޸�db������Ϣ
	gs_dbs_user_info_op_req_struct_op_base_info* base_info = db_req->mutable_base_info();
	base_info->set_op_type(gs_dbs_user_info_op_req::Update);
	base_info->set_health_cur(_get_health());
	base_info->set_health_last_update_timestamp(_get_health_last_update_time());
		//�޸�c ��db�ĵ�����Ϣ
	for (std::vector<stGameElement>::iterator it = bursts.begin() ; it != bursts.end() ; ++it)
	{
		struct_game_element* ele = m_start_sub_task_res.add_output();
		ele->set_type((struct_game_element_EELEMENTTYPE)it->eType);
		ele->set_id(it->iID);
		ele->set_sub_id(it->iSubID);
		ele->set_num_type(struct_game_element::INCREASE);
		ele->set_num(it->iNum);

		//�����߲�������� ������end��ʱ��ӣ�
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
	//���db�������Ƿ�����ӣ���������ֱ�ӱ���
	::google::protobuf::RepeatedPtrField< ::struct_game_element >::iterator iter = db_req.mutable_items()->begin();
	for (; iter != db_req.mutable_items()->end(); ++iter)
	{
		if (&*iter == &ele)
		{
			ASSERT(false && "gs_dbs_user_info_op_req �ṹ���������ͬstruct_game_element");
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
						ASSERT(_update_element_item_decrease(ele, &db_req, &c_rep_game_ele) && "UpdateGameElement decrease �����ڻ�����������");
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
						if (ele.id() == 1)//��Ҿ���
						{
							IncreaseExp(ele.num(), db_req, c_rep_game_ele, gs_c_ntf);
						}
						else if (ele.id() == 2)//Ӣ�۾���
						{
							//ASSERT(false && "��ʱ��֧��Ӣ�۾���");
							//������Ϣ gs_c_start_sub_task_req��ʱ����и��� �����ڸ���Ӣ�۾����ʱ�� Ҳ�Ǿݴ� ����EELEMENTTYPE_HERO_EXP����û��ָ��hero_id���Ǹ��ݳ����Ӣ�۶����Ӣ������
							
							if (ele.sub_id() != 0)//��ʱ�Ǹ��ض�Ӣ�۵�
							{
								UINT32 hero_id = ele.sub_id();
								UINT32 add_exp = ele.num();
								m_heroStrategy->IncreaseExp(hero_id, add_exp, GetLevel(), *(db_req.mutable_heros_base()), c_rep_game_ele);
							}
							else{//=0 ���Ǹ��ض�Ӣ�� ���Ǹ���սӢ�۵�
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
							ASSERT(false && "��֧�ֵľ�������");
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
				if( GetHero(ele.id()) ){//Ӣ���Ѿ�����

					CHeroCfg* hero_cfg = CHeroMgr::GetInstance()->GetHeroByHeroID(ele.id());
					ASSERT(hero_cfg);

					//��ȡת������Ƭ����
					stHeroStarCfg* need = CHeroMgr::GetInstance()->GetStarNeed(hero_cfg->iStar);
					ASSERT(need);

					CItemCfg* item_cfg = CItemMgr::GetInstance()->GetFragmentItemByHeroID(ele.id());
					ASSERT(item_cfg);
					
					struct_game_element fragment;
					//todo ��ƷID
					fragment.set_type(struct_game_element::EELEMENTTYPE_ITEM);
					fragment.set_id(item_cfg->m_iID);
					fragment.set_sub_id(1);
					fragment.set_num_type(struct_game_element::INCREASE);
					fragment.set_num(need->iToFragmentNum);

					bool r = UpdateGameElement(fragment, db_req, c_rep_game_ele, gs_c_ntf, c_heros);
					ASSERT(r);
				}
				else{//���������Ӣ�� ���´�����
					gs_dbs_user_info_op_req_struct_new_hero* hero_op = db_req.add_new_heros();
					//hero_op->set_op_type(gs_dbs_user_info_op_req::Add);

					//���db����Ϣ
					db_user_hero* db_hero = hero_op->mutable_hero();

					//���c����Ϣ
					ASSERT(c_heros);
					user_hero*  c_hero = c_heros->Add();

					bool r = CHeroMgr::GetInstance()->NewHero(ele.id(), db_hero, c_hero);
					ASSERT(r);

					//�޸�gs�������Ϣ
					r = AddNewHero(*db_hero, gs_c_ntf, db_req);
					ASSERT(r);
				}
			}
			break;
		case struct_game_element::EELEMENTTYPE_ARENA_POINT://����������
			{
				//��ҪҪ�о�������Ϣ ����������Ϣ����startchallenge��ʱ���½��ģ�
				ASSERT(m_arenaStrategy);
				if(!m_arenaStrategy->HasArenaData()){
					printf("������ user[%u] û�о�������Ϣ ����Ҫ�쾺�������� ���ﲻ���� \n\n");
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
			ASSERT(false && "��֧�ָ�������ӣ������Ҫ���");
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
//				if( GetHero(ele.id()) ){//Ӣ���Ѿ�����
//
//					CHeroCfg* hero_cfg = CHeroMgr::GetInstance()->GetHeroByHeroID(ele.id());
//					ASSERT(hero_cfg);
//
//					//��ȡת������Ƭ����
//					stHeroStarCfg* need = CHeroMgr::GetInstance()->GetStarNeed(hero_cfg->iStar);
//					ASSERT(need);
//
//					CItemCfg* item_cfg = CItemMgr::GetInstance()->GetFragmentItemByHeroID(ele.id());
//					ASSERT(item_cfg);
//
//					//struct_game_element fragment;
//					////todo ��ƷID
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
//				else{//���������Ӣ�� ���´�����
//					//���db����Ϣ
//					db_user_hero new_hero;
//					bool r = CHeroMgr::GetInstance()->NewHero(ele.id(), &new_hero, NULL);
//					ASSERT(r);
//					//�޸�gs�������Ϣ
//					r = AddNewHero(new_hero);
//					ASSERT(r);
//				}
//			}
//			break;
//		default:
//			ASSERT(false && "��֧�ָ�������ӣ������Ҫ���");
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
	//todo ����ʱ��� ���µ�ǰ������
	UINT32 now = (UINT32)(Now());
	UINT32 last_time = _get_health_last_update_time();
	UINT32 seconds = diffTime(now, last_time);
	UINT32 last_health = _get_health();
	UINT32 max_health = GetMaxHealth();//��һ������
	UINT32 max_health_pro = CUserMgr::GetMaxHealthPro();//�ڶ�������
	if (seconds >= HEALTH_RECOVER_SECONDS && max_health > last_health)//û��ͻ������ ���ҿɻظ�ʱ
	{
		UINT32 add_health = seconds/HEALTH_RECOVER_SECONDS;
		//UINT32 left = seconds - add_health*HEALTH_RECOVER_SECONDS;

		int should_add_health = min(add_health, max_health- last_health);//ʱ��ָ���Ӱ�� �ܵ���һ�����޿��ƣ�
		should_add_health = max(0, should_add_health);
		UINT32 cur_health = last_health + should_add_health;

		_set_health_last_update_time(last_time + add_health*HEALTH_RECOVER_SECONDS);
		_set_health(cur_health);

		return cur_health;
	}
	else{
		//ASSERT(last_health <= max_health_pro);
		//_set_health(max_health_pro);//ֻ����gs ����dbҲû�� ���иĵ�ʱ�� ���һֱ�������� �´�����Ҳ���޸�
		return last_health;
	}
}

UINT32 CUser::IncreaseHealth( UINT32 iHealth )
{
	UINT32 max_health_pro = CUserMgr::GetMaxHealthPro();
	UINT32 last_health = _get_health();
	if ( last_health >= max_health_pro){//������������
		return last_health;
	}

	UINT32 now = (UINT32)(Now());
	UINT32 seconds = diffTime(now, _get_health_last_update_time());
	
	UINT32 last_time = _get_health_last_update_time();
	UINT32 max_health = GetMaxHealth();//��һ������
	UINT32 cur_health = 0;
	if(seconds >= HEALTH_RECOVER_SECONDS && max_health > last_health)//û��ͻ������ ���ҿɻظ�ʱ
	{
		UINT32 add_health = seconds/HEALTH_RECOVER_SECONDS;
		//UINT32 left = seconds - add_health*HEALTH_RECOVER_SECONDS;

		int should_add_health = min(add_health, max_health- last_health);//ʱ��ָ���Ӱ�� �ܵ���һ�����޿��ƣ�
		should_add_health = max(0, should_add_health);
		cur_health = last_health + should_add_health;

		_set_health_last_update_time(last_time + add_health*HEALTH_RECOVER_SECONDS);
		//_set_health(cur_health);

		//�ټ����ⲿ�ָ���Ӱ�� �ܵ��ڶ������޵Ŀ���
		//cur_health = min(cur_health + iHealth, max_health_pro);
		if ( cur_health < max_health_pro){//��������������
			cur_health = cur_health + iHealth;//�������ڶ�������
		}
	}
	else{
		//cur_health = min(last_health + iHealth, max_health_pro);
		cur_health = last_health + iHealth;//last_health ��ʱ��= �ڶ������ޣ� ֮���������ڶ�������
		//����Ҫ�޸ĸ���ʱ��
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
	UINT32 max_health = GetMaxHealth();//��һ������
	UINT32 max_health_pro = CUserMgr::GetMaxHealthPro();

	if(seconds >= HEALTH_RECOVER_SECONDS && max_health > last_health)//û��ͻ������ ���ҿɻظ�ʱ
	{

		UINT32 add_health = seconds/HEALTH_RECOVER_SECONDS;
		//UINT32 left = seconds - add_health*HEALTH_RECOVER_SECONDS;

		int should_add_health = min(add_health, max_health- last_health);//ʱ��ָ���Ӱ�� �ܵ���һ�����޿��ƣ�
		should_add_health = max(0, should_add_health);
		UINT32 cur_health = last_health + should_add_health;

		_set_health_last_update_time(last_time + add_health*HEALTH_RECOVER_SECONDS);
		_set_health(cur_health);

		//�ٴ��������
		if (cur_health >= iHealth)
		{
			//������Զ��ظ�max��С���Զ��ظ�max ��ͻ��˿�������ʱ��
			//���� ���޸������Ĳ��� ������һ���������ֶΣ�
			//ֻҪ�� ��������ֵ���Զ��ظ��ɴ�����ֵ��--> ���������� �������ʱ
			//��������ʱ���������������ʱ

			//������� ������ȡ��ҳ�ʼ��Ϣʱ ������������� ����ֶξ�û��
			//�㲻�ø��Ķ�ʱ��
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
		//�ٴ��������
		if (cur_health >= iHealth)
		{
			//������Զ��ظ�max��С���Զ��ظ�max ��ͻ��˿�������ʱ������ʱ��Ҫ��db ������� dbһֱ�ǹ��ϵ����� �����߻�Ӻܶ�
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
	//todo ����ʱ��� ���µ�ǰ������
	UINT32 now = (UINT32)(Now());
	UINT32 last_value = _get_skill_point();
	UINT32 last_time = _get_skill_point_last_update_time();
	UINT32 seconds = diffTime(now, last_time);
	
	UINT32 max_value = GetMaxSkillPoint(GetVipLevel());//����
	if (seconds >= SKILLPOINT_RECOVER_SECONDS && max_value > last_value)
	{
		UINT32 add_value = seconds/SKILLPOINT_RECOVER_SECONDS;

		int should_add_value = min(add_value, max_value- last_value);//ʱ��ָ���Ӱ�� �ܵ����޿��ƣ�
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
	UINT32 max_value = GetMaxSkillPoint(GetVipLevel());//����
	if (last_value >= max_value)
	{
		return last_value;
	}

	UINT32 now = (UINT32)(Now());
	UINT32 seconds = diffTime(now, _get_skill_point_last_update_time());
	UINT32 last_time = _get_skill_point_last_update_time();
	if(seconds >= SKILLPOINT_RECOVER_SECONDS  /*&& max_value > last_value ǰ�������ж� ����Ҳ����������������*/ ){

		UINT32 add_value = seconds/SKILLPOINT_RECOVER_SECONDS;

		int should_add_value = min(add_value, max_value- last_value);//ʱ��ָ���Ӱ�� �ܵ����޿��ƣ�
		should_add_value = max(0, should_add_value);
		UINT32 cur_value = last_value + should_add_value;

		_set_skill_point_last_update_time(last_time + add_value*SKILLPOINT_RECOVER_SECONDS);
		_set_skill_point(cur_value);

		//�ټ����ⲿ�ָ���Ӱ��
		//cur_value = min(cur_value + iSkillPoint, max_value);
		cur_value = cur_value + iSkillPoint;
		_set_skill_point(cur_value);

		return cur_value;
	}
	else{
		//UINT32 cur_value = min(last_value + iSkillPoint, max_value);
		UINT32 cur_value = last_value + iSkillPoint;

		//����Ҫ�޸ĸ���ʱ��
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
	UINT32 max_value = GetMaxSkillPoint(GetVipLevel());//����
	
	if(seconds >= SKILLPOINT_RECOVER_SECONDS  && max_value > last_value){

		UINT32 add_value = seconds/SKILLPOINT_RECOVER_SECONDS;
		
		int should_add_value = min(add_value, max_value- last_value);//ʱ��ָ���Ӱ�� �ܵ���һ�����޿��ƣ�
		should_add_value = max(0, should_add_value);
		UINT32 cur_value = last_value + should_add_value;

		_set_skill_point_last_update_time(last_time + add_value*SKILLPOINT_RECOVER_SECONDS);
		_set_skill_point(cur_value);

		//�ٴ����ֵ
		if (cur_value >= iSkillPoint)
		{
			//������Զ��ظ�max��С���Զ��ظ�max ��ͻ��˿�������ʱ��
			//���� ���޸ĵĲ��� ������һ���������ֶΣ�
			//ֻҪ�� ����ֵ���Զ��ظ��ɴ�����ֵ��--> ������ֵ �������ʱ
			//��������ʱ���������������ʱ

			//������� ������ȡ��ҳ�ʼ��Ϣʱ ���ֵ������ ����ֶξ�û��
			//���ø��Ķ�ʱ��
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
		//�ٴ����ֵ
		if (cur_value >= iSkillPoint)
		{
			//������Զ��ظ�max��С���Զ��ظ�max ��ͻ��˿�������ʱ��
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
	//todo ����ʱ��� ���µ�ǰ������
	UINT32 now = (UINT32)(Now());
	UINT32 last_time = _get_invoke_gold_free_update_timestamp();

	//��һ�տ�����ѳ�ȡ5�Σ�00:00ˢ�´�������
	if (!isSameDay(now, last_time, CSysSettingMgr::GetInstance()->Global_Reset_Time))
	{
		UINT32 max_value = 0;
		bool r = CSysSettingMgr::GetInstance()->GetIntValue("Invoke_Gold_Free_Per_Day", max_value);
		ASSERT(r);

		//_set_invoke_gold_free_update_timestamp(now);//��������Ϊ0���߱������ʱ�� ��ʹ�õ�ʱ������ж��Ƿ���cd
		_set_invoke_gold_free_left_num(max_value);
	}

	return _get_invoke_gold_free_left_num();
}

bool CUser::DecreaseInvokeGoldFreeLeftNum( UINT32 iPoint )
{
	UINT32 now = (UINT32)(Now());
	UINT32 last_time = _get_invoke_gold_free_update_timestamp();//��һ�ε�ʹ��ʱ��
	
	//��һ�տ�����ѳ�ȡ5�Σ�00:00ˢ�´�������
	if (!isSameDay(now, last_time, CSysSettingMgr::GetInstance()->Global_Reset_Time)){
		UINT32 max_value = 0;
		bool r = CSysSettingMgr::GetInstance()->GetIntValue("Invoke_Gold_Free_Per_Day", max_value);
		ASSERT(r);
		//_set_invoke_gold_free_update_timestamp(now);//��������Ϊ0���߱������ʱ�� ��ʹ�õ�ʱ������ж��Ƿ���cd
		_set_invoke_gold_free_left_num(max_value);
	}

	UINT32 cur_value = _get_invoke_gold_free_left_num();
	if(cur_value < iPoint){// û��ʣ���ʹ�ô��� ���߲���cdʱ����
		return false;
	}

	//������
	UINT32 seconds = diffTime(now, last_time);
	UINT32 recover_time = -1;
	bool r = CSysSettingMgr::GetInstance()->GetIntValue("Invoke_Gold_Free_CD", recover_time);
	ASSERT(r);

	if(seconds >= recover_time){//�����Ѿ����� ���ǻظ� ���Ǽ����Ƿ�������� �Ƿ���cdʱ�䣡
		//�ٴ����ֵ
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
	//todo ����ʱ��� ���µ�ǰ������
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

		int should_add_value = min(add_value, max_value- last_value);//ʱ��ָ���Ӱ�� �ܵ����޿��ƣ�
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

		int should_add_value = min(add_value, max_value- last_value);//ʱ��ָ���Ӱ�� �ܵ���һ�����޿��ƣ�
		should_add_value = max(0, should_add_value);
		UINT32 cur_value = last_value + should_add_value;

		_set_invoke_diamond_free_update_timestamp(last_time + add_value*recover_time);
		_set_invoke_diamond_free_left_num(cur_value);

		//�ٴ����ֵ
		if (cur_value >= iPoint)
		{
			//������Զ��ظ�max��С���Զ��ظ�max ��ͻ��˿�������ʱ��
			//���� ���޸ĵĲ��� ������һ���������ֶΣ�
			//ֻҪ�� ����ֵ���Զ��ظ��ɴ�����ֵ��--> ������ֵ �������ʱ
			//��������ʱ���������������ʱ

			//������� ������ȡ��ҳ�ʼ��Ϣʱ ���ֵ������ ����ֶξ�û��
			//���ø��Ķ�ʱ��
			if (cur_value >= max_value && (cur_value - iPoint) < max_value)
			{
				_set_invoke_diamond_free_update_timestamp(now);//���������ʱ�� ���ǲ���ʱ�� ���Բ�Ҫ����
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
		//�ٴ����ֵ
		if (cur_value >= iPoint)
		{
			//������Զ��ظ�max��С���Զ��ظ�max ��ͻ��˿�������ʱ��
			if (cur_value >= max_value && (cur_value - iPoint) < max_value)
			{
				_set_invoke_diamond_free_update_timestamp(now);//���������ʱ�� ���ǲ���ʱ�� ���Բ�Ҫ����
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
		ASSERT(false && "��Ҫ��չCUser::IsConsumeEnough�����жϣ�");
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

	//����һ��������
	db_user_hero* new_hero = m_db_user_info->mutable_heros()->add_items();
	new_hero->CopyFrom(hero);

	bool r = m_heroStrategy->Add(new_hero);
	ASSERT(r);
	//�������ʧ���� ��ô����
	if (!r)
	{
		//ȥ���������
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
		{//�ʼ�����
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
		{//�ʼ�����
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
//	//�Ƿ�����
//	UINT32 cur_level = GetLevel();
//	UINT32 cur_exp = GetExp();
//	UINT32 actual_add_exp = CUserMgr::GetInstance()->IncreaseExp(cur_level, cur_exp, add_exp, up_level_num, new_cur_exp);
//	if (actual_add_exp)//һ����ĳЩ�ı��
//	{
//		if (up_level_num)
//		{
//			_set_level(cur_level + up_level_num);
//
//			if (cur_exp != new_cur_exp)//�ȼ����� ���ܵ�ǰ����ֵ���� gs�ڲ����Բ��޸� �����ⲿ����Ҫ���߿ͻ���������� ����Ԫ�ص�sub_id��������
//			{
//				_set_exp(new_cur_exp);
//			}
//		}
//		else{//�ȼ�����Ļ� �Ǿ���ֵһ������
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

			if (last_exp != new_cur_exp)//�ȼ����� ���ܵ�ǰ����ֵ���� �Ͳ���Ҫ����
			{
				_set_exp(new_cur_exp);
				db_req.mutable_base_info()->set_exp(new_cur_exp);
			}

			//�ȼ����� ����ǰ����ֵ���� ҲҪ���߿ͻ��� ��Ϊ��ҪtotalԪ�ص�sub_id����������������
			c_ele = c_rep_game_ele.Add();
			c_ele->set_type(struct_game_element::EELEMENTTYPE_PLAYER_EXP);
			c_ele->set_id(0);
			c_ele->set_sub_id(actual_add_exp);
			c_ele->set_num_type(struct_game_element::TOTAL);
			c_ele->set_num(new_cur_exp);

			//��������
			TriggerMission(EEventType_Level_Up, EEventSubType_Player_Level, db_req, gs_c_ntf, up_level_num);
			TriggerUserStatistics(EEventType_Level_Up, db_req, 0);
			TriggerGuidMail(last_level, GetLevel(), db_req);
		}
		else{//�ȼ�����Ļ� �Ǿ���ֵһ������
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
	//��������Ϣ�Ӹ����
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
			ASSERT(false && "��֧������");
			break;
		}
		
	}
}

void CUser::SetHeadOrHeadFrame(HEAD_CHANGE_TYPE change_type, UINT32 iTargetID, gs_dbs_user_info_op_req& db_req, gs_c_head_or_frame_change_res& res)
{
	if(change_type == HEAD)
	{
		//����Ƿ��и�Ӣ��id
		if(!GetHero(iTargetID))
		{
			res.set_error_code(e_gsc_errorcode_unknown_error);
			return;
		}
		db_req.mutable_base_info()->set_head_id(iTargetID);
	}
	else if(change_type == HEAD_FRAME)
	{
		//����Ƿ��и�ͷ��򼴵���
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
		//������ֲ�Ϊ��
		if(req.name().empty())
		{
			res.set_error_code(e_gsc_errorcode_unknown_error);
			return;
		}

		//������ֳ���
		if(req.name().length() > MAXLEN_USERNAME){
			res.set_error_code(e_gsc_errorcode_edit_player_info_name_too_long);
			return;
		}
		//�Ƿ������дʻ�
		bool r = CForbidWordMgr::GetInstance()->IsForbidWord(req.name());
		if (r)
		{
			res.set_error_code(e_gsc_errorcode_edit_player_info_name_forbid_word);
			return;
		}
	}
	
	if (req.has_head_id())
	{
		//����Ƿ��и�Ӣ��id
		if(!GetHero(req.head_id()))
		{
			res.set_error_code(e_gsc_errorcode_unknown_error);
			return;
		}
	}

	if (req.has_head_frame_id())
	{
		//����Ƿ��и�ͷ��򼴵���
		if(!GetItem(req.head_frame_id()))
		{
			res.set_error_code(e_gsc_errorcode_unknown_error);
			return;
		}
		
	}
	
	if (req.has_signature())
	{
		//������ֲ�Ϊ��
		if(req.signature().empty())
		{
			res.set_error_code(e_gsc_errorcode_unknown_error);
			return;
		}
		
		UINT32 Max_Length_Signature = 0;
		bool r = CSysSettingMgr::GetInstance()->GetIntValue("Max_Length_Signature", Max_Length_Signature);
		ASSERT(Max_Length_Signature && r);
		//������ֳ���
		if(req.signature().length() > Max_Length_Signature){
			res.set_error_code(e_gsc_errorcode_signature_too_long);
			return;
		}
		//�Ƿ������дʻ�
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

	//�ж�
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
	//gs������ʯ db��c_res�ڴ˺������÷�����
	bool r  = DecreaseDiamond(iCostDiamond);
	ASSERT(r);
	if (!r){
		return e_gsc_errorcode_not_enough_diamond;
	}

	//gs���½��������ˢ�´���
	gs_dbs_user_info_op_req::struct_op_sub_task* db_sub_task = db_req.mutable_sub_task();
	if (isSameDay(now, pSubTask->last_reset_time(), CSysSettingMgr::GetInstance()->Global_Reset_Time) ){
		pSubTask->set_last_day_reset_num(pSubTask->last_day_reset_num() + 1);
	}
	else{
		pSubTask->set_last_day_reset_num(1);
	}
	pSubTask->set_last_reset_time(now);
	pSubTask->set_last_day_enter_num(0);
	//pSubTask->set_last_enter_timestamp(0);//����ǲ���Ҫ���õ�

	//dbȫ������ ���������ˢ�´���
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
	c_output->set_num(pSubTask->last_day_enter_num());//ʵ��Ϊ0��

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


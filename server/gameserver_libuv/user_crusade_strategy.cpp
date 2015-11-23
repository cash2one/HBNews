#include "user_crusade_strategy.h"
#include "user.h"
#include "crusade_mgr.h"
#include "vip_mgr.h"
bool CUserCrusadeStrategy::Init(CUser* pUser, db_user_info& db_info)
{
	CHECK_NULL_RETURN_FALSE(pUser);
	m_pUser = pUser;
	if(db_info.has_user_crusade()){
		m_db_user_crusade = db_info.mutable_user_crusade();
		::google::protobuf::RepeatedPtrField< ::user_crusade_crusade_hero >::iterator me_it = m_db_user_crusade->mutable_me_heros()->begin();
		for (; me_it != m_db_user_crusade->mutable_me_heros()->end(); ++me_it)
		{
			ASSERT(m_map_me_heros.find(me_it->hero_id()) == m_map_me_heros.end());
			m_map_me_heros[me_it->hero_id()] = &(*me_it);
		}
		m_has_crusade = true;
	}else{
		m_db_user_crusade = db_info.mutable_user_crusade();
		m_db_user_crusade->set_wave(1);
	}
	return true;
}


void CUserCrusadeStrategy::GetCrusadeInfo(gs_c_user_info_res& res)
{
	user_crusade* crusade = res.mutable_crusade();
	if (MAX_NUM_CRUSADE_WAVE + 1 == m_db_user_crusade->wave())
	{
		crusade->set_is_all_through(true);
		return;
	}else{
		crusade->set_is_all_through(false);
	}
	
	if(m_db_user_crusade->me_heros_size()){
		crusade->mutable_me_heros()->CopyFrom(m_db_user_crusade->me_heros());
	}
	if (m_db_user_crusade->enemy_heros_size())
	{
		crusade->mutable_enemy_heros()->CopyFrom(m_db_user_crusade->enemy_heros());
	}
	crusade->set_wave(m_db_user_crusade->wave());

	UINT32 iProTimes = m_pUser->GetEventTotalValue(EEventType_Crusade_Promote_Reward, EEventSubType(m_db_user_crusade->wave()));
	if (0 == iProTimes)
	{
		crusade->set_left_promote_award_times(1);
	}else{
		crusade->set_left_promote_award_times(0);
	}
}

void CUserCrusadeStrategy::CrusadeStart( c_gs_crusade_start_req& req, gs_c_crusade_start_res& res, db_user_info& db_info)
{
	if (MAX_NUM_CRUSADE_WAVE + 1 == m_db_user_crusade->wave())//已通关
	{
		res.set_error_code(e_gsc_errorcode_unknown_error);
		return;
	}
	
	ASSERT(req.wave() <= MAX_NUM_CRUSADE_WAVE);

	if(req.wave() > MAX_NUM_CRUSADE_WAVE){
		res.set_error_code(e_gsc_errorcode_unknown_error);
		return;
	} 

	//判断波数对不对
	if(m_db_user_crusade->wave() != req.wave()){
		res.set_error_code(e_gsc_errorcode_unknown_error);
		return;
	}
	
	if(req.enemy_heros_size() != MAXNUM_HEROINTEAM || 0 == req.me_heros_size() || req.me_heros_size() >  MAXNUM_HEROINTEAM){
		res.set_error_code(e_gsc_errorcode_unknown_error);
		return;
	}

	//验证敌人出战阵容(看服务器保存的客户端阵容和客户端请求阵容是否一致)
	std::set<UINT32> setEnemy;
	if(m_db_user_crusade->enemy_heros().size()){
		::google::protobuf::RepeatedPtrField< ::user_crusade_crusade_hero >::const_iterator it_enemy = m_db_user_crusade->enemy_heros().begin();
		for (; it_enemy != m_db_user_crusade->enemy_heros().end(); ++it_enemy)
		{
			setEnemy.insert(it_enemy->hero_id());
		}

		::google::protobuf::RepeatedField< ::google::protobuf::uint32 >::const_iterator it = req.enemy_heros().begin();
		for (; it != req.enemy_heros().end(); ++it)
		{
			std::set<UINT32>::iterator it_id = setEnemy.find(*it);
			if (it_id == setEnemy.end())
			{
				res.set_error_code(e_gsc_errorcode_unknown_error);
				return;
			}
			setEnemy.erase(it_id);
		}
		if(setEnemy.size())
		{
			res.set_error_code(e_gsc_errorcode_unknown_error);
			return;
		}
	}
	
	//验证我方英雄出战阵容
	::google::protobuf::RepeatedField< ::google::protobuf::uint32 >::const_iterator it_me = req.me_heros().begin();
	for (; it_me != req.me_heros().end(); ++it_me)
	{
		if(!m_pUser->GetHero(*it_me)){//英雄是否存在
			res.set_error_code(e_gsc_errorcode_unknown_error);
			return;
		}
		std::map<UINT32, user_crusade::crusade_hero*>::iterator it_hero = m_map_me_heros.find(*it_me);
		if (it_hero != m_map_me_heros.end())
		{
			if(0 == it_hero->second->hp()){//是否活着
				res.set_error_code(e_gsc_errorcode_unknown_error);
				return;
			}
		}
	}

	//最新的一波(需要将敌方英雄保存)
	if(!m_db_user_crusade->enemy_heros_size()){
		::google::protobuf::RepeatedField< ::google::protobuf::uint32 >::const_iterator it = req.enemy_heros().begin();
		for (; it != req.enemy_heros().end(); ++it)
		{
			user_crusade::crusade_hero* enemy_hero = m_db_user_crusade->add_enemy_heros();
			enemy_hero->set_hero_id(*it);
		}
	}
	res.set_error_code(e_gsc_errorcode_success);
	res.set_wave(req.wave());
	m_is_fighting = true;
	return;
}

void CUserCrusadeStrategy::CrusadeEnd( c_gs_crusade_end_req& req, gs_c_crusade_end_res& res, gs_dbs_user_info_op_req& db_req, GS_C_NTF& ntf)
{
	if(!m_is_fighting){//在战斗中
		res.set_error_code(e_gsc_errorcode_unknown_error);
		return;
	}

	if (MAX_NUM_CRUSADE_WAVE + 1 == m_db_user_crusade->wave())//已通关
	{
		res.set_error_code(e_gsc_errorcode_unknown_error);
		return;
	}
	
	if (req.is_win())//赢了
	{	
		//检测自己的英雄信息
		::google::protobuf::RepeatedPtrField< ::user_crusade_crusade_hero >::const_iterator it_me = req.me_heros().begin();
		for (; it_me != req.me_heros().end(); ++it_me)
		{
			if(!m_pUser->GetHero(it_me->hero_id())){
				res.set_error_code(e_gsc_errorcode_unknown_error);
				return;
			}
		}
		
		//自己英雄信息
		_add_or_update_me_heros(req.me_heros());

		//敌人信息(赢了直接删除)
		m_db_user_crusade->clear_enemy_heros();
		
		ASSERT(m_db_user_crusade->wave() <= MAX_NUM_CRUSADE_WAVE);

		std::vector<stGameElement> stEles;
		CrusadeMgr::GetInstance()->GetCrusadeWavePrize(m_pUser->GetLevel(), m_db_user_crusade->wave(), stEles);
		//给奖励
		UINT32 iPromote = m_pUser->GetEventTotalValue(EEventType_Crusade_Promote_Reward, EEventSubType(m_db_user_crusade->wave()));
		if (iPromote >= 1)
		{
			CrusadeMgr::GetInstance()->PromotePrize(stEles);
		}
		m_pUser->UpdateGameElement(stEles, db_req, *res.mutable_outputs(), ntf);
		m_db_user_crusade->set_wave(m_db_user_crusade->wave() + 1);
	}else{//输了
		ASSERT(req.enemy_heros_size());
		if (!req.enemy_heros_size())
		{
			res.set_error_code(e_gsc_errorcode_unknown_error);
			return;
		}

		//自己英雄保存
		_add_or_update_me_heros(req.me_heros());

		//敌人英雄保存
		m_db_user_crusade->mutable_enemy_heros()->CopyFrom(req.enemy_heros());
	}
	
	gs_dbs_user_info_op_req::struct_op_crusade* op_crusade = db_req.mutable_op_crusade();
	op_crusade->set_op_type(gs_dbs_user_info_op_req::struct_op_crusade::CRUSADE_END);
	op_crusade->mutable_end()->mutable_me_heros()->CopyFrom(*req.mutable_me_heros());
	op_crusade->mutable_end()->mutable_enemy_heros()->CopyFrom(*req.mutable_enemy_heros());
	op_crusade->mutable_end()->set_wave(m_db_user_crusade->wave());

	res.set_error_code(e_gsc_errorcode_success);
	return;
}

void CUserCrusadeStrategy::CrusadeHeroLife( c_gs_crusade_hero_life_req& req, gs_c_crusade_hero_life_res& res, gs_dbs_user_info_op_req& db_req, GS_C_NTF& ntf)
{
	//验证英雄
	UINT32 iHeroID = req.hero_id();
	std::map<UINT32, user_crusade::crusade_hero*>::iterator it = m_map_me_heros.find(iHeroID);
	if (it == m_map_me_heros.end())
	{
		res.set_error_code(e_gsc_errorcode_no_this_hero);
		return;
	}
	
	//判断血量
	if (0 != it->second->hp())
	{
		res.set_error_code(e_gsc_errorcode_unknown_error);
		return;
	}

	//判断钻石是否满足
	UINT32 iTotalTimes = m_pUser->GetEventTotalValue(EEventType_Crusade_Hero_Life, EEventSubType_None);
    UINT32 iCostDiamond = CVipMgr::GetInstance()->GetCostDiamond(EBUY_CRUSUDE_HERO_LIFE, iTotalTimes + 1);
	if (iCostDiamond > m_pUser->GetDiamond())
	{
		res.set_error_code(e_gsc_errorcode_not_enough_diamond);
		return;
	}
	
	stGameElement stEle(EELEMENTTYPE_DIAMOND, 0, 0, stGameElement::DECREASE, iCostDiamond);
	m_pUser->UpdateGameElement(stEle, db_req, *res.mutable_consumes(), ntf);

	//复活英雄(直接删除)
	_del_me_hero(req.hero_id());
	
	gs_dbs_user_info_op_req::struct_op_crusade* op_crusade = db_req.mutable_op_crusade();
	op_crusade->set_op_type(gs_dbs_user_info_op_req::struct_op_crusade::HERO_LIFE);
	op_crusade->set_hero_id(req.hero_id());

	//增加购买次数
	m_pUser->UpdateEventInfo(EEventType_Crusade_Hero_Life, EEventSubType_None, db_req, Now(), 1);
	res.set_error_code(e_gsc_errorcode_success);
	return;
}

void CUserCrusadeStrategy::CrusadeReset( c_gs_crusade_reset_req& req, gs_c_crusade_reset_res& res, gs_dbs_user_info_op_req& db_req )
{
	//可以使用的重置次数
	UINT32 iCrusadeNum = m_pUser->GetEventTodayValue(EEventType_Crusade_Reset, EEventSubType_None);
	CVipCfg* cfg = CVipMgr::GetInstance()->GetVipCfgByVipLevel(m_pUser->GetVipLevel());
	if (!cfg)
	{
		res.set_error_code(e_gsc_errorcode_unknown_error);
		return;
	}
	if (iCrusadeNum  >= cfg->m_max_num_crusade)
	{
		res.set_error_code(e_gsc_errorcode_crusade_reset_times_not_enough);
		return;
	}

	_clear_crusade(db_req);
	
	db_req.mutable_op_crusade()->set_op_type(gs_dbs_user_info_op_req::struct_op_crusade::RESET);
	//增加次数
	m_pUser->UpdateEventInfo(EEventType_Crusade_Reset, EEventSubType_None, db_req, Now(), 1);
	res.set_error_code(e_gsc_errorcode_success);
	return;
}

void CUserCrusadeStrategy::CrusadePromoteAward( c_gs_crusade_promote_award_req& req, gs_c_crusade_promote_award_res& res, gs_dbs_user_info_op_req& db_req )
{
	//查看消耗次数
	UINT32 iPromoteTimes = m_pUser->GetEventTotalValue(EEventType_Crusade_Promote_Reward, EEventSubType(m_db_user_crusade->wave()));
	ASSERT(iPromoteTimes <= 1);
	if (1 == iPromoteTimes)//这波已经提升
	{
		res.set_error_code(e_gsc_errorcode_crusade_promote_times_not_enough);
		return;
	}
	//同步提升事件
	m_pUser->UpdateEventInfo(EEventType_Crusade_Promote_Reward, EEventSubType(m_db_user_crusade->wave()), db_req, Now(), 1);
}

void CUserCrusadeStrategy::CrusadeBuffRefresh(c_gs_crusade_buff_refresh_req& req, gs_c_crusade_buff_refresh_res& res, gs_dbs_user_info_op_req& db_req, GS_C_NTF& ntf)
{
	//查看是否提升奖励
	UINT32 iPromoteTimes = m_pUser->GetEventTotalValue(EEventType_Crusade_Promote_Reward, EEventSubType(m_db_user_crusade->wave()));
	if (0 == iPromoteTimes)
	{
		res.set_error_code(e_gsc_errorcode_unknown_error);
		return;
	}
	UINT32 iBuffTimes = m_pUser->GetEventTotalValue(EEventType_Crusade_Refresh_Buff, EEventSubType_None);
	UINT32 iCostDiamond = CVipMgr::GetInstance()->GetCostDiamond(EBUY_CRUSUDE_REFRESH_BUFF, iBuffTimes + 1);
	if (iCostDiamond > m_pUser->GetDiamond())
	{
		res.set_error_code(e_gsc_errorcode_not_enough_diamond);
		return;
	}

	//同步事件
	m_pUser->UpdateEventInfo(EEventType_Crusade_Refresh_Buff, EEventSubType_None, db_req, Now(), 1);
	
	stGameElement stEle(EELEMENTTYPE_DIAMOND, 0, 0, stGameElement::DECREASE, iCostDiamond);
	m_pUser->UpdateGameElement(stEle, db_req, *res.mutable_consumes(), ntf);
	res.set_error_code(e_gsc_errorcode_success);
	return;
}

void CUserCrusadeStrategy::_add_or_update_me_heros(const ::google::protobuf::RepeatedPtrField< ::user_crusade_crusade_hero >& me_heros )
{
	::google::protobuf::RepeatedPtrField< ::user_crusade_crusade_hero >::const_iterator it_me = me_heros.begin();
	for (; it_me != me_heros.end(); ++it_me)
	{
		std::map<UINT32, user_crusade::crusade_hero*>::iterator it = m_map_me_heros.find(it_me->hero_id());
		if (it != m_map_me_heros.end())
		{
			it->second->CopyFrom(*it_me);
		}else{
			user_crusade::crusade_hero* hero = m_db_user_crusade->add_me_heros();
			hero->CopyFrom(*it_me);
			m_map_me_heros[hero->hero_id()] = hero;
		}
	}
}

void CUserCrusadeStrategy::_del_me_hero( UINT32 iHeroID )
{
	std::map<UINT32, user_crusade::crusade_hero*>::iterator it = m_map_me_heros.find(iHeroID);
	if (it != m_map_me_heros.end())
	{
		m_map_me_heros.erase(it);
	}

	for(int i = 0; i < m_db_user_crusade->me_heros_size(); ++i){
		if (m_db_user_crusade->mutable_me_heros(i)->hero_id() == iHeroID)
		{
			m_db_user_crusade->mutable_me_heros()->SwapElements(i, m_db_user_crusade->me_heros_size() - 1);
			m_db_user_crusade->mutable_me_heros()->RemoveLast();
			break;
		}
	}
}

void CUserCrusadeStrategy::_clear_crusade(gs_dbs_user_info_op_req& db_req)
{
	m_db_user_crusade->clear_enemy_heros();
	m_db_user_crusade->clear_me_heros();
	m_db_user_crusade->set_wave(1);
	m_map_me_heros.clear();
	m_has_crusade = false;

	//英雄复活
	m_pUser->ClearEvent(EEventType_Crusade_Hero_Life, EEventSubType_None, db_req);
	//刷新buff
	m_pUser->ClearEvent(EEventType_Crusade_Refresh_Buff, EEventSubType_None, db_req);
	//提升奖励
	for (UINT32 i = 0; i < MAX_NUM_CRUSADE_WAVE; ++i)
	{
		m_pUser->ClearEvent(EEventType_Crusade_Promote_Reward, EEventSubType(i), db_req);
	}
}




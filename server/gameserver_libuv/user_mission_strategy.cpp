#include "user_mission_strategy.h"
#include "sys_setting_mgr.h"
#include "time_helper.h"
#include "user.h"
#include "event.h"
bool CUserMissionStrategy::Init(CUser* pUser, db_user_mission_event_array* mission_event_arr, db_user_mission_chain_array* chain_arr, gs_dbs_user_info_op_req& db_req)
{
	ASSERT(pUser &&  mission_event_arr && chain_arr);
	CHECK_NULL_RETURN_FALSE(pUser && mission_event_arr && chain_arr);
	
	m_pUser = pUser;
	m_mission_event_arr = mission_event_arr;
	m_mission_chain_arr = chain_arr;

	::google::protobuf::RepeatedPtrField< ::db_user_mission_event >::iterator it = mission_event_arr->mutable_events()->begin();
	for (; it != mission_event_arr->mutable_events()->end(); ++it)
	{
		EEventType eventType = GetEventType(it->mission_event_type());
		EEventSubType eSubType = EEventSubType(it->mission_event_sub_type());
		std::pair<EEventType, EEventSubType> key = std::make_pair(eventType, eSubType);
		ASSERT(m_mapEventType2Event.find(key) ==  m_mapEventType2Event.end());
		m_mapEventType2Event[key] = &*it;
	}
																								
	_add_count_event();

	::google::protobuf::RepeatedPtrField< ::db_user_mission_chain >::iterator iter = chain_arr->mutable_chains()->begin();
	for (; iter != chain_arr->mutable_chains()->end(); ++iter)
	{
		ASSERT(m_mapChainId2UserChain.find(iter->mission_chain_id()) == m_mapChainId2UserChain.end());
		m_mapChainId2UserChain[iter->mission_chain_id()] = &*iter;
	}
	
	//登陆事件
	UpdateEventInfo(EEventType_Login, EEventSubType_None, db_req, Now(), 1);

	return true;
}

bool CUserMissionStrategy::GetMissonInfo( ::google::protobuf::RepeatedPtrField< ::user_mission >& rep_user_mission )
{
	std::set<UINT32> setChainId;//已处理过的chainId

	std::map<UINT32, CMission*>& missions = CMissionMgr::GetInstance()->GetMissions();
	for (std::map<UINT32, CMission*>::iterator iter = missions.begin(); iter != missions.end(); ++iter)
	{
		MissionInfo missionInfo;
		if (iter->second->iMisChainId){//链式
			if(setChainId.find(iter->second->iMisChainId) == setChainId.end()){
				if(GetChainCurMission(iter->second->iMisChainId, missionInfo))
				{
					ASSERT(missionInfo.pMission);
				}
				setChainId.insert(iter->second->iMisChainId);
			}
		}
		else{//非链式(每一个任务都要进行计算)
			user_mission::MISSION_STATUS status;
			if (iter->second->eType == EMISSIONTYPE_MONTH_CARD_REWARD)//月卡处理EMISSIONTYPE_MONTH_CARD_REWARD，购买与支付的任务在内部处理，月卡进行单独处理
			{
				if(!IsMissionAlreadyGetReward(iter->second->iMissionId))
				{
					status = CMissionMgr::GetInstance()->GetMissionStatus(iter->second, m_pUser->UserID());
					ASSERT(status == user_mission::STATUS_CAN_ATTACH);//月卡没有配置进入与完成条件统一通过FillUserMission计算结果
					if (status  == user_mission::STATUS_CAN_ATTACH)
					{
						missionInfo.pMission = iter->second;
						missionInfo.mission_status = status;
					}
				}
			}
			//忽略EMISSIONTYPE_MONTH_CARD_BUY，EMISSIONTYPE_MONTH_CARD_PAY，处理非月卡的任务
			else if(iter->second->eType != EMISSIONTYPE_MONTH_CARD_BUY && iter->second->eType != EMISSIONTYPE_MONTH_CARD_PAY)
			{
				if (!IsMissionAlreadyGetReward(iter->second->iMissionId))
				{
					status = CMissionMgr::GetInstance()->GetMissionStatus(iter->second, m_pUser->UserID());
					if (status  != user_mission::STATUS_CAN_NOT_SEE)
					{
						missionInfo.pMission = iter->second;
						missionInfo.mission_status = status;
					}
				}
			}
		}
		if (missionInfo.pMission)
		{
			FillUserMission(missionInfo, rep_user_mission);//计算任务进度信息
		}
	}
	return true;
}

bool CUserMissionStrategy::FillUserMission(MissionInfo& missionInfo,  ::google::protobuf::RepeatedPtrField< ::user_mission >& rep_user_mission ){
	CMission* pMission = missionInfo.pMission;
	switch (missionInfo.pMission->eType)
	{
	case EMISSIONTYPE_HEALTH_LUNCH:
	case EMISSIONTYPE_HEALTH_DINNER:
	case EMISSIONTYPE_HEALTH_NIGHT:
		{
			user_mission* user_mis = rep_user_mission.Add();
			user_mis->set_mission_id(pMission->iMissionId);
			user_mis->set_mission_status(missionInfo.mission_status);
			user_mission::mission_event_need* event_need = user_mis->add_event_needs();
			event_need->set_event_type(user_mission::HEALTH_MEAL);
			event_need->set_event_sub_type(user_mission::NONE);
		}
		break;
	case EMISSIONTYPE_MONTH_CARD_REWARD:
		{
			MONTH_CARD_STATUS status = GetMissionMonthCardStatus(missionInfo.pMission->iMissionId);
			
			//任务添加(充值)
			if(status == MONTH_CARD_PAY || status == MONTH_CARD_BUY)
			{
				user_mission* user_mis = rep_user_mission.Add();
				user_mis->set_mission_status(user_mission::STATUS_IN);

				user_mission::mission_event_need* pNeed = user_mis->add_event_needs();
				UINT32 iEventVal = 0;
				UINT32 iNeedVal = 0;

				if(missionInfo.pMission->iMissionId == MONTH_CARD_REWARD_MISSION1_ID)
				{
					user_mis->set_mission_id(MONTH_CARD_BUY_MISSION1_ID);
					pNeed->set_event_type(user_mission::RECHARGE_MISSION_MONTH_CARD1);
					iEventVal = GetMissionEventValueByEventType(EEventType_Recharge_Mission_Month_Card1, EEventSubType_None, EMISSIONTYPE_MONTH_CARD_BUY);	
					ASSERT(CSysSettingMgr::GetInstance()->GetIntValue("Month_Card1_Recharge_Num", iNeedVal));
				}
				else if(missionInfo.pMission->iMissionId == MONTH_CARD_REWARD_MISSION2_ID)
				{
					user_mis->set_mission_id(MONTH_CARD_BUY_MISSION2_ID);
					pNeed->set_event_type(user_mission::RECHARGE_MISSION_MONTH_CARD2);
					iEventVal = GetMissionEventValueByEventType(EEventType_Recharge_Mission_Month_Card2, EEventSubType_None, EMISSIONTYPE_MONTH_CARD_BUY);	
					ASSERT(CSysSettingMgr::GetInstance()->GetIntValue("Month_Card2_Recharge_Num", iNeedVal));
				}

				pNeed->set_need_val(iNeedVal);
				pNeed->set_cur_val(iEventVal);
				pNeed->set_event_sub_type(user_mission::NONE);
			}
			//任务(奖励)
			if (status == MONTH_CARD_NORMAL || status == MONTH_CARD_PAY)
			{
				user_mission* user_mis = rep_user_mission.Add();
				user_mis->set_mission_id(missionInfo.pMission->iMissionId);
				user_mis->set_mission_status(user_mission::STATUS_CAN_ATTACH);
			}
		}
		break;
	case EMISSIONTYPE_DAILY:
		{
			user_mission* user_mis = rep_user_mission.Add();
			user_mis->set_mission_id(pMission->iMissionId);
			user_mis->set_mission_status(missionInfo.mission_status);
			
			if (missionInfo.mission_status != user_mission::STATUS_CAN_ATTACH)
			{
				//根据完成条件计算进度
				CalculateSchedule(pMission->finishCondition, EMISSIONTYPE_DAILY, user_mis->mutable_event_needs());
			}
		}
		break;
	case EMISSIONTYPE_ACHIEVEMENT:
		{
			user_mission* user_mis = rep_user_mission.Add();
			user_mis->set_mission_id(pMission->iMissionId);
			user_mis->set_mission_status(missionInfo.mission_status);

			if (missionInfo.mission_status != user_mission::STATUS_CAN_ATTACH)
			{
				//根据完成条件计算进度
				CalculateSchedule(pMission->finishCondition, EMISSIONTYPE_ACHIEVEMENT, user_mis->mutable_event_needs());
			}
		}
		break;
	default:
		return false;
	}
	return true;
}

void CUserMissionStrategy::CalculateSchedule( ICondition*(& conditions)[MAXNUM_CONDITION], EMISSIONTYPE eType, ::google::protobuf::RepeatedPtrField< ::user_mission_mission_event_need >* rep_mission_event )
{
	for (UINT32 i = 0; i < MAXNUM_CONDITION ; ++i)
	{
		ICondition* pCondition = conditions[i];
		if (!pCondition){
			continue;
		}
		
		UINT32 iNeedNum = 0;
		EEventType eEventType = EEventType_Min;
		EEventSubType eEventSubType = EEventSubType_Min;
		EConditionType eConType = pCondition->GetConditionType();
		switch (eConType)
		{
		case EConditionType_Event:
			{
				CCondition_Event* pEvent = dynamic_cast<CCondition_Event*>(pCondition);
				ASSERT(pEvent);
				if (!pEvent)
				{
					return;
				}
				iNeedNum = pEvent->GetNeedNum();
				eEventType = pEvent->GetEventType();
				eEventSubType = pEvent->GetEventSubType();
			}
			break;
		default:
			ASSERT(false);
			return;
		}
		user_mission::mission_event_need* pEvent = rep_mission_event->Add();  
		user_mission::MISSION_EVENT_TYPE event_type = user_mission::MISSION_EVENT_TYPE(GetEventType(eEventType));
		pEvent->set_event_type(event_type);
		pEvent->set_need_val(iNeedNum);
		user_mission::MISSION_EVENT_SUB_TYPE event_sub_type = user_mission::MISSION_EVENT_SUB_TYPE(GetEventSubType(eEventSubType));
		pEvent->set_event_sub_type(event_sub_type);
		
		UINT32 iCurVal = GetMissionEventValueByEventType(eEventType, eEventSubType, eType);
		pEvent->set_cur_val(iCurVal > iNeedNum ? iNeedNum : iCurVal);
	}
}

bool CUserMissionStrategy::GetChainCurMission( UINT32 iChainID, MissionInfo& missionInfo )
{
	std::map<UINT32, db_user_mission_chain*>::iterator it = m_mapChainId2UserChain.find(iChainID);
	if (it != m_mapChainId2UserChain.end()){
		CHECK_NULL_RETURN_FALSE(it->second);
		CMission* pMission = CMissionMgr::GetInstance()->GetMissionByMissionID(it->second->last_get_reward_mission_id());
		ASSERT(pMission);
		CHECK_NULL_RETURN_FALSE(pMission);
		UINT32 iMissionId = 0;

		//对于每天会重置的任务，如果判断领取时间不在当天的任务时间段，则从任务链的开始节点查找，对于永久类型的直接判断领取节点的下一个是否满足
		if(pMission->eMissionChainType == MissionChainType_Reset_Daily)
		{
			UINT32 iGlobalResetTime = CSysSettingMgr::GetInstance()->Global_Reset_Time;
			if( isSameDay(Now(), it->second->last_get_reward_timestamp(), iGlobalResetTime) && IsTimeConditionsSatisfy(pMission->enterCondition, m_pUser->UserID(), true, it->second->last_get_reward_timestamp()))
				iMissionId = pMission->iMissionId;
		}
		else if(pMission->eMissionChainType == MissionChainType_Forever)
		{
			iMissionId = pMission->iMissionId;
		}
		return CMissionMgr::GetInstance()->GetChainNextMission(it->second->mission_chain_id(), iMissionId, missionInfo, m_pUser->UserID());
	}else{//没做过该链的任务
		return CMissionMgr::GetInstance()->GetChainNextMission(iChainID, 0, missionInfo, m_pUser->UserID());	
	}
}


UINT32 CUserMissionStrategy::GetChainLastRewardID(UINT32 iChainID)
{
	std::map<UINT32, db_user_mission_chain*>::iterator it = m_mapChainId2UserChain.find(iChainID);
	if (it == m_mapChainId2UserChain.end())
	{
		return -1;
	}
	else
	{
		return it->second->last_get_reward_mission_id();
	}
}

UINT32 CUserMissionStrategy::GetMissionEventValueByEventType(EEventType eEventType, EEventSubType eEventSubType, EMISSIONTYPE eMissionType)
{
	//等级的任务(特殊处理)
	if (eEventType == EEventType_Level_Up || eEventType == EEventType_Hero_Collect)
	{
		std::pair<EEventType, EEventSubType> key = std::make_pair(eEventType, eEventSubType);
		std::map<std::pair<EEventType, EEventSubType>, db_user_mission_event*>::iterator it = m_mapEventType2Event.find(key);
		ASSERT(it != m_mapEventType2Event.end());
		return it->second->total_value();
	}

	std::pair<EEventType, EEventSubType> key = std::make_pair(eEventType, eEventSubType);
	std::map<std::pair<EEventType, EEventSubType>, db_user_mission_event*>::iterator it = m_mapEventType2Event.find(key);
	if (it == m_mapEventType2Event.end())
	{
		return 0;
	}

	switch(eMissionType)
	{
	case EMISSIONTYPE_DAILY:
		{
			UINT32 iGlobalTime = 0;
			CSysSettingMgr::GetInstance()->GetIntValue("Global_Reset_Time", iGlobalTime);
			if(isSameDay(Now(), it->second->last_update_timestamp(), iGlobalTime))
			{
				return it->second->last_day_value();
			}
			else
			{
				return 0;
			}
		}
		break;
	case EMISSIONTYPE_MONTH_CARD_BUY:
	case EMISSIONTYPE_MONTH_CARD_REWARD:
	case EMISSIONTYPE_MONTH_CARD_PAY:
		{
			return it->second->total_value();
		}
		break;
	case EMISSIONTYPE_ACHIEVEMENT:
		{
			return it->second->total_value();
		}
		break;
	default:
		ASSERT(false);
		return 0;
	}
}

UINT32 CUserMissionStrategy::GetEventTodayValue(EEventType eEventType, EEventSubType eEventSubType)
{
	std::pair<EEventType, EEventSubType> key = std::make_pair(eEventType, eEventSubType);
	std::map<std::pair<EEventType, EEventSubType>, db_user_mission_event*>::iterator it = m_mapEventType2Event.find(key);
	if (it == m_mapEventType2Event.end())
	{
		return 0;
	}

	if(isSameDay(Now(), it->second->last_update_timestamp(), CSysSettingMgr::GetInstance()->Global_Reset_Time))
	{
		return it->second->last_day_value();
	}
	else
	{
		return 0;
	}

}

bool CUserMissionStrategy::IsMissionAlreadyGetReward(UINT32 iMissionID)
{
	CMission* pMission = CMissionMgr::GetInstance()->GetMissionByMissionID(iMissionID);
	ASSERT(pMission);
	if (!pMission)
	{
		return true;
	}	
	
	EMISSIONTYPE eMisType = pMission->eType;
	ASSERT(eMisType != EMISSIONTYPE_MONTH_CARD_BUY  && eMisType != EMISSIONTYPE_MONTH_CARD_PAY);
	if (eMisType == EMISSIONTYPE_MONTH_CARD_BUY || eMisType == EMISSIONTYPE_MONTH_CARD_PAY)
	{
		return true;
	}

	std::pair<EEventType, EEventSubType> key = std::make_pair(EEventType_Get_Reward, EEventSubType(iMissionID));
	std::map<std::pair<EEventType, EEventSubType>, db_user_mission_event*>::iterator iter = m_mapEventType2Event.find(key);
	if (iter == m_mapEventType2Event.end())
	{
		return false;
	}
	else
	{
		if( pMission->eType == EMISSIONTYPE_DAILY ||
			pMission->eType == EMISSIONTYPE_MONTH_CARD_REWARD ||
			pMission->eType == 	EMISSIONTYPE_HEALTH_LUNCH ||
			pMission->eType == 	EMISSIONTYPE_HEALTH_DINNER||
			pMission->eType == 	EMISSIONTYPE_HEALTH_NIGHT)
		{
			UINT32 iUpdateTime = iter->second->last_update_timestamp();
			UINT32 iResetTime;
			CSysSettingMgr::GetInstance()->GetIntValue("Global_Reset_Time", iResetTime);
			if(isSameDay(Now(), iUpdateTime, iResetTime))
			{
				return true;
			}
			return false;//不在同一天
		}
		else if(pMission->eType == EMISSIONTYPE_ACHIEVEMENT)
		{
			return true;
		}
		else
		{
			ASSERT(false);
			return true;
		}
	}
}

UINT32 CUserMissionStrategy::GetMonthCardBuyTime(UINT32 iMissionID)
{
	UINT32 iBuyTime = 0;
	std::pair<EEventType, EEventSubType> key;
	if (iMissionID == MONTH_CARD_REWARD_MISSION1_ID)
	{
		key = std::make_pair(EEventType_Recharge_Mission_Month_Card1, EEventSubType_None);
	}
	else if(iMissionID == MONTH_CARD_REWARD_MISSION2_ID)
	{
		key = std::make_pair(EEventType_Recharge_Mission_Month_Card2, EEventSubType_None);
	}
	else
	{
		ASSERT(false);
		return 0;
	}

	std::map<std::pair<EEventType, EEventSubType>, db_user_mission_event*>::iterator iterEvent = m_mapEventType2Event.find(key);
	if(iterEvent != m_mapEventType2Event.end())
	{
		iBuyTime = iterEvent->second->last_update_timestamp();
	}
	else{
		return 0;
	}
	return iBuyTime;
}


MONTH_CARD_STATUS CUserMissionStrategy::GetMissionMonthCardStatus(UINT32 iMissionID)
{
	UINT32 iBuyTime = GetMonthCardBuyTime(iMissionID);

	UINT32 iMonthCardBuyIncreaseSeconds;
	ASSERT(CSysSettingMgr::GetInstance()->GetIntValue("Month_Card_Each_Buy_Increase_Seconds", iMonthCardBuyIncreaseSeconds));

	time_t now = Now();
	if (now - iBuyTime > iMonthCardBuyIncreaseSeconds)//月卡过期或没有购买过
	{
		return MONTH_CARD_BUY;
	}
	else
	{	
		UINT32 iMonthCardBuyLessThanSeconds = 0;
		UINT32 now = Now();
		UINT32 iMonthCardBuyIncreaseSeconds = 0;
		ASSERT(CSysSettingMgr::GetInstance()->GetIntValue("Month_Card_Each_Buy_Increase_Seconds", iMonthCardBuyLessThanSeconds));
		ASSERT(CSysSettingMgr::GetInstance()->GetIntValue("Month_Card_Buy_Less_Than_Seconds", iMonthCardBuyLessThanSeconds));

		if(now > iMonthCardBuyIncreaseSeconds - iMonthCardBuyLessThanSeconds && now < iBuyTime + iMonthCardBuyIncreaseSeconds)//27-30天(续费)
		{
			return MONTH_CARD_PAY;
		}
		else
		{
			return MONTH_CARD_NORMAL;
		}
	}
}

bool CUserMissionStrategy::GetMissionReward(UINT32 iMissionId, gs_dbs_user_info_op_req& db_req, gs_c_get_mission_reward_res& res, GS_C_NTF& ntf){
	CMission* pMission = CMissionMgr::GetInstance()->GetMissionByMissionID(iMissionId);
	ASSERT(pMission);
	CHECK_NULL_RETURN_FALSE(pMission);
	UINT32 now = Now();
	switch(pMission->eType)
	{
	case EMISSIONTYPE_HEALTH_LUNCH:
	case EMISSIONTYPE_HEALTH_DINNER:
	case EMISSIONTYPE_HEALTH_NIGHT:
		{
			//db_req(更新user_mission_chain, user_mission_event)
			gs_dbs_user_info_op_req::struct_op_mission* op_mission = db_req.add_missions();
			db_user_mission_event* pEvent = op_mission->mutable_mission_event();
			pEvent->set_mission_event_type(db_user_mission_event::GET_REWARD);
			pEvent->set_mission_event_sub_type(db_user_mission_event::MISSION_EVENT_SUB_TYPE(pMission->iMissionId));
			pEvent->set_last_update_timestamp(now);
			_update_event_info(db_user_mission_event::GET_REWARD, db_user_mission_event::MISSION_EVENT_SUB_TYPE(pMission->iMissionId), now);
			
			db_user_mission_chain* pChain = op_mission->mutable_mission_chain();
			pChain->set_mission_chain_id(pMission->iMisChainId);
			pChain->set_last_get_reward_mission_id(pMission->iMissionId);
			pChain->set_last_get_reward_timestamp(now);
			_update_chain_info(pMission->iMisChainId, pMission->iMissionId, now);

			//res
			res.set_error_code(e_gsc_errorcode_success);
			res.set_mission_id(iMissionId);
			res.set_status(user_mission::STATUS_DELED);
			m_pUser->UpdateGameElement(pMission->vecPrizes, db_req, *res.mutable_eles(), ntf);

			//mis_ntf
			MissionInfo misInfo;
			if(CMissionMgr::GetInstance()->GetChainNextMission(pMission->iMisChainId, pMission->iMissionId, misInfo, m_pUser->UserID()))
			{
				gs_c_mission_ntf_array::mission_opt* pMisNtf = ntf.mutable_mis_ntf_arr()->add_mis_opts();
				pMisNtf->set_opt(gs_c_mission_ntf_array::mission_opt::ADD);
				pMisNtf->mutable_mis()->set_mission_id(misInfo.pMission->iMissionId);
				pMisNtf->mutable_mis()->set_mission_status(misInfo.mission_status);
				user_mission::mission_event_need* event_need = pMisNtf->mutable_mis()->add_event_needs();
				event_need->set_event_type(user_mission::HEALTH_MEAL);
				event_need->set_event_sub_type(user_mission::NONE);
			}
		}
		break;
	case EMISSIONTYPE_MONTH_CARD_REWARD:
		{
			{//db_req(领取事件添加)
				gs_dbs_user_info_op_req::struct_op_mission* op_mission = db_req.add_missions();
				db_user_mission_event* pEvent = op_mission->mutable_mission_event();
				pEvent->set_mission_event_type(db_user_mission_event::GET_REWARD);
				pEvent->set_mission_event_sub_type(user_mission::MISSION_EVENT_SUB_TYPE(iMissionId));
				pEvent->set_last_update_timestamp(now);
				_update_event_info(db_user_mission_event::GET_REWARD, db_user_mission_event::MISSION_EVENT_SUB_TYPE(iMissionId), now);
			}
			//res
			res.set_error_code(e_gsc_errorcode_success);
			res.set_mission_id(iMissionId);
			res.set_status(user_mission::STATUS_DELED);
			m_pUser->UpdateGameElement(pMission->vecPrizes, db_req, *res.mutable_eles(), ntf);

		}
		break;
	case EMISSIONTYPE_DAILY:
	case EMISSIONTYPE_ACHIEVEMENT:
		{
			//db_req
			gs_dbs_user_info_op_req::struct_op_mission* op_mission = db_req.add_missions();
			db_user_mission_event* pEvent = op_mission->mutable_mission_event();
			pEvent->set_mission_event_type(db_user_mission_event::GET_REWARD);
			pEvent->set_mission_event_sub_type(iMissionId);
			pEvent->set_last_update_timestamp(now);
			_update_event_info(db_user_mission_event::GET_REWARD, db_user_mission_event::MISSION_EVENT_SUB_TYPE(iMissionId), now);
			if (pMission->iMisChainId)
			{
				db_user_mission_chain* pChain = op_mission->mutable_mission_chain();
				pChain->set_mission_chain_id(pMission->iMisChainId);
				pChain->set_last_get_reward_mission_id(pMission->iMissionId);
				pChain->set_last_get_reward_timestamp(now);
				_update_chain_info(pMission->iMisChainId, pMission->iMissionId, now);
			}
			//res
			res.set_error_code(e_gsc_errorcode_success);
			res.set_mission_id(iMissionId);
			res.set_status(user_mission::STATUS_DELED);
			m_pUser->UpdateGameElement(pMission->vecPrizes, db_req, *res.mutable_eles(), ntf);

			//mis_ntf
			if(pMission->iMisChainId)
			{
				MissionInfo misInfo;
				if(CMissionMgr::GetInstance()->GetChainNextMission(pMission->iMisChainId, pMission->iMissionId, misInfo, m_pUser->UserID()))
				{
					gs_c_mission_ntf_array::mission_opt* pMisNtf = ntf.mutable_mis_ntf_arr()->add_mis_opts();
					pMisNtf->set_opt(gs_c_mission_ntf_array::mission_opt::ADD);
					pMisNtf->mutable_mis()->set_mission_id(misInfo.pMission->iMissionId);
					pMisNtf->mutable_mis()->set_mission_status(misInfo.mission_status);

					if (misInfo.mission_status == user_mission::STATUS_IN)//进行中,计算进度信息
					{
						std::vector<ICondition*> vecCondition;
						misInfo.pMission->GetFinishCondition(vecCondition);
						for (std::vector<ICondition*>::iterator it = vecCondition.begin(); it != vecCondition.end(); ++it)
						{
							CCondition_Event* pConditionEvent = dynamic_cast<CCondition_Event*>(*it);
							ASSERT(pConditionEvent);
							CHECK_NULL_RETURN_FALSE(pConditionEvent);
							EEventType event_type =  pConditionEvent->GetEventType();
							EEventSubType event_sub_type = pConditionEvent->GetEventSubType();
							UINT32 iNeedValue = pConditionEvent->GetNeedNum();
							UINT32 iCurValue = GetMissionEventValueByEventType(event_type, event_sub_type, misInfo.pMission->eType);
							
							user_mission::mission_event_need* event_need = pMisNtf->mutable_mis()->add_event_needs();
							event_need->set_event_type(TransformType(GetEventType(event_type)));
							event_need->set_event_sub_type(GetEventSubType(event_sub_type));
							if (iCurValue >= iNeedValue)
							{
								event_need->set_cur_val(iNeedValue);
							}
							else
							{
								event_need->set_cur_val(iCurValue);
							}
							event_need->set_need_val(iNeedValue);
						}
					}
				}
			}
		}
		break;
	default:
		ASSERT(false);
		return false;
	}
	return true;
}

bool CUserMissionStrategy::TriggerMission( EEventType eEventType, EEventSubType eEventSubType, gs_dbs_user_info_op_req& db_req, GS_C_NTF& ntf, UINT32 iValue /*= 1*/ ){
	UINT32 iTriggerTime = Now();
	switch(eEventType)	
	{
	case EEventType_Recharge:
		{
			_trigger_month_card(iValue, db_req, *ntf.mutable_mis_ntf_arr());//如果是充值则会对月卡1和月卡2任务产生影响
			_fill_db_req(eEventType, eEventSubType, db_req, iTriggerTime, iValue);
			_update_event_info(GetEventType(eEventType), GetEventSubType(eEventSubType), iTriggerTime, iValue);
		}
		break;
	case EEventType_Level_Up:
	case EEventType_Hero_Collect:
		{
			_trigger_mission_by_condition(eEventType, eEventSubType, *ntf.mutable_mis_ntf_arr(), iValue);
			_update_event_info(GetEventType(eEventType), GetEventSubType(eEventSubType), iTriggerTime, iValue);
		}
		break;
	case EEventType_Complete_Chapter_FB_Three_Star:
		{
			if(_cal_chapter_fb_three_star(eEventType, eEventSubType)) 
			{//从没完成到完成
				_fill_db_req(eEventType, eEventSubType, db_req, iTriggerTime, iValue);
				_update_event_info(GetEventType(eEventType), GetEventSubType(eEventSubType), iTriggerTime, iValue);
				_trigger_mission_by_condition(eEventType, eEventSubType, *ntf.mutable_mis_ntf_arr(), iValue);
			}
		}
		break;
	default:
		{
			_trigger_mission_by_condition(eEventType, eEventSubType, *ntf.mutable_mis_ntf_arr(), iValue);
			_fill_db_req(eEventType, eEventSubType, db_req, iTriggerTime, iValue);
			_update_event_info(GetEventType(eEventType), GetEventSubType(eEventSubType), iTriggerTime, iValue);
		}
		break;
	}
	return true;
}

void CUserMissionStrategy::_update_event_info(db_user_mission_event::MISSION_EVENT_TYPE event_type, db_user_mission_event::MISSION_EVENT_SUB_TYPE sub_type, UINT32 iUpdateTimeStamp, UINT32 iValue/* = 1*/)
{
	db_user_mission_event* pEvent = NULL;

	EEventType eEventType = GetEventType(event_type);
	EEventSubType eSubType = EEventSubType(sub_type);

	std::pair<EEventType, EEventSubType> key = std::make_pair(eEventType, eSubType);
	std::map<std::pair<EEventType, EEventSubType>, db_user_mission_event*>::iterator it = m_mapEventType2Event.find(key);
	if (it == m_mapEventType2Event.end())
	{
		pEvent = m_mission_event_arr->add_events();
		m_mapEventType2Event[key] = pEvent;
	}
	else
	{
		pEvent = it->second;            
	}
	ASSERT(pEvent);
	if(!pEvent)
	{
		return;
	}
	switch(eEventType)
	{
	case EEventType_Level_Up:
	case EEventType_Recharge://累计充值
		{
			pEvent->set_mission_event_type(event_type);
			pEvent->set_mission_event_sub_type(sub_type);
			pEvent->set_total_value(iValue + pEvent->total_value());
		}
		break;
	case EEventType_Health_Meal:
		{
			pEvent->set_mission_event_type(event_type);
			pEvent->set_mission_event_sub_type(sub_type);
			pEvent->set_last_update_timestamp(iUpdateTimeStamp);
		}
		break;
	case EEventType_Get_Reward:
		{
			pEvent->set_mission_event_type(event_type);
			pEvent->set_mission_event_sub_type(sub_type);
			pEvent->set_last_update_timestamp(iUpdateTimeStamp);
		}
		break;
	case EEventType_Skill_Level_Up:
	case EEventType_Common_FB_Complete:
	case EEventType_Common_FB_All_Through:
	case EEventType_Common_FB_All_Through_Three_Star:
	case EEventType_Equip_Level_Up:
	case EEventType_Hero_Collect:
	case EEventType_Buy_Gold:
	case EEventType_Give_Friend_Health:
	case EEventType_Mission_Main:
	case EEventType_Mission_Free:
	case EEventType_Activity:
	case EEventType_Invoke:
	case EEventType_Arena_Challenge:
	case EEventType_Trial_Tower:
	case EEventType_Expedition:
	case EEventType_FB_Fail:
	case EEventType_Complete_Chapter_FB:
	case EEventType_Arena_Rank_Top:
	case EEventType_Complete_Chapter_FB_Three_Star:
	case EEventType_Buy_Health:
	case EEventType_Buy_Skill_Point:
	case EEventType_Login:
	case EEventType_Del_Friend:
	case EEventType_Crusade_Hero_Life:
	case EEventType_Crusade_Reset:
	case EEventType_Crusade_Promote_Reward:
	case EEventType_Crusade_Refresh_Buff:
		{
			pEvent->set_mission_event_type(event_type);
			pEvent->set_mission_event_sub_type(sub_type);
			if (isSameDay(iUpdateTimeStamp, pEvent->last_update_timestamp(), CSysSettingMgr::GetInstance()->Global_Reset_Time))
			{
				pEvent->set_last_day_value(iValue + pEvent->last_day_value());
			}
			else
			{
				pEvent->set_last_day_value(iValue);
			}
			pEvent->set_total_value(iValue + pEvent->total_value());
			pEvent->set_last_update_timestamp(iUpdateTimeStamp);
		}
		break;
	default:
		ASSERT(false && "不支持的类型");
	}
}

void CUserMissionStrategy::_update_chain_info(UINT32 iChainID, UINT32 iRewardID, UINT32 iUpdateTimeStamp)
{
	std::map<UINT32, db_user_mission_chain*>::iterator it = m_mapChainId2UserChain.find(iChainID);
	if (it == m_mapChainId2UserChain.end())
	{
		db_user_mission_chain* pChain = m_mission_chain_arr->add_chains();
		pChain->set_mission_chain_id(iChainID);
		pChain->set_last_get_reward_mission_id(iRewardID);
		pChain->set_last_get_reward_timestamp(iUpdateTimeStamp);
		m_mapChainId2UserChain[iChainID] = pChain;
	}
	else
	{
		it->second->set_last_get_reward_mission_id(iRewardID);
		it->second->set_last_get_reward_timestamp(iUpdateTimeStamp);
	}
}

db_user_mission_event* CUserMissionStrategy::_get_event_info(EEventType eEventType, EEventSubType eEventSubType)
{
	std::pair<EEventType, EEventSubType> key = std::make_pair(eEventType, eEventSubType);
	std::map<std::pair<EEventType, EEventSubType>, db_user_mission_event*>::iterator it = m_mapEventType2Event.find(key);
	if (it == m_mapEventType2Event.end())
	{
		return NULL;
	}
	return it->second;
}

UINT32 CUserMissionStrategy::GetEventTotalValue(EEventType eEventType, EEventSubType eEventSubType)
{
	db_user_mission_event* event_info = _get_event_info(eEventType, eEventSubType);
	if (event_info){
		return event_info->total_value();
	}
	else{
		return 0;
	}
}

void CUserMissionStrategy::UpdateEventInfo(EEventType eEventType, EEventSubType eEventSubType, gs_dbs_user_info_op_req& db_req, UINT32 iTriggerTime, UINT32 iValue)
{
	_fill_db_req(eEventType, eEventSubType, db_req, iTriggerTime, iValue);
	_update_event_info(GetEventType(eEventType), GetEventSubType(eEventSubType), iTriggerTime, iValue);
}


void CUserMissionStrategy::ClearEvent( EEventType eEventType, EEventSubType eEventSubType, gs_dbs_user_info_op_req& db_req)
{
	//将其last_day_value和total_value都设置成0
	db_user_mission_event* mission_event = _get_event_info(eEventType, eEventSubType);
	if (mission_event)
	{
		mission_event->set_last_day_value(0);
		mission_event->set_total_value(0);
		db_user_mission_event* db_mission_event = db_req.add_missions()->mutable_mission_event();
		db_mission_event->CopyFrom(*mission_event);
	}
}

void CUserMissionStrategy::_fill_db_req(EEventType eEventType, EEventSubType eEventSubType, gs_dbs_user_info_op_req& db_req, UINT32 iTriggerTime, UINT32 iValue)
{
	{//db_req
		db_user_mission_event* db_mission_event = db_req.add_missions()->mutable_mission_event();
		db_mission_event->set_mission_event_type(GetEventType(eEventType));
		db_mission_event->set_mission_event_sub_type(GetEventSubType(eEventSubType));
		db_mission_event->set_last_update_timestamp(iTriggerTime);
		db_user_mission_event* mission_event = _get_event_info(eEventType, eEventSubType);
		if (!mission_event)
		{
			db_mission_event->set_last_day_value(iValue);
			db_mission_event->set_total_value(iValue);
		}
		else
		{
			//UINT32 iResetTime = 0;
			//CSysSettingMgr::GetInstance()->GetIntValue("Global_Reset_Time", iResetTime);
			if (isSameDay(iTriggerTime, mission_event->last_update_timestamp(), CSysSettingMgr::GetInstance()->Global_Reset_Time))
			{
				db_mission_event->set_last_day_value(iValue + mission_event->last_day_value());
			}
			else
			{
				db_mission_event->set_last_day_value(iValue);
			}
			db_mission_event->set_total_value(iValue + mission_event->total_value());
		}
	}
}

void CUserMissionStrategy::_trigger_month_card(UINT32 iRechargeNum, gs_dbs_user_info_op_req& db_req, gs_c_mission_ntf_array& mis_ntf){
	std::vector<UINT32> vecMissionID;
	vecMissionID.push_back(MONTH_CARD_REWARD_MISSION1_ID);
	vecMissionID.push_back(MONTH_CARD_REWARD_MISSION2_ID);
	for (std::vector<UINT32>::iterator it = vecMissionID.begin(); it != vecMissionID.end(); ++it)
	{
		EEventType eEventType = EEventType_Min;
		if (*it == MONTH_CARD_REWARD_MISSION1_ID)
		{
			eEventType = EEventType_Recharge_Mission_Month_Card1;
		}
		else if(*it == MONTH_CARD_REWARD_MISSION2_ID)
		{
			eEventType = EEventType_Recharge_Mission_Month_Card2;
		}
		MONTH_CARD_STATUS status = GetMissionMonthCardStatus(*it);
		if (status == MONTH_CARD_PAY || status == MONTH_CARD_BUY)//购买或者续费
		{
			db_user_mission_event* mission_event = NULL;
			mission_event = _get_event_info(eEventType, EEventSubType_None);

			if (!mission_event)
			{
				mission_event = m_mission_event_arr->add_events();
				mission_event->set_mission_event_type(GetEventType(eEventType));
				mission_event->set_mission_event_sub_type(GetEventSubType(EEventSubType_None));
			}
			//当前充值数目
			UINT32 iCurRechargeNum = mission_event->total_value();
			UINT32 iNeedRechargeNum = 0;
			if (eEventType == EEventType_Recharge_Mission_Month_Card1)
			{
				ASSERT(CSysSettingMgr::GetInstance()->GetIntValue("Month_Card1_Recharge_Num", iNeedRechargeNum));
			}
			else if(eEventType == EEventType_Recharge_Mission_Month_Card2)
			{
				ASSERT(CSysSettingMgr::GetInstance()->GetIntValue("Month_Card2_Recharge_Num", iNeedRechargeNum));
			}

			if (iCurRechargeNum + iRechargeNum >= iNeedRechargeNum)//充值达到数目
			{
				mission_event->set_last_update_timestamp(Now());
				mission_event->set_total_value(0);

				//将客户端购买或者续费任务删除
				gs_c_mission_ntf_array::mission_opt* opt = mis_ntf.add_mis_opts();
				opt->set_opt(gs_c_mission_ntf_array::mission_opt::DEL);
				user_mission* user_mis = opt->mutable_mis();
				if(status == MONTH_CARD_PAY)
				{
					if(*it == MONTH_CARD_REWARD_MISSION1_ID)
						user_mis->set_mission_id(MONTH_CARD_PAY_MISSION1_ID);
					else if(*it == MONTH_CARD_REWARD_MISSION2_ID)
						user_mis->set_mission_id(MONTH_CARD_PAY_MISSION2_ID);
				}
				else if(status == MONTH_CARD_BUY)
				{
					if(*it == MONTH_CARD_REWARD_MISSION1_ID)
						user_mis->set_mission_id(MONTH_CARD_BUY_MISSION1_ID);
					else if(*it == MONTH_CARD_REWARD_MISSION2_ID)
						user_mis->set_mission_id(MONTH_CARD_BUY_MISSION2_ID);
				}
				user_mis->set_mission_status(user_mission::STATUS_DELED);

				//给予奖励任务
				if (status == MONTH_CARD_BUY)
				{
					opt = mis_ntf.add_mis_opts();
					opt->set_opt(gs_c_mission_ntf_array::mission_opt::ADD);
					user_mis = opt->mutable_mis();
					if(*it == MONTH_CARD_REWARD_MISSION1_ID)
						user_mis->set_mission_id(MONTH_CARD_REWARD_MISSION1_ID);
					else if(*it == MONTH_CARD_REWARD_MISSION2_ID)
						user_mis->set_mission_id(MONTH_CARD_REWARD_MISSION2_ID);
					user_mis->set_mission_status(user_mission::STATUS_CAN_ATTACH);
				}
			}
			else
			{
				mission_event->set_total_value(iCurRechargeNum + iRechargeNum);

				//充值进度通知客户端
				gs_c_mission_ntf_array::mission_opt* opt = mis_ntf.add_mis_opts();
				opt->set_opt(gs_c_mission_ntf_array::mission_opt::UPDATE);
				user_mission* user_mis = opt->mutable_mis();
				user_mis->set_mission_id(*it);
				user_mis->set_mission_status(user_mission::STATUS_IN);
				user_mission::mission_event_need* event_need = user_mis->add_event_needs();
				event_need->set_event_type(TransformType(GetEventType(eEventType)));
				event_need->set_event_sub_type(user_mission::NONE);
				event_need->set_cur_val(iCurRechargeNum + iRechargeNum);
				event_need->set_need_val(iNeedRechargeNum);

			}
			std::pair<EEventType, EEventSubType> key = std::make_pair(eEventType, EEventSubType_None);
			m_mapEventType2Event[key] = mission_event;

			db_user_mission_event* db_mission_event = db_req.add_missions()->mutable_mission_event();
			db_mission_event->CopyFrom(*mission_event);
		}
	}
}

bool CUserMissionStrategy::_trigger_mission_by_condition( EEventType eEventType, EEventSubType eEventSubType, gs_c_mission_ntf_array& mis_ntf, UINT32 iValue)
{
	//触发进入条件
	std::vector<CMission*> vecEnterMis;
	if (eEventType == EEventType_Level_Up)
	{
		CCondition_Level::EConditionLevelType eLevelType;
		switch (eEventSubType)
		{
		case EEventSubType_Player_Level:
			eLevelType = CCondition_Level::EConditionLevelType::EConditionLevelType_Player_Level;
			break;
		case EEventSubType_Vip_Level:
			eLevelType = CCondition_Level::EConditionLevelType::EConditionLevelType_Vip_Level;
			break;
		default:
			ASSERT(false);
			return false;
		}
		UINT32 iUserLevel = m_pUser->GetLevel();
		for (UINT32 iLevel = iUserLevel; iLevel > iUserLevel - iValue; --iLevel)
		{
			std::vector<CMission*>* vecMissions = CMissionMgr::GetInstance()->GetMissionsByLevelCondition(eLevelType, iLevel);
			if(vecMissions)
				std::copy(vecMissions->begin(), vecMissions->end(), std::back_inserter(vecEnterMis));
		}

		if (vecEnterMis.size())
		{
			for (std::vector<CMission*>::iterator iter = vecEnterMis.begin(); iter != vecEnterMis.end(); ++iter)
			{
				bool deal = true;
				if ((*iter)->iMisChainId)
				{
					//判断当前链的最后领取的任务id是否是此任务的前置id
					UINT32 iRewardID = GetChainLastRewardID((*iter)->iMisChainId);
					if (iRewardID != (*iter)->iPreMissionId)
					{
						deal = false;
					}
				}

				if(deal)
				{
					if(IsConditionsSatisfy((*iter)->enterCondition, m_pUser->UserID()))//根据当前等级直接判断能否进入
					{
						gs_c_mission_ntf_array::mission_opt* opt = mis_ntf.add_mis_opts();
						opt->set_opt(gs_c_mission_ntf_array::mission_opt::ADD);
						opt->mutable_mis()->set_mission_id((*iter)->iMissionId);

						std::vector<ICondition*> vecCondition;
						(*iter)->GetFinishCondition(vecCondition);

						opt->mutable_mis()->set_mission_status(user_mission::STATUS_IN);
						bool is_all_satisfy = true;
						for (std::vector<ICondition*>::iterator it = vecCondition.begin(); it != vecCondition.end(); ++it)
						{
							CCondition_Event* pConditionEvent = dynamic_cast<CCondition_Event*>(*it);
							ASSERT(pConditionEvent);
							if(!pConditionEvent)
								return false;

							EEventType event_type =  pConditionEvent->GetEventType();
							EEventSubType event_sub_type = pConditionEvent->GetEventSubType();
							UINT32 iNeedValue = pConditionEvent->GetNeedNum();
							UINT32 iCurValue = GetMissionEventValueByEventType(event_type, event_sub_type, (*iter)->eType);

							if (pConditionEvent->GetEventType() == EEventType_Level_Up)
							{
								iCurValue += iValue;
							}

							user_mission::mission_event_need* event_need = opt->mutable_mis()->add_event_needs();
							event_need->set_event_type(TransformType(GetEventType(event_type)));
							event_need->set_event_sub_type(GetEventSubType(event_sub_type));
							event_need->set_cur_val(iCurValue > iNeedValue ? iNeedValue : iCurValue);
							event_need->set_need_val(iNeedValue);
							if (iCurValue < iNeedValue)
							{
								is_all_satisfy = false;
							}
						}
						if (is_all_satisfy)
						{
							opt->mutable_mis()->set_mission_status(user_mission::STATUS_CAN_ATTACH);
						}
					}
					
					//std::vector<ICondition*> vecEnterCon;
					//(*iter)->GetEnterCondition(vecEnterCon);
					//bool can_enter = true;
					//for (std::vector<ICondition*>::iterator it = vecEnterCon.begin(); it != vecEnterCon.end(); ++it)
					//{
					//	if ((*it)->GetConditionType() == EConditionType_Level)
					//	{
					//		CCondition_Level* pLevel = dynamic_cast<CCondition_Level*>(*it);
					//		ASSERT(pLevel);
					//		CHECK_NULL_RETURN_FALSE(pLevel);
					//		UINT32 iLevel = pLevel->GetLevel();
					//		CCondition_Level::EConditionJudgeType eJudgeType = pLevel->GetJudgeType();
					//		UINT32 iPreLevel = GetMissionEventValueByEventType(EEventType_Level_Up, eEventSubType, (*iter)->eType);

					//		if (eJudgeType == CCondition_Level::EConditionJudgeType_E)//=
					//		{
					//			if (iPreLevel + iValue != iLevel)
					//			{
					//				can_enter = false;
					//			}
					//		}
					//		else if(eJudgeType == CCondition_Level::EConditionJudgeType_GTE)//>=
					//		{
					//			if (iPreLevel + iValue < iLevel)
					//			{
					//				can_enter = false;
					//			}
					//		}
					//	}
					//	else
					//	{
					//		if(!(*it)->IsSatisfy(m_pUser->UserID()))
					//			can_enter = false;
					//	}
					//}
					
					//能进入计算进度
					/*if (can_enter)
					{
						
					}*/
				}

			}
		}
	}

	//获取该事件涉及的任务列表
	std::vector<CMission*>* pFinishMission = CMissionMgr::GetInstance()->GetMissionsByEventType(eEventType, eEventSubType);
	if (pFinishMission)
	{
		std::vector<CMission*>::iterator it = pFinishMission->begin();
		for (; it != pFinishMission->end(); ++it)
		{
			//查看是否处理过了
			if(std::find(vecEnterMis.begin(), vecEnterMis.end(), *it) != vecEnterMis.end())
				continue;

			if((*it)->iMisChainId)//如果存在链，查看当前该链是否是当前任务(一个事件有可能触发一个链上的很多任务，只处理链上当前任务)
			{
				MissionInfo info;
				if(GetChainCurMission((*it)->iMisChainId, info))
				{
					ASSERT(info.pMission);
					if (info.pMission->iMissionId !=  (*it)->iMissionId)
					{
						continue;
					}
				}
				else{
					continue;//该链上没找到当前任务
				}
				
			}
			//查看任务状态
			user_mission::MISSION_STATUS mission_status = CMissionMgr::GetInstance()->GetMissionStatus(*it, m_pUser->UserID());//根据之前等级判断其状态
			if (mission_status == user_mission::STATUS_IN)//进行中
			{
				std::vector<ICondition*> vecCondition;
				(*it)->GetFinishCondition(vecCondition);
				if(vecCondition.size() == 1)
				{
					CCondition_Event* pConditionEvent = dynamic_cast<CCondition_Event*>(vecCondition[0]);
					ASSERT(pConditionEvent);
					CHECK_NULL_RETURN_FALSE(pConditionEvent);

					EEventType event_type =  pConditionEvent->GetEventType();
					EEventSubType event_sub_type = pConditionEvent->GetEventSubType();
					ASSERT(event_type  == eEventType && event_sub_type == eEventSubType);
					UINT32 iNeedValue = pConditionEvent->GetNeedNum();
					
					
					UINT32 iCurValue = GetMissionEventValueByEventType(event_type, event_sub_type, (*it)->eType);

					gs_c_mission_ntf_array::mission_opt* opt = mis_ntf.add_mis_opts();
					opt->set_opt(gs_c_mission_ntf_array::mission_opt::UPDATE);
					opt->mutable_mis()->set_mission_id((*it)->iMissionId);

					if (iCurValue + iValue >= iNeedValue)//完成
					{
						opt->mutable_mis()->set_mission_status(user_mission::STATUS_CAN_ATTACH);
					}
					else
					{
						opt->mutable_mis()->set_mission_status(user_mission::STATUS_IN);
						user_mission::mission_event_need* event_need = opt->mutable_mis()->add_event_needs();
						event_need->set_event_type(TransformType(GetEventType(event_type)));
						event_need->set_event_sub_type(GetEventSubType(event_sub_type));
						event_need->set_cur_val(iCurValue + iValue);
					}
				}
				else{
					std::vector<CCondition_Event*> vecNotSatisfyCondition;//不满足条件集合
					for (std::vector<ICondition*>::iterator iter = vecCondition.begin(); iter != vecCondition.end(); ++iter)
					{
						CCondition_Event* pConditionEvent = dynamic_cast<CCondition_Event*>(*iter);
						ASSERT(pConditionEvent);
						CHECK_NULL_RETURN_FALSE(pConditionEvent);

						if(!pConditionEvent->IsSatisfy(m_pUser->UserID(), (*it)->eType))
						{
							vecNotSatisfyCondition.push_back(pConditionEvent);
						}
					}
					CCondition_Event* pConditionEvent = NULL; 
					for (std::vector<CCondition_Event*>::iterator iter = vecNotSatisfyCondition.begin(); iter != vecNotSatisfyCondition.end(); ++iter)
					{
						if ((*iter)->GetEventType() == eEventType && (*iter)->GetEventSubType() == eEventSubType)
						{
							pConditionEvent = *iter;
						}
					}
					if(pConditionEvent)
					{
						gs_c_mission_ntf_array::mission_opt* opt = mis_ntf.add_mis_opts();
						opt->set_opt(gs_c_mission_ntf_array::mission_opt::UPDATE);
						opt->mutable_mis()->set_mission_id((*it)->iMissionId);
						UINT32 iCurValue = GetMissionEventValueByEventType(eEventType, eEventSubType, (*it)->eType);
						UINT32 iNeedValue = pConditionEvent->GetNeedNum();    
						if (vecNotSatisfyCondition.size() == 1)
						{
							if (iCurValue + iValue >= iNeedValue)//该条件也满足
							{
								opt->mutable_mis()->set_mission_status(user_mission::STATUS_CAN_ATTACH);
								continue;
							}
						}
						//将该条件的进度发给客户端
						UINT32 iUpdateNum = iCurValue + iValue >= iNeedValue ? iNeedValue : iCurValue + iValue; 
						opt->mutable_mis()->set_mission_status(user_mission::STATUS_IN);
						user_mission::mission_event_need* event_need = opt->mutable_mis()->add_event_needs();
						event_need->set_event_type(TransformType(GetEventType(eEventType)));
						event_need->set_event_sub_type(GetEventSubType(eEventSubType));
						event_need->set_cur_val(iUpdateNum);
						event_need->set_need_val(iNeedValue);
					}
				}
			}
		}
	}
	return true;
}

bool CUserMissionStrategy::_cal_chapter_fb_three_star(EEventType eEventType, EEventSubType eEventSubType)
{
	if (EEventType_Complete_Chapter_FB_Three_Star == eEventType)
	{
		std::pair<EEventType, EEventSubType> key = std::make_pair(eEventType, eEventSubType);
		std::map<std::pair<EEventType, EEventSubType>, db_user_mission_event*>::iterator it = m_mapEventType2Event.find(key);
		if (it == m_mapEventType2Event.end())
		{
			switch(eEventSubType)
			{
			case EEventSubType_HuaXia://三星通关华夏所有副本
				{
					//得到所有三星华夏主线,支线副本id
					std::vector<UINT32>* pVecIDS = CTaskMgr::GetInstance()->GetMainBranchSubTaskIDsByChapterID((UINT32)EArea_Huaxia);
					ASSERT(pVecIDS);

					//首先看最后一个是否完成
					if(!m_pUser->IsSubTaskDone(*(pVecIDS->end() - 1)))
						return false;
					//判断是否所有都是三星
					for (std::vector<UINT32>::iterator iter = pVecIDS->begin(); iter != pVecIDS->end(); ++iter)
					{
						db_user_sub_task* sub_task = m_pUser->GetSubTask(*iter);
						if (!sub_task)
						{
							return false;
						}
						UINT32 iStar = sub_task->star();
						if (3 != iStar)
						{
							return false;//未完成
						}
					}
					return true;//完成
				}
				break;
			default:
				ASSERT(false);
				return false;
			}
			return false;//已经完成
		}
	}
	return false;
}

void CUserMissionStrategy::_add_count_event()
{
	//添加等级事件，方便等级事件触发时，获取之前的等级(等级事件无需存储数据库，只需同步内存即可)
	db_user_mission_event* mission_event = m_mission_event_arr->add_events();
	mission_event->set_mission_event_type(GetEventType(EEventType_Level_Up));
	mission_event->set_mission_event_sub_type(GetEventSubType(EEventSubType_Player_Level));
	mission_event->set_total_value(m_pUser->GetLevel());
	std::pair<EEventType, EEventSubType> key = std::make_pair(EEventType_Level_Up, EEventSubType_Player_Level);
	//if(m_mapEventType2Event.find(key) ==  m_mapEventType2Event.end())
		m_mapEventType2Event[key] = mission_event;

	//vip等级
	mission_event = m_mission_event_arr->add_events();
	mission_event->set_mission_event_type(GetEventType(EEventType_Level_Up));
	mission_event->set_mission_event_sub_type(GetEventSubType(EEventSubType_Vip_Level));
	mission_event->set_total_value(m_pUser->GetVipLevel());
	key = std::make_pair(EEventType_Level_Up, EEventSubType_Vip_Level);
	//if(m_mapEventType2Event.find(key) ==  m_mapEventType2Event.end())
		m_mapEventType2Event[key] = mission_event;

	//英雄收集
	mission_event = m_mission_event_arr->add_events();
	mission_event->set_mission_event_type(GetEventType(EEventType_Hero_Collect));
	mission_event->set_mission_event_sub_type(GetEventSubType(EEventSubType_None));
	mission_event->set_total_value(m_pUser->GetHeroNum());
	key = std::make_pair(EEventType_Hero_Collect, EEventSubType_None);
	//if (m_mapEventType2Event.find(key) == m_mapEventType2Event.end())
		m_mapEventType2Event[key] = mission_event;
}





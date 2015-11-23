#include "mission_mgr.h"
#include "db_mgr.h"
#include "db_sql.h"
#include "def.h"
#include "str.h"
#include "condition.h"
#include "event.h"
bool CMissionMgr::Init()
{
	MYSQL* con = CDBMgr::GetInstance()->GetConnection();

	char* sql = SELECT_MISSION;
	int code = CDBMgr::Query(con, sql, strlen(sql));

	if (0 != code)
	{
		printf("query failed![%s]\n", sql);
		return false;
	}
	MYSQL_RES* res = mysql_store_result(con);

	{
		std::string need_fields[] = {"mission_id", "pre_mission_id", "mission_chain_id", "mission_chain_type", "mission_type", "show_condition_arr", "enter_condition_arr", "finish_condition_arr", "mission_reward"};
		std::vector<std::string> v_task_need_field(need_fields, need_fields + sizeof(need_fields)/sizeof(need_fields[0]));
		ASSERT(CDBMgr::check_fields(res, v_task_need_field));
	}

	MYSQL_ROW row;
	while (row = mysql_fetch_row(res))
	{
		int col = 0;
		CMission* pMission = new CMission;

		pMission->iMissionId = atoi(row[col++]);
		pMission->iPreMissionId = atoi(row[col++]);
		pMission->iMisChainId = atoi(row[col++]);
		pMission->eMissionChainType = (MissionChainType)atoi(row[col++]);
		ASSERT(pMission->eMissionChainType > MissionChainType_Min && pMission->eMissionChainType < MissionChainType_Max);
		pMission->eType = (EMISSIONTYPE)atoi(row[col++]);
		ASSERT(pMission->eType > EMISSIONTYPE_MIN && pMission->eType < EMISSIONTYPE_MAX);

		//解析显示条件(玩家能看到该任务）
		if (row[col++])
		{
			char* buf = ANSIToUTF8(row[col - 1]);
			bool r = ParseCondition(buf, pMission->showCondition);
			ASSERT(r);
		}

		//解析进入条件(有些任务能看到但不能进入 比如体力套餐不在领取时间段)
		if (row[col++])
		{
			char* buf = ANSIToUTF8(row[col - 1]);
			bool r = ParseCondition(buf, pMission->enterCondition);
			ASSERT(r);
			free(buf);

			//(将升级条件的相关任务提取出来，为了升级更快定位到相关的任务)
			for (UINT32 i = 0; i < MAXNUM_CONDITION; ++i)
			{
				if (pMission->enterCondition[i])
				{
					ICondition* pConditon = pMission->enterCondition[i];
					if (pConditon->GetConditionType() == EConditionType_Level)
					{
						CCondition_Level* conLevel = dynamic_cast<CCondition_Level*>(pConditon);
						ASSERT(conLevel);
						CCondition_Level::EConditionLevelType levelType = conLevel->GetLevelType();
						UINT32 iLevel = conLevel->GetLevel();
						std::pair<CCondition_Level::EConditionLevelType, UINT32> key = std::make_pair(levelType, iLevel);
						m_mapConditionLevel2Missions[key].push_back(pMission);
					}
				}
			}
		}

		//解析完成条件
		if (row[col++])
		{
			char* buf = ANSIToUTF8(row[col - 1]);
			bool r = ParseCondition(buf, pMission->finishCondition);
			ASSERT(r);
			free(buf);
			//将事件相关任务提取 当事件触发时直接定位事件对应的任务
			for (UINT32 i = 0; i < MAXNUM_CONDITION; ++i)
			{
				ICondition* pConditon = pMission->finishCondition[i];
				if (pConditon)
				{
					CCondition_Event* pEvent = dynamic_cast<CCondition_Event*>(pConditon);
					ASSERT(pEvent);
					EEventType		eEventType = pEvent->GetEventType();
					EEventSubType	eEventSubType = pEvent->GetEventSubType();
					ASSERT(eEventType && eEventSubType);
					ASSERT(eEventType > EEventType_Min && eEventType < EEventType_Max);
					if (pMission->eType == EMISSIONTYPE_DAILY)
					{
						ASSERT(eEventType != EEventType_Recharge);//每日中不会出现重置时间
					}
					std::pair<EEventType, EEventSubType> key = std::make_pair(eEventType, eEventSubType);
					m_mapEventType2Missions[key].push_back(pMission);
				}
			}
		}

		//任务奖励
		{
			char* buf = UTF8ToANSI(row[col++]);
			std::vector<std::string> elements;
			split_string(std::string(buf), elements, "|");
			free(buf);
			for (std::vector<std::string>::iterator it = elements.begin(); it != elements.end(); ++it)
			{
				std::vector<int> ele;
				split_string_2i(*it, ele, "-");
				ASSERT(ele.size() == 3);
				stGameElement gameEle((EELEMENTTYPE)ele[0], ele[1], NULL, stGameElement::INCREASE, ele[2]);
				pMission->vecPrizes.push_back(gameEle);
			}
		}

		AddMission(pMission);
	}
	mysql_free_result(res);
	return true;
}

bool CMissionMgr::AddMission(CMission* pMission)
{
	ASSERT(pMission && pMission->iMissionId && m_mapMissionID2Mission.find(pMission->iMissionId) == m_mapMissionID2Mission.end());
	m_mapMissionID2Mission[pMission->iMissionId] = pMission;

	if (0 != pMission->iMissionId)
	{
		m_mapChainId2Missions[pMission->iMisChainId].push_back(pMission);
	}

	//月卡相关
	if (pMission->eType == EMISSIONTYPE_MONTH_CARD_BUY || 
		pMission->eType == EMISSIONTYPE_MONTH_CARD_REWARD || 
		pMission->eType == EMISSIONTYPE_MONTH_CARD_PAY)
	{
		m_mapMissionID2MonthCard[pMission->eType] = pMission;
	}
	return true;
}

std::map<UINT32, CMission*>& CMissionMgr::GetMissions()
{
	return m_mapMissionID2Mission;
}

CMission* CMissionMgr::GetMissionByMissionID(UINT32 iMissionID)
{
	std::map<UINT32, CMission*>::iterator iter = m_mapMissionID2Mission.find(iMissionID);
	if (iter !=  m_mapMissionID2Mission.end())
	{
		return iter->second;
	}
	else
	{
		return NULL;
	}
}

bool CMissionMgr::GetChainNextMission( UINT32 iChainID, UINT32 iCurMissionID, MissionInfo& missionInfo, UINT32 iUserId )
{
	std::vector<CMission*>& list = GetMissionsByChainID(iChainID);

	if (!list.size())
	{
		ASSERT(false);
		return false;
	}

	MissionChainType eChainType = list[0]->eMissionChainType;

	for (std::vector<CMission*>::iterator it = list.begin() ; it != list.end() ; ++it){
		if (iCurMissionID)
		{
			if (*it && (*it)->iPreMissionId == iCurMissionID){
				if(eChainType == MissionChainType_Reset_Daily)//获取链上下一个可见任务(更多的是为了体力套餐)
				{
					while(it != list.end())
					{
						user_mission::MISSION_STATUS mission_status = GetMissionStatus(*it, iUserId);
						if (mission_status == user_mission::STATUS_CAN_NOT_SEE)
						{
							++it;
							continue;
						}
						else
						{
							missionInfo.pMission = *it;
							missionInfo.mission_status = mission_status;
							return true;
						}
					}
					return false;//整个链都没找到直接返回false
				}
				else if(eChainType == MissionChainType_Forever)//领取节点的下一个，不满足不往下找了
				{
					user_mission::MISSION_STATUS mission_status = GetMissionStatus(*it, iUserId);
					if (mission_status == user_mission::STATUS_CAN_NOT_SEE)
					{
						return false;
					}
					else
					{
						missionInfo.pMission = *it;
						missionInfo.mission_status = mission_status;
						return true;
					}
				}
			}
		}
		else
		{//从链的节点开始匹配
			user_mission::MISSION_STATUS mission_status = GetMissionStatus(*it, iUserId);
			if(eChainType == MissionChainType_Reset_Daily)//也是找到第一个可见的(更多的是为了体力套餐)
			{
				if (mission_status == user_mission::STATUS_CAN_NOT_SEE)
				{
					continue;
				}
				else
				{
					missionInfo.pMission = *it;
					missionInfo.mission_status = mission_status;
					return true;
				}
			}
			else if(eChainType == MissionChainType_Forever)
			{
				if (mission_status == user_mission::STATUS_CAN_NOT_SEE)
				{
					return false;
				}
				else
				{
					missionInfo.pMission = *it;
					missionInfo.mission_status = mission_status;
					return true;
				}
			}
		}
	}
	return false;
}

user_mission::MISSION_STATUS CMissionMgr::GetMissionStatus( CMission* pMission, UINT32 iUserId )
{
	//能否看到
	if(IsConditionsSatisfy(pMission->showCondition, iUserId)){
		//能否进入
		if(IsConditionsSatisfy(pMission->enterCondition, iUserId)){
			//能否完成
			if(IsConditionsSatisfy(pMission->finishCondition, iUserId, true, pMission->eType)){
				return user_mission::STATUS_CAN_ATTACH;
			}
			return  user_mission::STATUS_IN;
		}
		return user_mission::STATUS_NOT_TO_TIME;
	}
	return user_mission::STATUS_CAN_NOT_SEE;
}

std::vector<CMission*>* CMissionMgr::GetMissionsByEventType( EEventType eEventType, EEventSubType eEventSubType )
{
	std::pair<EEventType, EEventSubType> key = std::make_pair(eEventType, eEventSubType);
	std::map<std::pair<EEventType, EEventSubType>, std::vector<CMission*>>::iterator it = m_mapEventType2Missions.find(key);
	if (it == m_mapEventType2Missions.end())
	{
		return NULL;
	}
	else{
		return &it->second;
	}
}

std::vector<CMission*>& CMissionMgr::GetMissionsByChainID(UINT32 iChainID)
{
	std::map<UINT32, std::vector<CMission*>>::iterator iter = m_mapChainId2Missions.find(iChainID);
	ASSERT(iter != m_mapChainId2Missions.end());
	return iter->second;
}

std::vector<CMission*>* CMissionMgr::GetMissionsByLevelCondition( CCondition_Level::EConditionLevelType eConditonLevel, UINT32 iLevel )
{
	std::pair<CCondition_Level::EConditionLevelType, UINT32> key = std::make_pair(eConditonLevel, iLevel);
	std::map<std::pair<CCondition_Level::EConditionLevelType, UINT32>, std::vector<CMission*>>::iterator it = m_mapConditionLevel2Missions.find(key);
	if (it == m_mapConditionLevel2Missions.end())
	{
		return NULL;
	}
	else
	{
		return &it->second;
	}
}
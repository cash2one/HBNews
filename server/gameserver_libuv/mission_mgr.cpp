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

		//������ʾ����(����ܿ���������
		if (row[col++])
		{
			char* buf = ANSIToUTF8(row[col - 1]);
			bool r = ParseCondition(buf, pMission->showCondition);
			ASSERT(r);
		}

		//������������(��Щ�����ܿ��������ܽ��� ���������ײͲ�����ȡʱ���)
		if (row[col++])
		{
			char* buf = ANSIToUTF8(row[col - 1]);
			bool r = ParseCondition(buf, pMission->enterCondition);
			ASSERT(r);
			free(buf);

			//(���������������������ȡ������Ϊ���������춨λ����ص�����)
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

		//�����������
		if (row[col++])
		{
			char* buf = ANSIToUTF8(row[col - 1]);
			bool r = ParseCondition(buf, pMission->finishCondition);
			ASSERT(r);
			free(buf);
			//���¼����������ȡ ���¼�����ʱֱ�Ӷ�λ�¼���Ӧ������
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
						ASSERT(eEventType != EEventType_Recharge);//ÿ���в����������ʱ��
					}
					std::pair<EEventType, EEventSubType> key = std::make_pair(eEventType, eEventSubType);
					m_mapEventType2Missions[key].push_back(pMission);
				}
			}
		}

		//������
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

	//�¿����
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
				if(eChainType == MissionChainType_Reset_Daily)//��ȡ������һ���ɼ�����(�������Ϊ�������ײ�)
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
					return false;//��������û�ҵ�ֱ�ӷ���false
				}
				else if(eChainType == MissionChainType_Forever)//��ȡ�ڵ����һ���������㲻��������
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
		{//�����Ľڵ㿪ʼƥ��
			user_mission::MISSION_STATUS mission_status = GetMissionStatus(*it, iUserId);
			if(eChainType == MissionChainType_Reset_Daily)//Ҳ���ҵ���һ���ɼ���(�������Ϊ�������ײ�)
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
	//�ܷ񿴵�
	if(IsConditionsSatisfy(pMission->showCondition, iUserId)){
		//�ܷ����
		if(IsConditionsSatisfy(pMission->enterCondition, iUserId)){
			//�ܷ����
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
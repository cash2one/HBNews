#pragma once
#include "config.h"
#include "condition.h"
#include "event.h"
struct CMission{
	CMission(){
		iMissionId = 0;
		iPreMissionId = 0;
		iMisChainId = 0;
		eType = EMISSIONTYPE_MIN;
		memset(showCondition, 0, sizeof(showCondition));
		memset(enterCondition, 0, sizeof(enterCondition));
		memset(finishCondition, 0, sizeof(finishCondition));
	}

	UINT32 iMissionId;									//����id
	UINT32 iPreMissionId;								//������id
	EMISSIONTYPE eType;									//��������
	UINT32 iMisChainId;									//������id
	MissionChainType eMissionChainType;					//����������
	ICondition* showCondition[MAXNUM_CONDITION];		//��ʾ����
	ICondition* enterCondition[MAXNUM_CONDITION];		//��������
	ICondition* finishCondition[MAXNUM_CONDITION];		//�������
	std::vector<stGameElement>  vecPrizes;				//������

	void GetFinishCondition(std::vector<ICondition*>& vecCondition){
		for (int i = 0; i < MAXNUM_CONDITION; ++i){
			if (!finishCondition[i]){
				continue;
			}
			vecCondition.push_back(finishCondition[i]);
		}
	}
};

struct MissionInfo{
	MissionInfo():pMission(NULL){}

	CMission* pMission;
	user_mission::MISSION_STATUS mission_status;
};

class CMissionMgr
{
public:
	static CMissionMgr* GetInstance(){
		static CMissionMgr singleton;
		return &singleton;
	}

	bool Init();
	bool AddMission(CMission* pMission);
	std::map<UINT32, CMission*>& GetMissions();
	CMission* GetMissionByMissionID(UINT32 iMissionID);
	bool GetChainNextMission(UINT32 iChainID, UINT32 iCurMissionID, MissionInfo& missionInfo, UINT32 iUserId);
	user_mission::MISSION_STATUS GetMissionStatus(CMission* pMission, UINT32 iUserId);
	std::vector<CMission*>* GetMissionsByEventType(EEventType eEventType, EEventSubType eEventSubType);
	std::vector<CMission*>& GetMissionsByChainID(UINT32 iChainID);
	std::vector<CMission*>* GetMissionsByLevelCondition(CCondition_Level::EConditionLevelType eConditonLevel, UINT32 iLevel);
private:
	CMissionMgr(void){};
	std::map<UINT32, CMission*> m_mapMissionID2Mission;							//�����
	std::map<UINT32, std::vector<CMission*>> m_mapChainId2Missions;				//������id->���񼯺�
	std::map<UINT32, CMission*> m_mapMissionID2MonthCard;						//�¿����,�¿���������->CMission
	std::map<std::pair<CCondition_Level::EConditionLevelType, UINT32>, std::vector<CMission*>> m_mapConditionLevel2Missions;	//�ȼ���ص������б�
	std::map<std::pair<EEventType, EEventSubType>, std::vector<CMission*>> m_mapEventType2Missions;//�¼�����->�������¼��������б�
};
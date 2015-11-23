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

	UINT32 iMissionId;									//任务id
	UINT32 iPreMissionId;								//任务链id
	EMISSIONTYPE eType;									//任务类型
	UINT32 iMisChainId;									//任务链id
	MissionChainType eMissionChainType;					//任务链类型
	ICondition* showCondition[MAXNUM_CONDITION];		//显示条件
	ICondition* enterCondition[MAXNUM_CONDITION];		//进入条件
	ICondition* finishCondition[MAXNUM_CONDITION];		//完成条件
	std::vector<stGameElement>  vecPrizes;				//任务奖励

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
	std::map<UINT32, CMission*> m_mapMissionID2Mission;							//任务表
	std::map<UINT32, std::vector<CMission*>> m_mapChainId2Missions;				//任务链id->任务集合
	std::map<UINT32, CMission*> m_mapMissionID2MonthCard;						//月卡相关,月卡任务类型->CMission
	std::map<std::pair<CCondition_Level::EConditionLevelType, UINT32>, std::vector<CMission*>> m_mapConditionLevel2Missions;	//等级相关的任务列表
	std::map<std::pair<EEventType, EEventSubType>, std::vector<CMission*>> m_mapEventType2Missions;//事件类型->包含该事件的任务列表
};
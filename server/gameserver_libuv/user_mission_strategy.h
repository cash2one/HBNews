#ifndef user_mission_strategy_h__
#define user_mission_strategy_h__
#include "dbs_gs_proto.pb.h"
#include "def.h"
#include "mission_mgr.h"
class CUser;
class IUserMissionStrategy{
public:
	virtual ~IUserMissionStrategy() {};
	virtual bool Init(CUser* pUser, db_user_mission_event_array* mission_event_arr, db_user_mission_chain_array* chain_arr, gs_dbs_user_info_op_req& db_req) = 0;
	virtual bool GetMissonInfo(::google::protobuf::RepeatedPtrField< ::user_mission >& rep_user_mission) = 0;
	virtual bool GetChainCurMission(UINT32 iChainID, MissionInfo& missionInfo) =  0;
	virtual UINT32 GetMissionEventValueByEventType(EEventType eEventType, EEventSubType eEventSubType, EMISSIONTYPE eMissionType) = 0;
	virtual bool IsMissionAlreadyGetReward(UINT32 iMissionID) = 0;
	virtual MONTH_CARD_STATUS GetMissionMonthCardStatus(UINT32 iMissionId) = 0;
	virtual bool GetMissionReward(UINT32 iMissionId, gs_dbs_user_info_op_req& db_req, gs_c_get_mission_reward_res& res, GS_C_NTF& ntf) = 0;
	virtual bool TriggerMission(EEventType eEventType, EEventSubType eEventSubType, gs_dbs_user_info_op_req& db_req, GS_C_NTF& ntf, UINT32 iValue = 1) = 0;
	virtual UINT32 GetEventTodayValue(EEventType eEventType, EEventSubType eEventSubType) = 0;
	//add by baomin for invoke
	virtual UINT32 GetEventTotalValue(EEventType eEventType, EEventSubType eEventSubType) = 0;
	//只更新db和内存， 之前的invoke_sub_none还是保留  相当于祭坛信息保存了两份数据 todo 优化掉！！
	virtual void UpdateEventInfo(EEventType eEventType, EEventSubType eEventSubType, gs_dbs_user_info_op_req& db_req, UINT32 iTriggerTime, UINT32 iValue) = 0;

	virtual void ClearEvent(EEventType eEventType, EEventSubType eEventSubType, gs_dbs_user_info_op_req& db_req) = 0;
};

class CUserMissionStrategy : public IUserMissionStrategy
{
public:

	CUserMissionStrategy():m_pUser(NULL),m_mission_event_arr(NULL), m_mission_chain_arr(NULL){};
	virtual bool Init(CUser* pUser, db_user_mission_event_array* mission_event_arr, db_user_mission_chain_array* chain_arr, gs_dbs_user_info_op_req& db_req);
	virtual bool GetMissonInfo(::google::protobuf::RepeatedPtrField< ::user_mission >& rep_user_mission);
	
	virtual bool GetChainCurMission(UINT32 iChainID, MissionInfo& missionInfo);
	
	virtual UINT32 GetMissionEventValueByEventType(EEventType eEventType, EEventSubType eEventSubType, EMISSIONTYPE eMissionType);
	virtual bool IsMissionAlreadyGetReward(UINT32 iMissionID);
	virtual MONTH_CARD_STATUS GetMissionMonthCardStatus(UINT32 iMissionId);
	virtual bool GetMissionReward(UINT32 iMissionId, gs_dbs_user_info_op_req& db_req, gs_c_get_mission_reward_res& res, GS_C_NTF& ntf);
	virtual bool TriggerMission(EEventType eEventType, EEventSubType eEventSubType, gs_dbs_user_info_op_req& db_req, GS_C_NTF& ntf, UINT32 iValue = 1);

	UINT32 GetMonthCardBuyTime(UINT32 iMissionID);
	bool FillUserMission(MissionInfo& missionInfo,  ::google::protobuf::RepeatedPtrField< ::user_mission >& rep_user_mission );
	void CalculateSchedule( ICondition*(& conditions)[MAXNUM_CONDITION], EMISSIONTYPE eType,  ::google::protobuf::RepeatedPtrField< ::user_mission_mission_event_need >* rep_mission_event);
	UINT32 GetChainLastRewardID(UINT32 iChainID);
	UINT32 GetEventTodayValue(EEventType eEventType, EEventSubType eEventSubType);
//add by baomin for invoke
	UINT32 GetEventTotalValue(EEventType eEventType, EEventSubType eEventSubType);
	//只更新db和内存， 之前的invoke_sub_none还是保留  相当于祭坛信息保存了两份数据 todo 优化掉！！
	void UpdateEventInfo(EEventType eEventType, EEventSubType eEventSubType, gs_dbs_user_info_op_req& db_req, UINT32 iTriggerTime, UINT32 iValue);

	virtual void ClearEvent(EEventType eEventType, EEventSubType eEventSubType, gs_dbs_user_info_op_req& db_req);

private:
	void _update_event_info(db_user_mission_event::MISSION_EVENT_TYPE eEventType, db_user_mission_event::MISSION_EVENT_SUB_TYPE eSubType, UINT32 iUpdateTimeStamp, UINT32 iValue = 1);
	void _update_chain_info(UINT32 iChainID, UINT32 iRewardID, UINT32 iUpdateTimeStamp);
	void _trigger_month_card(UINT32 iRechargeNum, gs_dbs_user_info_op_req& db_req, gs_c_mission_ntf_array& mis_ntf);
	bool _trigger_mission_by_condition( EEventType eEventType, EEventSubType eEventSubType, gs_c_mission_ntf_array& mis_ntf, UINT32 iValue);
	void _fill_db_req(EEventType eEventType, EEventSubType eEventSubType, gs_dbs_user_info_op_req& db_req, UINT32 iTriggerTime, UINT32 iValue);
	db_user_mission_event* _get_event_info(EEventType eEventType, EEventSubType eEventSubType);
	bool _cal_chapter_fb_three_star(EEventType eEventType, EEventSubType eEventSubType);//针对特殊的事件类型计算其事件值如（三星通关所有章节任务）
	void _add_count_event();//添加数量相关的事件(战队等级，vip等级，英雄收集)

	CUser* m_pUser;
	db_user_mission_event_array* m_mission_event_arr;
	db_user_mission_chain_array* m_mission_chain_arr; 
	std::map<std::pair<EEventType, EEventSubType>, db_user_mission_event*> m_mapEventType2Event;
	std::map<UINT32, db_user_mission_chain*> m_mapChainId2UserChain;	//任务链id->该链上最后一个领取的任务信息
};

#endif
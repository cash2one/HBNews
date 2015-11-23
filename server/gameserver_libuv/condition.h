#ifndef __CONDITION_H__
#define __CONDITION_H__

#include "def.h"
#include "config.h"
#include "event.h"
enum EConditionType{
	EConditionType_MIN = 0,
	EConditionType_Time = 1,			//时间       格式:type-sub_type(对应枚举 EConditionTimeType)-min-max	 
	EConditionType_Occupation = 2,		//职业       格式:	
	EConditionType_Area = 3,			//势力
	EConditionType_Level = 4,			//等级       格式:type-sub_type(1.战队等级 2.vip)-judge_type(1.>= 2.=)-level
	EConditionType_Event = 5,			//事件		 格式:type-event_type-event_sub_type-num		
	EConditionType_MAX = 6
};

class ICondition{
	public:
		virtual ~ICondition(){};
		virtual bool IsSatisfy(UINT32 iUserID/*, UINT32 iTaskID*/) = 0;
		virtual EConditionType GetConditionType() = 0;
};


//时间相关的！ 一次性的  每天时间段的  每周时间段的
class CCondition_Time: public ICondition{

public:
	/*
	配肉眼时间：
	1 绝对时间  格式： 年/月/日 时:分:秒-年/月/日 时:分:秒
	2.循环时间-年  格式： 月/日-月/日    (时分秒不用填 从sys_setting读出)
	3.循环时间-月  格式： 月中的某日-月中的某日    (时分秒不用填 从sys_setting读出)
	4.循环时间-周  格式:  星期几-星期几  (时分秒不用填 从sys_setting读出)
	5.循环时间-日  格式:  时:分:秒-时:分:秒
	6.循环时间-时  格式:  分:秒-分:秒

	配秒数时间：
	1 绝对时间  格式： 开始时间戳-结束时间戳
	2.循环时间-年  格式： 从每年1月1日0点0分0秒到要配的起始日的0点0分0秒之间的秒数-从每年1月1日0点0分0秒到要配的结束日的0点0分0秒之间的秒数    (时分秒0点0分0秒 是因为时分秒会从sys_setting读出)
	3.循环时间-月  格式： 从每月1日0点0分0秒到要配的起始日的0点0分0秒之间的秒数-从每月1日0点0分0秒到要配的结束日的0点0分0秒之间的秒数    (时分秒0点0分0秒 是因为时分秒会从sys_setting读出)
	4.循环时间-周  格式:  从每周一0点0分0秒到要配的起始日的0点0分0秒之间的秒数-从每周一0点0分0秒到要配的结束日的0点0分0秒之间的秒数  (时分秒0点0分0秒 是因为时分秒会从sys_setting读出)
	5.循环时间-日  格式:  从每日0点0分0秒到要配的起始时间的秒数-从每日0点0分0秒到要配的结束时间的秒数
	6.循环时间-时  格式:  从每时的0分0秒到要配的起始时间的秒数-从每时的0分0秒到要配的结束时间的秒数
	*/
	enum EConditionTimeType{
		EConditionTimeType_MIN = 0,
		EConditionTimeType_Absolute = 1,
		EConditionTimeType_Loop_Year = 2,
		EConditionTimeType_Loop_Month = 3,
		EConditionTimeType_Loop_Week = 4,
		EConditionTimeType_Loop_Day = 5,
		EConditionTimeType_Loop_Hour = 6,
		EConditionTimeType_Forever = 7, //永久开启 不要配置后面两个参数！
		EConditionTimeType_Since_Server = 8,  //从开新服 开始计时
		EConditionTimeType_MAX = 9
	};

	virtual bool IsSatisfy(UINT32 iUserID/*, UINT32 iTaskID*/);

	bool IsSatisfy(UINT32 iUserID,  UINT32 iCheckTimestamp);//非接口函数

	virtual EConditionType GetConditionType(){
		return m_eConditionType;
	}

	bool SetContorl(EConditionTimeType eTimeType, UINT32 iStart, UINT32 iEnd);
private:
	 static const EConditionType m_eConditionType = EConditionType_Time;
	 EConditionTimeType m_eTimeType;
	 UINT32 m_iStart;	//开始时间戳或者秒数
	 UINT32 m_iEnd;	//结束时间戳或者秒数
};

class CCondition_Occupation: public ICondition{
public:

	CCondition_Occupation(){
		memset(m_noLessThanNum, 0 , sizeof m_noLessThanNum);
		memset(m_noMoreThanNum, MAXNUM_HEROINTEAM , sizeof m_noMoreThanNum);
	}

	virtual bool IsSatisfy(UINT32 iUserID/*, UINT32 iTaskID*/);

	virtual EConditionType GetConditionType(){
		return m_eConditionType;
	}

	bool SetContorl(EOccupation eOccupation, UINT8 iNoLessThan, UINT8 iNoMoreThan);

private:
	static const EConditionType m_eConditionType = EConditionType_Occupation;
	//UINT32 m_iTaskID;//所属的任务ID
	UINT8 m_noLessThanNum[EOccupation_MAX-1];
	UINT8 m_noMoreThanNum[EOccupation_MAX-1];
};


////
//class CTaskEnterCondition_NatureProperty: public ITaskEnterCondition{
//public:
//
//	CTaskEnterCondition_NatureProperty(){
//		memset(m_noLessThanNum, 0 , sizeof m_noLessThanNum);
//		memset(m_noMoreThanNum, MAXNUM_HEROINTEAM , sizeof m_noMoreThanNum);
//	}
//
//	virtual bool IsSatisfy(UINT32 iUserID/*, UINT32 iTaskID*/);
//
//	bool SetContorl(ENatureProperty eNatureProperty, UINT8 iNoLessThan, UINT8 iNoMoreThan);
//
//private:
//
//	UINT32 m_iTaskID;//所属的任务ID
//	UINT8 m_noLessThanNum[EOccupation_MAX-1];
//	UINT8 m_noMoreThanNum[EOccupation_MAX-1];
//};


class CCondition_Area: public ICondition{
public:
	
	CCondition_Area(){
		memset(m_noLessThanNum, 0 , sizeof m_noLessThanNum);
		memset(m_noMoreThanNum, MAXNUM_HEROINTEAM , sizeof m_noMoreThanNum);
	}

	virtual bool IsSatisfy(UINT32 iUserID/*, UINT32 iTaskID*/);

	virtual EConditionType GetConditionType(){
		return m_eConditionType;
	}

	bool SetContorl(EArea eArea, UINT8 iNoLessThan, UINT8 iNoMoreThan);

private:
	static const EConditionType m_eConditionType = EConditionType_Area;
	//UINT32 m_iTaskID;//所属的任务ID
	UINT8 m_noLessThanNum[EArea_MAX-1];
	UINT8 m_noMoreThanNum[EArea_MAX-1];
};


class CCondition_Level : public ICondition{
public:
	enum EConditionLevelType{
		EConditionLevelType_MIN = 0,
		EConditionLevelType_Player_Level = 1,		//玩家等级
		EConditionLevelType_Vip_Level = 2,			//vip等级
		EConditionLevelType_MAX = 3,
	};

	enum EConditionJudgeType{
		EConditionJudgeType_MIN = 0,
		EConditionJudgeType_GTE = 1,	//大于等于
		EConditionJudgeType_E  = 2,		//等于
		EConditionJudgeType_MAX = 3		
	};

	virtual bool IsSatisfy(UINT32 iUserID/*, UINT32 iTaskID*/);

	virtual EConditionType GetConditionType(){
		return m_eConditionType;
	}

	bool SetContorl(EConditionLevelType eLevelType, EConditionJudgeType eJudgeType, UINT32 iLevel);
	
	EConditionLevelType GetLevelType()
	{
		return m_LevelType;
	}
	UINT32 GetLevel()
	{
		return m_iLevel;
	}
	EConditionJudgeType GetJudgeType()
	{
		return m_JudgeType;
	}
private:
	static const EConditionType m_eConditionType = EConditionType_Level;
	EConditionLevelType m_LevelType;
	EConditionJudgeType m_JudgeType;
	UINT32 m_iLevel;
};

class CCondition_Event : public ICondition{
public:
	virtual bool IsSatisfy(UINT32 iUserID/*, UINT32 iTaskID*/){return false;};
	
	bool IsSatisfy(UINT32 iUserID, EMISSIONTYPE eMissionType);

	virtual EConditionType GetConditionType(){
		return m_eConditionType;
	}
	UINT32 GetNeedNum()
	{
		return m_iValue;
	}
	EEventType GetEventType()
	{
		return m_eventType;
	}
	EEventSubType GetEventSubType()
	{
		return m_eventSubType;
	}
	bool SetContorl(EEventType eEventType, EEventSubType eEventSubType, UINT32 iValue);
private:
	static const EConditionType m_eConditionType = EConditionType_Event;
	EEventType m_eventType;
	EEventSubType m_eventSubType;
	UINT32 m_iValue;
};



//时间条件不存在或者任意满足一个就行
//其他条件只要有一个不满足就算不满足
bool IsConditionsSatisfy(ICondition*(& conditions)[MAXNUM_CONDITION], UINT32 iUserID, bool useMissionType = false, EMISSIONTYPE eMissionType = EMISSIONTYPE_MIN);
bool IsTimeConditionsSatisfy(ICondition*(& conditions)[MAXNUM_CONDITION], UINT32 iUserID, bool use_check_timestamp = false , UINT32 check_timestamp = 0);

ICondition* GetFirstCondition(ICondition*(& conditions)[MAXNUM_CONDITION], EConditionType eConditionType);

//从字符串中解析条件 condition_str为ansi格式的
bool ParseCondition(char* condition_str, ICondition*(& conditions)[MAXNUM_CONDITION]);

//只解析时间条件
bool ParseTimeCondition(char* condition_str, ICondition*(& conditions)[MAXNUM_CONDITION]);


#endif
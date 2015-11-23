#ifndef __TASK_ENTER_CONDITION_H__
#define __TASK_ENTER_CONDITION_H__


#include "task_mgr.h"
#include "user_mgr.h"

#include "hero_mgr.h"
#include "user_hero_mgr.h"

class ITaskEnterCondition{
	public:
		virtual ~ITaskEnterCondition(){};
		virtual bool IsSatisfy(UINT32 iUserID/*, UINT32 iTaskID*/) = 0;
};


//时间相关的！ 一次性的  每天时间段的  每周时间段的
class CTaskEnterCondition_Time: public ITaskEnterCondition{

public:

	enum ETimeLimitType{
		ETimeLimitType_Absolute = 1,
		ETimeLimitType_Loop_Day = 2,
		ETimeLimitType_Loop_Week = 3,
		ETimeLimitType_Loop_Month = 4
	};

	virtual bool IsSatisfy(UINT32 iUserID/*, UINT32 iTaskID*/){
		return false;
	}

private:
	 ETimeLimitType m_eType;
	 
};


class CTaskEnterCondition_Occupation: public ITaskEnterCondition{
public:

	CTaskEnterCondition_Occupation(){
		memset(m_noLessThanNum, 0 , sizeof m_noLessThanNum);
		memset(m_noMoreThanNum, MAXNUM_HEROINTEAM , sizeof m_noMoreThanNum);
	}

	virtual bool IsSatisfy(UINT32 iUserID/*, UINT32 iTaskID*/);

	bool SetContorl(EOccupation eOccupation, UINT8 iNoLessThan, UINT8 iNoMoreThan);

private:

	UINT32 m_iTaskID;//所属的任务ID
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


class CTaskEnterCondition_Area: public ITaskEnterCondition{
public:

	CTaskEnterCondition_Area(){
		memset(m_noLessThanNum, 0 , sizeof m_noLessThanNum);
		memset(m_noMoreThanNum, MAXNUM_HEROINTEAM , sizeof m_noMoreThanNum);
	}

	virtual bool IsSatisfy(UINT32 iUserID/*, UINT32 iTaskID*/);

	bool SetContorl(EArea eArea, UINT8 iNoLessThan, UINT8 iNoMoreThan);

private:

	UINT32 m_iTaskID;//所属的任务ID
	UINT8 m_noLessThanNum[EArea_MAX-1];
	UINT8 m_noMoreThanNum[EArea_MAX-1];
};

#endif
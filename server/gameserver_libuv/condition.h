#ifndef __CONDITION_H__
#define __CONDITION_H__

#include "def.h"
#include "config.h"
#include "event.h"
enum EConditionType{
	EConditionType_MIN = 0,
	EConditionType_Time = 1,			//ʱ��       ��ʽ:type-sub_type(��Ӧö�� EConditionTimeType)-min-max	 
	EConditionType_Occupation = 2,		//ְҵ       ��ʽ:	
	EConditionType_Area = 3,			//����
	EConditionType_Level = 4,			//�ȼ�       ��ʽ:type-sub_type(1.ս�ӵȼ� 2.vip)-judge_type(1.>= 2.=)-level
	EConditionType_Event = 5,			//�¼�		 ��ʽ:type-event_type-event_sub_type-num		
	EConditionType_MAX = 6
};

class ICondition{
	public:
		virtual ~ICondition(){};
		virtual bool IsSatisfy(UINT32 iUserID/*, UINT32 iTaskID*/) = 0;
		virtual EConditionType GetConditionType() = 0;
};


//ʱ����صģ� һ���Ե�  ÿ��ʱ��ε�  ÿ��ʱ��ε�
class CCondition_Time: public ICondition{

public:
	/*
	������ʱ�䣺
	1 ����ʱ��  ��ʽ�� ��/��/�� ʱ:��:��-��/��/�� ʱ:��:��
	2.ѭ��ʱ��-��  ��ʽ�� ��/��-��/��    (ʱ���벻���� ��sys_setting����)
	3.ѭ��ʱ��-��  ��ʽ�� ���е�ĳ��-���е�ĳ��    (ʱ���벻���� ��sys_setting����)
	4.ѭ��ʱ��-��  ��ʽ:  ���ڼ�-���ڼ�  (ʱ���벻���� ��sys_setting����)
	5.ѭ��ʱ��-��  ��ʽ:  ʱ:��:��-ʱ:��:��
	6.ѭ��ʱ��-ʱ  ��ʽ:  ��:��-��:��

	������ʱ�䣺
	1 ����ʱ��  ��ʽ�� ��ʼʱ���-����ʱ���
	2.ѭ��ʱ��-��  ��ʽ�� ��ÿ��1��1��0��0��0�뵽Ҫ�����ʼ�յ�0��0��0��֮�������-��ÿ��1��1��0��0��0�뵽Ҫ��Ľ����յ�0��0��0��֮�������    (ʱ����0��0��0�� ����Ϊʱ������sys_setting����)
	3.ѭ��ʱ��-��  ��ʽ�� ��ÿ��1��0��0��0�뵽Ҫ�����ʼ�յ�0��0��0��֮�������-��ÿ��1��0��0��0�뵽Ҫ��Ľ����յ�0��0��0��֮�������    (ʱ����0��0��0�� ����Ϊʱ������sys_setting����)
	4.ѭ��ʱ��-��  ��ʽ:  ��ÿ��һ0��0��0�뵽Ҫ�����ʼ�յ�0��0��0��֮�������-��ÿ��һ0��0��0�뵽Ҫ��Ľ����յ�0��0��0��֮�������  (ʱ����0��0��0�� ����Ϊʱ������sys_setting����)
	5.ѭ��ʱ��-��  ��ʽ:  ��ÿ��0��0��0�뵽Ҫ�����ʼʱ�������-��ÿ��0��0��0�뵽Ҫ��Ľ���ʱ�������
	6.ѭ��ʱ��-ʱ  ��ʽ:  ��ÿʱ��0��0�뵽Ҫ�����ʼʱ�������-��ÿʱ��0��0�뵽Ҫ��Ľ���ʱ�������
	*/
	enum EConditionTimeType{
		EConditionTimeType_MIN = 0,
		EConditionTimeType_Absolute = 1,
		EConditionTimeType_Loop_Year = 2,
		EConditionTimeType_Loop_Month = 3,
		EConditionTimeType_Loop_Week = 4,
		EConditionTimeType_Loop_Day = 5,
		EConditionTimeType_Loop_Hour = 6,
		EConditionTimeType_Forever = 7, //���ÿ��� ��Ҫ���ú�������������
		EConditionTimeType_Since_Server = 8,  //�ӿ��·� ��ʼ��ʱ
		EConditionTimeType_MAX = 9
	};

	virtual bool IsSatisfy(UINT32 iUserID/*, UINT32 iTaskID*/);

	bool IsSatisfy(UINT32 iUserID,  UINT32 iCheckTimestamp);//�ǽӿں���

	virtual EConditionType GetConditionType(){
		return m_eConditionType;
	}

	bool SetContorl(EConditionTimeType eTimeType, UINT32 iStart, UINT32 iEnd);
private:
	 static const EConditionType m_eConditionType = EConditionType_Time;
	 EConditionTimeType m_eTimeType;
	 UINT32 m_iStart;	//��ʼʱ�����������
	 UINT32 m_iEnd;	//����ʱ�����������
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
	//UINT32 m_iTaskID;//����������ID
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
//	UINT32 m_iTaskID;//����������ID
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
	//UINT32 m_iTaskID;//����������ID
	UINT8 m_noLessThanNum[EArea_MAX-1];
	UINT8 m_noMoreThanNum[EArea_MAX-1];
};


class CCondition_Level : public ICondition{
public:
	enum EConditionLevelType{
		EConditionLevelType_MIN = 0,
		EConditionLevelType_Player_Level = 1,		//��ҵȼ�
		EConditionLevelType_Vip_Level = 2,			//vip�ȼ�
		EConditionLevelType_MAX = 3,
	};

	enum EConditionJudgeType{
		EConditionJudgeType_MIN = 0,
		EConditionJudgeType_GTE = 1,	//���ڵ���
		EConditionJudgeType_E  = 2,		//����
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



//ʱ�����������ڻ�����������һ������
//��������ֻҪ��һ����������㲻����
bool IsConditionsSatisfy(ICondition*(& conditions)[MAXNUM_CONDITION], UINT32 iUserID, bool useMissionType = false, EMISSIONTYPE eMissionType = EMISSIONTYPE_MIN);
bool IsTimeConditionsSatisfy(ICondition*(& conditions)[MAXNUM_CONDITION], UINT32 iUserID, bool use_check_timestamp = false , UINT32 check_timestamp = 0);

ICondition* GetFirstCondition(ICondition*(& conditions)[MAXNUM_CONDITION], EConditionType eConditionType);

//���ַ����н������� condition_strΪansi��ʽ��
bool ParseCondition(char* condition_str, ICondition*(& conditions)[MAXNUM_CONDITION]);

//ֻ����ʱ������
bool ParseTimeCondition(char* condition_str, ICondition*(& conditions)[MAXNUM_CONDITION]);


#endif
#include "condition.h"

#include "task_mgr.h"
#include "user_mgr.h"

#include "hero_mgr.h"
#include "user_hero_mgr.h"
#include "str.h"

bool CCondition_Occupation::IsSatisfy( UINT32 iUserID/*, UINT32 iTaskID*/ )
{
	/* //不会出现 本对象在构造之前 已经判断m_iTaskID的有效性！
	CTask* pTask = CTaskMgr::GetInstance()->GetTaskByTaskID(m_iTaskID);
	if(!pTask){
	return false;
	}
	*/

	CUser* pUser = CUserMgr::GetInstance()->GetUserByUserID(iUserID);
	if (!pUser)
	{
		return false;
	}

	//注意索引和职业的关系
	UINT8 eachOccupationNum[EOccupation_MAX-1] = {0};
	//数组引用
	UINT32 (&curTeam)[MAXNUM_HEROINTEAM] = pUser->m_hero_team.m_Teams[pUser->m_hero_team.m_curTeamIdx];
	for (int i = 0 ; i < MAXNUM_HEROINTEAM ; ++i)
	{
		if (!curTeam[i])
		{
			continue;
		}

		//判断
		/*
		CUserHero* pHeroInstance = CUserHeroMgr::GetInstance()->GetHeroInstanceByHeroInstanceID(curTeam[i]);
		if (!pHeroInstance)
		{
			//todo log error!
			continue;
		}
		*/

		if(!pUser->GetHero(curTeam[i])){
			ASSERT(false);
			continue;
		}

		CHeroCfg* pHero = CHeroMgr::GetInstance()->GetHeroByHeroID(curTeam[i]);
		if (!pHero)
		{
			//todo log error!
			continue;
		}
		//注意索引和职业的关系
		++eachOccupationNum[pHero->eOccupation-1];
	}

	//
	for (int i = EOccupation_Warrior ; i < EOccupation_MAX ; ++i)
	{
		if (eachOccupationNum[i-1] < m_noLessThanNum[i-1] || eachOccupationNum[i-1] > m_noMoreThanNum[i-1])
		{
			return false;
		}
	}

	return true;
}

bool CCondition_Occupation::SetContorl( EOccupation eOccupation, UINT8 iNoLessThan, UINT8 iNoMoreThan )
{
	//目前不判断是否重复设置过 先允许重复设置！

	if ((iNoLessThan < 0 || iNoLessThan > MAXNUM_HEROINTEAM))// >=某个数
	{
		return false;
	}

	if ((iNoMoreThan < 0 || iNoMoreThan > MAXNUM_HEROINTEAM))// <=某个数
	{
		return false;
	}

	m_noLessThanNum[eOccupation-1] = iNoLessThan;
	m_noMoreThanNum[eOccupation-1] = iNoMoreThan;

	return true;
}


//bool CTaskEnterCondition_NatureProperty::IsSatisfy( UINT32 iUserID/*, UINT32 iTaskID*/ )
//{
//	/* //不会出现 本对象在构造之前 已经判断m_iTaskID的有效性！
//	CTask* pTask = CTaskMgr::GetInstance()->GetTaskByTaskID(m_iTaskID);
//	if(!pTask){
//	return false;
//	}
//	*/
//
//	CUser* pUser = CUserMgr::GetInstance()->GetUserByUserID(iUserID);
//	if (!pUser)
//	{
//		return false;
//	}
//
//	//注意索引和自然属性的关系
//	UINT8 eachNaturePropertyNum[ENatureProperty_MAX-1] = {0};
//	//数组引用
//	UINT32 (&curTeam)[MAXNUM_HEROINTEAM] = pUser->team.m_Teams[pUser->team.m_curTeamIdx];
//	for (int i = 0 ; i < MAXNUM_HEROINTEAM ; ++i)
//	{
//		if (!curTeam[i])
//		{
//			continue;
//		}
//
//		//判断
//		CUserHero* pHeroInstance = CUserHeroMgr::GetInstance()->GetHeroInstanceByHeroInstanceID(curTeam[i]);
//		if (!pHeroInstance)
//		{
//			//todo log error!
//			continue;
//		}
//
//		CHeroCfg* pHero = CHeroMgr::GetInstance()->GetHeroByHeroID(pHeroInstance->iHeroID);
//		if (!pHero)
//		{
//			//todo log error!
//			continue;
//		}
//		//注意索引和自然属性的关系
//		++eachNaturePropertyNum[pHero->eNatureProperty-1];
//	}
//
//	//
//	for (int i = ENatureProperty_Huo ; i < ENatureProperty_MAX ; ++i)
//	{
//		if (eachNaturePropertyNum[i-1] < m_noLessThanNum[i-1] || eachNaturePropertyNum[i-1] > m_noMoreThanNum[i-1])
//		{
//			return false;
//		}
//	}
//
//	return true;
//}
//
//bool CTaskEnterCondition_NatureProperty::SetContorl( ENatureProperty eNatureProperty, UINT8 iNoLessThan, UINT8 iNoMoreThan )
//{
//	//目前不判断是否重复设置过 先允许重复设置！
//
//	if ((iNoLessThan < 0 || iNoLessThan > MAXNUM_HEROINTEAM))// >=某个数
//	{
//		return false;
//	}
//
//	if ((iNoMoreThan < 0 || iNoMoreThan > MAXNUM_HEROINTEAM))// <=某个数
//	{
//		return false;
//	}
//
//	m_noLessThanNum[eNatureProperty-1] = iNoLessThan;
//	m_noMoreThanNum[eNatureProperty-1] = iNoMoreThan;
//
//	return true;
//}


bool CCondition_Area::IsSatisfy( UINT32 iUserID/*, UINT32 iTaskID*/ )
{
	/* //不会出现 本对象在构造之前 已经判断m_iTaskID的有效性！
	CTask* pTask = CTaskMgr::GetInstance()->GetTaskByTaskID(m_iTaskID);
	if(!pTask){
	return false;
	}
	*/

	CUser* pUser = CUserMgr::GetInstance()->GetUserByUserID(iUserID);
	if (!pUser)
	{
		return false;
	}

	//注意索引和势力的关系
	UINT8 eachAreaNum[EArea_MAX-1] = {0};
	//数组引用
	UINT32 (&curTeam)[MAXNUM_HEROINTEAM] = pUser->m_hero_team.m_Teams[pUser->m_hero_team.m_curTeamIdx];
	for (int i = 0 ; i < MAXNUM_HEROINTEAM ; ++i)
	{
		if (!curTeam[i])
		{
			continue;
		}

		//判断英雄确实存在
		/*
		CUserHero* pHeroInstance = CUserHeroMgr::GetInstance()->GetHeroInstanceByHeroInstanceID(curTeam[i]);
		if (!pHeroInstance)
		{
			//todo log error!
			continue;
		}
		*/

		if(!pUser->GetHero(curTeam[i])){
			ASSERT(false);
			continue;
		}

		CHeroCfg* pHero = CHeroMgr::GetInstance()->GetHeroByHeroID(curTeam[i]);
		if (!pHero)
		{
			//todo log error!
			continue;
		}
		//注意索引和势力的关系
		++eachAreaNum[pHero->eArea-1];
	}

	//
	for (int i = EArea_Huaxia ; i < EArea_MAX ; ++i)
	{
		if (eachAreaNum[i-1] < m_noLessThanNum[i-1] || eachAreaNum[i-1] > m_noMoreThanNum[i-1])
		{
			return false;
		}
	}

	return true;
}

bool CCondition_Area::SetContorl( EArea eArea, UINT8 iNoLessThan, UINT8 iNoMoreThan )
{
	//目前不判断是否重复设置过 先允许重复设置！

	if ((iNoLessThan < 0 || iNoLessThan > MAXNUM_HEROINTEAM))// >=某个数
	{
		return false;
	}

	if ((iNoMoreThan < 0 || iNoMoreThan > MAXNUM_HEROINTEAM))// <=某个数
	{
		return false;
	}

	m_noLessThanNum[eArea-1] = iNoLessThan;
	m_noMoreThanNum[eArea-1] = iNoMoreThan;

	return true;
}

bool CCondition_Time::IsSatisfy( UINT32 iUserID/*, UINT32 iTaskID*/ )
{
	return IsSatisfy(iUserID, Now());
}

bool CCondition_Time::IsSatisfy( UINT32 iUserID, UINT32 iCheckTimestamp)
{
	switch (m_eTimeType)
	{
	case EConditionTimeType_Absolute:
		{
			UINT32 now = iCheckTimestamp;
			return now >= m_iStart && now <= m_iEnd;
		}
		break;
	case EConditionTimeType_Loop_Year:
		{
			ASSERT(false && "不支持的时间类型");
			return false;
		}
		break;
	case EConditionTimeType_Loop_Month:
		{
			time_t now = iCheckTimestamp;
			tm tm1 = GetTM(now);
			UINT8 day = tm1.tm_mday - 1;
			UINT32 seconds = day*24*60*60 + tm1.tm_hour*60*60 + tm1.tm_min*60 + tm1.tm_sec;
			return seconds >= m_iStart && seconds <= m_iEnd;
		}
		break;
	case EConditionTimeType_Loop_Week:
		{
			time_t now = iCheckTimestamp;
			tm tm1 = GetTM(now);
			UINT8 day = 0;
			if (tm1.tm_wday == 0)
			{
				day = 6;
			}
			else{
				day = tm1.tm_wday - 1;
			}

			UINT32 seconds = day*24*60*60 + tm1.tm_hour*60*60 + tm1.tm_min*60 + tm1.tm_sec;
			return seconds >= m_iStart && seconds <= m_iEnd;
		}
		break;
	case EConditionTimeType_Loop_Day:
		{
			time_t now = iCheckTimestamp;
			tm tm1 = GetTM(now);
			UINT32 seconds = tm1.tm_hour*60*60 + tm1.tm_min*60+tm1.tm_sec;

			return seconds >= m_iStart && seconds <= m_iEnd;
		}break;
	case EConditionTimeType_Loop_Hour:
		{
			time_t now = iCheckTimestamp;
			tm tm1 = GetTM(now);
			UINT32 seconds = tm1.tm_min*60+tm1.tm_sec;

			return seconds >= m_iStart && seconds <= m_iEnd;
		}break;
	case EConditionTimeType_Forever:{
		return true;
									}break;
	default:
		ASSERT(false && "不支持的时间类型");
	}

	return false;
}

bool CCondition_Time::SetContorl( EConditionTimeType eTimeType, UINT32 iStart, UINT32 iEnd )
{
	m_eTimeType = eTimeType;
	m_iStart = iStart;
	m_iEnd = iEnd;
	return true;
}

bool CCondition_Level::SetContorl( EConditionLevelType eLevelType, EConditionJudgeType eJudgeType, UINT32 iLevel )
{
	m_LevelType = eLevelType;
	m_JudgeType = eJudgeType;
	m_iLevel = iLevel;
	return true;
}

bool CCondition_Level::IsSatisfy( UINT32 iUserID/*, UINT32 iTaskID*/ )
{
	CUser* pUser = CUserMgr::GetInstance()->GetUserByUserID(iUserID);
	if (!pUser)
	{
		return false;
	}

	switch(m_LevelType)
	{
		case EConditionLevelType_Player_Level://玩家等级
			{
				switch(m_JudgeType)
				{
				case EConditionJudgeType_GTE://>=
					return pUser->GetLevel() >= m_iLevel;
				case EConditionJudgeType_E://=
					return pUser->GetLevel() == m_iLevel;
				default:
					ASSERT("不支持的类型" && false);
					break;
				}
			}
			break;
		case EConditionLevelType_Vip_Level://vip等级
			{
				switch(m_JudgeType)
				{
				case EConditionJudgeType_GTE://>=
					return pUser->GetVipLevel() >= m_iLevel;
				case EConditionJudgeType_E://=
					return pUser->GetVipLevel() == m_iLevel;
				default:
					ASSERT("不支持的类型" && false);
					break;
				}
			}
			break;
		default:
			ASSERT("不支持的类型" && false);
			break;
	}
	return false;
}


bool CCondition_Event::IsSatisfy(UINT32 iUserID, EMISSIONTYPE eMissionType)
{
	CUser* pUser = CUserMgr::GetInstance()->GetUserByUserID(iUserID);
	if (!pUser)
	{
		return false;
	}

	UINT32 iVal = pUser->GetMissionEventValueByEventType(m_eventType, m_eventSubType, eMissionType);
	if (iVal < m_iValue)
	{
		return false;
	}
	return true;
}

bool CCondition_Event::SetContorl( EEventType eEventType, EEventSubType eEventSubType, UINT32 iValue )
{
	m_eventType = eEventType;
	m_eventSubType = eEventSubType;
	m_iValue = iValue;
	return true;
}

bool ParseCondition(char* condition_str, ICondition*(& conditions)[MAXNUM_CONDITION]){
	//char* pBuf = UTF8ToANSI(row[col++]);

	ASSERT(condition_str);
	//必须为空的数组！
	for (UINT32 i = 0 ; i < MAXNUM_CONDITION ; ++i){
		ASSERT(conditions[i] == 0);
		if (conditions[i]){
			return false;
		}
	}

	//pBuf = UTF8ToANSI(row[col++]);
	vector<string> v_conditions;
	split_string(string(condition_str), v_conditions, "|");
	//free(pBuf);
	ASSERT(v_conditions.size()<= MAXNUM_CONDITION);
	UINT32 i = 0;
	for (vector<string>::iterator it = v_conditions.begin() ; it != v_conditions.end() ; ++it)
	{
		vector<int> acondition;
		split_string_2i(*it, acondition, "-");

		EConditionType eConType = (EConditionType)acondition[0];
		ASSERT(eConType > EConditionType_MIN && eConType < EConditionType_MAX);
		switch (eConType)
		{
		case EConditionType_Time:
			{
				CCondition_Time::EConditionTimeType eTimeType  = (CCondition_Time::EConditionTimeType)acondition[1];
				ASSERT( eTimeType > CCondition_Time::EConditionTimeType_MIN && eTimeType < CCondition_Time::EConditionTimeType_MAX);
				CCondition_Time* pCondition = new CCondition_Time;
				if (eTimeType == CCondition_Time::EConditionTimeType_Forever)
				{
					ASSERT(acondition.size() == 2);
					pCondition->SetContorl(eTimeType, 0, 0);
				}
				else{
					ASSERT(acondition.size() == 4);
					pCondition->SetContorl(eTimeType, acondition[2], acondition[3]);
				}

				//bool r = pTask->AppendCondition(CTask::EConditionFor_Enter, pCondition);
				//ASSERT(r);
				conditions[i++] = pCondition;
			}
			break;
		case EConditionType_Occupation:
			{
				ASSERT(acondition.size() == 4);
				EOccupation eOcc = (EOccupation)acondition[1];
				ASSERT( eOcc > EOccupation_MIN && eOcc < EOccupation_MAX);

				CCondition_Occupation* pCondition = (CCondition_Occupation*)(GetFirstCondition(conditions, EConditionType_Occupation));
				if (!pCondition)
				{
					pCondition = new CCondition_Occupation;
					//bool r = pTask->AppendCondition(CTask::EConditionFor_Enter, pCondition);
					//ASSERT(r);
					conditions[i++] = pCondition;
				}

				pCondition->SetContorl(eOcc, acondition[2], acondition[3]);

			}
			break;
		case EConditionType_Area:
			{
				ASSERT(acondition.size() == 4);
				EArea eArea = (EArea)acondition[1];
				ASSERT( eArea > EArea_MIN && eArea < EArea_MAX);

				CCondition_Area* pCondition = (CCondition_Area*)(GetFirstCondition(conditions, EConditionType_Area));
				if (!pCondition)
				{
					pCondition = new CCondition_Area;
					//bool r = pTask->AppendCondition(CTask::EConditionFor_Enter, pCondition);
					//ASSERT(r);
					conditions[i++] = pCondition;
				}

				pCondition->SetContorl(eArea, acondition[2], acondition[3]);
			}
			break;
		case EConditionType_Level:
			{
				ASSERT(acondition.size() == 4);
				CCondition_Level::EConditionLevelType eLevelType = (CCondition_Level::EConditionLevelType)acondition[1];
				ASSERT(eLevelType > CCondition_Level::EConditionLevelType_MIN && eLevelType < CCondition_Level::EConditionLevelType_MAX);

				CCondition_Level::EConditionJudgeType eJudgeType = (CCondition_Level::EConditionJudgeType)acondition[2];
				ASSERT(eLevelType > CCondition_Level::EConditionJudgeType_MIN && eLevelType < CCondition_Level::EConditionJudgeType_MAX);

				CCondition_Level* pCondition = new CCondition_Level;
				pCondition->SetContorl(eLevelType, eJudgeType, acondition[3]);

				conditions[i++] = pCondition;
			}
			break;
		case EConditionType_Event:
			{

				EEventType eEventType = (EEventType)acondition[1];
				ASSERT(eEventType > EEventType_Min && eEventType < EEventType_Max);

				EEventSubType eEventSubType = (EEventSubType)acondition[2];
				//ASSERT(eEventSubType > EEventSubType_Min && eEventSubType < EEventSubType_Max);

				CCondition_Event* pCondition = new CCondition_Event;
				ASSERT(acondition.size() == 4);

				pCondition->SetContorl(eEventType, eEventSubType, acondition[3]);
				conditions[i++] = pCondition;

			}
			break;
		default:
			ASSERT(false && "不支持的条件类型！");
			return false;
		}
	}

	return true;
}

bool ParseTimeCondition(char* condition_str, ICondition*(& conditions)[MAXNUM_CONDITION]){
	//char* pBuf = UTF8ToANSI(row[col++]);

	ASSERT(condition_str);
	//必须为空的数组！
	for (UINT32 i = 0 ; i < MAXNUM_CONDITION ; ++i){
		ASSERT(conditions[i] == 0);
		if (conditions[i]){
			return false;
		}
	}

	vector<string> v_conditions;
	split_string(string(condition_str), v_conditions, "|");
	//free(pBuf);
	//ASSERT(v_conditions.size() > 0 && v_conditions.size() <= MAXNUM_CONDITION);//配了活动包 就要配条件 否则容易漏了就导致是无条件！
	ASSERT( v_conditions.size() <= MAXNUM_CONDITION);//还是允许为空条件！
	UINT32 i = 0;
	for (vector<string>::iterator it = v_conditions.begin() ; it != v_conditions.end() ; ++it)
	{
		vector<int> acondition;
		split_string_2i(*it, acondition, "-");

		EConditionType eConType = (EConditionType)acondition[0];
		ASSERT(eConType ==  EConditionType_Time);

		CCondition_Time::EConditionTimeType eTimeType  = (CCondition_Time::EConditionTimeType)acondition[1];
		ASSERT( eTimeType > CCondition_Time::EConditionTimeType_MIN && eTimeType < CCondition_Time::EConditionTimeType_MAX);
		CCondition_Time* pCondition = new CCondition_Time;
		if (eTimeType == CCondition_Time::EConditionTimeType_Forever)
		{
			ASSERT(acondition.size() == 2);
			pCondition->SetContorl(eTimeType, 0, 0);
		}
		else{
			ASSERT(acondition.size() == 4);
			pCondition->SetContorl(eTimeType, acondition[2], acondition[3]);
		}

		conditions[i++] = pCondition;
		//bool r = pItem->AppendCondition(pCondition);
		//ASSERT(r);
	}

	return true;
}

bool IsConditionsSatisfy(ICondition*(& conditions)[MAXNUM_CONDITION], UINT32 iUserID, bool useMissionType/* = false*/, EMISSIONTYPE eMissionType/* = EMISSIONTYPE_MIN*/){

	bool has_time_cond = false;
	bool is_any_time_cond_satisfy = false;//时间条件不存在或者任意满足一个就行
	bool is_other_cond_satisfy = true;//其他条件只要有一个不满足就算不满足
	for (UINT32 i = 0; i < MAXNUM_CONDITION ; ++i)
	{
		ICondition* pCondition = conditions[i];
		if (!pCondition){
			continue;
		}

		if (!has_time_cond && pCondition->GetConditionType() == EConditionType_Time)
		{
			has_time_cond = true;
		}

		if (pCondition->GetConditionType() == EConditionType_Time){
			if (pCondition->IsSatisfy(iUserID)){
				is_any_time_cond_satisfy = true;
			}
		}
		else if(pCondition->GetConditionType() == EConditionType_Event)
		{
			CCondition_Event* pEventCondition = dynamic_cast<CCondition_Event*>(pCondition);
			ASSERT(pEventCondition);
			CHECK_NULL_RETURN_FALSE(pEventCondition);
			if (!useMissionType)
			{
				return false;
			}
			if(!pEventCondition->IsSatisfy(iUserID, eMissionType)){
				return false;
			}
		}
		else
		{
			if (!pCondition->IsSatisfy(iUserID)){//其他条件有一个不满足就认为不满足
				return false;
			}
		}
	}

	//既然到了此处 如果有其他条件 则都满足
	if (has_time_cond && !is_any_time_cond_satisfy){
		return false;
	}

	return true;
}

bool IsTimeConditionsSatisfy(ICondition*(& conditions)[MAXNUM_CONDITION], UINT32 iUserID, bool use_check_timestamp/* = false*/ , UINT32 check_timestamp/* = 0*/){
	for (UINT32 i = 0; i < MAXNUM_CONDITION ; ++i)
	{
		ICondition* pCondition = conditions[i];
		if (!pCondition){
			continue;
		}

		if (pCondition->GetConditionType() == EConditionType_Time){
			CCondition_Time* pTimeCondition = dynamic_cast<CCondition_Time*>(pCondition);
			if ( pTimeCondition && pTimeCondition->IsSatisfy(iUserID, check_timestamp)){
				return true;
			}
		}
	}

	return false;
}

ICondition* GetFirstCondition(ICondition*(& conditions)[MAXNUM_CONDITION], EConditionType eConditionType){
	for (UINT32 i = 0; i < MAXNUM_CONDITION ; ++i)
	{
		ICondition* pCondition = conditions[i];
		if (!pCondition)
		{
			break;
		}
		else{
			if (pCondition->GetConditionType() == eConditionType)
			{
				return pCondition;
			}
		}
	}

	return NULL;
}

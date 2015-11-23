#include "task_enter_condition.h"

bool CTaskEnterCondition_Occupation::IsSatisfy( UINT32 iUserID/*, UINT32 iTaskID*/ )
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
	UINT32 (&curTeam)[MAXNUM_HEROINTEAM] = pUser->team.m_Teams[pUser->team.m_curTeamIdx];
	for (int i = 0 ; i < MAXNUM_HEROINTEAM ; ++i)
	{
		if (!curTeam[i])
		{
			continue;
		}

		//判断
		CUserHero* pHeroInstance = CUserHeroMgr::GetInstance()->GetHeroInstanceByHeroInstanceID(curTeam[i]);
		if (!pHeroInstance)
		{
			//todo log error!
			continue;
		}

		CHero* pHero = CHeroMgr::GetInstance()->GetHeroByHeroID(pHeroInstance->iHeroID);
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

bool CTaskEnterCondition_Occupation::SetContorl( EOccupation eOccupation, UINT8 iNoLessThan, UINT8 iNoMoreThan )
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


bool CTaskEnterCondition_NatureProperty::IsSatisfy( UINT32 iUserID/*, UINT32 iTaskID*/ )
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

	//注意索引和自然属性的关系
	UINT8 eachNaturePropertyNum[ENatureProperty_MAX-1] = {0};
	//数组引用
	UINT32 (&curTeam)[MAXNUM_HEROINTEAM] = pUser->team.m_Teams[pUser->team.m_curTeamIdx];
	for (int i = 0 ; i < MAXNUM_HEROINTEAM ; ++i)
	{
		if (!curTeam[i])
		{
			continue;
		}

		//判断
		CUserHero* pHeroInstance = CUserHeroMgr::GetInstance()->GetHeroInstanceByHeroInstanceID(curTeam[i]);
		if (!pHeroInstance)
		{
			//todo log error!
			continue;
		}

		CHero* pHero = CHeroMgr::GetInstance()->GetHeroByHeroID(pHeroInstance->iHeroID);
		if (!pHero)
		{
			//todo log error!
			continue;
		}
		//注意索引和自然属性的关系
		++eachNaturePropertyNum[pHero->eNatureProperty-1];
	}

	//
	for (int i = ENatureProperty_Huo ; i < ENatureProperty_MAX ; ++i)
	{
		if (eachNaturePropertyNum[i-1] < m_noLessThanNum[i-1] || eachNaturePropertyNum[i-1] > m_noMoreThanNum[i-1])
		{
			return false;
		}
	}

	return true;
}

bool CTaskEnterCondition_NatureProperty::SetContorl( ENatureProperty eNatureProperty, UINT8 iNoLessThan, UINT8 iNoMoreThan )
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

	m_noLessThanNum[eNatureProperty-1] = iNoLessThan;
	m_noMoreThanNum[eNatureProperty-1] = iNoMoreThan;

	return true;
}


bool CTaskEnterCondition_Area::IsSatisfy( UINT32 iUserID/*, UINT32 iTaskID*/ )
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
	UINT32 (&curTeam)[MAXNUM_HEROINTEAM] = pUser->team.m_Teams[pUser->team.m_curTeamIdx];
	for (int i = 0 ; i < MAXNUM_HEROINTEAM ; ++i)
	{
		if (!curTeam[i])
		{
			continue;
		}

		//判断
		CUserHero* pHeroInstance = CUserHeroMgr::GetInstance()->GetHeroInstanceByHeroInstanceID(curTeam[i]);
		if (!pHeroInstance)
		{
			//todo log error!
			continue;
		}

		CHero* pHero = CHeroMgr::GetInstance()->GetHeroByHeroID(pHeroInstance->iHeroID);
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

bool CTaskEnterCondition_Area::SetContorl( EArea eArea, UINT8 iNoLessThan, UINT8 iNoMoreThan )
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
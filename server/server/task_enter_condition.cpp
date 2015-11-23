#include "task_enter_condition.h"

bool CTaskEnterCondition_Occupation::IsSatisfy( UINT32 iUserID/*, UINT32 iTaskID*/ )
{
	/* //������� �������ڹ���֮ǰ �Ѿ��ж�m_iTaskID����Ч�ԣ�
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

	//ע��������ְҵ�Ĺ�ϵ
	UINT8 eachOccupationNum[EOccupation_MAX-1] = {0};
	//��������
	UINT32 (&curTeam)[MAXNUM_HEROINTEAM] = pUser->team.m_Teams[pUser->team.m_curTeamIdx];
	for (int i = 0 ; i < MAXNUM_HEROINTEAM ; ++i)
	{
		if (!curTeam[i])
		{
			continue;
		}

		//�ж�
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
		//ע��������ְҵ�Ĺ�ϵ
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
	//Ŀǰ���ж��Ƿ��ظ����ù� �������ظ����ã�

	if ((iNoLessThan < 0 || iNoLessThan > MAXNUM_HEROINTEAM))// >=ĳ����
	{
		return false;
	}

	if ((iNoMoreThan < 0 || iNoMoreThan > MAXNUM_HEROINTEAM))// <=ĳ����
	{
		return false;
	}

	m_noLessThanNum[eOccupation-1] = iNoLessThan;
	m_noMoreThanNum[eOccupation-1] = iNoMoreThan;

	return true;
}


bool CTaskEnterCondition_NatureProperty::IsSatisfy( UINT32 iUserID/*, UINT32 iTaskID*/ )
{
	/* //������� �������ڹ���֮ǰ �Ѿ��ж�m_iTaskID����Ч�ԣ�
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

	//ע����������Ȼ���ԵĹ�ϵ
	UINT8 eachNaturePropertyNum[ENatureProperty_MAX-1] = {0};
	//��������
	UINT32 (&curTeam)[MAXNUM_HEROINTEAM] = pUser->team.m_Teams[pUser->team.m_curTeamIdx];
	for (int i = 0 ; i < MAXNUM_HEROINTEAM ; ++i)
	{
		if (!curTeam[i])
		{
			continue;
		}

		//�ж�
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
		//ע����������Ȼ���ԵĹ�ϵ
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
	//Ŀǰ���ж��Ƿ��ظ����ù� �������ظ����ã�

	if ((iNoLessThan < 0 || iNoLessThan > MAXNUM_HEROINTEAM))// >=ĳ����
	{
		return false;
	}

	if ((iNoMoreThan < 0 || iNoMoreThan > MAXNUM_HEROINTEAM))// <=ĳ����
	{
		return false;
	}

	m_noLessThanNum[eNatureProperty-1] = iNoLessThan;
	m_noMoreThanNum[eNatureProperty-1] = iNoMoreThan;

	return true;
}


bool CTaskEnterCondition_Area::IsSatisfy( UINT32 iUserID/*, UINT32 iTaskID*/ )
{
	/* //������� �������ڹ���֮ǰ �Ѿ��ж�m_iTaskID����Ч�ԣ�
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

	//ע�������������Ĺ�ϵ
	UINT8 eachAreaNum[EArea_MAX-1] = {0};
	//��������
	UINT32 (&curTeam)[MAXNUM_HEROINTEAM] = pUser->team.m_Teams[pUser->team.m_curTeamIdx];
	for (int i = 0 ; i < MAXNUM_HEROINTEAM ; ++i)
	{
		if (!curTeam[i])
		{
			continue;
		}

		//�ж�
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
		//ע�������������Ĺ�ϵ
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
	//Ŀǰ���ж��Ƿ��ظ����ù� �������ظ����ã�

	if ((iNoLessThan < 0 || iNoLessThan > MAXNUM_HEROINTEAM))// >=ĳ����
	{
		return false;
	}

	if ((iNoMoreThan < 0 || iNoMoreThan > MAXNUM_HEROINTEAM))// <=ĳ����
	{
		return false;
	}

	m_noLessThanNum[eArea-1] = iNoLessThan;
	m_noMoreThanNum[eArea-1] = iNoMoreThan;

	return true;
}
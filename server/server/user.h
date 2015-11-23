#ifndef __USER_H__
#define __USER_H__

#pragma once

#include "memory.h"

#include "def.h"
#include "config.h"

class CUserTeam{
public:

	CUserTeam():m_curTeamIdx(0){
		memset(m_Teams, 0, sizeof(m_Teams));
	}

	UINT32 UpdateTeam(UINT8 iTeamIdx, UINT32 (&heros)[MAXNUM_HEROINTEAM]);

private:
	UINT8 m_curTeamIdx;
	UINT32 m_Teams[MAXNUM_TEAM][MAXNUM_HEROINTEAM];//ÿ������洢����HeroInstanceID

	friend class CTaskEnterCondition_Occupation;
	friend class CTaskEnterCondition_NatureProperty;
	friend class CTaskEnterCondition_Area;
};


class CUser{

public:
	UINT32 GetHealth(){
		//todo ����ʱ��� ���µ�ǰ������

		return m_iHealth;
	}
private:
	UINT32 m_iUserID;
	UINT32 m_iLevel;
	UINT32 m_iHealth;
	CUserTeam team;//������Ϣ

	
	//friend class ITaskEnterCondition;//�û��� ������ʲ���
	
	friend class CTaskEnterCondition_Occupation;
	friend class CTaskEnterCondition_NatureProperty;
	friend class CTaskEnterCondition_Area;
};

#endif
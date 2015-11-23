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
	UINT32 m_Teams[MAXNUM_TEAM][MAXNUM_HEROINTEAM];//每个队伍存储的是HeroInstanceID

	friend class CTaskEnterCondition_Occupation;
	friend class CTaskEnterCondition_NatureProperty;
	friend class CTaskEnterCondition_Area;
};


class CUser{

public:
	UINT32 GetHealth(){
		//todo 根据时间戳 更新当前体力！

		return m_iHealth;
	}
private:
	UINT32 m_iUserID;
	UINT32 m_iLevel;
	UINT32 m_iHealth;
	CUserTeam team;//队伍信息

	
	//friend class ITaskEnterCondition;//用基类 子类访问不了
	
	friend class CTaskEnterCondition_Occupation;
	friend class CTaskEnterCondition_NatureProperty;
	friend class CTaskEnterCondition_Area;
};

#endif
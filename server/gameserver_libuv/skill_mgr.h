#ifndef skill_mgr_h__
#define skill_mgr_h__

#pragma once

#include <map>

#include "def.h"
#include "config.h"

#include "skill.h"


struct stSkillLevelNeed{
	UINT32 arrNeedGold[ESKILLTYPE_MAX];
};

class CSkillMgr{

public:
	//todo µ¥Àý¹¤¾ß
	inline static CSkillMgr* GetInstance(){
		static CSkillMgr singleton;
		return &singleton;
	}

	bool Init();

	bool UnInit();

	CPassiveSkillCfg* GetPassiveSkillCfgByID(UINT32 iPassiveSkillID);
	stSkillLevelNeed* GetLevelNeed(UINT32 iLevel);

private:

	CSkillMgr(){}

	std::map<UINT32, CPassiveSkillCfg*> m_map2PassiveSkill;//passive id ->passive skill
	std::map<UINT32, stSkillLevelNeed*> m_mapLevel2Need;// level ->need{gold, ...}
};


#endif // skill_mgr_h__
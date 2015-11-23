#pragma once
#ifndef _novice_guide_mgr_h__
#define _novice_guide_mgr_h__
#include <map>
#include "def.h"
#include "config.h"
class ICondition;
struct stGuide{
	stGuide(){
		iID = iPreGuildID = 0;
		memset(desc, 0, sizeof(desc));
		memset(triggerCondition, 0, sizeof(triggerCondition));
	}

	UINT32 iID;
	ICondition* triggerCondition[MAXNUM_CONDITION];	//´¥·¢Ìõ¼þ
	UINT32 iPreGuildID;
	char desc[512];
};

class CNoviceGuideMgr
{
public:
	static CNoviceGuideMgr* GetInstance(){
		static CNoviceGuideMgr singleton;
		return &singleton;
	}

	bool Init();
	
	stGuide* GetGuideByGuideID(UINT32 iGuideID);
private:
	CNoviceGuideMgr(void){};
	std::map<UINT32, stGuide*> m_mapGuid;
};

#endif
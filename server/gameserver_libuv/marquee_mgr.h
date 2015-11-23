#ifndef marquee_mgr_h__
#define marquee_mgr_h__
#pragma once
#include <map>
#include <vector>
#include "gs_c_proto.pb.h"
#include "def.h"
#include "sys_setting_mgr.h"
class CMarqueeMgr
{
public:

	struct stMarqueeCfg{
		stMarqueeCfg():iID(0), iShowTimes(0){}

		UINT32 iID;
		std::vector<UINT32> vecValue;
		UINT32 iShowTimes;
	};

	static CMarqueeMgr* GetInstance(){
		static CMarqueeMgr singleton;
		return &singleton;
	}

	bool Init();

	bool SendMarqueeToAllUsers(gs_c_marquee_ntf::MARQUEE_TYPE eType,  std::map<std::string, CValue>& values, UINT32 iValue = 0);
	
	bool SendMarqueeToAllUsers(gs_c_marquee_ntf::MARQUEE_TYPE eType,  const char* content);

	bool CanTriggerMarquee(gs_c_marquee_ntf::MARQUEE_TYPE eType, UINT32 iValue);

	std::vector<UINT32>* GetTriggerValue(gs_c_marquee_ntf::MARQUEE_TYPE eType);

private:
	CMarqueeMgr(void){};

	std::map<UINT32, stMarqueeCfg> m_mapID2MarqueeCfg;
};

#endif
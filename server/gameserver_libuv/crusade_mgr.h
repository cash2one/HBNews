#pragma once
#ifndef crusade_mgr_h__
#define crusade_mgr_h__
#include <map>
#include <vector>
#include <set>
#include "config.h"
class CrusadeMgr
{
public:
	static CrusadeMgr* GetInstance(){
		static CrusadeMgr singleton;
		return &singleton;
	}

	bool Init();

	bool GetCrusadeWavePrize(UINT32 iLevel, UINT32 iWave, std::vector<stGameElement>& vecEle);

	void PromotePrize(std::vector<stGameElement>& stEles);

private:
	CrusadeMgr(void){};
	std::map<UINT32, std::map<UINT32, std::vector<stGameElement>>> m_mapCrusadePrize;//level->(15���Ľ���)
	std::set<EELEMENTTYPE> m_setPromoteType;		//������������
	UINT32 m_iPromotePercent;
};

#endif
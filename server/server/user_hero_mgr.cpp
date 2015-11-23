#include "user_hero_mgr.h"

vector<CUserHero*>& CUserHeroMgr::GetHeroListByUserID( UINT32 iUserID )
{
	static vector<CUserHero*> emptyHeroList;
	map<UINT32, vector<CUserHero*>>::iterator it = m_mapUserID2HeroList.find(iUserID);
	if (it != m_mapUserID2HeroList.end())
	{
		return it->second;
	}
	else{
		return emptyHeroList;
	}
}

CUserHero* CUserHeroMgr::GetHeroInstanceByHeroInstanceID( UINT32 iHeroInstanceID )
{
	map<UINT32, CUserHero*>::iterator it = m_mapHeroInstanceID2HeroInstance.find(iHeroInstanceID);
	if (it != m_mapHeroInstanceID2HeroInstance.end())
	{
		return it->second;
	}

	return NULL;
}
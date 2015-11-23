#include "hero_mgr.h"


CHero* CHeroMgr::GetHeroByHeroID( UINT32 iHeroID )
{
	std::map<UINT32, CHero*>::iterator it = m_mapHeroID2Hero.find(iHeroID);
	if (it != m_mapHeroID2Hero.end())
	{
		return it->second;
	}

	return NULL;
}

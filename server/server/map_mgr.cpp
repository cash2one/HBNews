#include "map_mgr.h"

bool CMapMgr::Init()
{
	CMapPoint* a = new CMapPoint();
	a->iMappointID = 1;
	mMapMapPointID2MapPoint[1] = a;
	return true;
}

bool CMapMgr::UnInit()
{
	delete mMapMapPointID2MapPoint[1];
	return true;
}

CMapPoint* CMapMgr::GetMapPointByID( INT32 iMapPointID )
{
	CMapPoint* ret = NULL;
	map<UINT32, CMapPoint*>::iterator it = mMapMapPointID2MapPoint.find(iMapPointID);

	if (it != mMapMapPointID2MapPoint.end())
	{
		ret = it->second;
	}
	else{
		//todo log!
	}

	return ret;
}
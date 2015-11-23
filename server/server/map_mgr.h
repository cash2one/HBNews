
/*****************************************************************************
*   for : 基本配置
data : 20150312
author : baomin
*
******************************************************************************/
#ifndef __MAP_MGR_H__
#define __MAP_MGR_H__

#include <map>

#include "def.h"
#include "config.h"

using namespace std;

struct CMapPoint{//据点信息
	UINT32 iMappointID;//地图据点ID 全局唯一
	UINT16 iChapterID;//章节ID
	UINT8  bType;//据点类型
	CHAR strName[MAXLEN_MAPPOINTNAME];//
	UINT32 arrJoinedMapPointID[MAXNUM_JOINEDMAPPOINT];
	bool  isHidden;
};

class CMapMgr{
public:

	//todo 单例工具
	inline static CMapMgr* GetInstance(){
		static CMapMgr singleton;
		return &singleton;
	}

	bool Init();

	bool UnInit();

	CMapPoint* GetMapPointByID(INT32 iMapPointID);

private:

	map<UINT32, CMapPoint*> mMapMapPointID2MapPoint;

};

#endif
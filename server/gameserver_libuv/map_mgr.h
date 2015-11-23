
/*****************************************************************************
*   for : ��������
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

struct CMapPoint{//�ݵ���Ϣ
	UINT32 iMappointID;//��ͼ�ݵ�ID ȫ��Ψһ
	UINT16 iChapterID;//�½�ID
	UINT8  bType;//�ݵ����� ���� 1.���� 2.С��
	CHAR strName[MAXLEN_MAPPOINTNAME];//
	UINT32 arrJoinedMapPointID[MAXNUM_JOINEDMAPPOINT];
	UINT32 iEnterLevel;//����ȼ�
	bool  isHidden;
};

class CMapMgr{
public:

	//todo ��������
	inline static CMapMgr* GetInstance(){
		static CMapMgr singleton;
		return &singleton;
	}

	bool Init();

	bool UnInit();

	CMapPoint* GetMapPointByID(INT32 iMapPointID);
	
private:

	CMapMgr(){}

	bool AddMapPoint(CMapPoint* pPoint);

	map<UINT32, CMapPoint*> mMapMapPointID2MapPoint;

};

#endif
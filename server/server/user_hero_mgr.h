#ifndef __USER_HERO_MGR_H__
#define __USER_HERO_MGR_H__

#include <map>
#include <vector>
using namespace std;

#include "def.h"
#include "config.h"

class CUserHero{
	UINT32		iHeroInstanceID;	//Ӣ��ʵ��ID �洢 Ψһ
	UINT32		iUserID;	//�û�ID
	UINT32		iHeroID;	//Ӣ��ID
	UINT32		iLevel;		//Ӣ�۵ȼ�
	UINT32		iStar;		//Ӣ���Ǽ�

	friend class CTaskEnterCondition_Occupation;
	friend class CTaskEnterCondition_NatureProperty;
	friend class CTaskEnterCondition_Area;
};


class CUserHeroMgr{

public:
	//todo ��������
	inline static CUserHeroMgr* GetInstance(){
		static CUserHeroMgr singleton;
		return &singleton;
	}

	bool Init(){

	}

	bool UnInit();

	CUserHero* GetHeroInstanceByHeroInstanceID(UINT32 iHeroInstanceID);

	vector<CUserHero*>& GetHeroListByUserID(UINT32 iUserID);

private:
	map<UINT32, CUserHero*> m_mapHeroInstanceID2HeroInstance;
	map<UINT32, vector<CUserHero*>> m_mapUserID2HeroList;
	

};
#endif
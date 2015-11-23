#ifndef __USER_HERO_MGR_H__
#define __USER_HERO_MGR_H__

#include <map>
#include <vector>
using namespace std;

#include "def.h"
#include "config.h"

class CUserHero{
	UINT32		iHeroInstanceID;	//英雄实例ID 存储 唯一
	UINT32		iUserID;	//用户ID
	UINT32		iHeroID;	//英雄ID
	UINT32		iLevel;		//英雄等级
	UINT32		iStar;		//英雄星级

	friend class CTaskEnterCondition_Occupation;
	friend class CTaskEnterCondition_NatureProperty;
	friend class CTaskEnterCondition_Area;
};


class CUserHeroMgr{

public:
	//todo 单例工具
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
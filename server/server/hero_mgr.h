#ifndef __HERO_MGR_H__
#define __HERO_MGR_H__

#include <map>

//如果这个在map前面 会导致一堆语法错误 为什么？
#include "config.h"
#include "def.h"



struct CSkill{
	UINT32 iSkillID;
};

struct CHero{
	//名字和描述等等 服务器不需要
	UINT32	iHeroID;	//英雄ID
	EArea	eArea;		//国家
	UINT8	iStar;		//星级
	EOccupation	eOccupation;	//职业	
	ENatureProperty eNatureProperty;	//自然属性
	UINT32	arrProperty[EProperty_MAX];	//英雄属性
	CSkill	arrSkill[MAXNUM_HEROSKILL];	//英雄技能列表
};


class CHeroMgr{

public:
	//todo 单例工具
	inline static CHeroMgr* GetInstance(){
		static CHeroMgr singleton;
		return &singleton;
	}

	bool Init(){

	}

	bool UnInit();

	CHero* GetHeroByHeroID(UINT32 iHeroID);

private:
	std::map<UINT32, CHero*> m_mapHeroID2Hero;
};

#endif


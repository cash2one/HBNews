#ifndef fight_mgr_h__
#define fight_mgr_h__

#pragma once;

#include <map>
#include <vector>
#include "config.h"
#include "def.h"

struct stBattleElement
{
	EELEMENTTYPE	eElementType;	//场景对象类型。 比如小怪 boss  英雄  甚至宝箱
	UINT32  iElementSubType;	//子类型 可选
	UINT32	iElementID;		//场景对象ID
	UINT8	iElementPos;	//场景对象位置
};
//战斗配置
struct stBattle{
	UINT32 iBattleID;
	/*
	vector<UINT32>	m_vecSceneObjectType;	//场景对象类型。 比如小怪 boss  英雄  甚至宝箱
	vector<UINT32>	m_vecSceneObjectID;		//场景对象ID
	vector<UINT8>	m_vecSceneObjectPos;	//场景对象位置
	*/

	//这种顺序对齐大小更小
	//EELEMENTTYPE	m_vecSceneObjectType[MAXNUM_TASKBATTLE];	//场景对象类型。 比如小怪 boss  英雄  甚至宝箱
	//UINT8	m_vecSceneObjectPos[MAXNUM_TASKBATTLE];	//场景对象位置
	//UINT32	m_vecSceneObjectID[MAXNUM_TASKBATTLE];		//场景对象ID

	
	//服务器不需要！
	//stBattleElement elements[MAXNUM_TASKBATTLEPOS];//一场战斗中的怪物，boss等的部署
};

//struct stBattleControl{
//	UINT32 iBattleID;		//战斗ID
//	//UINT32 iDifficulty;		//难度系数 服务器不需要
//	UINT32 iTimeLimit;		//时间限制  秒为单位
//};

class CFightMgr{
public:
	//todo 单例工具
	inline static CFightMgr* GetInstance(){
		static CFightMgr singleton;
		return &singleton;
	}

	bool Init();

	bool UnInit();

	stBattle* GetBattleByID(UINT32 iBattleID);

	bool AddBattle(stBattle* pBattle);

private:

	CFightMgr(){}

	std::map<UINT32, stBattle*> m_mapBattleID2Battle;
};

#endif // fight_mgr_h__
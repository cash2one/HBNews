#ifndef invoke_mgr_h__
#define invoke_mgr_h__

#pragma once

#include <vector>
#include <map>
#include "def.h"
#include "config.h"
#include "condition.h"

enum EInvokeType{
	/*
	EInvokeType_MIN = 0,
	EInvokeType_GOLD_FREE = 1,		// 免费金币单次抽取
	EInvokeType_GOLD_1 = 2,			//金币单次抽取（玩家消耗金币）
	EInvokeType_GOLD_FIRST_N = 3,	//金币十连抽首次抽取
	EInvokeType_GOLD_N = 4,			//金币十连抽普通抽取
	EInvokeType_DIAMOND_FREE = 5,	//普通免费钻石抽取
	EInvokeType_DIAMOND_1 = 6,		//普通钻石抽取（玩家消耗了钻石）
	EInvokeType_DIAMOND_FIRST_N = 7,	//钻石十连抽首次抽取
	EInvokeType_DIAMOND_N = 8,		//钻石十连抽普通抽取
	
	EInvokeType_MAX = 9
	//9 与新手任务相关的抽取（可能会有2个，暂时未定）;10 将来会有魂匣系统，魂匣系统的相关抽取，魂匣系统的抽取可能需要一个计数器，当次数满足多少时;
	*/

	EInvokeType_MIN = 0,
	EInvokeType_GOLD_FREE = 1,		// 免费金币单次抽取
	EInvokeType_GOLD_1 = 2,			//金币单次抽取（玩家消耗金币）
	EInvokeType_GOLD_N = 3,			//金币十连抽取
	EInvokeType_DIAMOND_FREE = 4,	//普通免费钻石抽取
	EInvokeType_DIAMOND_1 = 5,		//普通钻石抽取（玩家消耗了钻石）
	EInvokeType_DIAMOND_N = 6,		//钻石十连抽取
	EInvokeType_VIP_DIAMOND_1 = 7,	//VIP钻石召唤
	EInvokeType_VIP_DIAMOND_N = 8,	//VIP钻石召唤
	//7 与新手任务相关的抽取（可能会有2个，暂时未定）;
	//8 将来会有魂匣系统，魂匣系统的相关抽取，魂匣系统的抽取可能需要一个计数器，当次数满足多少时;
	EInvokeType_FRIEND_POINT = 9,	//友情点抽取
	EInvokeType_MAX = 10
};

struct stInvokeCfg{
	UINT32 iPacketID;		//普通掉落包ID
	UINT32 iFirstPacketID;	//初次使用时的包ID
	UINT32 iActivityPacketID;		//活动掉落包ID
	ICondition* ActivityConditions[MAXNUM_CONDITION];	//活动时间限制

	//bool IsConditionsSatisfy(UINT32 iUserID){
	//	return _IsConditionsSatisfy(ActivityConditions, iUserID);
	//}
};

class CInvokeMgr{

public:
	//todo 单例工具
	inline static CInvokeMgr* GetInstance(){
		static CInvokeMgr singleton;
		return &singleton;
	}

	bool Init();

	bool UnInit();

	//invoke_num只有在友情点的时候才有用
	bool Invoke(EInvokeType invoke_type, UINT32 invoke_num, std::vector<stGameElement>& output, std::vector<stGameElement>& consume, bool isFirstTime = false);

private:
	stInvokeCfg* m_invokeType2Cfg[EInvokeType_MAX];
};

#endif // invoke_mgr_h__

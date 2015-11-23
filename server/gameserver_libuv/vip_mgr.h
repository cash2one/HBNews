#ifndef vip_h__
#define vip_h__
#include <map>
#include "def.h"
#include "config.h"
#include "sys_setting_mgr.h"
class CVipCfg{
public:
	UINT32 m_vip_level;
	UINT32 m_rmb;	//达到此等级需要达到的累计充值金额
	UINT32 m_buy_num_health;		//可购买体力次数
	UINT32 m_buy_num_instance;		//控制购买自由（支线）子任务次数
	UINT32 m_buy_num_arena;			//控制购买竞技场挑战次数
	UINT32 m_buy_num_gold;			//控制可点金次数
	UINT32 m_buy_skill_point;		//控制购买技能点次数限制
	UINT32 m_max_num_skill_point;	//玩家每次回复技能点
	UINT32 m_max_num_exp_battle;	//每日经验活动副本次数
	UINT32 m_max_num_gold_battle;	//每日金币活动副本次数
	UINT32 m_max_num_tower;			//每日爬塔次数
	UINT32 m_max_num_crusade;		//每日远征次数
};

//class CVipBuyCfg{
//public:
//	UINT32 m_buy_type;				//购买的东西：1.体力 2.点金 3.技能点
//	UINT32 m_no_less_than_buy_num;	//>=此购买次数的时候
//	UINT32 m_diamond;				//>=no_less_than_buy_num购买次数的时候需要花费的钻石数
//};

enum EBuy_Type{
	EBUY_MIN,
	EBUY_HEALTH = 1,			//体力
	EBUY_GOLD = 2,				//点金
	EBUY_SKILL = 3,				//技能点
	EBUY_BRANCHTASK_RESET = 4,	//EBUY_SUB_TASK_ENTER_NUM 自由任务重置 //自由（支线）子任务挑战次数
	EBUY_SHOP_REFRESH = 5,		//商店刷新
	EBUY_ARENA_CHALLENGE_NUM = 6,//竞技场挑战次数
	EBUY_CRUSUDE_REFRESH_BUFF = 7,
	EBUY_CRUSUDE_HERO_LIFE = 8,			 //英雄复活(远征)
	EBUY_MAX,
};

enum EVip_Level_Function{
	EVip_Level_Function_Min = 0,
	EVip_Level_Function_Speed_Kill_N = 1,//>1扫荡次数的vip开启等级
	EVip_Level_Function_Max = 2
};

class CVipMgr{
public:
	
	bool VipLevelSatisfy(EVip_Level_Function eVipFun, UINT32 iVipLevel){
		ASSERT(iVipLevel <= CSysSettingMgr::GetInstance()->Max_Num_Vip_Level);
		ASSERT(eVipFun > EVip_Level_Function_Min && eVipFun < EVip_Level_Function_Max);
		
		switch(eVipFun)
		{
		case EVip_Level_Function_Speed_Kill_N:
			{
				UINT32 iCfgVip = 0;
				ASSERT(CSysSettingMgr::GetInstance()->GetIntValue("Vip_Level_Function_Speed_Kill_N", iCfgVip));
				return iVipLevel >= iCfgVip;
			}
			break;
		default:
			ASSERT(false && "不支持的类型");
			return false;
			break;
		}
	}

	//todo 单例工具
	inline static CVipMgr* GetInstance(){
		static CVipMgr singleton;
		return &singleton;
	}

	bool Init();

	bool UnInit();
	
	//根据vip等级获取vip信息
	CVipCfg* GetVipCfgByVipLevel(UINT32 iVipLevel);
	
	UINT32 GetMaxSkillPoint(UINT32 iVipLevel);

	//根据购买类型和次数获取花费的钻石数
	UINT32 GetCostDiamond(EBuy_Type eBuyType, UINT32 iTimes);
private:
	std::map<UINT32, CVipCfg*> m_mapVipLevel2VipCfg;
	//std::map<EBuy_Type, std::map<UINT32, UINT32>>	m_mapBuyType2Cfg;//first:buy_type second:(first:num second:diamond)
	std::map<EBuy_Type, std::vector<std::pair<UINT32, UINT32>>> m_mapBuyType2Cfg;// first:buy_type second: list(first:num second:diamond) 从小到大排序
};

#endif
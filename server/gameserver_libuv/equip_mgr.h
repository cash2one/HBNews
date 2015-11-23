#ifndef equip_mgr_h__
#define equip_mgr_h__

#pragma once

#include <vector>
#include <map>
#include "def.h"
#include "equip.h"
#include "config.h"

struct stEquipLevelNeed{
	//UINT32 iLevel;
	UINT32 arrNeedGold[EEQUIPTYPE_MAX];
};

struct stEquipQualityNeed{
	//EEQUIPTYPE iQuality;
	UINT32 arrNeedGold[EEQUIPTYPE_MAX];
};

struct stEquipLevelUpInOut{
	//只读部分
	UINT32 iEquipID;//
	UINT32 iLevel;	//当前等级

	//写部分
	UINT32 iLevelUpNum;//能升级的等级
	UINT32 iGold	;//消耗的金币数
};

class CEquipMgr{

public:
	//todo 单例工具
	inline static CEquipMgr* GetInstance(){
		static CEquipMgr singleton;
		return &singleton;
	}

	bool Init();

	bool UnInit();

	CEquipCfg* GetEquipCfgByEquipID(UINT32 iEquipID);

	//根据当前条件 得出装备升级 所能增加的等级数 和 消耗的金币
	void GetCanUpLevelByType(const EEQUIPTYPE equip_type, const UINT32 cur_level, const UINT32 level_limit, const UINT32 gold_limit, UINT32* need_gold, UINT32* can_up_level );
	//根据当前条件 得出装备升级 所能增加的等级数 和 消耗的金币
	void LevelUpByEquipID(const UINT32 equip_id, const UINT32 cur_level, const UINT32 level_limit, const UINT32 gold_limit, UINT32* need_gold, UINT32* can_up_level );
	
	UINT32 LevelUp(const UINT32 level_limit, const UINT32 gold_limit, std::vector<stEquipLevelUpInOut>& in_out);
	
	EQUALITY GetQuality(const UINT32 equip_id){
		CEquipCfg* cfg = GetEquipCfgByEquipID(equip_id);
		if (cfg)
		{
			return cfg->eQuality;
		}
		else{
			printf("GetQuality 找不到物品[%u] !!\n", equip_id);
		}
		return EQUALITY_MIN;
	}

	UINT32 EvolutionToQualityGold(EQUALITY eToQuality, EEQUIPTYPE eEquipType){
		ASSERT(eToQuality > EQUALITY_MIN && eToQuality < EQUALITY_MAX);// 必须小于最大的品质！
		ASSERT(eEquipType > EEQUIPTYPE_MIN && eEquipType < EEQUIPTYPE_MAX);
		return m_mapQuality2Need[eToQuality].arrNeedGold[eEquipType];
	}
private:

	CEquipMgr(){
		memset(m_mapQuality2Need, 0, sizeof m_mapQuality2Need);
	}

	std::map<UINT32, CEquipCfg*> m_mapID2EquipCfg;//equipid ->equip
	std::map<UINT32, stEquipLevelNeed*> m_mapLevel2Need;//level->gold   因为level较多 不易枚举 所以不能用数组

	//std::map<EEQUIPTYPE, stEquipQualityGold*> m_mapQuality2Gold;//quality->gold
	stEquipQualityNeed m_mapQuality2Need[EQUALITY_MAX];
};

#endif // equip_mgr_h__
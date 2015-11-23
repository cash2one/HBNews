#ifndef equip_h__
#define equip_h__

#pragma once
#include <vector>
#include <string>
#include "config.h"
#include "def.h"

struct CEquipCfg{
	UINT32 iEquipID;//
	std::string name;
	EEQUIPTYPE eEquipType;
	EQUALITY eQuality;
	UINT32 iEvolutionEquipID;						//进化后的装备ID
	std::vector<stGameElement*> vEvolutionConsume;	//进化所需要需要的资源
};


//db_user_hero_equip

#endif // equip_h__
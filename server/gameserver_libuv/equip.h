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
	UINT32 iEvolutionEquipID;						//�������װ��ID
	std::vector<stGameElement*> vEvolutionConsume;	//��������Ҫ��Ҫ����Դ
};


//db_user_hero_equip

#endif // equip_h__
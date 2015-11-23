#ifndef item_h__
#define item_h__

#include <vector>
#include <string>

#include "config.h"
#include "def.h"
class struct_game_element;
class IItem{
	public:
		virtual ~IItem(){};
		virtual UINT32 GetNum()	= 0;
		virtual	UINT32 GetOverlapNum()	= 0;
		virtual UINT32 GetId() = 0;
		virtual void   SetId(UINT32 iItemId) = 0;
		virtual UINT16 GetQuality() = 0;                                                          
		virtual void   AddNum(UINT16 iNum = 1) = 0;
		virtual void   DecNum(UINT16 iNum = 1) = 0;
		virtual void   SetNum(UINT16 iNum) = 0;
};


class CItem : public IItem{
public:
	CItem(struct_game_element* pElement);

	virtual UINT32 GetNum();
	virtual void   AddNum(UINT16 iNum = 1);
	virtual void   DecNum(UINT16 iNum = 1);
	virtual void   SetNum(UINT16 iNum);
	
	virtual UINT32 GetId();
	virtual void   SetId(UINT32 iItemId);


	virtual	UINT32 GetOverlapNum();
	virtual UINT16 GetQuality();
private:
	struct_game_element* m_pItem;
	UINT16 m_iOverlapNum;
	UINT16 m_iQuality;
};

class CItemCfg{
public:
	enum EUseType{
		MIN = 0,
		EVOLUTION_MATERIAL = 1,	//1.进化材料
		EXP = 2,				//2.经验值道具
		SPEED_KILL_TICKET = 3,	//3.扫荡券
		COMMON = 4,				//4.杂物
		MATERIAL_FRAGMENT = 5,	//5.材料碎片
		HERO_FRAGMENT = 6,		//6.英雄碎片
		HEALTH_POTION = 7,		//7.体力药水
		GIFT = 8,				//8.礼包 
		OTHER = 9,				//9.其他（活动道具）
		MAX = 10
	};

	UINT32 m_iID;			//itemID
	EUseType  m_eUseType;	//使用类型
	std::string m_name;		//名字
	UINT8	m_iQuality;		//品质
	UINT32 m_iOverlapNum;	//最大可堆叠数量
	UINT32 m_iSellGold;		//出售金币
	std::vector<stGameElement*> m_use_output;	//使用之后的产出
	std::vector<stGameElement*> m_compose_input;	//合成所需要的输入
};

#endif // item_h__


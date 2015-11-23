#ifndef item_mgr_h__
#define item_mgr_h__

#pragma  once

#include <map>
#include "def.h"
#include "item.h"
#include "gs_c_proto.pb.h"
class CItemMgr{
public:
	//todo 单例工具
	inline static CItemMgr* GetInstance(){
		static CItemMgr singleton;
		return &singleton;
	}

	bool Init();

	bool UnInit();

	CItemCfg* GetItemCfgByItemID(UINT32 iItemID);

	//获取英雄对应的碎片
	CItemCfg* GetFragmentItemByHeroID(UINT32 iHeroID);

	bool CreateItem(struct_game_element* pEle);

	UINT32 GetSellGold(const ::google::protobuf::RepeatedPtrField< ::struct_game_element >& sell_items);

	struct_game_element* GetElementInCRes( ::google::protobuf::RepeatedPtrField< ::struct_game_element >& c_eles, struct_game_element::EELEMENTTYPE eType, UINT32 iID, bool create_if_not_exist = true );

private:
	CItemMgr(){}
	std::map<UINT32, CItemCfg*> m_mapItemID2ItemCfg;

	//方便计算用的索引
	std::map<UINT32, CItemCfg*> m_mapHeroID2Fragment;//英雄id -> 碎片道具
};

#endif // item_mgr_h__
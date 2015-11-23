#ifndef item_mgr_h__
#define item_mgr_h__

#pragma  once

#include <map>
#include "def.h"
#include "item.h"
#include "gs_c_proto.pb.h"
class CItemMgr{
public:
	//todo ��������
	inline static CItemMgr* GetInstance(){
		static CItemMgr singleton;
		return &singleton;
	}

	bool Init();

	bool UnInit();

	CItemCfg* GetItemCfgByItemID(UINT32 iItemID);

	//��ȡӢ�۶�Ӧ����Ƭ
	CItemCfg* GetFragmentItemByHeroID(UINT32 iHeroID);

	bool CreateItem(struct_game_element* pEle);

	UINT32 GetSellGold(const ::google::protobuf::RepeatedPtrField< ::struct_game_element >& sell_items);

	struct_game_element* GetElementInCRes( ::google::protobuf::RepeatedPtrField< ::struct_game_element >& c_eles, struct_game_element::EELEMENTTYPE eType, UINT32 iID, bool create_if_not_exist = true );

private:
	CItemMgr(){}
	std::map<UINT32, CItemCfg*> m_mapItemID2ItemCfg;

	//��������õ�����
	std::map<UINT32, CItemCfg*> m_mapHeroID2Fragment;//Ӣ��id -> ��Ƭ����
};

#endif // item_mgr_h__
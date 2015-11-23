#ifndef concrete_bag_h__
#define concrete_bag_h__

#include <google/protobuf/repeated_field.h>
#include <map>
#include "def.h"
#include "bag.h"

class CItemBag : public IBag
{
public:
	CItemBag():m_DBItems(NULL){

	}
	virtual bool   Init(struct_game_element_array* pElements);
	virtual bool   UnInit();

	virtual IItem* GetItem(UINT32 iItemId);

	virtual void AddItem(const struct_game_element& gameEle, ::google::protobuf::RepeatedPtrField< ::struct_game_element >* pRepeatedEle, ::google::protobuf::RepeatedPtrField<::struct_game_element>* p_c_rep_game_ele, UINT32& iActualAddNum);
	virtual void SetItem(const struct_game_element& gameEle, ::google::protobuf::RepeatedPtrField< ::struct_game_element >* pRepeatedEle, ::google::protobuf::RepeatedPtrField<struct_game_element>* p_c_rep_game_ele, UINT32& iActualAddNum);
	virtual bool DelItem(const struct_game_element& gameEle, ::google::protobuf::RepeatedPtrField< ::struct_game_element >* pRepeatedEle, ::google::protobuf::RepeatedPtrField<struct_game_element>* p_c_rep_game_ele);

	
	//virtual bool DelItems(std::vector<UINT32>& vecItemID);
	//virtual bool DelItem(UINT32 iItemID, UINT32 iNum); 

	virtual bool HasItems(std::vector<UINT32>& vecItemID);
	virtual bool HasItems(std::vector<stGameElement*>& vecEles);
	virtual bool HasItem(UINT iItemID);

	virtual bool CanAddItems(std::vector<stGameElement*>& vecEles);
	virtual bool CanAddItem(stGameElement* pEle);
	virtual bool CanAddItem(struct_game_element* pGameEle);

	virtual bool IsItemEnough(UINT32 iItemID, UINT32 iNum);
	virtual bool IsItemsEnough(::google::protobuf::RepeatedPtrField< ::struct_game_element >* pRepeatedEle);
	virtual bool IsItemsEnough(std::vector<stGameElement>& vecEles);
	
	virtual bool IsItemFull(struct_game_element* pEle);
	
	virtual bool CheckNeedSendMail(UINT32 iItemID);

	virtual bool ExchangePos(UINT32 src_pos, UINT32 tar_pos){return true;};
	virtual void SortOut(){};//±³°üÕûÀí
	virtual bool CanAddItemInPos(IItem* pItem, UINT32 pos){return true;};
	virtual bool AddItemInPos(IItem*, UINT32 pos){return true;};

private:
	bool _del_item(UINT32 iItemID);

	struct_game_element_array* m_DBItems;
	std::map<UINT32, IItem*> m_mapItem;
	typedef std::map<UINT32, IItem*>::iterator ITER_ITEM;
};

#endif
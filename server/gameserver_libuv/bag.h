#ifndef bag_h__
#define bag_h__

#include <vector>
#include "gs_c_proto.pb.h"
#include "item.h"
#include "def.h"
class struct_game_element_array;
class IBag{

public:
	virtual ~IBag(){};
	
	virtual bool   Init(struct_game_element_array* pElements) = 0;
	virtual bool   UnInit() = 0;

	virtual IItem* GetItem(UINT32 uItemId) = 0;

	////彻底删除某个道具
	//virtual bool DelItem(UINT32 iItemID) = 0;
	////删除某个道具指定数量
	//virtual bool DelItem(UINT32 iItemID, UINT32 iNum) = 0; 
	//删除道具
	//virtual bool DelItems(std::vector<UINT32>& vecItemID) = 0;

	virtual void AddItem(const struct_game_element& gameEle, ::google::protobuf::RepeatedPtrField< ::struct_game_element >* pRepeatedEle, ::google::protobuf::RepeatedPtrField<::struct_game_element>* p_c_rep_game_ele, UINT32& iActualAddNum) = 0;
	
	virtual void SetItem(const struct_game_element& gameEle, ::google::protobuf::RepeatedPtrField< ::struct_game_element >* pRepeatedEle, ::google::protobuf::RepeatedPtrField<struct_game_element>* p_c_rep_game_ele, UINT32& iActualAddNum) = 0;
	
	virtual bool DelItem(const struct_game_element& gameEle, ::google::protobuf::RepeatedPtrField< ::struct_game_element >* pRepeatedEle, ::google::protobuf::RepeatedPtrField<struct_game_element>* p_c_rep_game_ele) = 0;

	virtual bool AddItemInPos(IItem*, UINT32 pos) = 0;
	
	virtual bool HasItems(std::vector<stGameElement*>& vecEles) = 0;
	virtual bool CanAddItems(std::vector<stGameElement*>& vecEles) = 0;
	virtual bool CanAddItem(stGameElement* pEle) = 0;
	virtual bool CanAddItem(struct_game_element* pGameEle) = 0;
	virtual bool IsItemsEnough(::google::protobuf::RepeatedPtrField< ::struct_game_element >* pRepeatedEle) = 0;
	virtual bool IsItemEnough(UINT32 iItemID, UINT32 iNum) = 0;
	
	virtual bool IsItemFull(struct_game_element* pEle) = 0;

	virtual bool CanAddItemInPos(IItem* pItem, UINT32 pos) = 0;

	virtual bool ExchangePos(UINT32 src_pos, UINT32 tar_pos) = 0;
	//检测是否需要发送邮件，有些物品不需要发送邮件(头像框)
	virtual bool CheckNeedSendMail(UINT32 iItemID) = 0;

	virtual void SortOut() = 0;//背包整理
};

#endif // bag_h__

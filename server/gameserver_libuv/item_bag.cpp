#include "item_bag.h"
#include "item.h"
#include "dbs_gs_proto.pb.h"
#include "item_mgr.h"
bool   CItemBag::Init(struct_game_element_array* pElements)
{
	ASSERT(pElements);
	m_DBItems = pElements;
	google::protobuf::RepeatedPtrField<struct_game_element >::iterator iter = pElements->mutable_items()->begin();
	for (; iter !=  pElements->mutable_items()->end(); ++iter)
	{
		IItem* pItem = new CItem(&*iter);
		ASSERT(pItem);
		ASSERT(m_mapItem.find(iter->id()) == m_mapItem.end());
		m_mapItem[iter->id()] = pItem;
	}
	return true;
}

bool   CItemBag::UnInit()
{
	std::map<UINT32, IItem*>::iterator iter = m_mapItem.begin();
	for (; iter != m_mapItem.end(); ++iter)
	{
		delete iter->second;
	}
	m_mapItem.clear();
	return true;
}


void CItemBag::AddItem(const struct_game_element& gameEle, ::google::protobuf::RepeatedPtrField< ::struct_game_element >* pRepeatedEle, ::google::protobuf::RepeatedPtrField<::struct_game_element>* p_c_rep_game_ele, UINT32& iActualAddNum)
{
	ASSERT(pRepeatedEle && p_c_rep_game_ele);

	IItem* pItem = GetItem(gameEle.id());
	if(pItem)
	{
		ASSERT(pItem->GetNum() <= pItem->GetOverlapNum());	

		UINT32 iOverlapNum = pItem->GetOverlapNum();
		UINT32 iTotalNum = pItem->GetNum() + gameEle.num();

		if(iTotalNum <= iOverlapNum)//小于可叠加上限
		{		
			struct_game_element* pDBEle = pRepeatedEle->Add();
			pDBEle->CopyFrom(gameEle);

			pItem->SetNum(iTotalNum);

			struct_game_element* pGameEle = p_c_rep_game_ele->Add();
			pGameEle->CopyFrom(gameEle);

			iActualAddNum =  gameEle.num();
		}
		else
		{
			UINT32 iAddNum = iOverlapNum - pItem->GetNum();
			if (iAddNum)
			{
				struct_game_element* pDBEle = pRepeatedEle->Add();
				pDBEle->CopyFrom(gameEle);
				//重新设置道具数量
				pDBEle->set_num(iAddNum);

				struct_game_element* pGameEle = p_c_rep_game_ele->Add();
				pGameEle->CopyFrom(gameEle);
				pGameEle->set_num(iAddNum);

				pItem->SetNum(iOverlapNum);
			}
			iActualAddNum = iAddNum;
		}
	}
	else
	{	
		
		struct_game_element* pDBItem = m_DBItems->add_items();
		pDBItem->CopyFrom(gameEle);
		pItem = new CItem(pDBItem);
		ASSERT(pItem);
		m_mapItem[gameEle.id()] = pItem;

		//db_req
		struct_game_element* pDBEle = pRepeatedEle->Add();
		pDBEle->CopyFrom(gameEle);

		struct_game_element* pGameEle = p_c_rep_game_ele->Add();
		pGameEle->CopyFrom(gameEle);

		if (gameEle.num() > pItem->GetOverlapNum())
		{
			pItem->SetNum(pItem->GetOverlapNum());//本地重新设置数量

			pDBEle->set_num(pItem->GetOverlapNum());//db_req重新设置数量

			pGameEle->set_num(pItem->GetOverlapNum());
		}
		iActualAddNum =  pItem->GetNum();
	}
}


void CItemBag::SetItem(const struct_game_element& gameEle, ::google::protobuf::RepeatedPtrField< ::struct_game_element >* pRepeatedEle, ::google::protobuf::RepeatedPtrField<struct_game_element>* p_c_rep_game_ele, UINT32& iActualAddNum)
{
	::google::protobuf::RepeatedPtrField< ::struct_game_element >::iterator iter = pRepeatedEle->begin();
	for (; iter != pRepeatedEle->end(); ++iter)
	{
		if (gameEle.id() == iter->id())
		{
			ASSERT(false && "重复设置将导致数据库添加多份道具");
		}
	}

	ASSERT(pRepeatedEle && p_c_rep_game_ele);
	IItem* pItem = GetItem(gameEle.id());
	if(!pItem)
	{
		struct_game_element* pDBEle = m_DBItems->add_items();
		pDBEle->CopyFrom(gameEle);
		pItem = new CItem(pDBEle);
		ASSERT(pItem);
		m_mapItem[gameEle.id()] = pItem;//先添加 但是可能还要修正数量
	}
	UINT32 iRawNum = pItem->GetNum();
	//db_req
	struct_game_element* pDBEle = pRepeatedEle->Add();
	pDBEle->CopyFrom(gameEle);

	struct_game_element* pGameEle = p_c_rep_game_ele->Add();
	pGameEle->CopyFrom(gameEle);

	if (gameEle.num() > pItem->GetOverlapNum())
	{
		pItem->SetNum(pItem->GetOverlapNum());//本地重新设置数量

		pDBEle->set_num(pItem->GetOverlapNum());//db_req重新设置数量
		
		pGameEle->set_num(pItem->GetOverlapNum());//c重新设置数值

	}
	iActualAddNum = pItem->GetNum();
}

bool CItemBag::DelItem(const struct_game_element& gameEle, ::google::protobuf::RepeatedPtrField< ::struct_game_element >* pRepeatedEle, ::google::protobuf::RepeatedPtrField<struct_game_element>* p_c_rep_game_ele)
{
	ASSERT(pRepeatedEle && p_c_rep_game_ele);
	IItem* pItem = GetItem(gameEle.id());
	if(!pItem)
	{
		return false;
	}

	if (pItem->GetNum() < gameEle.num())
	{
		return false;
	}

	if (pItem->GetNum() == gameEle.num())
	{
		_del_item(pItem->GetId());//删除此条记录
	}
	else
	{
		pItem->SetNum(pItem->GetNum() - gameEle.num());
	}
		
	struct_game_element* pDBEle = pRepeatedEle->Add();
	pDBEle->CopyFrom(gameEle);

	struct_game_element* pGameEle = p_c_rep_game_ele->Add();
	pGameEle->CopyFrom(gameEle);
	return true;
}
IItem* CItemBag::GetItem(UINT32 iItemId)
{
	std::map<UINT32, IItem*>::iterator iter = m_mapItem.find(iItemId);
	if (iter != m_mapItem.end())
	{
		ASSERT(iter->second);
		return iter->second;
	}
	return NULL;
}

bool CItemBag::HasItems(std::vector<stGameElement*>& vecEles)
{
	for (size_t i = 0; i < vecEles.size(); ++i)
	{
		if (!HasItem(vecEles[i]->iID))
		{
			return false;
		}
	}
	return true;
}

bool CItemBag::HasItems(std::vector<UINT32>& vecItemID)
{
	for (size_t i = 0; i < vecItemID.size(); ++i)
	{
		if (!HasItem(vecItemID[i]))
		{
			return false;
		}
	}
	return true;
}

bool CItemBag::HasItem(UINT iItemID)
{
	ITER_ITEM iter = m_mapItem.find(iItemID);
	return iter != m_mapItem.end();
}

bool CItemBag::CanAddItems(std::vector<stGameElement*>& vecEles)
{
	for (size_t i = 0; i < vecEles.size(); ++i)
	{
		if (!CanAddItem(vecEles[i]))
		{
			return false;
		}
	}
	return true;
}

bool CItemBag::CanAddItem(stGameElement* pEle)
{
	ASSERT(pEle);
	ITER_ITEM iter = m_mapItem.find(pEle->iID);
	CItemCfg* pCfg = CItemMgr::GetInstance()->GetItemCfgByItemID(pEle->iID);
	ASSERT(pCfg);
	if (iter != m_mapItem.end())
	{
		if (iter->second->GetNum() + pEle->iNum > pCfg->m_iOverlapNum)
		{
			return false;
		}
	}
	else
	{
		if (pEle->iNum > pCfg->m_iOverlapNum)
		{
			return false;
		}
	}
	return true;
}

bool CItemBag::CanAddItem(struct_game_element* pGameEle)
{
	ASSERT(pGameEle);
	ITER_ITEM iter = m_mapItem.find(pGameEle->id());
	CItemCfg* pCfg = CItemMgr::GetInstance()->GetItemCfgByItemID(pGameEle->id());
	ASSERT(pCfg);
	if (iter != m_mapItem.end())
	{
		if (iter->second->GetNum() + pGameEle->num() > pCfg->m_iOverlapNum)
		{
			return false;
		}
	}
	else
	{
		if (pGameEle->num() > pCfg->m_iOverlapNum)
		{
			return false;
		}
	}
	return true;
}

bool CItemBag::_del_item(UINT32 iItemID)
{
	if (!HasItem(iItemID))
		return false;
	ITER_ITEM iter = m_mapItem.find(iItemID);
	delete iter->second;
	m_mapItem.erase(iter);
	UINT32 iSize = m_DBItems->mutable_items()->size();
	for (size_t i = 0 ; i < iSize; ++i)
	{
		if (m_DBItems->mutable_items(i)->id() == iItemID)
		{
			m_DBItems->mutable_items()->SwapElements(i, iSize - 1);
			m_DBItems->mutable_items()->RemoveLast();
			return true;
		}
	}
	return false;
}


//bool CItemBag::DelItems(std::vector<UINT32>& vecItemID)
//{
//	if(!HasItems(vecItemID))
//		return false;
//
//	for(size_t i = 0; i < vecItemID.size(); ++i)
//	{
//		_del_item(vecItemID[i]);
//	}
//	return true;
//}

//bool CItemBag::DelItem(UINT32 iItemID, UINT32 iNum) 
//{
//	ASSERT(HasItem(iItemID));
//	ITER_ITEM iter = m_mapItem.find(iItemID);
//	ASSERT(iter !=  m_mapItem.end());
//	IItem* pItem = iter->second;
//	ASSERT(pItem);
//	ASSERT(pItem->GetNum() >= iNum);
//	if (pItem->GetNum() > iNum)
//	{
//		pItem->SetNum(pItem->GetNum() - iNum);
//	}
//	else
//	{
//		delete iter->second;
//		m_mapItem.erase(iter);
//		UINT32 iSize = m_DBItems->mutable_items()->size();
//		for (size_t i = 0 ; i < iSize; ++i)
//		{
//			if (m_DBItems->mutable_items(i)->id() == iItemID)
//			{
//				m_DBItems->mutable_items()->SwapElements(i, iSize - 1);
//				m_DBItems->mutable_items()->RemoveLast();
//			}
//		}
//	}
//	return true;
//}

bool CItemBag::IsItemEnough(UINT32 iItemID, UINT32 iNum)
{
	IItem* pItem = GetItem(iItemID);
	if (!pItem || (pItem->GetNum() < iNum))
	{
		return false;
	}
	return true;
}

bool CItemBag::IsItemsEnough(::google::protobuf::RepeatedPtrField< ::struct_game_element >* pRepeatedEle)
{
	::google::protobuf::RepeatedPtrField< ::struct_game_element >::iterator iter = pRepeatedEle->begin();
	for (; iter != pRepeatedEle->end(); ++iter)
	{
		ASSERT(iter->type() == EELEMENTTYPE_ITEM);
		IItem* pItem = GetItem(iter->id());
		if (!pItem || pItem->GetNum() < iter->num())
		{
			return false;
		}
	}
	return true;
}

bool CItemBag::IsItemsEnough( std::vector<stGameElement>& vecEles )
{
	std::vector<stGameElement>::iterator iter = vecEles.begin();
	for (; iter != vecEles.end(); ++iter)
	{
		ASSERT(iter->eType == EELEMENTTYPE_ITEM);
		IItem* pItem = GetItem(iter->iID);
		if (!pItem || pItem->GetNum() < iter->iNum)
		{
			return false;
		}
	}
	return true;
}

bool CItemBag::IsItemFull(struct_game_element* pEle)
{
	ASSERT(pEle->type() == struct_game_element::EELEMENTTYPE_ITEM);
	IItem* pItem = GetItem(pEle->id());
	if (pItem)
	{
		if (pItem->GetNum() == pItem->GetOverlapNum())
		{
			return true;
		}
	}
	return false;
}

bool CItemBag::CheckNeedSendMail( UINT32 iItemID )
{
	if (iItemID == 901 ||
		iItemID == 902 ||
		iItemID == 903 ||
		iItemID == 904 ||
		iItemID == 905 ||
		iItemID == 906)
	{
		return false;
	}
	return true;
}

#include "item.h"
#include "gs_c_proto.pb.h"
#include "item_mgr.h"

CItem::CItem(struct_game_element* pElement)
{
	ASSERT(pElement);
	m_pItem = pElement;
	CItemCfg* pCfg = CItemMgr::GetInstance()->GetItemCfgByItemID(m_pItem->id());
	ASSERT(pCfg);
	m_iOverlapNum = pCfg->m_iOverlapNum;
	m_iQuality = pCfg->m_iQuality;
}

UINT32 CItem::GetNum()
{
	return m_pItem->num();
}

void CItem::AddNum(UINT16 iNum)
{
	ASSERT(m_pItem);
	m_pItem->set_num(m_pItem->num() + iNum);
}

void   CItem::DecNum(UINT16 iNum)
{
	ASSERT(m_pItem);
	m_pItem->set_num(m_pItem->num() - iNum);
}

void   CItem::SetNum(UINT16 iNum)
{
	ASSERT(m_pItem);
	m_pItem->set_num(iNum);
}

UINT32 CItem::GetId()
{
	return m_pItem->id();
}

void   CItem::SetId(UINT32 iItemId)
{
	m_pItem->set_id(iItemId);
}

UINT32 CItem::GetOverlapNum()
{
	return m_iOverlapNum;
}

UINT16 CItem::GetQuality()
{
	return m_iQuality;
}


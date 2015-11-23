#include <string>
#include <vector>
#include "item_mgr.h"
#include "db_mgr.h"
#include "db_sql.h"
#include "str.h"
using std::string;
using std::vector;

bool CItemMgr::Init()
{

	//todo 首先判断协议的枚举和手动定义的枚举是否一致

	MYSQL* con = CDBMgr::GetInstance()->GetConnection();

	//解析表！
	char* sql = SELECT_ITEM;
	int code = CDBMgr::Query(con, sql, (unsigned int)strlen(sql));
	if (0 != code)   //非0查询失败  
	{
		printf("query failed! [%s]\n", sql);  
		return false;  
	}

	int i = 0;
	do
	{
		MYSQL_RES* res = mysql_store_result( con );
		switch (i++)
		{
		case 0:{//base_info
				   {
					   //检查表结构是否有改变
					   string need_fields[] = {"id","item_id","use_type","name_cn","quality","overlap_num","sell_gold","use_output","compose_input"};
					   std::vector<std::string> v_need_fields(need_fields, need_fields + sizeof(need_fields)/sizeof(need_fields[0]));
					   ASSERT(CDBMgr::check_fields(res, v_need_fields));
				   }

				   MYSQL_ROW row;
				   while (row = mysql_fetch_row(res))
				   {
						int col = 1;//忽略数据库id
						CItemCfg* pItem = new CItemCfg;
						pItem->m_iID = atoi(row[col++]);
						pItem->m_eUseType = (CItemCfg::EUseType)atoi(row[col++]);
						pItem->m_name = row[col++];
						pItem->m_iQuality = atoi(row[col++]);
						pItem->m_iOverlapNum = atoi(row[col++]);
						pItem->m_iSellGold = atoi(row[col++]);

						{//use_output
							char* pBuf = UTF8ToANSI(row[col++]);
							std::vector<string> elements;
							split_string(string(pBuf), elements, "|");
							free(pBuf);
							for (vector<string>::iterator it = elements.begin() ; it != elements.end() ; ++it)
							{
								vector<int> ele;
								split_string_2i(*it, ele, "-");
								ASSERT(ele.size() == 3);

								stGameElement* pEle = new stGameElement((EELEMENTTYPE)ele[0], ele[1], NULL, stGameElement::INCREASE, ele[2]);
								pItem->m_use_output.push_back(pEle);

								//英雄碎片时 建个 英雄ID-》道具的索引
								if (pItem->m_eUseType == CItemCfg::HERO_FRAGMENT && pEle->eType == EELEMENTTYPE_HERO)
								{
									ASSERT(m_mapHeroID2Fragment.find(pEle->iID) == m_mapHeroID2Fragment.end());
									m_mapHeroID2Fragment[pEle->iID] = pItem;
								}
							}
						}

						{//compose_input
							char* pBuf = UTF8ToANSI(row[col++]);
							vector<string> elements;
							split_string(string(pBuf), elements, "|");
							free(pBuf);
							for (vector<string>::iterator it = elements.begin() ; it != elements.end() ; ++it)
							{
								vector<int> ele;
								split_string_2i(*it, ele, "-");
								ASSERT(ele.size() == 3);

								stGameElement* pEle = new stGameElement((EELEMENTTYPE)ele[0], ele[1], NULL, stGameElement::DECREASE, ele[2]);
								pItem->m_compose_input.push_back(pEle);
							}
						}

						ASSERT(m_mapItemID2ItemCfg.find(pItem->m_iID) == m_mapItemID2ItemCfg.end());
						m_mapItemID2ItemCfg[pItem->m_iID] = pItem;
				   }

				   ASSERT(!m_mapItemID2ItemCfg.empty());

			   }
			   break;
		}
		mysql_free_result(res);
	}while( !mysql_next_result( con ) );

	return true;
}

CItemCfg* CItemMgr::GetItemCfgByItemID( UINT32 iItemID )
{
	std::map<UINT32, CItemCfg*>::iterator it = m_mapItemID2ItemCfg.find(iItemID);
	if (it != m_mapItemID2ItemCfg.end())
	{
		return it->second;
	}

	return NULL;
}

CItemCfg* CItemMgr::GetFragmentItemByHeroID( UINT32 iHeroID )
{
	std::map<UINT32, CItemCfg*>::iterator it = m_mapHeroID2Fragment.find(iHeroID);
	if (it != m_mapHeroID2Fragment.end())
	{
		return it->second;
	}
	return NULL;
}

bool CItemMgr::CreateItem(struct_game_element* pEle)
{
	std::map<UINT32, CItemCfg*>::iterator iter = m_mapItemID2ItemCfg.find(pEle->id());
	if (iter ==  m_mapItemID2ItemCfg.end())
	{
		return false;
	}
	pEle->set_num_type(struct_game_element::TOTAL);
	pEle->set_sub_id(iter->second->m_eUseType);
	pEle->set_type(struct_game_element::EELEMENTTYPE_ITEM);
	return true;
}

UINT32 CItemMgr::GetSellGold(const ::google::protobuf::RepeatedPtrField< ::struct_game_element >& sell_items )
{
	UINT32 iGold = 0;
	::google::protobuf::RepeatedPtrField< ::struct_game_element >::const_iterator iter = sell_items.begin();
	for (; iter != sell_items.end(); ++iter)
	{
		ASSERT(iter->type() == struct_game_element::EELEMENTTYPE_ITEM);
		if (iter->type() != struct_game_element::EELEMENTTYPE_ITEM)
		{
			return -1;
		}

		CItemCfg* pItem = CItemMgr::GetInstance()->GetItemCfgByItemID(iter->id());
		if(!pItem)
		{
			ASSERT(false && "GetSellGold 道具不存在");
			return -1;
		}
		else
		{
			iGold += pItem->m_iSellGold * iter->num();
		}
	}

	return iGold;
}

struct_game_element* CItemMgr::GetElementInCRes( ::google::protobuf::RepeatedPtrField< ::struct_game_element >& c_eles, struct_game_element::EELEMENTTYPE eType, UINT32 iID, bool create_if_not_exist /*= true */ )
{
	struct_game_element* ret_ele = NULL;
	for (::google::protobuf::RepeatedPtrField< ::struct_game_element >::iterator it = c_eles.begin() ; it != c_eles.end(); ++it ){
		struct_game_element& ele = *it;
		if (ele.type() == eType && ele.id() == iID){
			ret_ele = &ele;
			return ret_ele;
		}
	}

	if (create_if_not_exist && !ret_ele){
		ret_ele = c_eles.Add();
	}

	return ret_ele;
}
#include <string>
#include <vector>
#include "equip_mgr.h"
#include "db_mgr.h"
#include "db_sql.h"
#include "str.h"

using std::string;
using std::vector;

bool CEquipMgr::Init()
{
	MYSQL* con = CDBMgr::GetInstance()->GetConnection();

	//解析表！
	char* sql = SELECT_EQUIP
		SELECT_EQUIP_LEVEL_GOLD
		SELECT_EQUIP_QUALITY_GOLD
		;
	int code = CDBMgr::Query(con, sql, (unsigned int)strlen(sql));
	if (0 != code)   //非0查询失败  
	{
		printf("query failed! [%s] [%d] [%s] \n", sql, mysql_errno(con), mysql_error(con));       
		return false;  
	}

	int i = 0;
	do
	{
		MYSQL_RES* res = mysql_store_result( con );
		switch (i++)
		{
		case 0:{//equip
				   {
					   //检查表结构是否有改变
					   string need_fields[] = {"id","equip_id", "equip_type", "name_cn","quality","evolution_equip_id","evolution_input"};
					   std::vector<std::string> v_need_fields(need_fields, need_fields + sizeof(need_fields)/sizeof(need_fields[0]));
					   ASSERT(CDBMgr::check_fields(res, v_need_fields));
				   }

				   MYSQL_ROW row;
				   while (row = mysql_fetch_row(res))
				   {
					   int col = 1;//忽略数据库id
					   CEquipCfg* pItem = new CEquipCfg;
					   pItem->iEquipID = atoi(row[col++]);
					   pItem->eEquipType = (EEQUIPTYPE)atoi(row[col++]);
					   ASSERT( pItem->eEquipType > EEQUIPTYPE_MIN && pItem->eEquipType < EEQUIPTYPE_MAX);
					   pItem->name = row[col++];
					   pItem->eQuality = (EQUALITY)atoi(row[col++]);
					   ASSERT( pItem->eQuality > EQUALITY_MIN && pItem->eQuality < EQUALITY_MAX);
					   pItem->iEvolutionEquipID = atoi(row[col++]);

					   {//evolution_input
						   char* pBuf = UTF8ToANSI(row[col++]);
						   std::vector<string> elements;
						   split_string(string(pBuf), elements, "|");
						   free(pBuf);
						   for (vector<string>::iterator it = elements.begin() ; it != elements.end() ; ++it)
						   {
							   vector<int> ele;
							   split_string_2i(*it, ele, "-");
							   ASSERT(ele.size() == 3);

							   stGameElement* pEle = new stGameElement((EELEMENTTYPE)ele[0], ele[1], NULL, stGameElement::DECREASE, ele[2]);
							   pItem->vEvolutionConsume.push_back(pEle);
						   }
					   }

					   ASSERT(m_mapID2EquipCfg.find(pItem->iEquipID) == m_mapID2EquipCfg.end());
					   m_mapID2EquipCfg[pItem->iEquipID] = pItem;
				   }

			   }
			   break;
		case 1:{//level gold
				   {
					   //检查表结构是否有改变
					   string need_fields[] = {"level","weapon","necklace","clothes","belt","ring","boots"};
					   std::vector<std::string> v_need_fields(need_fields, need_fields + sizeof(need_fields)/sizeof(need_fields[0]));
					   ASSERT(CDBMgr::check_fields(res, v_need_fields));
				   }

				   MYSQL_ROW row;
				   while (row = mysql_fetch_row(res))
				   {
					   int col = 0;
					   stEquipLevelNeed* pItem = new stEquipLevelNeed;
					   UINT32 iLevel = atoi(row[col++]);
					   pItem->arrNeedGold[EEQUIPTYPE_WEAPON] = atoi(row[col++]);
					   pItem->arrNeedGold[EEQUIPTYPE_NECKLACE] = atoi(row[col++]);
					   pItem->arrNeedGold[EEQUIPTYPE_CLOTHES] = atoi(row[col++]);
					   pItem->arrNeedGold[EEQUIPTYPE_BELT] = atoi(row[col++]);
					   pItem->arrNeedGold[EEQUIPTYPE_RING] = atoi(row[col++]);
					   pItem->arrNeedGold[EEQUIPTYPE_BOOTS] = atoi(row[col++]);
					   
					   ASSERT(m_mapLevel2Need.find(iLevel) == m_mapLevel2Need.end());
					   m_mapLevel2Need[iLevel] = pItem;
				   }

			   }
			   break;
		case 2:{//quality gold
				   {
					   //检查表结构是否有改变
					   string need_fields[] = {"quality","weapon","necklace","clothes","belt","ring","boots"};
					   std::vector<std::string> v_need_fields(need_fields, need_fields + sizeof(need_fields)/sizeof(need_fields[0]));
					   ASSERT(CDBMgr::check_fields(res, v_need_fields));
				   }

				   MYSQL_ROW row;
				   while (row = mysql_fetch_row(res))
				   {
					   int col = 0;
					 
					   EQUALITY iQuality = (EQUALITY)atoi(row[col++]);
					   ASSERT(iQuality > EQUALITY_MIN && iQuality < EQUALITY_MAX);
					   stEquipQualityNeed* pItem = &(m_mapQuality2Need[iQuality]);

					   pItem->arrNeedGold[EEQUIPTYPE_WEAPON] = atoi(row[col++]);
					   pItem->arrNeedGold[EEQUIPTYPE_NECKLACE] = atoi(row[col++]);
					   pItem->arrNeedGold[EEQUIPTYPE_CLOTHES] = atoi(row[col++]);
					   pItem->arrNeedGold[EEQUIPTYPE_BELT] = atoi(row[col++]);
					   pItem->arrNeedGold[EEQUIPTYPE_RING] = atoi(row[col++]);
					   pItem->arrNeedGold[EEQUIPTYPE_BOOTS] = atoi(row[col++]);
				   }
			   }
			   break;
		}
		mysql_free_result(res);
	}while( !mysql_next_result( con ) );

	return true;
}

CEquipCfg* CEquipMgr::GetEquipCfgByEquipID( UINT32 iEquipID )
{
	std::map<UINT32, CEquipCfg*>::iterator it = m_mapID2EquipCfg.find(iEquipID);
	if ( it != m_mapID2EquipCfg.end())
	{
		return it->second;
	}

	return NULL;
}

void CEquipMgr::GetCanUpLevelByType( const EEQUIPTYPE equip_type, const UINT32 cur_level, const UINT32 level_limit, const UINT32 gold_limit, UINT32* need_gold, UINT32* can_up_level )
{
	(*can_up_level) = 0;
	(*need_gold) = 0;

	UINT32 l = cur_level;
	while (true)
	{
		std::map<UINT32, stEquipLevelNeed*>::iterator it = m_mapLevel2Need.find(l);
		ASSERT(it != m_mapLevel2Need.end());

		stEquipLevelNeed* pNeed = it->second;
		ASSERT(pNeed);

		//need_gold += pNeed->arrNeedGold;
		if ( ((*need_gold) + pNeed->arrNeedGold[equip_type]) > gold_limit || l >= level_limit)
		{
			break;
		}
		else{
			(*need_gold) += pNeed->arrNeedGold[equip_type];
			++(*can_up_level);
			++l;
		}
	}
}

void CEquipMgr::LevelUpByEquipID( const UINT32 equip_id, const UINT32 cur_level, const UINT32 level_limit, const UINT32 gold_limit, UINT32* need_gold, UINT32* can_up_level )
{
	(*can_up_level) = 0;
	(*need_gold) = 0;
	CEquipCfg* cfg = GetEquipCfgByEquipID(equip_id);
	ASSERT(cfg);
	if (cfg)
	{
		GetCanUpLevelByType(cfg->eEquipType, cur_level, level_limit, gold_limit, need_gold, can_up_level);
	}
}

UINT32 CEquipMgr::LevelUp( const UINT32 level_limit, const UINT32 gold_limit, std::vector<stEquipLevelUpInOut>& in_out)
{
	UINT32 left_gold = gold_limit;
	UINT32 use_gold = 0;
	for (std::vector<stEquipLevelUpInOut>::iterator it = in_out.begin(); it != in_out.end() ; ++it)
	{
		//UINT32 need_gold = 0;
		//UINT32 can_up_level = 0;
		//UINT32 user_level = pUser->GetLevel();
		//UINT32 user_gold = pUser->GetGold();
		//UINT32 equip_level = it->iLevel;

		if (left_gold > 0)
		{
			LevelUpByEquipID(it->iEquipID, it->iLevel, level_limit, left_gold, &it->iGold, &it->iLevelUpNum);
			ASSERT(left_gold >= it->iGold);
			left_gold -= it->iGold;
			use_gold += it->iGold;
		}
		else{
			break;
		}
	}

	return use_gold;
}
#include "config.h"

#include "item_mgr.h"
#include "equip_mgr.h"
#include "str.h"

bool IsElementValid( const stGameElement& ele, bool check_num_limit /*= false*/ )
{
	//ASSERT(ele.eType > EELEMENTTYPE_MIN && ele.eType < EELEMENTTYPE_MAX);
	if (ele.eType <= EELEMENTTYPE_MIN || ele.eType >= EELEMENTTYPE_MAX){
		ASSERT(false);
		return false;
	}

	switch (ele.eType)
	{
	case EELEMENTTYPE_ITEM:
		  {
			//ASSERT(CItemMgr::GetInstance()->GetItemCfgByItemID(ele.iID));
			CItemCfg* item_cfg =  CItemMgr::GetInstance()->GetItemCfgByItemID(ele.iID);
			if (NULL  == item_cfg)
			{
				ASSERT(false);
				return false;
			}

			if (check_num_limit)
			{
				if (item_cfg->m_iOverlapNum < ele.iNum)//不管numtype是哪种 都不应该
				{
					return false;
				}
			}
		  }
		break;
	case EELEMENTTYPE_EQUIP:
		{
			//ASSERT(CItemMgr::GetInstance()->GetItemCfgByItemID(ele.iID));
			if (NULL == CEquipMgr::GetInstance()->GetEquipCfgByEquipID(ele.iID))
			{
				ASSERT(false);
				return false;
			}
		}
		break;
	case EELEMENTTYPE_EXP:
		{
			//1.玩家经验 2.英雄经验
			if (ele.iID != 1 && ele.iID != 2)
			{
				ASSERT(false);
				return false;
			}
		}
		break;
	}

	return true;
}

bool IsElementValid( EELEMENTTYPE eType, UINT32 iID)
{
	stGameElement ele(eType, iID, 0, (stGameElement::ENUMTYPE)0, 0);
	return IsElementValid(ele);
}

bool stGameElement::Parse( const char* utf8_str, std::vector<stGameElement> & ret, ENUMTYPE eNumType)
{
	std::vector<std::string> items;
	char* pBuf = UTF8ToANSI(utf8_str);
	split_string(std::string(pBuf), items, "|");
	free(pBuf);

	for (UINT32 i = 0; i < items.size(); ++i){
		std::vector<int> type_id_num;
		split_string_2i(items[i], type_id_num, "-");
		ASSERT(type_id_num.size() == 3);
		//ASSERT(type_id_num[0] > EELEMENTTYPE_MIN && type_id_num[0] < EELEMENTTYPE_MAX);
		//todo 判断具体的物件配置是否存在
		stGameElement ele((EELEMENTTYPE)type_id_num[0], type_id_num[1], 0, eNumType, type_id_num[2]);
		if (!IsElementValid(ele, true)){
			ASSERT(false);
			return false;
		}
		ret.push_back(ele);
	}

	return true;
}
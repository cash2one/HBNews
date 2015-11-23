#include "shop_mgr.h"

#include "str.h"
#include "db_mgr.h"
#include "db_sql.h"
#include "rand.h"
#include "sys_setting_mgr.h"
#include "time_helper.h"
using std::string;
using std::vector;

bool CShopMgr::Init()
{
	MYSQL* con = CDBMgr::GetInstance()->GetConnection();

	//解析表！
	char* sql = "SELECT * FROM `sys_shop_packet`;"
		"SELECT * FROM `sys_shop`;"
		"SELECT * FROM `sys_shop_part`;"
		//"SELECT * FROM `sys_shop_refresh` ORDER BY `refresh_num` ASC;"//需要排序  已经配置在sys_vip_buy中
		;
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
		case 0:{//sys_shop_packet
				   {
					   //检查表结构是否有改变
					   string need_fields[] = {"shop_packet_id", "element_type_id", "element_num_arr", "element_probability_arr", "element_consume_type_id_num_arr", "element_flag_arr"};
					   std::vector<std::string> v_need_fields(need_fields, need_fields + sizeof(need_fields)/sizeof(need_fields[0]));
					   ASSERT(CDBMgr::check_fields(res, v_need_fields));
				   }

				   MYSQL_ROW row;
				   while (row = mysql_fetch_row(res))
				   {
					   CShopPacket* pItem = NULL;
					   UINT32 iShopPacketID = atoi(row[0]);
					   std::map<UINT32, CShopPacket*>::iterator it = m_MapShopPacketID2ShopPacket.find(iShopPacketID);
					   if (it != m_MapShopPacketID2ShopPacket.end() && it->second != NULL){
							pItem = it->second;
					   }
					   else{
							pItem =  new CShopPacket;
							pItem->iShopPacketID = iShopPacketID;
							m_MapShopPacketID2ShopPacket[iShopPacketID] = pItem;
					   }
					  
					   CShopPacketElement* pEle = new CShopPacketElement;

					   int col = 1;//id已经取了
					   char* pBuf = UTF8ToANSI(row[col++]);
					   vector<int> type_id;
					   split_string_2i(string(pBuf), type_id, "-");
					   free(pBuf);

					   ASSERT(type_id.size() == 2);
					   ASSERT(type_id[0] > EELEMENTTYPE_MIN && type_id[0] < EELEMENTTYPE_MAX);
					   
					   pEle->eType = (EELEMENTTYPE)type_id[0];
					   pEle->iID = type_id[1];

					   pBuf = UTF8ToANSI(row[col++]);
					   vector<int> element_num_arr;
					   split_string_2i(string(pBuf), element_num_arr, "|");
					   free(pBuf);

					   pBuf = UTF8ToANSI(row[col++]);
					   vector<int> element_probability_arr;
					   split_string_2i(string(pBuf), element_probability_arr, "|");
					   free(pBuf);

					   pBuf = UTF8ToANSI(row[col++]);
					   vector<string> element_consume_type_id_num_arr;
					   split_string(string(pBuf), element_consume_type_id_num_arr, "|");
					   free(pBuf);

					   pBuf = UTF8ToANSI(row[col++]);
					   vector<int> element_flag_arr;
					   split_string_2i(string(pBuf), element_flag_arr, "|");
					   free(pBuf);

					   //数量一致
					   ASSERT( element_num_arr.size() == element_probability_arr.size() 
						   && element_num_arr.size() == element_consume_type_id_num_arr.size() 
						   && element_num_arr.size() == element_flag_arr.size());

					   UINT32 total_probability = 0;
					   for (UINT32 i = 0 ; i < element_probability_arr.size() ; ++i){
						   total_probability += element_probability_arr[i];

						   stShopPacketElementFactor* pEleFactor = new stShopPacketElementFactor;
						   pEleFactor->iNum = element_num_arr[i];
						   pEleFactor->iProbality = element_probability_arr[i];

						   vector<int> type_id_num;
						   split_string_2i(element_consume_type_id_num_arr[i], type_id_num, "-");

						   ASSERT(type_id_num.size() == 3);
						   ASSERT(type_id_num[0] > EELEMENTTYPE_MIN && type_id_num[0] < EELEMENTTYPE_MAX);
						   pEleFactor->eConsumeType = (EELEMENTTYPE)type_id_num[0];
						   pEleFactor->iConsumeID = type_id_num[1];
						   pEleFactor->iConsumeNum = type_id_num[2];

						   ASSERT(element_flag_arr[i] >= stShopItem::ESHOPITEMFLAG_NONE && element_flag_arr[i] < stShopItem::ESHOPITEMFLAG_MAX);
						   pEleFactor->eFlag = (stShopItem::ESHOPITEMFLAG)element_flag_arr[i];
						  
						   pEle->vFactors.push_back(pEleFactor);
					   }

					   ASSERT(total_probability == MAXNUM_PROBABILITY);
					   pItem->vPacketElements.push_back(pEle);
				   }
				   ASSERT(!m_MapShopPacketID2ShopPacket.empty());
			   }
			   break;
		case 1:{//sys_shop
				   {
					   //检查表结构是否有改变
					   string need_fields[] = {"shop_type", "shop_name", "activity_shop_part_id", "activity_time_type", "activity_time"};
					   std::vector<std::string> v_need_fields(need_fields, need_fields + sizeof(need_fields)/sizeof(need_fields[0]));
					   ASSERT(CDBMgr::check_fields(res, v_need_fields));
				   }

				   MYSQL_ROW row;
				   while (row = mysql_fetch_row(res))
				   {
					   int col = 0;
					   CShopCfg* pShopCfg = new CShopCfg;
					   pShopCfg->eShopType = (CShop::ESHOPTYPE)atoi(row[col++]);
					   ASSERT(pShopCfg->eShopType > CShop::ESHOPTYPE_MIN && pShopCfg->eShopType < CShop::ESHOPTYPE_MAX);
					   ASSERT(m_MapShopType2ShopCfg.find(pShopCfg->eShopType) == m_MapShopType2ShopCfg.end());
					   pShopCfg->strName = row[col++];
					   pShopCfg->iActivityShopPartID = atoi(row[col++]);
					   //ASSERT(m_MapShopPartID2ShopPart.find(pShopCfg->iActivityShopPartID) != m_MapShopPartID2ShopPart.end());//商店和part循环依赖了！ 所以只能再使用的来判断活动的这个！

					   pShopCfg->iActivityTimeType = atoi(row[col++]);
					   pShopCfg->activity_time = row[col++];
					   m_MapShopType2ShopCfg[pShopCfg->eShopType] = pShopCfg;
				   }
		}break;
		case 2:{//sys_shop_part
				   {
					   //检查表结构是否有改变
					   string need_fields[] = {"shop_part_id", "shop_type", "shop_packet_id_arr", "burst_num"};
					   std::vector<std::string> v_need_fields(need_fields, need_fields + sizeof(need_fields)/sizeof(need_fields[0]));
					   ASSERT(CDBMgr::check_fields(res, v_need_fields));
				   }

				   MYSQL_ROW row;
				   while (row = mysql_fetch_row(res))
				   {
					   CShop::ESHOPTYPE  shop_type = (CShop::ESHOPTYPE)atoi(row[1]);
					   ASSERT(shop_type > CShop::ESHOPTYPE_MIN && shop_type < CShop::ESHOPTYPE_MAX);
					   std::map<CShop::ESHOPTYPE, CShopCfg*>::iterator it = m_MapShopType2ShopCfg.find(shop_type);
					   ASSERT( it != m_MapShopType2ShopCfg.end() && it->second != NULL);
					   CShopCfg* pShop = it->second;

					   int col = 2;
					   CShopPart* pShopPart = new CShopPart;

					   char* pBuf = UTF8ToANSI(row[col++]);
					   vector<int> shop_packet_id_arr;
					   split_string_2i(string(pBuf), shop_packet_id_arr, "|");
					   free(pBuf);

					   for (UINT32 i = 0 ; i < shop_packet_id_arr.size(); ++i)
					   {
						   ASSERT(m_MapShopPacketID2ShopPacket.find(shop_packet_id_arr[i]) != m_MapShopPacketID2ShopPacket.end());
						   pShopPart->vShopPacketIDs.push_back(shop_packet_id_arr[i]);
					   }

					   pShopPart->iBurstNum = atoi(row[col++]);
					   pShop->vShopParts.push_back(pShopPart);
				   }
			   }break;
		//case 3:{//sys_shop_refresh 已经配置在了 sys_vip_buy里面
		//		   {
		//			   //检查表结构是否有改变
		//			   string need_fields[] = {"refresh_num", "diamond"};
		//			   std::vector<std::string> v_need_fields(need_fields, need_fields + sizeof(need_fields)/sizeof(need_fields[0]));
		//			   ASSERT(CDBMgr::check_fields(res, v_need_fields));
		//		   }

		//		   MYSQL_ROW row;
		//		   while (row = mysql_fetch_row(res)){
		//			   UINT32 col = 0;
		//			   stShopRefreshCfg* pItem = new stShopRefreshCfg();
		//			   pItem->iRefreshNum = atoi(row[col++]);
		//			   pItem->iDiamond = atoi(row[col++]);
		//			   m_VShopRefreshCfg.push_back(pItem);
		//		   }
		//	   }break;
		}
		mysql_free_result(res);
	}while( !mysql_next_result( con ) );

	return true;
}

CShopMgr::CShopPacket* CShopMgr::GetPacketByPacketID( UINT32 iPacketID )
{
	std::map<UINT32, CShopPacket*>::iterator it = m_MapShopPacketID2ShopPacket.find(iPacketID);
	if(it != m_MapShopPacketID2ShopPacket.end()){
		return it->second;
	}

	return NULL;
}

bool CShopMgr::FillItemsToEmptyShop( CShop::ESHOPTYPE eShopType, const std::map<std::pair<EELEMENTTYPE, UINT32>, CShopItemWeightFactor*>& mapWeightFactor, CShop &shop )
{
	ASSERT(shop.Num() == 0);
	ASSERT(eShopType > CShop::ESHOPTYPE_MIN && eShopType < CShop::ESHOPTYPE_MAX);
	if (shop.Num() != 0){
		return false;
	}

	std::map<CShop::ESHOPTYPE, CShopCfg*>::iterator it = m_MapShopType2ShopCfg.find(eShopType);
	if (it != m_MapShopType2ShopCfg.end())
	{
		ASSERT(it->second);
		return it->second->Get(mapWeightFactor, shop);
	}

	return false;
}

//UINT32 CShopMgr::GetManualRefreshDiamond( UINT32 iManualRefreshNum )
//{
//	ASSERT(m_VShopRefreshCfg.size());
//	//配置的是 <= cfg->iRefreshNum的次数
//	for (std::vector<stShopRefreshCfg*>::iterator it = m_VShopRefreshCfg.begin() ; it != m_VShopRefreshCfg.end(); ++it){
//		stShopRefreshCfg* cfg = *it;
//		ASSERT(cfg);
//		if ( iManualRefreshNum <= cfg->iRefreshNum ){
//			return cfg->iDiamond;
//		}
//	}
//
//	//比最大的次数还大时
//	return m_VShopRefreshCfg[m_VShopRefreshCfg.size()-1]->iDiamond;
//}
bool CShopMgr::CShopPacketElement::Get( CShop &ret )
{
	UINT32 num = vFactors.size();
	UINT32 probality = 0;
	bool bget = false;
	UINT32 race = CRandom::GetInstance()->GetRandom(0, MAXNUM_PROBABILITY);
	for (UINT32 i = 0 ; i < num ; ++i){
		stShopPacketElementFactor* factor = vFactors[i];
		ASSERT(factor);
		probality += factor->iProbality;
		if ( race <= probality ){
			stShopItem* item = new stShopItem;
			item->iPos = ret.Num();
			item->eType = eType;
			item->iID = iID;
			item->iNum = factor->iNum;
			item->eConsumeType = factor->eConsumeType;
			item->iConsumeID = factor->iConsumeID;
			item->iConsumeNum = factor->iConsumeNum;
			item->eFlag = factor->eFlag;
			item->bIsSold = false;//初始化时是没卖的

			bool r = ret.Append(item);
			ASSERT(r);

			bget = true;
			break;
		}
	}

	return !vFactors.empty() && bget;
}

CShopMgr::CShopPacketElement* CShopMgr::CShopPacket::Get( const std::map<std::pair<EELEMENTTYPE, UINT32>, CShopItemWeightFactor*>& mapWeightFactor )
{
	//首先计算权重

	//Shop_Packet_Element_base_Weight	1	10	物品基础权重
	//Shop_Packet_Element_extra_Weight_factor1	1	10	额外权重系数1 
	//Shop_Packet_Element_extra_Weight_factor2	1	10	额外权重系数2
	UINT32 base_weight = 0;
	UINT32 weight_factor1 = 0;
	UINT32 weight_factor2 = 0;
	bool r = CSysSettingMgr::GetInstance()->GetIntValue("Shop_Packet_Element_base_Weight", base_weight);
	ASSERT(r);
	r = CSysSettingMgr::GetInstance()->GetIntValue("Shop_Packet_Element_extra_Weight_factor1", weight_factor1);
	ASSERT(r);
	r = CSysSettingMgr::GetInstance()->GetIntValue("Shop_Packet_Element_extra_Weight_factor2", weight_factor2);
	ASSERT(r);
	UINT32 eles_num = vPacketElements.size();
	std::vector<UINT32> vWeights;
	UINT32 totalWeights = 0;
	for( UINT32 i = 0 ; i < eles_num ; ++i){
		ASSERT(vPacketElements[i]);
		UINT32 extra_weight = 0;
		std::map<std::pair<EELEMENTTYPE, UINT32>, CShopItemWeightFactor*>::const_iterator it = mapWeightFactor.find(std::make_pair(vPacketElements[i]->eType, vPacketElements[i]->iID));
		if (it != mapWeightFactor.end())
		{
			ASSERT(it->second);
			extra_weight = weight_factor1 * it->second->GetIsUserHeroEquipEvolutionNeed() + weight_factor2 * it->second->GetBuyNumToday();
		}
		else{
			extra_weight = 0;
		}

		vWeights.push_back(base_weight + extra_weight);
		totalWeights += base_weight + extra_weight;
	}

	//根据权重 N选1

	UINT32 race = CRandom::GetInstance()->GetRandom(0, totalWeights);
	UINT32 cur_race = 0;
	for (UINT32 i = 0; i < eles_num ; ++i)
	{
		cur_race += vWeights[i];
		if ( race <= cur_race )
		{
			return vPacketElements[i];
		}
	}

	return NULL;
}

bool CShopMgr::CShopCfg::Get( const std::map<std::pair<EELEMENTTYPE, UINT32>, CShopItemWeightFactor*>& mapWeightFactor, CShop &ret )
{
	ASSERT(ret.Num() == 0);
	UINT32 parts_num = vShopParts.size();
	ASSERT(parts_num);
	
	//todo 先判断是否在活动范围内 如果在的话 把活动部分也加进来！

	//遍历商店的每一个part 取出packet
	std::vector<UINT32> shop_packets;//所有的商店包！
	for ( UINT32 i = 0 ; i < parts_num ; ++i)
	{
		CShopPart* part = vShopParts[i];
		ASSERT(part);
		UINT32 packet_num = part->vShopPacketIDs.size();
		UINT32 burst_num = part->iBurstNum;

		//先轮播
		UINT32 loop = burst_num/packet_num;
		for (UINT32 j = 0; j < loop ; ++j)
		{
			for (UINT32 k = 0 ; k < packet_num ; ++k)
			{
				shop_packets.push_back(part->vShopPacketIDs[k]);
			}
		}

		//剩下的随机选择
		UINT32 left_num = burst_num - loop * packet_num;
		for (UINT32 j = 0 ; j < left_num ; ++j)
		{
			UINT32 idx = CRandom::GetInstance()->GetRandom(0, packet_num-1);
			shop_packets.push_back(part->vShopPacketIDs[idx]);
		}
	}

	//从packet中 根据权重 选择出elements
	std::vector<CShopPacketElement*> ret_eles;
	UINT32 ret_packet_num = shop_packets.size();
	for(UINT32 i = 0 ; i < ret_packet_num ; ++i)
	{
		CShopPacket* packet = CShopMgr::GetInstance()->GetPacketByPacketID(shop_packets[i]);
		ASSERT(packet);
		CShopPacketElement* ele = packet->Get(mapWeightFactor);
		ASSERT(ele);
		ret_eles.push_back(ele);
	}

	//从element中选择factor
	UINT32 eles_num = ret_eles.size();
	for (UINT32 i = 0 ; i < eles_num ; ++i)
	{
		CShopPacketElement* ele = ret_eles[i];
		ASSERT(ele);
		bool r = ele->Get(ret);
		ASSERT(r);
	}

	return ret.Num() > 0;
}

bool CShop::IsNeedAutoRefresh( UINT32 &new_stamp )
{
	UINT32* pResetTimeArr = NULL;
	UINT32 iResetTimeNum = 0;
	switch (eShopType)
	{
	case ESHOPTYPE_COMMON:
		{
			UINT32 (&Shop_Reset_Time)[MAXNUM_SHOP_AUTO_REFRESH] = CSysSettingMgr::GetInstance()->Shop_Common_Reset_Time;
			pResetTimeArr = Shop_Reset_Time;
			iResetTimeNum = CSysSettingMgr::GetInstance()->Shop_Common_Reset_Time_Num;
		}
		break;
	case ESHOPTYPE_QIWUGE:
		{
			UINT32 (&Shop_Reset_Time)[MAXNUM_SHOP_AUTO_REFRESH] = CSysSettingMgr::GetInstance()->Shop_Qiwuge_Reset_Time;
			pResetTimeArr = Shop_Reset_Time;
			iResetTimeNum = CSysSettingMgr::GetInstance()->Shop_Qiwuge_Reset_Time_Num;
		}
		break;
	case ESHOPTYPE_YIJIEBAOKU:
		{
			UINT32 (&Shop_Reset_Time)[MAXNUM_SHOP_AUTO_REFRESH] = CSysSettingMgr::GetInstance()->Shop_Yijiebaoku_Reset_Time;
			pResetTimeArr = Shop_Reset_Time;
			iResetTimeNum = CSysSettingMgr::GetInstance()->Shop_Yijiebaoku_Reset_Time_Num;
		}
		break;
	case ESHOPTYPE_ARENA:
		{
			UINT32 (&Shop_Reset_Time)[MAXNUM_SHOP_AUTO_REFRESH] = CSysSettingMgr::GetInstance()->Shop_Arena_Reset_Time;
			pResetTimeArr = Shop_Reset_Time;
			iResetTimeNum = CSysSettingMgr::GetInstance()->Shop_Arena_Reset_Time_Num;
		}
		break;
	case ESHOPTYPE_EXPEDITION:
		{
			UINT32 (&Shop_Reset_Time)[MAXNUM_SHOP_AUTO_REFRESH] = CSysSettingMgr::GetInstance()->Shop_Expedition_Reset_Time;
			pResetTimeArr = Shop_Reset_Time;
			iResetTimeNum = CSysSettingMgr::GetInstance()->Shop_Expedition_Reset_Time_Num;
		}
		break;
	default:
		ASSERT(false && "不支持的商店类型");
		return false;
	}

	ASSERT(pResetTimeArr);
	if (!pResetTimeArr){
		return false;
	}

	//UINT32 last_seconds = GetSecondsOfDay(iLastAutoRefreshTime);
	//UINT32 cur_seconds = GetSecondsOfDay(Now());

	new_stamp = Now();
	return !IsInSameInterval(pResetTimeArr, iResetTimeNum, iLastAutoRefreshTime, new_stamp);
}

UINT32 CShop::IncreaseLastDayManualRefreshNum()
{
	UINT32 now = Now();
	if (isSameDay(now, iLastManualRefreshTime, CSysSettingMgr::GetInstance()->Global_Reset_Time)){
		++iLastDayManualRefreshNum;
	}
	else{
		iLastDayManualRefreshNum = 1;
	}
	
	iLastManualRefreshTime = now;
	return iLastDayManualRefreshNum;
}

UINT32 CShop::GetLastDayManualRefreshNum()
{
	UINT32 now = Now();
	if (!isSameDay(now, iLastManualRefreshTime, CSysSettingMgr::GetInstance()->Global_Reset_Time)){
		iLastDayManualRefreshNum = 0;
		//iLastManualRefreshTime = now;//可以不设置 下次直接刷新操作的时候改为now！
	}

	return iLastDayManualRefreshNum;
}

UINT32 CShopItemWeightFactor::GetBuyNumToday()
{
	if (!isSameDay(Now(), m_iBuyLastTimestamp, CSysSettingMgr::GetInstance()->Global_Reset_Time))
	{
		m_iBuyLastTimestamp = 0;
	}

	return m_iBuyNumToday;
}

UINT32 CShopItemWeightFactor::AddBuyNumToday( UINT32 iBuyNum )
{
	UINT32 now = Now();
	if (!isSameDay(now, m_iBuyLastTimestamp, CSysSettingMgr::GetInstance()->Global_Reset_Time)){
		m_iBuyNumToday = iBuyNum;
		m_iBuyLastTimestamp = now;
	}
	else{
		m_iBuyNumToday += iBuyNum;
	}

	return m_iBuyNumToday;
}
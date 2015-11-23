#include "user_shop_strategy.h"
#include "user.h"
#include "sys_setting_mgr.h"
#include "condition.h"
#include "rand.h"
#include "vip_mgr.h"

bool CUserShopStrategy::Init( CUser* pUser, 
							 const ::google::protobuf::RepeatedPtrField< ::db_user_shop >& shop_data, 
							 const ::google::protobuf::RepeatedPtrField< ::db_user_shop_buy >& shop_buy_data)
{
	ASSERT(pUser);
	ASSERT(m_mapWeightFactor.empty());
	m_mapWeightFactor.clear();

	m_pUser = pUser;

	for (::google::protobuf::RepeatedPtrField< ::db_user_shop >::const_iterator it = shop_data.begin() ; it != shop_data.end() ; ++it){
		ASSERT(m_userShopType2Shop.find((CShop::ESHOPTYPE)it->shop_type()) == m_userShopType2Shop.end());
		CShop* pShop = new CShop((CShop::ESHOPTYPE)it->shop_type());
		ASSERT(pShop);
		//pShop->eShopType = (CShop::ESHOPTYPE)it->shop_type();					//商店类型
		pShop->iLastAutoRefreshTime = it->last_auto_refresh_time();				//上一次的自动刷新时间
		pShop->iLastManualRefreshTime = it->last_manual_refresh_time();			//上一次的手动刷新时间
		pShop->iLastDayManualRefreshNum = it->last_day_manual_refresh_num();	//最后时间当日手动刷新的次数 
		pShop->iRandomOpenStartTimestamp = it->random_open_start_timestamp();
		pShop->iRandomOpenEndTimestamp = it->random_open_end_timestamp();

		//pShop->iTotalManualRefreshNum = it->total_manual_refresh_num();		//总共的手动刷新次数

		m_userShopType2Shop[pShop->eShopType] = pShop;

		//初始化商店信息
		for (::google::protobuf::RepeatedPtrField< ::db_user_shop_item >::const_iterator itIn = it->items().begin() ; itIn != it->items().end() ; ++itIn){
			stShopItem* pItem = new stShopItem(*itIn);
			bool r = pShop->Append(pItem);
			ASSERT(r && "商店添加道具失败！");
			if (!r){
				delete pItem;
				return false;
			}
		}
	}

	//初始化购买信息
	for (::google::protobuf::RepeatedPtrField< ::db_user_shop_buy >::const_iterator it = shop_buy_data.begin() ; it != shop_buy_data.end() ; ++it){
		std::pair<EELEMENTTYPE, UINT32> key = std::make_pair((EELEMENTTYPE)it->element_type(), it->element_id());
		//ASSERT(m_mapWeightFactor.find(key) == m_mapWeightFactor.end());
		if (m_mapWeightFactor.find(key) != m_mapWeightFactor.end()){
			return false;
		}

		bool isNeed = pUser->IsUserHeroEquipEvolutionNeed((EELEMENTTYPE)it->element_type(), it->element_id());
		CShopItemWeightFactor* factor = new CShopItemWeightFactor(it->last_day_buy_num(), it->last_buy_time(), isNeed);
		m_mapWeightFactor[key] = factor;
	}

	return true;
}

bool CUserShopStrategy::Buy( CShop::ESHOPTYPE eShopType, UINT32 iPos, gs_dbs_user_info_op_req& db_req, gs_c_shop_buy_item_res &c_res, GS_C_NTF &ntf )
{
	ASSERT(m_pUser);

	bool is_open_and_create_new_shop = false;
	if(!CheckShopOpen(eShopType, false, is_open_and_create_new_shop)){//不生成商店！ 因为既然都到buy了 肯定拉到了商店信息
		c_res.set_error_code(e_gsc_errorcode_shop_no_unlock);
		return false;
	}

	//ASSERT(iPos > 0 && iPos < );
	CShop* pShop = _get_shop(eShopType);
	//ASSERT(pShop);

	if (!pShop){//运营活动的时候 商店能开 但是还是NULL  但是这里也是不允许Buy的！！
		c_res.set_error_code(e_gsc_errorcode_shop_no_unlock);
		return false;
	}

	//判断是否此次应该刷新
	UINT32 new_stamp = 0;//
	if(pShop->IsNeedAutoRefresh(new_stamp)){
		c_res.set_error_code(e_gsc_errorcode_shop_has_refreshed);
		bool r = AutoRefreshShop(pShop, new_stamp, db_req, c_res.mutable_refreshed_shop());
		ASSERT(r);
		return false;
	}

	stShopItem* pShopItem = pShop->Get(iPos);
	if (!pShopItem){
		c_res.set_error_code(e_gsc_errorcode_shop_no_this_item);
		return false;
	}

	if (pShopItem->bIsSold){
		c_res.set_error_code(e_gsc_errorcode_shop_item_sold);
		return false;
	}

	EELEMENTTYPE eType = pShopItem->eType;
	UINT32 iID = pShopItem->iID;
	UINT32 iNum = pShopItem->iNum;

	ASSERT(eType == EELEMENTTYPE_ITEM);
	if ( eType != EELEMENTTYPE_ITEM){
		c_res.set_error_code(e_gsc_errorcode_unknown_error);
		return false;
	}

	if (!m_pUser->IsConsumeEnough(pShopItem->eConsumeType, pShopItem->iConsumeID, pShopItem->iConsumeNum)){
		c_res.set_error_code(e_gsc_errorcode_no_enough_consume);
		return false;
	}

	pShopItem->bIsSold = true;//设置为已售

	//先消耗
	stGameElement consume(pShopItem->eConsumeType, pShopItem->iConsumeID, 0, stGameElement::DECREASE, pShopItem->iConsumeNum);
	bool r = m_pUser->UpdateGameElement(consume, db_req, *c_res.mutable_consume(), ntf);
	ASSERT(r);

	//再产出
	stGameElement output(eType, iID, 0 , stGameElement::INCREASE, iNum);
	r = m_pUser->UpdateGameElement(output, db_req, *c_res.mutable_output(), ntf);
	ASSERT(r);

	//添加购买信息
	bool bIsUserHeroEquipEvolutionNeed = m_pUser->IsUserHeroEquipEvolutionNeed(eType, iID);
	CShopItemWeightFactor* pItemInfo = NULL;
	std::pair<EELEMENTTYPE, UINT32> key = std::make_pair(eType, iID);
	std::map<std::pair<EELEMENTTYPE, UINT32>, CShopItemWeightFactor*>::iterator it = m_mapWeightFactor.find(key);
	if (it != m_mapWeightFactor.end()){
		pItemInfo = it->second;
		ASSERT(pItemInfo);
		pItemInfo->AddBuyNumToday(iNum);
		pItemInfo->SetIsUserHeroEquipEvolutionNeed(bIsUserHeroEquipEvolutionNeed);
	}
	else{
		pItemInfo = new CShopItemWeightFactor(iNum, Now(), bIsUserHeroEquipEvolutionNeed);
		m_mapWeightFactor[key] = pItemInfo;
	}

	//更新商店道具信息
	gs_dbs_user_info_op_req::struct_op_shop_item* op_shop_item = db_req.add_shop_items();
	op_shop_item->set_shop_type((user_shop::ESHOPTYPE)eShopType);
	//op_shop_item->set_pos(iPos);
	pShopItem->Fill(op_shop_item->mutable_item(), NULL);//此时没有刷新商店，并且不需要发新的道具信息给客户端 客户端从结果可以设置已售

	//更新购买信息
	db_user_shop_buy* buy_item = db_req.add_buy_items();
	buy_item->set_element_type((struct_game_element::EELEMENTTYPE)eType);
	buy_item->set_element_id(iID);
	buy_item->set_last_day_buy_num(pItemInfo->GetBuyNumToday());
	buy_item->set_last_buy_time(pItemInfo->GetLastBuyTime());

	c_res.set_shop_type((user_shop::ESHOPTYPE)eShopType);
	c_res.set_pos(iPos);
	c_res.set_error_code(e_gsc_errorcode_success);
	return true;
}

bool CUserShopStrategy::AutoRefreshShop( CShop::ESHOPTYPE eShopType, UINT32 new_stamp, gs_dbs_user_info_op_req &db_req, user_shop *c_shop )
{
	//判断玩家有没有这个商店
	CShop* pShop = _get_shop(eShopType);
	ASSERT(pShop);
	if (!pShop){
		return false;
	}
	
	return AutoRefreshShop(pShop, new_stamp, db_req, c_shop);
}

bool CUserShopStrategy::AutoRefreshShop( CShop* pShop, UINT32 new_stamp, gs_dbs_user_info_op_req &db_req, user_shop *c_shop )
{
	ASSERT(pShop);
	if (!pShop){
		return false;
	}
	pShop->ClearItems();//清理旧的道具数据

	//生成商店道具给用户
	bool r = CShopMgr::GetInstance()->FillItemsToEmptyShop(pShop->eShopType, m_mapWeightFactor, *pShop);
	ASSERT(r);
	if (!r){
		return false;
	}

	pShop->iLastAutoRefreshTime = new_stamp;
	gs_dbs_user_info_op_req::struct_op_shop* op_shop = db_req.add_shops();
	op_shop->set_shop_op_type(gs_dbs_user_info_op_req::struct_op_shop::AUTO_REFRESH);
	pShop->Fill(op_shop->mutable_shop(), c_shop);

	return true;
}
bool CUserShopStrategy::ManualRefreshShop( CShop::ESHOPTYPE eShopType, gs_dbs_user_info_op_req &db_req, gs_c_shop_refresh_res &c_res, GS_C_NTF ntf )
{
	//判断玩家有没有这个商店
	CShop* pShop = _get_shop(eShopType);
	ASSERT(pShop);
	if (!pShop){
		c_res.set_error_code(e_gsc_errorcode_shop_no_unlock);
		return false;
	}

	//UINT32 consume_num = CShopMgr::GetInstance()->GetManualRefreshDiamond(pShop->GetLastDayManualRefreshNum()+1);//下一次的
	UINT32 consume_num = CVipMgr::GetInstance()->GetCostDiamond(EBUY_SHOP_REFRESH, pShop->GetLastDayManualRefreshNum()+1);//下一次的

	ASSERT(m_pUser);
	if (!m_pUser->IsConsumeEnough(EELEMENTTYPE_DIAMOND, 0, consume_num)){
		c_res.set_error_code(e_gsc_errorcode_no_enough_consume);
		return false;
	}

	stGameElement consume(EELEMENTTYPE_DIAMOND, 0, 0, stGameElement::DECREASE, consume_num);
	bool r = m_pUser->UpdateGameElement(consume, db_req, *c_res.mutable_consume(), ntf);
	ASSERT(r);

	pShop->IncreaseLastDayManualRefreshNum();

	pShop->ClearItems();//清理旧的道具数据

	//生成商店道具给用户
	r = CShopMgr::GetInstance()->FillItemsToEmptyShop(eShopType, m_mapWeightFactor, *pShop);
	ASSERT(r);
	if (!r){
		c_res.set_error_code(e_gsc_errorcode_unknown_error);
		return false;
	}

	//如果手动刷新操作时 如果发现要自动刷新一下 就先自动刷新一下 但是只更新时间 不更新道具
	UINT32 new_stamp = 0;
	if(pShop->IsNeedAutoRefresh(new_stamp)){
		pShop->iLastAutoRefreshTime = new_stamp;//只更新时间 这里不需要道具 以前已经手动更新道具
	}

	gs_dbs_user_info_op_req::struct_op_shop* op_shop = db_req.add_shops();
	op_shop->set_shop_op_type(gs_dbs_user_info_op_req::struct_op_shop::MANUAL_REFRESH);
	pShop->Fill(op_shop->mutable_shop(), c_res.mutable_shop());

	c_res.set_error_code(e_gsc_errorcode_success);
	return true;
}

bool CUserShopStrategy::CheckShopOpen( CShop::ESHOPTYPE eShopType, bool create_new_if_open_but_not_exsist, bool &is_open_and_create_new_shop)
{
	//printf("todo ！！！！！ bool IsShopOpen(ESHOPTYPE eShopType)\n");
	//return true;

	is_open_and_create_new_shop = false;

	//为NULL 不一定没开 不为NULL也不一定开了
	CShop* pShop = _get_shop(eShopType);// 为NULL 时正常渠道肯定没开 但是可能因为活动是开的

	UINT32 now = Now();

	switch (eShopType)
	{
	case CShop::ESHOPTYPE_COMMON:
		{
			UINT32 Shop_Common_Switch;
			bool r = CSysSettingMgr::GetInstance()->GetIntValue("Shop_Common_Switch", Shop_Common_Switch);
			ASSERT(r);

			if (Shop_Common_Switch == 0){
				return false;
			}

			if (!pShop && create_new_if_open_but_not_exsist){
				pShop = new CShop(eShopType);
				//pShop->eShopType = eShopType;
				pShop->iLastAutoRefreshTime = now;
				//pShop->iLastManualRefreshTime = 0;
				//pShop->iLastDayManualRefreshNum = 0;
				//pShop->iRandomOpenStartTimestamp = 0;	
				//pShop->iRandomOpenEndTimestamp = 0;
				bool r = CShopMgr::GetInstance()->FillItemsToEmptyShop(eShopType, m_mapWeightFactor, *pShop);
				ASSERT(r);
				m_userShopType2Shop[eShopType] = pShop;
				is_open_and_create_new_shop = true;
			}

			ASSERT(pShop);
			return pShop != NULL;
		}
		break;
	case CShop::ESHOPTYPE_QIWUGE:
		{
			UINT32 Shop_Qiwuge_Switch;
			bool r = CSysSettingMgr::GetInstance()->GetIntValue("Shop_Qiwuge_Switch", Shop_Qiwuge_Switch);
			ASSERT(r);

			if (Shop_Qiwuge_Switch == 0){
				return false;
			}

			//vip 等级
			UINT32 Vip_Level_Function_Qiwuge;
			r = CSysSettingMgr::GetInstance()->GetIntValue("Vip_Level_Function_Qiwuge", Vip_Level_Function_Qiwuge);
			ASSERT(r);

			//vip等级不够
			if (m_pUser->GetVipLevel() < Vip_Level_Function_Qiwuge){
				//return false;
				//是否在运营活动时间
				if (IsConditionsSatisfy(CSysSettingMgr::GetInstance()->Shop_Qiwuge_Open_Activity_time, m_pUser->UserID())){
					//ASSERT(pShop);

					//todo!!! 这里是否要新建 如果要新建 怎么通知客户端和db！
					////vip等级不够 且之前没有商店 这个时候要因为运营活动而创建商店！
					//if (!pShop){
					//	pShop = new CShop;
					//	pShop->eShopType = eShopType;
					//	pShop->iLastDayManualRefreshNum = 0;
					//	pShop->iLastAutoRefreshTime = now;
					//	pShop->iTotalManualRefreshNum = 0;
					//	bool r = CShopMgr::GetInstance()->FillItemsToEmptyShop(eShopType, m_mapWeightFactor, *pShop);
					//	ASSERT(r);
					//	m_userShopType2Shop[eShopType] = pShop;
					//}
					//printf("todo!!! 这里是否要新建 如果要新建 怎么通知客户端和db！\n");

					if (!pShop && create_new_if_open_but_not_exsist){
						pShop = new CShop(eShopType);
						//pShop->eShopType = eShopType;
						pShop->iLastAutoRefreshTime = now;
						//pShop->iLastManualRefreshTime = 0;
						//pShop->iLastDayManualRefreshNum = 0;
						//pShop->iRandomOpenStartTimestamp = 0;	
						//pShop->iRandomOpenEndTimestamp = 0;
						bool r = CShopMgr::GetInstance()->FillItemsToEmptyShop(eShopType, m_mapWeightFactor, *pShop);
						ASSERT(r);
						m_userShopType2Shop[eShopType] = pShop;
						is_open_and_create_new_shop = true;
					}

					return true;
				}
				else{
					//不在运营活动时间 就看概率到商店没
					if(!pShop){//还没有开过这个商店 如果概率到了 会新建商店的！
						return false;
					}
					else{//商店已经开过
						//是否在概率开放时间内
						return now >= pShop->iRandomOpenStartTimestamp && now <= pShop->iRandomOpenEndTimestamp;
					}
				}
			}
			else{
				if (!pShop && create_new_if_open_but_not_exsist){
					pShop = new CShop(eShopType);
					//pShop->eShopType = eShopType;
					pShop->iLastAutoRefreshTime = now;
					//pShop->iLastManualRefreshTime = 0;
					//pShop->iLastDayManualRefreshNum = 0;
					//pShop->iRandomOpenStartTimestamp = 0;	
					//pShop->iRandomOpenEndTimestamp = 0;
					bool r = CShopMgr::GetInstance()->FillItemsToEmptyShop(eShopType, m_mapWeightFactor, *pShop);
					ASSERT(r);
					m_userShopType2Shop[eShopType] = pShop;
					is_open_and_create_new_shop = true;
				}
				ASSERT(pShop);

				//客户端没有vip判断的过程 而是需要这个来判断是否开启
				if (pShop->iRandomOpenStartTimestamp){
					pShop->iRandomOpenStartTimestamp = 0;
				}

				if (pShop->iRandomOpenEndTimestamp){
					pShop->iRandomOpenEndTimestamp = 0;
				}
				return true;
			}
		}
		break;
	case CShop::ESHOPTYPE_YIJIEBAOKU:
		{
			UINT32 Shop_Yijiebaoku_Switch;
			bool r = CSysSettingMgr::GetInstance()->GetIntValue("Shop_Yijiebaoku_Switch", Shop_Yijiebaoku_Switch);
			ASSERT(r);

			if (Shop_Yijiebaoku_Switch == 0){
				return false;
			}

			//vip 等级
			UINT32 Vip_Level_Function_Yijiebaoku;
			r = CSysSettingMgr::GetInstance()->GetIntValue("Vip_Level_Function_Yijiebaoku", Vip_Level_Function_Yijiebaoku);
			ASSERT(r);

			//vip等级不够
			if (m_pUser->GetVipLevel() < Vip_Level_Function_Yijiebaoku){
				//return false;
				//是否在运营活动时间
				if (IsConditionsSatisfy(CSysSettingMgr::GetInstance()->Shop_Yijiebaoku_Open_Activity_time, m_pUser->UserID())){
					//ASSERT(pShop);

					//todo!!! 这里是否要新建 如果要新建 怎么通知客户端和db！
					////vip等级不够 且之前没有商店 这个时候要因为运营活动而创建商店！
					//if (!pShop){
					//	pShop = new CShop;
					//	pShop->eShopType = eShopType;
					//	pShop->iLastDayManualRefreshNum = 0;
					//	pShop->iLastAutoRefreshTime = now;
					//	pShop->iTotalManualRefreshNum = 0;
					//	bool r = CShopMgr::GetInstance()->FillItemsToEmptyShop(eShopType, m_mapWeightFactor, *pShop);
					//	ASSERT(r);
					//	m_userShopType2Shop[eShopType] = pShop;
					//}
					//printf("todo!!! 这里是否要新建 如果要新建 怎么通知客户端和db！\n");
					
					if (!pShop && create_new_if_open_but_not_exsist){
						pShop = new CShop(eShopType);
						//pShop->eShopType = eShopType;
						pShop->iLastAutoRefreshTime = now;
						//pShop->iLastManualRefreshTime = 0;
						//pShop->iLastDayManualRefreshNum = 0;
						//pShop->iRandomOpenStartTimestamp = 0;	
						//pShop->iRandomOpenEndTimestamp = 0;
						bool r = CShopMgr::GetInstance()->FillItemsToEmptyShop(eShopType, m_mapWeightFactor, *pShop);
						ASSERT(r);
						m_userShopType2Shop[eShopType] = pShop;
						is_open_and_create_new_shop = true;
					}
					return true;
				}
				else{
					//不在运营活动时间 就看概率到商店没
					if(!pShop){//还没有开过这个商店 如果概率到了 会新建商店的！
						return false;
					}
					else{//商店已经开过
						//是否在概率开放时间内
						return now >= pShop->iRandomOpenStartTimestamp && now <= pShop->iRandomOpenEndTimestamp;
					}
				}
			}
			else{
				if (!pShop && create_new_if_open_but_not_exsist){
					pShop = new CShop(eShopType);
					//pShop->eShopType = eShopType;
					pShop->iLastAutoRefreshTime = now;
					//pShop->iLastManualRefreshTime = 0;
					//pShop->iLastDayManualRefreshNum = 0;
					//pShop->iRandomOpenStartTimestamp = 0;	
					//pShop->iRandomOpenEndTimestamp = 0;
					bool r = CShopMgr::GetInstance()->FillItemsToEmptyShop(eShopType, m_mapWeightFactor, *pShop);
					ASSERT(r);
					m_userShopType2Shop[eShopType] = pShop;
					is_open_and_create_new_shop = true;
				}

				//客户端没有vip判断的过程 而是需要这个来判断是否开启
				if (pShop->iRandomOpenStartTimestamp){
					pShop->iRandomOpenStartTimestamp = 0;
				}

				if (pShop->iRandomOpenEndTimestamp){
					pShop->iRandomOpenEndTimestamp = 0;
				}

				ASSERT(pShop);
				return true;
			}
		}
		break;
	case CShop::ESHOPTYPE_ARENA:
		{
			UINT32 Shop_Arena_Switch;
			bool r = CSysSettingMgr::GetInstance()->GetIntValue("Shop_Arena_Switch", Shop_Arena_Switch);
			ASSERT(r);

			if (Shop_Arena_Switch == 0){
				return false;
			}

			//竞技场开放等级 就可以了 不需要竞技场商店开放等级！
			ASSERT(m_pUser);
			if (m_pUser->GetLevel() < CSysSettingMgr::GetInstance()->User_Level_Function_Arena){
				return false;
			}

			if (!pShop && create_new_if_open_but_not_exsist){
				pShop = new CShop(eShopType);
				//pShop->eShopType = eShopType;
				pShop->iLastAutoRefreshTime = now;
				//pShop->iLastManualRefreshTime = 0;
				//pShop->iLastDayManualRefreshNum = 0;
				//pShop->iRandomOpenStartTimestamp = 0;	
				//pShop->iRandomOpenEndTimestamp = 0;
				bool r = CShopMgr::GetInstance()->FillItemsToEmptyShop(eShopType, m_mapWeightFactor, *pShop);
				ASSERT(r);
				m_userShopType2Shop[eShopType] = pShop;
				is_open_and_create_new_shop = true;
			}
			ASSERT(pShop);
			return pShop != NULL;
		}
		break;
	case CShop::ESHOPTYPE_EXPEDITION:
		{
			UINT32 Shop_Expedition_Switch;
			bool r = CSysSettingMgr::GetInstance()->GetIntValue("Shop_Expedition_Switch", Shop_Expedition_Switch);
			ASSERT(r);

			if (Shop_Expedition_Switch == 0){
				return false;
			}

			ASSERT(m_pUser);
			if (m_pUser->GetLevel() < CSysSettingMgr::GetInstance()->User_Level_Function_Expedition)
			{
				return false;
			}
			if (!pShop && create_new_if_open_but_not_exsist)
			{
				pShop = new CShop(eShopType);
				pShop->iLastAutoRefreshTime = now;
				bool r = CShopMgr::GetInstance()->FillItemsToEmptyShop(eShopType, m_mapWeightFactor, *pShop);
				ASSERT(r);
				m_userShopType2Shop[eShopType] = pShop;
				is_open_and_create_new_shop = true;
			}
			ASSERT(pShop);
			return pShop != NULL;
		}
		break;
	}

	return false;
}

e_gsc_errorcode CUserShopStrategy::GetShopInfo(c_gs_shop_info_req::PULLTYPE get_type, user_shop_ESHOPTYPE shop_type, gs_dbs_user_info_op_req &db_req, ::google::protobuf::RepeatedPtrField< ::user_shop > &c_res )
{
	if (get_type == c_gs_shop_info_req::ONE){
		if ( !user_shop_ESHOPTYPE_IsValid(shop_type)){//是否是有效的商店类型
			//c_res.set_error_code(e_gsc_errorcode_unknown_error);
			return e_gsc_errorcode_unknown_error;
		}

		CShop::ESHOPTYPE eShopType = (CShop::ESHOPTYPE)shop_type;
		bool is_open_and_create_new_shop = false;
		if(CheckShopOpen(eShopType, true, is_open_and_create_new_shop)){
			CShop* pShop = _get_shop(eShopType);
			ASSERT(pShop);//check中如果开放了 需要创建就会创建！

			if (is_open_and_create_new_shop){//
				gs_dbs_user_info_op_req::struct_op_shop* op_shop = db_req.add_shops();
				op_shop->set_shop_op_type(gs_dbs_user_info_op_req::struct_op_shop::CREATE);
				pShop->Fill(op_shop->mutable_shop(), c_res.Add());
			}
			else{//不是新开的 则可能要做auto_refresh
				UINT32 new_stamp = 0;
				if(pShop->IsNeedAutoRefresh(new_stamp)){
					bool r = AutoRefreshShop(pShop, new_stamp, db_req, c_res.Add());//告诉db刷新商店！ 不需要在这里给客户端 下面会统一给
					ASSERT(r);
				}
				else{
					pShop->Fill(NULL, c_res.Add());
				}
			}
		}
		else{
			//return e_gsc_errorcode_shop_no_unlock;//商店还未解锁 单个商店没有拉到也不是给错误码 因为是数组 给空数组即可
		}
	}
	else if (get_type == c_gs_shop_info_req::ALL){
		for (UINT32 iShopType = CShop::ESHOPTYPE_MIN +1 ; iShopType < CShop::ESHOPTYPE_MAX ; ++iShopType){
			CShop::ESHOPTYPE eShopType = (CShop::ESHOPTYPE)iShopType;
			bool is_open_and_create_new_shop = false;
			if(CheckShopOpen(eShopType, true, is_open_and_create_new_shop)){
				CShop* pShop = _get_shop(eShopType);
				ASSERT(pShop);

				if (is_open_and_create_new_shop){//
					gs_dbs_user_info_op_req::struct_op_shop* op_shop = db_req.add_shops();
					op_shop->set_shop_op_type(gs_dbs_user_info_op_req::struct_op_shop::CREATE);
					pShop->Fill(op_shop->mutable_shop(), c_res.Add());
				}
				else{//不是新开的 则可能要做auto_refresh
					UINT32 new_stamp = 0;
					if(pShop->IsNeedAutoRefresh(new_stamp)){
						bool r = AutoRefreshShop(pShop, new_stamp, db_req, c_res.Add());//告诉db刷新商店！ 不需要在这里给客户端 下面会统一给
						ASSERT(r);
					}
					else{
						pShop->Fill(NULL, c_res.Add());
					}
				}
			}
		}
	}
	else{
		//c_res.set_error_code(e_gsc_errorcode_unknown_error);
		return e_gsc_errorcode_unknown_error;
	}

	//c_res.set_error_code(e_gsc_errorcode_success);
	return e_gsc_errorcode_success;
}

//void CUserShopStrategy::GetShopInfo( ESHOPTYPE eShopType, CShop* &ret_shop, bool &is_open, bool & is_new_create, db_req, c_res )
//{
//	printf("todo ！！！！！ bool IsShopOpen(ESHOPTYPE eShopType)\n");
//
//	ret_shop = NULL;
//	is_open = false;
//	is_new_create = false;
//
//	//为NULL 不一定没开 不为NULL也不一定开了
//	ret_shop = _get_shop(eShopType);// 为NULL 时正常渠道肯定没开 但是可能因为活动是开的
//	//ret_shop = _get_shop(eShopType);
//
//	UINT32 now = Now();
//
//	switch (eShopType)
//	{
//	case ESHOPTYPE_COMMON:
//		{
//			UINT32 Shop_Common_Switch;
//			bool r = CSysSettingMgr::GetInstance()->GetIntValue("Shop_Common_Switch", Shop_Common_Switch);
//			ASSERT(r);
//
//			if (Shop_Common_Switch == 0){
//				return;
//			}
//
//			ASSERT(ret_shop);
//			//return ret_shop != NULL;
//			is_open = true;
//			//is_new_create = false;//在新建玩家的时候就已经创建了
//		}
//		break;
//	case ESHOPTYPE_QIWUGE:
//		{
//			UINT32 Shop_Qiwuge_Switch;
//			bool r = CSysSettingMgr::GetInstance()->GetIntValue("Shop_Qiwuge_Switch", Shop_Qiwuge_Switch);
//			ASSERT(r);
//
//			if (Shop_Qiwuge_Switch == 0){
//				return;
//			}
//
//			//vip 等级
//			UINT32 Vip_Level_Function_Qiwuge;
//			bool r = CSysSettingMgr::GetInstance()->GetIntValue("Vip_Level_Function_Qiwuge", Vip_Level_Function_Qiwuge);
//			ASSERT(r);
//
//			//vip等级不够
//			if (m_pUser->GetVipLevel() < Vip_Level_Function_Qiwuge){
//				//return false;
//				//是否在运营活动时间
//				if (IsConditionsSatisfy(CSysSettingMgr::GetInstance()->Shop_Qiwuge_Open_Activity_time, m_pUser->UserID())){
//					//ASSERT(ret_shop);
//
//					//vip等级不够 且之前没有商店 这个时候要因为运营活动而创建商店！
//					if (!ret_shop){
//						ret_shop = new CShop;
//						ret_shop->eShopType = eShopType;
//						ret_shop->iLastDayManualRefreshNum = 0;
//						ret_shop->iLastAutoRefreshTime = now;
//						//ret_shop->iTotalManualRefreshNum = 0;
//						bool r = CShopMgr::GetInstance()->FillItemsToEmptyShop(eShopType, m_mapWeightFactor, *ret_shop);
//						ASSERT(r);
//						m_userShopType2Shop[eShopType] = ret_shop;
//					}
//
//					is_open = true;
//					is_new_create = true;
//					return;
//				}
//				else{
//					//不在运营活动时间 就看概率到商店没
//					if(!ret_shop){//还没有开过这个商店 如果概率到了 会新建商店的！
//						return;
//					}
//					else{//商店已经开过
//						//是否在概率开放时间内
//						is_open = ( now >= ret_shop->iRandomOpenStartTimestamp && now <= ret_shop->iRandomOpenEndTimestamp );
//						return;
//					}
//				}
//			}
//			else{
//				is_open = true;
//			}
//		}
//		break;
//	case ESHOPTYPE_YIJIEBAOKU:
//		{
//			UINT32 Shop_Yijiebaoku_Switch;
//			bool r = CSysSettingMgr::GetInstance()->GetIntValue("Shop_Yijiebaoku_Switch", Shop_Yijiebaoku_Switch);
//			ASSERT(r);
//
//			if (Shop_Yijiebaoku_Switch == 0){
//				return;
//			}
//
//			//vip 等级
//			UINT32 Vip_Level_Function_Yijiebaoku;
//			bool r = CSysSettingMgr::GetInstance()->GetIntValue("Vip_Level_Function_Yijiebaoku", Vip_Level_Function_Yijiebaoku);
//			ASSERT(r);
//
//			//vip等级不够
//			if (m_pUser->GetVipLevel() < Vip_Level_Function_Yijiebaoku){
//				//return false;
//				//是否在运营活动时间
//				if (IsConditionsSatisfy(CSysSettingMgr::GetInstance()->Shop_Yijiebaoku_Open_Activity_time, m_pUser->UserID())){
//					//ASSERT(ret_shop);
//
//					//vip等级不够 且之前没有商店 这个时候要因为运营活动而创建商店！
//					if (!ret_shop){
//						ret_shop = new CShop;
//						ret_shop->eShopType = eShopType;
//						ret_shop->iLastDayManualRefreshNum = 0;
//						ret_shop->iLastAutoRefreshTime = now;
//						ret_shop->iTotalManualRefreshNum = 0;
//						bool r = CShopMgr::GetInstance()->FillItemsToEmptyShop(eShopType, m_mapWeightFactor, *ret_shop);
//						ASSERT(r);
//						m_userShopType2Shop[eShopType] = ret_shop;
//					}
//
//					is_open = true;
//					is_new_create = true;
//					return;
//				}
//				else{
//					//不在运营活动时间 就看概率到商店没
//					if(!ret_shop){//还没有开过这个商店 如果概率到了 会新建商店的！
//						return;
//					}
//					else{//商店已经开过
//						//是否在概率开放时间内
//						//return now >= ret_shop->iRandomOpenStartTimestamp && now <= ret_shop->iRandomOpenEndTimestamp;
//						is_open = ( now >= ret_shop->iRandomOpenStartTimestamp && now <= ret_shop->iRandomOpenEndTimestamp );
//						return;
//					}
//				}
//			}
//			else{
//				return true;
//			}
//		}
//		break;
//	case ESHOPTYPE_ARENA:
//		{
//			UINT32 Shop_Arena_Switch;
//			bool r = CSysSettingMgr::GetInstance()->GetIntValue("Shop_Arena_Switch", Shop_Arena_Switch);
//			ASSERT(r);
//
//			if (Shop_Arena_Switch == 0){
//				return;
//			}
//		}
//		break;
//	case ESHOPTYPE_EXPEDITION:
//		{
//			UINT32 Shop_Expedition_Switch;
//			bool r = CSysSettingMgr::GetInstance()->GetIntValue("Shop_Expedition_Switch", Shop_Expedition_Switch);
//			ASSERT(r);
//
//			if (Shop_Expedition_Switch == 0){
//				return;
//			}
//		}
//		break;
//
//	}
//
//	return;
//}

bool CUserShopStrategy::OnRandomOpen( CTask::ETASKTYPE eTaskType, gs_dbs_user_info_op_req &db_req, user_shop &c_shop )
{
	CShop::ESHOPTYPE eShopType;
	UINT32 iOpenProbability = 0;
	UINT32 iTimeLimit = 0;
	switch (eTaskType)
	{
	case CTask::ETASKTYPE_MAIN:
		{
			eShopType = CShop::ESHOPTYPE_QIWUGE;
			if(IsShopOpenForever(eShopType)){//永久开启 不需要随机
				return false;
			}

			bool r = CSysSettingMgr::GetInstance()->GetIntValue("Shop_Qiwuge_Open_Probability", iOpenProbability);
			ASSERT(r);

			r = CSysSettingMgr::GetInstance()->GetIntValue("Shop_Qiwuge_Open_Time_Limit", iTimeLimit);
			ASSERT(r);
		}
		break;
	case CTask::ETASKTYPE_BRANCH:
		{
			eShopType = CShop::ESHOPTYPE_YIJIEBAOKU;
			if(IsShopOpenForever(eShopType)){//永久开启 不需要随机
				return false;
			}

			bool r = CSysSettingMgr::GetInstance()->GetIntValue("Shop_Yijiebaoku_Open_Probability", iOpenProbability);
			ASSERT(r);

			r = CSysSettingMgr::GetInstance()->GetIntValue("Shop_Yijiebaoku_Open_Time_Limit", iTimeLimit);
			ASSERT(r);
		}
		break;
	default:
		ASSERT(false && "OnRandomOpen不支持的任务类型");
		return false;
	}

	UINT32 race = CRandom::GetInstance()->GetRandom(0, MAXNUM_PROBABILITY);
	if (race <= iOpenProbability){//随机到了商店
		UINT32 now = Now();
		UINT32 end = now + iTimeLimit;
		CShop* pShop = _get_shop(eShopType);
		if (pShop){
			pShop->iRandomOpenStartTimestamp = now;
			pShop->iRandomOpenEndTimestamp = end;

			//判断是否此次应该刷新
			UINT32 new_stamp = 0;//
			if(pShop->IsNeedAutoRefresh(new_stamp)){
				bool r = AutoRefreshShop(pShop, new_stamp, db_req, &c_shop);
				ASSERT(r);
			}
			else{
				gs_dbs_user_info_op_req::struct_op_shop* op_shop = db_req.add_shops();
				op_shop->set_shop_op_type(gs_dbs_user_info_op_req::struct_op_shop::RANDOM_REFRESH);
				pShop->Fill(op_shop->mutable_shop(), &c_shop);
			}
		}
		else{
			pShop = new CShop(eShopType);
			//pShop->eShopType = eShopType;
			pShop->iLastAutoRefreshTime = now;
			//pShop->iLastManualRefreshTime = 0;
			//pShop->iLastDayManualRefreshNum = 0;
			pShop->iRandomOpenStartTimestamp = now;
			pShop->iRandomOpenEndTimestamp = end;
			bool r = CShopMgr::GetInstance()->FillItemsToEmptyShop(eShopType, m_mapWeightFactor, *pShop);
			ASSERT(r);
			m_userShopType2Shop[eShopType] = pShop;
			
			gs_dbs_user_info_op_req::struct_op_shop* op_shop = db_req.add_shops();
			op_shop->set_shop_op_type(gs_dbs_user_info_op_req::struct_op_shop::CREATE);
			pShop->Fill(op_shop->mutable_shop(), &c_shop);
		}

		return true;//随机到了商店
	}

	return false;//没有随机到商店
}

bool CUserShopStrategy::IsShopOpenForever( CShop::ESHOPTYPE eShopType ){
	switch (eShopType)
	{
	case CShop::ESHOPTYPE_COMMON:
		return true;
		break;
	case CShop::ESHOPTYPE_QIWUGE:
		{
			//vip 等级
			UINT32 Vip_Level_Function_Qiwuge;
			bool r = CSysSettingMgr::GetInstance()->GetIntValue("Vip_Level_Function_Qiwuge", Vip_Level_Function_Qiwuge);
			ASSERT(r);

			//vip等级不够
			if (m_pUser->GetVipLevel() >= Vip_Level_Function_Qiwuge){
				return true;
			}
		}
		break;
	case CShop::ESHOPTYPE_YIJIEBAOKU:
		{
			//vip 等级
			UINT32 Vip_Level_Function_Yijiebaoku;
			bool r = CSysSettingMgr::GetInstance()->GetIntValue("Vip_Level_Function_Yijiebaoku", Vip_Level_Function_Yijiebaoku);
			ASSERT(r);

			//vip等级不够
			if (m_pUser->GetVipLevel() >= Vip_Level_Function_Yijiebaoku){
				return true;
			}
		}
		break;
	default:
		ASSERT(false && "IsShopOpenForever暂不支持的商店类型");
		return false;
	}

	return false;
}
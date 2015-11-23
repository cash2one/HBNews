#ifndef user_shop_strategy_h__
#define user_shop_strategy_h__

#include "def.h"
#include "config.h"
#include "shop_mgr.h"
#include "dbs_gs_proto.pb.h"
#include "task_mgr.h"

class CUser;

class CUserShopStrategy{

public:
	bool Init(CUser* pUser, const ::google::protobuf::RepeatedPtrField< ::db_user_shop >& shop_data, const ::google::protobuf::RepeatedPtrField< ::db_user_shop_buy >&  shop_buy_data);

	//返回值为是否新创建
	//void GetShopInfo(ESHOPTYPE eShopType, CShop* &ret_shop, bool &is_open, bool & is_new_create, db_req, c_shop);
	//当前不能使用的商店 不出现在这里  受全局开关 运营活动 vip等级  随机商店限时 等影响
	//bool GetShopInfo(const c_gs_shop_info_req &req, gs_dbs_user_info_op_req &db_req, gs_c_shop_info_res &c_res);
	e_gsc_errorcode GetShopInfo(c_gs_shop_info_req::PULLTYPE get_type, user_shop_ESHOPTYPE shop_type, gs_dbs_user_info_op_req &db_req, ::google::protobuf::RepeatedPtrField< ::user_shop > &c_res );

	//bool Buy(EELEMENTTYPE eType, UINT32 iID, UINT32 iNum);
	bool Buy( CShop::ESHOPTYPE eShopType, UINT32 iPos, gs_dbs_user_info_op_req& db_req, gs_c_shop_buy_item_res &c_res, GS_C_NTF &ntf );
	bool ManualRefreshShop( CShop::ESHOPTYPE eShopType, gs_dbs_user_info_op_req &db_req, gs_c_shop_refresh_res &c_res, GS_C_NTF ntf );

	//判断是否达到了永久开启条件
	bool IsShopOpenForever(CShop::ESHOPTYPE eShopType);
	//判断商店是否打开 如果已经打开 但是商店还不存在时 可以选择是否创建
	bool CheckShopOpen( CShop::ESHOPTYPE eShopType, bool create_new_if_open_but_not_exsist, bool &is_open_and_create_new_shop);
	bool AutoRefreshShop( CShop::ESHOPTYPE eShopType, UINT32 new_stamp, gs_dbs_user_info_op_req &db_req, user_shop *c_shop );
	bool AutoRefreshShop( CShop* pShop, UINT32 new_stamp, gs_dbs_user_info_op_req &db_req, user_shop *c_shop );

	//副本结束 或者 扫荡的时候 有几率触发商店
	bool OnRandomOpen(CTask::ETASKTYPE eTaskType, gs_dbs_user_info_op_req &db_req, user_shop &c_shop);

	//升级vip的时候 可能会创建商店
	bool OnVipLevelUp(UINT32 from, UINT32 to, gs_dbs_user_info_op_req &db_req, user_shop &c_shop){
		return false;
	}
private:


	CShop* _get_shop( CShop::ESHOPTYPE eShopType){
		std::map<CShop::ESHOPTYPE, CShop*>::iterator it = m_userShopType2Shop.find(eShopType);
		if (it != m_userShopType2Shop.end()){
			return it->second;
		}

		return NULL;
	}

	CUser* m_pUser;
	//std::map<ESHOPTYPE, std::vector<stShopItem>> m_userShopItems;	//shopid -> shopitems
	std::map<CShop::ESHOPTYPE, CShop*> m_userShopType2Shop;	//shopid -> shopitems
	std::map<std::pair<EELEMENTTYPE, UINT32>, CShopItemWeightFactor*> m_mapWeightFactor;// 玩家的购买信息
};
#endif // user_shop_strategy_h__
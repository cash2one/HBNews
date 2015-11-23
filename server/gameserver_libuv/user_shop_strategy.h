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

	//����ֵΪ�Ƿ��´���
	//void GetShopInfo(ESHOPTYPE eShopType, CShop* &ret_shop, bool &is_open, bool & is_new_create, db_req, c_shop);
	//��ǰ����ʹ�õ��̵� ������������  ��ȫ�ֿ��� ��Ӫ� vip�ȼ�  ����̵���ʱ ��Ӱ��
	//bool GetShopInfo(const c_gs_shop_info_req &req, gs_dbs_user_info_op_req &db_req, gs_c_shop_info_res &c_res);
	e_gsc_errorcode GetShopInfo(c_gs_shop_info_req::PULLTYPE get_type, user_shop_ESHOPTYPE shop_type, gs_dbs_user_info_op_req &db_req, ::google::protobuf::RepeatedPtrField< ::user_shop > &c_res );

	//bool Buy(EELEMENTTYPE eType, UINT32 iID, UINT32 iNum);
	bool Buy( CShop::ESHOPTYPE eShopType, UINT32 iPos, gs_dbs_user_info_op_req& db_req, gs_c_shop_buy_item_res &c_res, GS_C_NTF &ntf );
	bool ManualRefreshShop( CShop::ESHOPTYPE eShopType, gs_dbs_user_info_op_req &db_req, gs_c_shop_refresh_res &c_res, GS_C_NTF ntf );

	//�ж��Ƿ�ﵽ�����ÿ�������
	bool IsShopOpenForever(CShop::ESHOPTYPE eShopType);
	//�ж��̵��Ƿ�� ����Ѿ��� �����̵껹������ʱ ����ѡ���Ƿ񴴽�
	bool CheckShopOpen( CShop::ESHOPTYPE eShopType, bool create_new_if_open_but_not_exsist, bool &is_open_and_create_new_shop);
	bool AutoRefreshShop( CShop::ESHOPTYPE eShopType, UINT32 new_stamp, gs_dbs_user_info_op_req &db_req, user_shop *c_shop );
	bool AutoRefreshShop( CShop* pShop, UINT32 new_stamp, gs_dbs_user_info_op_req &db_req, user_shop *c_shop );

	//�������� ���� ɨ����ʱ�� �м��ʴ����̵�
	bool OnRandomOpen(CTask::ETASKTYPE eTaskType, gs_dbs_user_info_op_req &db_req, user_shop &c_shop);

	//����vip��ʱ�� ���ܻᴴ���̵�
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
	std::map<std::pair<EELEMENTTYPE, UINT32>, CShopItemWeightFactor*> m_mapWeightFactor;// ��ҵĹ�����Ϣ
};
#endif // user_shop_strategy_h__
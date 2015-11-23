#include "pay_handle.h"
#include "order.h"
#include "db_async_command.h"
#define CHECK_KEY_RETURN_FALSE(var_map, key) if (var_map.find(key) == var_map.end()){return false;}

bool CPayHandle::AdwoPaied( webserver::http_request* r )
{
	/*
	newPrice
	֧�� �۸�
	oldPrice
	ԭʼ �۸�
	payMoney
	֧�� ���
	orderCode
	����
	cpCode
	CP �ඩ���� �ඩ���� ������Ϊ�� ������Ϊ�� ������Ϊ��
	commodityId
	��Ʒ id
	orderCount
	���� ����
	commodityInfo
	��Ʒ ����
	currentCount
	֧�� ֮ǰ��Ʒ���� ֮ǰ��Ʒ���� ֮ǰ��Ʒ���� ֮ǰ��Ʒ����
	reqPayMoney
	���� ���
	createTime
	֧�� ����ʱ�� ����ʱ�� ����ʱ��
	sign
	����ֵ�� ����ֵ�� AppKey AppKey AppKey AppKey �� MD5 MD5 ֵ
	HashToMD5HexHashToMD5Hex HashToMD5HexHashToMD5HexHashToMD5Hex HashToMD5HexHashToMD5HexHashToMD5HexHashToMD5HexHashToMD5Hex(String.format("{0}{1}{2}"String.format("{0}{1}{2}"String.format("{0}{1}{2}"String.format("{0}{1}{2}"String.format("{0}{1}{2}" String.format("{0}{1}{2}" String.format("{0}{1}{2}"String.format("{0}{1}{2}" String.format("{0}{1}{2}"String.format("{0}{1}{2}"String.format("{0}{1}{2}" String.format("{0}{1}{2}" String.format("{0}{1}{2}" String.format("{0}{1}{2}",appId, ,appId, orderSerial,AppKey))
	*/

	CHECK_KEY_RETURN_FALSE(r->params_, "newPrice");
	CHECK_KEY_RETURN_FALSE(r->params_, "oldPrice");
	CHECK_KEY_RETURN_FALSE(r->params_, "payMoney");
	CHECK_KEY_RETURN_FALSE(r->params_, "orderCode");
	CHECK_KEY_RETURN_FALSE(r->params_, "cpCode");
	CHECK_KEY_RETURN_FALSE(r->params_, "commodityId");
	CHECK_KEY_RETURN_FALSE(r->params_, "orderCount");
	CHECK_KEY_RETURN_FALSE(r->params_, "commodityInfo");
	CHECK_KEY_RETURN_FALSE(r->params_, "currentCount");
	CHECK_KEY_RETURN_FALSE(r->params_, "reqPayMoney");
	CHECK_KEY_RETURN_FALSE(r->params_, "createTime");
	CHECK_KEY_RETURN_FALSE(r->params_, "sign");

	r->params_.find("newPrice") != r->params_.end();

	CCommodity::EPLANTFROM plantform_id = CCommodity::EPLANTFROM::ADWO;
	const std::string order_id = r->params_["orderCode"];
	UINT32 create_time = atoi(r->params_["createTime"].c_str());
	COrder::ESTATUS status = COrder::ESTATUS::JUST_PAIED;
	UINT32 commodity_id = atoi(r->params_["commodityId"].c_str());
	UINT32 commodity_num = atoi(r->params_["orderCount"].c_str());
	float present_price = atof(r->params_["newPrice"].c_str());
	float original_price = atof(r->params_["oldPrice"].c_str());

	//�ж���Ʒid �� ƽ̨�Ƿ���� ����һ�£�
	CCommodity* c = CCommodityMgr::GetInstance()->GetCommodityByID(commodity_id);
	if (!c || c->pay_plantform != plantform_id){
		return false;
	}

	//todo!!!
	UINT32 user_id = 1;
	UINT16 area_id = 1;

	COrder* pOrder = new COrder(plantform_id, 
		order_id, 
		area_id,
		user_id,
		create_time, 
		status, 
		commodity_id, 
		commodity_num, 
		present_price, 
		original_price);                                                                                                                                                          

	COrderNotifyCommand* pCom = new COrderNotifyCommand(COrderNotifyCommand::ORDER_NOTIFY, pOrder);
	CAsyncCommandModule::GetInstance()->AddCommand(pCom);
	return true;
}
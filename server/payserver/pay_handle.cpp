#include "pay_handle.h"
#include "order.h"
#include "db_async_command.h"
#define CHECK_KEY_RETURN_FALSE(var_map, key) if (var_map.find(key) == var_map.end()){return false;}

bool CPayHandle::AdwoPaied( webserver::http_request* r )
{
	/*
	newPrice
	支付 价格
	oldPrice
	原始 价格
	payMoney
	支付 金额
	orderCode
	订单
	cpCode
	CP 侧订单号 侧订单号 ，可以为空 ，可以为空 ，可以为空
	commodityId
	商品 id
	orderCount
	订购 数量
	commodityInfo
	商品 描述
	currentCount
	支付 之前商品数量 之前商品数量 之前商品数量 之前商品数量
	reqPayMoney
	请求 金额
	createTime
	支付 请求时间 请求时间 请求时间
	sign
	参数值与 参数值与 AppKey AppKey AppKey AppKey 的 MD5 MD5 值
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

	//判断商品id 和 平台是否存在 并且一致！
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
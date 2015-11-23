#ifndef commodity_mgr_h__
#define commodity_mgr_h__

#include <string>
#include <vector>
#include <map>
#include "def.h"

struct stGameElement{
	UINT32 type;//元素类型 参考gs_c的协议配置
	UINT32 id;
	UINT32 num; //这里的数字是增量

	stGameElement(UINT32 iType, UINT32 iID, UINT32 iNum):type(iType), id(iID), num(iNum){

	}
};

class CCommodity{
public:
	enum EPLANTFROM{
		MIN = 0,
		ADWO = 1,//安沃
		ALIPAY = 2,//支付宝
		MAX = 3
	};

	UINT32 id;
	EPLANTFROM pay_plantform;
	float priority;
	std::string name;
	float present_price;
	float original_price;
	std::vector<stGameElement> eles;
};

class CCommodityMgr{

public:

	//todo 单例工具
	inline static CCommodityMgr* GetInstance(){
		static CCommodityMgr singleton;
		return &singleton;
	}

	bool Init();

	CCommodity* GetCommodityByID(UINT32 iCommodityID);
	std::vector<CCommodity*>& GetCommodityOfPlantform(CCommodity::EPLANTFROM ePlantform);

private:
	std::map<UINT32, CCommodity*> m_mapCCommodity;
	std::map<CCommodity::EPLANTFROM, std::vector<CCommodity*>> m_mapPlantformCCommodity;
};

#endif // commodity_mgr_h__
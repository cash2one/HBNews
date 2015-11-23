#ifndef commodity_mgr_h__
#define commodity_mgr_h__

#include <string>
#include <vector>
#include <map>
#include "def.h"

struct stGameElement{
	UINT32 type;//Ԫ������ �ο�gs_c��Э������
	UINT32 id;
	UINT32 num; //���������������

	stGameElement(UINT32 iType, UINT32 iID, UINT32 iNum):type(iType), id(iID), num(iNum){

	}
};

class CCommodity{
public:
	enum EPLANTFROM{
		MIN = 0,
		ADWO = 1,//����
		ALIPAY = 2,//֧����
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

	//todo ��������
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
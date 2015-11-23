#ifndef sign_mgr_h__
#define sign_mgr_h__
#include <map>
#include "def.h"

struct stGameElement;
class CSignMgr
{
public:
	static CSignMgr* GetInstance(){
		static CSignMgr singleton;
		return &singleton;
	}

	bool Init();
	
	//根据次数获取奖励信息
	stGameElement* GetSign1Reward(UINT32 iYear, UINT32 iMonth, UINT32 iTimes);

	//根据累计签到类型获取奖励信息
	stGameElement* GetSignNReward(UINT32 iYear, UINT32 iMonth, UINT32 iType);

private:
	CSignMgr(void){};
	std::map<std::pair<UINT32, UINT32>, std::vector<stGameElement>> m_mapSign1Time2Rewards;//first : 年月 second:每日奖励集合
	std::map<std::pair<UINT32, UINT32>, std::vector<stGameElement>> m_mapSignNTime2Rewards;//first: 年月 second:累计签到奖励集合

};

#endif
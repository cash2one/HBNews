#ifndef user_mgr_h__
#define user_mgr_h__

#pragma once

#include <map>
#include <vector>
#include <string>

#include "user.h"

#include "def.h"

class db_user_info;

struct stUserLevelConfig{
	UINT32 iLevel;
	UINT32 iExpForNextLevel;//升到此级 所需要的经验！
	//UINT32 iMaxHealth;
};

//只允许在server_for_user线程中使用！
class CUserMgr{
public:
	//todo 单例工具
	inline static CUserMgr* GetInstance(){
		static CUserMgr singleton;
		return &singleton;
	}

	bool Init();

	bool UnInit();

	CUser* GetUserByUserID(UINT32 iUserID);
	CUser* GetUserByName(const std::string& name);
	bool AddUser(CUser* pUser);
	bool RemoveUser(UINT32 user_id);

	std::string GetPlantformAccount(UINT32 platform_id, UINT8 platform_sub_id, const char* raw_account);
	static UINT32 GetMaxHealth(UINT32 level);
	static UINT32 GetMaxHealthPro();

	//static UINT32 GetMaxSkillPoint(UINT32 iVipLevel){
	//	return MAXNUM_SKILLPOINT;
	//}

	//填充新用户信息
	static bool InitNewUser(db_user_info* user_info, UINT32 user_id);

	std::vector<stGameElement>& GetInitPlayerBelongings(){
		return m_initPlayerBelongings;
	}

	UINT32 IncreaseExp(const UINT32 cur_level, const UINT32 cur_exp, UINT32 add_exp, UINT32& up_level_num, UINT32& new_cur_exp );
	

	/************************************************************************/
	/* 
	玩家等级提升时，按照提升之前的等级给与一定体力，参数详见sys_setting-Health_Present_Factor_When_User_Level_Up参数
	例1：
	如果玩家从3级升级到4级，则补充3*2点体力
	例2：
	玩家从3级直接提升到5级，则一次性补充3*2+4*2=14点体力
	*/
	/************************************************************************/
	UINT32 GetHealthPrizeOfLevelUp(UINT32 from, UINT32 to);

	std::map<UINT32, CUser*>* GetOnlineUsers(){
		return &m_mapUserID2User;
	}
private:

	CUserMgr(){}
	
	std::map<UINT32, CUser*> m_mapUserID2User;
	std::map<UINT32, stUserLevelConfig*> m_mapLevel2UserCfg;
	std::vector<stGameElement> m_initPlayerBelongings;	//玩家初始财产
};

#endif // user_mgr_h__
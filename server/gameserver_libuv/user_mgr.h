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
	UINT32 iExpForNextLevel;//�����˼� ����Ҫ�ľ��飡
	//UINT32 iMaxHealth;
};

//ֻ������server_for_user�߳���ʹ�ã�
class CUserMgr{
public:
	//todo ��������
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

	//������û���Ϣ
	static bool InitNewUser(db_user_info* user_info, UINT32 user_id);

	std::vector<stGameElement>& GetInitPlayerBelongings(){
		return m_initPlayerBelongings;
	}

	UINT32 IncreaseExp(const UINT32 cur_level, const UINT32 cur_exp, UINT32 add_exp, UINT32& up_level_num, UINT32& new_cur_exp );
	

	/************************************************************************/
	/* 
	��ҵȼ�����ʱ����������֮ǰ�ĵȼ�����һ���������������sys_setting-Health_Present_Factor_When_User_Level_Up����
	��1��
	�����Ҵ�3��������4�����򲹳�3*2������
	��2��
	��Ҵ�3��ֱ��������5������һ���Բ���3*2+4*2=14������
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
	std::vector<stGameElement> m_initPlayerBelongings;	//��ҳ�ʼ�Ʋ�
};

#endif // user_mgr_h__
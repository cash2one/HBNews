#include <stdlib.h>
#include <stdio.h>

#include "def.h"

#include "db_mgr.h"
#include "server_for_user.h"
#include "client_for_as.h"
#include "client_for_dbs.h"
#include "server_for_gm.h"
#include "client_for_ps.h"

#include "burst_mgr.h"
#include "map_mgr.h"
#include "task_mgr.h"
#include "item_mgr.h"
#include "equip_mgr.h"
#include "skill_mgr.h"
#include "hero_mgr.h"

#include "user_mgr.h"


#include "game_config.h"
#include "client_config_mgr.h"
#include "mail_mgr.h"

#include "sys_setting_mgr.h"
#include "vip_mgr.h"
#include "invoke_mgr.h"
#include "shop_mgr.h"
#include "mission_mgr.h"
#include "sign_mgr.h"
#include "novice_guide_mgr.h"
#include "marquee_mgr.h"
#include "forbid_word_mgr.h"
#include "friend_mgr.h"
#include "rank_mgr.h"
#include "crusade_mgr.h"
int main()
{
	bool r = false;
	//��һ˳λ�� �ʼ��ʼ�������ļ�
	r = GameConfig::GetInstance()->Init();
	ASSERT(r && "�������������ó�ʼ��ʧ�ܣ�");

	//�ڶ�˳λ��
	const DBConfig& cfg = GameConfig::GetInstance()->GetDBConfig();
	r = CDBMgr::GetInstance()->Init(cfg.m_host.c_str(), cfg.m_port, cfg.m_user.c_str(), cfg.m_passwd.c_str(), cfg.m_db.c_str());
	ASSERT(r && "���ݿ�ģ���ʼ��ʧ�ܣ�");

	//��ȡ�ͻ�����Ҫ�������ļ�
	r = CClientConfigMgr::GetInstance()->Init();
	ASSERT(r && "�ͻ������ó�ʼ��ʧ�ܣ�");

	r = CSysSettingMgr::GetInstance()->Init();
	ASSERT(r && "��������Ϸ���ó�ʼ��ʧ�ܣ�");

	r = CItemMgr::GetInstance()->Init();
	ASSERT(r && "itemģ���ʼ��ʧ�ܣ�");

	r = CEquipMgr::GetInstance()->Init();
	ASSERT(r && "equipģ���ʼ��ʧ�ܣ�");

	r = CSkillMgr::GetInstance()->Init();
	ASSERT(r && "skillģ���ʼ��ʧ�ܣ�");
	
	r = CHeroMgr::GetInstance()->Init();
	ASSERT(r && "heroģ���ʼ��ʧ�ܣ�");

	r = CFightMgr::GetInstance()->Init();
	ASSERT(r && "fightģ���ʼ��ʧ�ܣ�");

	r = CBurstMgr::GetInstance()->Init();
	ASSERT(r && "burstģ���ʼ��ʧ�ܣ�");

	r = CInvokeMgr::GetInstance()->Init();
	ASSERT(r && "Invoke��̳ģ���ʼ��ʧ�ܣ�");

	r = CMapMgr::GetInstance()->Init();
	ASSERT(r && "mapģ���ʼ��ʧ�ܣ�");

	r = CTaskMgr::GetInstance()->Init();
	ASSERT(r && "taskģ���ʼ��ʧ�ܣ�");

	r = CVipMgr::GetInstance()->Init();
	ASSERT(r && "vipģ���ʼ��ʧ��!");

	r = CUserMgr::GetInstance()->Init();
	ASSERT(r && "���ģ���ʼ��ʧ��!");

	r = CShopMgr::GetInstance()->Init();
	ASSERT(r && "�̵�ģ���ʼ��ʧ��!");
	
	r = CMissionMgr::GetInstance()->Init();
	ASSERT(r && "����ģ���ʼ��ʧ��!");

	r = CArena::GetInstance()->Init();
	ASSERT(r && "������ģ���ʼ��ʧ��!");

	r = CSignMgr::GetInstance()->Init();
	ASSERT(r && "ǩ��ģ���ʼ��ʧ�ܣ�");
	
	r = CNoviceGuideMgr::GetInstance()->Init();
	ASSERT(r  && "���������ʼ��ʧ��!");
	
	r = CMarqueeMgr::GetInstance()->Init();
	ASSERT(r && "����Ƴ�ʼ��ʧ��");
	
	r = CForbidWordMgr::GetInstance()->Init();
	ASSERT(r && "���δʻ����ʧ��");
	
	r = CFriendMgr::GetInstance()->Init();
	ASSERT(r && "����ģ�����ʧ��");
	
	r = CRankMgr::GetInstance()->Init();
	ASSERT(r && "���м���ʧ��");

	r = CrusadeMgr::GetInstance()->Init();
	ASSERT(r && "Զ��ģ���ʼ��ʧ��");

	//����dbs�����߳�
	r = CClientForDBS::GetInstance()->Init();
	ASSERT(r && "CClientForDBSģ���ʼ��ʧ�ܣ�");
	r = CClientForDBS::GetInstance()->Start();
	ASSERT(r && "CClientForDBSģ������ʧ�ܣ�");

	//����as�����߳�
	r = CClientForAS::GetInstance()->Init();
	ASSERT(r && "CClientForASģ���ʼ��ʧ�ܣ�");
	r = CClientForAS::GetInstance()->Start();
	ASSERT(r && "CClientForASģ������ʧ�ܣ�");

	//����ps�����߳�
	r = CClientForPS::GetInstance()->Init();
	ASSERT(r && "CClientForPSģ���ʼ��ʧ�ܣ�");
	r = CClientForPS::GetInstance()->Start();
	ASSERT(r && "CClientForPSģ������ʧ�ܣ�");


	//����GM�����߳�
	r = CServerForGM::GetInstance()->Init();
	ASSERT(r && "CServerForGMģ���ʼ��ʧ�ܣ�");
	r = CServerForGM::GetInstance()->Start();
	ASSERT(r && "CServerForGMģ������ʧ�ܣ�");

	//�������û������߳�
	r = CServerForUser::GetInstance()->Init();
	ASSERT(r && "CServerForUserģ���ʼ��ʧ�ܣ�");
	r = CServerForUser::GetInstance()->Start();
	ASSERT(r && "CServerForUserģ������ʧ�ܣ�");
	
	//�ر�˳���������˳���෴�� ������ϵ��
	CServerForUser::GetInstance()->Join();
	CServerForGM::GetInstance()->Join();
	CClientForPS::GetInstance()->Join();
	CClientForAS::GetInstance()->Join();
	CClientForDBS::GetInstance()->Join();
	
	return 0;
}
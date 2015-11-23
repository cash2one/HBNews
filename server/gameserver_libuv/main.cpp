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
	//第一顺位！ 最开始初始化配置文件
	r = GameConfig::GetInstance()->Init();
	ASSERT(r && "服务器程序配置初始化失败！");

	//第二顺位！
	const DBConfig& cfg = GameConfig::GetInstance()->GetDBConfig();
	r = CDBMgr::GetInstance()->Init(cfg.m_host.c_str(), cfg.m_port, cfg.m_user.c_str(), cfg.m_passwd.c_str(), cfg.m_db.c_str());
	ASSERT(r && "数据库模块初始化失败！");

	//读取客户端需要的配置文件
	r = CClientConfigMgr::GetInstance()->Init();
	ASSERT(r && "客户端配置初始化失败！");

	r = CSysSettingMgr::GetInstance()->Init();
	ASSERT(r && "服务器游戏配置初始化失败！");

	r = CItemMgr::GetInstance()->Init();
	ASSERT(r && "item模块初始化失败！");

	r = CEquipMgr::GetInstance()->Init();
	ASSERT(r && "equip模块初始化失败！");

	r = CSkillMgr::GetInstance()->Init();
	ASSERT(r && "skill模块初始化失败！");
	
	r = CHeroMgr::GetInstance()->Init();
	ASSERT(r && "hero模块初始化失败！");

	r = CFightMgr::GetInstance()->Init();
	ASSERT(r && "fight模块初始化失败！");

	r = CBurstMgr::GetInstance()->Init();
	ASSERT(r && "burst模块初始化失败！");

	r = CInvokeMgr::GetInstance()->Init();
	ASSERT(r && "Invoke祭坛模块初始化失败！");

	r = CMapMgr::GetInstance()->Init();
	ASSERT(r && "map模块初始化失败！");

	r = CTaskMgr::GetInstance()->Init();
	ASSERT(r && "task模块初始化失败！");

	r = CVipMgr::GetInstance()->Init();
	ASSERT(r && "vip模块初始化失败!");

	r = CUserMgr::GetInstance()->Init();
	ASSERT(r && "玩家模块初始化失败!");

	r = CShopMgr::GetInstance()->Init();
	ASSERT(r && "商店模块初始化失败!");
	
	r = CMissionMgr::GetInstance()->Init();
	ASSERT(r && "任务模块初始化失败!");

	r = CArena::GetInstance()->Init();
	ASSERT(r && "竞技场模块初始化失败!");

	r = CSignMgr::GetInstance()->Init();
	ASSERT(r && "签到模块初始化失败！");
	
	r = CNoviceGuideMgr::GetInstance()->Init();
	ASSERT(r  && "新手任务初始化失败!");
	
	r = CMarqueeMgr::GetInstance()->Init();
	ASSERT(r && "跑马灯初始化失败");
	
	r = CForbidWordMgr::GetInstance()->Init();
	ASSERT(r && "屏蔽词汇加载失败");
	
	r = CFriendMgr::GetInstance()->Init();
	ASSERT(r && "好友模块加载失败");
	
	r = CRankMgr::GetInstance()->Init();
	ASSERT(r && "排行加载失败");

	r = CrusadeMgr::GetInstance()->Init();
	ASSERT(r && "远征模块初始化失败");

	//启动dbs链接线程
	r = CClientForDBS::GetInstance()->Init();
	ASSERT(r && "CClientForDBS模块初始化失败！");
	r = CClientForDBS::GetInstance()->Start();
	ASSERT(r && "CClientForDBS模块启动失败！");

	//启动as链接线程
	r = CClientForAS::GetInstance()->Init();
	ASSERT(r && "CClientForAS模块初始化失败！");
	r = CClientForAS::GetInstance()->Start();
	ASSERT(r && "CClientForAS模块启动失败！");

	//启动ps链接线程
	r = CClientForPS::GetInstance()->Init();
	ASSERT(r && "CClientForPS模块初始化失败！");
	r = CClientForPS::GetInstance()->Start();
	ASSERT(r && "CClientForPS模块启动失败！");


	//启动GM链接线程
	r = CServerForGM::GetInstance()->Init();
	ASSERT(r && "CServerForGM模块初始化失败！");
	r = CServerForGM::GetInstance()->Start();
	ASSERT(r && "CServerForGM模块启动失败！");

	//再启动用户链接线程
	r = CServerForUser::GetInstance()->Init();
	ASSERT(r && "CServerForUser模块初始化失败！");
	r = CServerForUser::GetInstance()->Start();
	ASSERT(r && "CServerForUser模块启动失败！");
	
	//关闭顺序则和启动顺序相反！ 依赖关系！
	CServerForUser::GetInstance()->Join();
	CServerForGM::GetInstance()->Join();
	CClientForPS::GetInstance()->Join();
	CClientForAS::GetInstance()->Join();
	CClientForDBS::GetInstance()->Join();
	
	return 0;
}
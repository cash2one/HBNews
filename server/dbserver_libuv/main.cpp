#include "game_config.h"
#include "db_mgr.h"
#include "async_command_module.h"
#include "server_for_gs.h"
#include "mail_mgr.h"
int main()
{
	//初始化配置文件
	GameConfig::GetInstance()->Init();

	//数据库连接
	const DBConfig& cfg = GameConfig::GetInstance()->GetDBConfig();
	CDBMgr::GetInstance()->Init(cfg.m_host, cfg.m_port, cfg.m_user, cfg.m_passwd, cfg.m_db);
	
	CMailMgr::GetInstance()->Init();

	CAsyncCommandModule::GetInstance()->Init(CAsyncCommandModule::COMMOND_TRY_COMBINE);
	CAsyncCommandModule::GetInstance()->Start();

	CServerForGS::GetInstance()->Init();
	CServerForGS::GetInstance()->Start();

	CServerForGS::GetInstance()->Join();
	CAsyncCommandModule::GetInstance()->Join();

	return 0;
}
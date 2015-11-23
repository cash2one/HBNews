/************************************************************************/
/* 

register
login
getArea  ����������Ϣ + ÿ�����ĵȼ���Ϣ

*/
/************************************************************************/

#include <stdlib.h>
#include <stdio.h>
#include <iostream>

#include "uv.h"
#include "def.h"

#include "db_mgr.h"

#include "guid.h"

#include "async_command_module.h"

#include "server_for_user.h"
#include "server_for_gs.h"

#include "game_config.h"

int main()
{
	GameConfig::GetInstance()->Init();

	//test(GuidGenerator(), std::cout);

	const DBConfig& cfg = GameConfig::GetInstance()->GetDBConfig();
	CDBMgr::GetInstance()->Init(cfg.m_host, cfg.m_port, cfg.m_user, cfg.m_passwd, cfg.m_db);

	CAsyncCommandModule::GetInstance()->Init(CAsyncCommandModule::COMMOND_SINGLE);
	CAsyncCommandModule::GetInstance()->Start();

	CServerForGS::GetInstance()->Init();
	CServerForGS::GetInstance()->Start();

	CServerForUser::GetInstance()->Init();
	CServerForUser::GetInstance()->Start();

	CServerForUser::GetInstance()->Join();
	CServerForGS::GetInstance()->Join();
	CAsyncCommandModule::GetInstance()->Join(); //һ��Ҫ����� �������ݿ��ȶϵ� �������߳̿��ܻ���������д��
	CDBMgr::GetInstance()->UnInit();
	return 0;
}
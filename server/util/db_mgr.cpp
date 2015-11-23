#include <stdio.h>
#include "def.h"
#include "db_mgr.h"
//#include "game_config.h"
#include "time_helper.h"

bool CDBMgr::Init(const std::string& host, int port, const std::string& user, const std::string& passwd, const std::string& db)
{
	m_host = host;
	m_port = port;
	m_user = user;
	m_passwd = passwd;
	m_db = db;

	mysql_init(&m_con);  
	//const DBConfig& cfg = GameConfig::GetInstance()->GetDBConfig();
	if (!mysql_real_connect(&m_con, m_host.c_str(), m_user.c_str(), m_passwd.c_str(), m_db.c_str(), m_port, NULL, CLIENT_MULTI_STATEMENTS))
	{
		printf("Error connecting to database: %s\n", mysql_error(&m_con));  
		//ASSERT(false && "Error connecting to database");
		return false;
	}
	else{
		//>>>>设置链接为utf8  本地环境不需要这一步 但是 测试服务器不设置这个 会将中文显示成?
		int r = Query(&m_con, "SET NAMES 'UTF8'", strlen("SET NAMES 'UTF8'"));
		ASSERT(r == 0);
		r = Query(&m_con, "SET CHARACTER SET UTF8", strlen("SET CHARACTER SET UTF8"));
		ASSERT(r == 0);
		r = Query(&m_con, "SET CHARACTER_SET_RESULTS=UTF8'", strlen("SET CHARACTER_SET_RESULTS=UTF8"));
		ASSERT(r == 0);
		//<<<
		m_bHasInited = true;
		printf("connected mysql successfully!\n"); 
	}

	/*
	if (!mysql_select_db(&m_con, "wh"))   //非0查询失败  
	{
		
		printf("mysql_select_db failed!\n");  
		ASSERT(false && "Error connecting to database");
		return false;  
	}
	*/

	return true;
}

bool CDBMgr::ReInit()
{
	mysql_close(&m_con);
	mysql_init(&m_con);
	//const DBConfig& cfg = GameConfig::GetInstance()->GetDBConfig();
	if (!mysql_real_connect(&m_con, m_host.c_str(), m_user.c_str(), m_passwd.c_str(), m_db.c_str(), m_port, NULL, CLIENT_MULTI_STATEMENTS))
	{
		printf("Error connecting to database: %s\n", mysql_error(&m_con));  
		//ASSERT(false && "Error connecting to database");
		return false;
	}
	else{
		//>>>>设置链接为utf8  本地环境不需要这一步 但是 测试服务器不设置这个 会将中文显示成?
		int r = Query(&m_con, "SET NAMES 'UTF8'", strlen("SET NAMES 'UTF8'"));
		ASSERT(r == 0);
		r = Query(&m_con, "SET CHARACTER SET UTF8", strlen("SET CHARACTER SET UTF8"));
		ASSERT(r == 0);
		r = Query(&m_con, "SET CHARACTER_SET_RESULTS=UTF8'", strlen("SET CHARACTER_SET_RESULTS=UTF8"));
		ASSERT(r == 0);
		//<<<
		m_bHasInited = true;
		printf("connected mysql successfully!\n"); 
	}

	/*
	if (!mysql_select_db(&m_con, "wh"))   //非0查询失败  
	{

	printf("mysql_select_db failed!\n");  
	ASSERT(false && "Error connecting to database");
	return false;  
	}
	*/

	return true;
}

bool CDBMgr::UnInit()
{
	ASSERT(m_bHasInited);
	mysql_close(&m_con);

	return false;
}

MYSQL* CDBMgr::GetConnection()
{
	ASSERT(m_bHasInited);
	return &m_con;
}

bool CDBMgr::check_fields(MYSQL_RES* res, std::vector<std::string> &need_fields_name){

	ASSERT(res);
	int num_fields = mysql_num_fields(res);

	if (num_fields != need_fields_name.size())
	{
		printf("need fields num[%d] != real fields num[%d] \n", need_fields_name.size(), num_fields);
		return false;
	}

	MYSQL_FIELD* fields = mysql_fetch_fields(res);
	//MYSQL_FIELD* fields = mysql_fetch_field(res);

	for(int i = 0; i < num_fields; i++)
	{
		if (strcmpi(fields[i].name, need_fields_name[i].c_str()) != 0)
		{
			//printf("check_fields index[%d] : [%s](real) != [%s](need) \n", i, need_fields_name[i].c_str(), fields[i].name);

			for (int j = 0 ; j < num_fields ; ++j)
			{
				printf("check_fields index[%d] : [%s](real) != [%s](need) \n", j, need_fields_name[j].c_str(), fields[j].name);
			}

			return false;
		}
	}

	return true;
}

int CDBMgr::Query( MYSQL* con, const char* sql, unsigned long length )
{
	ASSERT(con && sql && length);
	int i = 0 ;
	do{
		MYSQL_RES* res = mysql_store_result( con );
		if (res || i++ >= 1){//第二次的时候 打印信息！
			printf("\n pre query left result no free！！！ \n\n");
		}
		mysql_free_result(res);
	}while( !mysql_next_result( con ) );

	printf("\n before query: 【%s】\n\n", sql);
	CTimeChecker tc;
	int code = mysql_real_query(con, sql, length);
	tc.PrintfDuration();
	if(code != 0){//非0时 表示失败
		unsigned int err_num = mysql_errno(con);
		if (2013 == err_num || 2006 == err_num){
			printf("disconnect!! query failed! [%s] [%d] [%s] \n", sql, mysql_errno(con), mysql_error(con));
			printf("start reconnect! \n");

			bool r = CDBMgr::GetInstance()->ReInit();
			ASSERT(r);//重新连接
			if (r){
				printf("reconnect success! \n");
				code = mysql_real_query(con, sql, length);
				if(code != 0){
					printf("re-query result code[%d]: [%s] [%d] [%s] \n", code, sql, mysql_errno(con), mysql_error(con));
				}
				else{
					printf("re-query success! [%s]\n", sql);
				}
			}
			else{
				printf("reconnect failed! \n");
			}
		}
		else{
			printf("query failed! [%s] [%d] [%s] \n", sql, mysql_errno(con), mysql_error(con));
		}
	}
	else{
		printf("query success! [%s]\n", sql);
	}

	return code;
}

int CDBMgr::Query( MYSQL* con, const std::string& sql)
{
	return Query(con, sql.c_str(), sql.length());
}

/*
bool CDBMgr::check_fields( MYSQL_RES* res, std::string (&need_fields_name)[] )
{
	int num_fields = mysql_num_fields(res);

	if (num_fields != (sizeof(need_fields_name) / sizeof(need_fields_name[0])))
	{
		return false;
	}

	MYSQL_FIELD* fields = mysql_fetch_fields(res);

	for(int i = 0; i < num_fields; i++)
	{
		if (strcmpi(fields[i].name, need_fields_name[i].c_str()) != 0)
		{
			return false;
		}
	}

	return false;
}
*/
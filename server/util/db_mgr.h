#ifndef db_mgr_h__
#define db_mgr_h__

#include <WinSock2.h>//！！！一定要加上 因为mysql先加载windows.h 会使用winsock 然后winsock2会重定义！
#include "mysql.h"
#include <string>
#include <vector>
class CDBMgr
{
public:
	//todo 单例工具
	inline static CDBMgr* GetInstance(){
		static CDBMgr singleton;
		return &singleton;
	}

	bool Init(const std::string& host, int port, const std::string& user, const std::string& passwd, const std::string& db);
	bool ReInit();

	bool UnInit();

	MYSQL* GetConnection();

	static bool check_fields(MYSQL_RES* res, std::vector<std::string> &need_fields_name);
	//static bool check_fields(MYSQL_RES* res, std::string (&need_fields_name)[]);

	static int Query( MYSQL* con, const char* sql, unsigned long length );
	static int Query( MYSQL* con, const std::string& sql);
private:

	CDBMgr(): m_bHasInited(false){}

	bool m_bHasInited;
	MYSQL m_con;


	std::string	m_host;
	int		m_port;
	std::string	m_user;
	std::string	m_passwd;
	std::string m_db;
};

#endif // db_mgr_h__
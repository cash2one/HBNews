#ifndef config_member_h__
#define config_member_h__

#include <vector>
#include <string>

class DBConfig
{
public:
	std::string	m_host;
	int		m_port;
	std::string	m_user;
	std::string	m_passwd;
	std::string m_db;
};

class ServerForGSConfig{
public:
	std::string	m_ip;
	int		m_port;
};

#endif // config_member_h__
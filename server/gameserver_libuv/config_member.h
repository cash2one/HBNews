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

class ServerForUserConfig{
public:
	std::string	m_ip;
	int		m_port;
};

class ClientForASConfig{
public:
	std::string	m_ip;
	int		m_port;
};

class ClientForDBSConfig{
public:
	std::string m_ip;
	int			m_port;
};

class ServerForGMConfig{
public:
	std::string m_ip;
	int			m_port;
};

class ClientForPSConfig{
public:
	std::string	m_ip;
	int		m_port;
};

#endif // config_member_h__
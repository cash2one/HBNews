#ifndef _ConfigMember_H_
#define _ConfigMember_H_
#include <vector>
#include <string>
struct DBConfig
{
	std::string m_DBName;
	std::string	m_IP;
	int		m_Port;
	std::string	m_UserName;
	std::string	m_PassWord;
};

struct GenerateExcelConfig
{
	std::vector<std::string> m_GenerateExcelConfig;
};

struct LoadExcelConfigClient
{
	bool m_active;
	std::string fromDir;
	std::vector<std::string> m_LoadExcelConfig;
};

struct LoadExcelConfigServer
{
	std::string fromDir;
	bool m_active;
	std::vector<std::string> m_LoadExcelConfig;
};

#endif
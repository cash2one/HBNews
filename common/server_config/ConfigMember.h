#ifndef ConfigMember_h__
#define ConfigMember_h__

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

struct LoadExcelConfig
{
	std::vector<std::string> m_LoadExcelConfig;
};

#endif // ConfigMember_h__
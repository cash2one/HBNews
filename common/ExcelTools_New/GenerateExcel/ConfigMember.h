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

struct ConfigItem
{
	std::string m_strFileName;
	bool   m_bDB;
};

struct GenerateExcelConfig
{
	std::string strFromDir;
	std::string strToDir;
	std::vector<ConfigItem> m_GenerateExcelConfig;
};

struct LoadExcelConfig
{
	std::vector<std::string> m_LoadExcelConfig;
};

#endif
#include "game_config.h"
#include <rapidxml.hpp>
#include <rapidxml_utils.hpp>
#include <rapidxml_print.hpp>
#include <iostream>
#include <windows.h>

void GameConfig::ParseAll()
{
	using namespace rapidxml;
	//读取配置文件
	{
		char* path = "./config_as.xml";
		file<> fdoc(path);
		//std::cout << fdoc.data() << std::endl;
		xml_document<> doc;
		try
		{
			doc.parse<0>(fdoc.data());//以0结尾的xml字符串
		}
		catch(rapidxml::parse_error& e)
		{
			std::string err("parse xml error. ");
			err += e.what();
			MessageBoxA(NULL, err.c_str(), "提示", MB_OK);
			//return 0;
		}
		//std::cout << doc.name() << std::endl;

		//获取根节点
		xml_node<>* root = doc.first_node();
		//std::cout << root->name() << std::endl;

		//遍历根节点下的子节点
		for (rapidxml::xml_node<char> *pNode = root->first_node(); pNode != NULL; pNode = pNode->next_sibling())
		{
			//std::cout << pNode->name();
			if (!strcmp(pNode->name(), "DB"))
			{
				m_DBConfig.m_host = pNode->first_node("host")->value(); 
				m_DBConfig.m_port = atoi(pNode->first_node("port")->value());
				m_DBConfig.m_user = pNode->first_node("user")->value();
				m_DBConfig.m_passwd = pNode->first_node("passwd")->value();
				m_DBConfig.m_db = pNode->first_node("db")->value();
			}
			else if (!strcmp(pNode->name(), "Server_For_User"))
			{
				m_ServerForUserConfig.m_ip = pNode->first_node("ip")->value(); 
				m_ServerForUserConfig.m_port = atoi(pNode->first_node("port")->value());
			}
			else if(!strcmp(pNode->name(), "Server_For_GS"))
			{
				m_ServerForGSConfig.m_ip = pNode->first_node("ip")->value(); 
				m_ServerForGSConfig.m_port = atoi(pNode->first_node("port")->value());
			}
		}
	}
}

const DBConfig& GameConfig::GetDBConfig()
{
	return m_DBConfig;
}

const ServerForUserConfig& GameConfig::GetServerForUserConfig()
{
	return m_ServerForUserConfig;
}

const ServerForGSConfig& GameConfig::GetServerForGSConfig()
{
	return m_ServerForGSConfig;
}
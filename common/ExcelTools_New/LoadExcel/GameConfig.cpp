#include "GameConfig.h"
#include <rapidxml.hpp>
#include <rapidxml_utils.hpp>
#include <rapidxml_print.hpp>
#include <iostream>
#include <windows.h>
GameConfig* GameConfig::ms_pInstance = NULL;


void GameConfig::ParseAll()
{
	using namespace rapidxml;
	//读取配置文件
	{
		file<> fdoc("../Bin/config.xml");
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
		//获取根节点
		xml_node<>* root = doc.first_node();

		//遍历根节点下的子节点
		for (rapidxml::xml_node<char> *pNode = root->first_node(); pNode != NULL; pNode = pNode->next_sibling())
		{
			if (!strcmp(pNode->name(), "DB"))
			{
				m_DBConfig.m_DBName = pNode->first_node("DBName")->value();
				m_DBConfig.m_IP = pNode->first_node("IP")->value(); 
				m_DBConfig.m_Port = atoi(pNode->first_node("Port")->value());
				m_DBConfig.m_UserName = pNode->first_node("UserName")->value();
				m_DBConfig.m_PassWord = pNode->first_node("PassWord")->value();
			}
			else if (!strcmp(pNode->name(), "GenerateExcel"))
			{
				for (xml_node<char>* pChildNode = pNode->first_node(); pChildNode != NULL; pChildNode = pChildNode->next_sibling())
				{
					m_GenerateExcelConfig.m_GenerateExcelConfig.push_back(pChildNode->value());
				}
			}
			else if(!strcmp(pNode->name(), "LoadExcelClient"))
			{
				for (rapidxml::xml_attribute<>* attr = pNode->first_attribute(); attr; attr = attr->next_attribute())
				{
					if( !strcmp("fromDir", attr->name()))
					{
						m_LoadExcelConfigClient.fromDir = attr->value();
					}
					if (!strcmp("isActive", attr->name()))
					{
						m_LoadExcelConfigClient.m_active = !!atoi(attr->value());
					}
				}
				for (xml_node<char>* pChildNode = pNode->first_node(); pChildNode != NULL; pChildNode = pChildNode->next_sibling())
				{
					m_LoadExcelConfigClient.m_LoadExcelConfig.push_back(pChildNode->value());
				}
			}
			else if(!strcmp(pNode->name(), "LoadExcelServer"))
			{
				for (rapidxml::xml_attribute<>* attr = pNode->first_attribute(); attr; attr = attr->next_attribute())
				{
					if( !strcmp("fromDir", attr->name()))
					{
						m_LoadExcelConfigServer.fromDir = attr->value();
					}
					if (!strcmp("isActive", attr->name()))
					{
						m_LoadExcelConfigServer.m_active = !!atoi(attr->value());
					}
				}
				for (xml_node<char>* pChildNode = pNode->first_node(); pChildNode != NULL; pChildNode = pChildNode->next_sibling())
				{
					m_LoadExcelConfigServer.m_LoadExcelConfig.push_back(pChildNode->value());
				}
			}
		}
	}
}

const DBConfig& GameConfig::GetDBConfig()
{
	return m_DBConfig;
}

const GenerateExcelConfig& GameConfig::GetGenerateExcelConfig()
{
	return m_GenerateExcelConfig;
}

const LoadExcelConfigClient& GameConfig::GetLoadExcelConfigClient()
{
	return m_LoadExcelConfigClient;
}
const LoadExcelConfigServer& GameConfig::GetLoadExcelConfigServer()
{
	return m_LoadExcelConfigServer;
}
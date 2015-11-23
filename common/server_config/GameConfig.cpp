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
	//��ȡ�����ļ�
	{
		char* path = "./config_gs.xml";
		file<> fdoc(path);
		std::cout << fdoc.data() << std::endl;
		xml_document<> doc;
		try
		{
			doc.parse<0>(fdoc.data());//��0��β��xml�ַ���
		}
		catch(rapidxml::parse_error& e)
		{
			std::string err("parse xml error. ");
			err += e.what();
			MessageBoxA(NULL, err.c_str(), "��ʾ", MB_OK);
			//return 0;
		}
		std::cout << doc.name() << std::endl;

		//��ȡ���ڵ�
		xml_node<>* root = doc.first_node();
		std::cout << root->name() << std::endl;

		//�������ڵ��µ��ӽڵ�
		for (rapidxml::xml_node<char> *pNode = root->first_node(); pNode != NULL; pNode = pNode->next_sibling())
		{
			std::cout << pNode->name();
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
			else if(!strcmp(pNode->name(), "LoadExcel"))
			{
				for (xml_node<char>* pChildNode = pNode->first_node(); pChildNode != NULL; pChildNode = pChildNode->next_sibling())
				{
					m_LoadExcelConfig.m_LoadExcelConfig.push_back(pChildNode->value());
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

const LoadExcelConfig& GameConfig::GetLoadExcelConfig()
{
	return m_LoadExcelConfig;
}
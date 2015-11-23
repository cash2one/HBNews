#pragma once
#include "ConfigMember.h"
class GameConfig
{
public:
	static GameConfig* GetInstance()
	{
		if (!ms_pInstance)
		{
			ms_pInstance = new GameConfig;
		}
		return ms_pInstance;
	}

	static void Destory()
	{
		if (ms_pInstance)
		{
			delete ms_pInstance;
			ms_pInstance = NULL;
		}
	}
	void ParseAll();

	const DBConfig& GetDBConfig();
	const GenerateExcelConfig& GetGenerateExcelConfig();
	const LoadExcelConfigClient& GetLoadExcelConfigClient();
	const LoadExcelConfigServer& GetLoadExcelConfigServer();
private:
	static GameConfig* ms_pInstance;

	DBConfig m_DBConfig;


	GenerateExcelConfig m_GenerateExcelConfig;


	LoadExcelConfigClient m_LoadExcelConfigClient;
	LoadExcelConfigServer m_LoadExcelConfigServer;
};


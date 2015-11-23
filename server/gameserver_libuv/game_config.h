#ifndef game_config_h__
#define game_config_h__

#pragma once
#include "config_member.h"
class GameConfig
{
public:
	static GameConfig* GetInstance()
	{ 
		static GameConfig singleton;
		return &singleton;
	}

	bool Init(){
		ParseAll();
		return true;
	}

	const DBConfig& GetDBConfig();
	const ServerForUserConfig& GetServerForUserConfig();
	const ClientForASConfig& GetClientForASConfig();
	const ClientForDBSConfig& GetClientForDBSConfig();
	const ServerForGMConfig& GetServerForGMConfig();
	const ClientForPSConfig& GetClientForPSConfig();
	
private:

	void ParseAll();

	DBConfig m_DBConfig;
	ServerForUserConfig m_ServerForUserConfig;
	ClientForASConfig m_ClientForASConfig;
	ClientForDBSConfig m_ClientForDBSConfig;
	ServerForGMConfig m_ServerForGMConfig;
	ClientForPSConfig m_ClientForPSConfig;
};

#endif // game_config_h__
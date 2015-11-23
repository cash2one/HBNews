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
	const ServerForGSConfig& GetServerForGSConfig();
private:

	GameConfig(){}
	void ParseAll();

	DBConfig m_DBConfig;
	ServerForUserConfig m_ServerForUserConfig;
	ServerForGSConfig m_ServerForGSConfig;
};

#endif // game_config_h__
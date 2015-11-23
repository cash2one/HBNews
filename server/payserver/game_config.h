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
	const ServerForGSConfig& GetServerForGSConfig();
private:

	void ParseAll();

	DBConfig m_DBConfig;
	ServerForGSConfig m_ServerForGSConfig;
};

#endif // game_config_h__
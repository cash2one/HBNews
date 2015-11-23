#ifndef client_id_h__
#define client_id_h__

#pragma once
#include <vector>
#include <string>
#include "config.h"
#include "def.h"
class CClientID
{
public:
	struct FieldConfig
	{
		const CHAR* m_szFieldStr;
		UINT8  m_uFieldLength;
	};

	static CClientID* GetInstance()
	{
		static CClientID singleton;
		return &singleton;
	}
	
	std::string GetClientID(EELEMENTTYPE eType, UINT16 uId);
	
	UINT32   GetServerID(const std::string& strClientId);

private:

	CClientID(){
		ASSERT(Init());
	}
	bool Init();
	std::vector<FieldConfig> m_vecFieldConfig;
};

#endif
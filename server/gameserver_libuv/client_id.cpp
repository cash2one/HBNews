#include "client_id.h"

bool CClientID::Init()
{
	m_vecFieldConfig.resize(EELEMENTTYPE_MAX);
	FieldConfig fieldConfig;

	fieldConfig.m_szFieldStr = "PassiveSkill";
	fieldConfig.m_uFieldLength = 3;
	m_vecFieldConfig[EELEMENTTYPE_SKILL] = fieldConfig;


	fieldConfig.m_szFieldStr = "Section";
	fieldConfig.m_uFieldLength = 4;
	m_vecFieldConfig[EELEMENTTYPE_TASK]  = fieldConfig;			//系列任务

	fieldConfig.m_szFieldStr = "LevelData";
	fieldConfig.m_uFieldLength = 3;
	m_vecFieldConfig[EELEMENTTYPE_SUB_TASK]   = fieldConfig;			//子任务

	fieldConfig.m_szFieldStr = "Battle";
	fieldConfig.m_uFieldLength = 4;
	m_vecFieldConfig[EELEMENTTYPE_BATTLE]      = fieldConfig;			//战斗

	fieldConfig.m_szFieldStr = "Hero";
	fieldConfig.m_uFieldLength = 3;
	m_vecFieldConfig[EELEMENTTYPE_HERO]        = fieldConfig;			//英雄

	fieldConfig.m_szFieldStr = "Boss";
	fieldConfig.m_uFieldLength = 3;
	m_vecFieldConfig[EELEMENTTYPE_BOSS]        = fieldConfig;

	fieldConfig.m_szFieldStr = "Monster";
	fieldConfig.m_uFieldLength = 3;
	m_vecFieldConfig[EELEMENTTYPE_MONSTER]     = fieldConfig;

	fieldConfig.m_szFieldStr = "HeroEquip";
	fieldConfig.m_uFieldLength = 4;
	m_vecFieldConfig[EELEMENTTYPE_ITEM]        = fieldConfig;	

	return true;
}


std::string CClientID::GetClientID(EELEMENTTYPE eType, UINT16 uId)
{
	CHAR szId[32] = {0};
	sprintf_s(szId, sizeof(szId), "%d", uId);
	ASSERT(m_vecFieldConfig[eType].m_uFieldLength >= strlen(szId));
	
	CHAR szClientId[128] = {0};
	sprintf_s(szClientId, sizeof(szClientId), "%s%.*d", m_vecFieldConfig[eType].m_szFieldStr, m_vecFieldConfig[eType].m_uFieldLength, uId);
	return szClientId;
}

UINT32   CClientID::GetServerID(const std::string& strClientId)
{
	ASSERT(strClientId.size());
	for (size_t i = 0; i < strClientId.size(); ++i)
	{
		if (strClientId[i] >= '0' && strClientId[i] <= '9')
		{
			return atoi(strClientId.substr(i, strClientId.size()).c_str());
		}
	}
	return INVALID_SERVER_ID;
}


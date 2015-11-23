#include "crusade_mgr.h"
#include "db_mgr.h"
#include "sys_setting_mgr.h"
#include "db_sql.h"
#include "str.h"
bool CrusadeMgr::Init()
{
	MYSQL* con = CDBMgr::GetInstance()->GetConnection();
	CHECK_NULL_RETURN_FALSE(con);

	char* sql = SELECT_CRUSADE_PRIZE;
	int code = CDBMgr::Query(con, sql, strlen(sql));

	if (0 != code)
	{
		printf("query failed! [%s]\n", sql);
		return false;
	}
	MYSQL_RES* res = mysql_store_result(con);

	{
		std::string need_fields[] = {"level", "prize"};
		std::vector<std::string> v_task_need_field(need_fields, need_fields + sizeof(need_fields)/sizeof(need_fields[0]));
		ASSERT(CDBMgr::check_fields(res, v_task_need_field));
	}

	MYSQL_ROW row;

	UINT32 iCrusadeLevel = 0;
	bool r = CSysSettingMgr::GetInstance()->GetIntValue("User_Level_Function_Crusade", iCrusadeLevel);
	CHECK_FALSE_RETURN_FALSE(r);

	while(row = mysql_fetch_row(res))
	{
		int col = 0;
		UINT32 iLevel = atoi(row[col++]);
		ASSERT(iLevel == iCrusadeLevel++);
		
		ASSERT(m_mapCrusadePrize.find(iLevel) == m_mapCrusadePrize.end());
		std::map<UINT32, std::vector<stGameElement>>& mvElement = m_mapCrusadePrize[iLevel];
		if(!row[col]){
			return false;
		}

		char* buf = UTF8ToANSI(row[col]);
		std::vector<std::string> vecWaves;
		split_string(buf, vecWaves, "|");
		free(buf);
		ASSERT(MAX_NUM_CRUSADE_WAVE == vecWaves.size());//总共有15波
		int wave = 0;
		for (std::vector<std::string>::iterator it = vecWaves.begin(); it != vecWaves.end(); ++it)
		{
			std::vector<stGameElement>& vecWaveEle = mvElement[wave++];
			ASSERT((*it).size());
			std::vector<std::string> strWaveEles;//每一波
			split_string(*it, strWaveEles, ",");
			ASSERT(strWaveEles.size());
			for (std::vector<std::string>::iterator itEle = strWaveEles.begin(); itEle != strWaveEles.end(); ++itEle)
			{
				std::vector<int> ele;
				split_string_2i(*itEle, ele, "-");
				ASSERT(ele.size() == 3);
				stGameElement gameEle((EELEMENTTYPE)ele[0], ele[1], NULL, stGameElement::INCREASE, ele[2]);
	/*			bool r = IsElementValid(gameEle);
				ASSERT(r);
				CHECK_FALSE_RETURN_FALSE(r);*/
				vecWaveEle.push_back(gameEle);
			}
		}

	}

	const char* crusade_promote_type = NULL;
	r = CSysSettingMgr::GetInstance()->GetStringValue("Crusade_Promote_Type", crusade_promote_type);
	ASSERT(r && crusade_promote_type);
	CHECK_FALSE_RETURN_FALSE(r && crusade_promote_type);

	std::vector<int> vecType;
	split_string_2i(crusade_promote_type, vecType, "|");
	for (std::vector<int>::iterator it = vecType.begin(); it != vecType.end(); ++it)
	{
		EELEMENTTYPE eType = EELEMENTTYPE(*it);
		ASSERT(eType > EELEMENTTYPE_MIN && eType < EELEMENTTYPE_MAX);
		CHECK_FALSE_RETURN_FALSE(eType > EELEMENTTYPE_MIN && eType < EELEMENTTYPE_MAX);
		m_setPromoteType.insert(EELEMENTTYPE(*it));
	}

	r = CSysSettingMgr::GetInstance()->GetIntValue("Crusade_Promote_Percent", m_iPromotePercent);
	ASSERT(r && m_iPromotePercent);
	CHECK_FALSE_RETURN_FALSE(r && m_iPromotePercent);
	return true;
}

bool CrusadeMgr::GetCrusadeWavePrize( UINT32 iLevel, UINT32 iWave, std::vector<stGameElement>& vecEle)
{
	std::map<UINT32, std::map<UINT32, std::vector<stGameElement>>>::iterator it_1 = m_mapCrusadePrize.find(iLevel);
	ASSERT(it_1 != m_mapCrusadePrize.end());
	
	CHECK_FALSE_RETURN_FALSE(it_1 != m_mapCrusadePrize.end());

	std::map<UINT32, std::vector<stGameElement>>::iterator it_2 = it_1->second.find(iWave);
	ASSERT(it_2 != it_1->second.end());
	CHECK_FALSE_RETURN_FALSE(it_2 != it_1->second.end());

	vecEle = it_2->second;
	return true;
}

void CrusadeMgr::PromotePrize(std::vector<stGameElement>& stEles)
{
	for (std::vector<stGameElement>::iterator it = stEles.begin(); it != stEles.end(); ++it)
	{
		if (m_setPromoteType.find(it->eType) != m_setPromoteType.end())
		{
			it->iNum = it->iNum * (1 + double(m_iPromotePercent/100));
		}
	}
}

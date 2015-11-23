#include <vector>
#include <string>
#include "sign_mgr.h"
#include "db_sql.h"
#include "db_mgr.h"
#include "str.h"
#include "config.h"
#include "time_helper.h"

bool CSignMgr::Init()
{
	MYSQL* con = CDBMgr::GetInstance()->GetConnection();
	
	char* sql = SELECT_SIGN;
	int code = CDBMgr::Query(con, sql, (unsigned int)strlen(sql));

	if (0 != code)
	{
		printf("query failed! [%s]\n", sql);
		return false;
	}
	MYSQL_RES* res = mysql_store_result(con);

	{
		std::string need_fields[] = {"id", "time", "sign_reward_1", "sign_reward_n"};
		std::vector<std::string> v_task_need_field(need_fields, need_fields + sizeof(need_fields)/sizeof(need_fields[0]));
		ASSERT(CDBMgr::check_fields(res, v_task_need_field));
	}

	MYSQL_ROW row;
	while(row = mysql_fetch_row(res))
	{
		int col = 1;
		ASSERT(row[col++]);
		
		if (!row[col - 1])
		{
			return false;
		}

		char* pBuf = UTF8ToANSI(row[col - 1]);
		std::vector<int> eleTime;
		split_string_2i(pBuf, eleTime, "|");
		ASSERT(eleTime.size() == 2);

		std::pair<int, int> key = std::make_pair(eleTime[0], eleTime[1]);
		ASSERT(m_mapSign1Time2Rewards.find(key) == m_mapSign1Time2Rewards.end());
		std::vector<stGameElement>& vecEleSign1 = m_mapSign1Time2Rewards[key];
		free(pBuf);
		
		ASSERT(row[col++]);
		pBuf = UTF8ToANSI(row[col - 1]);
		std::vector<std::string> elements;
		split_string(pBuf, elements, "|");
		ASSERT(GetDaysInMonth(eleTime[0], eleTime[1]) == elements.size());
		free(pBuf);
		
		for (std::vector<std::string>::iterator it = elements.begin(); it != elements.end(); ++it)
		{
			std::vector<int> ele;
			split_string_2i(*it, ele, "-");
			ASSERT(ele.size() == 3);
			stGameElement gameEle((EELEMENTTYPE)ele[0], ele[1], NULL, stGameElement::INCREASE, ele[2]);
			vecEleSign1.push_back(gameEle);
		}


		ASSERT(row[col++]);
		if (!row[col - 1])
		{
			return false;
		}
		ASSERT(m_mapSignNTime2Rewards.find(key) == m_mapSignNTime2Rewards.end());
		std::vector<stGameElement>& vecEleSignN = m_mapSignNTime2Rewards[key];
		pBuf = UTF8ToANSI(row[col - 1]);
		elements.clear();
		split_string(pBuf, elements, "|");
		ASSERT(elements.size() == 4);
		free(pBuf);

		for (std::vector<std::string>::iterator it = elements.begin(); it != elements.end(); ++it)
		{
			std::vector<int> ele;
			split_string_2i(*it, ele, "-");
			ASSERT(ele.size() == 3);
			stGameElement gameEle((EELEMENTTYPE)ele[0], ele[1], NULL, stGameElement::INCREASE, ele[2]);
			vecEleSignN.push_back(gameEle);
		}
	}
	return true;
}

stGameElement* CSignMgr::GetSign1Reward( UINT32 iYear, UINT32 iMonth, UINT32 iTimes )
{
	std::pair<int, int> key = std::make_pair(iYear, iMonth);
	std::map<std::pair<UINT32, UINT32>, std::vector<stGameElement>>::iterator it = m_mapSign1Time2Rewards.find(key);
	ASSERT(it != m_mapSign1Time2Rewards.end());
	if (it == m_mapSign1Time2Rewards.end())
	{
		return NULL;

	}
	else
	{
		return &it->second[iTimes];
	}
}

stGameElement* CSignMgr::GetSignNReward(UINT32 iYear, UINT32 iMonth,  UINT32 iType )
{
	ASSERT(iType >= 0 && iType <= 3);
	std::pair<int, int> key = std::make_pair(iYear, iMonth);
	std::map<std::pair<UINT32, UINT32>, std::vector<stGameElement>>::iterator it = m_mapSignNTime2Rewards.find(key);
	ASSERT(it != m_mapSignNTime2Rewards.end());
	if (it == m_mapSignNTime2Rewards.end())
	{
		return NULL;

	}
	else
	{
		return &it->second[iType];
	}
}

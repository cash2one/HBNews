#include "novice_guide_mgr.h"
#include "db_sql.h"
#include "db_mgr.h"
#include "str.h"
#include "condition.h"
bool CNoviceGuideMgr::Init()
{
	MYSQL* con = CDBMgr::GetInstance()->GetConnection();

	char* sql = SELECT_GUIDE;
	int code = CDBMgr::Query(con, sql, (unsigned int)strlen(sql));

	if (0 != code)
	{
		printf("query failed! [%s]\n", sql);
		return false;
	}
	MYSQL_RES* res = mysql_store_result(con);

	{
		std::string need_fields[] = {"id", "trigger_condition", "pre_guild_id", "desc"};
		std::vector<std::string> v_task_need_field(need_fields, need_fields + sizeof(need_fields)/sizeof(need_fields[0]));
		ASSERT(CDBMgr::check_fields(res, v_task_need_field));
	}

	MYSQL_ROW row;
	while(row = mysql_fetch_row(res))
	{
		UINT32 col = 0;
		stGuide* pGuide = new stGuide;
		pGuide->iID = atoi(row[col++]);
		
		if (row[col++])
		{
			char* pBuf = UTF8ToANSI(row[col - 1]);
			bool r = ParseCondition(pBuf, pGuide->triggerCondition);
			free(pBuf);
		}

		pGuide->iPreGuildID = atoi(row[col++]);

		if (row[col++])
		{	char* pBuf = UTF8ToANSI(row[col - 1]);
			strncpy(pGuide->desc, pBuf, sizeof(pGuide->desc));
			free(pBuf);
		}
	
		ASSERT(m_mapGuid.find(pGuide->iID) ==  m_mapGuid.end());
		CHECK_FALSE_RETURN_FALSE(m_mapGuid.find(pGuide->iID) ==  m_mapGuid.end());
		m_mapGuid[pGuide->iID] = pGuide;
	}
	return true;
}

stGuide* CNoviceGuideMgr::GetGuideByGuideID( UINT32 iGuideID )
{
	std::map<UINT32, stGuide*>::iterator it = m_mapGuid.find(iGuideID);

	if(it == m_mapGuid.end())
		return NULL;

	return it->second;
}

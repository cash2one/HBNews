
#include <string>
#include <vector>
#include "skill_mgr.h"
#include "db_mgr.h"
#include "db_sql.h"
#include "str.h"

using std::string;
using std::vector;

bool CSkillMgr::Init()
{
	MYSQL* con = CDBMgr::GetInstance()->GetConnection();

	//解析表！
	char* sql = "SELECT * FROM `sys_skill_passive`;"
		"SELECT * FROM `sys_skill_level_gold`;"
		;

	int code = CDBMgr::Query(con, sql, (unsigned int)strlen(sql));
	if (0 != code)   //非0查询失败  
	{
		printf("query failed! [%s] [%d] [%s] \n", sql, mysql_errno(con), mysql_error(con));
		return false;  
	}

	int i = 0;
	do
	{
		MYSQL_RES* res = mysql_store_result( con );
		switch (i++)
		{
		case 0:{//skill
				   {
					   //检查表结构是否有改变
					   string need_fields[] = {"passive_skill_id","name"};
					   std::vector<std::string> v_need_fields(need_fields, need_fields + sizeof(need_fields)/sizeof(need_fields[0]));
					   ASSERT(CDBMgr::check_fields(res, v_need_fields));
				   }

				   MYSQL_ROW row;
				   while (row = mysql_fetch_row(res))
				   {
					   int col = 0;//忽略数据库id
					   CPassiveSkillCfg* pItem = new CPassiveSkillCfg;
					   pItem->iSkillID = atoi(row[col++]);
					   pItem->name = row[col++];

					   ASSERT(m_map2PassiveSkill.find(pItem->iSkillID) == m_map2PassiveSkill.end());
					   m_map2PassiveSkill[pItem->iSkillID] = pItem;
				   }
			   }
			   break;
		case 1:{//skill level gold
				   {
					   //检查表结构是否有改变
					   string need_fields[] = {"level","bisha","aoyi","passive1","passive2","passive3"};
					   std::vector<std::string> v_need_fields(need_fields, need_fields + sizeof(need_fields)/sizeof(need_fields[0]));
					   ASSERT(CDBMgr::check_fields(res, v_need_fields));
				   }

				   MYSQL_ROW row;
				   while (row = mysql_fetch_row(res))
				   {
					   int col = 0;
					   stSkillLevelNeed* pItem = new stSkillLevelNeed;
					   UINT32 iLevel = atoi(row[col++]);

					   ASSERT(m_mapLevel2Need.find(iLevel) == m_mapLevel2Need.end());
					   
					   pItem->arrNeedGold[ESKILLTYPE_BISHA] = atoi(row[col++]);
					   pItem->arrNeedGold[ESKILLTYPE_AOYI] = atoi(row[col++]);
					   pItem->arrNeedGold[ESKILLTYPE_PASSIVE1] = atoi(row[col++]);
					   pItem->arrNeedGold[ESKILLTYPE_PASSIVE2] = atoi(row[col++]);
					   pItem->arrNeedGold[ESKILLTYPE_PASSIVE3] = atoi(row[col++]);

					   m_mapLevel2Need[iLevel] = pItem;
				   }

			   }
			   break;
		}
		mysql_free_result(res);
	}while( !mysql_next_result( con ) );

	return true;
}

CPassiveSkillCfg* CSkillMgr::GetPassiveSkillCfgByID( UINT32 iPassiveSkillID )
{
	std::map<UINT32, CPassiveSkillCfg*>::iterator it = m_map2PassiveSkill.find(iPassiveSkillID);
	if (it != m_map2PassiveSkill.end())
	{
		return it->second;
	}

	return NULL;
}

stSkillLevelNeed* CSkillMgr::GetLevelNeed( UINT32 iLevel )
{
	std::map<UINT32, stSkillLevelNeed*>::iterator it = m_mapLevel2Need.find(iLevel);
	if (it != m_mapLevel2Need.end() )
	{
		return it->second;
	}

	return NULL;
}

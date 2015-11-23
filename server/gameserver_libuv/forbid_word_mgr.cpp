#include "forbid_word_mgr.h"
#include "db_mgr.h"
#include "db_sql.h"
#include "def.h"
bool CForbidWordMgr::Init()
{
	MYSQL* con = CDBMgr::GetInstance()->GetConnection();

	char* sql = SELECT_FORBID_WORD;
	int code = CDBMgr::GetInstance()->Query(con, sql, (unsigned int)strlen(sql));
	if (0 != code)
	{
		printf("query failed![%s]\n", sql);
		return false;
	}

	MYSQL_RES* res = mysql_store_result(con);

	{
		std::string need_fields[] = {"forbid_word"};
		std::vector<std::string> v_task_need_field(need_fields, need_fields + sizeof(need_fields)/sizeof(need_fields[0]));
		ASSERT(CDBMgr::check_fields(res, v_task_need_field));
	}

	MYSQL_ROW row;
	while(row = mysql_fetch_row(res))
	{
		int col = 0;
		ASSERT(row[col] && row[col] != "");
		m_vecForbidWord.push_back(row[col++]);
	}

	return true;
}

bool CForbidWordMgr::IsForbidWord(const std::string& word)
{
	for (size_t i = 0; i < m_vecForbidWord.size(); ++i)
	{
		if(word.find(m_vecForbidWord[i]) != std::string::npos)
			return true;
	}
	return false;
}

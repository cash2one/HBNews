#include "marquee_mgr.h"
#include "db_sql.h"
#include "db_mgr.h"
#include "str.h"
#include "user_mgr.h"
#include "sys_setting_mgr.h"
#include "server_for_user.h"
bool CMarqueeMgr::Init()
{
	MYSQL* con = CDBMgr::GetInstance()->GetConnection();

	char* sql = SELECT_MARQUEE;
	int code = CDBMgr::Query(con, sql, (unsigned int)strlen(sql));

	if (0 != code)
	{
		printf("query failed! [%s]\n", sql);
		return false;
	}
	MYSQL_RES* res = mysql_store_result(con);

	{
		std::string need_fields[] = {"id", "value", "show_times"};
		std::vector<std::string> v_task_need_field(need_fields, need_fields + sizeof(need_fields)/sizeof(need_fields[0]));
		ASSERT(CDBMgr::check_fields(res, v_task_need_field));
	}

	MYSQL_ROW row;
	while(row = mysql_fetch_row(res))
	{
		int col = 0;

		stMarqueeCfg cfg;
		cfg.iID = atoi(row[col++]);

		char* value = UTF8ToANSI(row[col++]);
		std::vector<int> ele;
		split_string_2i(std::string(value), ele, ",");
		if (cfg.iID == 1 || cfg.iID == 3 || cfg.iID == 4)
		{
			ASSERT(1 == ele.size());
		}
		cfg.vecValue.insert(cfg.vecValue.begin(), ele.begin(), ele.end());
		free(value);

		cfg.iShowTimes = atoi(row[col++]);
		ASSERT(m_mapID2MarqueeCfg.find(cfg.iID) == m_mapID2MarqueeCfg.end());
		m_mapID2MarqueeCfg[cfg.iID] = cfg;
	}

	//判断每种类型都有
	UINT32 type = (UINT32)gs_c_marquee_ntf::MARQUEE_MIN + 1;
	for(; type < (UINT32)gs_c_marquee_ntf::MARQUEE_MAX; ++type){
		ASSERT(m_mapID2MarqueeCfg.find(type) != m_mapID2MarqueeCfg.end());
	}
	return true;
}



bool CMarqueeMgr::SendMarqueeToAllUsers( gs_c_marquee_ntf::MARQUEE_TYPE eType,  std::map<std::string, CValue>& values,  UINT32 iValue /*= 0*/)
{
	ASSERT(eType > gs_c_marquee_ntf::MARQUEE_MIN && eType < gs_c_marquee_ntf::MARQUEE_MAX);
	
	gs_c_marquee_ntf marquee_ntf;
	marquee_ntf.set_marquee_type(eType);
	marquee_ntf.set_marquee_times(m_mapID2MarqueeCfg[eType].iShowTimes);
	
	char buf[128] = {0};
	switch(eType)
	{
	case gs_c_marquee_ntf::MARQUEE_INVOKE:
	case gs_c_marquee_ntf::MARQUEE_INVOKE_HERO:
		{
			char* name = UTF8ToANSI(values["user_name"].GetStringValue());
			sprintf_s(buf, sizeof(buf), "1&%s|||2&%d", name, values["hero_id"].GetIntValue());
			free(name);
		}
		break;
	case gs_c_marquee_ntf::MARQUEE_HERO_EVOLUTION:
		{
			ASSERT(iValue);
			char* name = UTF8ToANSI(values["user_name"].GetStringValue());
			sprintf_s(buf, sizeof(buf), "1&%s|||2&%d|||3&%d", name, values["hero_id"].GetIntValue(), iValue);
			free(name);
		}
		break;
	case gs_c_marquee_ntf::MARQUEE_HERO_STAR_UP:
		{
			ASSERT(iValue);
			char* name = UTF8ToANSI(values["user_name"].GetStringValue());
			sprintf_s(buf, sizeof(buf), "1&%s|||2&%d|||4&%d", name, values["hero_id"].GetIntValue(), iValue);
			free(name);
		}
		break;
	case gs_c_marquee_ntf::MARQUEE_ARENA_CHALLENGE:
		{
			ASSERT(iValue);
			char* name = UTF8ToANSI(values["user_name"].GetStringValue());
			sprintf_s(buf, sizeof buf, "1&%s|||5&%d", name, iValue);
			free(name);
		}
		break;
	default:
		ASSERT(false && "不支持类型");
		return false;
	}
	char* content  = ANSIToUTF8(buf);
	marquee_ntf.set_content(content);
	free(content);
	CServerForUser::GetInstance()->BroadCast(e_msg_gs_c_marquee_ntf, &marquee_ntf);
	return true;
}

bool CMarqueeMgr::SendMarqueeToAllUsers(gs_c_marquee_ntf::MARQUEE_TYPE eType,  const char* content)
{
	ASSERT(eType > gs_c_marquee_ntf::MARQUEE_MIN && eType < gs_c_marquee_ntf::MARQUEE_MAX);
	gs_c_marquee_ntf marquee_ntf;
	marquee_ntf.set_marquee_type(eType);
	marquee_ntf.set_marquee_times(m_mapID2MarqueeCfg[eType].iShowTimes);
	marquee_ntf.set_content(content);
	CServerForUser::GetInstance()->BroadCast(e_msg_gs_c_marquee_ntf, &marquee_ntf);
	return true;
}

bool CMarqueeMgr::CanTriggerMarquee( gs_c_marquee_ntf::MARQUEE_TYPE eType, UINT32 iValue)
{
	const stMarqueeCfg& cfg = m_mapID2MarqueeCfg[(UINT32)eType];
	
	switch(eType)
	{
	case gs_c_marquee_ntf::MARQUEE_INVOKE:
	case gs_c_marquee_ntf::MARQUEE_INVOKE_HERO:
	case gs_c_marquee_ntf::MARQUEE_HERO_STAR_UP:
		{
			return iValue >= cfg.vecValue[0];
		}
		break;
	case gs_c_marquee_ntf::MARQUEE_HERO_EVOLUTION:
	case gs_c_marquee_ntf::MARQUEE_ARENA_CHALLENGE:
		{
			return std::find(cfg.vecValue.begin(), cfg.vecValue.end(), iValue) != cfg.vecValue.end();
		}
		break;
	default:
		ASSERT(false && "不支持类型");
		return false;
	}
}


std::vector<UINT32>* CMarqueeMgr::GetTriggerValue(gs_c_marquee_ntf::MARQUEE_TYPE eType){
	ASSERT(eType > gs_c_marquee_ntf::MARQUEE_MIN && eType < gs_c_marquee_ntf::MARQUEE_MAX);

	std::map<UINT32, stMarqueeCfg>::iterator it = m_mapID2MarqueeCfg.find(eType);
	ASSERT(it != m_mapID2MarqueeCfg.end());
	return &it->second.vecValue;
}
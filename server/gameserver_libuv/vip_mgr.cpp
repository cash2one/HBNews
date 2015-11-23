#include "vip_mgr.h"
#include "db_sql.h"
#include "db_mgr.h"
#include "str.h"
bool CVipMgr::Init()
{
	MYSQL* con = CDBMgr::GetInstance()->GetConnection();

	//解析表！
	std::string strSql;
	strSql = SELECT_VIP;
	strSql += SELECT_VIP_BUY;

	int code = mysql_real_query(con, strSql.c_str(), strSql.length());
	if (0 != code)   //非0查询失败  
	{
		printf("query failed! [%s]\n", strSql.c_str());  
		return false;  
	}
	
	int i = 0;
	do
	{
		MYSQL_RES* res = mysql_store_result(con);
		switch(i++)
		{
		case 0:
			{
				{//"prize",将被配置到每日任务中去
					std::string need_fields[] = {"vip_level", "rmb", "buy_num_health", "buy_num_instance", "buy_num_arena", "buy_num_gold", "max_num_skill_point", "max_num_exp_battle", "max_num_gold_battle", "max_num_tower", "max_num_crusade", "max_times_buy_skill_point"};
					std::vector<std::string> v_need_fields(need_fields, need_fields + sizeof(need_fields)/sizeof(need_fields[0]));
					ASSERT(CDBMgr::check_fields(res, v_need_fields));
				}
				MYSQL_ROW row;
				while(row = mysql_fetch_row(res))
				{
					int col = 0;
					CVipCfg* pVipCfg = new CVipCfg;
					pVipCfg->m_vip_level = atoi(row[col++]);
					pVipCfg->m_rmb = atoi(row[col++]);
					//pVipCfg->m_prize = atoi[col++];
					pVipCfg->m_buy_num_health = atoi(row[col++]);
					pVipCfg->m_buy_num_instance = atoi(row[col++]);
					pVipCfg->m_buy_num_arena = atoi(row[col++]);
					pVipCfg->m_buy_num_gold = atoi(row[col++]);
					pVipCfg->m_max_num_skill_point = atoi(row[col++]);
					pVipCfg->m_max_num_exp_battle = atoi(row[col++]);
					pVipCfg->m_max_num_gold_battle = atoi(row[col++]);
					pVipCfg->m_max_num_tower = atoi(row[col++]);		
					pVipCfg->m_max_num_crusade = atoi(row[col++]);	
					pVipCfg->m_buy_skill_point = atoi(row[col++]);
					ASSERT(m_mapVipLevel2VipCfg.find(pVipCfg->m_vip_level) == m_mapVipLevel2VipCfg.end());
					m_mapVipLevel2VipCfg[pVipCfg->m_vip_level] = pVipCfg;
				}
			}
			break;
		case 1:
			{
				{
					std::string need_fields[] = {"buy_type", "no_less_than_buy_num", "diamond"};
					std::vector<std::string> v_need_fields(need_fields, need_fields + sizeof(need_fields)/sizeof(need_fields[0]));
					ASSERT(CDBMgr::check_fields(res, v_need_fields));
				}
				MYSQL_ROW row;
				while(row = mysql_fetch_row(res))
				{
					int col = 0;
					EBuy_Type eType = EBuy_Type(atoi(row[col++]));
					UINT32 iNoLessThenNum = atoi(row[col++]);
					UINT32 iDiamond = atoi(row[col++]);
					//ASSERT(m_mapBuyType2Cfg[eType].find(iNoLessThenNum) == m_mapBuyType2Cfg[eType].end());
					//m_mapBuyType2Cfg[eType][iNoLessThenNum] = iDiamond;
					m_mapBuyType2Cfg[eType].push_back(std::make_pair(iNoLessThenNum, iDiamond));

				}
			}
			break;
		}
		mysql_free_result(res);
	}
	while(!mysql_next_result(con));

	return true;
}

CVipCfg* CVipMgr::GetVipCfgByVipLevel( UINT32 iVipLevel )
{
	std::map<UINT32, CVipCfg*>::iterator iter = m_mapVipLevel2VipCfg.find(iVipLevel);
	if (iter == m_mapVipLevel2VipCfg.end())
	{
		return NULL;
	}
	return iter->second;
}

UINT32 CVipMgr::GetCostDiamond( EBuy_Type eBuyType, UINT32 iTimes )
{
	ASSERT(iTimes >= 1);
	if (iTimes < 1){
		return -1;
	}

	std::map<EBuy_Type, std::vector<std::pair<UINT32, UINT32>>>::iterator iter = m_mapBuyType2Cfg.find(eBuyType);
	ASSERT(iter != m_mapBuyType2Cfg.end());
	if (iter == m_mapBuyType2Cfg.end()){
		return -1;
	}
	std::vector<std::pair<UINT32, UINT32>>& vBuyInfo = iter->second;
	ASSERT(!vBuyInfo.empty());
	if (vBuyInfo.empty()){
		return -1;
	}
	
	//std::vector<std::pair<UINT32, UINT32>>::iterator ite = vBuyInfo.end();
	ASSERT( iTimes >= vBuyInfo[vBuyInfo.size()-1].first);//必须超过第一个 否则报错！
	if (iTimes < vBuyInfo[vBuyInfo.size()-1].first){
		return -1;
	}

	for (std::vector<std::pair<UINT32, UINT32>>::iterator it = vBuyInfo.begin(); it != vBuyInfo.end(); ++it){
		if (iTimes >= it->first)
		{
			return it->second;
		}
	}

	ASSERT(false && "获取钻石消耗失败");
	return -1;
}

UINT32 CVipMgr::GetMaxSkillPoint( UINT32 iVipLevel )
{
	CVipCfg* cfg = GetVipCfgByVipLevel(iVipLevel);
	ASSERT(cfg);
	if (cfg)
	{
		return cfg->m_max_num_skill_point;
	}

	return 0;
}
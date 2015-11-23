#include "rank_mgr.h"
#include "arena.h"
#include "db_mgr.h"
#include "time_helper.h"
#include "client_for_dbs.h"
bool CRankMgr::Init(){
	//排行榜加载
	MYSQL* con = CDBMgr::GetInstance()->GetConnection();
	CHECK_NULL_RETURN_FALSE(con);

	char* sql = 
	//战斗力排行(全部)
	"set @rownum=0; "
	"SELECT  (@rownum:=@rownum+1) as `fight_rank`, `user_id` "
		"FROM `user` "
		"ORDER BY `fight_power` DESC, `level`  DESC, `vip` DESC, `user_id` ASC; "

	//等级排行(全部)
	"set @rownum=0; "
	"SELECT (@rownum:=@rownum+1) as `level_rank`, `user_id` "
		"FROM `user` "
		"ORDER BY `level`  DESC, `fight_power` DESC,  `vip` DESC, `user_id` ASC; "
	//战力排行(详细，前50)
	"set @rownum=0; "
	"SELECT (@rownum:=@rownum+1) as `fight_rank`, t1.user_id, t1.`head_id`, t1.`head_frame_id`, t1.`level`, t1.`name`, t1.`vip`, t1.`fight_power`, t2.rank as `arena_rank` "
		"FROM `user` AS t1 "
		"LEFT JOIN `user_arena` AS t2 ON t1.user_id = t2.user_id "
		"ORDER BY t1.`fight_power` DESC, t1.`level`  DESC, t1.`vip` DESC, t1.`user_id` ASC LIMIT 50; "
	//等级排行(详细)
	"set @rownum=0; "
	"SELECT (@rownum:=@rownum+1) as `level_rank`, t1.`user_id`, t1.`head_id`, t1.`head_frame_id`, t1.`level`, t1.`name`, t1.`vip`, t1.`fight_power`, t2.rank as `arena_rank` "
		"FROM `user` AS t1 "
		"LEFT JOIN `user_arena` AS t2 ON t1.user_id = t2.user_id "
		"ORDER BY t1.`level`  DESC,t1.`fight_power` DESC,  t1.`vip` DESC, t1.`user_id` ASC LIMIT 50; "
	//竞技场排行(详细)
	"SELECT t1.`rank`, t2.`user_id`, t2.`head_id`, t2.`head_frame_id`, t2.`level`, t2.`name`, t2.`vip`, t2.`fight_power`  "
		"FROM `user_arena`  AS t1  "
		"LEFT JOIN `user` AS t2 ON t1.user_id = t2.user_id "
		"WHERE t1.`rank` != 0 "
		"ORDER BY t1.`rank` ASC LIMIT 50; ";
	int code = CDBMgr::GetInstance()->Query(con, sql, (unsigned int)strlen(sql));
	if (0 != code)   //非0查询失败  
	{
		printf("query failed! [%s] [%d] [%s] \n", sql, mysql_errno(con), mysql_error(con));       
		return false;  
	
	}
	UINT32 iIndex = 0;
	ClearRank();
	int i = 0;
	do
	{
		MYSQL_RES* res = mysql_store_result( con );
		if (!res)
		{
			continue;
		}
		switch (i++)
		{
		case 0:
			{//全部战力排行(不含详细信息)
				{
					//检查表结构是否有改变
					std::string need_fields[] = { "fight_rank", "user_id"};
					std::vector<std::string> v_need_fields(need_fields, need_fields + sizeof(need_fields)/sizeof(need_fields[0]));
					ASSERT(CDBMgr::check_fields(res, v_need_fields));
				}
				MYSQL_ROW row;
				while(row = mysql_fetch_row(res))
				{
					UINT32 col = 0;
					UINT32 iRank = atoi(row[col++]);
					UINT32 iUserID = atoi(row[col++]);
					m_mapUsersFightPowerRanks[iUserID] = iRank;
				}
			}break;
		case 1:
			{//全部等级排行(不含详细信息)
				{
					//检查表结构是否有改变
					std::string need_fields[] = { "level_rank", "user_id"};
					std::vector<std::string> v_need_fields(need_fields, need_fields + sizeof(need_fields)/sizeof(need_fields[0]));
					ASSERT(CDBMgr::check_fields(res, v_need_fields));
				}
				MYSQL_ROW row;
				while(row = mysql_fetch_row(res))
				{
					UINT32 col = 0;
					UINT32 iLevelRank = atoi(row[col++]);
					UINT32 iUserID = atoi(row[col++]);
					m_mapUsersLevelRanks[iUserID] = iLevelRank;
				}
			}break;
		case 2:{//战力排行(包含详细信息)
				   {
					   //检查表结构是否有改变
					   std::string need_fields[] = { "fight_rank", "user_id", "head_id", "head_frame_id", "level", "name", "vip", "fight_power", "arena_rank"};
					   std::vector<std::string> v_need_fields(need_fields, need_fields + sizeof(need_fields)/sizeof(need_fields[0]));
					   ASSERT(CDBMgr::check_fields(res, v_need_fields));
				   }
				   iIndex = 0;
				   MYSQL_ROW row;
				   while (row = mysql_fetch_row(res)){
					   UINT32 col = 0;
					   stRank& rank = vecFightPowerRank[iIndex++];
					   rank.fight_rank = atoi(row[col++]);
					   UINT32 iUserID = atoi(row[col++]);
					   rank.head_id = atoi(row[col++]);
					   rank.head_frame_id = atoi(row[col++]);
					   rank.level = atoi(row[col++]);
					   sprintf_s(rank.name, MAXLEN_USERNAME, row[col++]);
					   rank.vip = atoi(row[col++]);
					   rank.fight_power = atoi(row[col++]);
					   if (row[col++])//有竞技场排行
					   {
						   UINT32 iArenaRank = atoi(row[col - 1]);
						   rank.arena_rank = iArenaRank;
						   ASSERT(iUserID >0);
						   CArenaContestant* pContestant = CArena::GetInstance()->GetArenaContestantByUserID(iUserID);
						   //ASSERT(pContestant);
						   if(pContestant){
							   for (UINT32 i = 0 ; i < MAXNUM_HEROINTEAM; ++i){
								   CArenaHero* pHero = pContestant->m_defend_heros[i];
								   if(pHero){
									  gs_c_rank_res::user_hero_brief_info* hero = rank.heros.Add();
									  hero->set_hero_id(pHero->hero_id);
									  hero->set_level(pHero->level);
									  hero->set_star(pHero->star);
									  hero->set_quality(pHero->quality);
								   }
							   }
						   }
					   }
					   else{
						   rank.arena_rank = 0;
					   }
					   std::map<UINT32, UINT32>::iterator it = m_mapUsersLevelRanks.find(iUserID);
					   if (it != m_mapUsersLevelRanks.end())
					   {
						    rank.level_rank = it->second;
					   }else{
							rank.level_rank = 0;
					   }
					  
				   }
				   ASSERT(MAX_NUM_RANK == iIndex);
			   }
			   break;
		case 3:
			{//等级排行(包含详细信息)
				{
					//检查表结构是否有改变
					std::string need_fields[] = { "level_rank", "user_id", "head_id", "head_frame_id", "level", "name", "vip", "fight_power", "arena_rank"};
					std::vector<std::string> v_need_fields(need_fields, need_fields + sizeof(need_fields)/sizeof(need_fields[0]));
					ASSERT(CDBMgr::check_fields(res, v_need_fields));
				}
				iIndex = 0;
				MYSQL_ROW row;
				while (row = mysql_fetch_row(res)){
					UINT32 col = 0;
					stRank& rank = vecLevelRank[iIndex++];
					rank.level_rank = atoi(row[col++]);
					UINT32 iUserID = atoi(row[col++]);
					rank.head_id = atoi(row[col++]);
					rank.head_frame_id = atoi(row[col++]);
					rank.level = atoi(row[col++]);
					sprintf_s(rank.name, MAXLEN_USERNAME, row[col++]);
					rank.vip = atoi(row[col++]);
					rank.fight_power = atoi(row[col++]);
					if (row[col++])//有竞技场排行
					{
						UINT32 iArenaRank = atoi(row[col - 1]);
						rank.arena_rank = iArenaRank;
						ASSERT(iUserID >0);
						CArenaContestant* pContestant = CArena::GetInstance()->GetArenaContestantByUserID(iUserID);
						if(pContestant){
							for (UINT32 i = 0 ; i < MAXNUM_HEROINTEAM; ++i){
								CArenaHero* pHero = pContestant->m_defend_heros[i];
								if(pHero){
									gs_c_rank_res::user_hero_brief_info* hero = rank.heros.Add();
									hero->set_hero_id(pHero->hero_id);
									hero->set_level(pHero->level);
									hero->set_star(pHero->star);
									hero->set_quality(pHero->quality);
								}
							}
						}
					}
					else{
						rank.arena_rank = 0;
					}
					std::map<UINT32, UINT32>::iterator it = m_mapUsersFightPowerRanks.find(iUserID);
					if (it != m_mapUsersFightPowerRanks.end())
					{
						rank.fight_rank = it->second;
					}
					else{
						rank.fight_rank = 0;
					}
				}
				ASSERT(MAX_NUM_RANK == iIndex);
			}
			break;
		case 4:
			{//竞技场排行
				{
					//检查表结构是否有改变
					std::string need_fields[] = { "rank", "user_id", "head_id", "head_frame_id", "level", "name", "vip", "fight_power"};
					std::vector<std::string> v_need_fields(need_fields, need_fields + sizeof(need_fields)/sizeof(need_fields[0]));
					ASSERT(CDBMgr::check_fields(res, v_need_fields));
				}
				iIndex = 0;
				MYSQL_ROW row;
				while (row = mysql_fetch_row(res)){
					UINT32 col = 0;
					stRank& rank = vecArenaRank[iIndex++];
					UINT32 iArenaRank = atoi(row[col++]);
					ASSERT(iArenaRank);
					rank.arena_rank = iArenaRank;
					CArenaContestant* pContestant = CArena::GetInstance()->GetArenaContestantInRankList(iArenaRank);
					ASSERT(pContestant);
					for (UINT32 i = 0 ; i < MAXNUM_HEROINTEAM; ++i){
						CArenaHero* pHero = pContestant->m_defend_heros[i];
						if(pHero){
							gs_c_rank_res::user_hero_brief_info* hero = rank.heros.Add();
							hero->set_hero_id(pHero->hero_id);
							hero->set_level(pHero->level);
							hero->set_star(pHero->star);
							hero->set_quality(pHero->quality);
						}
					}
					UINT32 iUserID = atoi(row[col++]);
					rank.head_id = atoi(row[col++]);
					rank.head_frame_id = atoi(row[col++]);
					rank.level = atoi(row[col++]);
					sprintf_s(rank.name, MAXLEN_USERNAME, row[col++]);
					rank.vip = atoi(row[col++]);
					rank.fight_power = atoi(row[col++]);

					std::map<UINT32, UINT32>::iterator itLevel = m_mapUsersLevelRanks.find(iUserID);
					if (itLevel != m_mapUsersLevelRanks.end())
					{
						rank.level_rank = itLevel->second;
					}else{
						rank.level_rank = 0;
					}

					std::map<UINT32, UINT32>::iterator itFight = m_mapUsersFightPowerRanks.find(iUserID);
					if (itFight != m_mapUsersFightPowerRanks.end())
					{
						rank.fight_rank = itFight->second;
					}
					else{
						rank.fight_rank = 0;
					}
				}
				ASSERT(MAX_NUM_RANK == iIndex);
			}
			break;
		}
		mysql_free_result(res);
	}while( !mysql_next_result( con ) );
	m_iLastUpdateTime = Now();
	return true;
}

void CRankMgr::ClearRank()
{
	vecArenaRank.clear();
	vecFightPowerRank.clear();
	vecLevelRank.clear();
	vecGuildRank.clear();

	vecArenaRank.resize(MAX_NUM_RANK);
	vecFightPowerRank.resize(MAX_NUM_RANK);
	vecLevelRank.resize(MAX_NUM_RANK);
	vecGuildRank.resize(MAX_NUM_RANK);

	m_mapUsersLevelRanks.clear();
	m_mapUsersFightPowerRanks.clear();
}

void CRankMgr::OnTime( UINT32 iTimer )
{
	if(iTimer - m_iLastUpdateTime == 1800)//半小时
	{
		//需要从数据库拉取最新的数据
		gs_dbs_rank_req* db_req = new gs_dbs_rank_req;
		bool r = CClientForDBS::GetInstance()->SendAsync(CClientForDBSAsync::EASyncOP_User_Rank, 0, db_req);
		if (!r)
		{
			printf("获取排行榜请求DB线程失败！");
			delete db_req;
			return;
		}
		m_iLastUpdateTime = Now();
	}
}

void CRankMgr::UpdateRank( dbs_gs_rank_res& res )
{
	ClearRank();
	::google::protobuf::RepeatedPtrField< ::dbs_gs_rank_res_rank_brief_info >::const_iterator it = res.level_brief_ranks().begin();
	for (; it != res.level_brief_ranks().end(); ++it)
	{
		ASSERT(m_mapUsersLevelRanks.find(it->user_id()) == m_mapUsersLevelRanks.end());
		m_mapUsersLevelRanks[it->user_id()] = it->rank();
	}

	it = res.fight_power_brief_ranks().begin();
	for (; it != res.fight_power_brief_ranks().end(); ++it)
	{
		ASSERT(m_mapUsersFightPowerRanks.find(it->user_id()) == m_mapUsersFightPowerRanks.end());
		m_mapUsersFightPowerRanks[it->user_id()] = it->rank();
	}
	
	//战斗力排名
	UINT32 iIndex = 0;
	::google::protobuf::RepeatedPtrField< ::dbs_gs_rank_res_rank_info >::const_iterator itRank = res.fight_power_ranks().begin();
	for (; itRank != res.fight_power_ranks().end(); ++itRank)
	{
		stRank& rank = vecFightPowerRank[iIndex++];
		UINT32 iUserID = itRank->user_id();
		rank.head_id = itRank->head_id();
		rank.head_frame_id = itRank->head_frame_id();
		rank.level = itRank->level();
		strncpy(rank.name, itRank->name().c_str() , MAXLEN_USERNAME);

		CArenaContestant* pCon = CArena::GetInstance()->GetArenaContestantByUserID(iUserID);
		if (pCon)
		{
			rank.arena_rank = pCon->m_rank;
			for (UINT32 i = 0 ; i < MAXNUM_HEROINTEAM; ++i){
				CArenaHero* pHero = pCon->m_defend_heros[i];
				if(pHero){
					gs_c_rank_res::user_hero_brief_info* hero = rank.heros.Add();
					hero->set_hero_id(pHero->hero_id);
					hero->set_level(pHero->level);
					hero->set_star(pHero->star);
					hero->set_quality(pHero->quality);
				}
			}
		}else{
			rank.arena_rank = 0;
		}
		
		if (m_mapUsersFightPowerRanks.find(iUserID) != m_mapUsersFightPowerRanks.end())
		{
			rank.fight_rank = m_mapUsersFightPowerRanks[iUserID];
		}else{
			rank.fight_rank = 0;
		}

		rank.fight_power = itRank->fight_power();

		if (m_mapUsersLevelRanks.find(iUserID) != m_mapUsersLevelRanks.end())
		{
			rank.level_rank = m_mapUsersLevelRanks[iUserID];
		}else{
			rank.level_rank = 0;
		}
		rank.vip = itRank->vip();
	}
	ASSERT(MAX_NUM_RANK == iIndex);
	//等级排名
	iIndex = 0;
	itRank = res.level_ranks().begin();
	for (; itRank != res.level_ranks().end(); ++itRank)
	{
		stRank& rank = vecLevelRank[iIndex++];
		UINT32 iUserID = itRank->user_id();
		rank.head_id = itRank->head_id();
		rank.head_frame_id = itRank->head_frame_id();
		rank.level = itRank->level();
		strncpy(rank.name, itRank->name().c_str(), MAXLEN_USERNAME);

		CArenaContestant* pCon = CArena::GetInstance()->GetArenaContestantByUserID(iUserID);
		if (pCon)
		{
			rank.arena_rank = pCon->m_rank;
			for (UINT32 i = 0 ; i < MAXNUM_HEROINTEAM; ++i){
				CArenaHero* pHero = pCon->m_defend_heros[i];
				if(pHero){
					gs_c_rank_res::user_hero_brief_info* hero = rank.heros.Add();
					hero->set_hero_id(pHero->hero_id);
					hero->set_level(pHero->level);
					hero->set_star(pHero->star);
					hero->set_quality(pHero->quality);
				}
			}
		}else{
			rank.arena_rank = 0;
		}

		if (m_mapUsersFightPowerRanks.find(iUserID) != m_mapUsersFightPowerRanks.end())
		{
			rank.fight_rank = m_mapUsersFightPowerRanks[iUserID];
		}else{
			rank.fight_rank = 0;
		}

		rank.fight_power = itRank->fight_power();

		if (m_mapUsersLevelRanks.find(iUserID) != m_mapUsersLevelRanks.end())
		{
			rank.level_rank = m_mapUsersLevelRanks[iUserID];
		}else{
			rank.level_rank = 0;
		}
		rank.vip = itRank->vip();
	}
	ASSERT(MAX_NUM_RANK == iIndex);

	//竞技场排名
	iIndex = 0;
	itRank = res.arena_ranks().begin();
	for (; itRank != res.arena_ranks().end(); ++itRank)
	{
		stRank& rank = vecArenaRank[iIndex++];
		UINT32 iUserID = itRank->user_id();
		rank.head_id = itRank->head_id();
		rank.head_frame_id = itRank->head_frame_id();
		rank.level = itRank->level();
		strncpy(rank.name, itRank->name().c_str(), MAXLEN_USERNAME);

		CArenaContestant* pCon = CArena::GetInstance()->GetArenaContestantByUserID(iUserID);
		ASSERT(pCon);
		if (pCon)
		{
			rank.arena_rank = pCon->m_rank;
			for (UINT32 i = 0 ; i < MAXNUM_HEROINTEAM; ++i){
				CArenaHero* pHero = pCon->m_defend_heros[i];
				if(pHero){
					gs_c_rank_res::user_hero_brief_info* hero = rank.heros.Add();
					hero->set_hero_id(pHero->hero_id);
					hero->set_level(pHero->level);
					hero->set_star(pHero->star);
					hero->set_quality(pHero->quality);
				}
			}
		}else{
			rank.arena_rank = 0;
		}

		if (m_mapUsersFightPowerRanks.find(iUserID) != m_mapUsersFightPowerRanks.end())
		{
			rank.fight_rank = m_mapUsersFightPowerRanks[iUserID];
		}else{
			rank.fight_rank = 0;
		}

		rank.fight_power = itRank->fight_power();

		if (m_mapUsersLevelRanks.find(iUserID) != m_mapUsersLevelRanks.end())
		{
			rank.level_rank = m_mapUsersLevelRanks[iUserID];
		}else{
			rank.level_rank = 0;
		}
		rank.vip = itRank->vip();
	}
	ASSERT(MAX_NUM_RANK == iIndex);

}

bool CRankMgr::GetRank(UINT32 iUserID, c_gs_rank_req::RANK_TYPE eType, gs_c_rank_res& res)
{
	switch(eType)
	{
	case c_gs_rank_req::ARENA:{
		for(std::vector<stRank>::iterator it = vecArenaRank.begin(); it != vecArenaRank.end(); ++it)
		{
			gs_c_rank_res::rank_info* info = res.add_ranks();
			info->set_head_id(it->head_id);
			info->set_head_frame_id(it->head_frame_id);
			info->set_level(it->level);
			info->set_name(it->name);
			info->set_arena_rank(it->arena_rank);	//竞技场排名
			info->set_fight_rank(it->fight_rank);	//战力排名
			info->set_fight_power(it->fight_power);//战力
			info->set_level_rank(it->level_rank);	//等级排名
			info->set_vip(it->vip);	//vip
			info->mutable_heros()->CopyFrom(it->heros);

			CArenaContestant* pCon = CArena::GetInstance()->GetArenaContestantByUserID(iUserID);
			if (pCon)
			{
				res.set_me_cur_rank(pCon->m_rank);
				res.set_me_cur_value(pCon->m_fighting_power_me);
			}else{
				res.set_me_cur_rank(0);
				res.set_me_cur_value(0);
			}
		}
	}break;
	case c_gs_rank_req::FIGHT_POWER:{
		for(std::vector<stRank>::iterator it = vecFightPowerRank.begin(); it != vecFightPowerRank.end(); ++it)
		{
			gs_c_rank_res::rank_info* info = res.add_ranks();
			info->set_head_id(it->head_id);
			info->set_head_frame_id(it->head_frame_id);
			info->set_level(it->level);
			info->set_name(it->name);
			info->set_arena_rank(it->arena_rank);	//竞技场排名
			info->set_fight_rank(it->fight_rank);	//战力排名
			info->set_fight_power(it->fight_power);//战力
			info->set_level_rank(it->level_rank);	//等级排名
			info->set_vip(it->vip);	//vip
			info->mutable_heros()->CopyFrom(it->heros);
		}
		if (m_mapUsersFightPowerRanks.find(iUserID) != m_mapUsersFightPowerRanks.end())
		{
			res.set_me_cur_rank(m_mapUsersFightPowerRanks[iUserID]);
			//res.set_me_cur_value();战力客户端读取
		}else{
			res.set_me_cur_rank(0);
		}
	}break;
	case c_gs_rank_req::LEVEL:
		{
			for(std::vector<stRank>::iterator it = vecLevelRank.begin(); it != vecLevelRank.end(); ++it)
			{
				gs_c_rank_res::rank_info* info = res.add_ranks();
				info->set_head_id(it->head_id);
				info->set_head_frame_id(it->head_frame_id);
				info->set_level(it->level);
				info->set_name(it->name);
				info->set_arena_rank(it->arena_rank);	//竞技场排名
				info->set_fight_rank(it->fight_rank);	//战力排名
				info->set_fight_power(it->fight_power);//战力
				info->set_level_rank(it->level_rank);	//等级排名
				info->set_vip(it->vip);	//vip
				info->mutable_heros()->CopyFrom(it->heros);
			}
			if (m_mapUsersLevelRanks.find(iUserID) != m_mapUsersLevelRanks.end())
			{
				res.set_me_cur_rank(m_mapUsersLevelRanks[iUserID]);
			}else{
				res.set_me_cur_rank(0);
			}
		}break;
	case c_gs_rank_req::GUILD:{
		}break;
	default:
		res.set_error_code(e_gsc_errorcode_unknown_error);
		ASSERT(false);
		return false;
	}
	res.set_rank_type(eType);
	res.set_error_code(e_gsc_errorcode_success);
	return true;
}

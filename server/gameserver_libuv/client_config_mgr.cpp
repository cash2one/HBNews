#include <iostream>
#include <fstream>
#include <stdlib.h>
#include <sstream>

#include <set>
#include "client_config_mgr.h"
#include "db_mgr.h"
#include "net_config.h"
#include "def.h"
#include "str.h"

#include "server_for_user.h"

bool CClientConfigMgr::Init()
{
	ASSERT(m_hasInited == false);

	//client_config和配置表及pb结构的方式， 现在暂时不用， 改用直接发json文件内容的方式， 因为是版本不一样时才下载 所以多消耗的流量并不严重
	MYSQL* con = CDBMgr::GetInstance()->GetConnection();
	char sql[1024*10] = {0};
	sprintf_s(sql, sizeof(sql), 
		"SELECT * FROM `client_config`;"//首先获取所有的表的md5值
		"SELECT * FROM `client_config_chaptermap`;"
		"SELECT * FROM `client_config_task`;"
		"SELECT * FROM `client_config_subtask`;"
		"SELECT * FROM `client_config_battle`;"
		"SELECT * FROM `client_config_heroprop`;"
		"SELECT * FROM `client_config_bossprop`;"
		"SELECT * FROM `client_config_monsterprop`;"
		"SELECT * FROM `client_config_equip`;"
		"SELECT * FROM `client_config_aoyi`;"
		"SELECT * FROM `client_config_bag`;"
		"SELECT * FROM `client_config_passive`;"
		"SELECT * FROM `client_config_heroskill`;"
		"SELECT * FROM `client_config_bossskill`;"
		"SELECT * FROM `client_config_buff`;"
		"SELECT * FROM `client_config_coinprice`;"
		"SELECT * FROM `client_config_prizeshow`;"
		"SELECT * FROM `client_config_realtimecheck`;"
		"SELECT * FROM `client_config_resetprice`;"
		"SELECT * FROM `client_config_spprice`;"
		"SELECT * FROM `client_config_staprice`;"
		"SELECT * FROM `client_config_triggerattack`;"
		"SELECT * FROM `client_config_triggerstartcondition`;"
		"SELECT * FROM `client_config_vip`;"
		"SELECT * FROM `client_config_vipfunction`;"
		"SELECT * FROM `client_config_shopresetprice`;"
		"SELECT * FROM `client_config_herostar`;"
		"SELECT * FROM `client_config_evolutioncoin`;"
		"SELECT * FROM `client_config_qualityrule`;"
		"SELECT * FROM `client_config_rule`;"
		);
	int code = CDBMgr::Query(con, sql, (unsigned int)strlen(sql));
	if (0 != code)   //非0查询失败  
	{
		printf("query failed! [%s] [%d] [%s] \n", sql, mysql_errno(con), mysql_error(con));
		return false;
	}

	int i = 0;
	std::set<std::string> names;
	do
	{
		MYSQL_RES* res = mysql_store_result( con );
		switch (i++)
		{
		case 0:{//所有配置表的md5
				   {
					   std::string need_fields[] = {"name","md5", "last_update_time"};
					   std::vector<std::string> v_need_field(need_fields, need_fields + sizeof(need_fields)/sizeof(need_fields[0]));
					   ASSERT(CDBMgr::check_fields(res, v_need_field));
				   }

				   MYSQL_ROW row;
				   while (row = mysql_fetch_row(res)){
					   ASSERT(row[0]);
					   char* name = UTF8ToANSI(row[0]);
					   names.insert(std::string(name));
					   free(name);

					   gs_c_json_config_version_res::config_version* pItem = m_config_version.add_versions();
					   ASSERT(pItem);
					   int col = 0;
					   
					   name = row[col++];
					   char* md5 = row[col++];

					   CHECK_NULL_RETURN_FALSE(name && md5);
					   ASSERT(name && md5);
					   pItem->set_name(name);
					   pItem->set_md5(md5);
				   }
			   }
			   break;

		case 1:{//chapter_map
				   {
					   std::string need_fields[] = {"id","capterType","pointType","openLevel"};
					   std::vector<std::string> v_need_field(need_fields, need_fields + sizeof(need_fields)/sizeof(need_fields[0]));
					   ASSERT(CDBMgr::check_fields(res, v_need_field));
				   }

				   ASSERT(names.find("chapter_map") != names.end());
				   CHECK_FALSE_RETURN_FALSE(names.find("chapter_map") != names.end());

				   MYSQL_ROW row;
				   while (row = mysql_fetch_row(res))
				   {
					   int col = 0;
					   CHAPTERMAP* pItem = m_config.mutable_chapter_map()->add_items();
					   ASSERT(pItem);
					   pItem->set_id(row[col++]);
					   pItem->set_captertype(atoi(row[col++]));
					   pItem->set_pointtype(atoi(row[col++]));
					   pItem->set_openlevel(atoi(row[col++]));
				   }
			   }
			break;
		case 2:{//task
				   {
					   std::string need_fields[] = {"id","seriesType","seriesName_ch","rewardId","openTimeType","openTime","totalEnterNum","enterLimit","pointParentId","nextPoint","priority"};
					   std::vector<std::string> v_need_field(need_fields, need_fields + sizeof(need_fields)/sizeof(need_fields[0]));
					   ASSERT(CDBMgr::check_fields(res, v_need_field));
				   }

				   ASSERT(names.find("task") != names.end());
				   CHECK_FALSE_RETURN_FALSE(names.find("task") != names.end());

				   MYSQL_ROW row;
				   while (row = mysql_fetch_row(res))
				   {
					   int col = 0;
					   TASK* pItem = m_config.mutable_task()->add_items();
					   ASSERT(pItem);
					   pItem->set_id(row[col++]);
					   pItem->set_seriestype(atoi(row[col++]));
					   pItem->set_seriesname_ch(row[col++]);
					   pItem->set_rewardid(row[col++]);
					   pItem->set_opentimetype(atoi(row[col++]));
					   pItem->set_opentime(row[col++]);
					   pItem->set_totalenternum(atoi(row[col++]));
					   pItem->set_enterlimit(row[col++]);
					   pItem->set_pointparentid(row[col++]);
					   pItem->set_nextpoint(row[col++]);
					   pItem->set_priority(atof(row[col++]));


				   }
			   }
			   break;
		case 3:{//sub_task
				   {
					   std::string need_fields[] = {"id","battleData","diffs","totalTimes","name_ch","expendSt","fightCount","idExp","heroExp","dropGold","pointParentId","priority", "fightPower"};
					   std::vector<std::string> v_need_field(need_fields, need_fields + sizeof(need_fields)/sizeof(need_fields[0]));
					   ASSERT(CDBMgr::check_fields(res, v_need_field));
				   }

				   ASSERT(names.find("sub_task") != names.end());
				   CHECK_FALSE_RETURN_FALSE(names.find("sub_task") != names.end());

				   MYSQL_ROW row;
				   while (row = mysql_fetch_row(res))
				   {
					   int col = 0;
					   SUBTASK* pItem = m_config.mutable_sub_task()->add_items();
					   ASSERT(pItem);
					   pItem->set_id(row[col++]);
					   pItem->set_battledata(row[col++]);
					   pItem->set_diffs(row[col++]);
					   pItem->set_totaltimes(row[col++]);
					   pItem->set_name_ch(row[col++]);
					   pItem->set_expendst(atoi(row[col++]));
					   pItem->set_fightcount(atoi(row[col++]));
					   pItem->set_idexp(atoi(row[col++]));
					   pItem->set_heroexp(atoi(row[col++]));
					   pItem->set_dropgold(atoi(row[col++]));
					   pItem->set_pointparentid(row[col++]);
					   pItem->set_priority(atof(row[col++]));
					   pItem->set_fightpower(atoi(row[col++]));
				   }
			   }
			   break;
		case 4:{//battle
				   {
					   std::string need_fields[] = {"id","infos","positions"};
					   std::vector<std::string> v_need_field(need_fields, need_fields + sizeof(need_fields)/sizeof(need_fields[0]));
					   ASSERT(CDBMgr::check_fields(res, v_need_field));
				   }

				   ASSERT(names.find("battle") != names.end());
				   CHECK_FALSE_RETURN_FALSE(names.find("battle") != names.end());

				   MYSQL_ROW row;
				   while (row = mysql_fetch_row(res))
				   {
					   int col = 0;
					   BATTLE* pItem = m_config.mutable_battle()->add_items();
					   ASSERT(pItem);
					   pItem->set_id(row[col++]);
					   pItem->set_infos(row[col++]);
					   pItem->set_positions(row[col++]);
				   }
			   }
			   break;
		case 5:{//hero_prop
				   {
					   std::string need_fields[] = {"id","area","property","occupation","health","healthGrow","damage","damageGrow","pdd","pddGrow","mdd","mddGrow","hitLevel","hitGrow","dodgeLevel","dodgeGrow","criLevel","criGrow","tenacityLevel","tenacityGrow","pddPierce","pddPierceGrow","type_Normal","type_Skill","percent_Skill","addNum_Skill","addNumGrow_Skill","type_AoYi","percent_AoYi","addNum_AoYi","addNumGrow_AoYi","passiveSkill_1","passiveSkill_2","passiveSkill_3","range_X","range_Y","range_Z","attackSpeed","h_Dis","v_Dis","moveSpeed","star","stage_B","hitNum_B","hitNum_A","posNum","recoverTime_Skill","aoYiNum","equip_Weapon","equip_Necklace","equip_Clothes","equip_Belt","equip_Ring","equip_Boot","needId","skillTriggerProbability","skillTriggerDuration"};
					   std::vector<std::string> v_need_field(need_fields, need_fields + sizeof(need_fields)/sizeof(need_fields[0]));
					   ASSERT(CDBMgr::check_fields(res, v_need_field));
				   }

				   ASSERT(names.find("hero_prop") != names.end());
				   CHECK_FALSE_RETURN_FALSE(names.find("hero_prop") != names.end());

				   MYSQL_ROW row;
				   while (row = mysql_fetch_row(res))
				   {
					   int col = 0;
					   HEROPROP * pItem = m_config.mutable_hero_prop()->add_items();
					   ASSERT(pItem);
					   pItem->set_id(row[col++]);
					   pItem->set_area(atoi(row[col++]));
					   pItem->set_property(atoi(row[col++]));
					   pItem->set_occupation(atoi(row[col++]));
					   pItem->set_health(atoi(row[col++]));
					   pItem->set_healthgrow(row[col++]);
					   pItem->set_damage(atoi(row[col++]));
					   pItem->set_damagegrow(row[col++]);
					   pItem->set_pdd(atoi(row[col++]));
					   pItem->set_pddgrow(row[col++]);
					   pItem->set_mdd(atoi(row[col++]));
					   pItem->set_mddgrow(row[col++]);
					   pItem->set_hitlevel(atoi(row[col++]));
					   pItem->set_hitgrow(row[col++]);
					   pItem->set_dodgelevel(atoi(row[col++]));
					   pItem->set_dodgegrow(row[col++]);
					   pItem->set_crilevel(atoi(row[col++]));
					   pItem->set_crigrow(row[col++]);
					   pItem->set_tenacitylevel(atoi(row[col++]));
					   pItem->set_tenacitygrow(row[col++]);
					   pItem->set_pddpierce(atoi(row[col++]));
					   pItem->set_pddpiercegrow(row[col++]);
					   pItem->set_type_normal(atoi(row[col++]));
					   pItem->set_type_skill(atoi(row[col++]));
					   pItem->set_percent_skill(atoi(row[col++]));
					   pItem->set_addnum_skill(atoi(row[col++]));
					   pItem->set_addnumgrow_skill(atoi(row[col++]));
					   pItem->set_type_aoyi(atoi(row[col++]));
					   pItem->set_percent_aoyi(atoi(row[col++]));
					   pItem->set_addnum_aoyi(atoi(row[col++]));
					   pItem->set_addnumgrow_aoyi(atoi(row[col++]));
					   pItem->set_passiveskill_1(row[col++]);
					   pItem->set_passiveskill_2(row[col++]);
					   pItem->set_passiveskill_3(row[col++]);
					   pItem->set_range_x(atof(row[col++]));
					   pItem->set_range_y(atof(row[col++]));
					   pItem->set_range_z(atof(row[col++]));
					   pItem->set_attackspeed(atof(row[col++]));
					   pItem->set_h_dis(atof(row[col++]));
					   pItem->set_v_dis(atof(row[col++]));
					   pItem->set_movespeed(atof(row[col++]));
					   pItem->set_star(atoi(row[col++]));
					   pItem->set_stage_b(atoi(row[col++]));
					   pItem->set_hitnum_b(row[col++]);
					   pItem->set_hitnum_a(atoi(row[col++]));
					   pItem->set_posnum(atoi(row[col++]));
					   pItem->set_recovertime_skill(atof(row[col++]));
					   pItem->set_aoyinum(atoi(row[col++]));
					   pItem->set_equip_weapon(row[col++]);
					   pItem->set_equip_necklace(row[col++]);
					   pItem->set_equip_clothes(row[col++]);
					   pItem->set_equip_belt(row[col++]);
					   pItem->set_equip_ring(row[col++]);
					   pItem->set_equip_boot(row[col++]);
					   pItem->set_needid(row[col++]);
					   pItem->set_skilltriggerprobability(atoi(row[col++]));
					   pItem->set_skilltriggerduration(atof(row[col++]));
				   }
			   }
			   break;
		case 6:{//boss_prop
				   {
					   std::string need_fields[] = {"id","pointId","area","occupation","level","health","damage","pdd","mdd","hitLevel","dodgeLevel","criLevel","tenacityLevel","pddPierce","passiveSkillId","passiveSkillLevel","range_X","range_Y","range_Z","attackSpeed","h_Dis","v_Dis","moveSpeed","star","stage_B","hitNum_B","skillTimeList","warningTimeList","maxIntensity","recoverIntensityRate","minIntensity","type_Normal"};
					   std::vector<std::string> v_need_field(need_fields, need_fields + sizeof(need_fields)/sizeof(need_fields[0]));
					   ASSERT(CDBMgr::check_fields(res, v_need_field));
				   }

				   ASSERT(names.find("boss_prop") != names.end());
				   CHECK_FALSE_RETURN_FALSE(names.find("boss_prop") != names.end());

				   MYSQL_ROW row;
				   while (row = mysql_fetch_row(res))
				   {
					   int col = 0;
					   BOSSPROP * pItem = m_config.mutable_boss_prop()->add_items();
					   ASSERT(pItem);
					   pItem->set_id(row[col++]);
					   pItem->set_pointid(row[col++]);
					   pItem->set_area(atoi(row[col++]));
					   pItem->set_occupation(atoi(row[col++]));
					   pItem->set_level(atoi(row[col++]));
					   pItem->set_health(atoi(row[col++]));
					   pItem->set_damage(atoi(row[col++]));
					   pItem->set_pdd(atoi(row[col++]));
					   pItem->set_mdd(atoi(row[col++]));
					   pItem->set_hitlevel(atoi(row[col++]));
					   pItem->set_dodgelevel(atoi(row[col++]));
					   pItem->set_crilevel(atoi(row[col++]));
					   pItem->set_tenacitylevel(atoi(row[col++]));
					   pItem->set_pddpierce(atoi(row[col++]));
					   pItem->set_passiveskillid(row[col++]);
					   pItem->set_passiveskilllevel(row[col++]);
					   pItem->set_range_x(atof(row[col++]));
					   pItem->set_range_y(atof(row[col++]));
					   pItem->set_range_z(atof(row[col++]));
					   pItem->set_attackspeed(atof(row[col++]));
					   pItem->set_h_dis(atof(row[col++]));
					   pItem->set_v_dis(atof(row[col++]));
					   pItem->set_movespeed(atof(row[col++]));
					   pItem->set_star(atoi(row[col++]));
					   pItem->set_stage_b(atoi(row[col++]));
					   pItem->set_hitnum_b(row[col++]);
					   pItem->set_skilltimelist(row[col++]);
					   pItem->set_warningtimelist(row[col++]);
					   pItem->set_maxintensity(atoi(row[col++]));
					   pItem->set_recoverintensityrate(atoi(row[col++]));
					   pItem->set_minintensity(atoi(row[col++]));
					   pItem->set_type_normal(atoi(row[col++]));
				   }
			   }
			   break;
		case 7:{//monster_prop
				   {
					   std::string need_fields[] = {"id","area","occupation","level","health","damage","pdd","mdd","hitLevel","dodgeLevel","criLevel","tenacityLevel","pddPierce","range_X","range_Y","range_Z","attackSpeed","h_Dis","v_Dis","moveSpeed","star","maxIntensity","recoverIntensityRate","minIntensity","type_Normal","passiveSkillId","passiveSkillLevel"};
					   std::vector<std::string> v_need_field(need_fields, need_fields + sizeof(need_fields)/sizeof(need_fields[0]));
					   ASSERT(CDBMgr::check_fields(res, v_need_field));
				   }

				   ASSERT(names.find("monster_prop") != names.end());
				   CHECK_FALSE_RETURN_FALSE(names.find("monster_prop") != names.end());

				   MYSQL_ROW row;
				   while (row = mysql_fetch_row(res))
				   {
					   int col = 0;
					   MONSTERPROP * pItem = m_config.mutable_monster_prop()->add_items();
					   ASSERT(pItem);
					   pItem->set_id(row[col++]);
					   pItem->set_area(atoi(row[col++]));
					   pItem->set_occupation(atoi(row[col++]));
					   pItem->set_level(atoi(row[col++]));
					   pItem->set_health(atoi(row[col++]));
					   pItem->set_damage(atoi(row[col++]));
					   pItem->set_pdd(atoi(row[col++]));
					   pItem->set_mdd(atoi(row[col++]));
					   pItem->set_hitlevel(atoi(row[col++]));
					   pItem->set_dodgelevel(atoi(row[col++]));
					   pItem->set_crilevel(atoi(row[col++]));
					   pItem->set_tenacitylevel(atoi(row[col++]));
					   pItem->set_pddpierce(atoi(row[col++]));
					   pItem->set_range_x(atof(row[col++]));
					   pItem->set_range_y(atof(row[col++]));
					   pItem->set_range_z(atof(row[col++]));
					   pItem->set_attackspeed(atof(row[col++]));
					   pItem->set_h_dis(atof(row[col++]));
					   pItem->set_v_dis(atof(row[col++]));
					   pItem->set_movespeed(atof(row[col++]));
					   pItem->set_star(atoi(row[col++]));
					   pItem->set_maxintensity(atoi(row[col++]));
					   pItem->set_recoverintensityrate(atoi(row[col++]));
					   pItem->set_minintensity(atoi(row[col++]));
					   pItem->set_type_normal(atoi(row[col++]));
					   pItem->set_passiveskillid(row[col++]);
					   pItem->set_passiveskilllevel(row[col++]);
				   }
			   }
			   break;
		case 8:{//equip
				   {
					   std::string need_fields[] = {"id","pos","name_ch","name_en","quality","health","healthGrow","damage","damageGrow","pdd","pddGrow","mdd","mddGrow","hitLevel","hitGrow","dodgeLevel","dodgeGrow","criLevel","criGrow","tenacityLevel","tenacityGrow","pddPierce","pddPierceGrow","needs","needCounts","nextId","icon"};
					   std::vector<std::string> v_need_field(need_fields, need_fields + sizeof(need_fields)/sizeof(need_fields[0]));
					   ASSERT(CDBMgr::check_fields(res, v_need_field));
				   }

				   ASSERT(names.find("equip") != names.end());
				   CHECK_FALSE_RETURN_FALSE(names.find("equip") != names.end());

				   MYSQL_ROW row;
				   while (row = mysql_fetch_row(res))
				   {
					   int col = 0;
					   EQUIP * pItem = m_config.mutable_equip()->add_items();
					   ASSERT(pItem);
					   pItem->set_id(row[col++]);
					   pItem->set_pos(atoi(row[col++]));
					   pItem->set_name_ch(row[col++]);
					   pItem->set_name_en(row[col++]);
					   pItem->set_quality(atoi(row[col++]));
					   pItem->set_health(atoi(row[col++]));
					   pItem->set_healthgrow(atoi(row[col++]));
					   pItem->set_damage(atoi(row[col++]));
					   pItem->set_damagegrow(atoi(row[col++]));
					   pItem->set_pdd(atoi(row[col++]));
					   pItem->set_pddgrow(atoi(row[col++]));
					   pItem->set_mdd(atoi(row[col++]));
					   pItem->set_mddgrow(atoi(row[col++]));
					   pItem->set_hitlevel(atoi(row[col++]));
					   pItem->set_hitgrow(atoi(row[col++]));
					   pItem->set_dodgelevel(atoi(row[col++]));
					   pItem->set_dodgegrow(atoi(row[col++]));
					   pItem->set_crilevel(atoi(row[col++]));
					   pItem->set_crigrow(atoi(row[col++]));
					   pItem->set_tenacitylevel(atoi(row[col++]));
					   pItem->set_tenacitygrow(atoi(row[col++]));
					   pItem->set_pddpierce(atoi(row[col++]));
					   pItem->set_pddpiercegrow(atoi(row[col++]));
					   pItem->set_needs(row[col++]);
					   pItem->set_needcounts(row[col++]);
					   pItem->set_nextid(row[col++]);
					   pItem->set_icon(row[col++]);

				   }
			   }
			   break;
		case 9:{//aoyi

					printf("aoyi客户端配置不由服务器控制 这部分配置比较重要和难调 配置跟着客户端作版本更新!!\n");
				   //不由服务器控制 这部分配置比较重要和难调 配置跟着客户端作版本更新
					break;

					{
						std::string need_fields[] = {"id","factor"};
						std::vector<std::string> v_need_field(need_fields, need_fields + sizeof(need_fields)/sizeof(need_fields[0]));
						ASSERT(CDBMgr::check_fields(res, v_need_field));
					}

					ASSERT(names.find("aoyi") != names.end());
					CHECK_FALSE_RETURN_FALSE(names.find("aoyi") != names.end());

				   MYSQL_ROW row;
				   while (row = mysql_fetch_row(res))
				   {
					   int col = 0;
					   AOYI * pItem = m_config.mutable_aoyi()->add_items();
					   ASSERT(pItem);
					   pItem->set_id(row[col++]);
					   pItem->set_factor(atof(row[col++]));
				   }
					 
				} break;

		case 10:{//bag
				   {
					   std::string need_fields[] = {"id","useType","quality","sellPrice","exp","needs","needCounts","needGolds"};
					   std::vector<std::string> v_need_field(need_fields, need_fields + sizeof(need_fields)/sizeof(need_fields[0]));
					   ASSERT(CDBMgr::check_fields(res, v_need_field));
				   }

				   ASSERT(names.find("bag") != names.end());
				   CHECK_FALSE_RETURN_FALSE(names.find("bag") != names.end());

				   MYSQL_ROW row;
				   while (row = mysql_fetch_row(res))
				   {
					   int col = 0;
					   BAG * pItem = m_config.mutable_bag()->add_items();
					   ASSERT(pItem);
					   pItem->set_id(row[col++]);
					   pItem->set_usetype(atoi(row[col++]));
					   pItem->set_quality(atoi(row[col++]));
					   pItem->set_sellprice(atoi(row[col++]));
					   pItem->set_exp(row[col++]);
					   pItem->set_needs(row[col++]);
					   pItem->set_needcounts(row[col++]);
					   pItem->set_needgolds(atoi(row[col++]));
				   }

			   }
			   break;
		case 11:{//passive
				   {
					   std::string need_fields[] = {"id","triggerEnum","triggerId","buffId","target_Team","target_State","target_Occupation"};
					   std::vector<std::string> v_need_field(need_fields, need_fields + sizeof(need_fields)/sizeof(need_fields[0]));
					   ASSERT(CDBMgr::check_fields(res, v_need_field));
				   }

				   ASSERT(names.find("passive") != names.end());
				   CHECK_FALSE_RETURN_FALSE(names.find("passive") != names.end());

				   MYSQL_ROW row;
				   while (row = mysql_fetch_row(res))
				   {
					   int col = 0;
					   PASSIVE * pItem = m_config.mutable_passive()->add_items();
					   ASSERT(pItem);
					   pItem->set_id(row[col++]);
					   pItem->set_triggerenum(atoi(row[col++]));
					   pItem->set_triggerid(row[col++]);
					   pItem->set_buffid(row[col++]);
					   pItem->set_target_team(atoi(row[col++]));
					   pItem->set_target_state(atoi(row[col++]));
					   pItem->set_target_occupation(atoi(row[col++]));
				   }
			   }
			   break;
		case 12:{//heroskill

				   //不由服务器控制 这部分配置比较重要和难调 配置跟着客户端作版本更新
				   printf("heroskill客户端配置不由服务器控制 这部分配置比较重要和难调 配置跟着客户端作版本更新 !! \n");
				   break;

				   {
					   std::string need_fields[] = {"id","factor","distance","height","range_x","range_y","range_z","waitTime","intensity"};
					   std::vector<std::string> v_need_field(need_fields, need_fields + sizeof(need_fields)/sizeof(need_fields[0]));
					   ASSERT(CDBMgr::check_fields(res, v_need_field));
				   }

				   ASSERT(names.find("heroskill") != names.end());
				   CHECK_FALSE_RETURN_FALSE(names.find("heroskill") != names.end());

				   MYSQL_ROW row;
				   while (row = mysql_fetch_row(res))
				   {
					   int col = 0;
					   HEROSKILL * pItem = m_config.mutable_hero_skill()->add_items();
					   ASSERT(pItem);
					   pItem->set_id(row[col++]);
					   pItem->set_factor(atof(row[col++]));
					   pItem->set_distance(atof(row[col++]));
					   pItem->set_height(atof(row[col++]));
					   pItem->set_range_x(atof(row[col++]));
					   pItem->set_range_y(atof(row[col++]));
					   pItem->set_range_z(atof(row[col++]));
					   pItem->set_waittime(atof(row[col++]));
					   pItem->set_intensity(atoi(row[col++]));
				   }
			   }
			   break;
		case 13:{//bossskill

				   //不由服务器控制 这部分配置比较重要和难调 配置跟着客户端作版本更新
				   printf("bossskill客户端配置不由服务器控制 这部分配置比较重要和难调 配置跟着客户端作版本更新 !! \n");
				   break;

				   {
					   std::string need_fields[] = {"id","factor","distance","height","range_x","range_y","range_z","waitTime"};
					   std::vector<std::string> v_need_field(need_fields, need_fields + sizeof(need_fields)/sizeof(need_fields[0]));
					   ASSERT(CDBMgr::check_fields(res, v_need_field));
				   }

				   ASSERT(names.find("bossskill") != names.end());
				   CHECK_FALSE_RETURN_FALSE(names.find("bossskill") != names.end());

				   MYSQL_ROW row;
				   while (row = mysql_fetch_row(res))
				   {
					   int col = 0;
					   BOSSSKILL * pItem = m_config.mutable_boss_skill()->add_items();
					   ASSERT(pItem);
					   pItem->set_id(row[col++]);
					   pItem->set_factor(atof(row[col++]));
					   pItem->set_distance(atof(row[col++]));
					   pItem->set_height(atof(row[col++]));
					   pItem->set_range_x(atof(row[col++]));
					   pItem->set_range_y(atof(row[col++]));
					   pItem->set_range_z(atof(row[col++]));
					   pItem->set_waittime(atof(row[col++]));
				   }
			   }
			   break;
		case 14:{//buff
					 {
						 std::string need_fields[] = {"id","type","duration","durationGrow","interval","value","valueGrow","useType","lifeLine","buffType","maxLimitCount"};
						 std::vector<std::string> v_need_field(need_fields, need_fields + sizeof(need_fields)/sizeof(need_fields[0]));
						 ASSERT(CDBMgr::check_fields(res, v_need_field));
					 }

					 ASSERT(names.find("buff") != names.end());
					 CHECK_FALSE_RETURN_FALSE(names.find("buff") != names.end());
					
					 MYSQL_ROW row;
					 while (row = mysql_fetch_row(res))
					 {
							int col = 0;
							BUFF* pItem = m_config.mutable_buff()->add_items();
							ASSERT(pItem);
							pItem->set_id(row[col++]);
							pItem->set_type(atoi(row[col++]));
							pItem->set_duration(atof(row[col++]));
							pItem->set_durationgrow(atof(row[col++]));
							pItem->set_interval(atof(row[col++]));
							pItem->set_value(atoi(row[col++]));
							pItem->set_valuegrow(atoi(row[col++]));
							pItem->set_usetype(atoi(row[col++]));
							pItem->set_lifeline(atoi(row[col++]));
							pItem->set_bufftype(atoi(row[col++]));
							pItem->set_maxlimitcount(atoi(row[col++]));
					}
				}
				break;
		case 15:{//coinprice
					{
						std::string need_fields[] = {"id","needGems"};
						std::vector<std::string> v_need_field(need_fields, need_fields + sizeof(need_fields)/sizeof(need_fields[0]));
						ASSERT(CDBMgr::check_fields(res, v_need_field));
					}

					ASSERT(names.find("coinprice") != names.end());
					CHECK_FALSE_RETURN_FALSE(names.find("coinprice") != names.end());

					MYSQL_ROW row;
					while (row = mysql_fetch_row(res))
					{
						int col = 0;
						COINPRICE * pItem = m_config.mutable_coin_price()->add_items();
						ASSERT(pItem);
						pItem->set_id(row[col++]);
						pItem->set_needgems(atoi(row[col++]));
					}
				}
				break;
		case 16:{//prizeshow
					{
						std::string need_fields[] = {"id","type","counts","awards"};
						std::vector<std::string> v_need_field(need_fields, need_fields + sizeof(need_fields)/sizeof(need_fields[0]));
						ASSERT(CDBMgr::check_fields(res, v_need_field));
					}

					ASSERT(names.find("prizeshow") != names.end());
					CHECK_FALSE_RETURN_FALSE(names.find("prizeshow") != names.end());

					MYSQL_ROW row;
					while (row = mysql_fetch_row(res))
					{
						int col = 0;
						PRIZESHOW * pItem = m_config.mutable_prize_show()->add_items();
						ASSERT(pItem);
						pItem->set_id(row[col++]);
						pItem->set_type(row[col++]);
						pItem->set_counts(row[col++]);
						pItem->set_awards(row[col++]);
					}
				}
				break;
		case 17:{//realtimecheck
					{
						std::string need_fields[] = {"id","type","num"};
						std::vector<std::string> v_need_field(need_fields, need_fields + sizeof(need_fields)/sizeof(need_fields[0]));
						ASSERT(CDBMgr::check_fields(res, v_need_field));
					}

					ASSERT(names.find("realtimecheck") != names.end());
					CHECK_FALSE_RETURN_FALSE(names.find("realtimecheck") != names.end());

					MYSQL_ROW row;
					while (row = mysql_fetch_row(res))
					{
						int col = 0;
						REALTIMECHECK * pItem = m_config.mutable_realtime_check()->add_items();
						ASSERT(pItem);
						pItem->set_id(row[col++]);
						pItem->set_type(atoi(row[col++]));
						pItem->set_num(atoi(row[col++]));
					}
				}
				break;
		case 18:{//resetprice
					{
						std::string need_fields[] = {"id","needGems"};
						std::vector<std::string> v_need_field(need_fields, need_fields + sizeof(need_fields)/sizeof(need_fields[0]));
						ASSERT(CDBMgr::check_fields(res, v_need_field));
					}

					ASSERT(names.find("resetprice") != names.end());
					CHECK_FALSE_RETURN_FALSE(names.find("resetprice") != names.end());

					MYSQL_ROW row;
					while (row = mysql_fetch_row(res))
					{
						int col = 0;
						RESETPRICE * pItem = m_config.mutable_reset_price()->add_items();
						ASSERT(pItem);
						pItem->set_id(row[col++]);
						pItem->set_needgems(atoi(row[col++]));
					}
				}
				break;
		case 19:{//spprice
					{
						std::string need_fields[] = {"id","needGems"};
						std::vector<std::string> v_need_field(need_fields, need_fields + sizeof(need_fields)/sizeof(need_fields[0]));
						ASSERT(CDBMgr::check_fields(res, v_need_field));
					}

					ASSERT(names.find("spprice") != names.end());
					CHECK_FALSE_RETURN_FALSE(names.find("spprice") != names.end());

					MYSQL_ROW row;
					while (row = mysql_fetch_row(res))
					{
						int col = 0;
						SPPRICE * pItem = m_config.mutable_spprice()->add_items();
						ASSERT(pItem);
						pItem->set_id(row[col++]);
						pItem->set_needgems(atoi(row[col++]));
					}

				}
				break;
		case 20:{//staprice
					{
						std::string need_fields[] = {"id","needGems"};
						std::vector<std::string> v_need_field(need_fields, need_fields + sizeof(need_fields)/sizeof(need_fields[0]));
						ASSERT(CDBMgr::check_fields(res, v_need_field));
					}

					ASSERT(names.find("staprice") != names.end());
					CHECK_FALSE_RETURN_FALSE(names.find("staprice") != names.end());

					MYSQL_ROW row;
					while (row = mysql_fetch_row(res))
					{
						int col = 0;
						STAPRICE * pItem = m_config.mutable_staprice()->add_items();
						ASSERT(pItem);
						pItem->set_id(row[col++]);
						pItem->set_needgems(atoi(row[col++]));
					}
				}
				break;
		case 21:{//triggerattack
					{
						std::string need_fields[] = {"id","type","cd"};
						std::vector<std::string> v_need_field(need_fields, need_fields + sizeof(need_fields)/sizeof(need_fields[0]));
						ASSERT(CDBMgr::check_fields(res, v_need_field));
					}

					ASSERT(names.find("triggerattack") != names.end());
					CHECK_FALSE_RETURN_FALSE(names.find("triggerattack") != names.end());

					MYSQL_ROW row;
					while (row = mysql_fetch_row(res))
					{
						int col = 0;
						TRIGGERATTACK * pItem = m_config.mutable_trigger_attack()->add_items();
						ASSERT(pItem);
						pItem->set_id(row[col++]);
						pItem->set_type(atoi(row[col++]));
						pItem->set_cd(atof(row[col++]));
					}
				}
				break;
		case 22:{//triggerstartcondition
					{
						std::string need_fields[] = {"id","trigger_Time","trigger_Team","trigger_State","trigger_Occupation","trigger_Mark","trigger_Count"};
						std::vector<std::string> v_need_field(need_fields, need_fields + sizeof(need_fields)/sizeof(need_fields[0]));
						ASSERT(CDBMgr::check_fields(res, v_need_field));
					}

					ASSERT(names.find("triggerstartcondition") != names.end());
					CHECK_FALSE_RETURN_FALSE(names.find("triggerstartcondition") != names.end());

					MYSQL_ROW row;
					while (row = mysql_fetch_row(res))
					{
						int col = 0;
						TRIGGERSTARTCONDITION * pItem = m_config.mutable_trigger_start_condition()->add_items();
						ASSERT(pItem);
						pItem->set_id(row[col++]);
						pItem->set_trigger_time(atoi(row[col++]));
						pItem->set_trigger_team(atoi(row[col++]));
						pItem->set_trigger_state(atoi(row[col++]));
						pItem->set_trigger_occupation(atoi(row[col++]));
						pItem->set_trigger_mark(atoi(row[col++]));
						pItem->set_trigger_count(atoi(row[col++]));
					}
				}
				break;
		case 23:{//vip
					{
						std::string need_fields[] = {"id","needRMB","freeItemCount","buyPowerCount","buyFBCount","buyArenaCount","buyGoldsCount","maxSkillPoint","expBattleNum","goldBattleNum","towerNum","crusadeNum","buySkillPoint"};
						std::vector<std::string> v_need_field(need_fields, need_fields + sizeof(need_fields)/sizeof(need_fields[0]));
						ASSERT(CDBMgr::check_fields(res, v_need_field));
					}

					ASSERT(names.find("vip") != names.end());
					CHECK_FALSE_RETURN_FALSE(names.find("vip") != names.end());

					MYSQL_ROW row;
					while (row = mysql_fetch_row(res))
					{
						int col = 0;
						VIP * pItem = m_config.mutable_vip()->add_items();
						ASSERT(pItem);
						pItem->set_id(row[col++]);
						pItem->set_needrmb(atoi(row[col++]));
						pItem->set_freeitemcount(atoi(row[col++]));
						pItem->set_buypowercount(atoi(row[col++]));
						pItem->set_buyfbcount(atoi(row[col++]));
						pItem->set_buyarenacount(atoi(row[col++]));
						pItem->set_buygoldscount(atoi(row[col++]));
						pItem->set_maxskillpoint(atoi(row[col++]));
						pItem->set_expbattlenum(atoi(row[col++]));
						pItem->set_goldbattlenum(atoi(row[col++]));
						pItem->set_towernum(atoi(row[col++]));
						pItem->set_crusadenum(atoi(row[col++]));
						pItem->set_buyskillpoint(atoi(row[col++]));
					}

				}
				break;
		case 24:{//vipfunction
					{
						std::string need_fields[] = {"id","openVip"};
						std::vector<std::string> v_need_field(need_fields, need_fields + sizeof(need_fields)/sizeof(need_fields[0]));
						ASSERT(CDBMgr::check_fields(res, v_need_field));
					}

					ASSERT(names.find("vipfunction") != names.end());
					CHECK_FALSE_RETURN_FALSE(names.find("vipfunction") != names.end());

					MYSQL_ROW row;
					while (row = mysql_fetch_row(res))
					{
						int col = 0;
						VIPFUNCTION * pItem = m_config.mutable_vip_function()->add_items();
						ASSERT(pItem);
						pItem->set_id(row[col++]);
						pItem->set_openvip(atoi(row[col++]));
					}

				}
				break;
		case 25:
			{//shopresetprice
				{
					std::string need_fields[] = {"id","needGems"};
					std::vector<std::string> v_need_field(need_fields, need_fields + sizeof(need_fields)/sizeof(need_fields[0]));
					ASSERT(CDBMgr::check_fields(res, v_need_field));
				}

				ASSERT(names.find("shopresetprice") != names.end());
				CHECK_FALSE_RETURN_FALSE(names.find("shopresetprice") != names.end());

				MYSQL_ROW row;
				while (row = mysql_fetch_row(res))
				{
					int col = 0;
					SHOPRESETPRICE * pItem = m_config.mutable_shop_resetprice()->add_items();
					ASSERT(pItem);
					pItem->set_id(row[col++]);
					pItem->set_needgems(atoi(row[col++]));

				}
			}
			break;
		case 26://herostar
			{
				{
					std::string need_fields[] = {"id","needNum"};
					std::vector<std::string> v_need_field(need_fields, need_fields + sizeof(need_fields)/sizeof(need_fields[0]));
					ASSERT(CDBMgr::check_fields(res, v_need_field));

					ASSERT(names.find("herostar") != names.end());
					CHECK_FALSE_RETURN_FALSE(names.find("herostar") != names.end());

					MYSQL_ROW row;
					while (row = mysql_fetch_row(res))
					{
						int col = 0;
						HEROSTAR * pItem = m_config.mutable_hero_star()->add_items();
						ASSERT(pItem);
						pItem->set_id(row[col++]);
						pItem->set_neednum(atoi(row[col++]));
					}
				}

			}
			break;
		case 27://evolutioncoin
			{
				{
					std::string need_fields[] = {"id","equip_Weapon","equip_Necklace","equip_Clothes","equip_Belt","equip_Ring","equip_Boot"};
					std::vector<std::string> v_need_field(need_fields, need_fields + sizeof(need_fields)/sizeof(need_fields[0]));
					ASSERT(CDBMgr::check_fields(res, v_need_field));

					ASSERT(names.find("evolutioncoin") != names.end());
					CHECK_FALSE_RETURN_FALSE(names.find("evolutioncoin") != names.end());

					MYSQL_ROW row;
					while (row = mysql_fetch_row(res))
					{
						int col = 0;
						EVOLUTIONCOIN * pItem = m_config.mutable_evolution_coin()->add_items();
						ASSERT(pItem);

						pItem->set_id(row[col++]);
						pItem->set_equip_weapon(row[col++]);
						pItem->set_equip_necklace(row[col++]);
						pItem->set_equip_clothes(row[col++]);
						pItem->set_equip_belt(row[col++]);
						pItem->set_equip_ring(row[col++]);
						pItem->set_equip_boot(row[col++]);


					}
				}

			}
			break;
		case 28://qualityrule
			{
				{
					std::string need_fields[] = {"id","needLevel","needGolds","health","damage","pdd","mdd","hitLevel","dodgeLevel","criLevel","tenacityLevel","pddPierce","open"};
					std::vector<std::string> v_need_field(need_fields, need_fields + sizeof(need_fields)/sizeof(need_fields[0]));
					ASSERT(CDBMgr::check_fields(res, v_need_field));

					ASSERT(names.find("qualityrule") != names.end());
					CHECK_FALSE_RETURN_FALSE(names.find("qualityrule") != names.end());

					MYSQL_ROW row;
					while (row = mysql_fetch_row(res))
					{
						int col = 0;
						QUALITYRULE * pItem = m_config.mutable_quality_rule()->add_items();
						ASSERT(pItem);
						pItem->set_id(row[col++]);
						pItem->set_needlevel(atoi(row[col++]));
						pItem->set_needgolds(atoi(row[col++]));
						pItem->set_health(atoi(row[col++]));
						pItem->set_damage(atoi(row[col++]));
						pItem->set_pdd(atoi(row[col++]));
						pItem->set_mdd(atoi(row[col++]));
						pItem->set_hitlevel(atoi(row[col++]));
						pItem->set_dodgelevel(atoi(row[col++]));
						pItem->set_crilevel(atoi(row[col++]));
						pItem->set_tenacitylevel(atoi(row[col++]));
						pItem->set_pddpierce(atoi(row[col++]));
						pItem->set_open(atoi(row[col++]));


					}
				}

			}
			break;
		case 29://rule
			{
				{
					std::string need_fields[] = {"id","upgrade_Exp_User","upgrade_Exp_Hero","factor_Hit","factor_Dodge","factor_Cri","factor_Tenacity","upgrade_Gold_Skill","upgrade_Gold_AoYi","upgrade_Gold_Passive_1","upgrade_Gold_Passive_2","upgrade_Gold_Passive_3","upgrade_Gold_Weapon","upgrade_Gold_Necklace","upgrade_Gold_Clothes","upgrade_Gold_Belt","upgrade_Gold_Ring","upgrade_Gold_Boot"};
					std::vector<std::string> v_need_field(need_fields, need_fields + sizeof(need_fields)/sizeof(need_fields[0]));
					ASSERT(CDBMgr::check_fields(res, v_need_field));

					ASSERT(names.find("rule") != names.end());
					CHECK_FALSE_RETURN_FALSE(names.find("rule") != names.end());

					MYSQL_ROW row;
					while (row = mysql_fetch_row(res))
					{
						int col = 0;
						RULE * pItem = m_config.mutable_rule()->add_items();
						ASSERT(pItem);
						pItem->set_id(row[col++]);
						pItem->set_upgrade_exp_user(atoi(row[col++]));
						pItem->set_upgrade_exp_hero(atoi(row[col++]));
						pItem->set_factor_hit(atoi(row[col++]));
						pItem->set_factor_dodge(atoi(row[col++]));
						pItem->set_factor_cri(atoi(row[col++]));
						pItem->set_factor_tenacity(atoi(row[col++]));
						pItem->set_upgrade_gold_skill(atoi(row[col++]));
						pItem->set_upgrade_gold_aoyi(atoi(row[col++]));
						pItem->set_upgrade_gold_passive_1(atoi(row[col++]));
						pItem->set_upgrade_gold_passive_2(atoi(row[col++]));
						pItem->set_upgrade_gold_passive_3(atoi(row[col++]));
						pItem->set_upgrade_gold_weapon(atoi(row[col++]));
						pItem->set_upgrade_gold_necklace(atoi(row[col++]));
						pItem->set_upgrade_gold_clothes(atoi(row[col++]));
						pItem->set_upgrade_gold_belt(atoi(row[col++]));
						pItem->set_upgrade_gold_ring(atoi(row[col++]));
						pItem->set_upgrade_gold_boot(atoi(row[col++]));


					}
				}

			}
			break;
		}
		
		mysql_free_result(res);
	}while( !mysql_next_result( con ) );
	//<<<<

	
	//>>> 保存配置到内存
	m_config.set_error_code(e_gsc_errorcode_success);
	UINT32 body_len = m_config.ByteSize();

	UINT32 msg_id = e_msg_gs_c_config_res;
	UINT32 len = body_len + PACKET_HEAD_LEN;

	m_config_bin = new BYTE[len];
	ASSERT(m_config_bin);
	m_config_bin_len = len;
	memset(m_config_bin, 0, len);

	*((UINT32*)m_config_bin) = len;
	*(((UINT32*)m_config_bin) + 1) = msg_id;

	bool r = m_config.SerializeToArray(m_config_bin + PACKET_HEAD_LEN, m_config_bin_len - PACKET_HEAD_LEN);
	ASSERT(r);
	//<<<保存配置到内存


	//保存版本md5信息到内存
	ASSERT(m_config_version.versions_size());
	m_config_version.set_error_code(e_gsc_errorcode_success);
	body_len = m_config_version.ByteSize();

	msg_id = e_msg_gs_c_json_config_version_res;
	len = body_len + PACKET_HEAD_LEN;

	m_config_version_bin = new BYTE[len];
	ASSERT(m_config_version_bin);
	m_config_version_bin_len = len;
	memset(m_config_version_bin, 0, len);

	*((UINT32*)m_config_version_bin) = len;
	*(((UINT32*)m_config_version_bin) + 1) = msg_id;

	r = m_config_version.SerializeToArray(m_config_version_bin + PACKET_HEAD_LEN, m_config_version_bin_len - PACKET_HEAD_LEN);
	ASSERT(r);
	//<<<


	//------------------上为以pb结构和表形式 给客户端配置信息和版本控制时------------------------

	//------------------下为 以json文件 给客户端配置信息和版本控制时-----------------------------
	r = _parseMd5();
	CHECK_FALSE_RETURN_FALSE(r);
	r = _parseJsonFile();
	CHECK_FALSE_RETURN_FALSE(r);

	m_hasInited = true;
	return true;
}

bool CClientConfigMgr::_parseMd5()
{
	ASSERT(!m_hasInited);
	gs_c_json_config_version_res temp_version_res;
	temp_version_res.set_error_code(e_gsc_errorcode_success);
	//char buffer[1024];
	std::ifstream in("./client_config/MD5Keys.txt");//ansi格式的！
	if (! in.is_open()){
		printf("Error opening file");
		return false;
	}

	//while (!in.eof() ){
	//	in.getline (buffer,100);  
	//	cout << buffer << endl;  
	//}

	std::string buffer;
	while(getline(in, buffer)){
		//printf(buffer.c_str());

		std::vector<std::string> v;
		split_string(buffer, v, ":");
		ASSERT(v.size() == 2 && m_mapConfigs.find(v[0]) == m_mapConfigs.end());
		CHECK_FALSE_RETURN_FALSE(v.size() == 2 && m_mapConfigs.find(v[0]) == m_mapConfigs.end());
		CClientConfig* pCfg = new CClientConfig;
		gs_c_json_config_version_res::config_version* pItem = temp_version_res.add_versions();

		char* name = ANSIToUTF8(v[0].c_str());
		pCfg->file_name = name;
		pItem->set_name(name);
		free(name);

		char* md5 = ANSIToUTF8(v[1].c_str());
		pCfg->md5 = md5;
		pItem->set_md5(md5);
		free(md5);

		m_mapConfigs[pCfg->file_name] = pCfg;
	}

	//保存版本md5信息到内存
	ASSERT(temp_version_res.versions_size());
	UINT32 body_len = temp_version_res.ByteSize();

	UINT32 msg_id = e_msg_gs_c_json_config_version_res;
	UINT32 len = body_len + PACKET_HEAD_LEN;

	m_json_config_version_bin = new BYTE[len];
	ASSERT(m_json_config_version_bin);
	m_json_config_version_bin_len = len;
	memset(m_json_config_version_bin, 0, len);

	*((UINT32*)m_json_config_version_bin) = len;
	*(((UINT32*)m_json_config_version_bin) + 1) = msg_id;

	bool r = temp_version_res.SerializeToArray(m_json_config_version_bin + PACKET_HEAD_LEN, m_json_config_version_bin_len - PACKET_HEAD_LEN);
	ASSERT(r);

	return true;  
}

bool CClientConfigMgr::_parseJsonFile()
{
	ASSERT(!m_hasInited);
	gs_c_json_config_res temp_config_res;
	temp_config_res.set_error_code(e_gsc_errorcode_success);
	for(std::map<std::string, CClientConfig*>::iterator it = m_mapConfigs.begin() ; it != m_mapConfigs.end(); ++it){
		CClientConfig* pCfg = it->second;
		ASSERT(pCfg);
		CHECK_FALSE_RETURN_FALSE(pCfg);

		std::string file_name = "./client_config/";
		file_name += pCfg->file_name + ".txt";

		std::ifstream in(file_name.c_str());//ansi格式的！
		if (! in.is_open()){
			printf("Error opening file : %s", file_name.c_str());
			return false;
		}

		std::stringstream buffer;  
		buffer << in.rdbuf();  
		//std::string contents(buffer.str());
		//pCfg->content = buffer.str();

		char* content = ANSIToUTF8(buffer.str().c_str());
		pCfg->content = content;
		free(content);

		gs_c_json_config_res::config* pItem = temp_config_res.add_configs();
		pItem->set_name(pCfg->file_name);
		pItem->set_md5(pCfg->md5);
		pItem->set_allocated_content(&pCfg->content);//
	}

	ASSERT(temp_config_res.configs_size());
	UINT32 body_len = temp_config_res.ByteSize();

	UINT32 msg_id = e_msg_gs_c_json_config_res;
	UINT32 len = body_len + PACKET_HEAD_LEN;

	m_json_config_bin = new BYTE[len];
	ASSERT(m_json_config_bin);
	m_json_config_bin_len = len;
	memset(m_json_config_bin, 0, len);

	*((UINT32*)m_json_config_bin) = len;
	*(((UINT32*)m_json_config_bin) + 1) = msg_id;

	bool r = temp_config_res.SerializeToArray(m_json_config_bin + PACKET_HEAD_LEN, m_json_config_bin_len - PACKET_HEAD_LEN);
	ASSERT(r);

	for (::google::protobuf::RepeatedPtrField< ::gs_c_json_config_res_config >::iterator it = temp_config_res.mutable_configs()->begin(); it != temp_config_res.mutable_configs()->end(); ++it){
		::gs_c_json_config_res::config* pItem = &*it;
		pItem->release_content();
	}

	return true;  
}

bool CClientConfigMgr::handle_json_config_req( uv_stream_t* stream, const c_gs_json_config_req &req )
{
	ASSERT(m_hasInited);
	if (req.names_size() == 0){//发送完整的数据
		CServerForUser::GetInstance()->send(stream, m_json_config_bin, m_json_config_bin_len);
	}
	else{//有选择的给

		gs_c_json_config_res res;
		res.set_error_code(e_gsc_errorcode_success);
		for ( ::google::protobuf::RepeatedPtrField< ::std::string>::const_iterator it = req.names().begin(); it != req.names().end(); ++it){
			
			std::map<std::string, CClientConfig*>::iterator itMap = m_mapConfigs.find(*it);
			if (itMap == m_mapConfigs.end()){
				res.set_error_code(e_gsc_errorcode_json_config_no_some_file);
				break;//要不要break？要的话 及时终止 但是可能有些正确更新的配置文件也没有了
			}
			else{
				CClientConfig* pCfg = itMap->second;
				ASSERT(pCfg);

				::gs_c_json_config_res::config* pItem = res.add_configs();
				pItem->set_name(pCfg->file_name);
				pItem->set_md5(pCfg->md5);
				pItem->set_allocated_content(&(pCfg->content));//因为可能会比较大
			}
		}

		//先send
		CServerForUser::GetInstance()->send(stream, e_msg_gs_c_json_config_res, &res);

		//去掉set_allocated_content设置的数据 否则会释放掉CClientConfig配置的内存
		for (::google::protobuf::RepeatedPtrField< ::gs_c_json_config_res_config >::iterator it = res.mutable_configs()->begin(); it != res.mutable_configs()->end(); ++it){
			::gs_c_json_config_res::config* pItem = &*it;
			pItem->release_content();
		}
	}

	return true;
}
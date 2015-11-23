#include <string>
#include <vector>
#include "hero_mgr.h"
#include "db_mgr.h"
#include "db_sql.h"
#include "str.h"

#include "skill_mgr.h"
#include "equip_mgr.h"

#include "dbs_gs_proto.pb.h"
#include "time_helper.h"

#include "sys_setting_mgr.h"

using std::string;
using std::vector;

bool CHeroMgr::Init()
{
	MYSQL* con = CDBMgr::GetInstance()->GetConnection();
	ASSERT(con);
	char sql[1024*10] = {0};
	sprintf_s(sql, sizeof(sql),
		"SELECT * FROM `sys_hero`;"
		"SELECT * FROM `sys_hero_star_need`;"
		"SELECT * FROM `sys_hero_level_need`;"
		"SELECT * FROM `sys_hero_quality`;"
		);
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
		case 0:{//hero
				   {
					   //,"nature_property" 不需要了
					   std::string need_fields[] = {"id","hero_id","name_cn","area","occupation","star","quality","passive_skill1","passive_skill2","passive_skill3","weapon","necklace","clothes","belt","ring","boots"};
					   std::vector<std::string> v_need_field(need_fields, need_fields + sizeof(need_fields)/sizeof(need_fields[0]));
					   ASSERT(CDBMgr::check_fields(res, v_need_field));
				   }

				   MYSQL_ROW row;
				   while (row = mysql_fetch_row(res))
				   {
					   CHeroCfg* pItem = new CHeroCfg;
					   int col = 1;
					   pItem->iHeroID = atoi(row[col++]);

					   ASSERT(m_mapHeroID2Hero.find(pItem->iHeroID) == m_mapHeroID2Hero.end());

					   pItem->strName = row[col++];
					   pItem->eArea = (EArea)atoi(row[col++]);
					   //pItem->eNatureProperty = (ENatureProperty)atoi(row[col++]);
					   pItem->eOccupation = (EOccupation)atoi(row[col++]);
					   pItem->iStar = atoi(row[col++]);
					   pItem->eQuality = (EQUALITY)atoi(row[col++]);

					   pItem->arrPassiveSkill[0] = atoi(row[col++]);
					   ASSERT(pItem->arrPassiveSkill[0] && CSkillMgr::GetInstance()->GetPassiveSkillCfgByID(pItem->arrPassiveSkill[0]));//验证！

					   pItem->arrPassiveSkill[1] = atoi(row[col++]);
					   ASSERT(pItem->arrPassiveSkill[1] && CSkillMgr::GetInstance()->GetPassiveSkillCfgByID(pItem->arrPassiveSkill[1]));//验证！

					   pItem->arrPassiveSkill[2] = atoi(row[col++]);
					   ASSERT(pItem->arrPassiveSkill[2] && CSkillMgr::GetInstance()->GetPassiveSkillCfgByID(pItem->arrPassiveSkill[2]));//验证！

					   pItem->arrEquip[EEQUIPTYPE_WEAPON] = atoi(row[col++]);
					   ASSERT(CEquipMgr::GetInstance()->GetEquipCfgByEquipID(pItem->arrEquip[EEQUIPTYPE_WEAPON]));

					   pItem->arrEquip[EEQUIPTYPE_NECKLACE] = atoi(row[col++]);
					   ASSERT(CEquipMgr::GetInstance()->GetEquipCfgByEquipID(pItem->arrEquip[EEQUIPTYPE_NECKLACE]));
					 
					   pItem->arrEquip[EEQUIPTYPE_CLOTHES] = atoi(row[col++]);
					   ASSERT(CEquipMgr::GetInstance()->GetEquipCfgByEquipID(pItem->arrEquip[EEQUIPTYPE_CLOTHES]));

					   pItem->arrEquip[EEQUIPTYPE_BELT] = atoi(row[col++]);
					   ASSERT(CEquipMgr::GetInstance()->GetEquipCfgByEquipID(pItem->arrEquip[EEQUIPTYPE_BELT]));

					   pItem->arrEquip[EEQUIPTYPE_RING] = atoi(row[col++]);
					   ASSERT(CEquipMgr::GetInstance()->GetEquipCfgByEquipID(pItem->arrEquip[EEQUIPTYPE_RING]));

					   pItem->arrEquip[EEQUIPTYPE_BOOTS] = atoi(row[col++]);
					   ASSERT(CEquipMgr::GetInstance()->GetEquipCfgByEquipID(pItem->arrEquip[EEQUIPTYPE_BOOTS]));

					   m_mapHeroID2Hero[pItem->iHeroID] = pItem;
				   }

				   //if(XXX){
					  // printf("sys_hero表为空！");
					  // //return -1;//不释放的话 会导致后续的命令执行失败！
					  // break;
				   //}

				   ASSERT(!m_mapHeroID2Hero.empty());
			   }
			   break;

		case 1:{//hero fragment
				   {
					   std::string need_fields[] = {"star","fragment","to_fragment"};
					   std::vector<std::string> v_need_field(need_fields, need_fields + sizeof(need_fields)/sizeof(need_fields[0]));
					   ASSERT(CDBMgr::check_fields(res, v_need_field));
				   }

				   MYSQL_ROW row;
				   while (row = mysql_fetch_row(res))
				   {
					   //db上没有任务配置！ 并且GS上CUserTaskStrategy后面会对这里的subtaskid作有效性验证！
					   //UINT32 iSubTaskID = atoi(row[0]);
					   //if (CTaskMgr::GetInstance()->GetSubTaskBySubTaskID(iSubTaskID))
					   //{
					   stHeroStarCfg* pItem = new stHeroStarCfg;
					   ASSERT(pItem);
					   int col = 0;
					   UINT32 iStar = atoi(row[col++]);

					   ASSERT(m_mapStar2FragmentNeedNum.find(iStar) == m_mapStar2FragmentNeedNum.end());

					   pItem->iFragmentNum = atoi(row[col++]);
					   pItem->iToFragmentNum = atoi(row[col++]);
					   m_mapStar2FragmentNeedNum[iStar] = pItem;

				   }
				   ASSERT(!m_mapStar2FragmentNeedNum.empty());
			   }
			   break;

		case 2:{//sys_hero_level_need
				   {
					   std::string need_fields[] = {"level","exp_for_next_level"};
					   std::vector<std::string> v_need_field(need_fields, need_fields + sizeof(need_fields)/sizeof(need_fields[0]));
					   ASSERT(CDBMgr::check_fields(res, v_need_field));
				   }

				   MYSQL_ROW row;
				   while (row = mysql_fetch_row(res))
				   {
					   //db上没有任务配置！ 并且GS上CUserTaskStrategy后面会对这里的subtaskid作有效性验证！
					   //UINT32 iSubTaskID = atoi(row[0]);
					   //if (CTaskMgr::GetInstance()->GetSubTaskBySubTaskID(iSubTaskID))
					   //{
					   stHeroLevelNeed* pItem = new stHeroLevelNeed;
					   ASSERT(pItem);
					   int col = 0;
					   UINT32 iLevel = atoi(row[col++]);
					   ASSERT(m_mapLevel2NextLevelNeedExp.find(iLevel) == m_mapLevel2NextLevelNeedExp.end());

					   pItem->iExpForNextLevel = atoi(row[col++]);
					   m_mapLevel2NextLevelNeedExp[iLevel] = pItem;
				   }

				   ASSERT(!m_mapLevel2NextLevelNeedExp.empty());
			   }
			   break;

		case 3:{//hero quality
				   {
					   std::string need_fields[] = {"quality","need_level", "gold", "open_passive_skill"};
					   std::vector<std::string> v_need_field(need_fields, need_fields + sizeof(need_fields)/sizeof(need_fields[0]));
					   ASSERT(CDBMgr::check_fields(res, v_need_field));
				   }

				   MYSQL_ROW row;
				   bool Has = false;
				   while (row = mysql_fetch_row(res))
				   {
					   //db上没有任务配置！ 并且GS上CUserTaskStrategy后面会对这里的subtaskid作有效性验证！
					   //UINT32 iSubTaskID = atoi(row[0]);
					   //if (CTaskMgr::GetInstance()->GetSubTaskBySubTaskID(iSubTaskID))
					   //{

					   int col = 0;
					   EQUALITY eQuality = (EQUALITY)atoi(row[col++]);
					   ASSERT( eQuality > EQUALITY_MIN && eQuality < EQUALITY_MAX);

					   stHeroQualityCfg* pItem = &(m_arrQualityNeed[eQuality]);
					   //ASSERT(pItem);
					  
					   pItem->iNeedLevel = atoi(row[col++]);
					   pItem->iGold = atoi(row[col++]);
					   pItem->iPassiveSkillNum = atoi(row[col++]);
					   Has = true;
				   }

				   ASSERT(Has);
			   }
			   break;
		}

		mysql_free_result(res);
	}while( !mysql_next_result( con ) );

	return true;
}

CHeroCfg* CHeroMgr::GetHeroByHeroID( UINT32 iHeroID )
{
	std::map<UINT32, CHeroCfg*>::iterator it = m_mapHeroID2Hero.find(iHeroID);
	if (it != m_mapHeroID2Hero.end())
	{
		return it->second;
	}

	return NULL;
}

stHeroLevelNeed* CHeroMgr::GetNextLevelNeed( UINT32 iCurLevel )
{
	std::map<UINT32, stHeroLevelNeed*>::iterator it = m_mapLevel2NextLevelNeedExp.find(iCurLevel);
	if (it != m_mapLevel2NextLevelNeedExp.end())
	{
		return it->second;
	}

	return NULL;
}

stHeroStarCfg* CHeroMgr::GetStarNeed( UINT32 iToStar )
{
	std::map<UINT32, stHeroStarCfg*>::iterator it = m_mapStar2FragmentNeedNum.find(iToStar);
	if (it != m_mapStar2FragmentNeedNum.end())
	{
		return it->second;
	}

	return NULL;
}

stHeroQualityCfg* CHeroMgr::GetQualityCfg( EQUALITY eToQuality )
{
	if ( eToQuality > EQUALITY_MIN && eToQuality < EQUALITY_MAX)
	{
		return &m_arrQualityNeed[eToQuality];
	}

	return NULL;
}

bool CHeroMgr::UpdateHeroSkillByQuality( CUser* pUser, UINT32 hero_id, EQUALITY quality_from, EQUALITY quality_to )
{
	//todo

	return false;
}

bool CHeroMgr::NewHero(UINT32 hero_id, db_user_hero* db_hero_new, user_hero* c_hero_new )
{
	//ASSERT(db_hero_new && c_hero_new);
	ASSERT(db_hero_new || c_hero_new);//至少要有一个 不然进来没意义
	if(!db_hero_new && !c_hero_new){
		return false;
	}

	CHeroCfg* heroCfg = GetHeroByHeroID(hero_id);
	ASSERT(heroCfg);

	stHeroQualityCfg* heroQualityCfg = GetQualityCfg(heroCfg->eQuality);
	ASSERT(heroQualityCfg);

	UINT32 create_timestamp = Now();

	//填充dbs gs上面的结构
	if (db_hero_new)
	{
		db_hero_new->set_hero_id(heroCfg->iHeroID);
		db_hero_new->set_star(heroCfg->iStar);
		db_hero_new->set_quality(heroCfg->eQuality);
		db_hero_new->set_level(1);
		db_hero_new->set_cur_exp(0);
		db_hero_new->set_got_timestamp(create_timestamp);

		for (int i = EEQUIPTYPE_WEAPON ; i < EEQUIPTYPE_MAX ; ++i)
		{
			UINT32 equip_id = heroCfg->arrEquip[i];
			if (equip_id)
			{
				CEquipCfg* equip_cfg = CEquipMgr::GetInstance()->GetEquipCfgByEquipID(equip_id);
				ASSERT(equip_cfg);

				db_user_hero_equip* db_equip = db_hero_new->add_equips();
				db_equip->set_equip_id(equip_cfg->iEquipID);
				db_equip->set_level(1);
			}
		}

		//设置英雄技能信息
		//必杀
		db_user_hero_skill* db_skill = db_hero_new->add_skills();
		db_skill->set_type(user_hero_skill::BISHA);
		//db_skill->set_skill_id(0);
		db_skill->set_level(1);

		//奥义
		db_skill = db_hero_new->add_skills();
		db_skill->set_type(user_hero_skill::AOYI);
		//db_skill->set_skill_id(0);
		db_skill->set_level(1);

		//被动技能
		for (int i = 0 ; i < MAXNUM_HEROPASSIVESKILL ; ++i)
		{
			UINT32 passive_skill_id = heroCfg->arrPassiveSkill[i];
			if (passive_skill_id)
			{
				CPassiveSkillCfg* cfg = CSkillMgr::GetInstance()->GetPassiveSkillCfgByID(passive_skill_id);
				ASSERT(cfg);
				db_skill = db_hero_new->add_skills();
				
				if ( i < heroQualityCfg->iPassiveSkillNum )//不同品质 初始技能开放不一样！
				{
					db_skill->set_level(1);
				}
				else{
					db_skill->set_level(0);
				}

				user_hero_skill::e_skill_type skill_type;
				switch (i)
				{
				case 0:{
					skill_type = user_hero_skill::PASSIVE1;
					   }break;
				case 1:{
					skill_type = user_hero_skill::PASSIVE2;
					   }break;
				case 2:{
					skill_type = user_hero_skill::PASSIVE3;
					   }break;
				default:
					ASSERT(false);
				}
				db_skill->set_type(skill_type);
			}
		}
	}
	
	//填充c的结构
	if (c_hero_new)
	{
		c_hero_new->set_hero_id(heroCfg->iHeroID);
		c_hero_new->set_star(heroCfg->iStar);
		c_hero_new->set_quality(heroCfg->eQuality);
		c_hero_new->set_level(1);
		c_hero_new->set_cur_exp(0);
		c_hero_new->set_got_timestamp(create_timestamp);

		for (int i = EEQUIPTYPE_WEAPON ; i < EEQUIPTYPE_MAX ; ++i)
		{
			UINT32 equip_id = heroCfg->arrEquip[i];
			if (equip_id)
			{
				CEquipCfg* equip_cfg = CEquipMgr::GetInstance()->GetEquipCfgByEquipID(equip_id);
				ASSERT(equip_cfg);
				user_hero_equip* c_equip = c_hero_new->add_equips();
				c_equip->set_equip_id(equip_cfg->iEquipID);
				c_equip->set_level(1);
			}
		}

		user_hero_skill* c_skill = c_hero_new->add_skills();
		c_skill->set_type(user_hero_skill::BISHA);
		//c_skill->set_skill_id(0);
		c_skill->set_level(1);

		c_skill = c_hero_new->add_skills();
		c_skill->set_type(user_hero_skill::AOYI);
		//c_skill->set_skill_id(0);
		c_skill->set_level(1);

		//被动技能
		for (int i = 0 ; i < MAXNUM_HEROPASSIVESKILL ; ++i)
		{
			UINT32 passive_skill_id = heroCfg->arrPassiveSkill[i];
			if (passive_skill_id)
			{
				CPassiveSkillCfg* cfg = CSkillMgr::GetInstance()->GetPassiveSkillCfgByID(passive_skill_id);
				ASSERT(cfg);
				c_skill = c_hero_new->add_skills();
				if ( i < heroQualityCfg->iPassiveSkillNum )//不同品质 初始技能开放不一样！
				{
					c_skill->set_level(1);
				}
				else{
					c_skill->set_level(0);
				}

				user_hero_skill::e_skill_type skill_type;
				switch (i)
				{
				case 0:{
					skill_type = user_hero_skill::PASSIVE1;
					   }break;
				case 1:{
					skill_type = user_hero_skill::PASSIVE2;
					   }break;
				case 2:{
					skill_type = user_hero_skill::PASSIVE3;
					   }break;
				default:
					ASSERT(false);
				}
				c_skill->set_type(skill_type);
			}
		}
	}

	return true;
}

UINT32 CHeroMgr::GetHeroMaxLevel()
{
	return CSysSettingMgr::GetInstance()->Max_Num_User_Level;
}

UINT32 CHeroMgr::GetHeroMaxQuality()
{
	return CSysSettingMgr::GetInstance()->Max_Num_Hero_Quality;
}

UINT32 CHeroMgr::GetHeroMaxStar()
{
	return CSysSettingMgr::GetInstance()->Max_Num_Hero_Star;
}

bool CHeroMgr::IsLevelFull( UINT32 iCurLevel, UINT32 iLevelLimit )
{
	if (iCurLevel >= iLevelLimit || iCurLevel >= GetHeroMaxLevel()){
		return true;
	}

	return false;
}

bool CHeroMgr::IsExpFull( UINT32 iCurLevel, UINT32 iCurExp, UINT32 iLevelLimit )
{
	if (!IsLevelFull(iCurLevel, iLevelLimit)){
		return false;
	}

	//判断本级是否经验已满
	stHeroLevelNeed* need = GetNextLevelNeed(iCurLevel);
	ASSERT(need);
	if (need && need->iExpForNextLevel > iCurExp){
		return false;
	}

	//找不到的配置的时候 为了安全 认为已满
	return true;
}

gs_dbs_user_info_op_req::struct_op_hero_base* CHeroMgr::GetHeroInDBReq( ::google::protobuf::RepeatedPtrField< ::gs_dbs_user_info_op_req::struct_op_hero_base >& db_heros, UINT32 iHeroID, bool create_if_not_exist/* = true*/ )
{
	gs_dbs_user_info_op_req::struct_op_hero_base* ret_hero = NULL;
	for (::google::protobuf::RepeatedPtrField< ::gs_dbs_user_info_op_req_struct_op_hero_base >::iterator it = db_heros.begin() ; it != db_heros.end(); ++it ){
		gs_dbs_user_info_op_req::struct_op_hero_base& hero = *it;
		if (hero.hero_id() == iHeroID){
			ret_hero = &hero;
			return ret_hero;
		}
	}

	if (create_if_not_exist && !ret_hero){
		ret_hero = db_heros.Add();
	}

	return ret_hero;
}
//UINT32 CHeroMgr::GetHeroEquipID( UINT32 iHeroID, EEQUIPTYPE eEquipType )
//{
//	ASSERT(eEquipType > EEQUIPTYPE_MIN && eEquipType < EEQUIPTYPE_MAX);
//	CHeroCfg* hero = GetHeroByHeroID(iHeroID);
//	ASSERT(hero);
//	if (hero)
//	{
//		return hero->arrEquip[eEquipType];
//	}
//
//	return NULL;
//}
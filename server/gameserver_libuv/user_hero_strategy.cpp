#include "user_hero_strategy.h"
#include "dbs_gs_proto.pb.h"

#include "def.h"
#include "hero_mgr.h"
#include "equip_mgr.h"
#include "item_mgr.h"

bool CUserHeroStrategy::Init( db_user_hero_array* hero_info )
{
	ASSERT(hero_info);

	::google::protobuf::RepeatedPtrField< ::db_user_hero >::iterator it = hero_info->mutable_items()->begin();
	for ( ;it != hero_info->mutable_items()->end() ; ++it)
	{
		ASSERT(mMapHeroID2Hero.find(it->hero_id()) == mMapHeroID2Hero.end());
		mMapHeroID2Hero[it->hero_id()] = &(*it);
	}

	return true;
}

bool CUserHeroStrategy::Add( db_user_hero* hero )
{
	ASSERT(hero && hero->skills_size() && hero->equips_size());

	ASSERT(mMapHeroID2Hero.find(hero->hero_id()) == mMapHeroID2Hero.end());

	mMapHeroID2Hero[hero->hero_id()] = hero;

	//::google::protobuf::RepeatedPtrField< ::db_user_hero_equip >::iterator it = hero->mutable_equips()->begin();
	//for ( ; it != hero->mutable_equips()->end(); ++it)
	//{
	//	mMap2Equip[std::make_pair<UINT32,UINT32>(hero->hero_id(), it->equip_id())] = &(*it);
	//}

	//::google::protobuf::RepeatedPtrField< ::db_user_hero_skill >::iterator it = hero->mutable_skills()->begin();
	//for ( ; it != hero->mutable_skills()->end(); ++it)
	//{
	//	if(it->type() == db_user_hero_skill::PASSIVE){
	//		mMap2Equip[std::make_pair<UINT32,UINT32>(hero->hero_id(), it->equip_id())] = &(*it);
	//	}
	//	else{//aoyi 和 

	//	}
	//	
	//}

	return true;
}

db_user_hero_equip* CUserHeroStrategy::GetEquip( UINT32 hero_id, UINT32 equip_id )
{
	db_user_hero* hero = Get(hero_id);
	if (hero)
	{
		::google::protobuf::RepeatedPtrField< ::db_user_hero_equip >::iterator it = hero->mutable_equips()->begin();
		for ( ; it != hero->mutable_equips()->end() ; ++it)
		{
			if (it->equip_id() == equip_id)
			{
				return &(*it);
			}
		}
	}

	return NULL;
}

db_user_hero_equip* CUserHeroStrategy::GetEquip( UINT32 hero_id, EEQUIPTYPE equip_type )
{
	ASSERT(equip_type > EEQUIPTYPE_MIN && equip_type < EEQUIPTYPE_MAX);

	db_user_hero* hero = Get(hero_id);
	if (hero)
	{
		::google::protobuf::RepeatedPtrField< ::db_user_hero_equip >::iterator it = hero->mutable_equips()->begin();
		for ( ; it != hero->mutable_equips()->end() ; ++it)
		{
			CEquipCfg* equip_cfg = CEquipMgr::GetInstance()->GetEquipCfgByEquipID(it->equip_id());
			if ( equip_cfg->eEquipType == equip_type)
			{
				return &(*it);
			}
		}
	}

	return NULL;
}

::google::protobuf::RepeatedPtrField< ::db_user_hero_equip >* CUserHeroStrategy::GetEquips( UINT32 hero_id )
{
	db_user_hero* hero = Get(hero_id);
	//ASSERT(hero);
	if(!hero){
		return NULL;
	}
	return hero->mutable_equips();
}

db_user_hero_skill* CUserHeroStrategy::GetSkill( UINT32 hero_id, UINT32 skill_type )
{
	if ( ! user_hero_skill::e_skill_type_IsValid(skill_type))
	{
		return NULL;
	}
	user_hero_skill::e_skill_type type = (user_hero_skill::e_skill_type)skill_type;
	db_user_hero* hero = Get(hero_id);
	if (hero)
	{
		::google::protobuf::RepeatedPtrField< ::db_user_hero_skill >::iterator it = hero->mutable_skills()->begin();
		for ( ; it != hero->mutable_skills()->end() ; ++it)
		{
			if ( it->type() == type)
			{
				return &(*it);
			}
		}
	}

	return NULL;
}

UINT32 CUserHeroStrategy::IncreaseExp( const UINT32 hero_id, const UINT32 add_exp, const UINT32 level_limit, UINT32& up_level_num, UINT32& new_cur_exp )
{
	db_user_hero* hero = Get(hero_id);
	if (!hero)
	{
		return false;
	}

	//
	up_level_num = 0;
	UINT32 last_exp =  hero->cur_exp();
	UINT32 temp_cur_exp = last_exp;
	UINT32 actual_add_exp = 0;

	new_cur_exp = last_exp + add_exp;
	UINT32 cur_level = hero->level();
	ASSERT(cur_level <= level_limit);//状态判断
	stHeroLevelNeed* cfg = CHeroMgr::GetInstance()->GetNextLevelNeed(cur_level);
	ASSERT(cfg);
	ASSERT(last_exp <= cfg->iExpForNextLevel);
	while ( cur_level < level_limit && cfg && cfg->iExpForNextLevel)
	{
		if (new_cur_exp < cfg->iExpForNextLevel)
		{
			break;
		}

		if ( temp_cur_exp != 0)
		{
			actual_add_exp += cfg->iExpForNextLevel - temp_cur_exp;
			temp_cur_exp = 0;
		}
		else{
			actual_add_exp += cfg->iExpForNextLevel;
		}

		++up_level_num;
		new_cur_exp -= cfg->iExpForNextLevel;
		cfg = CHeroMgr::GetInstance()->GetNextLevelNeed(++cur_level);
	}

	ASSERT(cur_level <= level_limit);

	UINT32 actual_cur_exp = min(new_cur_exp, cfg->iExpForNextLevel);//
	ASSERT(actual_cur_exp >= temp_cur_exp);
	actual_add_exp += actual_cur_exp - temp_cur_exp;
	new_cur_exp = actual_cur_exp;

	if (up_level_num){//升级了 

		hero->set_level(cur_level);

		//if(last_exp != new_cur_exp){//大多数情况肯定是变了的 为了减少已经几率很小的赋值多一个判断反而低效
		hero->set_cur_exp(new_cur_exp);
		//}
	}
	else{//不能升级 经验不够 或者到限制了
		ASSERT(cfg);//此时这个cfg是 <=所能到达的那个等级 的cfg
		if (last_exp >= cfg->iExpForNextLevel)//起始经验肯定不会超过配置的！此时 如果英雄经验已经满了 满了没升级 说明此时 英雄等级已经到了玩家等级到限制了
		{
			ASSERT(last_exp == cfg->iExpForNextLevel);
		}
		else{//上次经验没满 
			hero->set_cur_exp(new_cur_exp);//实际上可以直接设置这个语句 不需要上面的判断！
		}
	}

	return actual_add_exp;
}

UINT32 CUserHeroStrategy::IncreaseExp( const UINT32 hero_id, const UINT32 add_exp, const UINT32 level_limit, ::google::protobuf::RepeatedPtrField< ::gs_dbs_user_info_op_req_struct_op_hero_base >& db_heros, ::google::protobuf::RepeatedPtrField< ::struct_game_element >& c_eles )
{
	db_user_hero* hero = Get(hero_id);
	ASSERT(hero);
	UINT32 level_up_num = 0;
	UINT32 new_cur_exp = 0;
	UINT32 last_exp = hero->cur_exp();
	UINT32 actual_add_exp = IncreaseExp(hero->hero_id(), add_exp, level_limit, level_up_num, new_cur_exp);

	if ( actual_add_exp == 0)
	{
		return actual_add_exp;
	}

	//有改变！
	//虽然可能只升级 但是经验不改变 但是这种情况很少 不作额外的判断反而更高效 直接发新经验值给客户端

	//操作db 经验 有改变才改
	gs_dbs_user_info_op_req::struct_op_hero_base* db_hero_base = NULL;
	if (last_exp != new_cur_exp)
	{
		//db_hero_base = db_heros.Add();
		db_hero_base = CHeroMgr::GetInstance()->GetHeroInDBReq(db_heros, hero->hero_id());
		ASSERT(db_hero_base);
		db_hero_base->set_op_type(gs_dbs_user_info_op_req::Update);
		db_hero_base->set_hero_id(hero->hero_id());
		db_hero_base->set_cur_exp(hero->cur_exp());
	}

	//给c的 经验 当前经验槽有没有改变都给 因为增量要在元素的sub_id
	//struct_game_element* c_ele = c_eles.Add();
	//c_ele->set_type(struct_game_element::EELEMENTTYPE_HERO_EXP);
	//c_ele->set_id(hero->hero_id());
	//c_ele->set_sub_id(actual_add_exp);
	//c_ele->set_num_type(struct_game_element::TOTAL);
	//c_ele->set_num(hero->cur_exp());

	//复用同一个struct_game_element对象
	struct_game_element* c_ele = CItemMgr::GetInstance()->GetElementInCRes(c_eles, struct_game_element::EELEMENTTYPE_HERO_EXP, hero->hero_id(), true);
	ASSERT(c_ele);
	c_ele->set_type(struct_game_element::EELEMENTTYPE_HERO_EXP);
	c_ele->set_id(hero->hero_id());
	c_ele->set_sub_id( c_ele->sub_id() + actual_add_exp);//累加获取的经验值
	c_ele->set_num_type(struct_game_element::TOTAL);
	c_ele->set_num(hero->cur_exp());

	if (level_up_num)
	{
		//给db 给cs
		if (db_hero_base == NULL)
		{
			//db_hero_base = db_heros.Add();
			db_hero_base = CHeroMgr::GetInstance()->GetHeroInDBReq(db_heros, hero->hero_id());
			ASSERT(db_hero_base);
			db_hero_base->set_op_type(gs_dbs_user_info_op_req::Update);
			db_hero_base->set_hero_id(hero->hero_id());
		}

		db_hero_base->set_level(hero->level());

		//struct_game_element* c_ele = c_eles.Add();
		struct_game_element* c_ele = CItemMgr::GetInstance()->GetElementInCRes(c_eles, struct_game_element::EELEMENTTYPE_HERO_LEVEL, hero->hero_id(), true);
		ASSERT(c_ele);
		c_ele->set_type(struct_game_element::EELEMENTTYPE_HERO_LEVEL);
		c_ele->set_id(hero->hero_id());
		c_ele->set_sub_id(c_ele->sub_id() + level_up_num);//升级增量
		c_ele->set_num_type(struct_game_element::TOTAL);
		c_ele->set_num(hero->level());
	}

	return actual_add_exp;
}

UINT32 CUserHeroStrategy::GetHeroNum()
{
	return mMapHeroID2Hero.size();
}

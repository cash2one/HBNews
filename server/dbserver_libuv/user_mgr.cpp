#include "user_mgr.h"
#include "time_helper.h"

db_user_hero* CUserMgr::GetHero( UINT32 iUserID, UINT32 iHeroID )
{
	db_user_info* hero = GetUserInfo(iUserID);
	ASSERT(hero);
	google::protobuf::RepeatedPtrField< ::db_user_hero >::iterator it = hero->mutable_heros()->mutable_items()->begin();
	for ( ; it != hero->mutable_heros()->mutable_items()->end(); ++it)
	{
		if (it->hero_id() == iHeroID)
		{
			return &(*it);
		}
	}

	return NULL;
}

db_user_hero_equip* CUserMgr::GetEquip( UINT32 iUserID, UINT32 iHeroID, UINT32 iEquipID )
{
	db_user_hero* hero = GetHero(iUserID, iHeroID);
	google::protobuf::RepeatedPtrField< ::db_user_hero_equip >::iterator it = hero->mutable_equips()->begin();
	for ( ; it != hero->mutable_equips()->end() ; ++it)
	{
		if ( iEquipID == it->equip_id())
		{
			return &(*it);
		}
	}

	return NULL;
}

db_user_hero_skill* CUserMgr::GetSkill( UINT32 iUserID, UINT32 iHeroID, user_hero_skill_e_skill_type eSkillType )
{
	db_user_hero* hero = GetHero(iUserID, iHeroID);
	google::protobuf::RepeatedPtrField< ::db_user_hero_skill >::iterator it = hero->mutable_skills()->begin();
	for ( ; it != hero->mutable_skills()->end() ; ++it)
	{
		if ( eSkillType == it->type())
		{
			return &(*it);
		}
	}

	return NULL;
}

struct_game_element* CUserMgr::GetItem(UINT32 iUserID, UINT32 iItemID)
{
	db_user_info* user = GetUserInfo(iUserID);
	ASSERT(user);
	::google::protobuf::RepeatedPtrField< ::struct_game_element >::iterator iter = user->mutable_items()->mutable_items()->begin();
	for (; iter != user->mutable_items()->mutable_items()->end(); ++iter)
	{
		if (iter->id() == iItemID)
		{
			return &*iter;
		}
	}
	return NULL;
}


db_user_mail*       CUserMgr::GetMail(UINT32 iUserID, UINT32 iMailID)
{
	db_user_info* user = GetUserInfo(iUserID);
	ASSERT(user);
	::google::protobuf::RepeatedPtrField< ::db_user_mail >::iterator iter = user->mutable_mails_received()->mutable_mails()->begin();
	for (; iter != user->mutable_mails_received()->mutable_mails()->end(); ++iter)
	{
		if (iter->mail_id() == iMailID)
		{
			return &*iter;
		}
	}
	return NULL;
}

db_user_mission_event* CUserMgr::GetMissionEvent(UINT32 iUserID, db_user_mission_event::MISSION_EVENT_TYPE event_type, UINT32 event_sub_type)
{
	db_user_info* user = GetUserInfo(iUserID);
	ASSERT(user);
	::google::protobuf::RepeatedPtrField< ::db_user_mission_event >::iterator iter = user->mutable_mission_events()->mutable_events()->begin();
	for (; iter != user->mutable_mission_events()->mutable_events()->end(); ++iter)
	{
		if (iter->mission_event_type() == event_type && iter->mission_event_sub_type() == event_sub_type)
		{
			return &*iter; 
		}
	}
	return NULL;
}

db_user_mission_chain* CUserMgr::GetMissionChain(UINT32 iUserID, UINT32 iChainID)
{
	db_user_info* user = GetUserInfo(iUserID);
	ASSERT(user);
	::google::protobuf::RepeatedPtrField< ::db_user_mission_chain >::iterator iter = user->mutable_mission_chains()->mutable_chains()->begin();
	for (; iter != user->mutable_mission_chains()->mutable_chains()->end(); ++iter)
	{
		if (iter->mission_chain_id() == iChainID)
		{
			return &*iter; 
		}
	}
	return NULL;
}


bool CUserMgr::_op_base_info( UINT32 iUserID, gs_dbs_user_info_op_req_struct_op_base_info* op )
{
	ASSERT(op);
	db_user_info* user_info = GetUserInfo(iUserID);
	ASSERT(user_info);
	if (op->op_type() == gs_dbs_user_info_op_req_EOPType_Update)
	{
		ASSERT(user_info->has_user_base_info());
		db_user_base_info*  old_data = user_info->mutable_user_base_info();
		if (op->has_name())
		{
			old_data->set_name(op->name());
		}

		if (op->has_level())
		{
			old_data->set_level(op->level());
		}

		if (op->has_exp())
		{
			old_data->set_exp(op->exp());
		}

		if (op->has_vip())
		{
			old_data->set_vip(op->vip());
		}

		if (op->has_diamond())
		{
			old_data->set_diamond(op->diamond());
		}

		if (op->has_gold())
		{
			old_data->set_gold(op->gold());
		}
		
		if (op->has_head_id())
		{
			old_data->set_head_id(op->head_id());
		}

		if (op->has_head_frame_id())
		{
			old_data->set_head_frame_id(op->head_frame_id());
		}

		if (op->has_health_cur())
		{
			old_data->set_health_cur(op->health_cur());
		}

		if (op->has_health_last_update_timestamp())
		{
			old_data->set_health_last_update_timestamp(op->health_last_update_timestamp());
		}

		if (op->has_skill_point())
		{
			old_data->set_skill_point(op->skill_point());
		}

		if (op->has_skill_point_update_timestamp())
		{
			old_data->set_skill_point_last_update_timestamp(op->skill_point_update_timestamp());
		}

		if (op->has_invoke_gold_free_left_num())//上一次金币免费召唤当天剩余使用次数
		{
			old_data->set_invoke_gold_free_left_num(op->invoke_gold_free_left_num());
		}

		if (op->has_invoke_gold_free_update_timestamp())//上一次金币免费召唤 计算cd刷新的时间 ： 初始化的时候和满到不满的这个时间
		{
			old_data->set_invoke_gold_free_update_timestamp(op->invoke_gold_free_update_timestamp());
		} 

		if (op->has_invoke_gold_n_total_num())//总共金币十连抽召唤的次数
		{
			old_data->set_invoke_gold_n_total_num(op->invoke_gold_n_total_num());
		}

		if (op->has_invoke_diamond_free_left_num())//上一次钻石免费召唤当天剩余使用次数
		{
			old_data->set_invoke_diamond_free_left_num(op->invoke_diamond_free_left_num());
		}

		if (op->has_invoke_diamond_free_update_timestamp())//上一次钻石免费召唤 计算cd刷新的时间 ： 初始化的时候和满到不满的这个时间
		{
			old_data->set_invoke_diamond_free_update_timestamp(op->invoke_diamond_free_update_timestamp());
		}

		if (op->has_invoke_diamond_n_total_num())//总共钻石十连抽召唤的次数
		{
			old_data->set_invoke_diamond_n_total_num(op->invoke_diamond_n_total_num());
		}

		if (op->has_friend_point())
		{
			old_data->set_friend_point(op->friend_point());
		}

		if (op->has_signature())
		{
			old_data->set_signature(op->signature());
		}

		if (op->has_crusade_point())
		{
			old_data->set_crusade_point(op->crusade_point());
		}
	}
	else{
		//暂时没有这种需求
		ASSERT(false);
		return false;
	}

	return true;
}

bool CUserMgr::_op_sub_task( UINT32 iUserID, gs_dbs_user_info_op_req_struct_op_sub_task* op_sub_task )
{
	ASSERT(op_sub_task);
	db_user_info* user_info = GetUserInfo(iUserID);
	ASSERT(user_info);
	if (op_sub_task->op_type() == gs_dbs_user_info_op_req_EOPType_Add_Or_Update)
	{
		db_user_sub_task*  old_sub_task = NULL;
		if (user_info->has_sub_tasks())//不判断的话 直接使用会出错
		{
			::google::protobuf::RepeatedPtrField< ::db_user_sub_task >::iterator it = user_info->mutable_sub_tasks()->mutable_items()->begin();
			for (; it != user_info->mutable_sub_tasks()->mutable_items()->end(); ++it)
			{
				if (it->sub_task_id() == op_sub_task->sub_task_id())
				{
					old_sub_task = &(*it);
					break;
				}
			}
		}

		//已经存在 则更新子任务完成信息！
		if(old_sub_task){
			if (op_sub_task->has_star()){
				old_sub_task->set_star(op_sub_task->star());
			}

			if (op_sub_task->has_last_day_enter_num()){
				old_sub_task->set_last_day_enter_num(op_sub_task->last_day_enter_num());
			}

			if (op_sub_task->has_last_enter_timestamp()){
				old_sub_task->set_last_enter_timestamp(op_sub_task->last_enter_timestamp());
			}

			if (op_sub_task->has_total_enter_num()){
				old_sub_task->set_total_enter_num(op_sub_task->total_enter_num());
			}

			if (op_sub_task->has_last_reset_time()){
				old_sub_task->set_last_reset_time(op_sub_task->last_reset_time());
			}

			if (op_sub_task->has_last_day_reset_num()){
				old_sub_task->set_last_day_reset_num(op_sub_task->last_day_reset_num());
			}
		}
		else{//不存在 则添加子任务完成信息！
			
			db_user_sub_task* new_sub_task = user_info->mutable_sub_tasks()->add_items();
			new_sub_task->set_sub_task_id(op_sub_task->sub_task_id());
			new_sub_task->set_star(op_sub_task->star());
			new_sub_task->set_last_day_enter_num(op_sub_task->last_day_enter_num());
			new_sub_task->set_last_enter_timestamp(op_sub_task->last_enter_timestamp());
			new_sub_task->set_total_enter_num(op_sub_task->total_enter_num());
			new_sub_task->set_last_reset_time(op_sub_task->last_reset_time());
			new_sub_task->set_last_day_reset_num(op_sub_task->last_day_reset_num());
		}
	}
	else if (op_sub_task->op_type() == gs_dbs_user_info_op_req_EOPType_Delete)
	{
		//暂时没有这种需求
		ASSERT(false);
		return false;
	}
	else{
		ASSERT(false);
		return false;
	}

	return true;
}

bool CUserMgr::_op_item(UINT32 iUserID, const struct_game_element& op_game_ele)
{
	db_user_info* user_info = GetUserInfo(iUserID);
	ASSERT(user_info);
	if(struct_game_element::INCREASE == op_game_ele.num_type())
	{
		struct_game_element* pEle = GetItem(iUserID, op_game_ele.id());
		if (pEle)
		{
			UINT32 iNum = pEle->num() + op_game_ele.num();
			pEle->set_num(iNum);
		}
		else
		{
			struct_game_element* pNewEle = user_info->mutable_items()->add_items();
			pNewEle->CopyFrom(op_game_ele);
		}
	}
	else if(struct_game_element::DECREASE == op_game_ele.num_type())
	{
		struct_game_element* pEle = GetItem(iUserID, op_game_ele.id());
		if (pEle)
		{
			UINT32 iNum = pEle->num() - op_game_ele.num();
			ASSERT(iNum >= 0);
			if (iNum > 0)
			{
				pEle->set_num(iNum);
			}
			else
			{//删除pb中道具
				bool has_find = false;
				UINT32 iSize = user_info->mutable_items()->mutable_items()->size();
				UINT32 i = 0;
				for (; i < iSize; ++i)
				{
					if (user_info->mutable_items()->mutable_items(i)->id() == op_game_ele.id())
					{
						has_find = true;
						break;
					}
				}

				if (has_find)
				{
					user_info->mutable_items()->mutable_items()->SwapElements(i, iSize - 1);
					user_info->mutable_items()->mutable_items()->RemoveLast();
				}
				else
				{
					printf("[%s][%u]error! 不支持的操作！", __FUNCTION__, __LINE__);
				}
			}
		}
		else
		{
			printf("[%s][%u]error! 不支持的操作！", __FUNCTION__, __LINE__);
		}

	}
	else if(struct_game_element::TOTAL == op_game_ele.num_type())
	{
		struct_game_element* pEle = GetItem(iUserID, op_game_ele.id());
		if (!pEle)
		{
			struct_game_element* pNewEle = user_info->mutable_items()->add_items();
			pNewEle->CopyFrom(op_game_ele);
		}
		else
		{
			pEle->set_num(op_game_ele.num());
		}
	}
	else
	{
		printf("[%s][%u]error! 不支持的操作！", __FUNCTION__, __LINE__);
		return false;
	}
	return true;
}

bool CUserMgr::_op_mail(UINT32 iUserID, const gs_dbs_user_info_op_req_struct_op_mail& op_mail)
{
	db_user_info* user_info = GetUserInfo(iUserID);
	ASSERT(user_info);

	if (gs_dbs_user_info_op_req::Add == op_mail.op_type())
	{
		db_user_mail* pMail = user_info->mutable_mails_received()->add_mails();
		pMail->CopyFrom(op_mail.mail());
	}
	else if(gs_dbs_user_info_op_req::Update == op_mail.op_type())
	{
		db_user_mail* pMail = GetMail(iUserID, op_mail.mail_id());
		if (pMail)
		{
			pMail->set_status(MAIL_STATUS_READED);
		}
		else
		{
			printf("[%s][%u]error! 不支持的操作！", __FUNCTION__, __LINE__);
		}
	}
	else if(gs_dbs_user_info_op_req::Delete == op_mail.op_type())
	{
		UINT32 iSize = user_info->mutable_mails_received()->mutable_mails()->size();
		for (size_t i = 0; i < iSize; ++i)
		{
			if (user_info->mutable_mails_received()->mutable_mails(i)->mail_id() == op_mail.mail_id())
			{
				user_info->mutable_mails_received()->mutable_mails()->SwapElements(i, iSize - 1);
				user_info->mutable_mails_received()->mutable_mails()->RemoveLast();
				break;
			}
		}
	}
	else
	{
		printf("[%s][%u]error! 不支持的操作！", __FUNCTION__, __LINE__);
		return false;
	}
	return true;
}

bool CUserMgr::_op_mission(UINT32 iUserID, const gs_dbs_user_info_op_req_struct_op_mission& op_mission){
	db_user_info* user_info = GetUserInfo(iUserID);
	ASSERT(user_info);
	ASSERT(op_mission.has_mission_event() || op_mission.has_mission_chain());
	if (!op_mission.has_mission_event() && !op_mission.has_mission_chain())
	{
		printf("任务更新操作信息为空");
		return false;
	}
	
	if(op_mission.has_mission_event())
	{
		db_user_mission_event* mission_event = GetMissionEvent(iUserID, op_mission.mission_event().mission_event_type(),  op_mission.mission_event().mission_event_sub_type());
		if (mission_event)
		{
			if (mission_event->has_last_update_timestamp())
			{
				mission_event->set_last_update_timestamp(op_mission.mission_event().last_update_timestamp());
			}
			if (mission_event->has_last_day_value())
			{
				mission_event->set_last_day_value(op_mission.mission_event().last_day_value());
			}
			if (mission_event->has_total_value())
			{
				mission_event->set_total_value(op_mission.mission_event().total_value());
			}
		}
		else
		{
			mission_event = user_info->mutable_mission_events()->add_events();
			mission_event->CopyFrom(op_mission.mission_event());
		}
	}
	
	if (op_mission.has_mission_chain())
	{
		db_user_mission_chain* mission_chain = GetMissionChain(iUserID, op_mission.mission_chain().mission_chain_id());
		if (mission_chain)
		{
			mission_chain->set_last_get_reward_mission_id(op_mission.mission_chain().last_get_reward_mission_id());
			mission_chain->set_last_get_reward_timestamp(op_mission.mission_chain().last_get_reward_timestamp());
		}
		else
		{
			mission_chain = user_info->mutable_mission_chains()->add_chains();
			mission_chain->CopyFrom(op_mission.mission_chain());
		}
	}
	return true;
}

bool CUserMgr::_op_sign(UINT32 iUserID, const gs_dbs_user_info_op_req_struct_op_sign& op_sign)
{
	db_user_info* user_info = GetUserInfo(iUserID);
	CHECK_NULL_RETURN_FALSE(user_info);

	if (op_sign.has_sign_1_times())
	{
		user_info->mutable_sign()->set_sign_1_times(op_sign.sign_1_times());
	}
	
	if (op_sign.has_sign_n_reward_status())
	{
		user_info->mutable_sign()->set_sign_n_reward_status(op_sign.sign_n_reward_status());
	}

	if (op_sign.has_last_sign_time())
	{
		user_info->mutable_sign()->set_last_sign_time(op_sign.last_sign_time());
	}

	if (op_sign.has_add_signed_times())
	{
		user_info->mutable_sign()->set_add_signed_times(op_sign.add_signed_times());
	}
	return true;
}

bool CUserMgr::_op_novice_guide(UINT32 iUserID, const gs_dbs_user_info_op_req::struct_op_novice_guide& novice_guide)
{
	db_user_info* user_info = GetUserInfo(iUserID);
	CHECK_NULL_RETURN_FALSE(user_info);

	ASSERT(novice_guide.op_type() == gs_dbs_user_info_op_req::Add);
	CHECK_FALSE_RETURN_FALSE(novice_guide.op_type() == gs_dbs_user_info_op_req::Add);

	user_info->mutable_guide()->add_guide_ids(novice_guide.guide_id());
	return true;
}


bool CUserMgr::_new_hero( UINT32 iUserID, const gs_dbs_user_info_op_req_struct_new_hero& new_hero )
{
	db_user_info* user_info = GetUserInfo(iUserID);
	ASSERT(user_info);

	db_user_hero* hero = user_info->mutable_heros()->add_items();
	hero->CopyFrom(new_hero.hero());

	return true;
}

bool CUserMgr::_op_hero_base(UINT32 iUserID, const gs_dbs_user_info_op_req_struct_op_hero_base& op_hero_base)
{
	db_user_info* user_info = GetUserInfo(iUserID);
	ASSERT(user_info);
	if (op_hero_base.op_type() == gs_dbs_user_info_op_req::Update)
	{
		db_user_hero* old_hero = GetHero(iUserID, op_hero_base.hero_id());
		ASSERT(old_hero);

		if (op_hero_base.has_star())
		{
			old_hero->set_star(op_hero_base.star());
		}

		if (op_hero_base.has_quality())
		{
			old_hero->set_quality(op_hero_base.quality());
		}

		if (op_hero_base.has_level())
		{
			old_hero->set_level(op_hero_base.level());
		}

		if (op_hero_base.has_cur_exp())
		{
			old_hero->set_cur_exp(op_hero_base.cur_exp());
		}
	}
	else if (op_hero_base.op_type() == gs_dbs_user_info_op_req::Delete)
	{
		db_user_hero_array* heros = user_info->mutable_heros();
		UINT32 size = heros->mutable_items()->size();
		
		bool has_find = false;
		UINT32 i = 0;
		for( ; i < size ; ++i)
		{
			if (heros->mutable_items(i)->hero_id() ==  op_hero_base.hero_id())
			{
				has_find = true;
				break;
			}
		}

		if (has_find)
		{
			heros->mutable_items()->SwapElements(i, size-1);
			heros->mutable_items()->RemoveLast();
		}
		else{
			printf("[%s][%u]error! 不支持的操作！", __FUNCTION__, __LINE__);
		}
		
		
	}
	else{
		printf("[%s][%u]error! 不支持的操作！", __FUNCTION__, __LINE__);
		return false;
	}
	return true;
}

bool CUserMgr::_op_hero_equip(UINT32 iUserID, const gs_dbs_user_info_op_req_struct_op_hero_equip& op_hero_equip)
{
	db_user_hero_equip* equip =  GetEquip(iUserID, op_hero_equip.hero_id(), op_hero_equip.equip_id());
	ASSERT(equip);
	
	if (op_hero_equip.op_type() == gs_dbs_user_info_op_req::Update){
		if(op_hero_equip.has_level()){//装备升级操作
			equip->set_level(op_hero_equip.level());
		}
		else if (op_hero_equip.has_evolution_equip_id())//装备进化操作
		{
			equip->set_equip_id(op_hero_equip.evolution_equip_id());
		}
		else{
			printf("[%s][%u]error! 不支持的操作！", __FUNCTION__, __LINE__);
		}
	}
	else{
		printf("[%s][%u]error! 不支持的操作！", __FUNCTION__, __LINE__);
		return false;
	}
	return true;
}

bool CUserMgr::_op_hero_skill(UINT32 iUserID, const gs_dbs_user_info_op_req_struct_op_hero_skill& op_hero_skill)
{
	db_user_hero_skill* skill =  GetSkill(iUserID, op_hero_skill.hero_id(), op_hero_skill.type());
	ASSERT(skill);

	if (op_hero_skill.op_type() == gs_dbs_user_info_op_req::Update)
	{
		skill->set_level(op_hero_skill.level());
	}
	else{
		printf("[%s][%u]error! 不支持的操作！", __FUNCTION__, __LINE__);
		return false;
	}

	return true;
}

bool CUserMgr::_op_friend( UINT32 iUserID, const gs_dbs_user_info_op_req::struct_op_friend& op_friend )
{
	switch(op_friend.op_type()){
		case gs_dbs_user_info_op_req::struct_op_friend::GET_ELEMENT1:{
			ASSERT(op_friend.has_friend_id());
			db_user_friend::friend_info* fri = _get_friend(iUserID, op_friend.friend_id());
			ASSERT(fri);
			fri->set_friend_give_timestamp(0);
												   }break;
		case gs_dbs_user_info_op_req::struct_op_friend::GET_ELEMENTN:{
			db_user_info* user = GetUserInfo(iUserID);
			CHECK_NULL_RETURN_FALSE(user);
			::google::protobuf::RepeatedPtrField< ::db_user_friend_friend_info >::iterator iter = user->mutable_user_friend()->mutable_friends()->begin();
			for (; iter != user->mutable_user_friend()->mutable_friends()->end(); ++iter)
			{
				iter->set_friend_give_timestamp(0);
			}
												   }break;
		case gs_dbs_user_info_op_req::struct_op_friend::GIVE_ELEMENT1:{
			ASSERT(op_friend.has_friend_id());
			db_user_friend::friend_info* fri = _get_friend(iUserID, op_friend.friend_id());
			ASSERT(fri);
			fri->set_me_give_timestamp(Now());
													}break;
		case gs_dbs_user_info_op_req::struct_op_friend::GIVE_ELEMENTN:{
			db_user_info* user = GetUserInfo(iUserID);
			CHECK_NULL_RETURN_FALSE(user);
			::google::protobuf::RepeatedPtrField< ::db_user_friend_friend_info >::iterator iter = user->mutable_user_friend()->mutable_friends()->begin();
			for (; iter != user->mutable_user_friend()->mutable_friends()->end(); ++iter)
			{
				iter->set_me_give_timestamp(Now());
			}
													}break;
		case gs_dbs_user_info_op_req::struct_op_friend::GIVE_UP_APPLY1:{
			ASSERT(op_friend.has_friend_id());
			bool r = _del_user_apply(iUserID, op_friend.friend_id());
			ASSERT(r);
													 }break;
		case gs_dbs_user_info_op_req::struct_op_friend::GIVE_UP_APPLYN:{
			bool r = _del_user_apply(iUserID, 0);//全部删除
			ASSERT(r);
													 }break;
		case gs_dbs_user_info_op_req::struct_op_friend::AFFIRM_AGREE1:{
			ASSERT(op_friend.has_friend_id());
			bool r = _affirm_friend(iUserID, op_friend.friend_id());
			ASSERT(r);
													}break;
		case gs_dbs_user_info_op_req::struct_op_friend::AFFIRM_AGREEN:{
			bool r = _affirm_friend(iUserID, 0);
			ASSERT(r);
													}break;
		case gs_dbs_user_info_op_req::struct_op_friend::AFFIRM_REFUSE1:{
			ASSERT(op_friend.has_friend_id());
			bool r = _del_user_affirm(iUserID, op_friend.friend_id());
			ASSERT(r);
													 }break;
		case gs_dbs_user_info_op_req::struct_op_friend::AFFIRM_REFUSEN:{
			bool r = _del_user_affirm(iUserID, 0);
			ASSERT(r);
													 }break;
		case gs_dbs_user_info_op_req::struct_op_friend::ADD_FRIEND_GS_ONLINE:{
			ASSERT(op_friend.has_base_info());
			db_user_info* info = GetUserInfo(iUserID);
			ASSERT(info);
			friend_base_info* friend_info = info->mutable_user_friend()->add_applies();
			friend_info->CopyFrom(op_friend.base_info());
														   }break;
		case gs_dbs_user_info_op_req::struct_op_friend::DEL_FRIEND:{
			bool r = _del_user_friend(iUserID, op_friend.friend_id());
			ASSERT(r);
												 }break;
		case gs_dbs_user_info_op_req::struct_op_friend::DEL_RECOMMEND_FRIEND:{
			bool r = _del_user_recommend(iUserID, op_friend.friend_id());
			//ASSERT(r);
																			 }break;
		default:
			ASSERT(false);
			return false;
	}
	return true;
}

db_user_friend::friend_info* CUserMgr::_get_friend(UINT32 iUserID, UINT32 iFriend)
{
	db_user_info* user = GetUserInfo(iUserID);
	CHECK_NULL_RETURN_FALSE(user);
	::google::protobuf::RepeatedPtrField< ::db_user_friend_friend_info >::iterator iter = user->mutable_user_friend()->mutable_friends()->begin();
	for (; iter != user->mutable_user_friend()->mutable_friends()->end(); ++iter)
	{
		if (iter->mutable_base_info()->user_id() == iFriend)
		{
			return &*iter;
		}
	}
	return NULL;
}

bool CUserMgr::_del_user_apply(UINT32 iUserID, UINT32 iFriendID)
{
	db_user_info* user = GetUserInfo(iUserID);
	CHECK_NULL_RETURN_FALSE(user);
	if(iFriendID)
	{
		UINT32 iSize = user->mutable_user_friend()->applies_size();
		for (size_t i = 0; i < iSize; ++i)
		{
			if (user->mutable_user_friend()->mutable_applies(i)->user_id() == iFriendID)
			{
				user->mutable_user_friend()->mutable_applies()->SwapElements(i, iSize - 1);
				user->mutable_user_friend()->mutable_applies()->RemoveLast();
				return true;
			}
		}
	}else{
		user->mutable_user_friend()->clear_applies();
		return true;
	}
	return false;
}

bool CUserMgr::_del_user_affirm(UINT32 iUserID, UINT32 iFriendID)
{
	db_user_info* user = GetUserInfo(iUserID);
	CHECK_NULL_RETURN_FALSE(user);
	if(iFriendID)
	{
		UINT32 iSize = user->mutable_user_friend()->confirms_size();
		for (size_t i = 0; i < iSize; ++i)
		{
			if (user->mutable_user_friend()->mutable_confirms(i)->user_id() == iFriendID)
			{
				user->mutable_user_friend()->mutable_confirms()->SwapElements(i, iSize - 1);
				user->mutable_user_friend()->mutable_confirms()->RemoveLast();
				return true;
			}
		}
	}else{
		user->mutable_user_friend()->clear_confirms();
		return true;
	}
	return false;
}

bool CUserMgr::_del_user_recommend(UINT32 iUserID, UINT32 iFriendID){
	db_user_info* user = GetUserInfo(iUserID);
	CHECK_NULL_RETURN_FALSE(user);
	if(iFriendID)
	{
		UINT32 iSize = user->mutable_user_friend()->recommends_size();
		for (size_t i = 0; i < iSize; ++i)
		{
			if (user->mutable_user_friend()->mutable_recommends(i)->user_id() == iFriendID)
			{
				user->mutable_user_friend()->mutable_recommends()->SwapElements(i, iSize - 1);
				user->mutable_user_friend()->mutable_recommends()->RemoveLast();
				return true;
			}
		}
	}else{
		user->mutable_user_friend()->clear_recommends();
		return true;
	}
	return false;
}

bool CUserMgr::_del_user_friend(UINT32 iUserID, UINT32 iFriendID)
{
	db_user_info* user = GetUserInfo(iUserID);
	CHECK_NULL_RETURN_FALSE(user);
	UINT32 iSize = user->mutable_user_friend()->friends_size();
	for (size_t i = 0; i < iSize; ++i)
	{
		if (user->mutable_user_friend()->mutable_friends(i)->mutable_base_info()->user_id() == iFriendID)
		{
			user->mutable_user_friend()->mutable_friends()->SwapElements(i, iSize - 1);
			user->mutable_user_friend()->mutable_friends()->RemoveLast();
			return true;
		}
	}
	return false;
}

bool CUserMgr::_affirm_friend(UINT32 iUserID, UINT32 iFriendID)
{//确认列表->朋友列表
	db_user_info* user = GetUserInfo(iUserID);
	CHECK_NULL_RETURN_FALSE(user);
	if (iFriendID)
	{
		UINT32 iSize = user->mutable_user_friend()->confirms_size();
		for (size_t i = 0; i < iSize; ++i)
		{
			if (user->mutable_user_friend()->mutable_confirms(i)->user_id() == iFriendID)
			{
				db_user_friend::friend_info* fri = user->mutable_user_friend()->add_friends();
				fri->set_friend_give_timestamp(0);
				fri->set_me_give_timestamp(0);
				fri->mutable_base_info()->CopyFrom(*user->mutable_user_friend()->mutable_confirms(i));
				user->mutable_user_friend()->mutable_confirms()->SwapElements(i, iSize - 1);
				user->mutable_user_friend()->mutable_confirms()->RemoveLast();
				return true;
			}
		}
	}
	else{//全部
		::google::protobuf::RepeatedPtrField< ::friend_base_info >::iterator it = user->mutable_user_friend()->mutable_confirms()->begin();
		for (; it != user->mutable_user_friend()->mutable_confirms()->end(); ++it)
		{
			db_user_friend::friend_info* fri = user->mutable_user_friend()->add_friends();
			fri->set_friend_give_timestamp(0);
			fri->set_me_give_timestamp(0);
			fri->mutable_base_info()->CopyFrom(*it);
		}
		user->mutable_user_friend()->clear_confirms();
		return true;
	}
	return false;
}

void CUserMgr::UpdateFriend(gs_dbs_user_friend_op_req& friend_req)
{
	db_user_info* user_info = GetUserInfo(friend_req.user_id());
	if(!user_info)
		return;
	switch(friend_req.op_type()){
	case gs_dbs_user_friend_op_req::FRIEND_GIVE_ELEMENT:{
		db_user_friend::friend_info* fri = _get_friend(friend_req.user_id(), friend_req.friend_user_id());
		ASSERT(fri);
		fri->set_friend_give_timestamp(Now());
														}break;
	case gs_dbs_user_friend_op_req::FRIEND_GIVE_UP_APPLY:{//从待确认列表中删除
		ASSERT(_del_user_affirm(friend_req.user_id(), friend_req.friend_user_id()));
														 }break;
	case gs_dbs_user_friend_op_req::FRIEND_AFFIRM_AGREE:{//申请列表->好友
		ASSERT(_friend_affirm_friend(friend_req.user_id(), friend_req.friend_user_id()));
														}break;
	case gs_dbs_user_friend_op_req::FRIEND_AFFIRM_REFUSE:{//从申请列表删除
		ASSERT(_del_user_apply(friend_req.user_id(), friend_req.friend_user_id()));
														 }break;
	case gs_dbs_user_friend_op_req::FRIEND_DEL_FRIEND:{//删除好友
		ASSERT(_del_user_friend(friend_req.user_id(), friend_req.friend_user_id()));
													  }break;
	case gs_dbs_user_friend_op_req::FRIEND_ADD_FRIEND:{//加入到待确认列表
		ASSERT(friend_req.has_base_info());
		db_user_info* info = GetUserInfo(friend_req.user_id());
		friend_base_info* base_info = info->mutable_user_friend()->add_confirms();
		base_info->CopyFrom(friend_req.base_info());
													  }break;
	}
}

bool CUserMgr::_friend_affirm_friend(UINT32 iUserID, UINT32 iFriendID)
{//申请列表->朋友
	db_user_info* user = GetUserInfo(iUserID);
	ASSERT(user);
	UINT32 iSize = user->mutable_user_friend()->applies_size();
	for (size_t i = 0; i < iSize; ++i)
	{
		if (user->mutable_user_friend()->mutable_applies(i)->user_id() == iFriendID)
		{
			db_user_friend::friend_info* fri = user->mutable_user_friend()->add_friends();
			fri->set_friend_give_timestamp(0);
			fri->set_me_give_timestamp(0);
			fri->mutable_base_info()->CopyFrom(*user->mutable_user_friend()->mutable_applies(i));
			user->mutable_user_friend()->mutable_applies()->SwapElements(i, iSize - 1);
			user->mutable_user_friend()->mutable_applies()->RemoveLast();
			return true;
		}
	}
	return false;
}

bool CUserMgr::_op_crusade( UINT32 iUserID, const gs_dbs_user_info_op_req::struct_op_crusade& op_crusade )
{
	db_user_info* user = GetUserInfo(iUserID);
	ASSERT(user);
	switch(op_crusade.op_type()){
	case gs_dbs_user_info_op_req::struct_op_crusade::CRUSADE_END:
			{
				ASSERT(op_crusade.has_end());
				//我的英雄
				::google::protobuf::RepeatedPtrField< ::user_crusade_crusade_hero >::const_iterator it = op_crusade.end().me_heros().begin();
				for (; it != op_crusade.end().me_heros().end(); ++it)
				{
					user_crusade::crusade_hero* hero = _get_crusade_me_hero(iUserID, it->hero_id());
					if (hero)
					{
						hero->CopyFrom(*it);
					}else{
						hero = user->mutable_user_crusade()->add_me_heros();
						hero->CopyFrom(*it);
					}
				}
				
				//敌人英雄(直接拷贝进去)
				user->mutable_user_crusade()->mutable_enemy_heros()->CopyFrom(op_crusade.end().enemy_heros());

				user->mutable_user_crusade()->set_wave(op_crusade.end().wave());
			}break;
	case gs_dbs_user_info_op_req::struct_op_crusade::HERO_LIFE:
			{//从me_hero中移除
				ASSERT(op_crusade.has_hero_id());
				UINT32 iSize = user->mutable_user_crusade()->me_heros_size();
				for (size_t i = 0; i < iSize; ++i)
				{
					if (user->mutable_user_crusade()->mutable_me_heros(i)->hero_id() == op_crusade.hero_id())
					{
						user->mutable_user_crusade()->mutable_me_heros()->SwapElements(i, iSize - 1);
						user->mutable_user_crusade()->mutable_me_heros()->RemoveLast();
						return true;
					}
				}
			}break;
	case gs_dbs_user_info_op_req::struct_op_crusade::RESET:
			{
				user->mutable_user_crusade()->clear_me_heros();
				user->mutable_user_crusade()->clear_enemy_heros();
				user->mutable_user_crusade()->set_wave(1);
			}break;
	default:
		ASSERT(false);
		return false;
	}
	return true;
}

user_crusade::crusade_hero* CUserMgr::_get_crusade_me_hero( UINT32 iUserID, UINT32 iMeHeroID )
{
	db_user_info* user = GetUserInfo(iUserID);
	ASSERT(user);

	::google::protobuf::RepeatedPtrField< ::user_crusade_crusade_hero >::iterator it = user->mutable_user_crusade()->mutable_me_heros()->begin();
	for (; it != user->mutable_user_crusade()->mutable_me_heros()->end(); ++it)
	{
		if (it->hero_id() == iMeHeroID)
		{
			return &(*it);
		}
	}
	return NULL;
}
//bool CUserMgr::_op_shop( UINT32 iUserID, const gs_dbs_user_info_op_req::struct_op_shop& op_shop )
//{
//
//}
//
//bool CUserMgr::_op_shop_item( UINT32 iUserID, const gs_dbs_user_info_op_req::struct_op_shop_item& op_shop_item )
//{
//
//}
//
//bool CUserMgr::_op_shop_buy( UINT32 iUserID, const db_user_shop_buy& op_shop_buy )
//{
//
//}
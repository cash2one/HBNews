#include <string>
#include "db_sync_operator.h"
#include "db_mgr.h"
#include "time_helper.h"
#include "str.h"

#include "dbs_gs_proto.pb.h"
#include "pb_helper.h"

//#include "task_mgr.h"

#include "db_async_command.h" // for stGetUserInfoRet中的枚举
#include "user_mgr.h"
#include "mail_mgr.h"
int db_sync_operator::get_user_info(UINT32 user_id, db_user_info* user_info )
{
	ASSERT(user_info);
	MYSQL* con = CDBMgr::GetInstance()->GetConnection();
	ASSERT(con);

	//加载未接收系统邮件
	char sql[1024*10] = {0};
	sprintf_s(sql, sizeof(sql),
		"SELECT * FROM `user` WHERE `user_id` = %u;"
		"SELECT `sub_task_id`,`star`, unix_timestamp(`last_enter_time`) as last_enter_time, `last_day_enter_num`,`total_enter_num`, `last_reset_time`, `last_day_reset_num` FROM `user_sub_task` WHERE `user_id` = %u;"
		"SELECT * FROM `user_hero` WHERE `user_id` = %u;"
		"SELECT * FROM `user_hero_equip` WHERE `user_id` = %u;"
		"SELECT * FROM `user_hero_skill` WHERE `user_id` = %u;"
		//玩家邮箱(已接收,删除过期邮件，并把道具给玩家加上)
		"CALL get_user_mail(%u);"
		//玩家邮箱(未接收全服邮件)	
		"CALL get_user_unreceived_sys_mail(%u);"
		//背包和碎片道具
		"SELECT `item_id`, `num` FROM `user_item` WHERE `user_id` = %u;"
		"SELECT * FROM `user_shop` WHERE `user_id` = %u;"
		"SELECT * FROM `user_shop_item` WHERE `user_id` = %u;"
		"SELECT * FROM `user_shop_buy` WHERE `user_id` = %u;"
		
		"SELECT * FROM `user_arena` WHERE `user_id` = %u;"
		"SELECT * FROM `user_arena_log` WHERE `attacker_user_id` = %u OR `defender_user_id` = %u ORDER BY `time` DESC LIMIT 10;"

		//mission
		"SELECT * FROM `user_mission_event` WHERE `user_id` = %u;"
		"SELECT * FROM `user_mission_chain` WHERE `user_id` = %u;"
		"SELECT * FROM `user_sign` WHERE `user_id` = %u;"
		"SELECT `guide_id` FROM `user_guide` WHERE `user_id` = %u;"
		//好友列表
		"SELECT t1.`last_friend_give_element_timestamp`, t1.`last_me_give_element_timestamp`, "
				"t2.`user_id`, t2.`head_id`, t2.`head_frame_id`, t2.`name`, t2.`level`, t2.`vip`, " //基本信息
				"t3.`rank`, "		//竞技场排名
				"t4.`last_update_timestamp` "//最后登陆时间
			"FROM `user_friend` AS t1 "
			"LEFT JOIN `user` AS t2 ON t1.`friend_id` = t2.`user_id` "
			"LEFT JOIN `user_arena` AS t3 ON t1.`friend_id` = t3.`user_id` "
			"LEFT JOIN `user_mission_event` AS t4 ON t1.`friend_id` = t4.`user_id` AND `mission_event_type` = 1002 "
			"WHERE t1.`user_id` = %u;"
		//申请列表
		"SELECT t2.`user_id`, t2.`head_id`, t2.`head_frame_id`, t2.`name`, t2.`level`, t2.`vip`, t3.`rank`, t4.`last_update_timestamp` "
				"FROM `user_friend_apply` AS t1 "
				"LEFT JOIN `user` AS t2 ON t1.`apply_user_id` = t2.`user_id` "
				"LEFT JOIN `user_arena` AS t3 ON t1.`apply_user_id` = t3.`user_id` "
				"LEFT JOIN `user_mission_event` AS t4 ON t1.`apply_user_id` = t4.`user_id` AND `mission_event_type` = 1002 "
				"WHERE t1.`user_id` = %u;"
		//待确认列表
		"SELECT t2.`user_id`, t2.`head_id`, t2.`head_frame_id`, t2.`name`, t2.`level`, t2.`vip`, t3.`rank`, t4.`last_update_timestamp` "
			"FROM `user_friend_apply` AS t1 "
			"LEFT JOIN `user` AS t2 ON t1.`user_id` = t2.`user_id` "
			"LEFT JOIN `user_arena` AS t3 ON t1.`user_id` = t3.`user_id` "
			"LEFT JOIN `user_mission_event` AS t4 ON t1.`user_id` = t4.`user_id` AND `mission_event_type` = 1002 "
			"WHERE t1.`apply_user_id` = %u; "
		//随机5名今日在线玩家(排除自己,排除已经成为好友，排除已经在申请列表中)
		"SELECT t1.user_id, t2.head_id, t2.head_frame_id, t2.`name`, t2.`level`, t2.vip, t3.rank, t1.last_update_timestamp FROM user_mission_event AS t1  " 
		"LEFT JOIN user AS t2 ON t1.user_id = t2.user_id   "
		"LEFT JOIN user_arena AS t3 ON t1.user_id = t3.user_id  " 
			"WHERE t1.mission_event_type = 1002 AND TO_DAYS(FROM_UNIXTIME(t1.last_update_timestamp))  = TO_DAYS(CURDATE()) " 
			"AND t1.user_id != %u AND t1.user_id NOT IN(SELECT friend_id FROM user_friend WHERE user_id = %u) AND t1.user_id NOT IN(SELECT apply_user_id from user_friend_apply WHERE user_id = %u)  "
			"ORDER BY RAND() LIMIT 5;"
		//远征英雄信息
		"SELECT * FROM `user_crusade_hero` WHERE `user_id` = %u;"
		//远征敌人英雄信息
		"SELECT * FROM `user_crusade_enemy` WHERE `user_id` = %u;"
		//远征波数信息
		"SELECT * FROM `user_crusade` WHERE `user_id` = %u;",
		user_id, 
		user_id, 
		user_id, 
		user_id, 
		user_id, 
		user_id, 
		user_id, 
		user_id, 
		user_id,
		user_id,
		user_id,
		user_id,
		user_id,//user_arena
		user_id,//user_arena_log
		user_id,
		user_id,
		user_id,
		user_id,
		user_id,
		user_id,
		user_id,
		user_id,
		user_id,
		user_id,
		user_id,
		user_id,
		user_id
		);
	//int code = mysql_real_query(con, sql, (unsigned int)strlen(sql));
	int code = CDBMgr::Query(con, sql, (unsigned int)strlen(sql));
	if (0 != code){   //非0查询失败  
		printf("query failed! [%s] [%d] [%s] \n", sql, mysql_errno(con), mysql_error(con));
		return stDBGetUserInfoRet::System_Error;
	}

	stDBGetUserInfoRet::EGetUserInfoRet error_code = stDBGetUserInfoRet::Success;
	int i = 0;
	std::map<UINT32, db_user_mail*>	mapReceUserMail;//first:mailid
	std::map<UINT32, db_user_mail*>	mapUnReceUserMail;//first:mailid
	do
	{
		MYSQL_RES* res = mysql_store_result( con );	
		if (!res)
		{
			continue;
		}
		if( error_code == stDBGetUserInfoRet::Success){
			switch (i++)
			{
			case 0:{//base_info
					   {
						   std::string need_fields[] = {"user_id","name","level","exp","vip","diamond","gold","head_id", "head_frame_id",
							"health_cur","health_last_update_timestamp", "skill_point", "skill_point_update_timestamp",
							"invoke_gold_free_left_num","invoke_gold_free_update_timestamp","invoke_gold_n_total_num","invoke_diamond_free_left_num","invoke_diamond_free_update_timestamp",
							"invoke_diamond_n_total_num", "friend_point", "signature", "fight_power", "crusade_point"
						   };
						   std::vector<std::string> v_need_field(need_fields, need_fields + sizeof(need_fields)/sizeof(need_fields[0]));
						   ASSERT(CDBMgr::check_fields(res, v_need_field));
					   }

					   MYSQL_ROW row;
					   if (row = mysql_fetch_row(res))
					   {
						   int col = 0;
						   db_user_base_info* user_base_info = user_info->mutable_user_base_info();
						   user_base_info->set_user_id(atoi(row[col++]));
						   user_base_info->set_name(row[col++]);
						   user_base_info->set_level(atoi(row[col++]));
						   user_base_info->set_exp(atoi(row[col++]));
						   user_base_info->set_vip(atoi(row[col++]));
						   user_base_info->set_diamond(atoi(row[col++]));
						   user_base_info->set_gold(atoi(row[col++]));
						   user_base_info->set_head_id(atoi(row[col++]));
						   user_base_info->set_head_frame_id(atoi(row[col++]));
						   user_base_info->set_health_cur(atoi(row[col++]));
						   user_base_info->set_health_last_update_timestamp(atoi(row[col++]));
						   user_base_info->set_skill_point(atoi(row[col++]));
						   user_base_info->set_skill_point_last_update_timestamp(atoi(row[col++]));

						   user_base_info->set_invoke_gold_free_left_num(atoi(row[col++]));	//上一次金币免费召唤当天已经使用的次数
						   user_base_info->set_invoke_gold_free_update_timestamp(atoi(row[col++]));		//上一次金币免费召唤的时间
						   user_base_info->set_invoke_gold_n_total_num(atoi(row[col++]));		//总共金币免费召唤的次数
						   user_base_info->set_invoke_diamond_free_left_num(atoi(row[col++]));	//总共金币免费召唤的次数
						   user_base_info->set_invoke_diamond_free_update_timestamp(atoi(row[col++]));	//上一次钻石免费召唤的时间
						   user_base_info->set_invoke_diamond_n_total_num(atoi(row[col++]));		//总共钻石免费召唤的次数
						   user_base_info->set_friend_point(atoi(row[col++]));
						   user_base_info->set_signature(row[col++]);
						   user_base_info->set_fight_power(atoi(row[col++]));
						   user_base_info->set_crusade_point(atoi(row[col++]));
					   }
					   else{
						   printf("找不到玩家[%u]在user表基本信息！", user_id);
						   //return -1;//不释放的话 会导致后续的命令执行失败！
						   error_code = stDBGetUserInfoRet::Not_Exist_User;
						   break;
					   }
					  
				   } break;
			case 1:{//sub_task
					   {
						   std::string need_fields[] = {"sub_task_id","star","last_enter_time","last_day_enter_num","total_enter_num", "last_reset_time", "last_day_reset_num"};
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
						   db_user_sub_task* pItem = user_info->mutable_sub_tasks()->add_items();
						   ASSERT(pItem);
						   int col = 0;

						   pItem->set_sub_task_id(atoi(row[col++]));
						   pItem->set_star(atoi(row[col++]));
						   pItem->set_last_enter_timestamp(atoi(row[col++]));
						   pItem->set_last_day_enter_num(atoi(row[col++]));
						   pItem->set_total_enter_num(atoi(row[col++]));
						   pItem->set_last_reset_time(atoi(row[col++]));
						   pItem->set_last_day_reset_num(atoi(row[col++]));

						   //pItem->set_has_got_item1(atoi(row[col++]));
						   //pItem->set_has_got_item2(atoi(row[col++]));
						   //pItem->set_has_got_item3(atoi(row[col++]));
						   //pItem->set_has_got_item4(atoi(row[col++]));
						   //} 
						   //else
						   //{
						   //	printf("玩家[%u]有无效的子任务[%u]信息！\n", user_id, iSubTaskID);
						   //}
					   }
				   } break;
			case 2:{//hero
					   {
						   std::string need_fields[] = {"user_id","hero_id","star","quality","level","cur_exp","got_timestamp"};
						   std::vector<std::string> v_need_field(need_fields, need_fields + sizeof(need_fields)/sizeof(need_fields[0]));
						   ASSERT(CDBMgr::check_fields(res, v_need_field));
					   }

					   MYSQL_ROW row;
					   while (row = mysql_fetch_row(res))
					   {
						   db_user_hero* pItem = user_info->mutable_heros()->add_items();
						   ASSERT(pItem);
						   int col = 1;

						   pItem->set_hero_id(atoi(row[col++]));
						   pItem->set_star(atoi(row[col++]));
						   pItem->set_quality(atoi(row[col++]));
						   pItem->set_level(atoi(row[col++]));
						   pItem->set_cur_exp(atoi(row[col++]));
						   pItem->set_got_timestamp(atoi(row[col++]));

						    //db上没有配置！ 并且GS上CUserXXXStrategy后面会对这里的id作有效性验证！
						   //ASSERT(CHeroMgr::GetInstance()->GetHeroByHeroID(pItem->hero_id()));
					   }
				   }
				   break;
			case 3:{//hero_equip
					   {
						   std::string need_fields[] = {"user_id","hero_id","equip_id","level"};
						   std::vector<std::string> v_need_field(need_fields, need_fields + sizeof(need_fields)/sizeof(need_fields[0]));
						   ASSERT(CDBMgr::check_fields(res, v_need_field));
					   }

					   MYSQL_ROW row;
					   while (row = mysql_fetch_row(res))
					   {
						   db_user_hero* pHero = NULL;
						   UINT32 hero_id = atoi(row[1]);
						   
						   ::google::protobuf::RepeatedPtrField< ::db_user_hero >::iterator it = user_info->mutable_heros()->mutable_items()->begin();
						   for (; it != user_info->mutable_heros()->mutable_items()->end(); ++it)
						   {
							   if (it->hero_id() == hero_id)
							   {
									pHero = &(*it);
									break;
							   }
						   }

						   ASSERT(pHero);
						   db_user_hero_equip* pItem = pHero->add_equips();
						   
						   int col = 2;
						   pItem->set_equip_id(atoi(row[col++]));
						   pItem->set_level(atoi(row[col++]));
						   //db上没有配置！ 并且GS上CUserXXXStrategy后面会对这里的id作有效性验证！
					   }
				   }
				   break;
			case 4:{//hero_skill
					   {
						   std::string need_fields[] = {"user_id","hero_id","bisha_level","aoyi_level","passive1_level","passive2_level","passive3_level"};
						   std::vector<std::string> v_need_field(need_fields, need_fields + sizeof(need_fields)/sizeof(need_fields[0]));
						   ASSERT(CDBMgr::check_fields(res, v_need_field));
					   }

					   MYSQL_ROW row;
					   while (row = mysql_fetch_row(res))
					   {
						   db_user_hero* pHero = NULL;
						   UINT32 hero_id = atoi(row[1]);

						   ::google::protobuf::RepeatedPtrField< ::db_user_hero >::iterator it = user_info->mutable_heros()->mutable_items()->begin();
						   for (; it != user_info->mutable_heros()->mutable_items()->end(); ++it)
						   {
							   if (it->hero_id() == hero_id)
							   {
								   pHero = &(*it);
								   break;
							   }
						   }

						   ASSERT(pHero);//一定有某个英雄 反过来也要判断英雄一定有技

						   int col = 2;
						   UINT32 bisha_level = atoi(row[col++]);
						   UINT32 aoyi_level = atoi(row[col++]);
						   UINT32 passive1_level = atoi(row[col++]);
						   UINT32 passive2_level = atoi(row[col++]);
						   UINT32 passive3_level = atoi(row[col++]);

						   db_user_hero_skill* skill = pHero->add_skills();
						   skill->set_type(user_hero_skill::BISHA);
						   skill->set_level(bisha_level);

						   skill = pHero->add_skills();
						   skill->set_type(user_hero_skill::AOYI);
						   skill->set_level(aoyi_level);

						   skill = pHero->add_skills();
						   skill->set_type(user_hero_skill::PASSIVE1);
						   skill->set_level(passive1_level);

						   skill = pHero->add_skills();
						   skill->set_type(user_hero_skill::PASSIVE2);
						   skill->set_level(passive2_level);

						   skill = pHero->add_skills();
						   skill->set_type(user_hero_skill::PASSIVE3);
						   skill->set_level(passive3_level);

						   //db上没有配置！ 并且GS上CUserXXXStrategy后面会对这里的id作有效性验证！
					   }
				   }
				   break;

			case 5:
				{	//邮件基本信息

					{
						std::string need_fields[] = {"mail_id", "send_time", "content", "status", "type"};
						std::vector<std::string> v_need_field(need_fields, need_fields + sizeof(need_fields)/sizeof(need_fields[0]));
						ASSERT(CDBMgr::check_fields(res, v_need_field));
					}
					MYSQL_ROW row;
					while(row = mysql_fetch_row(res))
					{
						int col = 0;
						db_user_mail* pMail = user_info->mutable_mails_received()->add_mails();
						pMail->set_mail_id(atoi(row[col++]));
						pMail->set_send_time(atoi(row[col++]));
						if (row[col])
						{
							pMail->set_content(row[col]);
						}
						col++;
						pMail->set_status(MAIL_STATUS(atoi(row[col++])));
						pMail->set_type(MAIL_TYPE(atoi(row[col++])));
						mapReceUserMail[atoi(row[0])] = pMail;
					}
				}
				break;
	
			case 6:
				{
					//邮件附件信息(道具，金币，经验等)
					{
						std::string need_fields[] = {"mail_id", "id", "element_id", "type", "num"};
						std::vector<std::string> v_need_field(need_fields, need_fields + sizeof(need_fields)/sizeof(need_fields[0]));
						ASSERT(CDBMgr::check_fields(res, v_need_field));
					}
					MYSQL_ROW row;
					while (row = mysql_fetch_row(res))
					{
						int col = 0;
						ASSERT(mapReceUserMail.find(atoi(row[col++])) != mapReceUserMail.end());
						db_user_mail* pMail = mapReceUserMail[atoi(row[col - 1])];
						ASSERT(pMail);
						db_mail_attach* pAttach = pMail->add_attachs();
						pAttach->set_attach_id(atoi(row[col++]));
						pAttach->mutable_element()->set_id(atoi(row[col++]));
						pAttach->mutable_element()->set_type(struct_game_element::EELEMENTTYPE(atoi(row[col++])));
						pAttach->mutable_element()->set_num(atoi(row[col++]));
						pAttach->mutable_element()->set_num_type(struct_game_element::INCREASE);
					}
				}
				break;
			case 7:
				{//未接收邮件
					{
						std::string need_fields[] = {"id", "send_time", "content", "type"};
						std::vector<std::string> v_need_field(need_fields, need_fields + sizeof(need_fields)/sizeof(need_fields[0]));
						ASSERT(CDBMgr::check_fields(res, v_need_field));
					}
					MYSQL_ROW row;
					while(row = mysql_fetch_row(res))
					{
						int col = 1;
						db_user_mail* pMail = user_info->mutable_mails_unreceived()->add_mails();
						pMail->set_mail_id(0);
						pMail->set_send_time(atoi(row[col++]));
						if (row[col])
						{
							pMail->set_content(row[col]);
						}
						col++;
						pMail->set_status(MAIL_STATUS_NOT_READED);
						pMail->set_type(MAIL_TYPE(atoi(row[col++])));
						mapUnReceUserMail[atoi(row[0])] = pMail;
					}
				}
				break;
			case 8:
				{
					//未接收邮件附件信息(道具，金币，经验等)
					{
						std::string need_fields[] = {"mail_id", "element_id", "type", "num"};
						std::vector<std::string> v_need_field(need_fields, need_fields + sizeof(need_fields)/sizeof(need_fields[0]));
						ASSERT(CDBMgr::check_fields(res, v_need_field));
					}
					MYSQL_ROW row;
					while (row = mysql_fetch_row(res))
					{
						int col = 0;
						ASSERT(mapUnReceUserMail.find(atoi(row[col++])) != mapUnReceUserMail.end());
						db_user_mail* pMail = mapUnReceUserMail[atoi(row[col - 1])];
						ASSERT(pMail);
						db_mail_attach* pAttach = pMail->add_attachs();
						pAttach->set_attach_id(0);
						pAttach->mutable_element()->set_id(atoi(row[col++]));
						pAttach->mutable_element()->set_type(struct_game_element::EELEMENTTYPE(atoi(row[col++])));
						pAttach->mutable_element()->set_num(atoi(row[col++]));
						pAttach->mutable_element()->set_num_type(struct_game_element::INCREASE);
					}
				}
				break;
			case 9:
				{//item&fragmet
					{
						std::string need_fields[] = {"item_id", "num"};
						std::vector<std::string> v_need_field(need_fields, need_fields + sizeof(need_fields)/sizeof(need_fields[0]));
						ASSERT(CDBMgr::check_fields(res, v_need_field));
					}

					MYSQL_ROW row;
					while (row = mysql_fetch_row(res))
					{
						int col = 0;
						struct_game_element* pElement = user_info->mutable_items()->add_items();
						ASSERT(pElement);
						pElement->set_id(atoi(row[col++]));	
						pElement->set_num(atoi(row[col++]));
						pElement->set_type(struct_game_element::EELEMENTTYPE_ITEM);
						pElement->set_num_type(struct_game_element::TOTAL);
					}
				}
				break;
			case 10:
				{//user_shop
					{
						std::string need_fields[] = {"user_id", "shop_type", "last_auto_refresh_time", "last_manual_refresh_time", "last_day_manual_refresh_num", "total_manual_refresh_num", "random_open_start_timestamp", "random_open_end_timestamp"};
						std::vector<std::string> v_need_field(need_fields, need_fields + sizeof(need_fields)/sizeof(need_fields[0]));
						ASSERT(CDBMgr::check_fields(res, v_need_field));
					}

					MYSQL_ROW row;
					while (row = mysql_fetch_row(res))
					{
						int col = 1;//忽略 user_id
						db_user_shop* pItem = user_info->mutable_shops()->add_items();
						ASSERT(pItem);

						pItem->set_shop_type((user_shop_ESHOPTYPE)atoi(row[col++]));
						ASSERT(pItem->shop_type() > user_shop_ESHOPTYPE::user_shop_ESHOPTYPE_min && pItem->shop_type() < user_shop_ESHOPTYPE::user_shop_ESHOPTYPE_max);
						pItem->set_last_auto_refresh_time(atoi(row[col++]));
						pItem->set_last_manual_refresh_time(atoi(row[col++]));
						pItem->set_last_day_manual_refresh_num(atoi(row[col++]));
						
						//total_manual_refresh_num 忽略这个字段
						col++;

						pItem->set_random_open_start_timestamp(atoi(row[col++]));
						pItem->set_random_open_end_timestamp(atoi(row[col++]));
					}
				}
				break;
			case 11:
				{//user_shop_item
					{
						std::string need_fields[] = {"user_id", "shop_type", "pos", "element_type", "element_id", "element_num", "consume_type", "consume_id", "consume_num", "flag", "is_sold"};
						std::vector<std::string> v_need_field(need_fields, need_fields + sizeof(need_fields)/sizeof(need_fields[0]));
						ASSERT(CDBMgr::check_fields(res, v_need_field));
					}

					MYSQL_ROW row;
					while (row = mysql_fetch_row(res))
					{
					
						user_shop_ESHOPTYPE shop_type = (user_shop_ESHOPTYPE)atoi(row[1]);
						ASSERT(shop_type > user_shop_ESHOPTYPE::user_shop_ESHOPTYPE_min && shop_type < user_shop_ESHOPTYPE::user_shop_ESHOPTYPE_max);
						
						db_user_shop* pShop = NULL;
						for ( ::google::protobuf::RepeatedPtrField< ::db_user_shop >::iterator it = user_info->mutable_shops()->mutable_items()->begin() ; it != user_info->mutable_shops()->mutable_items()->end(); ++it){
							if (it->shop_type() == shop_type){
								pShop = &*it;
							}
						}
						
						//判断所属的商店存在
						ASSERT(pShop);
						
						db_user_shop_item* pItem = pShop->add_items();
						ASSERT(pItem);

						//pItem->set_shop_type(shop_type);
						int col = 2;
						pItem->set_pos(atoi(row[col++]));
						pItem->set_element_type((struct_game_element::EELEMENTTYPE)atoi(row[col++]));
						ASSERT(struct_game_element::EELEMENTTYPE_IsValid(pItem->element_type()));
						pItem->set_element_id(atoi(row[col++]));
						pItem->set_element_num(atoi(row[col++]));

						pItem->set_consume_type((struct_game_element::EELEMENTTYPE)atoi(row[col++]));
						ASSERT(struct_game_element::EELEMENTTYPE_IsValid(pItem->consume_type()));
						pItem->set_consume_id(atoi(row[col++]));
						pItem->set_consume_num(atoi(row[col++]));

						pItem->set_flag((user_shop_item::ESHOPITEMFLAG)atoi(row[col++]));
						ASSERT(user_shop_item::ESHOPITEMFLAG_IsValid(pItem->flag()));
						pItem->set_is_sold(atoi(row[col++]));
					}
				}
				break;
			case 12:
				{//user_shop_buy
					{
						std::string need_fields[] = {"user_id", "element_type", "element_id", "last_day_buy_num", "last_buy_time"};
						std::vector<std::string> v_need_field(need_fields, need_fields + sizeof(need_fields)/sizeof(need_fields[0]));
						ASSERT(CDBMgr::check_fields(res, v_need_field));
					}

					MYSQL_ROW row;
					while (row = mysql_fetch_row(res))
					{
						int col = 1;
						db_user_shop_buy* pItem = user_info->mutable_buy_items()->add_items();
						ASSERT(pItem);

						pItem->set_element_type((struct_game_element::EELEMENTTYPE)atoi(row[col++]));
						ASSERT(struct_game_element::EELEMENTTYPE_IsValid(pItem->element_type()));
						//todo 在gs上面做IsElementValid()！

						pItem->set_element_id(atoi(row[col++]));
						pItem->set_last_day_buy_num(atoi(row[col++]));
						pItem->set_last_buy_time(atoi(row[col++]));
					}
				}
				break;
			
			case 13:
				{//user_arena
					{
						std::string need_fields[] = {"rank", "user_id", "score", "hero_id1", "hero_id2", "hero_id3", "hero_id4", "hero_id5", "last_challenge_time", "last_day_challenge_num", "history_highest_rank", "last_time_buy_arena_challenge", "last_day_buy_arena_challenge_num"};
						std::vector<std::string> v_need_field(need_fields, need_fields + sizeof(need_fields)/sizeof(need_fields[0]));
						ASSERT(CDBMgr::check_fields(res, v_need_field));
					}

					MYSQL_ROW row;
					if (row = mysql_fetch_row(res)){
						db_user_arena* pItem = user_info->mutable_arena();
						ASSERT(pItem);
						int col = 0;
						pItem->set_rank(atoi(row[col++]));

						//pItem->set_user_id(atoi(row[col++]));
						col++;//忽略user_id

						pItem->set_score(atoi(row[col++]));
						pItem->add_heros(atoi(row[col++]));//hero_id1
						pItem->add_heros(atoi(row[col++]));//hero_id2
						pItem->add_heros(atoi(row[col++]));//hero_id3
						pItem->add_heros(atoi(row[col++]));//hero_id4
						pItem->add_heros(atoi(row[col++]));//hero_id5
						pItem->set_last_challenge_time(atoi(row[col++]));
						pItem->set_last_day_challenge_num(atoi(row[col++]));
						pItem->set_history_highest_rank(atoi(row[col++]));
						pItem->set_last_time_buy_arena_challenge(atoi(row[col++]));
						pItem->set_last_day_buy_arena_challenge_num(atoi(row[col++]));
					}
				}
				break;
			case 14:
				{//user_arena_log
					{
						std::string need_fields[] = {"attacker_user_id", "attacker_rank", "attacker_power", "attacker_name", "attacker_level", "attacker_hero_id1", "attacker_hero_quality1", "attacker_hero_level1", "attacker_hero_star1", "attacker_hero_id2", "attacker_hero_quality2", "attacker_hero_level2", "attacker_hero_star2", "attacker_hero_id3", "attacker_hero_quality3", "attacker_hero_level3", "attacker_hero_star3", "attacker_hero_id4", "attacker_hero_quality4", "attacker_hero_level4", "attacker_hero_star4", "attacker_hero_id5", "attacker_hero_quality5", "attacker_hero_level5", "attacker_hero_star5", "defender_user_id", "defender_rank", "defender_power", "defender_name", "defender_level", "defender_hero_id1", "defender_hero_quality1", "defender_hero_level1", "defender_hero_star1", "defender_hero_id2", "defender_hero_quality2", "defender_hero_level2", "defender_hero_star2", "defender_hero_id3", "defender_hero_quality3", "defender_hero_level3", "defender_hero_star3", "defender_hero_id4", "defender_hero_quality4", "defender_hero_level4", "defender_hero_star4", "defender_hero_id5", "defender_hero_quality5", "defender_hero_level5", "defender_hero_star5", "is_win", "time"};
						std::vector<std::string> v_need_field(need_fields, need_fields + sizeof(need_fields)/sizeof(need_fields[0]));
						ASSERT(CDBMgr::check_fields(res, v_need_field));
					}

					MYSQL_ROW row;
					while (row = mysql_fetch_row(res))
					{
						ASSERT(user_info->has_arena());

						int col = 0;
						UINT32 attacker_user_id = atoi(row[col++]);
						UINT32 attacker_rank = atoi(row[col++]);
						UINT32 attacker_power = atoi(row[col++]);
						std::string attacker_name = (row[col++]);
						UINT32 attacker_level = atoi(row[col++]);
						
						UINT32 attacker_hero_id1 = atoi(row[col++]);
						UINT32 attacker_hero_quality1 = atoi(row[col++]);
						UINT32 attacker_hero_level1 = atoi(row[col++]);
						UINT32 attacker_hero_star1 = atoi(row[col++]);
						
						UINT32 attacker_hero_id2 = atoi(row[col++]);
						UINT32 attacker_hero_quality2 = atoi(row[col++]);
						UINT32 attacker_hero_level2 = atoi(row[col++]);
						UINT32 attacker_hero_star2 = atoi(row[col++]);
						
						UINT32 attacker_hero_id3 = atoi(row[col++]);
						UINT32 attacker_hero_quality3 = atoi(row[col++]);
						UINT32 attacker_hero_level3 = atoi(row[col++]);
						UINT32 attacker_hero_star3 = atoi(row[col++]);

						UINT32 attacker_hero_id4 = atoi(row[col++]);
						UINT32 attacker_hero_quality4 = atoi(row[col++]);
						UINT32 attacker_hero_level4 = atoi(row[col++]);
						UINT32 attacker_hero_star4 = atoi(row[col++]);

						UINT32 attacker_hero_id5 = atoi(row[col++]);
						UINT32 attacker_hero_quality5 = atoi(row[col++]);
						UINT32 attacker_hero_level5 = atoi(row[col++]);
						UINT32 attacker_hero_star5 = atoi(row[col++]);

						UINT32 defender_user_id = atoi(row[col++]);
						UINT32 defender_rank = atoi(row[col++]);
						UINT32 defender_power = atoi(row[col++]);
						std::string defender_name = (row[col++]);
						UINT32 defender_level = atoi(row[col++]);

						UINT32 defender_hero_id1 = atoi(row[col++]);
						UINT32 defender_hero_quality1 = atoi(row[col++]);
						UINT32 defender_hero_level1 = atoi(row[col++]);
						UINT32 defender_hero_star1 = atoi(row[col++]);

						UINT32 defender_hero_id2 = atoi(row[col++]);
						UINT32 defender_hero_quality2 = atoi(row[col++]);
						UINT32 defender_hero_level2 = atoi(row[col++]);
						UINT32 defender_hero_star2 = atoi(row[col++]);

						UINT32 defender_hero_id3 = atoi(row[col++]);
						UINT32 defender_hero_quality3 = atoi(row[col++]);
						UINT32 defender_hero_level3 = atoi(row[col++]);
						UINT32 defender_hero_star3 = atoi(row[col++]);

						UINT32 defender_hero_id4 = atoi(row[col++]);
						UINT32 defender_hero_quality4 = atoi(row[col++]);
						UINT32 defender_hero_level4 = atoi(row[col++]);
						UINT32 defender_hero_star4 = atoi(row[col++]);

						UINT32 defender_hero_id5 = atoi(row[col++]);
						UINT32 defender_hero_quality5 = atoi(row[col++]);
						UINT32 defender_hero_level5 = atoi(row[col++]);
						UINT32 defender_hero_star5 = atoi(row[col++]);
						UINT32 is_win = atoi(row[col++]);
						UINT32 time = atoi(row[col++]);

						db_user_arena_log* pItem = user_info->mutable_arena()->add_logs();
						ASSERT(pItem);

						pItem->set_is_attacker_win(is_win);
						pItem->set_time(time);

						if (user_id == attacker_user_id){
							pItem->set_is_me_attacker(true);
							pItem->set_me_rank(attacker_rank);

							//显示对手信息
							pItem->set_match_user_id(defender_user_id);
							pItem->set_match_rank(defender_rank);
							pItem->set_match_power(defender_power);
							pItem->set_match_name(defender_name);
							pItem->set_match_level(defender_level);

							db_user_arena_log::db_user_arena_log_hero* pLog = pItem->add_match_heros();
							pLog->set_hero_id(defender_hero_id1);
							pLog->set_quality(defender_hero_quality1);
							pLog->set_level(defender_hero_level1);
							pLog->set_star(defender_hero_star1);

							pLog = pItem->add_match_heros();
							pLog->set_hero_id(defender_hero_id2);
							pLog->set_quality(defender_hero_quality2);
							pLog->set_level(defender_hero_level2);
							pLog->set_star(defender_hero_star2);

							pLog = pItem->add_match_heros();
							pLog->set_hero_id(defender_hero_id3);
							pLog->set_quality(defender_hero_quality3);
							pLog->set_level(defender_hero_level3);
							pLog->set_star(defender_hero_star3);

							pLog = pItem->add_match_heros();
							pLog->set_hero_id(defender_hero_id4);
							pLog->set_quality(defender_hero_quality4);
							pLog->set_level(defender_hero_level4);
							pLog->set_star(defender_hero_star4);

							pLog = pItem->add_match_heros();
							pLog->set_hero_id(defender_hero_id5);
							pLog->set_quality(defender_hero_quality5);
							pLog->set_level(defender_hero_level5);
							pLog->set_star(defender_hero_star5);
						}
						else if(user_id == defender_user_id){
							pItem->set_is_me_attacker(false);
							pItem->set_me_rank(defender_rank);

							//显示对手信息
							pItem->set_match_user_id(attacker_user_id);
							pItem->set_match_rank(attacker_rank);
							pItem->set_match_power(attacker_power);
							pItem->set_match_name(attacker_name);
							pItem->set_match_level(attacker_level);

							db_user_arena_log::db_user_arena_log_hero* pLog = pItem->add_match_heros();
							pLog->set_hero_id(attacker_hero_id1);
							pLog->set_quality(attacker_hero_quality1);
							pLog->set_level(attacker_hero_level1);
							pLog->set_star(attacker_hero_star1);

							pLog = pItem->add_match_heros();
							pLog->set_hero_id(attacker_hero_id2);
							pLog->set_quality(attacker_hero_quality2);
							pLog->set_level(attacker_hero_level2);
							pLog->set_star(attacker_hero_star2);

							pLog = pItem->add_match_heros();
							pLog->set_hero_id(attacker_hero_id3);
							pLog->set_quality(attacker_hero_quality3);
							pLog->set_level(attacker_hero_level3);
							pLog->set_star(attacker_hero_star3);

							pLog = pItem->add_match_heros();
							pLog->set_hero_id(attacker_hero_id4);
							pLog->set_quality(attacker_hero_quality4);
							pLog->set_level(attacker_hero_level4);
							pLog->set_star(attacker_hero_star4);

							pLog = pItem->add_match_heros();
							pLog->set_hero_id(attacker_hero_id5);
							pLog->set_quality(attacker_hero_quality5);
							pLog->set_level(attacker_hero_level5);
							pLog->set_star(attacker_hero_star5);
						}
						else{
							printf("I[%u] am neither defender nor attacker！！！ \n", user_id);
							ASSERT(false);
							return false;
						}
					}
				}
				break;
			case 15:
				{//user_mission_event
					{
						std::string need_fields[] = {"user_id", "mission_event_type", "mission_event_sub_type", "last_update_timestamp", "last_day_value", "total_value"};
						std::vector<std::string> v_need_field(need_fields, need_fields + sizeof(need_fields)/sizeof(need_fields[0]));
						ASSERT(CDBMgr::check_fields(res, v_need_field));
					}

					MYSQL_ROW row;
					while(row = mysql_fetch_row(res))
					{
						int col = 1;
						db_user_mission_event* pEvent = user_info->mutable_mission_events()->add_events();
						pEvent->set_mission_event_type((db_user_mission_event::MISSION_EVENT_TYPE)atoi(row[col++]));
						pEvent->set_mission_event_sub_type(atoi(row[col++]));
						if (row[col++])
						{
							pEvent->set_last_update_timestamp(atoi(row[col - 1]));
						}
						if (row[col++])
						{
							pEvent->set_last_day_value(atoi(row[col - 1]));
						}
						if (row[col++])
						{
							pEvent->set_total_value(atoi(row[col - 1]));
						}
					}
				}
				break;
			case 16:
				{//user_mission_chain
					{
						std::string need_fields[] = {"user_id", "mission_chain_id", "last_get_reward_mission_id", "last_get_reward_timestamp"};
						std::vector<std::string> v_need_field(need_fields, need_fields + sizeof(need_fields)/sizeof(need_fields[0]));
						ASSERT(CDBMgr::check_fields(res, v_need_field));
					}

					MYSQL_ROW row;
					while (row = mysql_fetch_row(res))
					{
						int col = 1;
						db_user_mission_chain* pChain = user_info->mutable_mission_chains()->add_chains();
						pChain->set_mission_chain_id(atoi(row[col++]));
						pChain->set_last_get_reward_mission_id(atoi(row[col++]));
						pChain->set_last_get_reward_timestamp(atoi(row[col++]));
					}
				}
				break;
			case 17:
				{//user_sign
					{
						std::string need_fields[] = {"user_id", "sign_1_times", "sign_n_reward_status", "last_sign_time", "add_signed_times"};
						std::vector<std::string> v_need_field(need_fields, need_fields + sizeof(need_fields)/sizeof(need_fields[0]));
						ASSERT(CDBMgr::check_fields(res, v_need_field));
					}
					MYSQL_ROW row;
					if (row = mysql_fetch_row(res))
					{
						int col = 1;
						db_user_sign* pSign = user_info->mutable_sign();
						pSign->set_sign_1_times(atoi(row[col++]));
						pSign->set_sign_n_reward_status(row[col++]);
						pSign->set_last_sign_time(atoi(row[col++]));
						pSign->set_add_signed_times(atoi(row[col++]));
					}
				}
				break;
			case 18:
				{//user_guide
					{
						std::string need_fields[] = {"guide_id"};
						std::vector<std::string> v_need_field(need_fields, need_fields + sizeof(need_fields)/sizeof(need_fields[0]));
						ASSERT(CDBMgr::check_fields(res, v_need_field));
					}
					MYSQL_ROW row;
					while(row = mysql_fetch_row(res))
					{
						int col = 0;
						user_info->mutable_guide()->add_guide_ids(atoi(row[col++]));
					}
				}
				break;
			case 19:
				{//user * user_friend
					{
						std::string need_fields[] = {"last_friend_give_element_timestamp", "last_me_give_element_timestamp", "user_id", "head_id", "head_frame_id", "name", "level", "vip", "rank", "last_update_timestamp"};
						std::vector<std::string> v_need_field(need_fields, need_fields + sizeof(need_fields)/sizeof(need_fields[0]));
						ASSERT(CDBMgr::check_fields(res, v_need_field));
					}
					MYSQL_ROW row;
					while(row = mysql_fetch_row(res))
					{
						int col = 0;
						db_user_friend::friend_info* fri = user_info->mutable_user_friend()->add_friends();
						fri->set_friend_give_timestamp(atoi(row[col++]));
						fri->set_me_give_timestamp(atoi(row[col++]));
						fri->mutable_base_info()->set_user_id(atoi(row[col++]));
						fri->mutable_base_info()->set_head_id(atoi(row[col++]));
						fri->mutable_base_info()->set_head_frame_id(atoi(row[col++]));
						fri->mutable_base_info()->set_name(row[col++]);
						fri->mutable_base_info()->set_level(atoi(row[col++]));
						fri->mutable_base_info()->set_vip(atoi(row[col++]));
						if (row[col++])
						{
							fri->mutable_base_info()->set_arena_rank(atoi(row[col - 1]));
						}else{
							fri->mutable_base_info()->set_arena_rank(0);
						}
						
						if (row[col++])
						{
							fri->mutable_base_info()->set_last_login_time(atoi(row[col - 1]));
						}else{
							fri->mutable_base_info()->set_last_login_time(Now());
						}
					}
				}
				break;
			case 20:
				{//user * user_friend_apply 申请列表 
					{
						std::string need_fields[] = {"user_id", "head_id", "head_frame_id", "name", "level", "vip", "rank", "last_update_timestamp"};
						std::vector<std::string> v_need_field(need_fields, need_fields + sizeof(need_fields)/sizeof(need_fields[0]));
						ASSERT(CDBMgr::check_fields(res, v_need_field));
					}
					MYSQL_ROW row;
					while(row = mysql_fetch_row(res))
					{
						int col = 0;
						friend_base_info* fri = user_info->mutable_user_friend()->add_applies();
						fri->set_user_id(atoi(row[col++]));
						fri->set_head_id(atoi(row[col++]));
						fri->set_head_frame_id(atoi(row[col++]));
						fri->set_name(row[col++]);
						fri->set_level(atoi(row[col++]));
						fri->set_vip(atoi(row[col++]));
						if(row[col++])
							fri->set_arena_rank(atoi(row[col - 1]));
						else{
							fri->set_arena_rank(0);
						}

						if(row[col++])
							fri->set_last_login_time(atoi(row[col - 1]));
						else{
							fri->set_last_login_time(0);
						}
					}
				}
				break;
			case 21:
				{//user * user_friend_apply 待确认列表
					{
						std::string need_fields[] = {"user_id", "head_id", "head_frame_id", "name", "level", "vip", "rank", "last_update_timestamp"};
						std::vector<std::string> v_need_field(need_fields, need_fields + sizeof(need_fields)/sizeof(need_fields[0]));
						ASSERT(CDBMgr::check_fields(res, v_need_field));
					}
					MYSQL_ROW row;
					while (row = mysql_fetch_row(res))
					{
						int col = 0;
						friend_base_info* fri = user_info->mutable_user_friend()->add_confirms();
						fri->set_user_id(atoi(row[col++]));
						fri->set_head_id(atoi(row[col++]));
						fri->set_head_frame_id(atoi(row[col++]));
						fri->set_name(row[col++]);
						fri->set_level(atoi(row[col++]));
						fri->set_vip(atoi(row[col++]));
						if(row[col++]){
							fri->set_arena_rank(atoi(row[col - 1]));
						}
						else{
							fri->set_arena_rank(0);
						}
						
						if(row[col++])
							fri->set_last_login_time(atoi(row[col - 1]));
						else{
							fri->set_last_login_time(0);
						}	
					}
				}
				break;
			case 22:
				{//推荐列表
					{
						std::string need_fields[] = {"user_id", "head_id", "head_frame_id", "name", "level", "vip", "rank", "last_update_timestamp"};
						std::vector<std::string> v_need_field(need_fields, need_fields + sizeof(need_fields)/sizeof(need_fields[0]));
						ASSERT(CDBMgr::check_fields(res, v_need_field));
					}
					MYSQL_ROW row;
					while (row = mysql_fetch_row(res))
					{
						int col = 0;
						friend_base_info* fri = user_info->mutable_user_friend()->add_recommends();
						fri->set_user_id(atoi(row[col++]));
						fri->set_head_id(atoi(row[col++]));
						fri->set_head_frame_id(atoi(row[col++]));
						fri->set_name(row[col++]);
						fri->set_level(atoi(row[col++]));
						fri->set_vip(atoi(row[col++]));

						if(row[col++]){
							fri->set_arena_rank(atoi(row[col - 1]));
						}
						else{
							fri->set_arena_rank(0);
						}

						if(row[col++])
							fri->set_last_login_time(atoi(row[col - 1]));
						else{
							fri->set_last_login_time(0);
						}	
					}
				}
				break;
			case 23:
				{//远征英雄信息
					{
						std::string need_fields[] = {"user_id", "hero_id", "hp", "ao_yi", "bi_sha"};
						std::vector<std::string> v_need_field(need_fields, need_fields + sizeof(need_fields)/sizeof(need_fields[0]));
						ASSERT(CDBMgr::check_fields(res, v_need_field));
					}
					
					MYSQL_ROW row;
					while (row = mysql_fetch_row(res))
					{
						int col = 1;
						user_crusade::crusade_hero* me_cr_hero = user_info->mutable_user_crusade()->add_me_heros();
						me_cr_hero->set_hero_id(atoi(row[col++]));
						me_cr_hero->set_hp(atoi(row[col++]));
						me_cr_hero->set_ao_yi(atoi(row[col++]));
						me_cr_hero->set_bi_sha(atoi(row[col++]));
					}

				}break;
			case 24:
				{//远征敌人英雄信息
					{
						std::string need_fields[] = {"user_id", "hero_id", "hp", "ao_yi", "bi_sha"};
						std::vector<std::string> v_need_field(need_fields, need_fields + sizeof(need_fields)/sizeof(need_fields[0]));
						ASSERT(CDBMgr::check_fields(res, v_need_field));
					}

					MYSQL_ROW row;
					while (row = mysql_fetch_row(res))
					{
						int col = 1;
						user_crusade::crusade_hero* enemy_cr_hero = user_info->mutable_user_crusade()->add_enemy_heros();
						enemy_cr_hero->set_hero_id(atoi(row[col++]));
						enemy_cr_hero->set_hp(atoi(row[col++]));
						enemy_cr_hero->set_ao_yi(atoi(row[col++]));
						enemy_cr_hero->set_bi_sha(atoi(row[col++]));
					}
				}break;
			case 25:
				{//当前远征所在波数
					{
						std::string need_fields[] = {"user_id", "wave"};
						std::vector<std::string> v_need_field(need_fields, need_fields + sizeof(need_fields)/sizeof(need_fields[0]));
						ASSERT(CDBMgr::check_fields(res, v_need_field));
					}

					MYSQL_ROW row;
					if (row = mysql_fetch_row(res))
					{
						int col = 1;
						user_info->mutable_user_crusade()->set_wave(atoi(row[col++]));
					}
				}break;
			}
		}
		mysql_free_result(res);
	}while( !mysql_next_result( con ) );

	return error_code;
}

int db_sync_operator::add_or_update_sub_task( UINT32 user_id, gs_dbs_user_info_op_req_struct_op_sub_task* sub_task )
{
	ASSERT(sub_task);
	MYSQL* con = CDBMgr::GetInstance()->GetConnection();
	ASSERT(con);
	
	std::string sql;
	int r = _get_sql_of_add_or_update_sub_task(sql, user_id, sub_task);
	if ( r != 0){
		ASSERT(false);
		return r;
	}

	int code = CDBMgr::Query(con, sql);
	if (0 != code){   //非0查询失败  
		printf("query failed! [%s] [%d] [%s] \n", sql.c_str(), mysql_errno(con), mysql_error(con));
		return -1;
	}

	return 0;
}

int db_sync_operator::_get_sql_of_add_or_update_sub_task(std::string& strAppendHere, UINT32 user_id, gs_dbs_user_info_op_req_struct_op_sub_task* sub_task )
{
	ASSERT(sub_task);
	if (!sub_task){
		return -1;
	}

	char sql[1024*10] = {0};
	int used = sprintf_s(sql, sizeof(sql), " INSERT INTO `user_sub_task` "
		" SET  `user_id` = %u, "
		" `sub_task_id` = %u, "
		" `star` = %u, "
		" `last_enter_time` = from_unixtime(%u), "
		" `last_day_enter_num` = %u, "
		" `total_enter_num` = %u, "
		" `last_reset_time` = %u, "
		" `last_day_reset_num` = %u "
		" ON DUPLICATE KEY UPDATE "
		//" `star` =  %u, "//会出现star没有赋值的情况！ 结果直接就设置为0了
		" `star` = GREATEST(`star`, %u), "
		" `last_enter_time` = from_unixtime(%u), "
		" `last_day_enter_num` = %u, "
		" `total_enter_num` =  %u, "
		" `last_reset_time` = %u, "
		" `last_day_reset_num` = %u ;"
		, user_id, sub_task->sub_task_id(), sub_task->star(), sub_task->last_enter_timestamp(), sub_task->last_day_enter_num(), sub_task->total_enter_num(), sub_task->last_reset_time(), sub_task->last_day_reset_num()
		, sub_task->star(), sub_task->last_enter_timestamp(), sub_task->last_day_enter_num(), sub_task->total_enter_num(), sub_task->last_reset_time(), sub_task->last_day_reset_num()
		);

	ASSERT(used != -1);
	if (used == -1){
		return -1;
	}

	strAppendHere.append(sql);
	return 0;
}

int db_sync_operator::_new_hero(char* &p, int &left, UINT32 user_id, const db_user_hero& hero){
	//英雄 只有Add操作的时候需要同时写入数据库 装备和技能信息
	//其他时候只需要写入英雄基本信息 装备和技能信息的更新下面有独立的操作
	//英雄基本信息
	int used = sprintf_s(p, left, 
		" INSERT INTO `user_hero` "
		" SET `user_id` = %u, `hero_id` = %u, `star` = %u, `quality` = %u, `level` = %u, `cur_exp` = %u, `got_timestamp` = %u ;",
		user_id, hero.hero_id(), hero.star(), hero.quality(), hero.level(), hero.cur_exp(), hero.got_timestamp()
		);

	ASSERT(used != -1);
	if (used == -1)
	{
		return -1;
	}
	p += used;
	left -= used;

	//英雄装备信息
	{
		::google::protobuf::RepeatedPtrField< ::db_user_hero_equip >::const_iterator it_in = hero.equips().begin();
		for ( ; it_in != hero.equips().end() ; ++it_in)
		{
			used = sprintf_s(p, left, 
				" INSERT INTO `user_hero_equip` "
				" SET `user_id` = %u, `hero_id` = %u, `equip_id` = %u, `level` = %u ;",
				user_id, hero.hero_id(), it_in->equip_id(), it_in->level()
				);

			ASSERT(used != -1);
			if (used == -1)
			{
				return -1;
			}
			p += used;
			left -= used;
		}
	}

	//英雄技能信息
	{
		//::google::protobuf::RepeatedPtrField< ::db_user_hero_skill >::const_iterator it_in = hero->skills().begin();
		//for ( ; it_in != hero->skills().end() ; ++it_in)
		//{

		//}

		ASSERT(hero.skills().size() == 5);//五个技能

		UINT32 bisha_level = hero.skills(0).level();
		UINT32 aoyi_level = hero.skills(1).level();
		UINT32 passive1_level = hero.skills(2).level();
		UINT32 passive2_level = hero.skills(3).level();
		UINT32 passive3_level = hero.skills(4).level();

		used = sprintf_s(p, left, 
			" INSERT INTO `user_hero_skill` "
			" SET `user_id` = %u, `hero_id` = %u, `bisha_level` = %u, `aoyi_level` = %u, `passive1_level` = %u, `passive2_level` = %u, `passive3_level` = %u ;",
			user_id, hero.hero_id(), bisha_level, aoyi_level, passive1_level, passive2_level, passive3_level
			);

		ASSERT(used != -1);
		if (used == -1)
		{
			return -1;
		}
		p += used;
		left -= used;
	}

	return 0;
}

int db_sync_operator::new_create_user( db_user_info* user_info )
{
	ASSERT(user_info);
	MYSQL* con = CDBMgr::GetInstance()->GetConnection();
	ASSERT(con);
	
	std::string sql;
	int r = _get_sql_of_new_create_user(sql, user_info);
	if (r != 0){
		ASSERT(false);
		return r;
	}
	
	//int code = mysql_real_query(con, sql, (unsigned int)strlen(sql));
	int code = CDBMgr::Query(con, sql);
	if (0 != code){   //非0查询失败  
		printf("query failed! [%s] [%d] [%s] \n", sql.c_str(), mysql_errno(con), mysql_error(con));
		return -1;
	}

	printf("成功创建新用户数据[%u]到DB中！\n", user_info->user_base_info().user_id());
	return 0;
}

int db_sync_operator::_get_sql_of_new_create_user(std::string& strAppendHere, db_user_info* user_info )
{
	ASSERT(user_info);
	char sql[1024*10] = {0};
	char name[1024] = {0};

	char *p = sql;
	int left = sizeof sql;
	int used = 0;

	const db_user_base_info& user_base_info = user_info->user_base_info();

	MYSQL* con = CDBMgr::GetInstance()->GetConnection();
	ASSERT(con);
	mysql_real_escape_string(con, name, user_base_info.name().c_str(), user_base_info.name().size());

	used = sprintf_s(sql, left, " INSERT INTO `user` SET"
		"`user_id` = %u, "
		"`name` = '%s', "
		"`level` = %u, "
		"`exp` = %u, "
		"`vip` = %u, "
		"`diamond` = %u, "
		"`gold` = %u, "
		"`health_cur` = %u, "
		"`health_last_update_timestamp` = %u, "
		"`skill_point` = %u, "
		"`skill_point_update_timestamp` = %u ,"
		"`invoke_gold_free_left_num` = %u, "
		"`invoke_gold_free_update_timestamp` = %u, "
		"`invoke_gold_n_total_num` = %u, "
		"`invoke_diamond_free_left_num` = %u, "
		"`invoke_diamond_free_update_timestamp` = %u, "
		"`invoke_diamond_n_total_num` = %u ,"
		"`head_id` = %u,"
		"`head_frame_id` = %u,"
		"`friend_point` = %u,"
		"`signature` = '%s',"
		"`fight_power` = %u,"
		"`crusade_point` = %u;"
		,
		user_base_info.user_id(),
		name,
		user_base_info.level(),
		user_base_info.exp(),
		user_base_info.vip(),
		user_base_info.diamond(),
		user_base_info.gold(),
		user_base_info.health_cur(),
		user_base_info.health_last_update_timestamp(),
		user_base_info.skill_point(),
		user_base_info.skill_point_last_update_timestamp(),
		user_base_info.invoke_gold_free_left_num(),
		user_base_info.invoke_gold_free_update_timestamp(),
		user_base_info.invoke_gold_n_total_num(),
		user_base_info.invoke_diamond_free_left_num(),
		user_base_info.invoke_diamond_free_update_timestamp(),
		user_base_info.invoke_diamond_n_total_num(),
		user_base_info.head_id(),
		user_base_info.head_frame_id(),
		user_base_info.friend_point(),
		user_base_info.signature().c_str(),
		user_base_info.fight_power(),
		user_base_info.crusade_point()
		);

	ASSERT(used != -1);
	if (used == -1){
		return -1;
	}
	p += used;
	left -= used;

	//todo 添加其他部分的信息

	//英雄
	for (::google::protobuf::RepeatedPtrField< ::db_user_hero >::const_iterator it = user_info->heros().items().begin() ; it != user_info->heros().items().end() ; ++it){
		int r = _new_hero(p, left, user_base_info.user_id(), *it);
		ASSERT(r == 0);
		if ( r == -1){
			return -1;
		}
	}
	//道具
	for (::google::protobuf::RepeatedPtrField< ::struct_game_element >::const_iterator it = user_info->items().items().begin() ; it != user_info->items().items().end() ; ++it){
		int r = _add_or_update_game_element(p, left, user_base_info.user_id(), *it);
		ASSERT(r == 0);
		if ( r == -1){
			return -1;
		}
	}

	//邮件 todo!!


	//各个商店信息
	for (::google::protobuf::RepeatedPtrField< ::db_user_shop >::const_iterator it = user_info->shops().items().begin() ; it != user_info->shops().items().end() ; ++it){
		int r = _add_or_update_shop(p, left, user_base_info.user_id(), *it, gs_dbs_user_info_op_req::struct_op_shop::CREATE );
		ASSERT(r == 0);
		if ( r == -1){
			return -1;
		}
	}

	//购物记录信息
	for (::google::protobuf::RepeatedPtrField< ::db_user_shop_buy >::const_iterator it = user_info->buy_items().items().begin() ; it != user_info->buy_items().items().end() ; ++it){
		int r = _add_or_update_shop_buy(p, left, user_base_info.user_id(), *it);
		ASSERT(r == 0);
		if ( r == -1){
			return -1;
		}
	}

	strAppendHere.append(sql);
	return 0;
}

int db_sync_operator::_get_sql_of_update_base_info(std::string& strAppendHere, UINT32 user_id, gs_dbs_user_info_op_req_struct_op_base_info* user_info )
{
	ASSERT(user_info);
	const gs_dbs_user_info_op_req_struct_op_base_info& user_info_ref = *user_info;

	char sql[1024*10] = {0};

	char* p = sql;
	UINT32 left = sizeof(sql);

	int used = sprintf_s(p, left, " UPDATE `user` SET ");
	if (used == -1)
	{
		return -1;
	}
	p += used;
	left -= used;

	UINT32 left_init = left;

	SQL_APPEND_FIELD_S(user_info_ref, name);
	SQL_APPEND_FIELD_U(user_info_ref, level);
	SQL_APPEND_FIELD_U(user_info_ref, exp);
	SQL_APPEND_FIELD_U(user_info_ref, vip);
	SQL_APPEND_FIELD_U(user_info_ref, diamond);
	SQL_APPEND_FIELD_U(user_info_ref, gold);
	SQL_APPEND_FIELD_U(user_info_ref, health_cur);
	SQL_APPEND_FIELD_U(user_info_ref, health_last_update_timestamp);
	SQL_APPEND_FIELD_U(user_info_ref, skill_point);
	SQL_APPEND_FIELD_U(user_info_ref, skill_point_update_timestamp);
	SQL_APPEND_FIELD_U(user_info_ref, invoke_gold_free_left_num);
	SQL_APPEND_FIELD_U(user_info_ref, invoke_gold_free_update_timestamp);
	SQL_APPEND_FIELD_U(user_info_ref, invoke_gold_n_total_num);
	SQL_APPEND_FIELD_U(user_info_ref, invoke_diamond_free_left_num);
	SQL_APPEND_FIELD_U(user_info_ref, invoke_diamond_free_update_timestamp);
	SQL_APPEND_FIELD_U(user_info_ref, invoke_diamond_n_total_num);
	SQL_APPEND_FIELD_U(user_info_ref, head_id);
	SQL_APPEND_FIELD_U(user_info_ref, head_frame_id);
	SQL_APPEND_FIELD_U(user_info_ref, friend_point);
	SQL_APPEND_FIELD_S(user_info_ref, signature);
	SQL_APPEND_FIELD_U(user_info_ref, fight_power);
	SQL_APPEND_FIELD_U(user_info_ref, crusade_point);
	//没有修改的字段！
	if (left == left_init){
		ASSERT(false);
		return -1;
	}
	
	//去掉尾巴的,
	*(p-1) = ' ';
	used = sprintf_s(p, left, " WHERE `user_id`= %u;", user_id);
	if (used == -1){
		return -1;
	}

	strAppendHere.append(sql);

	return 0;
}

int db_sync_operator::update_base_info(UINT32 user_id, gs_dbs_user_info_op_req_struct_op_base_info* user_info )
{
	ASSERT(user_info);
	MYSQL* con = CDBMgr::GetInstance()->GetConnection();
	ASSERT(con);

	std::string sql;
	int r = _get_sql_of_update_base_info(sql, user_id, user_info);
	if (r != 0){
		return r;
	}

	int code = CDBMgr::Query(con, sql);
	if (0 != code){   //非0查询失败  
		printf("query failed! [%s] [%d] [%s] \n", sql.c_str(), mysql_errno(con), mysql_error(con));
		return -1;
	}
	printf("成功更新用户[%u]数据【%s】到DB中！\n", user_id, user_info->DebugString().c_str());
	return 0;
}

int db_sync_operator::_get_sql_of_new_hero(std::string& strAppendHere, UINT32 user_id, const ::google::protobuf::RepeatedPtrField< ::gs_dbs_user_info_op_req_struct_new_hero >& op_heros )
{
	if (op_heros.size() <= 0){
		return 0;
	}

	char sql[1024*10] = {0};
	int left = sizeof(sql);
	char* p = sql;
	::google::protobuf::RepeatedPtrField< ::gs_dbs_user_info_op_req_struct_new_hero >::const_iterator it = op_heros.begin();
	for ( ; it != op_heros.end() ; ++it){
		const db_user_hero& hero = it->hero();

		//英雄 只有Add操作的时候需要同时写入数据库 装备和技能信息
		//其他时候只需要写入英雄基本信息 装备和技能信息的更新下面有独立的操作
		//英雄基本信息
		int used = sprintf_s(p, left, 
			" INSERT INTO `user_hero` "
			" SET `user_id` = %u, `hero_id` = %u, `star` = %u, `quality` = %u, `level` = %u, `cur_exp` = %u, `got_timestamp` = %u ;",
			user_id, hero.hero_id(), hero.star(), hero.quality(), hero.level(), hero.cur_exp(), hero.got_timestamp()
			);

		ASSERT(used != -1);
		if (used == -1){
			return -1;
		}
		p += used;
		left -= used;

		//英雄装备信息
		{
			::google::protobuf::RepeatedPtrField< ::db_user_hero_equip >::const_iterator it_in = hero.equips().begin();
			for ( ; it_in != hero.equips().end() ; ++it_in){
				used = sprintf_s(p, left, 
					" INSERT INTO `user_hero_equip` "
					" SET `user_id` = %u, `hero_id` = %u, `equip_id` = %u, `level` = %u ;",
					user_id, hero.hero_id(), it_in->equip_id(), it_in->level()
					);

				ASSERT(used != -1);
				if (used == -1){
					return -1;
				}
				p += used;
				left -= used;
			}
		}

		//英雄技能信息
		{
			ASSERT(hero.skills().size() == 5);//五个技能

			UINT32 bisha_level = hero.skills(0).level();
			UINT32 aoyi_level = hero.skills(1).level();
			UINT32 passive1_level = hero.skills(2).level();
			UINT32 passive2_level = hero.skills(3).level();
			UINT32 passive3_level = hero.skills(4).level();

			used = sprintf_s(p, left, 
				" INSERT INTO `user_hero_skill` "
				" SET `user_id` = %u, `hero_id` = %u, `bisha_level` = %u, `aoyi_level` = %u, `passive1_level` = %u, `passive2_level` = %u, `passive3_level` = %u ;",
				user_id, hero.hero_id(), bisha_level, aoyi_level, passive1_level, passive2_level, passive3_level
				);

			ASSERT(used != -1);
			if (used == -1){
				return -1;
			}
			p += used;
			left -= used;
		}
	}

	strAppendHere.append(sql);
	return 0;
}

int db_sync_operator::new_hero( UINT32 user_id, const ::google::protobuf::RepeatedPtrField< ::gs_dbs_user_info_op_req_struct_new_hero >& op_heros )
{
	if (op_heros.size() <= 0){
		return 0;
	}

	MYSQL* con = CDBMgr::GetInstance()->GetConnection();
	ASSERT(con);
	
	std::string sql;
	int r = _get_sql_of_new_hero(sql, user_id, op_heros);
	if (r != 0){
		return r;
	}

	//int code = mysql_real_query(con, sql, (unsigned int)strlen(sql));
	int code = CDBMgr::Query(con, sql);
	if (0 != code)   //非0查询失败  
	{
		printf("query failed! [%s] [%d] [%s] \n", sql.c_str(), mysql_errno(con), mysql_error(con));
		return -1;
	}

	//printf("成功更新用户[%u]数据【%s】到DB中！\n", user_id, op_heros.DebugString().c_str());

	return 0;
}

int db_sync_operator::_get_sql_of_op_hero_base(std::string& strAppendHere, UINT32 user_id, const ::google::protobuf::RepeatedPtrField< ::gs_dbs_user_info_op_req_struct_op_hero_base >& op_base_heros )
{
	if (op_base_heros.size() <= 0){
		return 0;
	}

	char sql[1024*10] = {0};
	int left = sizeof(sql);
	char* p = sql;
	::google::protobuf::RepeatedPtrField< ::gs_dbs_user_info_op_req_struct_op_hero_base >::const_iterator it = op_base_heros.begin();
	for ( ; it != op_base_heros.end() ; ++it){
		if (it->op_type() == gs_dbs_user_info_op_req::Update){
			//验证是否有数据要修改
			if (!it->has_star() 
				&& !it->has_quality()
				&& !it->has_level()
				&& !it->has_cur_exp()
				){
				printf("op_hero_base data error!没有任何要更新的!! 用户[%u]数据【%s】！\n", user_id, it->DebugString().c_str());
				continue;
			}

			int used = sprintf_s(p, left, " UPDATE `user_hero` SET ");
			if (used == -1){
				return -1;
			}
			p += used;
			left -= used;

			const gs_dbs_user_info_op_req::struct_op_hero_base& ref = *it;
			SQL_APPEND_FIELD_U(ref, star);
			SQL_APPEND_FIELD_U(ref, quality);
			SQL_APPEND_FIELD_U(ref, level);
			SQL_APPEND_FIELD_U(ref, cur_exp);

			//去掉尾巴的,
			*(p-1) = ' ';
			used = sprintf_s(p, left, " WHERE `user_id`= %u AND `hero_id`=%u ; ", user_id, it->hero_id());
			if (used == -1){
				return -1;
			}

			p += used;
			left -= used;
		}
		else if (it->op_type() == gs_dbs_user_info_op_req::Delete){
			int used = sprintf_s(p, left, 
				" DELETE FROM `user_hero` WHERE `user_id` = %u AND `hero_id` = %u ;"
				" DELETE FROM `user_hero_equip` WHERE `user_id` = %u AND `hero_id` = %u ;"
				" DELETE FROM `user_hero_skill` WHERE `user_id` = %u AND `hero_id` = %u ;",
				user_id, it->hero_id(),
				user_id, it->hero_id(),
				user_id, it->hero_id()
				);

			ASSERT(used != -1);
			if (used == -1){
				return -1;
			}
			p += used;
			left -= used;
		}
		else{
			//ASSERT(false);
			//return -1;
			printf("op_hero_base data error!不支持的操作类型!! 用户[%u]数据【%s】！\n", user_id, it->DebugString().c_str());
			continue;
		}
	}

	strAppendHere.append(sql);
	return 0;
}


int db_sync_operator::op_hero_base( UINT32 user_id, const ::google::protobuf::RepeatedPtrField< ::gs_dbs_user_info_op_req_struct_op_hero_base >& op_base_heros )
{
	if (op_base_heros.size() <= 0){
		return 0;
	}

	MYSQL* con = CDBMgr::GetInstance()->GetConnection();
	ASSERT(con);

	std::string sql;
	int r = _get_sql_of_op_hero_base(sql, user_id, op_base_heros);
	if (r != 0){
		return r;
	}
	//int code = mysql_real_query(con, sql, (unsigned int)strlen(sql));
	int code = CDBMgr::Query(con, sql);
	if (0 != code){   //非0查询失败  
		printf("query failed! [%s] [%d] [%s] \n", sql.c_str(), mysql_errno(con), mysql_error(con));
		return -1;
	}
	//printf("成功更新用户[%u]数据【%s】到DB中！\n", user_id, it->DebugString().c_str());
	return 0;
}

int db_sync_operator::_get_sql_of_op_hero_equip(std::string& strAppendHere, UINT32 user_id, const ::google::protobuf::RepeatedPtrField< ::gs_dbs_user_info_op_req_struct_op_hero_equip >& op_hero_equips )
{
	if (op_hero_equips.size() <= 0){
		return 0;
	}

	char sql[1024*10] = {0};
	int left = sizeof(sql);
	char* p = sql;
	int used = 0;

	::google::protobuf::RepeatedPtrField< ::gs_dbs_user_info_op_req_struct_op_hero_equip >::const_iterator it = op_hero_equips.begin();
	for ( ; it != op_hero_equips.end() ; ++it){
		if (it->op_type() == gs_dbs_user_info_op_req::Update){
			if (!it->has_level()
				&& !it->has_evolution_equip_id()){
				printf("op_hero_equip data error!没有任何要更新的!! 用户[%u]数据【%s】！\n", user_id, it->DebugString().c_str());
				continue;
			}

			used = sprintf_s(p, left, " UPDATE `user_hero_equip` SET ");
			if (used == -1){
				return -1;
			}
			p += used;
			left -= used;

			const gs_dbs_user_info_op_req::struct_op_hero_equip& ref = *it;
			SQL_APPEND_FIELD_U(ref, level);

			if(it->has_evolution_equip_id()){
				used = sprintf_s(p, left, " `equip_id`=%u,", it->evolution_equip_id());
				if (used == -1){
					return -1;
				}
				p += used;
				left -= used;
			}

			//去掉尾巴的,
			*(p-1) = ' ';
			used = sprintf_s(p, left, " WHERE `user_id`= %u AND `hero_id`=%u AND `equip_id`=%u;", user_id, it->hero_id(), it->equip_id());
			if (used == -1){
				return -1;
			}
			p += used;
			left -= used;
		}
		//else{//没有其他操作
		//}
	}

	strAppendHere.append(sql);
	return 0;
}

int db_sync_operator::op_hero_equip( UINT32 user_id, const ::google::protobuf::RepeatedPtrField< ::gs_dbs_user_info_op_req_struct_op_hero_equip >& op_hero_equips )
{
	if (op_hero_equips.size() <= 0){
		return 0;
	}

	MYSQL* con = CDBMgr::GetInstance()->GetConnection();
	ASSERT(con);
	
	std::string sql;
	int r = _get_sql_of_op_hero_equip(sql, user_id, op_hero_equips);
	if (r != 0){
		return r;
	}

	//int code = mysql_real_query(con, sql, (unsigned int)strlen(sql));
	int code = CDBMgr::Query(con, sql);
	if (0 != code){   //非0查询失败  
		printf("query failed! [%s] [%d] [%s] \n", sql.c_str(), mysql_errno(con), mysql_error(con));
		return -1;
	}
	//printf("成功更新用户[%u]数据【%s】到DB中！\n", user_id, it->DebugString().c_str());
	return 0;
}

int db_sync_operator::_get_sql_of_op_hero_skill(std::string& strAppendHere, UINT32 user_id, const ::google::protobuf::RepeatedPtrField< ::gs_dbs_user_info_op_req_struct_op_hero_skill >& op_hero_skills )
{
	if (op_hero_skills.size() <= 0){
		return 0;
	}

	char sql[1024*10] = {0};
	int left = sizeof(sql);
	char* p = sql;
	int used = 0;

	::google::protobuf::RepeatedPtrField< ::gs_dbs_user_info_op_req_struct_op_hero_skill >::const_iterator it = op_hero_skills.begin();
	for ( ; it != op_hero_skills.end() ; ++it){
		if (it-> op_type() == gs_dbs_user_info_op_req::Update){
			if (!it->has_level()
				){
				printf("op_hero_skill data error!没有任何要更新的!! 用户[%u]数据【%s】！\n", user_id, it->DebugString().c_str());
				continue;
			}

			used = sprintf_s(p, left, " UPDATE `user_hero_skill` SET ");
			if (used == -1){
				return -1;
			}
			p += used;
			left -= used;

			std::string field;
			switch (it->type())
			{
			case user_hero_skill::BISHA:
				field = "bisha_level";
				break;
			case user_hero_skill::AOYI:
				field = "aoyi_level";
				break;
			case user_hero_skill::PASSIVE1:
				field = "passive1_level";
				break;
			case user_hero_skill::PASSIVE2:
				field = "passive2_level";
				break;
			case user_hero_skill::PASSIVE3:
				field = "passive3_level";
				break;
			default:{
				return -1;
				//printf()
					}
			}

			used = sprintf_s(p, left, " `%s`=%u,", field.c_str(), it->level());
			if (used == -1){
				return -1;
			}
			p += used;
			left -= used;

			//去掉尾巴的,
			*(p-1) = ' ';
			used = sprintf_s(p, left, " WHERE `user_id`= %u AND `hero_id`=%u ;", user_id, it->hero_id());
			if (used == -1){
				return -1;
			}

			p += used;
			left -= used;
		}
		//else{//没有其他操作
		//}
	}

	strAppendHere.append(sql);
	return 0;
}	

int db_sync_operator::op_hero_skill( UINT32 user_id, const ::google::protobuf::RepeatedPtrField< ::gs_dbs_user_info_op_req_struct_op_hero_skill >& op_hero_skills )
{
	if (op_hero_skills.size() <= 0){
		return 0;
	}

	MYSQL* con = CDBMgr::GetInstance()->GetConnection();
	ASSERT(con);
	std::string sql;
	int r = _get_sql_of_op_hero_skill(sql, user_id, op_hero_skills);
	if (r != 0){
		return r;
	}

	//int code = mysql_real_query(con, sql, (unsigned int)strlen(sql));
	int code = CDBMgr::Query(con, sql);
	if (0 != code)   //非0查询失败  
	{
		printf("query failed! [%s] [%d] [%s] \n", sql.c_str(), mysql_errno(con), mysql_error(con));
		return -1;
	}
	//printf("成功更新用户[%u]数据【%s】到DB中！\n", user_id, it->DebugString().c_str());
	return 0;
}	

int db_sync_operator::_get_sql_of_op_mail(std::string& strAppendHere, UINT32 user_id, const ::google::protobuf::RepeatedPtrField< ::gs_dbs_user_info_op_req_struct_op_mail >& rep_op_mail)
{
	::google::protobuf::RepeatedPtrField< ::gs_dbs_user_info_op_req_struct_op_mail >::const_iterator iter = rep_op_mail.begin();
	std::string strSql;
	std::string strSqlUpdate;

	char sql[1024*10] = {0};
	UINT32 iSysNum = 0;
	for (; iter != rep_op_mail.end(); ++iter){
		if(iter->op_type() == gs_dbs_user_info_op_req::Add){
			std::string strSqlAdd;
			const db_user_mail& db_mail = (*iter).mail();
			if (db_mail.has_content()){
				char* pContent = UTF8ToANSI(db_mail.content().c_str());
				sprintf_s(sql, sizeof(sql), 
					"INSERT INTO `user_mail` SET `mail_id` = %u, `user_id` = %u, `send_time` = from_unixtime(%u), `content` = '%s', `status` = %u, `type` = %u;",
					db_mail.mail_id(), user_id, db_mail.send_time(), 
					pContent, db_mail.status(), db_mail.type());

				free(pContent);
			}
			else{
				sprintf_s(sql, sizeof(sql), 
					"INSERT INTO `user_mail` SET `mail_id` = %u, `user_id` = %u, `send_time` = from_unixtime(%u), `status` = %u, `type` = %u;",
					db_mail.mail_id(), user_id, db_mail.send_time(), 
					db_mail.status(), db_mail.type());
			}

			strSqlAdd += sql;
			if (db_mail.attachs_size())
			{
				strSqlAdd += "INSERT INTO `user_mail_attach` VALUES";
				::google::protobuf::RepeatedPtrField< ::db_mail_attach >::const_iterator iter = db_mail.attachs().begin();
				for (; iter != db_mail.attachs().end(); ++iter){

					sprintf_s(sql, sizeof(sql), "(%d, %d, %d, %d, %d, %d),", 
						iter->attach_id(), iter->element().type(), iter->element().id(), iter->element().num(), db_mail.mail_id(), user_id);
					strSqlAdd += sql;
				}
				strSqlAdd.erase(strSqlAdd.rfind(','));
			}
			char* pSql = ANSIToUTF8(strSqlAdd.c_str());
			strSql += pSql + std::string(";");
			free(pSql);
		}
		else if(iter->op_type() == gs_dbs_user_info_op_req::Update){
			sprintf_s(sql, sizeof(sql), "%d,", iter->mail_id());
			strSqlUpdate += sql;
		}
		else if(iter->op_type() == gs_dbs_user_info_op_req::Delete){
			sprintf_s(sql, sizeof(sql), 
				"DELETE FROM `user_mail` WHERE `mail_id` = %d;"
				"DELETE FROM `user_mail_attach` WHERE `mail_id` = %d;", iter->mail_id(), iter->mail_id());
			strSql += sql;
		}
	}

	if (strSqlUpdate.size()){
		strSqlUpdate.erase(strSqlUpdate.rfind(','));
		sprintf_s(sql, sizeof(sql), "UPDATE `user_mail` SET `status` = %d WHERE `mail_id` IN(%s);", MAIL_STATUS_READED, strSqlUpdate.c_str());
		strSql += sql;
	}

	strAppendHere.append(strSql);
	return 0;
}

int db_sync_operator::op_mail(UINT32 user_id, const ::google::protobuf::RepeatedPtrField< ::gs_dbs_user_info_op_req_struct_op_mail >& rep_op_mail)
{
	MYSQL* con = CDBMgr::GetInstance()->GetConnection();
	ASSERT(con);

	std::string sql;
	int r = _get_sql_of_op_mail(sql, user_id, rep_op_mail);
	if (r != 0){
		return r;
	}

	int code = CDBMgr::GetInstance()->Query(con, sql);
	if (0 != code){
		printf("query failed! [%s] [%d] [%s] \n", sql.c_str(), mysql_errno(con), mysql_error(con));
		return -1;
	}

	return 0;
}


int db_sync_operator::_add_or_update_game_element(char* &p, int& left, UINT32 user_id, const struct_game_element& ele)
{
	//num_type , user_id, item_id, num
	int used = sprintf_s(p, left, "CALL set_user_item(%d, %d, %d, %d) ;", ele.num_type(), user_id, ele.id(), ele.num());
	if (used == -1){
		return -1;
	}

	p += used;
	left -= used;
	return 0;
}

int db_sync_operator::_add_or_update_shop_item(char* &p, int& left, UINT32 user_id, UINT32 shop_type, const db_user_shop_item& ele)
{
	//num_type , user_id, item_id, num
	int used = sprintf_s(p, left, "CALL add_or_update_shop_item(%d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d);", 
		user_id, 
		shop_type, 
		ele.pos(), 
		ele.element_type(), 
		ele.element_id(), 
		ele.element_num(), 
		ele.consume_type(), 
		ele.consume_id(), 
		ele.consume_num(), 
		ele.flag(), 
		ele.is_sold()
		);
	if (used == -1)
	{
		return -1;
	}

	p += used;
	left -= used;
	return 0;
}


int db_sync_operator::_add_or_update_shop( char* &p, int& left, UINT32 user_id, const db_user_shop& shop, gs_dbs_user_info_op_req::struct_op_shop::ESHOPOPType op_type)
{
	//num_type , user_id, item_id, num
	int used = sprintf_s(p, left, "INSERT INTO `user_shop` SET "
		"`user_id` = %u, "
		"`shop_type` = %u, "
		"`last_auto_refresh_time` = %u, "
		"`last_manual_refresh_time` = %u, "
		"`last_day_manual_refresh_num` = %u, "
		"`total_manual_refresh_num` = 0, "
		"`random_open_start_timestamp` = %u, "
		"`random_open_end_timestamp` = %u "
		"ON DUPLICATE KEY UPDATE "
		"`last_auto_refresh_time` = %u, "
		"`last_manual_refresh_time` = %u, "
		"`last_day_manual_refresh_num` = %u, "
		"`total_manual_refresh_num` = `total_manual_refresh_num` + %u, "
		"`random_open_start_timestamp` = %u, "
		"`random_open_end_timestamp` = %u ;"
		"DELETE FROM `user_shop_item` WHERE `user_id` = %u AND `shop_type` = %u AND pos >= %u;"//删掉旧商店多余的部分！
		,
		user_id, 
		shop.shop_type(), 
		shop.last_auto_refresh_time(),
		shop.last_manual_refresh_time(),
		shop.last_day_manual_refresh_num(),
		shop.random_open_start_timestamp(),
		shop.random_open_end_timestamp(),
		//----------update---------
		shop.last_auto_refresh_time(),
		shop.last_manual_refresh_time(),
		shop.last_day_manual_refresh_num(),
		(op_type == gs_dbs_user_info_op_req::struct_op_shop::MANUAL_REFRESH ? 1 : 0),
		shop.random_open_start_timestamp(),
		shop.random_open_end_timestamp(),
		user_id, shop.shop_type(), shop.items_size()
		);
	if (used == -1)
	{
		return -1;
	}

	p += used;
	left -= used;

	for (::google::protobuf::RepeatedPtrField< ::db_user_shop_item >::const_iterator it = shop.items().begin() ; it != shop.items().end() ; ++it)
	{
		int r = _add_or_update_shop_item(p, left, user_id, shop.shop_type(), *it);
		ASSERT(r == 0);
		if (r != 0){
			return -1;
		}
	}

	return 0;
}

int db_sync_operator::_add_or_update_shop_buy(char* &p, int& left, UINT32 user_id, const db_user_shop_buy& ele)
{
	//num_type , user_id, item_id, num
	int used = sprintf_s(p, left, "CALL add_or_update_shop_buy(%d, %d, %d, %d, %d);", user_id, ele.element_type(), ele.element_id(), ele.last_day_buy_num(), ele.last_buy_time());
	if (used == -1)
	{
		return -1;
	}

	p += used;
	left -= used;
	return 0;
}

int db_sync_operator::add_or_update_game_element(UINT32 user_id, const ::google::protobuf::RepeatedPtrField< ::struct_game_element >& eles)
{
	MYSQL* con = CDBMgr::GetInstance()->GetConnection();
	ASSERT(con);

	std::string sql;
	int r = _get_sql_of_add_or_update_game_element(sql, user_id, eles);
	if (r != 0){
		ASSERT(false);
		return r;
	}

	int code = CDBMgr::Query(con, sql);
	if (0 != code){   //非0查询失败  
		printf("query failed! [%s] [%d] [%s] \n", sql.c_str(), mysql_errno(con), mysql_error(con));
		return -1;
	}

	MYSQL_RES* res = mysql_store_result( con );	
	if(res){
		MYSQL_ROW row;
		if(row = mysql_fetch_row(res)){
			if(-1 == atoi(row[0])){
				printf("warning! user_item 中不存在 user_id [%d] 记录\n", user_id);
			}
			else if(-2 == atoi(row[0])){
				printf("warning! user_item中user_id [%d] 数量不足\n", user_id);
			}
		}
		mysql_free_result(res);
	}

	return 0;
}

int db_sync_operator::_get_sql_of_add_or_update_game_element(std::string& strAppendHere, UINT32 user_id, const ::google::protobuf::RepeatedPtrField< ::struct_game_element >& eles)
{
	char sql[1024*10] = {0};
	int left = sizeof(sql);
	char* p = sql;
	int used = 0;

	::google::protobuf::RepeatedPtrField< ::struct_game_element >::const_iterator iter = eles.begin();
	for (; iter != eles.end(); ++iter){
		int r = _add_or_update_game_element(p, left, user_id, *iter);
		ASSERT(r == 0);
		if (r != 0){
			return -1;
		}
	}

	strAppendHere.append(sql);
	return 0;
}

int db_sync_operator::add_or_update_game_element(UINT32 user_id, const struct_game_element& ele)
{
	//ASSERT(pGameEle);
	MYSQL* con = CDBMgr::GetInstance()->GetConnection();
	ASSERT(con);
	
	char sql[1024*10] = {0};
	//num_type , user_id, item_id, num
	//sprintf_s(sql, sizeof(sql), "CALL set_user_item(%d, %d, %d, %d)", pGameEle->num_type(), user_id, pGameEle->id(), pGameEle->num());
	int left = sizeof(sql);
	char* p = sql;
	int used = 0;
	int r = _add_or_update_game_element(p, left, user_id, ele);
	ASSERT(r  == 0);

	int code = CDBMgr::Query(con, sql, (unsigned int)strlen(sql));
	if (0 != code){   //非0查询失败  
		printf("query failed! [%s] [%d] [%s] \n", sql, mysql_errno(con), mysql_error(con));
		return -1;
	}
	MYSQL_RES* res = mysql_store_result( con );	
	if(res)
	{
		MYSQL_ROW row;
		if(row = mysql_fetch_row(res))
		{
			if(-1 == atoi(row[0]))
			{
				printf("warning! user_item 中不存在 user_id [%d] item_id [%d] 记录\n", user_id, ele.id());
			}
			else if(-2 == atoi(row[0]))
			{
				printf("warning! user_item中user_id [%d] item_id [%d] 数量不足\n", user_id, ele.id());
			}
		}
		mysql_free_result(res);
	}

	return 0;
}


int db_sync_operator::op_user_new_mail( stAddMail* new_mail)
{
	MYSQL* con = CDBMgr::GetInstance()->GetConnection();
	ASSERT(con);
	

	//将在线的插入到user_mail中(注：给指定玩家发送邮件直接插入到user_mail中，给所有玩家发送邮件如果在线也会直接插入到user_mail中)
	std::string strSql;
	::google::protobuf::RepeatedField< ::google::protobuf::uint32 >::iterator it_user = new_mail->user_ids.begin();
	::google::protobuf::RepeatedField< ::google::protobuf::uint32 >::iterator it_mail_id = new_mail->mail_ids.begin();
	::google::protobuf::RepeatedField< ::google::protobuf::uint32 >::iterator it_attach_id = new_mail->mail_attach_ids.begin();
	std::string strMail;
	std::string strMailAttach;
	for (; it_user != new_mail->user_ids.end(); ++it_user)
	{
		for (; it_mail_id != new_mail->mail_ids.end();)
		{
			char bufMail[1024] = {0};
			sprintf_s(bufMail, sizeof(bufMail), 
				"(%d,%d,from_unixtime(%d),'%s',%d,%d),", 
				*it_mail_id, *it_user, new_mail->mail.send_time(), new_mail->mail.content().c_str(), new_mail->mail.status(), new_mail->mail.type());
			strMail += bufMail;
			for (;it_attach_id != new_mail->mail_attach_ids.end();)
			{
				if(new_mail->mail.attachs_size()){
					::google::protobuf::RepeatedPtrField< ::db_mail_attach >::iterator it_attach = new_mail->mail.mutable_attachs()->begin();
					for (; it_attach != new_mail->mail.mutable_attachs()->end(); ++it_attach)
					{
						char bufAttach[1024] = {0};
						sprintf_s(bufAttach, sizeof(bufAttach), "(%d,%d,%d,%d,%d,%d),",
							*it_attach_id, it_attach->mutable_element()->type(), it_attach->mutable_element()->id(), it_attach->mutable_element()->num(), *it_mail_id, *it_user);
							strMailAttach += bufAttach;
						it_attach_id++;
					}
				}
				break;
			}
			++it_mail_id;
			break;
		}
	}

	if (strMail.length()){
		strMail.erase(strMail.length() - 1);
		strSql += "INSERT `user_mail`  VALUES " + strMail + ";";
	}

	if (strMailAttach.length()){
		strMailAttach.erase(strMailAttach.length() - 1);
		strSql += "INSERT INTO `user_mail_attach`  VALUES " + strMailAttach + ";"; 
	}
	
	if(strSql.length()){
		int code = CDBMgr::Query(con, strSql);
		if (0 != code){   //非0查询失败  
		
			printf("query failed! [%s] [%d] [%s] \n", strSql.c_str(), mysql_errno(con), mysql_error(con));
			return -1;
		}
	}

	if(new_mail->all){//全服邮件
		//1.将邮件插入到全服邮件表中 user_sys_mail
		const db_user_mail* pMail = &new_mail->mail;
		ASSERT(pMail->has_content());//系统邮件肯定需要content

		char sql[1024 * 10] = {0};
		sprintf_s(sql, sizeof(sql), "INSERT INTO `user_sys_mail` SET  `content` = '%s', `type` = %d, `send_time` = from_unixtime(%u), `valid_time` = %u;SELECT  LAST_INSERT_ID();",
			pMail->content().c_str(),  pMail->type(), pMail->send_time(), Now());


		int code = CDBMgr::Query(con, sql, strlen(sql));
		if (0 != code)   //非0查询失败  
		{
			printf("query failed! [%s] [%d] [%s] \n", sql, mysql_errno(con), mysql_error(con));
			return -1;
		}
		//获取递增mailID
		UINT32 iMailID = -1;
		do 
		{
			MYSQL_RES* res = mysql_store_result(con);
			if (res)
			{
				MYSQL_ROW row;
				if (row = mysql_fetch_row(res))
				{
					iMailID = atoi(row[0]);
				}
				mysql_free_result(res);
			}
		}
		while(!mysql_next_result( con ));
		ASSERT(iMailID != -1);
		
		//2.插入到user_sys_mail_attach中
		std::string strSql;
		std::string strSqlAtt;
		if(new_mail->mail.attachs_size()){
			::google::protobuf::RepeatedPtrField< ::db_mail_attach >::iterator iter = new_mail->mail.mutable_attachs()->begin();
			for (; iter != new_mail->mail.mutable_attachs()->end(); ++iter)
			{
				memset(sql, 0, sizeof(sql));
				sprintf_s(sql, sizeof(sql), "(%d, %d, %d, %d),", iter->mutable_element()->type(),
					iter->mutable_element()->id(), iter->mutable_element()->num(), iMailID);
				strSqlAtt += sql;
			}
			if (strSqlAtt.length())
			{
				strSqlAtt.erase(strSqlAtt.length() - 1);
				strSql += std::string("INSERT INTO `user_sys_mail_attach`(`type`,`element_id`,`num`,`mail_id`) VALUES ") + strSqlAtt + ";";
			}
		}

		//3.根据玩家获取在线用户，将在线用户的全服邮件插入到user_received_sys_mail
		std::string strReceived;
		if (new_mail->user_ids.size())
		{
			//插入到user_received_sys_mail中
			for (size_t i = 0; i < new_mail->user_ids.size(); ++i)
			{
				sprintf_s(sql, sizeof(sql), "(%d, %d),", new_mail->user_ids.Get(i), iMailID);
				strReceived += sql;
			}
			strReceived.erase(strReceived.length() - 1);
			strSql += "INSERT INTO `user_received_sys_mail` VALUES" + strReceived + ";";
		}

		code = CDBMgr::Query(con, strSql);
		if (0 != code)   //非0查询失败  
		{
			printf("query failed! [%s] [%d] [%s] \n", strSql.c_str(), mysql_errno(con), mysql_error(con));
			return -1;
		}
	}
	return 0;
}

int db_sync_operator::add_or_update_shop( UINT32 user_id, const ::google::protobuf::RepeatedPtrField< gs_dbs_user_info_op_req::struct_op_shop >& eles )
{
	MYSQL* con = CDBMgr::GetInstance()->GetConnection();
	ASSERT(con);

	std::string sql;
	int r = _get_sql_of_add_or_update_shop(sql, user_id, eles);
	if (r != 0){
		ASSERT(false);
		return r;
	}

	int code = CDBMgr::Query(con, sql);
	if (0 != code){   //非0查询失败  
		printf("query failed! [%s] [%d] [%s] \n", sql.c_str(), mysql_errno(con), mysql_error(con));
		return -1;
	}
	return 0;
}

int db_sync_operator::_get_sql_of_add_or_update_shop(std::string& strAppendHere, UINT32 user_id, const ::google::protobuf::RepeatedPtrField< gs_dbs_user_info_op_req::struct_op_shop >& eles )
{
	char sql[1024*10] = {0};
	int left = sizeof(sql);
	char* p = sql;
	int used = 0;
	//int r = _add_or_update_game_element(p, left, user_id, ele);
	//ASSERT(r  == 0);

	for (::google::protobuf::RepeatedPtrField< gs_dbs_user_info_op_req::struct_op_shop >::const_iterator it = eles.begin() ; it != eles.end() ; ++it){
		int r = _add_or_update_shop(p, left, user_id, it->shop(), it->shop_op_type());
		ASSERT(r == 0);
		if(r != 0){
			return -1;
		}
	}
	strAppendHere.append(sql);
	return 0;
}

int db_sync_operator::add_or_update_shop_item( UINT32 user_id, const ::google::protobuf::RepeatedPtrField< gs_dbs_user_info_op_req::struct_op_shop_item >& eles )
{
	//ASSERT(pGameEle);
	MYSQL* con = CDBMgr::GetInstance()->GetConnection();
	ASSERT(con);

	std::string sql;
	int r = _get_sql_of_add_or_update_shop_item(sql, user_id, eles);
	if (r != 0){
		ASSERT(false);
		return r;
	}

	int code = CDBMgr::Query(con, sql);
	if (0 != code){   //非0查询失败  
		printf("query failed! [%s] [%d] [%s] \n", sql.c_str(), mysql_errno(con), mysql_error(con));
		return -1;
	}
	return 0;
}


int db_sync_operator::_get_sql_of_add_or_update_shop_item(std::string& strAppendHere, UINT32 user_id, const ::google::protobuf::RepeatedPtrField< gs_dbs_user_info_op_req::struct_op_shop_item >& eles )
{
	char sql[1024*10] = {0};
	int left = sizeof(sql);
	char* p = sql;
	int used = 0;
	//int r = _add_or_update_game_element(p, left, user_id, ele);
	//ASSERT(r  == 0);

	for (::google::protobuf::RepeatedPtrField< gs_dbs_user_info_op_req::struct_op_shop_item >::const_iterator it = eles.begin() ; it != eles.end() ; ++it){
		int r = _add_or_update_shop_item(p, left, user_id, it->shop_type(), it->item());
		ASSERT(r == 0);
		if(r != 0){
			return -1;
		}
	}

	strAppendHere.append(sql);

	return 0;
}

int db_sync_operator::add_or_update_shop_buy(UINT32 user_id, const ::google::protobuf::RepeatedPtrField< ::db_user_shop_buy >& eles)
{
	//ASSERT(pGameEle);
	MYSQL* con = CDBMgr::GetInstance()->GetConnection();
	ASSERT(con);

	std::string sql;
	int r = _get_sql_of_add_or_update_shop_buy(sql, user_id, eles);
	if ( r != 0){
		ASSERT(false);
		return r;
	}

	int code = CDBMgr::Query(con, sql);
	if (0 != code){   //非0查询失败  
		printf("query failed! [%s] [%d] [%s] \n", sql.c_str(), mysql_errno(con), mysql_error(con));
		return -1;
	}
	return 0;
}

int db_sync_operator::_get_sql_of_add_or_update_shop_buy(std::string& strAppendHere, UINT32 user_id, const ::google::protobuf::RepeatedPtrField< ::db_user_shop_buy >& eles)
{
	char sql[1024*10] = {0};
	int left = sizeof(sql);
	char* p = sql;
	int used = 0;

	for (::google::protobuf::RepeatedPtrField< ::db_user_shop_buy >::const_iterator it = eles.begin() ; it != eles.end() ; ++it){
		int r = _add_or_update_shop_buy(p, left, user_id, *it);
		ASSERT(r == 0);
		if(r != 0){
			return -1;
		}
	}

	strAppendHere.append(sql);
	return 0;
}

int db_sync_operator::add_or_update_mission(UINT32 user_id, const ::google::protobuf::RepeatedPtrField< ::gs_dbs_user_info_op_req_struct_op_mission >& eles)
{
	MYSQL* con = CDBMgr::GetInstance()->GetConnection();
	ASSERT(con);

	std::string sql;
	_get_sql_of_add_or_update_mission(sql, user_id, eles);
	
	int code = CDBMgr::Query(con, sql);
	if (0 != code){
		printf("query failed! [%s] [%d] [%s] \n", sql.c_str(), mysql_errno(con), mysql_error(con));
		return -1;
	}
	return 0;
}

int db_sync_operator::_get_sql_of_add_or_update_mission(std::string& strAppendHere, UINT32 user_id, const ::google::protobuf::RepeatedPtrField< ::gs_dbs_user_info_op_req_struct_op_mission >& eles)
{
	char sql[1024 * 10] = {0};
	int left = sizeof(sql);
	char* p = sql;
	int used = 0;

	for (::google::protobuf::RepeatedPtrField< ::gs_dbs_user_info_op_req_struct_op_mission >::const_iterator it = eles.begin() ; it != eles.end() ; ++it){
		if((*it).has_mission_event()){
			const db_user_mission_event& mission_event = (*it).mission_event();

			used = sprintf_s(p, left, "INSERT INTO `user_mission_event` SET  `user_id` = %u, `mission_event_type` = %u, `mission_event_sub_type` = %u,",
				user_id, mission_event.mission_event_type(), mission_event.mission_event_sub_type(),  mission_event.last_update_timestamp());
			if (-1 == used){
				return -1;
			}
			p += used;
			left -= used;

			SQL_APPEND_FIELD_U(mission_event, last_update_timestamp);
			SQL_APPEND_FIELD_U(mission_event, last_day_value);
			SQL_APPEND_FIELD_U(mission_event, total_value);

			*(p - 1) = ' ';
			used = sprintf_s(p, left,  "ON DUPLICATE KEY UPDATE ");
			if (used == -1){
				return -1;
			}
			p += used;
			left -= used;

			SQL_APPEND_FIELD_U(mission_event, last_update_timestamp);
			SQL_APPEND_FIELD_U(mission_event, last_day_value);
			SQL_APPEND_FIELD_U(mission_event, total_value);

			*(p - 1) = ';';

		}
		if((*it).has_mission_chain()){
			const db_user_mission_chain& mission_chain = (*it).mission_chain();
			used = sprintf_s(p, left, "INSERT INTO `user_mission_chain` SET `user_id` = %u, `mission_chain_id` = %u, `last_get_reward_mission_id` = %u, `last_get_reward_timestamp` = %u ON DUPLICATE KEY UPDATE `last_get_reward_mission_id` = %u, `last_get_reward_timestamp` = %u;",
				user_id, mission_chain.mission_chain_id(), mission_chain.last_get_reward_mission_id(), mission_chain.last_get_reward_timestamp(), mission_chain.last_get_reward_mission_id(), mission_chain.last_get_reward_timestamp());
			if (-1 == used){
				return -1;
			}
			p += used;
			left -= used;
		}
	}
	strAppendHere.append(sql);
	return 0;
}

//竞技场相关
int db_sync_operator::add_arena(UINT32 user_id, const gs_dbs_user_info_op_req::struct_new_arena& new_arena ){
	MYSQL* con = CDBMgr::GetInstance()->GetConnection();
	ASSERT(con);

	ASSERT(new_arena.IsInitialized());
	if (!new_arena.IsInitialized()){
		return -1;
	}
	std::string sql;
	int r = _get_sql_of_add_arena(sql, user_id, new_arena);
	if (r != 0){
		ASSERT(false);
		return r;
	}

	int code = CDBMgr::Query(con, sql);
	if (0 != code){   //非0查询失败  
		printf("query failed! [%s] [%d] [%s] \n", sql.c_str(), mysql_errno(con), mysql_error(con));
		return -1;
	}
	return 0;
}

int db_sync_operator::_get_sql_of_add_arena(std::string& strAppendHere, UINT32 user_id, const gs_dbs_user_info_op_req::struct_new_arena& new_arena ){
	ASSERT(new_arena.IsInitialized());
	if (!new_arena.IsInitialized()){
		return -1;
	}

	char sql[1024*1] = {0};
	int left = sizeof(sql);
	char* p = sql;
	int used = 0;

	//num_type , user_id, item_id, num
	used = sprintf_s(p, left, " INSERT INTO `user_arena` SET "
		" `rank` = %u,"
		" `user_id` = %u,"
		" `score` = %u,"
		" `hero_id1` = %u,"
		" `hero_id2` = %u,"
		" `hero_id3` = %u,"
		" `hero_id4` = %u,"
		" `hero_id5` = %u,"
		" `last_challenge_time` = %u,"
		" `last_day_challenge_num` = %u,"
		" `history_highest_rank` = %u, "
		" `last_time_buy_arena_challenge` = %u, "
		" `last_day_buy_arena_challenge_num` = %u;"
		, new_arena.rank()
		, user_id
		, new_arena.score()
		, new_arena.heros_size() >= 1 ? new_arena.heros(0) : 0
		, new_arena.heros_size() >= 2 ? new_arena.heros(1) : 0
		, new_arena.heros_size() >= 3 ? new_arena.heros(2) : 0
		, new_arena.heros_size() >= 4 ? new_arena.heros(3) : 0
		, new_arena.heros_size() >= 5 ? new_arena.heros(4) : 0
		, new_arena.last_challenge_time()
		, new_arena.last_day_challenge_num()
		, new_arena.history_highest_rank()
		, new_arena.last_time_buy_arena_challenge()
		, new_arena.last_day_buy_arena_challenge_num()
		);

	if (used == -1){
		return -1;
	}
	p += used;
	left -= used;

	strAppendHere.append(sql);
	return 0;
}

//竞技场相关
int db_sync_operator::update_arena(UINT32 user_id, const gs_dbs_user_info_op_req::struct_op_arena& op_arena ){
	
	ASSERT(op_arena.heros_size() <= 5);
	if (op_arena.heros_size() > 5){
		return -1;
	}
	
	MYSQL* con = CDBMgr::GetInstance()->GetConnection();
	ASSERT(con);

	std::string sql;
	int r = _get_sql_of_update_arena(sql, user_id, op_arena);
	if (r != 0){
		ASSERT(false);
		return r;
	}
	
	//int code = mysql_real_query(con, sql, (unsigned int)strlen(sql));
	int code = CDBMgr::Query(con, sql);
	if (0 != code){  //非0查询失败
		printf("query failed! [%s] [%d] [%s] \n", sql.c_str(), mysql_errno(con), mysql_error(con));
		return -1;
	}

	printf("成功更新用户[%u]竞技场数据【%s】到DB中！\n", user_id, op_arena.DebugString().c_str());
	return 0;
}

//竞技场相关
int db_sync_operator::_get_sql_of_update_arena(std::string& strAppendHere, UINT32 user_id, const gs_dbs_user_info_op_req::struct_op_arena& op_arena ){

	ASSERT(op_arena.heros_size() <= 5);
	if (op_arena.heros_size() > 5){
		return -1;
	}

	char sql[1024*5] = {0};
	int left = sizeof(sql);
	char* p = sql;
	int used = 0;

	used = sprintf_s(p, left, " UPDATE `user_arena` SET ");
	if (used == -1){
		return -1;
	}

	p += used;
	left -= used;

	UINT32 left_init = left;

	SQL_APPEND_FIELD_U(op_arena, rank);
	SQL_APPEND_FIELD_U(op_arena, score);
	SQL_APPEND_FIELD_U(op_arena, last_challenge_time);
	SQL_APPEND_FIELD_U(op_arena, last_day_challenge_num);
	SQL_APPEND_FIELD_U(op_arena, history_highest_rank);
	SQL_APPEND_FIELD_U(op_arena, last_time_buy_arena_challenge);
	SQL_APPEND_FIELD_U(op_arena, last_day_buy_arena_challenge_num);

	//repeated uint32 heros = 7;
	if(op_arena.heros_size() > 0){//如果等于0的话 本来是不操作英雄的 结果全部都设置为0了！ 全部设置为0 使用独立的clear_arena_team请求
		for ( UINT32 i = 0 ; i < op_arena.heros_size(); ++i){
			used = sprintf_s(p, left, " `hero_id%u` = %u ,", i+1, op_arena.heros(i));
			if (used == -1){
				return -1;
			}
			p += used;
			left -= used;
		}

		UINT32 max_hero_num = 5;
		for (UINT32 i = op_arena.heros_size() ; i < max_hero_num ; ++i){
			used = sprintf_s(p, left, " `hero_id%u` = %u ,", i+1, 0);
			if (used == -1){
				return -1;
			}
			p += used;
			left -= used;
		}
	}

	//有修改字段的！
	if (left == left_init){
		ASSERT(false);
		return -1;
	}

	//去掉尾巴的,
	*(p-1) = ' ';
	used = sprintf_s(p, left, " WHERE `user_id`= %u;", user_id);
	if (used == -1){
		return -1;
	}

	strAppendHere.append(sql);
	return 0;
}

//此时竞技场信息一定存在 在逻辑层保证, 这里只作update
//int db_sync_operator::op_arena_team(UINT32 user_id, const gs_dbs_user_info_op_req::struct_op_arena_team& op_arena_team ){
//	MYSQL* con = CDBMgr::GetInstance()->GetConnection();
//	ASSERT(con);
//
//	char sql[1024*1] = {0};
//	int left = sizeof(sql);
//	char* p = sql;
//	int used = 0;
//
//	used = sprintf_s(p, left, " UPDATE `user_arena` SET "
//		" `hero_id1` = %u,"
//		" `hero_id2` = %u,"
//		" `hero_id3` = %u,"
//		" `hero_id4` = %u,"
//		" `hero_id5` = %u "
//		" WHERE `user_id` = %u;"
//		, op_arena_team.heros_size() >= 1 ? op_arena_team.heros(0) : 0
//		, op_arena_team.heros_size() >= 2 ? op_arena_team.heros(1) : 0
//		, op_arena_team.heros_size() >= 3 ? op_arena_team.heros(2) : 0
//		, op_arena_team.heros_size() >= 4 ? op_arena_team.heros(3) : 0
//		, op_arena_team.heros_size() >= 5 ? op_arena_team.heros(4) : 0
//		, user_id
//		);
//
//	if (used == -1){
//		return -1;
//	}
//
//	p += used;
//	left -= used;
//
//	int code = CDBMgr::Query(con, sql, (unsigned int)strlen(sql));
//	if (0 != code){   //非0查询失败  
//		printf("query failed! [%s] [%d] [%s] \n", sql, mysql_errno(con), mysql_error(con));
//		return -1;
//	}
//	return 0;
//}

//添加竞技场挑战结果 给内存双方添加日志 并只给被挑战者修改名次
//给db 添加一条日志 并只给被挑战者修改名次
int db_sync_operator::op_arena_challenge(UINT32 user_id, const gs_dbs_user_info_op_req::struct_op_arena_challenge& op_arena_challenge ){
	MYSQL* con = CDBMgr::GetInstance()->GetConnection();
	ASSERT(con);

	std::string sql;
	int r = _get_sql_of_op_arena_challenge(sql, user_id, op_arena_challenge);
	if (r != 0){
		ASSERT(false);
		return r;
	}

	int code = CDBMgr::Query(con, sql);
	if (0 != code){   //非0查询失败  
		printf("query failed! [%s] [%d] [%s] \n", sql.c_str(), mysql_errno(con), mysql_error(con));
		return -1;
	}
	return 0;
}

int db_sync_operator::_get_sql_of_op_arena_challenge(std::string& strAppendHere, UINT32 user_id, const gs_dbs_user_info_op_req::struct_op_arena_challenge& op_arena_challenge ){
	char sql[1024*10] = {0};
	int left = sizeof(sql);
	char* p = sql;
	int used = 0;

	//更新防守者的名次 一定存在数据库信息！并且此时还在榜单上 由上层逻辑保证
	if (op_arena_challenge.is_win()){
		int used = sprintf_s(p, left, " UPDATE `user_arena` SET "
			" `rank` = %u "
			" WHERE `user_id` = %u;"
			, op_arena_challenge.attacker_rank()//输的时候 交换排名 只更新防守者的 攻击者的在上面add_or_update_arena已经做了
			, op_arena_challenge.defender_user_id()
			);

		if (used == -1){
			return -1;
		}
		p += used;
		left -= used;
	}

	used = sprintf_s(p, left, " INSERT INTO `user_arena_log` SET "
		" `attacker_user_id` = %u , "
		" `attacker_rank` = %u , "
		" `attacker_power` = %u , "
		" `attacker_name` = '%s' , "
		" `attacker_level` = %u , "

		" `attacker_hero_id1` = %u , "
		" `attacker_hero_quality1` = %u , "
		" `attacker_hero_level1` = %u , "
		" `attacker_hero_star1` = %u , "

		" `attacker_hero_id2` = %u , "
		" `attacker_hero_quality2` = %u , "
		" `attacker_hero_level2` = %u , "
		" `attacker_hero_star2` = %u , "

		" `attacker_hero_id3` = %u , "
		" `attacker_hero_quality3` = %u , "
		" `attacker_hero_level3` = %u , "
		" `attacker_hero_star3` = %u , "

		" `attacker_hero_id4` = %u , "
		" `attacker_hero_quality4` = %u , "
		" `attacker_hero_level4` = %u , "
		" `attacker_hero_star4` = %u , "

		" `attacker_hero_id5` = %u , "
		" `attacker_hero_quality5` = %u , "
		" `attacker_hero_level5` = %u , "
		" `attacker_hero_star5` = %u , "

		" `defender_user_id` = %u , "
		" `defender_rank` = %u , "
		" `defender_power` = %u , "
		" `defender_name` = '%s' , "
		" `defender_level` = %u , "

		" `defender_hero_id1` = %u , "
		" `defender_hero_quality1` = %u , "
		" `defender_hero_level1` = %u , "
		" `defender_hero_star1` = %u , "

		" `defender_hero_id2` = %u , "
		" `defender_hero_quality2` = %u , "
		" `defender_hero_level2` = %u , "
		" `defender_hero_star2` = %u , "

		" `defender_hero_id3` = %u , "
		" `defender_hero_quality3` = %u , "
		" `defender_hero_level3` = %u , "
		" `defender_hero_star3` = %u , "

		" `defender_hero_id4` = %u , "
		" `defender_hero_quality4` = %u , "
		" `defender_hero_level4` = %u , "
		" `defender_hero_star4` = %u , "

		" `defender_hero_id5` = %u , "
		" `defender_hero_quality5` = %u , "
		" `defender_hero_level5` = %u , "
		" `defender_hero_star5` = %u , "
		" `is_win` = %u , "
		" `time` = %u ;"
		, op_arena_challenge.attacker_user_id()
		, op_arena_challenge.attacker_rank()
		, op_arena_challenge.attacker_power()
		, op_arena_challenge.attacker_name().c_str()
		, op_arena_challenge.attacker_level()
		, op_arena_challenge.attacker_heros_size() >= 1 ? op_arena_challenge.attacker_heros(0).hero_id() : 0
		, op_arena_challenge.attacker_heros_size() >= 1 ? op_arena_challenge.attacker_heros(0).quality() : 0
		, op_arena_challenge.attacker_heros_size() >= 1 ? op_arena_challenge.attacker_heros(0).level() : 0
		, op_arena_challenge.attacker_heros_size() >= 1 ? op_arena_challenge.attacker_heros(0).star() : 0

		, op_arena_challenge.attacker_heros_size() >= 2 ? op_arena_challenge.attacker_heros(1).hero_id() : 0
		, op_arena_challenge.attacker_heros_size() >= 2 ? op_arena_challenge.attacker_heros(1).quality() : 0
		, op_arena_challenge.attacker_heros_size() >= 2 ? op_arena_challenge.attacker_heros(1).level() : 0
		, op_arena_challenge.attacker_heros_size() >= 2 ? op_arena_challenge.attacker_heros(1).star() : 0

		, op_arena_challenge.attacker_heros_size() >= 3 ? op_arena_challenge.attacker_heros(2).hero_id() : 0
		, op_arena_challenge.attacker_heros_size() >= 3 ? op_arena_challenge.attacker_heros(2).quality() : 0
		, op_arena_challenge.attacker_heros_size() >= 3 ? op_arena_challenge.attacker_heros(2).level() : 0
		, op_arena_challenge.attacker_heros_size() >= 3 ? op_arena_challenge.attacker_heros(2).star() : 0

		, op_arena_challenge.attacker_heros_size() >= 4 ? op_arena_challenge.attacker_heros(3).hero_id() : 0
		, op_arena_challenge.attacker_heros_size() >= 4 ? op_arena_challenge.attacker_heros(3).quality() : 0
		, op_arena_challenge.attacker_heros_size() >= 4 ? op_arena_challenge.attacker_heros(3).level() : 0
		, op_arena_challenge.attacker_heros_size() >= 4 ? op_arena_challenge.attacker_heros(3).star() : 0

		, op_arena_challenge.attacker_heros_size() >= 5 ? op_arena_challenge.attacker_heros(4).hero_id() : 0
		, op_arena_challenge.attacker_heros_size() >= 5 ? op_arena_challenge.attacker_heros(4).quality() : 0
		, op_arena_challenge.attacker_heros_size() >= 5 ? op_arena_challenge.attacker_heros(4).level() : 0
		, op_arena_challenge.attacker_heros_size() >= 5 ? op_arena_challenge.attacker_heros(4).star() : 0


		, op_arena_challenge.defender_user_id()
		, op_arena_challenge.defender_rank()
		, op_arena_challenge.defender_power()
		, op_arena_challenge.defender_name().c_str()
		, op_arena_challenge.defender_level()

		, op_arena_challenge.defender_heros_size() >= 1 ? op_arena_challenge.defender_heros(0).hero_id() : 0
		, op_arena_challenge.defender_heros_size() >= 1 ? op_arena_challenge.defender_heros(0).quality() : 0
		, op_arena_challenge.defender_heros_size() >= 1 ? op_arena_challenge.defender_heros(0).level() : 0
		, op_arena_challenge.defender_heros_size() >= 1 ? op_arena_challenge.defender_heros(0).star() : 0

		, op_arena_challenge.defender_heros_size() >= 2 ? op_arena_challenge.defender_heros(1).hero_id() : 0
		, op_arena_challenge.defender_heros_size() >= 2 ? op_arena_challenge.defender_heros(1).quality() : 0
		, op_arena_challenge.defender_heros_size() >= 2 ? op_arena_challenge.defender_heros(1).level() : 0
		, op_arena_challenge.defender_heros_size() >= 2 ? op_arena_challenge.defender_heros(1).star() : 0

		, op_arena_challenge.defender_heros_size() >= 3 ? op_arena_challenge.defender_heros(2).hero_id() : 0
		, op_arena_challenge.defender_heros_size() >= 3 ? op_arena_challenge.defender_heros(2).quality() : 0
		, op_arena_challenge.defender_heros_size() >= 3 ? op_arena_challenge.defender_heros(2).level() : 0
		, op_arena_challenge.defender_heros_size() >= 3 ? op_arena_challenge.defender_heros(2).star() : 0

		, op_arena_challenge.defender_heros_size() >= 4 ? op_arena_challenge.defender_heros(3).hero_id() : 0
		, op_arena_challenge.defender_heros_size() >= 4 ? op_arena_challenge.defender_heros(3).quality() : 0
		, op_arena_challenge.defender_heros_size() >= 4 ? op_arena_challenge.defender_heros(3).level() : 0
		, op_arena_challenge.defender_heros_size() >= 4 ? op_arena_challenge.defender_heros(3).star() : 0

		, op_arena_challenge.defender_heros_size() >= 5 ? op_arena_challenge.defender_heros(4).hero_id() : 0
		, op_arena_challenge.defender_heros_size() >= 5 ? op_arena_challenge.defender_heros(4).quality() : 0
		, op_arena_challenge.defender_heros_size() >= 5 ? op_arena_challenge.defender_heros(4).level() : 0
		, op_arena_challenge.defender_heros_size() >= 5 ? op_arena_challenge.defender_heros(4).star() : 0

		, op_arena_challenge.is_win()
		, op_arena_challenge.time()
		);

	if (used == -1){
		return -1;
	}
	p += used;
	left -= used;

	strAppendHere.append(sql);
	return 0;
}

int db_sync_operator::clear_arena_team( UINT32 user_id )
{
	MYSQL* con = CDBMgr::GetInstance()->GetConnection();
	ASSERT(con);

	std::string sql;
	int r = _get_sql_of_clear_arena_team(sql, user_id);
	if (r != 0){
		ASSERT(false);
		return r;
	}

	int code = CDBMgr::Query(con, sql);
	if (0 != code){  //非0查询失败
		printf("query failed! [%s] [%d] [%s] \n", sql.c_str(), mysql_errno(con), mysql_error(con));
		return -1;
	}

	printf("成功清空用户[%u]竞技场防守队伍数据到DB中！\n", user_id);
	return 0;
}

int db_sync_operator::_get_sql_of_clear_arena_team(std::string& strAppendHere, UINT32 user_id )
{
	char sql[1024*5] = {0};
	int left = sizeof(sql);
	char* p = sql;
	int used = 0;

	used = sprintf_s(p, left, " UPDATE `user_arena` SET `hero_id1` = 0, `hero_id2` = 0, `hero_id3` = 0, `hero_id4` = 0, `hero_id5` = 0 WHERE `user_id` = %u;", user_id);
	if (used == -1){
		return -1;
	}
	p += used;
	left -= used;

	strAppendHere.append(sql);
	return 0;
}

int db_sync_operator::update_sign( UINT32 user_id, const gs_dbs_user_info_op_req::struct_op_sign& op_sign )
{
	MYSQL* con = CDBMgr::GetInstance()->GetConnection();
	ASSERT(con);

	std::string sql;
	int r = _get_sql_of_update_sign(sql, user_id, op_sign);
	if (r != 0){
		ASSERT(false);
		return r;
	}

	//int code = mysql_real_query(con, sql, (unsigned int)strlen(sql));
	int code = CDBMgr::Query(con, sql);
	if (0 != code){  //非0查询失败
		printf("query failed! [%s] [%d] [%s] \n", sql.c_str(), mysql_errno(con), mysql_error(con));
		return -1;
	}

	printf("成功更新用户[%u]签到数据到DB中！\n", user_id);
	return 0;
}

int db_sync_operator::_get_sql_of_update_sign(std::string& strAppendHere, UINT32 user_id, const gs_dbs_user_info_op_req::struct_op_sign& op_sign )
{
	char sql[1024] = {0};
	int left = sizeof(sql);
	int left_init = left;
	char* p = sql;
	int used = 0;

	used = sprintf_s(p, left, "UPDATE `user_sign` SET  ");
	if (used == -1){
		return -1;
	}
	p += used;
	left -= used;

	SQL_APPEND_FIELD_U(op_sign, sign_1_times);
	SQL_APPEND_FIELD_S(op_sign, sign_n_reward_status);
	SQL_APPEND_FIELD_U(op_sign, last_sign_time);
	SQL_APPEND_FIELD_U(op_sign, add_signed_times);

	//有修改字段的！
	if (left == left_init){
		ASSERT(false);
		return -1;
	}

	//去掉尾巴的,
	*(p-1) = ' ';
	used = sprintf_s(p, left, " WHERE `user_id`= %u;", user_id);
	if (used == -1){
		return -1;
	}

	strAppendHere.append(sql);
	return 0;
}


int db_sync_operator::add_sign(UINT32 user_id, const gs_dbs_user_info_op_req::struct_new_sign& new_sign)
{
	MYSQL* con = CDBMgr::GetInstance()->GetConnection();
	ASSERT(con);

	std::string sql;
	int r = _get_sql_of_add_sign(sql, user_id, new_sign);
	if (r != 0){
		ASSERT(false);
		return r;
	}

	int code = CDBMgr::Query(con, sql);
	if (0 != code){  //非0查询失败
		printf("query failed! [%s] [%d] [%s] \n", sql.c_str(), mysql_errno(con), mysql_error(con));
		return -1;
	}

	printf("成功新建用户[%u]签到数据到DB中！\n", user_id);
	return 0;
}

int db_sync_operator::_get_sql_of_add_sign(std::string& strAppendHere, UINT32 user_id, const gs_dbs_user_info_op_req::struct_new_sign& new_sign)
{
	char sql[1024] = {0};
	int left = sizeof(sql);
	char* p = sql;
	int used = 0;

	used = sprintf_s(p, left, "INSERT INTO `user_sign` SET `user_id` = %u, `sign_1_times` = %u, `sign_n_reward_status` = '%s', `last_sign_time` = %d, `add_signed_times` = %u;", user_id,
		new_sign.sign_1_times(), new_sign.sign_n_reward_status().c_str(), new_sign.last_sign_time(), new_sign.add_signed_times());
	if (used == -1){
		return -1;
	}
	strAppendHere.append(sql);
	return 0;
}

int db_sync_operator::add_novice_guide( UINT32 user_id, const gs_dbs_user_info_op_req::struct_op_novice_guide& op_guide )
{
	ASSERT(op_guide.op_type() == gs_dbs_user_info_op_req::Add);

	MYSQL* con = CDBMgr::GetInstance()->GetConnection();
	ASSERT(con);

	std::string sql;
	int r = _get_sql_of_add_novice_guide(sql, user_id, op_guide);
	if (r != 0){
		ASSERT(false);
		return r;
	}

	int code = CDBMgr::Query(con, sql);
	if (0 != code){  //非0查询失败
		printf("query failed! [%s] [%d] [%s] \n", sql.c_str(), mysql_errno(con), mysql_error(con));
		return -1;
	}

	printf("成功添加用户[%u]新手引导数据到DB中！\n", user_id);
	return 0;
}

int db_sync_operator::_get_sql_of_add_novice_guide(std::string& strAppendHere, UINT32 user_id, const gs_dbs_user_info_op_req::struct_op_novice_guide& op_guide )
{
	ASSERT(op_guide.op_type() == gs_dbs_user_info_op_req::Add);
	if (op_guide.op_type() != gs_dbs_user_info_op_req::Add){
		ASSERT(false);
		return -1;
	}

	char sql[1024] = {0};
	int left = sizeof(sql);
	char* p = sql;
	int used = 0;

	used = sprintf_s(p, left, "INSERT INTO `user_guide` SET `user_id` = %u, `guide_id` = %u, `time` = NOW();", user_id,
		op_guide.guide_id());
	if (used == -1){
		return -1;
	}

	strAppendHere.append(sql);
	return 0;
}

int db_sync_operator::add_new_user_statis(UINT32 user_id, const gs_dbs_user_info_op_req::struct_new_user_statis& new_statis)
{
	MYSQL* con = CDBMgr::GetInstance()->GetConnection();
	ASSERT(con);
	
	std::string sql;
	int r = _get_sql_of_add_new_user_statis(sql, user_id, new_statis);
	if (r != 0){
		ASSERT(false);
		return r;
	}

	int code = CDBMgr::Query(con, sql);
	if (0 != code){  //非0查询失败
		printf("query failed! [%s] [%d] [%s] \n", sql.c_str(), mysql_errno(con), mysql_error(con));
		return -1;
	}

	printf("成功添加玩家[%u]统计信息到DB中！\n", user_id);
	return 0;
}

int db_sync_operator::_get_sql_of_add_new_user_statis(std::string& strAppendHere, UINT32 user_id, const gs_dbs_user_info_op_req::struct_new_user_statis& new_statis)
{
	char sql[1024] = {0};
	int left = sizeof(sql);
	char* p = sql;
	int used = 0;

	used = sprintf_s(p, left, "INSERT INTO `user_statistics` SET `user_id` = %u,  `event_type` = %u,`event_occur_time` = from_unixtime(%u), `cur_id` = %u, `level` = %u, `vip` = %u;", 
		user_id, new_statis.event_type(), new_statis.occur_time(), new_statis.cur_id(), new_statis.level(), new_statis.vip());

	if (used == -1){
		return -1;
	}

	strAppendHere.append(sql);
	return 0;
}

int db_sync_operator::edit_user_name(UINT32 user_id, const char* name)
{
	MYSQL* con = CDBMgr::GetInstance()->GetConnection();
	ASSERT(con);
	
	char sql[1024] = {0};
	sprintf_s(sql, sizeof(sql), "CALL edit_user_name(%d,'%s');", user_id, name);
	int code = CDBMgr::GetInstance()->Query(con, sql, strlen(sql));

	if(0 != code){
		printf("query failed! [%s] [%d] [%s] \n", sql, mysql_errno(con), mysql_error(con));
		return stDBEditUserNameRet::System_Error;
	}
	
	MYSQL_RES* res = mysql_store_result(con);
	MYSQL_ROW row;
	int ret = -1;
	if (row = mysql_fetch_row(res))
	{
		ret = atoi(row[0]);
	}

	if (-1 == ret)
	{
		return stDBEditUserNameRet::Same_Name;
	}
	else if (0 == ret)
	{
		printf("成功修改玩家[%u]姓名", user_id);
		return stDBEditUserNameRet::Success;
	}
	else
	{
		printf("query failed! [%s] [%d] [%s] \n", sql, mysql_errno(con), mysql_error(con));
		ASSERT(false);
		return stDBEditUserNameRet::Same_Name;
	}
}

int db_sync_operator::op_user_new_arena_mail( gs_dbs_user_new_arena_mail_req* req )
{
	MYSQL* con = CDBMgr::GetInstance()->GetConnection();
	ASSERT(con);

	std::string sql;
	int r = _get_sql_of_op_user_new_arena_mail(sql, req);
	if (r != 0){
		ASSERT(false);
		return r;
	}

	if(sql.length()){
		int code = CDBMgr::Query(con, sql);
		if (0 != code){   //非0查询失败  
			printf("query failed! [%s] [%d] [%s] \n", sql.c_str(), mysql_errno(con), mysql_error(con));
			return -1;
		}
	}
	return 0;
}

int db_sync_operator::_get_sql_of_op_user_new_arena_mail(std::string& strAppendHere, gs_dbs_user_new_arena_mail_req* req )
{
	char sql[1024] = {0};
	std::string strMail;
	std::string strMailAttach;
	std::string strSql;

	::google::protobuf::RepeatedPtrField< ::arena_mail >::iterator it = req->mutable_mails()->begin();
	for (; it != req->mutable_mails()->end(); ++it)
	{
		UINT32 iUserID = it->user_id();
		db_user_mail* new_mail = it->mutable_mail();
		char bufMail[1024] = {0};
		sprintf_s(bufMail, sizeof(bufMail), 
			"(%d,%d,from_unixtime(%d),'%s',%d,%d),", 
			new_mail->mail_id(), iUserID, new_mail->send_time(), new_mail->content().c_str(), new_mail->status(), new_mail->type());
		strMail += bufMail;

		if(new_mail->attachs_size()){
			::google::protobuf::RepeatedPtrField< ::db_mail_attach >::iterator it_attach = new_mail->mutable_attachs()->begin();
			for (; it_attach != new_mail->mutable_attachs()->end(); ++it_attach)
			{
				char bufAttach[1024] = {0};
				sprintf_s(bufAttach, sizeof(bufAttach), "(%d,%d,%d,%d,%d,%d),",
					it_attach->attach_id(), it_attach->mutable_element()->type(), it_attach->mutable_element()->id(), it_attach->mutable_element()->num(), new_mail->mail_id(), iUserID);
				strMailAttach += bufAttach;
			}
		}
	}

	if (strMail.length()){
		strMail.erase(strMail.length() - 1);
		strSql += "INSERT `user_mail`  VALUES " + strMail + ";";
	}

	if (strMailAttach.length()){
		strMailAttach.erase(strMailAttach.length() - 1);
		strSql += "INSERT INTO `user_mail_attach`  VALUES " + strMailAttach + ";"; 
	}

	strAppendHere.append(strSql);
	return 0;
}

int db_sync_operator::search_user(const gs_dbs_search_user_req& search_req, dbs_gs_search_user_res& rRes)
{
	MYSQL* con = CDBMgr::GetInstance()->GetConnection();
	ASSERT(con);
	
	char buf[1024] = {0};
	if (search_req.has_search_id())
	{
		sprintf_s(buf, sizeof buf, 
			"SELECT t1.user_id, t1.head_id, t1.head_frame_id, t1.`name`, t1.`level`, t1.vip, t2.rank, t3.last_update_timestamp, t1.`signature` "
			"FROM user AS t1 "
			"LEFT JOIN `user_arena` AS t2 ON t1.user_id = t2.user_id "
			"LEFT JOIN `user_mission_event` AS t3 ON t1.user_id = t3.user_id AND `mission_event_type` = 12 "
			"WHERE t1.`user_id` = %u", search_req.search_id());
	}
	else if(search_req.has_search_name()){
		sprintf_s(buf, sizeof buf, 
			"SELECT t1.user_id, t1.head_id, t1.head_frame_id, t1.`name`, t1.`level`, t1.vip, t2.rank, t3.last_update_timestamp, t1.`signature` "
			"FROM user AS t1 "
			"LEFT JOIN `user_arena` AS t2 ON t1.user_id = t2.user_id "
			"LEFT JOIN `user_mission_event` AS t3 ON t1.user_id = t3.user_id AND `mission_event_type` = 12 "
			"WHERE t1.`name` = '%s'", search_req.search_name().c_str());
	}
	else{
		ASSERT(false);
		return -1;
	}
	
	int code = CDBMgr::Query(con, buf, strlen(buf));
	if (0 != code)
	{
		printf("query failed! [%s] [%d] [%s] \n", buf, mysql_errno(con), mysql_error(con));
		rRes.set_error_code(e_dbsgs_errorcode_unkown_error);
		return -1;
	}

	MYSQL_RES* res = mysql_store_result( con );	
	if (res)
	{
		MYSQL_ROW row = mysql_fetch_row(res);
		
		if(row)
		{
			{
				std::string need_fields[] = {"user_id","head_id","head_frame_id","name","level", "vip", "rank", "last_update_timestamp", "signature"};
				std::vector<std::string> v_need_field(need_fields, need_fields + sizeof(need_fields)/sizeof(need_fields[0]));
				ASSERT(CDBMgr::check_fields(res, v_need_field));
			}
			int col = 0;
			friend_base_info* base_info = rRes.mutable_base_info();
			base_info->set_user_id(atoi(row[col++]));
			base_info->set_head_id(atoi(row[col++]));
			base_info->set_head_frame_id(atoi(row[col++]));
			base_info->set_name(row[col++]);
			base_info->set_level(atoi(row[col++]));
			base_info->set_vip(atoi(row[col++]));
			
			if(row[col++])
				base_info->set_arena_rank(atoi(row[col - 1]));
			else{
				base_info->set_arena_rank(0);
			}

			if(row[col++])
				base_info->set_last_login_time(atoi(row[col - 1]));
			else{
				base_info->set_last_login_time(0);
			}
			base_info->set_signature(row[col++]);
			rRes.set_error_code(e_dbsgs_errorcode_success);
			return 0;
		}else{
			rRes.set_error_code(e_dbsgs_errorcode_user_info_not_exsit);
			return -1;
		}
		
	}
	else{
		rRes.set_error_code(e_dbsgs_errorcode_unkown_error);
		return -1;
	}

	return 0;
}

int db_sync_operator::op_friend(UINT32 user_id, const ::google::protobuf::RepeatedPtrField< ::gs_dbs_user_info_op_req_struct_op_friend >& friends)
{
	MYSQL* con = CDBMgr::GetInstance()->GetConnection();
	ASSERT(con);

	::google::protobuf::RepeatedPtrField< ::gs_dbs_user_info_op_req_struct_op_friend >::const_iterator it = friends.begin();
	std::string sql;

	int r = _get_sql_of_op_friend(sql, user_id, friends);
	if(0 != r){
		ASSERT(false);
		return r;
	}

	if (sql.length())
	{
		int code = CDBMgr::Query(con, sql);
		if (0 != code)
		{
			printf("query failed! [%s] [%d] [%s] \n", sql.c_str(), mysql_errno(con), mysql_error(con));
			return -1;
		}
	}
	return 0;
}

int db_sync_operator::_get_sql_of_op_friend(std::string& strAppendHere, UINT32 user_id, const ::google::protobuf::RepeatedPtrField< ::gs_dbs_user_info_op_req_struct_op_friend >& friends)
{
	::google::protobuf::RepeatedPtrField< ::gs_dbs_user_info_op_req_struct_op_friend >::const_iterator it = friends.begin();
	char buf[1024] = {0};
	std::string str;
	int use = 0;
	for (; it != friends.end(); ++it)
	{
		memset(buf, 0, sizeof buf);
		switch(it->op_type())
		{//删除就统一由这边删除
		case gs_dbs_user_info_op_req::struct_op_friend::GET_ELEMENT1:{
			use = sprintf_s(buf, sizeof buf, "UPDATE `user_friend` SET `last_friend_give_element_timestamp` = 0 WHERE `user_id` = %u AND `friend_id` = %u;", user_id,  it->friend_id());
			if (-1 == use)
			{
				return -1;
			}
			str += buf;
																	 }break;
		case gs_dbs_user_info_op_req::struct_op_friend::GET_ELEMENTN:{
			use = sprintf_s(buf, sizeof buf, "UPDATE `user_friend` SET `last_friend_give_element_timestamp` = 0 WHERE `user_id` = %u;", user_id);
			if (-1 == use)
			{
				return -1;
			}
			str += buf;
																	 }break;
		case gs_dbs_user_info_op_req::struct_op_friend::GIVE_ELEMENT1:{
			use = sprintf_s(buf, sizeof buf, "UPDATE `user_friend` SET `last_me_give_element_timestamp` = %u WHERE `user_id` = %u AND `friend_id` = %u;", (UINT32)Now(), user_id,  it->friend_id());
			if (-1 == use)
			{
				return -1;
			}
			str += buf;
																	  }break;
		case gs_dbs_user_info_op_req::struct_op_friend::GIVE_ELEMENTN:{
			use = sprintf_s(buf, sizeof buf, "UPDATE `user_friend` SET `last_me_give_element_timestamp` = %u WHERE `user_id` = %u;", (UINT32)Now(), user_id,  it->friend_id());
			if (-1 == use)
			{
				return -1;
			}
			str += buf;
																	  }break;
		case gs_dbs_user_info_op_req::struct_op_friend::GIVE_UP_APPLY1:{
			use = sprintf_s(buf, sizeof buf, "DELETE FROM `user_friend_apply` WHERE `user_id` = %u AND `apply_user_id` = %u;", user_id,  it->friend_id());
			if (-1 == use)
			{
				return -1;
			}
			str += buf;
																	   }break;
		case gs_dbs_user_info_op_req::struct_op_friend::GIVE_UP_APPLYN:{
			use = sprintf_s(buf, sizeof buf, "DELETE FROM `user_friend_apply` WHERE `user_id` = %u;", user_id);
			if (-1 == use)
			{
				return -1;
			}
			str += buf;
																	   }break;
		case gs_dbs_user_info_op_req::struct_op_friend::AFFIRM_AGREE1:{
			use = sprintf_s(buf, sizeof buf, 
				"INSERT INTO `user_friend` SET `user_id` = %u, `friend_id` = %u, `last_friend_give_element_timestamp` = 0, `last_me_give_element_timestamp` = 0;"
				"DELETE FROM `user_friend_apply` WHERE `apply_user_id` = %u AND `user_id` = %u;", user_id, it->friend_id(), user_id, it->friend_id());
			if (-1 == use)
			{
				return -1;
			}
			str += buf;
																	  }break;
		case gs_dbs_user_info_op_req::struct_op_friend::AFFIRM_AGREEN:{
			use = sprintf_s(buf, sizeof buf, "INSERT INTO `user_friend` SELECT `apply_user_id`, `user_id`, 0, 0 FROM `user_friend_apply` WHERE `apply_user_id` = %u;"
				"DELETE FROM `user_friend_apply` WHERE `apply_user_id` = %u;", user_id, user_id);
			if (-1 == use)
			{
				return -1;
			}
			str += buf;
																	  }break;
		case gs_dbs_user_info_op_req::struct_op_friend::AFFIRM_REFUSE1:{
			use = sprintf_s(buf, sizeof buf, "DELETE FROM `user_friend_apply` WHERE `apply_user_id` = %u AND `user_id` = %u;", user_id, it->friend_id());
			if (-1 == use)
			{
				return -1;
			}
			str += buf;
																	   }break;
		case gs_dbs_user_info_op_req::struct_op_friend::AFFIRM_REFUSEN:{
			use = sprintf_s(buf, sizeof buf, "DELETE FROM `user_friend_apply` WHERE `apply_user_id` = %u;", user_id);
			if (-1 == use)
			{
				return -1;
			}
			str += buf;
																	   }break;
		case gs_dbs_user_info_op_req::struct_op_friend::ADD_FRIEND_GS_ONLINE:{
			use = sprintf_s(buf, sizeof buf, "INSERT INTO `user_friend_apply` SET `user_id` = %u, `apply_user_id` = %u;", user_id, it->friend_id());
			if (-1 == use)
			{
				return -1;
			}
			str += buf;
																			 }break;
		case gs_dbs_user_info_op_req::struct_op_friend::DEL_FRIEND:{
			use = sprintf_s(buf, sizeof buf, "DELETE FROM `user_friend` WHERE `user_id` = %u AND `friend_id` = %u;", user_id, it->friend_id());
			if (-1 == use)
			{
				return -1;
			}
			str += buf;
																   }break;
		case gs_dbs_user_info_op_req::struct_op_friend::DEL_RECOMMEND_FRIEND:{

																			 }break;
		default:
			ASSERT(false);
			return -1;
		}
	}

	strAppendHere.append(str);
	return 0;
}

int db_sync_operator::op_friend( UINT32 user_id, const gs_dbs_user_friend_op_req& friend_op )
{
	MYSQL* con = CDBMgr::GetInstance()->GetConnection();
	ASSERT(con);
	std::string sql;
	int r = _get_sql_of_op_friend(sql, user_id, friend_op);
	if (0 != r)
	{
		ASSERT(false);
		return r;
	}
	if (sql.length())
	{
		int code = CDBMgr::Query(con, sql);
		if (0 != code)
		{
			printf("query failed! [%s] [%d] [%s] \n", sql.c_str(), mysql_errno(con), mysql_error(con));
			return -1;
		}
	}
	return 0;
}


int db_sync_operator::_get_sql_of_op_friend(std::string& strAppendHere, UINT32 user_id, const gs_dbs_user_friend_op_req& friend_op )
{
	char sql[1024] = {0};
	int use = 0;
	switch(friend_op.op_type()){
		case gs_dbs_user_friend_op_req::FRIEND_GIVE_ELEMENT:{
			use = sprintf_s(sql, sizeof sql, "UPDATE `user_friend` SET `last_friend_give_element_timestamp` = %u  WHERE  `user_id` = %u  AND `friend_id` = %u;", (UINT32)Now(), user_id, friend_op.friend_user_id());
			if (-1 == use)
			{
				return -1;
			}
															}break;
		case gs_dbs_user_friend_op_req::FRIEND_GIVE_UP_APPLY:{//从待确认中删除（由申请方进行删除，此处就不做删除操作）
															 }break;
		case gs_dbs_user_friend_op_req::FRIEND_AFFIRM_AGREE:{//加入到好友列表（从申请列表删除操作有另一方做）
			use = sprintf_s(sql, sizeof sql, "INSERT INTO `user_friend` SET `user_id` = %u, `friend_id` = %u, `last_friend_give_element_timestamp` = 0, `last_me_give_element_timestamp` = 0;", user_id, friend_op.friend_user_id());
			if (-1 == use)
			{
				return -1;
			}
															}break;
		case gs_dbs_user_friend_op_req::FRIEND_AFFIRM_REFUSE:{//从申请列表中删除(由另一方删除)
															 }break;
		case gs_dbs_user_friend_op_req::FRIEND_DEL_FRIEND:{//从好友列表中删除
			use = sprintf_s(sql, sizeof sql, "DELETE FROM `user_friend` WHERE `user_id` = %u  AND `friend_id` = %u;", user_id, friend_op.friend_user_id());
			if (-1 == use)
			{
				return -1;
			}
														  }break;
		case gs_dbs_user_friend_op_req::FRIEND_ADD_FRIEND:{//无需操作，在线已经由另一方添加
														  }break;
		default:
			ASSERT(false);
			return -1;
	}

	strAppendHere.append(sql);
	return 0;
}

int db_sync_operator::add_friend_offline( gs_dbs_friend_offline_req& req )
{
	MYSQL* con = CDBMgr::GetInstance()->GetConnection();
	ASSERT(con);

	char sql[1024] = {0};
	if (req.has_friend_info())
	{
		sprintf_s(sql, sizeof sql, "CALL `add_friend`(%u, %u)", req.user_id(), req.mutable_friend_info()->user_id());
	}else if(req.has_friend_id()){
		sprintf_s(sql, sizeof sql, "CALL `add_friend`(%u, %u);SELECT t1.user_id, t1.head_id, t1.head_frame_id, t1.`name`, t1.`level`, t1.vip, t2.rank, t3.last_update_timestamp "
									"FROM user AS t1 "
									"LEFT JOIN `user_arena` AS t2 ON t1.user_id = t2.user_id "
									"LEFT JOIN `user_mission_event` AS t3 ON t1.user_id = t3.user_id AND `mission_event_type` = 12 "
									"WHERE t1.`user_id` = %u;", req.user_id(), req.friend_id(), req.friend_id());
	}
	else{
		ASSERT(false);
		return stDBFriendOffLineRet::System_Error;
	}
	int code = CDBMgr::Query(con, sql, strlen(sql));
	
	if(0 != code){
		printf("query failed! [%s] [%d] [%s] \n", sql, mysql_errno(con), mysql_error(con));
		return stDBFriendOffLineRet::System_Error;
	}
	
	int i = 0;
	do
	{
		MYSQL_RES* res = mysql_store_result( con );	
		if (!res)
		{
			continue;
		}
		switch(i++)
		{
		case 0:{
				   {
					   std::string need_fields[] = {"ret"};
					   std::vector<std::string> v_need_field(need_fields, need_fields + sizeof(need_fields)/sizeof(need_fields[0]));
					   ASSERT(CDBMgr::check_fields(res, v_need_field));
				   }
				   MYSQL_ROW row;
				   int ret = -1;
				   if (row = mysql_fetch_row(res)){
					   ret = atoi(row[0]);
				   }

				   if (-1 == ret)
				   {
				   		return stDBFriendOffLineRet::FRIEND_CONFIRM_FULL;
				   }
				   else if (0 == ret)
				   {
					   if(req.has_friend_info())
							return stDBFriendOffLineRet::SUCCESS;
				   }
				   else
				   {
				   		ASSERT(false);
				   		return stDBFriendOffLineRet::FRIEND_CONFIRM_FULL;
				   }

			   }break;
		case 1:{
				   {
					   std::string need_fields[] = {"user_id","head_id","head_frame_id","name","level", "vip", "rank", "last_update_timestamp", "signature"};
					   std::vector<std::string> v_need_field(need_fields, need_fields + sizeof(need_fields)/sizeof(need_fields[0]));
					   ASSERT(CDBMgr::check_fields(res, v_need_field));
				   }
					
				   MYSQL_ROW row;
				   if (row = mysql_fetch_row(res)){
					   int col = 0;
					   friend_base_info* base_info = req.mutable_friend_info();
					   base_info->set_user_id(atoi(row[col++]));
					   base_info->set_head_id(atoi(row[col++]));
					   base_info->set_head_frame_id(atoi(row[col++]));
					   base_info->set_name(row[col++]);
					   base_info->set_level(atoi(row[col++]));
					   base_info->set_vip(atoi(row[col++]));
					   
					   if(row[col++])
						   base_info->set_arena_rank(atoi(row[col - 1]));
					   else{
						   base_info->set_arena_rank(0);
					   }

					   if(row[col++])
						   base_info->set_last_login_time(atoi(row[col - 1]));
					   else{
						   base_info->set_last_login_time(0);
					   }
					   base_info->set_signature(row[col++]);
					   return stDBFriendOffLineRet::SUCCESS;
				   }else{
					   return stDBFriendOffLineRet::Unkown_Error;																						
				   }
			   }break;
		}
		mysql_free_result(res);
	}while( !mysql_next_result( con ) );
	return stDBFriendOffLineRet::Unkown_Error;
}

int db_sync_operator::affirm_agree_offline( gs_dbs_friend_offline_req& req )
{
	MYSQL* con = CDBMgr::GetInstance()->GetConnection();
	ASSERT(con);

	std::string ids;
	::google::protobuf::RepeatedField< ::google::protobuf::uint32 >::const_iterator it = req.affirm_ids().begin();
	for (; it != req.affirm_ids().end(); ++it)
	{
		char buf[32] = {0};
		_itoa_s(*it, buf, sizeof buf, 10);
		ids += buf ;
		ids += ",";
	}
	if (ids.length())
	{
		//首先检测同意的好友是否有满的
		ids.erase(ids.length() - 1);//将最后的,删除
		char sql[1024] = {0};
		sprintf_s(sql, sizeof sql, "CALL `is_affirm_friend_full`('%s');", ids.c_str());
		int code = CDBMgr::Query(con, sql, strlen(sql));
		if(0 != code){
			printf("query failed! [%s] [%d] [%s] \n", sql, mysql_errno(con), mysql_error(con));
			return stDBFriendOffLineRet::System_Error;
		}

		MYSQL_RES* res = mysql_store_result(con);
		MYSQL_ROW row;
		int ret = -1;
		if (row = mysql_fetch_row(res))
		{
			ret = atoi(row[0]);
		}

		if (-1 == ret)
		{
			return stDBFriendOffLineRet::FRIEND_NUM_REACH_MAX;
		}
		
		//插入数据
		std::string db_sql;
		if(0 == ret){
			it = req.affirm_ids().begin();
			for (; it != req.affirm_ids().end(); ++it)
			{
				memset(sql, 0, sizeof sql);
				sprintf_s(sql, sizeof sql, "INSERT INTO `user_friend` SET `user_id` = %u, `friend_id` = %u, `last_friend_give_element_timestamp` = 0, `last_me_give_element_timestamp` = 0;"
											"INSERT INTO `user_friend` SET `user_id` = %u, `friend_id` = %u, `last_friend_give_element_timestamp` = 0, `last_me_give_element_timestamp` = 0;"
											"DELETE FROM `user_friend_apply` WHERE `user_id` = %u AND `apply_user_id` = %u;",
											*it, req.user_id(), req.user_id(), *it, *it, req.user_id());
				db_sql += sql;
			}
			code = CDBMgr::Query(con, db_sql.c_str(), db_sql.length());
			if(0 != code){
				printf("query failed! [%s] [%d] [%s] \n", db_sql.c_str(), mysql_errno(con), mysql_error(con));
				return stDBFriendOffLineRet::System_Error;
			}
			return stDBFriendOffLineRet::SUCCESS;
		}
	}
	return stDBFriendOffLineRet::Unkown_Error;
}

int db_sync_operator::get_rank( dbs_gs_rank_res& rank_res )
{
	MYSQL* con = CDBMgr::GetInstance()->GetConnection();
	ASSERT(con);

	char* sql = 
		//战力排行
		"set @rownum=0; "
		"SELECT  (@rownum:=@rownum+1) as `fight_rank`, `user_id` "
			"FROM `user` "
			"ORDER BY `fight_power` DESC, `level`  DESC, `vip` DESC, `user_id` ASC; "

		//等级排行
		"set @rownum=0; "
		"SELECT (@rownum:=@rownum+1) as `level_rank`, `user_id` "
			"FROM `user` "
			"ORDER BY `level`  DESC, `fight_power` DESC,  `vip` DESC, `user_id` ASC; "
		//战力排行(详细)
		"SELECT `user_id`, `head_id`, `head_frame_id`, `level`, `name`, `vip`, `fight_power` "
			"FROM `user`  "
		"ORDER BY `fight_power` DESC, `level`  DESC, `vip` DESC, `user_id` ASC LIMIT 50; "
		//等级排行(详细)
		"SELECT  `user_id`, `head_id`, `head_frame_id`, `level`, `name`, `vip`, `fight_power`  "
			"FROM `user` "
		"ORDER BY `level`  DESC, `fight_power` DESC,  `vip` DESC, `user_id` ASC LIMIT 50; "
		//竞技场排行(详细)
		"SELECT t2.user_id, t2.`head_id`, t2.`head_frame_id`, t2.`level`, t2.`name`, t2.`vip`, t2.`fight_power`  "
			"FROM `user_arena`  AS t1  "
		"LEFT JOIN `user` AS t2 ON t1.user_id = t2.user_id "
		"WHERE t1.`rank` != 0  "
		"ORDER BY t1.`rank` ASC LIMIT 50; ";
		
	int code = CDBMgr::Query(con, sql, strlen(sql));
	if (0 != code)
	{
		printf("query failed! [%s] [%d] [%s] \n", sql, mysql_errno(con), mysql_error(con));
		return -1;
	}

	int i = 0;
	do
	{
		MYSQL_RES* res = mysql_store_result( con );	
		if (!res)
		{
			continue;
		}
		switch(i++)
		{
		case 0:{
				   {
					   std::string need_fields[] = {"fight_rank", "user_id"};
					   std::vector<std::string> v_need_field(need_fields, need_fields + sizeof(need_fields)/sizeof(need_fields[0]));
					   ASSERT(CDBMgr::check_fields(res, v_need_field));
				   }
				   MYSQL_ROW row;
				   while(row = mysql_fetch_row(res))
				   {
						dbs_gs_rank_res::rank_brief_info* brief_info = rank_res.add_fight_power_brief_ranks();
						brief_info->set_rank(atoi(row[0]));
						brief_info->set_user_id(atoi(row[1]));	
				   }

			   }break;
		case 1:{
				   {
					   std::string need_fields[] = {"level_rank", "user_id"};
					   std::vector<std::string> v_need_field(need_fields, need_fields + sizeof(need_fields)/sizeof(need_fields[0]));
					   ASSERT(CDBMgr::check_fields(res, v_need_field));
				   }

				   MYSQL_ROW row;
				   while(row = mysql_fetch_row(res))
				   {
					   dbs_gs_rank_res::rank_brief_info* brief_info = rank_res.add_level_brief_ranks();
					   brief_info->set_rank(atoi(row[0]));
					   brief_info->set_user_id(atoi(row[1]));	
				   }
			   }break;
		case 2:{//战力排行(详细)
					{
						std::string need_fields[] = {"user_id", "head_id", "head_frame_id", "level", "name", "vip", "fight_power"};
						std::vector<std::string> v_need_field(need_fields, need_fields + sizeof(need_fields)/sizeof(need_fields[0]));
						ASSERT(CDBMgr::check_fields(res, v_need_field));
					}

					MYSQL_ROW  row;
					while (row = mysql_fetch_row(res))
					{
						dbs_gs_rank_res::rank_info* info = rank_res.add_fight_power_ranks();
						int col = 0;
						info->set_user_id(atoi(row[col++]));
						info->set_head_id(atoi(row[col++]));
						info->set_head_frame_id(atoi(row[col++]));
						info->set_level(atoi(row[col++]));
						info->set_name(row[col++]);
						info->set_vip(atoi(row[col++]));
						info->set_fight_power(atoi(row[col++]));
					}
			   }break;
		case 3:{//等级排行(详细)
				   {
					   std::string need_fields[] = {"user_id", "head_id", "head_frame_id", "level", "name", "vip", "fight_power"};
					   std::vector<std::string> v_need_field(need_fields, need_fields + sizeof(need_fields)/sizeof(need_fields[0]));
					   ASSERT(CDBMgr::check_fields(res, v_need_field));
				   }

				   MYSQL_ROW  row;
				   while (row = mysql_fetch_row(res))
				   {
					   dbs_gs_rank_res::rank_info* info = rank_res.add_level_ranks();
					   int col = 0;
					   info->set_user_id(atoi(row[col++]));
					   info->set_head_id(atoi(row[col++]));
					   info->set_head_frame_id(atoi(row[col++]));
					   info->set_level(atoi(row[col++]));
					   info->set_name(row[col++]);
					   info->set_vip(atoi(row[col++]));
					   info->set_fight_power(atoi(row[col++]));
				   }
			   }break;
		case 4:{//竞技场排行(详细)
				   {
					   std::string need_fields[] = {"user_id", "head_id", "head_frame_id", "level", "name", "vip", "fight_power"};
					   std::vector<std::string> v_need_field(need_fields, need_fields + sizeof(need_fields)/sizeof(need_fields[0]));
					   ASSERT(CDBMgr::check_fields(res, v_need_field));
				   }

				   MYSQL_ROW  row;
				   while (row = mysql_fetch_row(res))
				   {
					   dbs_gs_rank_res::rank_info* info = rank_res.add_arena_ranks();
					   int col = 0;
					   info->set_user_id(atoi(row[col++]));
					   info->set_head_id(atoi(row[col++]));
					   info->set_head_frame_id(atoi(row[col++]));
					   info->set_level(atoi(row[col++]));
					   info->set_name(row[col++]);
					   info->set_vip(atoi(row[col++]));
					   info->set_fight_power(atoi(row[col++]));
				   }
			   }break;
		}
		mysql_free_result(res);
	}while( !mysql_next_result( con ) );
	return 0;
}

int db_sync_operator::_get_sql_of_op_crusade(std::string& strAppendHere, UINT32 user_id, const gs_dbs_user_info_op_req_struct_op_crusade& op_crusade)
{
	char sql[1024] = {0};
	std::string str;
	int use = 0;
	switch(op_crusade.op_type())
	{
	case gs_dbs_user_info_op_req::struct_op_crusade::CRUSADE_END:
		{
			//自己信息(有就更新，没有就插入)
			::google::protobuf::RepeatedPtrField< ::user_crusade_crusade_hero >::const_iterator it = op_crusade.end().me_heros().begin();
			for (; it != op_crusade.end().me_heros().end(); ++it)
			{
				use = sprintf_s(sql, sizeof sql, 
					"INSERT INTO `user_crusade_hero` SET `user_id` = %u, `hero_id` = %u, `hp` = %u, `ao_yi` = %u, `bi_sha` = %u ON DUPLICATE KEY UPDATE `hp` = %u, `ao_yi` = %u, `bi_sha` = %u;",
					user_id, it->hero_id(), it->hp(), it->ao_yi(), it->bi_sha(), it->hp(), it->ao_yi(), it->bi_sha());
				if (-1 == use)
				{
					return -1;
				}
				str += sql;
			}
			//敌人信息(将敌人信息替换)
			memset(sql, 0, sizeof sql);
			use = sprintf_s(sql, sizeof sql, "DELETE FROM `user_crusade_enemy` WHERE `user_id` = %u;", user_id);
			if (-1 == use)
			{
				return -1;
			}
			str += sql;

			memset(sql, 0, sizeof sql);

			if (op_crusade.end().enemy_heros_size())
			{
				str += "INSERT INTO `user_crusade_enemy` VALUES ";
				::google::protobuf::RepeatedPtrField< ::user_crusade_crusade_hero >::const_iterator it_enemy = op_crusade.end().enemy_heros().begin();
				for (; it_enemy != op_crusade.end().enemy_heros().end(); ++it_enemy)
				{
					memset(sql, 0, sizeof sql);
					use = sprintf_s(sql, sizeof sql, 
						"(%u, %u, %u, %u, %u),",
						user_id, it_enemy->hero_id(), it_enemy->hp(), it_enemy->ao_yi(), it_enemy->bi_sha());
					str += sql;

				}
				str.erase(str.length() - 1);//去除,
				str += ";";
			}

			//user_crusade
			memset(sql, 0, sizeof sql);
			use = sprintf_s(sql, sizeof sql, "INSERT INTO `user_crusade` SET `user_id` = %u, `wave` = %u ON DUPLICATE KEY UPDATE `wave` = %u;",
				user_id, op_crusade.end().wave(), op_crusade.end().wave());
			if (-1 == use){
				return -1;
			}
			str += sql;
		}break;
	case gs_dbs_user_info_op_req::struct_op_crusade::HERO_LIFE:
		{
			use = sprintf_s(sql, sizeof sql, "DELETE FROM `user_crusade_hero` WHERE `user_id` = %u AND `hero_id` = %u;", user_id, op_crusade.hero_id());
			if (-1 == use){
				return -1;
			}
			str += sql;
		}break;
	case gs_dbs_user_info_op_req::struct_op_crusade::RESET:
		{
			use = sprintf_s(sql, sizeof sql, "DELETE FROM `user_crusade_hero` WHERE `user_id` = %u;"
				"DELETE FROM `user_crusade_enemy` WHERE `user_id` = %u;"
				"UPDATE `user_crusade` SET `wave` = 1 WHERE `user_id` = %u;",user_id, user_id, user_id);
			if (-1 == use){
				return -1;
			}
			str += sql;
		}break;
	default:
		ASSERT(false);
		return -1;
	}
	strAppendHere.append(str);
	return 0;
}
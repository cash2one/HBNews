#ifndef db_sync_operator_h__
#define db_sync_operator_h__
#pragma once

#include <map>
#include <string>
#include "def.h"

#include "dbs_gs_proto.pb.h"
class gs_dbs_user_info_op_req_struct_op_mail;
class gs_dbs_user_info_op_req_struct_op_game_element;
class gs_dbs_user_info_op_req_struct_op_user_base_info;
class gs_dbs_user_info_op_req_struct_op_hero;
//class db_user_info;
//class gs_dbs_user_info_op_req_struct_op_sub_task;
struct stAddMail;
namespace db_sync_operator{

	//创建新玩家信息
	int new_create_user(db_user_info* user_info);
	int _get_sql_of_new_create_user(std::string& strAppendHere, db_user_info* user_info );

	//获取玩家信息
	int get_user_info(UINT32 user_id, db_user_info* user_info);

	int update_base_info(UINT32 user_id, gs_dbs_user_info_op_req_struct_op_base_info* user_info );
	int _get_sql_of_update_base_info(std::string& strAppendHere, UINT32 user_id, gs_dbs_user_info_op_req_struct_op_base_info* user_info );
	
	int op_user_new_mail(stAddMail* new_mail);

	int op_user_new_arena_mail(gs_dbs_user_new_arena_mail_req* req);
	int _get_sql_of_op_user_new_arena_mail(std::string& strAppendHere, gs_dbs_user_new_arena_mail_req* req );
	
	//添加或者修改子任务完成信息
	int add_or_update_sub_task(UINT32 user_id, gs_dbs_user_info_op_req_struct_op_sub_task* sub_task);
	int _get_sql_of_add_or_update_sub_task(std::string& strAppendHere, UINT32 user_id, gs_dbs_user_info_op_req_struct_op_sub_task* sub_task );
	//int delete_sub_task(UINT32 user_id, db_user_info* user_info);

	int op_hero(UINT32 user_id, const ::google::protobuf::RepeatedPtrField< ::gs_dbs_user_info_op_req_struct_op_hero >& op_heros);

	int new_hero(UINT32 user_id, const ::google::protobuf::RepeatedPtrField< ::gs_dbs_user_info_op_req_struct_new_hero >& op_heros);
	int _get_sql_of_new_hero(std::string& strAppendHere, UINT32 user_id, const ::google::protobuf::RepeatedPtrField< ::gs_dbs_user_info_op_req_struct_new_hero >& op_heros );

	int op_hero_base(UINT32 user_id, const ::google::protobuf::RepeatedPtrField< ::gs_dbs_user_info_op_req_struct_op_hero_base >& op_base_heros);
	int _get_sql_of_op_hero_base(std::string& strAppendHere, UINT32 user_id, const ::google::protobuf::RepeatedPtrField< ::gs_dbs_user_info_op_req_struct_op_hero_base >& op_base_heros );

	int op_hero_equip(UINT32 user_id, const ::google::protobuf::RepeatedPtrField< ::gs_dbs_user_info_op_req_struct_op_hero_equip >& op_hero_equips);
	int _get_sql_of_op_hero_equip(std::string& strAppendHere, UINT32 user_id, const ::google::protobuf::RepeatedPtrField< ::gs_dbs_user_info_op_req_struct_op_hero_equip >& op_hero_equips );

	int op_hero_skill(UINT32 user_id, const ::google::protobuf::RepeatedPtrField< ::gs_dbs_user_info_op_req_struct_op_hero_skill >& op_hero_skills);
	int _get_sql_of_op_hero_skill(std::string& strAppendHere, UINT32 user_id, const ::google::protobuf::RepeatedPtrField< ::gs_dbs_user_info_op_req_struct_op_hero_skill >& op_hero_skills );

	int op_mail(UINT32 user_id, const ::google::protobuf::RepeatedPtrField< ::gs_dbs_user_info_op_req_struct_op_mail >& rep_op_mail);
	//int op_received_sys_mail(UINT32 user_id, const gs_dbs_user_info_op_req::struct_add_received_sys_mail& op_add_received_sys_mail);
	int _get_sql_of_op_mail(std::string& strAppendHere, UINT32 user_id, const ::google::protobuf::RepeatedPtrField< ::gs_dbs_user_info_op_req_struct_op_mail >& rep_op_mail);


	int add_or_update_game_element(UINT32 user_id, const struct_game_element& ele);
	
	int add_or_update_game_element(UINT32 user_id, const ::google::protobuf::RepeatedPtrField< ::struct_game_element >& eles);
	int _get_sql_of_add_or_update_game_element(std::string& strAppendHere, UINT32 user_id, const ::google::protobuf::RepeatedPtrField< ::struct_game_element >& eles);

	//商店创建或者刷新
	int add_or_update_shop( UINT32 user_id, const ::google::protobuf::RepeatedPtrField< gs_dbs_user_info_op_req::struct_op_shop >& eles );
	int _get_sql_of_add_or_update_shop(std::string& strAppendHere, UINT32 user_id, const ::google::protobuf::RepeatedPtrField< gs_dbs_user_info_op_req::struct_op_shop >& eles );

	//添加或者更新玩家商店物品
	int add_or_update_shop_item( UINT32 user_id, const ::google::protobuf::RepeatedPtrField< gs_dbs_user_info_op_req::struct_op_shop_item >& eles );
	int _get_sql_of_add_or_update_shop_item(std::string& strAppendHere, UINT32 user_id, const ::google::protobuf::RepeatedPtrField< gs_dbs_user_info_op_req::struct_op_shop_item >& eles );

	//添加或者更新玩家购买记录
	int add_or_update_shop_buy(UINT32 user_id, const ::google::protobuf::RepeatedPtrField< ::db_user_shop_buy >& eles);
	int _get_sql_of_add_or_update_shop_buy(std::string& strAppendHere, UINT32 user_id, const ::google::protobuf::RepeatedPtrField< ::db_user_shop_buy >& eles);

	//添加或更新玩家任务信息(任务事件,任务链信息)
	int add_or_update_mission(UINT32 user_id, const ::google::protobuf::RepeatedPtrField< ::gs_dbs_user_info_op_req_struct_op_mission >& eles);
	int _get_sql_of_add_or_update_mission(std::string& strAppendHere, UINT32 user_id, const ::google::protobuf::RepeatedPtrField< ::gs_dbs_user_info_op_req_struct_op_mission >& eles);
	
	int search_user(const gs_dbs_search_user_req& search_req, dbs_gs_search_user_res& rRes);

	int op_friend(UINT32 user_id, const ::google::protobuf::RepeatedPtrField< ::gs_dbs_user_info_op_req_struct_op_friend >& friends);
	int _get_sql_of_op_friend(std::string& strAppendHere, UINT32 user_id, const ::google::protobuf::RepeatedPtrField< ::gs_dbs_user_info_op_req_struct_op_friend >& friends);

	int op_friend(UINT32 user_id, const gs_dbs_user_friend_op_req& friend_op);
	int _get_sql_of_op_friend(std::string& strAppendHere, UINT32 user_id, const gs_dbs_user_friend_op_req& friend_op );
	
	int add_friend_offline(gs_dbs_friend_offline_req& req);
	int affirm_agree_offline( gs_dbs_friend_offline_req& req );

	int get_rank(dbs_gs_rank_res& res);

	int _get_sql_of_op_crusade(std::string& strAppendHere, UINT32, const gs_dbs_user_info_op_req_struct_op_crusade& op_crusade);
//private:

	//竞技场相关
	//添加全量玩家竞技场信
	int add_arena(UINT32 user_id, const gs_dbs_user_info_op_req::struct_new_arena& new_arena );
	int _get_sql_of_add_arena(std::string& strAppendHere, UINT32 user_id, const gs_dbs_user_info_op_req::struct_new_arena& new_arena );

	//更新玩家竞技场信息 挑战者的排名在这里更新 防守者的排名在op_arena_challenge更新
	int update_arena(UINT32 user_id, const gs_dbs_user_info_op_req::struct_op_arena& op_arena );
	int _get_sql_of_update_arena(std::string& strAppendHere, UINT32 user_id, const gs_dbs_user_info_op_req::struct_op_arena& op_arena );

	//此时竞技场信息一定存在 在逻辑层保证, 这里只作update
	//int op_arena_team(UINT32 user_id, const gs_dbs_user_info_op_req::struct_op_arena_team& op_arena_team );
	int clear_arena_team(UINT32 user_id);
	int _get_sql_of_clear_arena_team(std::string& strAppendHere, UINT32 user_id );

	//添加竞技场挑战结果 给内存双方添加日志 并只给被挑战者修改名次
	//给db 添加一条日志 并只给被挑战者修改名次
	int op_arena_challenge(UINT32 user_id, const gs_dbs_user_info_op_req::struct_op_arena_challenge& op_arena_challenge );
	int _get_sql_of_op_arena_challenge(std::string& strAppendHere, UINT32 user_id, const gs_dbs_user_info_op_req::struct_op_arena_challenge& op_arena_challenge );

	int add_sign(UINT32 user_id, const gs_dbs_user_info_op_req::struct_new_sign& new_sign);
	int _get_sql_of_add_sign(std::string& strAppendHere, UINT32 user_id, const gs_dbs_user_info_op_req::struct_new_sign& new_sign);

	int update_sign(UINT32 user_id, const gs_dbs_user_info_op_req::struct_op_sign& op_sign);
	int _get_sql_of_update_sign(std::string& strAppendHere, UINT32 user_id, const gs_dbs_user_info_op_req::struct_op_sign& op_sign );

	int add_novice_guide(UINT32 user_id, const gs_dbs_user_info_op_req::struct_op_novice_guide& op_guide);
	int _get_sql_of_add_novice_guide(std::string& strAppendHere, UINT32 user_id, const gs_dbs_user_info_op_req::struct_op_novice_guide& op_guide );


	int add_new_user_statis(UINT32 user_id, const gs_dbs_user_info_op_req::struct_new_user_statis& new_statis);
	int _get_sql_of_add_new_user_statis(std::string& strAppendHere, UINT32 user_id, const gs_dbs_user_info_op_req::struct_new_user_statis& new_statis);

	int edit_user_name(UINT32 user_id, const char* name);
	
	//private:
	int _new_hero(char* &p, int &left, UINT32 user_id, const db_user_hero& hero);
	int _add_or_update_game_element(char* &p, int& left, UINT32 user_id, const struct_game_element& ele);

	int _add_or_update_shop_item(char* &p, int& left, UINT32 user_id, UINT32 shop_type, const db_user_shop_item& ele);
	int _add_or_update_shop(char* &p, int& left, UINT32 user_id, const db_user_shop& shop, gs_dbs_user_info_op_req::struct_op_shop::ESHOPOPType op_type);
	int _add_or_update_shop_buy(char* &p, int& left, UINT32 user_id, const db_user_shop_buy& ele);
}



#endif // db_sync_operator_h__
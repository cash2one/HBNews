#include "db_async_command.h"
#include "db_sync_operator.h"
#include "server_for_gs.h"
#include "libuv_helper.h"
#include "mail_mgr.h"
bool CUserCommand::do_get_info()
{
	stDBGetUserInfoRet* ret = new stDBGetUserInfoRet;

	ret->user_info = new db_user_info;
	ret->ret = (stDBGetUserInfoRet::EGetUserInfoRet)db_sync_operator::get_user_info(m_user_id, ret->user_info);

	stGSAsyncData* asyncData = new stGSAsyncData();
	ASSERT(asyncData);
	asyncData->op = stGSAsyncData::EGSAsyncOP_User_Info_Res;
	asyncData->gs_connect_id = m_gs_connect_id;
	asyncData->user_connect_id = m_user_connect_id;
	asyncData->user_id = m_user_id;
	//asyncData->data = ret;

	//
	if (ret->ret == stDBGetUserInfoRet::Success)
	{
		printf("成功从DB中拉取到用户数据[%u]！\n", m_user_id);
		asyncData->data = ret;
	}
	else if(ret->ret == stDBGetUserInfoRet::Not_Exist_User){
		delete ret->user_info;
		ret->user_info = NULL;
		asyncData->data = ret;
	}
	else {
		//todo log error!!!
		delete ret->user_info;
		ret->user_info = NULL;
		asyncData->data = ret;
	}

	uv_async_t* async = CServerForGS::GetInstance()->CreateAsync();
	ASSERT(async);
	async->data = asyncData; 
	int r = uv_async_send(async);
	ASSERT(r == 0);
	if (r != 0)
	{
		PRINTF_UV_ERROR(r);
		delete ret->user_info;
		delete ret;
		delete asyncData;
		async->data = NULL;
		CServerForGS::GetInstance()->DestroyAsync(async);
	}

	return r == 0;
}

bool CUserCommand::do_op_name(){
	gs_dbs_edit_name_req* req = (gs_dbs_edit_name_req*)data;
	stDBEditUserNameRet* editRet = new stDBEditUserNameRet;
	editRet->ret = (stDBEditUserNameRet::EEditUserNameRet)db_sync_operator::edit_user_name(m_user_id, req->name().c_str()); 


	stGSAsyncData* asyncData = new stGSAsyncData;
	ASSERT(asyncData);
	asyncData->op = stGSAsyncData::EGSAsyncOP_User_Edit_User_Name_Res;
	asyncData->user_id = m_user_id;
	if(stDBEditUserNameRet::Success == editRet->ret)
	{
		strncpy(editRet->name, req->name().c_str(), sizeof(editRet->name));
	}
	asyncData->data = editRet;
	asyncData->gs_connect_id = m_gs_connect_id;
	uv_async_t* async = CServerForGS::GetInstance()->CreateAsync();
	ASSERT(async);
	async->data = asyncData;
	int r = uv_async_send(async);
	ASSERT(r == 0);
	if (r != 0)
	{
		PRINTF_UV_ERROR(r);
		delete editRet;
		delete asyncData;
		async->data = NULL;
		CServerForGS::GetInstance()->DestroyAsync(async);
	}

	delete req;
	return r == 0;
}

bool CUserCommand::GetSql(std::string& strAppendHere){
	//if (!CanCombine()){
	//	return false;
	//}

	int r = 0;
	switch ((int)m_optype)
	{
		case OP_INFO:{
			r = get_sql_of_op_info(strAppendHere);
			ASSERT(r);
			CHECK_FALSE_RETURN_FALSE(r);
		}break;
		case New_Create:{
			r = get_sql_of_new_create(strAppendHere);
			ASSERT(r);
			CHECK_FALSE_RETURN_FALSE(r);
		}break;
		case OP_ARENA_MAIL:{
			r = get_sql_of_op_arena_mail(strAppendHere);
			ASSERT(r);
			CHECK_FALSE_RETURN_FALSE(r);
		}break;
		case OP_FRIEND:{
			r = get_sql_of_op_friend(strAppendHere);
			ASSERT(r);
			CHECK_FALSE_RETURN_FALSE(r);
		}break;
	
		default:
			ASSERT(false);
			return false;
	}

	return true;
}

bool CUserCommand::do_op_info()
{
	gs_dbs_user_info_op_req* req = (gs_dbs_user_info_op_req*)data;
	ASSERT(req);

	bool r = true;

	//玩家基本信息
	if (req->has_base_info()){
		r &= op_base_info();
		ASSERT(r);
	}

	//任务信息
	if(req->has_sub_task()){
		r &= op_sub_task();
		ASSERT(r);
	}
	
	//邮件信息
	if (req->op_mails_size()){
		r &= op_mail();
		ASSERT(r);
	}
	
	//道具信息
	if (req->items_size()){
		r &= op_game_element();
		ASSERT(r);
	}

	//邮件信息 有独立的函数

	//英雄操作
	r &= op_hero();
	ASSERT(r);

	//商店信息
	if(req->shops_size()){
		r &= op_shop();
		ASSERT(r);
	}

	//商店道具信息
	if(req->shop_items_size()){
		r &= op_shop_items();
		ASSERT(r);
	}

	//道具购买信息
	if(req->buy_items_size()){
		r &= op_shop_buy();
		ASSERT(r);
	}
	
	//任务信息
	if (req->missions_size()){
		r &= op_mission();
		ASSERT(r);
	}

	//竞技场信息
	if(req->has_new_arena()){
		r &= (0 == db_sync_operator::add_arena(req->user_id(), req->new_arena()));
	}

	if(req->has_op_arena()){
		r &= (0 == db_sync_operator::update_arena(req->user_id(), req->op_arena()));
	}

	if (req->has_clear_arena_team()){
		r &= (0 == db_sync_operator::clear_arena_team(req->user_id()));
	}

	if (req->has_arena_challenge()){
		r &= (0 == db_sync_operator::op_arena_challenge(req->user_id(), req->arena_challenge()));
	}
	
	if(req->has_new_sign()){
		r &= (0 == db_sync_operator::add_sign(req->user_id(), req->new_sign()));
	}

	if(req->has_sign()){
		r &= (0 == db_sync_operator::update_sign(req->user_id(), req->sign()));
	}
	
	if (req->has_novice_guide()){
		r &=  (0 == db_sync_operator::add_novice_guide(req->user_id(), req->novice_guide()));
	}

	//玩家成长信息
	if (req->has_new_user_statis()){
		r &=  (0 == db_sync_operator::add_new_user_statis(req->user_id(), req->new_user_statis()));
	}

	//朋友信息
	if (req->op_friends_size())
	{
		r &= (0 == db_sync_operator::op_friend(req->user_id(), req->op_friends()));
		ASSERT(r);
	}
	//其他的信息
	
	delete req;
	return r;
}

bool CUserCommand::get_sql_of_op_info( std::string& strAppendHere )
{
	gs_dbs_user_info_op_req* req = (gs_dbs_user_info_op_req*)data;
	ASSERT(req);

	bool r = true;
	//玩家基本信息
	if (req->has_base_info()){
		r = _get_sql_of_op_base_info(strAppendHere);
		ASSERT(r);
		CHECK_FALSE_RETURN_FALSE(r);
	}

	//任务信息
	if(req->has_sub_task()){
		r = _get_sql_of_op_sub_task(strAppendHere);
		ASSERT(r);
		CHECK_FALSE_RETURN_FALSE(r);
	}

	//邮件信息
	if (req->op_mails_size()){
		r = _get_sql_of_op_mail(strAppendHere);
		ASSERT(r);
		CHECK_FALSE_RETURN_FALSE(r);
	}

	//道具信息
	if (req->items_size()){
		r = _get_sql_of_op_game_element(strAppendHere);
		ASSERT(r);
		CHECK_FALSE_RETURN_FALSE(r);
	}

	//邮件信息 有独立的函数

	//英雄操作
	r = _get_sql_of_op_hero(strAppendHere);
	ASSERT(r);
	CHECK_FALSE_RETURN_FALSE(r);

	//商店信息
	if(req->shops_size()){
		r = _get_sql_of_op_shop(strAppendHere);
		ASSERT(r);
		CHECK_FALSE_RETURN_FALSE(r);
	}

	//商店道具信息
	if(req->shop_items_size()){
		r = _get_sql_of_op_shop_items(strAppendHere);
		ASSERT(r);
		CHECK_FALSE_RETURN_FALSE(r);
	}

	//道具购买信息
	if(req->buy_items_size()){
		r = _get_sql_of_op_shop_buy(strAppendHere);
		ASSERT(r);
		CHECK_FALSE_RETURN_FALSE(r);
	}

	//任务信息
	if (req->missions_size()){
		r = _get_sql_of_op_mission(strAppendHere);
		ASSERT(r);
		CHECK_FALSE_RETURN_FALSE(r);
	}

	//竞技场信息
	if(req->has_new_arena()){
		r = (0 == db_sync_operator::_get_sql_of_add_arena(strAppendHere, req->user_id(), req->new_arena()));
		ASSERT(r);
		CHECK_FALSE_RETURN_FALSE(r);
	}

	if(req->has_op_arena()){
		r = (0 == db_sync_operator::_get_sql_of_update_arena(strAppendHere, req->user_id(), req->op_arena()));
		ASSERT(r);
		CHECK_FALSE_RETURN_FALSE(r);
	}

	if (req->has_clear_arena_team()){
		r = (0 == db_sync_operator::_get_sql_of_clear_arena_team(strAppendHere, req->user_id()));
		ASSERT(r);
		CHECK_FALSE_RETURN_FALSE(r);
	}

	if (req->has_arena_challenge()){
		r = (0 == db_sync_operator::_get_sql_of_op_arena_challenge(strAppendHere, req->user_id(), req->arena_challenge()));
		ASSERT(r);
		CHECK_FALSE_RETURN_FALSE(r);
	}

	if(req->has_new_sign()){
		r = (0 == db_sync_operator::_get_sql_of_add_sign(strAppendHere, req->user_id(), req->new_sign()));
		ASSERT(r);
		CHECK_FALSE_RETURN_FALSE(r);
	}

	if(req->has_sign()){
		r = (0 == db_sync_operator::_get_sql_of_update_sign(strAppendHere, req->user_id(), req->sign()));
		ASSERT(r);
		CHECK_FALSE_RETURN_FALSE(r);
	}

	if (req->has_novice_guide()){
		r = (0 == db_sync_operator::_get_sql_of_add_novice_guide(strAppendHere, req->user_id(), req->novice_guide()));
		ASSERT(r);
		CHECK_FALSE_RETURN_FALSE(r);
	}

	//玩家成长信息
	if (req->has_new_user_statis()){
		r = (0 == db_sync_operator::_get_sql_of_add_new_user_statis(strAppendHere, req->user_id(), req->new_user_statis()));
		ASSERT(r);
		CHECK_FALSE_RETURN_FALSE(r);
	}

	//朋友信息
	if (req->op_friends_size()){
		r = (0 == db_sync_operator::_get_sql_of_op_friend(strAppendHere, req->user_id(), req->op_friends()));
		ASSERT(r);
		CHECK_FALSE_RETURN_FALSE(r);
	}

	//远征信息
	if (req->has_op_crusade())
	{
		r = (0 == db_sync_operator::_get_sql_of_op_crusade(strAppendHere, req->user_id(), req->op_crusade()));
		ASSERT(r);
		CHECK_FALSE_RETURN_FALSE(r);
	}

	//其他的信息
	delete req;
	return r;
}

bool CUserCommand::do_new_create()
{
	db_user_info* new_user = (db_user_info*)data;
	ASSERT(new_user && new_user->has_user_base_info());

	int r = db_sync_operator::new_create_user(new_user);
	//delete new_user;
	ASSERT(r == 0);
	return r == 0;
}

bool CUserCommand::get_sql_of_new_create(std::string& strAppendHere)
{
	db_user_info* new_user = (db_user_info*)data;
	ASSERT(new_user && new_user->has_user_base_info());

	int r = db_sync_operator::_get_sql_of_new_create_user(strAppendHere, new_user);
	//delete new_user;
	ASSERT(r == 0);
	return r == 0;
}

bool CUserCommand::do_op_mail()
{
	stAddMail* mail = (stAddMail*)data;
	ASSERT(mail);
	int r = db_sync_operator::op_user_new_mail(mail);
	ASSERT(r == 0);

	delete mail;          
	return true;
}

bool CUserCommand::do_op_arena_mail()
{
	gs_dbs_user_new_arena_mail_req* req = (gs_dbs_user_new_arena_mail_req*)data;
	ASSERT(req);
	int r = db_sync_operator::op_user_new_arena_mail(req);
	ASSERT(r == 0);
	delete req;
	return true;
}

bool CUserCommand::get_sql_of_op_arena_mail(std::string& strAppendHere)
{
	gs_dbs_user_new_arena_mail_req* req = (gs_dbs_user_new_arena_mail_req*)data;
	ASSERT(req);
	int r = db_sync_operator::_get_sql_of_op_user_new_arena_mail(strAppendHere, req);
	ASSERT(r == 0);
	delete req;
	return true;
}


bool CUserCommand::op_base_info(){
	gs_dbs_user_info_op_req* req = (gs_dbs_user_info_op_req*)data;
	ASSERT(req);

	gs_dbs_user_info_op_req_struct_op_base_info* data = req->mutable_base_info();
	ASSERT(data);

	if (data->op_type() == gs_dbs_user_info_op_req_EOPType_Update)
	{
		int r = db_sync_operator::update_base_info(m_user_id, data);
		ASSERT(r == 0);
	}
	else{
		//暂时没有这种需求
		ASSERT(false);
		return false;
	}

	return true;
}

bool CUserCommand::_get_sql_of_op_base_info(std::string& strAppendHere){
	gs_dbs_user_info_op_req* req = (gs_dbs_user_info_op_req*)data;
	ASSERT(req);
	CHECK_NULL_RETURN_FALSE(req);

	gs_dbs_user_info_op_req_struct_op_base_info* data = req->mutable_base_info();
	ASSERT(data);
	CHECK_NULL_RETURN_FALSE(data);

	if (data->op_type() == gs_dbs_user_info_op_req_EOPType_Update){
		int r = db_sync_operator::_get_sql_of_update_base_info(strAppendHere, m_user_id, data);
		ASSERT(r == 0);
		CHECK_FALSE_RETURN_FALSE(r == 0);
	}
	else{
		//暂时没有这种需求
		ASSERT(false);
		return false;
	}

	return true;
}

bool CUserCommand::op_sub_task()
{
	gs_dbs_user_info_op_req* req = (gs_dbs_user_info_op_req*)data;
	ASSERT(req);

	gs_dbs_user_info_op_req_struct_op_sub_task* op_sub_task = req->mutable_sub_task();
	ASSERT(op_sub_task);

	if (op_sub_task->op_type() == gs_dbs_user_info_op_req_EOPType_Add_Or_Update)
	{
		int r = db_sync_operator::add_or_update_sub_task(m_user_id, op_sub_task);
		ASSERT(r == 0);
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

bool CUserCommand::_get_sql_of_op_sub_task(std::string& strAppendHere)
{
	gs_dbs_user_info_op_req* req = (gs_dbs_user_info_op_req*)data;
	ASSERT(req);

	gs_dbs_user_info_op_req_struct_op_sub_task* op_sub_task = req->mutable_sub_task();
	ASSERT(op_sub_task);

	if (op_sub_task->op_type() == gs_dbs_user_info_op_req_EOPType_Add_Or_Update)
	{
		int r = db_sync_operator::_get_sql_of_add_or_update_sub_task(strAppendHere, m_user_id, op_sub_task);
		ASSERT(r == 0);
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

bool CUserCommand::op_hero()
{
	gs_dbs_user_info_op_req* req = (gs_dbs_user_info_op_req*)data;
	ASSERT(req);

	int r = db_sync_operator::new_hero(m_user_id, req->new_heros());
	ASSERT(r == 0);

	r = db_sync_operator::op_hero_base(m_user_id, req->heros_base());
	ASSERT(r == 0);

	r = db_sync_operator::op_hero_equip(m_user_id, req->hero_equips());
	ASSERT(r == 0);

	r = db_sync_operator::op_hero_skill(m_user_id, req->hero_skills());
	ASSERT(r == 0);

	return r == 0;
}

bool CUserCommand::_get_sql_of_op_hero(std::string& strAppendHere)
{
	gs_dbs_user_info_op_req* req = (gs_dbs_user_info_op_req*)data;
	ASSERT(req);
	CHECK_FALSE_RETURN_FALSE(req);

	int r = db_sync_operator::_get_sql_of_new_hero(strAppendHere, m_user_id, req->new_heros());
	ASSERT(r == 0);
	CHECK_FALSE_RETURN_FALSE(r == 0);

	r = db_sync_operator::_get_sql_of_op_hero_base(strAppendHere, m_user_id, req->heros_base());
	ASSERT(r == 0);
	CHECK_FALSE_RETURN_FALSE(r == 0);

	r = db_sync_operator::_get_sql_of_op_hero_equip(strAppendHere, m_user_id, req->hero_equips());
	ASSERT(r == 0);
	CHECK_FALSE_RETURN_FALSE(r == 0);

	r = db_sync_operator::_get_sql_of_op_hero_skill(strAppendHere, m_user_id, req->hero_skills());
	ASSERT(r == 0);
	CHECK_FALSE_RETURN_FALSE(r == 0);

	return r == 0;
}


bool CUserCommand::op_mail()
{
	gs_dbs_user_info_op_req* req = (gs_dbs_user_info_op_req*)data;
	ASSERT(req);
	
	int r = db_sync_operator::op_mail(m_user_id, req->op_mails());
	ASSERT(r == 0);
	return r == 0;
}

bool CUserCommand::_get_sql_of_op_mail(std::string& strAppendHere)
{
	gs_dbs_user_info_op_req* req = (gs_dbs_user_info_op_req*)data;
	ASSERT(req);

	int r = db_sync_operator::_get_sql_of_op_mail(strAppendHere, m_user_id, req->op_mails());
	ASSERT(r == 0);
	return r == 0;
}

bool CUserCommand::op_game_element()
{
	gs_dbs_user_info_op_req* req = (gs_dbs_user_info_op_req*)data;
	ASSERT(req);

	//::google::protobuf::RepeatedPtrField< ::struct_game_element >::iterator iter = req->mutable_items()->begin();
	//for (; iter != req->mutable_items()->end(); ++iter)
	//{
	//	int r = db_sync_operator::add_or_update_game_element(m_user_id, *iter);
	//	ASSERT(r == 0);
	//}

	int r = db_sync_operator::add_or_update_game_element(m_user_id, req->items());
	ASSERT(r == 0);
	return r == 0;
}

bool CUserCommand::_get_sql_of_op_game_element(std::string& strAppendHere)
{
	gs_dbs_user_info_op_req* req = (gs_dbs_user_info_op_req*)data;
	ASSERT(req);

	//::google::protobuf::RepeatedPtrField< ::struct_game_element >::iterator iter = req->mutable_items()->begin();
	//for (; iter != req->mutable_items()->end(); ++iter)
	//{
	//	int r = db_sync_operator::add_or_update_game_element(m_user_id, *iter);
	//	ASSERT(r == 0);
	//}

	int r = db_sync_operator::_get_sql_of_add_or_update_game_element(strAppendHere, m_user_id, req->items());
	ASSERT(r == 0);
	return r == 0;
}

bool CUserCommand::op_shop()
{
	gs_dbs_user_info_op_req* req = (gs_dbs_user_info_op_req*)data;
	ASSERT(req);

	//::google::protobuf::RepeatedPtrField< ::struct_game_element >::iterator iter = req->mutable_items()->begin();
	//for (; iter != req->mutable_items()->end(); ++iter)
	//{
	//	int r = db_sync_operator::add_or_update_game_element(m_user_id, *iter);
	//	ASSERT(r == 0);
	//}

	int r = db_sync_operator::add_or_update_shop(m_user_id, req->shops());
	ASSERT(r == 0);
	return r == 0;
}

bool CUserCommand::_get_sql_of_op_shop(std::string& strAppendHere)
{
	gs_dbs_user_info_op_req* req = (gs_dbs_user_info_op_req*)data;
	ASSERT(req);

	//::google::protobuf::RepeatedPtrField< ::struct_game_element >::iterator iter = req->mutable_items()->begin();
	//for (; iter != req->mutable_items()->end(); ++iter)
	//{
	//	int r = db_sync_operator::add_or_update_game_element(m_user_id, *iter);
	//	ASSERT(r == 0);
	//}

	int r = db_sync_operator::_get_sql_of_add_or_update_shop(strAppendHere, m_user_id, req->shops());
	ASSERT(r == 0);
	return r == 0;
}

bool CUserCommand::op_shop_items()
{
	gs_dbs_user_info_op_req* req = (gs_dbs_user_info_op_req*)data;
	ASSERT(req);

	int r = db_sync_operator::add_or_update_shop_item(m_user_id, req->shop_items());
	ASSERT(r == 0);
	return r == 0;
}

bool CUserCommand::_get_sql_of_op_shop_items(std::string& strAppendHere)
{
	gs_dbs_user_info_op_req* req = (gs_dbs_user_info_op_req*)data;
	ASSERT(req);

	//::google::protobuf::RepeatedPtrField< ::struct_game_element >::iterator iter = req->mutable_items()->begin();
	//for (; iter != req->mutable_items()->end(); ++iter)
	//{
	//	int r = db_sync_operator::add_or_update_game_element(m_user_id, *iter);
	//	ASSERT(r == 0);
	//}

	int r = db_sync_operator::_get_sql_of_add_or_update_shop_item(strAppendHere, m_user_id, req->shop_items());
	ASSERT(r == 0);
	return r == 0;
}


bool CUserCommand::op_shop_buy()
{
	gs_dbs_user_info_op_req* req = (gs_dbs_user_info_op_req*)data;
	ASSERT(req);

	int r = db_sync_operator::add_or_update_shop_buy(m_user_id, req->buy_items());
	ASSERT(r == 0);
	return r == 0;
}


bool CUserCommand::_get_sql_of_op_shop_buy(std::string& strAppendHere)
{
	gs_dbs_user_info_op_req* req = (gs_dbs_user_info_op_req*)data;
	ASSERT(req);

	int r = db_sync_operator::_get_sql_of_add_or_update_shop_buy(strAppendHere, m_user_id, req->buy_items());
	ASSERT(r == 0);
	return r == 0;
}

bool CUserCommand::op_mission()
{
	gs_dbs_user_info_op_req* req = (gs_dbs_user_info_op_req*)data;
	ASSERT(req);

	int r = db_sync_operator::add_or_update_mission(m_user_id, req->missions());
	ASSERT(r == 0);

	return true;
}

bool CUserCommand::_get_sql_of_op_mission(std::string& strAppendHere)
{
	gs_dbs_user_info_op_req* req = (gs_dbs_user_info_op_req*)data;
	ASSERT(req);

	int r = db_sync_operator::_get_sql_of_add_or_update_mission(strAppendHere, m_user_id, req->missions());
	ASSERT(r == 0);

	return true;
}

bool CUserCommand::do_search_user()
{
	gs_dbs_search_user_req* req = (gs_dbs_search_user_req*)data;
	ASSERT(req);

	dbs_gs_search_user_res* res = new dbs_gs_search_user_res;
	int ret = db_sync_operator::search_user(*req, *res);
	
	stGSAsyncData* asyncData = new stGSAsyncData();
	ASSERT(asyncData);
	asyncData->op = stGSAsyncData::EGSAsyncOP_User_Search_User;
	asyncData->gs_connect_id = m_gs_connect_id;
	//asyncData->user_connect_id = m_user_connect_id;
	asyncData->user_id = m_user_id;
	asyncData->data = res;
	uv_async_t* async = CServerForGS::GetInstance()->CreateAsync();
	ASSERT(async);
	async->data = asyncData; 
	int r = uv_async_send(async);
	ASSERT(r == 0);
	if (r != 0)
	{
		PRINTF_UV_ERROR(r);
		delete res;
		delete asyncData;
		asyncData->data = NULL;
		CServerForGS::GetInstance()->DestroyAsync(async);
		return false;
	}

	delete req;
	return true;
}

bool CUserCommand::do_op_rank()
{
	dbs_gs_rank_res* res = new dbs_gs_rank_res;
	int code = db_sync_operator::get_rank(*res);
	if (-1 == code)
	{
		res->set_error_code(e_dbsgs_errorcode_unkown_error);
	}
	else if(0 == code){
		res->set_error_code(e_dbsgs_errorcode_success);
	}else{
		res->set_error_code(e_dbsgs_errorcode_unkown_error);
	}

	stGSAsyncData* asyncData = new stGSAsyncData();
	ASSERT(asyncData);
	asyncData->op = stGSAsyncData::EGSAsyncOP_Rank_res;
	asyncData->gs_connect_id = m_gs_connect_id;
	//asyncData->user_connect_id = m_user_connect_id;
	//asyncData->user_id = m_user_id;
	asyncData->data = res;
	uv_async_t* async = CServerForGS::GetInstance()->CreateAsync();
	ASSERT(async);
	async->data = asyncData; 
	int r = uv_async_send(async);
	ASSERT(r == 0);
	if (r != 0)
	{
		PRINTF_UV_ERROR(r);
		delete res;
		delete asyncData;
		asyncData->data = NULL;
		CServerForGS::GetInstance()->DestroyAsync(async);
		return false;
	}

	return true;
}

bool CUserCommand::do_op_friend_offline()
{
	stDBFriendOffLineRet* ret = new stDBFriendOffLineRet;

	gs_dbs_friend_offline_req* req = (gs_dbs_friend_offline_req*)data;
	ASSERT(req);
	switch(req->op_type()){
		case gs_dbs_friend_offline_req::ADD_FRIEND:
			ret->ret = (stDBFriendOffLineRet::EFriendOffLineRet)db_sync_operator::add_friend_offline(*req);
			break;
		case gs_dbs_friend_offline_req::AFFIRM_AGREE1:
		case gs_dbs_friend_offline_req::AFFIRM_AGREEN:
			ret->ret = (stDBFriendOffLineRet::EFriendOffLineRet)db_sync_operator::affirm_agree_offline(*req);
			break;
		default:
			ASSERT(false);
			return false;
	}
	
	ret->req.CopyFrom(*req);

	stGSAsyncData* asyncData = new stGSAsyncData();
	ASSERT(asyncData);
	asyncData->op = stGSAsyncData::EGSAsyncOP_User_Friend_OffLine_Res;
	asyncData->gs_connect_id = m_gs_connect_id;
	asyncData->user_id = m_user_id;
	asyncData->data = ret;

	uv_async_t* async = CServerForGS::GetInstance()->CreateAsync();
	ASSERT(async);
	async->data = asyncData; 
	int r = uv_async_send(async);
	ASSERT(r == 0);
	if (r != 0)
	{
		PRINTF_UV_ERROR(r);
		delete ret;
		delete asyncData;
		async->data = NULL;
		CServerForGS::GetInstance()->DestroyAsync(async);
		return false;
	}

	delete req;
	return true;
}

bool CUserCommand::do_op_friend()
{
	gs_dbs_user_friend_op_req* req = (gs_dbs_user_friend_op_req*)data;
	ASSERT(req);

	int r = db_sync_operator::op_friend(m_user_id, *req);
	ASSERT(r == 0);

	delete req;
	return true;
}

bool CUserCommand::get_sql_of_op_friend(std::string& strAppendHere)
{
	gs_dbs_user_friend_op_req* req = (gs_dbs_user_friend_op_req*)data;
	ASSERT(req);

	int r = db_sync_operator::_get_sql_of_op_friend(strAppendHere, m_user_id, *req);
	ASSERT(r == 0);

	delete req;
	return true;
}


#ifndef db_sync_operator_h__
#define db_sync_operator_h__
#pragma once

#include <map>
#include <string>
#include "as_c_proto.pb.h"
#include "def.h"

struct CArea;

namespace db_sync_operator{

	//int register_account(const as_c_register_req& req, UINT32& user_id);
	int register_account(const UINT32 plantform_id, const UINT32 plantform_sub_id, const std::string& account, const std::string& password, UINT32& ret_user_id);
	//int login(const as_c_login_req& req, UINT32& user_id);
	int login(const UINT32 plantform_id, const UINT32 plantform_sub_id, const std::string& account, const std::string& password, UINT32& ret_user_id);

	//如果不存在账户则创建
	int login_if_no_register(const UINT32 plantform_id, const UINT32 plantform_sub_id, const std::string& account, const std::string& password, UINT32& ret_user_id);

	//初始化的时候做这个操作
	//int get_area_list(std::map<UINT32, CArea*> &areas);
}


#endif // db_sync_operator_h__
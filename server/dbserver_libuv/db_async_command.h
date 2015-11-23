#ifndef db_async_command_h__
#define db_async_command_h__

#pragma once

#include "async_command_module.h"
#include "guid.h"
#include "dbs_gs_proto.pb.h"
struct stDBGetUserInfoRet{
	enum EGetUserInfoRet{
		Success = 0,
		System_Error = 1,
		Not_Exist_User = 2,
		Exist_But_has_Error = 3
	};

	EGetUserInfoRet ret;
	db_user_info* user_info;
};

struct stDBEditUserNameRet{
	enum EEditUserNameRet{
		System_Error = 0,
		Success = 1,
		Same_Name = 2,//同名
	};
	EEditUserNameRet ret;
	char name[63];//MAXLEN_USERNAME
};

struct stDBFriendOffLineRet{
	enum EFriendOffLineRet{
		System_Error = 0,
		SUCCESS = 1,
		FRIEND_CONFIRM_FULL = 2,
		Unkown_Error = 3,
		FRIEND_NUM_REACH_MAX = 4,
	};
	EFriendOffLineRet ret;
	gs_dbs_friend_offline_req req;
};

class CUserCommand : public ICommand{
public:
	enum OPTYPE{
		NONE = 0,
		GET_INFO = 1,
		OP_INFO = 2,	//
		New_Create = 3,	//新建玩家基本信息
		OP_MAIL = 4,
		OP_NAME = 5,
		OP_ARENA_MAIL = 6,
		SEARCH_USER = 7,
		OP_FRIEND_OFFLINE = 8,
		OP_FRIEND = 9,
		OP_RANK = 10
	};

	virtual void OnExecute(){
		switch ((int)m_optype)
		{
			case GET_INFO:{
				do_get_info();
			}break;
			case OP_INFO:{
				do_op_info();
			}break;
			case New_Create:{
				do_new_create();
			}break;
			case OP_MAIL:{
				do_op_mail();
			}break;
			case OP_NAME:{
				do_op_name();
			}break;
			case OP_ARENA_MAIL:{
				do_op_arena_mail();
			}break;
			case SEARCH_USER:{
				do_search_user();
			}break;
			case OP_FRIEND_OFFLINE:{
				do_op_friend_offline();
			}break;
			case OP_FRIEND:{
				do_op_friend();
			}break;
			case OP_RANK:{
				do_op_rank();
			}break;
		}
	}

	//获取直接的sql
	virtual bool GetSql(std::string& strAppendHere);

	//不使用返回结果的操作 可以合并起来
	//合并之后 可以直接执行sql语句 而不调用onexceute
	virtual bool CanCombine(){
		//不使用返回结果的操作 可以合并起来
		switch (m_optype)
		{
		case OP_INFO:
		case New_Create:
		case OP_ARENA_MAIL:
		case OP_FRIEND:
			return true;
		}

		return false;
	}
	
	CUserCommand::OPTYPE Optype() const { return m_optype; }
	void Optype(CUserCommand::OPTYPE val) { m_optype = val; }
	Guid Gs_connect_id() const { return m_gs_connect_id; }
	void Gs_connect_id(Guid& val) { m_gs_connect_id = val; }
	Guid User_connect_id() const { return m_user_connect_id; }
	void User_connect_id(Guid& val) { m_user_connect_id = val; }
	UINT32 User_id() const { return m_user_id; }
	void User_id(UINT32 val) { m_user_id = val; }
	void* Data() const { return data; }
	void Data(void* val) { data = val; }
private:

	bool do_get_info();
	
	bool do_op_rank();

	bool do_op_info();
	bool get_sql_of_op_info(std::string& strAppendHere);

	bool do_new_create();
	bool get_sql_of_new_create(std::string& strAppendHere);

	bool do_op_mail();

	bool do_op_name();

	bool do_op_arena_mail();
	bool get_sql_of_op_arena_mail(std::string& strAppendHere);

	bool do_search_user();
	bool do_op_friend_offline();

	bool do_op_friend();
	bool get_sql_of_op_friend(std::string& strAppendHere);

	//玩家基本信息
	bool op_base_info();
	bool _get_sql_of_op_base_info(std::string& strAppendHere);


	//子任务信息
	bool op_sub_task();
	bool _get_sql_of_op_sub_task(std::string& strAppendHere);

	//邮件信息
	bool op_mail();
	bool _get_sql_of_op_mail(std::string& strAppendHere);

	//道具信息
	bool op_game_element();
	bool _get_sql_of_op_game_element(std::string& strAppendHere);

	//英雄信息操作
	bool op_hero();
	bool _get_sql_of_op_hero(std::string& strAppendHere);

	//商店相关操作
	bool op_shop();			//商店级操作 商店创建 或者 刷新
	bool _get_sql_of_op_shop(std::string& strAppendHere);

	bool op_shop_items();	//操作商店里的道具信息
	bool _get_sql_of_op_shop_items(std::string& strAppendHere);

	bool op_shop_buy();		//操作玩家购买记录
	bool _get_sql_of_op_shop_buy(std::string& strAppendHere);

	bool op_mission();
	bool _get_sql_of_op_mission(std::string& strAppendHere);
	OPTYPE m_optype;
	Guid m_gs_connect_id;   //dbs上 gs客户端链接id
	Guid m_user_connect_id;//gs上所属的玩家客户端连接id 在获取到用户之前 dbs需要为gs保存这个 才能告知gs发给哪个链接  获取之后 gs可以通过userid 找到链接 这里就不需要设置了
	UINT32 m_user_id;
	void* data;//额外数据 
};

#endif // db_async_command_h__
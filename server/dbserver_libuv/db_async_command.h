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
		Same_Name = 2,//ͬ��
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
		New_Create = 3,	//�½���һ�����Ϣ
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

	//��ȡֱ�ӵ�sql
	virtual bool GetSql(std::string& strAppendHere);

	//��ʹ�÷��ؽ���Ĳ��� ���Ժϲ�����
	//�ϲ�֮�� ����ֱ��ִ��sql��� ��������onexceute
	virtual bool CanCombine(){
		//��ʹ�÷��ؽ���Ĳ��� ���Ժϲ�����
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

	//��һ�����Ϣ
	bool op_base_info();
	bool _get_sql_of_op_base_info(std::string& strAppendHere);


	//��������Ϣ
	bool op_sub_task();
	bool _get_sql_of_op_sub_task(std::string& strAppendHere);

	//�ʼ���Ϣ
	bool op_mail();
	bool _get_sql_of_op_mail(std::string& strAppendHere);

	//������Ϣ
	bool op_game_element();
	bool _get_sql_of_op_game_element(std::string& strAppendHere);

	//Ӣ����Ϣ����
	bool op_hero();
	bool _get_sql_of_op_hero(std::string& strAppendHere);

	//�̵���ز���
	bool op_shop();			//�̵꼶���� �̵괴�� ���� ˢ��
	bool _get_sql_of_op_shop(std::string& strAppendHere);

	bool op_shop_items();	//�����̵���ĵ�����Ϣ
	bool _get_sql_of_op_shop_items(std::string& strAppendHere);

	bool op_shop_buy();		//������ҹ����¼
	bool _get_sql_of_op_shop_buy(std::string& strAppendHere);

	bool op_mission();
	bool _get_sql_of_op_mission(std::string& strAppendHere);
	OPTYPE m_optype;
	Guid m_gs_connect_id;   //dbs�� gs�ͻ�������id
	Guid m_user_connect_id;//gs����������ҿͻ�������id �ڻ�ȡ���û�֮ǰ dbs��ҪΪgs������� ���ܸ�֪gs�����ĸ�����  ��ȡ֮�� gs����ͨ��userid �ҵ����� ����Ͳ���Ҫ������
	UINT32 m_user_id;
	void* data;//�������� 
};

#endif // db_async_command_h__
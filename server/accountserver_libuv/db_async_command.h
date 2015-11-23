#ifndef db_async_command_h__
#define db_async_command_h__

#pragma once

#include "async_command_module.h"
#include "guid.h"

class CAccountCommand : public ICommand{
public:
	enum OPTYPE{
		NONE,
		REGISTER,
		LOGIN,
		LOGIN_IF_NO_REGISTER //登陆时，如果不存在则注册
	};

	virtual void OnExecute(){
		switch ((int)m_optype)
		{
			case REGISTER:{
				do_register();
			}break;
			case LOGIN:{
				do_login();
			}break;
			case LOGIN_IF_NO_REGISTER:{
				do_login_if_no_register();
			}
		}
	}

	virtual bool GetSql(std::string& strAppendHere){
		return false;
	}

	virtual bool CanCombine(){
		return false;
	}	

	CAccountCommand::OPTYPE Optype() const { return m_optype; }
	void Optype(CAccountCommand::OPTYPE val) { m_optype = val; }
	UINT32 Plantform_id() const { return m_plantform_id; }
	void Plantform_id(UINT32 val) { m_plantform_id = val; }
	UINT32 Plantform_sub_id() const { return m_plantform_sub_id; }
	void Plantform_sub_id(UINT32 val) { m_plantform_sub_id = val; }
	std::string Account() const { return m_account; }
	void Account(const std::string &val) { m_account = val; }
	std::string Password() const { return m_password; }
	void Password(const std::string &val) { m_password = val; }
	Guid& Client_id() { return m_client_id; }
	void Client_id(Guid& val) { m_client_id = val; }
private:

	bool do_register();

	bool do_login();

	bool do_login_if_no_register();

	OPTYPE m_optype;
	UINT32 m_plantform_id;
	UINT32 m_plantform_sub_id;
	std::string m_account;
	std::string m_password;

	Guid m_client_id;//发送这个命令 所属的客户端连接
};

#endif // db_async_command_h__
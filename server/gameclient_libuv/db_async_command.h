#ifndef db_async_command_h__
#define db_async_command_h__

#pragma once

#include "async_command_module.h"

//#include "WinBase.h"//为了获取毫秒级的时间！
class CLogCommond : public ICommand{
public:

	enum ESERVERTYPE{
		ESERVERTYPE_MIN = 0,
		ESERVERTYPE_AS = 1,
		ESERVERTYPE_GS = 2,
		ESERVERTYPE_MAX = 3
	};

	enum ELOGTYPE{
		ELOGTYPE_MIN = 0,
		ELOGTYPE_SEND = 1,
		ELOGTYPE_RECEIVE = 2,
		ELOGTYPE_MAX = 3
	};

	virtual void OnExecute();

	virtual bool GetSql(std::string& strAppendHere){
		return false;
	}

	virtual bool CanCombine(){
		return false;
	}	

	void Init(ESERVERTYPE server_type, ELOGTYPE log_type, UINT32 msg_id, UINT32 user_id, UINT32 time){
		m_server_type = server_type;
		m_log_type = log_type;
		m_msg_id = msg_id;
		m_user_id = user_id;
		m_time = time;
		//GetLocalTime(&m_time);
	}

private:
	ESERVERTYPE m_server_type;
	ELOGTYPE m_log_type;
	UINT32 m_msg_id;
	UINT32 m_user_id;
	UINT32 m_time;

	//SYSTEMTIME m_time;
};

#endif // db_async_command_h__
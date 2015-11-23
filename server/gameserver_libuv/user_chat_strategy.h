#pragma once
#ifndef user_chat_strategy_h__
#define user_chat_strategy_h__
#include "gs_c_proto.pb.h"
class CUser;
class IUserChatStrategy{
public:
	virtual void Chat(c_gs_chat_req& req, gs_c_chat_res& res) = 0;
	virtual bool  Init(CUser* pUser) = 0;
};

class CUserChatStrategy : public IUserChatStrategy
{
public:
	virtual void Chat(c_gs_chat_req& req, gs_c_chat_res& res);
	virtual bool Init(CUser* pUser);
	CUserChatStrategy(void){};
private:
	CUser* m_pUser;
};

#endif
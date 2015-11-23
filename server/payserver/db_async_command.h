#ifndef db_async_command_h__
#define db_async_command_h__

#pragma once

#include <string>
#include "async_command_module.h"
#include "guid.h"
#include "order.h"


//收到订单通知时的处理 新的 或者 失败的需要再处理的订单
class COrderNotifyCommand : public ICommand{
public:
	enum OPTYPE{
		NONE,
		ORDER_NOTIFY
	};

	COrderNotifyCommand(OPTYPE optype, COrder* pOrder);

	~COrderNotifyCommand();

	virtual bool GetSql(std::string& strAppendHere){return false;}				//获取要执行的sql 如果合并起来 则可以不执行OnExecute 由自己手动执行
	virtual bool CanCombine(){ return false; }

	virtual void OnExecute(){
		switch (m_optype)
		{
			case ORDER_NOTIFY:{
				order_notify();
			}break;

			default:
				ASSERT(false);
		}
	}

private:

	bool order_notify();

	OPTYPE m_optype;
	COrder* m_Order;
	//Socket* m_client_id;//发送这个命令 所属的客户端连接
	//这个地方不需要用到http客户端
};

class COrderProcessedCommand : public ICommand{
public:

	COrderProcessedCommand(const std::string &order_id, const COrder::ESTATUS eStatus){
		m_Order_id = order_id;
		m_eStatus = eStatus;
	}

	~COrderProcessedCommand(){}

	virtual bool GetSql(std::string& strAppendHere){return false;}				//获取要执行的sql 如果合并起来 则可以不执行OnExecute 由自己手动执行
	virtual bool CanCombine(){ return false; }

	virtual void OnExecute();

private:
	std::string m_Order_id;
	COrder::ESTATUS m_eStatus;
};
#endif // db_async_command_h__
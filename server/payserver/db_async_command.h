#ifndef db_async_command_h__
#define db_async_command_h__

#pragma once

#include <string>
#include "async_command_module.h"
#include "guid.h"
#include "order.h"


//�յ�����֪ͨʱ�Ĵ��� �µ� ���� ʧ�ܵ���Ҫ�ٴ���Ķ���
class COrderNotifyCommand : public ICommand{
public:
	enum OPTYPE{
		NONE,
		ORDER_NOTIFY
	};

	COrderNotifyCommand(OPTYPE optype, COrder* pOrder);

	~COrderNotifyCommand();

	virtual bool GetSql(std::string& strAppendHere){return false;}				//��ȡҪִ�е�sql ����ϲ����� ����Բ�ִ��OnExecute ���Լ��ֶ�ִ��
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
	//Socket* m_client_id;//����������� �����Ŀͻ�������
	//����ط�����Ҫ�õ�http�ͻ���
};

class COrderProcessedCommand : public ICommand{
public:

	COrderProcessedCommand(const std::string &order_id, const COrder::ESTATUS eStatus){
		m_Order_id = order_id;
		m_eStatus = eStatus;
	}

	~COrderProcessedCommand(){}

	virtual bool GetSql(std::string& strAppendHere){return false;}				//��ȡҪִ�е�sql ����ϲ����� ����Բ�ִ��OnExecute ���Լ��ֶ�ִ��
	virtual bool CanCombine(){ return false; }

	virtual void OnExecute();

private:
	std::string m_Order_id;
	COrder::ESTATUS m_eStatus;
};
#endif // db_async_command_h__
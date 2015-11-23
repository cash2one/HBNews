#include "db_async_command.h"
#include "db_sync_operator.h"

#include "ps_gs_proto.pb.h"
#include "order.h"
#include "commodity_mgr.h"
#include "server_for_gs.h"

COrderNotifyCommand::COrderNotifyCommand(OPTYPE optype, COrder* pOrder ) :m_optype(optype),m_Order(pOrder)
{

}

COrderNotifyCommand::~COrderNotifyCommand()
{
	delete m_Order;
}

bool COrderNotifyCommand::order_notify()
{
	//������
	UINT32 user_id = NULL;
	int r = db_sync_operator::order_notify(m_Order);

	if (r == 0){
		//����û�д˼�¼ ��洢�˼�¼ ���Ҹ�֪gs

		ps_gs_paied_req* req = new ps_gs_paied_req;
		req->set_order_id(m_Order->m_order_id);

		//��ȡ�����Ϣ
		req->set_user_id(m_Order->m_user_id);

		//��ȡ��Ʒ��Ӧ�Ķ���
		CCommodity* p = CCommodityMgr::GetInstance()->GetCommodityByID(m_Order->m_commodity_id);
		ASSERT(p);
		CHECK_NULL_RETURN_FALSE(p);

		for (std::vector<stGameElement>::iterator it = p->eles.begin(); it != p->eles.end(); ++it){
			ps_gs_paied_req::game_element* ele = req->add_eles();
			ele->set_type(it->type);
			ele->set_id(it->id);
			ele->set_num(it->num);
		}

		bool ret = CServerForGS::SendAsync(m_Order->m_area_id, e_msg_ps_gs_paied_req, req);
		ASSERT(ret);
		CHECK_FALSE_RETURN_FALSE(ret);
	}
	else{//���ݿ����Ѿ����˴˶���
		//��������
	}

	return true;
}


void COrderProcessedCommand::OnExecute()
{
	//������
	UINT32 user_id = NULL;
	int r = db_sync_operator::order_process(m_Order_id, m_eStatus);
	ASSERT(r == 0);
}
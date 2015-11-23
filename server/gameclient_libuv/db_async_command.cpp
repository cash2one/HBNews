#include "db_async_command.h"
#include "db_sync_operator.h"


void CLogCommond::OnExecute()
{
	bool r = db_sync_operator::msg_log(m_server_type, m_log_type, m_user_id, m_msg_id, m_time);
	ASSERT(r);
}
//#include <string>
#include "db_sync_operator.h"
#include "db_mgr.h"
//#include "str.h"
#include "db_async_command.h"

//static char g_sql[1024*100] = {0};
//int g_sql_pos = 0;

bool db_sync_operator::msg_log(UINT32 iServerType, UINT32 iLogType, UINT32 iUserID, UINT32 iMsgID, UINT32 time )
{
	MYSQL* con = CDBMgr::GetInstance()->GetConnection();
	char sql[1024] = {0};

	//if (iLogType == CLogCommond::ELOGTYPE_SEND){
	//	sprintf_s(sql, sizeof(sql), "INSERT INTO `log_msg_user_send` SET `server_type` = %u, `user_id` = %u, `msg_id` = %u, `time` = FROM_UNIXTIME(%u)", iServerType, iUserID, iMsgID, time);
	//}
	//else if(iLogType == CLogCommond::ELOGTYPE_RECEIVE){
	//	sprintf_s(sql, sizeof(sql), "INSERT INTO `log_msg_user_receive` SET `server_type` = %u, `user_id` = %u, `msg_id` = %u, `time` = FROM_UNIXTIME(%u)", iServerType, iUserID, iMsgID, time);
	//}
	if(iLogType == CLogCommond::ELOGTYPE_SEND || iLogType == CLogCommond::ELOGTYPE_RECEIVE){
		sprintf_s(sql, sizeof(sql), "CALL log_msg(%u, %u, %u, %u, %u);", iLogType, iServerType, iUserID, iMsgID, time);
	}
	else{
		ASSERT(false);
		return true;
	}

	//int code = mysql_real_query(con, sql, (unsigned int)strlen(sql));
	int code = CDBMgr::Query(con, sql, (unsigned int)strlen(sql));
	if (0 != code)   //∑«0≤È—Ø ß∞‹  
	{
		printf("query failed! [%s] [%d] [%s] \n", sql, mysql_errno(con), mysql_error(con));
		return false;  
	}

	return true;
}

#include "db_sync_operator.h"
#include "db_mgr.h"

#include "server_for_gs.h"
#include "order.h"
int db_sync_operator::order_notify(const COrder* pOrder){

	ASSERT(pOrder);
	MYSQL* con = CDBMgr::GetInstance()->GetConnection();

	char sql[1024] = {0};
	//IN var_plantform_id int, IN var_order_id varchar(127), IN var_create_time int, IN var_status int, IN var_commodity_id int, IN var_commodity_num int, IN var_present_price float, IN var_original_price float
	sprintf_s(sql, sizeof(sql), "CALL public_order_notify(%u, '%s', %u, %u, %u, %u, %f, %f);", 
		pOrder->m_plantform_id, 
		pOrder->m_order_id.c_str(),
		pOrder->m_create_time, 
		pOrder->m_status,
		pOrder->m_commodity_id, 
		pOrder->m_commodity_num, 
		pOrder->m_present_price,
		pOrder->m_original_price
		);

	//int code = mysql_real_query(con, sql, (unsigned int)strlen(sql));
	int code = CDBMgr::Query(con, sql, (unsigned int)strlen(sql));
	if (0 != code){   //非0查询失败  
		printf("query failed! [%s] [%d] [%s] \n", sql, mysql_errno(con), mysql_error(con));
		return e_psgs_errorcode_unkown_error;
	}

	/* Process all results */
	printf("\n total affected rows: %lld \n", mysql_affected_rows(con));

	int r = e_psgs_errorcode_success;

	int i = 0;
	do{
		MYSQL_RES* res = mysql_store_result( con );
		if(i++ == 0){//只处理第一个！
			MYSQL_ROW row;
			if (row = mysql_fetch_row(res)){
				r = atoi(row[0]);
			}
		}
		mysql_free_result(res);
	}while( !mysql_next_result( con ) );

	return r;
}

int db_sync_operator::order_process(const std::string &order_id, const COrder::ESTATUS eStatus){
	MYSQL* con = CDBMgr::GetInstance()->GetConnection();

	char sql[1024] = {0};
	//IN var_plantform_id int, IN var_order_id varchar(127), IN var_create_time int, IN var_status int, IN var_commodity_id int, IN var_commodity_num int, IN var_present_price float, IN var_original_price float
	sprintf_s(sql, sizeof(sql), "CALL public_order_process('%s', %u);", order_id.c_str(),eStatus);

	//int code = mysql_real_query(con, sql, (unsigned int)strlen(sql));
	int code = CDBMgr::Query(con, sql, (unsigned int)strlen(sql));
	if (0 != code){   //非0查询失败  
		printf("query failed! [%s] [%d] [%s] \n", sql, mysql_errno(con), mysql_error(con));
		return e_psgs_errorcode_unkown_error;
	}

	/* Process all results */
	printf("\n total affected rows: %lld \n", mysql_affected_rows(con));

	int r = e_psgs_errorcode_success;

	int i = 0;
	do{
		MYSQL_RES* res = mysql_store_result( con );
		if(i++ == 0){//只处理第一个！
			MYSQL_ROW row;
			if (row = mysql_fetch_row(res)){
				r = atoi(row[0]);
			}
		}
		mysql_free_result(res);
	}while( !mysql_next_result( con ) );

	return r;
}
#include <string>
#include "db_sync_operator.h"
#include "db_mgr.h"
#include "as_c_proto.pb.h"

#include "str.h"
#include "area.h"

int db_sync_operator::register_account(const UINT32 plantform_id, const UINT32 plantform_sub_id, const std::string& account, const std::string& password, UINT32& ret_user_id){

	MYSQL* con = CDBMgr::GetInstance()->GetConnection();

	char sql[1024] = {0};
	//IN `in_plantform_id` int, IN `in_plantform_sub_id` int, IN `in_plantform_account` varchar(63), IN `in_plantform_password` varchar(63), IN `in_token` varchar(63), IN `in_name` varchar(63)
	sprintf_s(sql, sizeof(sql), "CALL public_register(%d, %d, '%s', '%s')", plantform_id, plantform_sub_id, account.c_str(), password.c_str());

	//int code = mysql_real_query(con, sql, (unsigned int)strlen(sql));
	int code = CDBMgr::Query(con, sql, (unsigned int)strlen(sql));
	if (0 != code)   //非0查询失败  
	{
		printf("query failed! [%s] [%d] [%s] \n", sql, mysql_errno(con), mysql_error(con));
		return e_asc_errorcode_unkown_error;
	}

	/* Process all results */
	printf("\n total affected rows: %lld \n", mysql_affected_rows(con));

	int r = e_asc_errorcode_success;

	//这种会出 error !2014 Commands out of sync; you can't run this command now

	//MYSQL_RES* res = mysql_store_result(con);   //保存查询结果  
	//MYSQL_ROW row;
	//if (row = mysql_fetch_row(res))
	//{
	//	int ret = atoi(row[0]);
	//	if (ret != 0)
	//	{
	//		r = e_asc_errorcode::e_asc_errorcode_registe_already;//todo 具体的错误码！
	//	}
	//	else{
	//		ret_user_id = atoi(row[1]);
	//	}
	//}
	//else{

	//	r = e_asc_errorcode_unkown_error;
	//}
	//mysql_free_result(res);


	int i = 0;
	do
	{
		MYSQL_RES* res = mysql_store_result( con );
		if(i++ == 0){//只处理第一个！
			MYSQL_ROW row;
			if (row = mysql_fetch_row(res))
			{
				int ret = atoi(row[0]);
				if (ret != 0)
				{
					r = e_asc_errorcode::e_asc_errorcode_registe_already;//todo 具体的错误码！
				}
				else{
					ret_user_id = atoi(row[1]);
				}
			}
			else{

				r = e_asc_errorcode_unkown_error;
			}
		}
		mysql_free_result(res);
	}while( !mysql_next_result( con ) );

	return r;
}


int db_sync_operator::login(const UINT32 plantform_id, const UINT32 plantform_sub_id, const std::string& account, const std::string& password, UINT32& ret_user_id)
{
	MYSQL* con = CDBMgr::GetInstance()->GetConnection();
	char sql[1024] = {0};

	sprintf_s(sql, sizeof(sql), "CALL public_login(%d, %d, '%s', '%s')", plantform_id, plantform_sub_id, account.c_str(), password.c_str());

	//int code = mysql_real_query(con, sql, (unsigned int)strlen(sql));
	int code = CDBMgr::Query(con, sql, (unsigned int)strlen(sql));
	if (0 != code)   //非0查询失败  
	{
		printf("query failed! [%s] [%d] [%s] \n", sql, mysql_errno(con), mysql_error(con));
		return e_asc_errorcode_unkown_error;  
	}


	int r = e_asc_errorcode_success;

	//MYSQL_RES* res = mysql_store_result(con);   //保存查询结果  
	//MYSQL_ROW row;
	//if (row = mysql_fetch_row(res))
	//{
	//	int ret = atoi(row[0]);
	//	if (ret != 0)
	//	{
	//		r = e_asc_errorcode::e_asc_errorcode_account_not_exist_or_wrong_password;//todo 具体的错误码！
	//	}
	//	else{
	//		ret_user_id = atoi(row[1]);
	//	}
	//}
	//else{
	//	r = e_asc_errorcode_unkown_error;
	//}

	//mysql_free_result(res);

	int i = 0;
	do
	{
		MYSQL_RES* res = mysql_store_result( con );
		if(i++ == 0){//只处理第一个！
			MYSQL_ROW row;
			if (row = mysql_fetch_row(res))
			{
				int ret = atoi(row[0]);
				if (ret != 0)
				{
					r = e_asc_errorcode::e_asc_errorcode_account_not_exist_or_wrong_password;//todo 具体的错误码！
				}
				else{
					ret_user_id = atoi(row[1]);
				}
			}
			else{
				r = e_asc_errorcode_unkown_error;
			}
		}
		mysql_free_result(res);
	}while( !mysql_next_result( con ) );

	return r;
}

int db_sync_operator::login_if_no_register(const UINT32 plantform_id, const UINT32 plantform_sub_id, const std::string& account, const std::string& password, UINT32& ret_user_id)
{
	MYSQL* con = CDBMgr::GetInstance()->GetConnection();
	char sql[1024] = {0};

	sprintf_s(sql, sizeof(sql), "CALL public_login_if_no_register(%d, %d, '%s', '%s')", plantform_id, plantform_sub_id, account.c_str(), password.c_str());

	//int code = mysql_real_query(con, sql, (unsigned int)strlen(sql));
	int code = CDBMgr::Query(con, sql, (unsigned int)strlen(sql));
	if (0 != code)   //非0查询失败  
	{
		printf("query failed! [%s] [%d] [%s] \n", sql, mysql_errno(con), mysql_error(con));
		return e_asc_errorcode_unkown_error;  
	}


	int r = e_asc_errorcode_success;

	//MYSQL_RES* res = mysql_store_result(con);   //保存查询结果  
	//MYSQL_ROW row;
	//if (row = mysql_fetch_row(res))
	//{
	//	int ret = atoi(row[0]);
	//	if (ret != 0)
	//	{
	//		r = e_asc_errorcode::e_asc_errorcode_account_not_exist_or_wrong_password;//todo 具体的错误码！
	//	}
	//	else{
	//		ret_user_id = atoi(row[1]);
	//	}
	//}
	//else{
	//	r = e_asc_errorcode_unkown_error;
	//}

	//mysql_free_result(res);

	int i = 0;
	do
	{
		MYSQL_RES* res = mysql_store_result( con );
		if(i++ == 0){//只处理第一个！
			MYSQL_ROW row;
			if (row = mysql_fetch_row(res))
			{
				int ret = atoi(row[0]);
				if (ret != 0)
				{
					r = e_asc_errorcode::e_asc_errorcode_wrong_password;//todo 具体的错误码！
				}
				else{
					ret_user_id = atoi(row[1]);
				}
			}
			else{
				r = e_asc_errorcode_unkown_error;
			}
		}
		mysql_free_result(res);
	}while( !mysql_next_result( con ) );

	return r;
}


//int get_area_list(std::map<UINT32, CArea*> &areas){
//	MYSQL* con = CDBMgr::GetInstance()->GetConnection();
//	char sql[1024] = {0};
//
//	sprintf_s(sql, sizeof(sql), "SELECT * FROM `area`");
//
//	int code = mysql_real_query(con, sql, (unsigned int)strlen(sql));
//	if (0 != code)   //非0查询失败  
//	{
//		printf("query failed! [%s] [%d] [%s] \n", sql, mysql_errno(con), mysql_error(con));
//		return e_asc_errorcode_unkown_error;  
//	}
//
//	int r = 0;
//	MYSQL_RES* res = mysql_store_result(con);   //保存查询结果  
//
//	std::string need_fields[] = {"area_id", "name", "ip", "port", "hidden"};
//	std::vector<std::string> v_need_field(need_fields, need_fields + sizeof(need_fields)/sizeof(need_fields[0]));
//	ASSERT(CDBMgr::check_fields(res, v_need_field));
//
//	MYSQL_ROW row;
//	while (row = mysql_fetch_row(res))
//	{
//		int col = 0;
//		CArea* pArea = new CArea;
//
//		pArea->area_id = atoi(row[col++]);
//
//		ASSERT(areas.find(pArea->area_id) == areas.end());
//
//		char* pBuf = UTF8ToANSI(row[col++]);
//		pArea->name = pBuf;
//		free(pBuf);
//
//		pBuf = UTF8ToANSI(row[col++]);
//		pArea->ip = pBuf;
//		free(pBuf);
//
//		pArea->port = atoi(row[col++]);
//
//		pArea->isHidden = (bool)atoi(row[col++]);
//
//		areas[pArea->area_id] = pArea;
//
//	}
//	//else{
//	//	r = e_asc_errorcode_unkown_error;
//	//}
//
//	if (areas.size() == 0)
//	{
//		printf("\n 没有区服信息!!!");
//	}
//
//	mysql_free_result(res);
//	return r;
//}
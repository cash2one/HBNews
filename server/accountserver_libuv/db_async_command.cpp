#include "db_async_command.h"
#include "db_sync_operator.h"
#include "as_c_proto.pb.h"
#include "server_for_user.h"
#include "libuv_helper.h"

bool CAccountCommand::do_register()
{
	//阻塞！
	UINT32 user_id = NULL;
	int r = db_sync_operator::register_account(m_plantform_id, m_plantform_sub_id, m_account, m_password, user_id);

	as_c_register_res* ret = new as_c_register_res;
	ret->set_error_code(r);
	//ret->set_user_id(user_id);
	//ret.set_token("hello world!");

	stUserAsyncData* asyncData = new stUserAsyncData();
	asyncData->op = EUserAsyncOP_Register;
	asyncData->client_id = m_client_id;
	asyncData->data = ret;

	uv_async_t* async = CServerForUser::GetInstance()->CreateAsync();
	ASSERT(async);

	async->data = asyncData; 
	r = uv_async_send(async);
	ASSERT(r == 0);
	if (r != 0)
	{
		PRINTF_UV_ERROR(r);
		delete ret;
		delete asyncData;
		async->data = NULL;
		CServerForUser::GetInstance()->DestroyAsync(async);
	}

	//不直接在这里做 而是利用async回复主线程！
	/*
	int ret_len = ret.ByteSize();
	uv_buf_t ret_buf;
	buf_alloc((uv_handle_t*)stream, ret_len + sizeof(UINT32), &ret_buf);
	*(UINT32*)(ret_buf.base) = e_msg_gs_c::e_msg_gs_c_register_res;
	ret.SerializeToArray(ret_buf.base + sizeof(UINT32), ret_len);

	uv_write_t* write_req = (uv_write_t*)req_alloc();
	int r = uv_write(write_req, stream, &ret_buf, 1, write_cb);
	buf_free(&ret_buf);
	ASSERT(r == 0);
	*/

	return true;
}

bool CAccountCommand::do_login()
{
	//阻塞！
	UINT32 user_id = NULL;
	int r = db_sync_operator::login(m_plantform_id, m_plantform_sub_id, m_account, m_password, user_id);

	as_c_login_res* ret = new as_c_login_res;
	ret->set_error_code(r);

	stUserAsyncData* asyncData = new stUserAsyncData();
	asyncData->op = EUserAsyncOP_Login;
	asyncData->client_id = m_client_id;
	asyncData->user_id = user_id;
	asyncData->data = ret;

	uv_async_t* async = CServerForUser::GetInstance()->CreateAsync();
	ASSERT(async);
	async->data = asyncData; 
	r = uv_async_send(async);
	ASSERT(r == 0);
	if (r != 0)
	{
		PRINTF_UV_ERROR(r);
		delete ret;
		delete asyncData;
		async->data = NULL;
		CServerForUser::GetInstance()->DestroyAsync(async);
	}


	//不直接在这里做 而是利用async回复主线程！
	/*
	int ret_len = ret.ByteSize();
	uv_buf_t ret_buf;
	buf_alloc((uv_handle_t*)stream, ret_len + sizeof(UINT32), &ret_buf);
	*(UINT32*)(ret_buf.base) = e_msg_gs_c::e_msg_gs_c_register_res;
	ret.SerializeToArray(ret_buf.base + sizeof(UINT32), ret_len);

	uv_write_t* write_req = (uv_write_t*)req_alloc();
	int r = uv_write(write_req, stream, &ret_buf, 1, write_cb);
	buf_free(&ret_buf);
	ASSERT(r == 0);
	*/

	return true;
}

bool CAccountCommand::do_login_if_no_register()
{
	//阻塞！
	UINT32 user_id = NULL;
	int r = db_sync_operator::login_if_no_register(m_plantform_id, m_plantform_sub_id, m_account, m_password, user_id);

	as_c_login_res* ret = new as_c_login_res;
	ret->set_error_code(r);

	stUserAsyncData* asyncData = new stUserAsyncData();
	asyncData->op = EUserAsyncOP_Login;
	asyncData->client_id = m_client_id;
	asyncData->user_id = user_id;
	asyncData->data = ret;

	uv_async_t* async = CServerForUser::GetInstance()->CreateAsync();
	ASSERT(async);
	async->data = asyncData; 
	r = uv_async_send(async);
	ASSERT(r == 0);
	if (r != 0)
	{
		PRINTF_UV_ERROR(r);
		delete ret;
		delete asyncData;
		async->data = NULL;
		CServerForUser::GetInstance()->DestroyAsync(async);
	}


	//不直接在这里做 而是利用async回复主线程！
	/*
	int ret_len = ret.ByteSize();
	uv_buf_t ret_buf;
	buf_alloc((uv_handle_t*)stream, ret_len + sizeof(UINT32), &ret_buf);
	*(UINT32*)(ret_buf.base) = e_msg_gs_c::e_msg_gs_c_register_res;
	ret.SerializeToArray(ret_buf.base + sizeof(UINT32), ret_len);

	uv_write_t* write_req = (uv_write_t*)req_alloc();
	int r = uv_write(write_req, stream, &ret_buf, 1, write_cb);
	buf_free(&ret_buf);
	ASSERT(r == 0);
	*/

	return true;
}
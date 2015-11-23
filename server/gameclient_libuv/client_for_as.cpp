#include "client_for_as.h"
#include "def.h"
#include "net_config.h"
#include "libuv_helper.h"

#include "client_for_gs.h"
#include "user_mgr.h"
#include "db_async_command.h"
#include "time_helper.h"
//
//static std::string user_name = "moto4444";
//static std::string user_name = "hello118";
//static std::string password = "111111";

static std::string user_name = "uiuiaaa";
static bool g_log = false;
UINT32 CClientForAS::m_connect_client_num = 0;

bool CClientForAS::Init()
{
	ASSERT(add_message_handle(e_msg_as_c::e_msg_as_c_register_res, handle_msg_register_res));
	ASSERT(add_message_handle(e_msg_as_c::e_msg_as_c_login_res, handle_msg_login_res));
	ASSERT(add_message_handle(e_msg_as_c::e_msg_as_c_enter_area_res, handle_msg_enter_area_res));
	ASSERT(add_message_handle(e_msg_as_c::e_msg_as_c_login_if_no_register_res, handle_msg_login_if_no_register_res));

	return true;
}

bool CClientForAS::Start()
{
	int r = uv_thread_create(&m_thread, run, this);
	return r == 0;
}

bool CClientForAS::Join()
{
	int r = uv_thread_join(&m_thread);
	ASSERT( r == 0) ;
	return true;
}

uv_async_t* CClientForAS::CreateAsync()
{
	ASSERT(m_loop);
	uv_async_t* async = (uv_async_t*)malloc(sizeof uv_async_t);
	ASSERT(async);
	int r = uv_async_init(m_loop, async, async_cb);
	ASSERT(r == 0);
	return async;
}

void CClientForAS::DestroyAsync(uv_async_t* handle)
{
	ASSERT(handle);
	free(handle);
}

void CClientForAS::run( void* arg )
{
	struct sockaddr_in addr;
	int r;

	CClientForAS* client_for_as = (CClientForAS*)arg;
	ASSERT(client_for_as);

	client_for_as->m_loop = (uv_loop_t*)malloc(sizeof uv_loop_t);
	uv_loop_init(client_for_as->m_loop);
	client_for_as->m_loop->data = client_for_as;//

	//ASSERT(0 == uv_ip4_addr("127.0.0.1", 1234, &addr));
	//ASSERT(0 == uv_ip4_addr("211.157.191.149", 1234, &addr));
	//struct sockaddr_in dest = uv_ip4_addr("127.0.0.1", 80);
	//ASSERT(0 == uv_ip4_addr("127.0.0.1", 54321, &addr));
	//ASSERT(0 == uv_ip4_addr("127.0.0.1", 54320, &addr));

	char * ip = "127.0.0.1";
	//char * ip = "211.157.191.149";
	//char * ip = "211.157.191.157";
	//char * ip = "192.168.199.202";

	int port = 54320;
	ASSERT(0 == uv_ip4_addr(ip, port, &addr));

	r = uv_tcp_init(client_for_as->m_loop, &client_for_as->m_client);
	ASSERT(r == 0);

	uv_connect_t* connect = (uv_connect_t*)req_alloc();
	r = uv_tcp_connect(connect, &client_for_as->m_client, (const struct sockaddr*)(&addr), on_server_connection);
	ASSERT(r == 0);

	//uv_idle_init(client_for_as->m_loop, &client_for_as->m_idler);
	//uv_idle_start(&client_for_as->m_idler, on_idle);

	r = uv_run(client_for_as->m_loop, UV_RUN_DEFAULT);
	//ASSERT(0 && "Blackhole server dropped out of event loop.");

	ASSERT(r == 0);
}


void CClientForAS::on_server_connection( uv_connect_t* req, int status )
{
	ASSERT(req);
	uv_stream_t* con = req->handle;
	DWORD threadid =  GetCurrentThreadId();
	req_free((uv_req_t*)req);//后面不要用req了 否则就是野指针 同时别的线程会修改它的值！
	if (status != 0){
		//printf("%s:%s", uv_err_name(status), uv_strerror(status));
		PRINTF_UV_ERROR(status);
		printf("[%u][%p]AS服务器连接失败...\n", threadid, con);
		return ;
	}
	ASSERT(status == 0);

	printf("[%u][%p]AS服务器连接成功...\n", threadid, con);
	int r = uv_read_start(con, alloc_cb, on_receive);
	if (r) {
		FATAL("uv_read_start failed");
	}

	printf("[%u][%p]AS服务器连接读启动成功...\n", threadid, con);

	printf("as客户端链接数为:[%u]\n", ++m_connect_client_num);

	c_as_register_req reg_req;
	reg_req.set_plantform_id(1);
	reg_req.set_plantform_sub_id(1);
	//reg_req.set_account(m_user_name);
	//reg_req.set_password(m_password);

	CClientForAS* client_as = get_client((uv_handle_t*)con);
	ASSERT(client_as);
	reg_req.set_account(client_as->m_user_name);
	reg_req.set_password(client_as->m_password);

	//这是错误的用法 req被释放掉了然后被其他的线程用来填充了别的链接！
	//send(req->handle, e_msg_c_as_register_req, &reg_req);
	send(con, e_msg_c_as_register_req, &reg_req);

	ASSERT(r == 0);
}

void CClientForAS::on_receive( uv_stream_t* stream, ssize_t nread, const uv_buf_t* buf )
{
	int r;
	//todo net_protect作一些限制处理
	if (nread > 0){

		//CClientForAS::GetInstance()->process_msg(stream, buf->base, nread);

		CClientForAS* client_as = get_client((uv_handle_t*)stream);
		ASSERT(client_as);
		client_as->process_msg(stream, buf->base, nread);

		free(buf->base);//释放内存
	}
	else if (nread == 0)//OK, but nothing read
	{

	}
	else{

		//ASSERT(uv_last_error(stream->loop).code == UV_EOF);

		if (nread == UV_EOF) {
			printf("[%p]AS主动断开\n", stream);

		} else if (nread == UV_ECONNRESET) {
			printf("[%p]AS异常断开\n", stream);

		} else {
			printf("[%p] %s : %s \n", stream, uv_err_name(nread), uv_strerror(nread));
		}
		//uv_shutdown_t* req = (uv_shutdown_t*)req_alloc();
		//r = uv_shutdown(req, stream, after_shutdown);
		//ASSERT(r == 0);

		close_client(stream);
	}
}

void CClientForAS::after_shutdown( uv_shutdown_t* req, int status )
{
	ASSERT(req);
	ASSERT(req->handle);

	//uv_close((uv_handle_t*)req->handle, after_close);
	if(uv_is_active((uv_handle_t*)req->handle)){
		printf("[%p]after_shutdown and uv_is_active \n", req->handle);
		uv_close((uv_handle_t*)req->handle, after_close);
	}
	else{
		printf("[%p]after_shutdown but not uv_is_active! \n", req->handle);
		after_close((uv_handle_t*)req->handle);
	}
	req_free((uv_req_t*)req);
}

void CClientForAS::after_close( uv_handle_t* handle )
{
	uv_tcp_t* client = (uv_tcp_t*)handle;

	printf("[%p] CClientForAS::after_close", handle);
	//ASSERT(client == &(CClientForAS::GetInstance()->m_client));
	
	CClientForAS* client_as = get_client((uv_handle_t*)client);
	ASSERT(client_as);
	ASSERT( client == &(client_as->m_client));
}

void CClientForAS::close_client( uv_stream_t* client )
{
	uv_shutdown_t* req = (uv_shutdown_t*)req_alloc();
	int r = uv_shutdown(req, client, after_shutdown);
	//ASSERT(r == 0);//可能会出现这种情况
	if ( r != 0){
		printf("[%p]c关闭as链接！uv_shutdown失败！准备直接close！！\n", client);
		if(uv_is_active((uv_handle_t*)client)){
			uv_close((uv_handle_t*)client, after_close);
		}
		else{
			after_close((uv_handle_t*)client);
		}
	}
}

bool CClientForAS::send( uv_stream_t* stream, UINT32 msg_id, google::protobuf::Message* pMessage/*, uv_write_cb write_cb*/ )
{
	//ASSERT(uv_is_writable(stream));//可能会出现的正常情况！
	if (!uv_is_writable(stream)){
		printf("\n\n\n[%p]CClientForAS::send  uv_is_writable false!!! \n\n\n", stream);
		return false;
	}

	uv_buf_t ret_buf;
	UINT32 body_len = pMessage->ByteSize();
	//UINT32 head_len = sizeof(UINT32) + sizeof(UINT32);//4直接的包长度 + 4字节的消息id
	buf_alloc((uv_handle_t*)stream, PACKET_HEAD_LEN + body_len, &ret_buf);

	ASSERT(ret_buf.base && ret_buf.len);
	*((UINT32*)(ret_buf.base)) = PACKET_HEAD_LEN + body_len;
	*((UINT32*)(ret_buf.base)+1) = msg_id;
	INT32 r = pMessage->SerializeToArray(ret_buf.base + PACKET_HEAD_LEN, body_len);
	ASSERT(r);

	uv_write_t* write_req = (uv_write_t*)req_alloc();
	write_req->data = (void*)msg_id;//
	r = uv_write(write_req, stream, &ret_buf, 1, after_send);
	buf_free(&ret_buf);

	if (r != 0){
		PRINTF_UV_ERROR(r);
		//ASSERT(r == 0);//可能会出现的

		close_client(stream);

		return false;
	}
	else{
		printf("[%u][%p]CClientForAS::send msgid[%d]  bytes len[%d] \n", GetCurrentThreadId(), stream, msg_id, PACKET_HEAD_LEN + body_len);
		return true;
	}
}

void CClientForAS::after_send( uv_write_t* req, int status )
{
	ASSERT(req != NULL);
	if (status == 0)
	{
		//write_cb_called++;
		printf("[%u][%p]CClientForAS::after_send 给AS发送信息成功\n", GetCurrentThreadId(), req->handle);

		if (g_log){
			CClientForAS* client_as = get_client((uv_handle_t*)req->handle);
			ASSERT(client_as);
			CLogCommond* commond = new CLogCommond();
			commond->Init(CLogCommond::ESERVERTYPE_AS, CLogCommond::ELOGTYPE_SEND, UINT32(req->data), client_as->m_user_id, Now());
			CAsyncCommandModule::GetInstance()->AddCommand(commond);
		}
	}
	else{
		if (status == UV_EOF) {
			printf("[p]写时, AS主动断开\n", req->handle);

		} else if (status == UV_ECONNRESET) {
			printf("[p]写时, AS异常断开\n", req->handle);

		} else {
			printf("[p] %s : %s \n", req->handle, uv_err_name(status), uv_strerror(status));
		}

		//uv_shutdown_t* shut_r = (uv_shutdown_t*)req_alloc();
		//int r = uv_shutdown(shut_r, req->handle, after_shutdown);
		//ASSERT(r == 0);

		close_client(req->handle);
	}

	req_free((uv_req_t*)req);
}

void CClientForAS::after_async_close( uv_handle_t* handle )
{
	ASSERT(handle != NULL);

	//CClientForAS::GetInstance()->DestroyAsync((uv_async_t*)handle);

	CClientForAS* client_as = get_client(handle);
	ASSERT(client_as);
	client_as->DestroyAsync((uv_async_t*)handle);
}

bool CClientForAS::add_message_handle( e_msg_as_c msgID, func_message_handle handle )
{
	if (m_mapHandle[msgID])
	{
		printf("already exsit handle for %u", msgID);
		return false;
	}

	m_mapHandle[msgID] = handle;
	return true;
}

bool CClientForAS::process_msg( uv_stream_t* stream, const char * buf, int len )
{
	ASSERT(buf);
	ASSERT(len >= PACKET_HEAD_LEN);//
	int pos = 0;
	int bytes_left = len;

	char* p = const_cast<char*>(buf);
	while (bytes_left != 0)
	{
		UINT32 msg_len = *(UINT32 *)(p);
		p += sizeof msg_len;

		ASSERT(bytes_left >= msg_len);
		bytes_left -= msg_len;

		UINT32 msg_id = *(UINT32 *)(p);
		ASSERT(msg_id < e_msg_as_c::e_msg_as_c_max);
		p += sizeof msg_id;

		func_message_handle pMsgHandle = m_mapHandle[msg_id];
		if (pMsgHandle){
			bool r = pMsgHandle(stream, p, msg_len - sizeof(msg_len) - sizeof(msg_id));
			if (g_log){
				CClientForAS* client_as = get_client((uv_handle_t*)stream);
				ASSERT(client_as);
				UINT32 user_id = client_as->m_user_id;
				CLogCommond* commond = new CLogCommond();
				commond->Init(CLogCommond::ESERVERTYPE_AS, CLogCommond::ELOGTYPE_RECEIVE, msg_id, user_id, Now());
				CAsyncCommandModule::GetInstance()->AddCommand(commond);
			}
		}
		else{
			printf("CClientForAS::process_msg没有找到消息msgid[%d][%s]的处理函数\n", msg_id, e_msg_as_c_Name((e_msg_as_c)msg_id).c_str());
			//return false;
		}

		p += msg_len - sizeof(msg_len) - sizeof(msg_id);
	}

	return true;
}

void CClientForAS::async_cb( uv_async_t* handle )
{
	ASSERT(handle);
	CClientForASASync* async = (CClientForASASync*)handle->data;
	ASSERT(async->op);
	
	delete async;
	uv_close((uv_handle_t*)handle, after_async_close);
}

//bool CClientForAS::handle_msg_register_res( uv_stream_t* stream, const char * buf, int len )
//{
//	as_c_register_res res;
//	bool r = res.ParseFromArray(buf, len);
//	if (r)
//	{
//		printf("注册结果[%d][%s] \n", res.error_code(), e_asc_errorcode_Name((e_asc_errorcode)res.error_code()).c_str());
//
//		c_as_login_req login_req;
//		login_req.set_plantform_id(1);
//		login_req.set_plantform_sub_id(1);
//		//login_req.set_account(m_user_name);
//		//login_req.set_password(m_password);
//
//		CClientForAS* client_as = get_client((uv_handle_t*)stream);
//		ASSERT(client_as);
//		login_req.set_account(client_as->m_user_name);
//		login_req.set_password(client_as->m_password);
//
//		send(stream, e_msg_c_as_login_req, &login_req);
//
//		if ( res.error_code() == e_asc_errorcode_success){
//			return true;
//		}
//		else{
//			return false;
//		}
//	}
//	else{
//		PRINTF_PBPARSE_ERROR(e_msg_as_c_register_res);
//		return false;
//	}
//}

bool CClientForAS::handle_msg_register_res( uv_stream_t* stream, const char * buf, int len )
{
	as_c_register_res res;
	bool r = res.ParseFromArray(buf, len);
	if (r)
	{
		printf("注册结果[%d][%s] \n", res.error_code(), e_asc_errorcode_Name((e_asc_errorcode)res.error_code()).c_str());

		c_as_login_if_no_register_req login_req;
		login_req.set_plantform_id(1);
		login_req.set_plantform_sub_id(1);
		//login_req.set_account(m_user_name);
		//login_req.set_password(m_password);

		CClientForAS* client_as = get_client((uv_handle_t*)stream);
		ASSERT(client_as);
		login_req.set_account(client_as->m_user_name);
		login_req.set_password(client_as->m_password);

		//login_req.set_account("unityTestAccount");
		//login_req.set_password("Adwo");

		send(stream, e_msg_c_as_login_if_no_register_req, &login_req);

		if ( res.error_code() == e_asc_errorcode_success){
			return true;
		}
		else{
			return false;
		}
	}
	else{
		PRINTF_PBPARSE_ERROR(e_msg_as_c_register_res);
		return false;
	}
}

bool CClientForAS::handle_msg_login_res( uv_stream_t* stream, const char * buf, int len )
{
	as_c_login_res res;
	bool r = res.ParseFromArray(buf, len);
	if (r)
	{
		printf("登陆结果[%d][%s] \n", res.error_code(), e_asc_errorcode_Name((e_asc_errorcode)res.error_code()).c_str());
		//return res.error_code() == e_asc_errorcode_success;

		if (res.error_code() != e_asc_errorcode_success)
		{
			//printf("msg[%d][%s] error[%d][%s]", msg_id, e_msg_as_c_Name((e_msg_as_c)msg_id).c_str(),  res.error_code(), e_asc_errorcode_Name((e_asc_errorcode)res.error_code()).c_str());
		}
		else{
			const area_list& areas = res.areas();
			c_as_enter_area_req req;

			const area& a = areas.areas(0);
			req.set_area_id(a.area_id());

			send(stream, e_msg_c_as_enter_area_req, &req);
		}
	}
	else{
		PRINTF_PBPARSE_ERROR(e_msg_as_c_login_res);
		return false;
	}

	return true;
}

bool CClientForAS::handle_msg_login_if_no_register_res( uv_stream_t* stream, const char * buf, int len )
{
	as_c_login_res res;
	bool r = res.ParseFromArray(buf, len);
	if (r)
	{
		printf("登陆（注册）结果[%d][%s] \n", res.error_code(), e_asc_errorcode_Name((e_asc_errorcode)res.error_code()).c_str());
		//return res.error_code() == e_asc_errorcode_success;

		if (res.error_code() != e_asc_errorcode_success)
		{
			//printf("msg[%d][%s] error[%d][%s]", msg_id, e_msg_as_c_Name((e_msg_as_c)msg_id).c_str(),  res.error_code(), e_asc_errorcode_Name((e_asc_errorcode)res.error_code()).c_str());
		}
		else{
			const area_list& areas = res.areas();
			c_as_enter_area_req req;

			const area& a = areas.areas(0);
			req.set_area_id(a.area_id());

			send(stream, e_msg_c_as_enter_area_req, &req);
		}
	}
	else{
		PRINTF_PBPARSE_ERROR(e_msg_as_c_login_if_no_register_res);
		return false;
	}

	return true;
}

bool CClientForAS::handle_msg_enter_area_res( uv_stream_t* stream, const char * buf, int len )
{
	as_c_enter_area_res res;
	bool r = res.ParseFromArray(buf, len);
	if (r)
	{
		printf("选区结果[%d][%s] \n", res.error_code(), e_asc_errorcode_Name((e_asc_errorcode)res.error_code()).c_str());
		
		if (res.error_code() == e_asc_errorcode_success)
		{

			//单个玩家的时候
			////启动gs客户端线程
			//CClientForGS::GetInstance()->Init();
			//CClientForGS::GetInstance()->ip = res.ip();
			//CClientForGS::GetInstance()->port = res.port();
			//ASSERT(res.mutable_token()->size() == GUID_BYTE_LEN);
			////CClientForGS::GetInstance()->token = Guid(res.mutable_token()->c_str());//注意 这种写法得不到正确的token数据！
			//CClientForGS::GetInstance()->token = Guid((BYTE*)(res.mutable_token()->c_str()));
			//CClientForGS::GetInstance()->Start();

			//多个玩家的时候
			//启动gs客户端线程
			CClientForGS* client_gs = new CClientForGS;
			client_gs->Init();
			client_gs->ip = res.ip();
			client_gs->port = res.port();
			ASSERT(res.mutable_token()->size() == GUID_BYTE_LEN);
			//CClientForGS::GetInstance()->token = Guid(res.mutable_token()->c_str());//注意 这种写法得不到正确的token数据！
			client_gs->token = Guid((BYTE*)(res.mutable_token()->c_str()));
			client_gs->Start();

			CUserMgr::GetAllGSUsers().push_back(client_gs);
		}
	}
	else{
		PRINTF_PBPARSE_ERROR(e_msg_as_c_enter_area_res);
		return false;
	}
	ASSERT(r);

	return true;
}
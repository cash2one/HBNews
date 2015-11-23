#include "client_for_ps.h"
#include "def.h"
#include "net_config.h"
#include "libuv_helper.h"

#include "server_for_user.h"
#include "game_config.h"

bool CClientForPS::Init()
{
	ASSERT(add_message_handle(e_msg_ps_gs::e_msg_ps_gs_paied_req, handle_msg_paied_req));
	return true;
}

bool CClientForPS::Start()
{
	int r = uv_thread_create(&m_thread, run, this);
	return r == 0;
}

bool CClientForPS::Join()
{
	int r = uv_thread_join(&m_thread);
	ASSERT( r == 0) ;
	return true;
}

uv_async_t* CClientForPS::CreateAsync()
{
	ASSERT(m_loop);
	uv_async_t* async = (uv_async_t*)malloc(sizeof uv_async_t);
	ASSERT(async);
	int r = uv_async_init(m_loop, async, async_cb);
	ASSERT(r == 0);
	return async;
}

void CClientForPS::DestroyAsync(uv_async_t* handle)
{
	ASSERT(handle);
	free(handle);
}

bool CClientForPS::SendAsync( UINT32 msg_id, google::protobuf::Message* pMessage )
{
	//这个地方发给db   这里不等db处理完毕 直接回复客户端  db
	CClientForPSASync* asynData = new CClientForPSASync();
	asynData->msg_id = msg_id;
	//asynData->user_connect_id = ctx->ConnectID;//这里不需要设置了 在获取userInfo之后 gs已经把userid和链接 绑定在一起了 不需要直接用链接id 反而容易出错！
	asynData->msg = pMessage;

	uv_async_t* async = CClientForPS::GetInstance()->CreateAsync();
	async->data = asynData;

	int r = uv_async_send(async);
	ASSERT(r == 0);
	if (r != 0){
		PRINTF_UV_ERROR(r);
		//delete db_req;
		delete asynData;
		async->data = NULL;
		CClientForPS::GetInstance()->DestroyAsync(async);
		return false;
	}

	return true;
}

void CClientForPS::async_cb( uv_async_t* handle )
{
	CClientForPSASync* async = (CClientForPSASync*)handle->data;
	ASSERT(async && e_msg_gs_ps_IsValid(async->msg_id) && async->msg);
	bool r = CClientForPS::GetInstance()->Send(async->msg_id, async->msg);
	ASSERT(r);
	delete async->msg;

	delete async;
	uv_close((uv_handle_t*)handle, after_async_close);
}

void CClientForPS::run( void* arg )
{
	struct sockaddr_in addr;
	int r;

	CClientForPS* client_for_as = (CClientForPS*)arg;
	ASSERT(client_for_as);

	client_for_as->m_loop = (uv_loop_t*)malloc(sizeof uv_loop_t);
	uv_loop_init(client_for_as->m_loop);

	//ASSERT(0 == uv_ip4_addr("127.0.0.1", 1234, &addr));
	//ASSERT(0 == uv_ip4_addr("211.157.191.149", 1234, &addr));
	//struct sockaddr_in dest = uv_ip4_addr("127.0.0.1", 80);

	//ASSERT(0 == uv_ip4_addr("127.0.0.1", 54321, &addr));
	//ASSERT(0 == uv_ip4_addr("127.0.0.1", 54320, &addr));

	const ClientForPSConfig& cfg = GameConfig::GetInstance()->GetClientForPSConfig();
	const char * ip = cfg.m_ip.c_str();
	int port = cfg.m_port;

	ASSERT(0 == uv_ip4_addr(ip, port, &addr));

	r = uv_tcp_init(client_for_as->m_loop, &client_for_as->m_client);
	ASSERT(r == 0);

	uv_connect_t* connect = (uv_connect_t*)req_alloc();
	r = uv_tcp_connect(connect, &client_for_as->m_client, (const struct sockaddr*)(&addr), on_server_connection);
	ASSERT(r == 0);

	//uv_idle_init(loop, &idle);
	//uv_idle_start(&idle,idle_cb);

	r = uv_run(client_for_as->m_loop, UV_RUN_DEFAULT);
	//ASSERT(0 && "Blackhole server dropped out of event loop.");

	ASSERT(r == 0);
}

void CClientForPS::on_server_connection( uv_connect_t* req, int status )
{
	ASSERT(req);
	uv_stream_t* con = req->handle;

	req_free((uv_req_t*)req);
	//ASSERT(status == 0);
	if (status == 0)
	{
		printf("PS服务器连接成功！\n");
		int r = uv_read_start(con, alloc_cb, on_receive);
		if (r == 0) {
			printf("PS服务器连接读启动成功！\n");
		}
		else{
			PRINTF_UV_ERROR(r);
			FATAL("PS服务器 uv_read_start failed \n");
		}
	}
	else{
		//printf("%s:%s", uv_err_name(status), uv_strerror(status));
		PRINTF_UV_ERROR(status);
		FATAL("PS服务器连接失败！ \n");
	}
}

void CClientForPS::on_receive( uv_stream_t* stream, ssize_t nread, const uv_buf_t* buf )
{
	int r;
	//todo net_protect作一些限制处理
	if (nread > 0){
		CClientForPS::GetInstance()->process_msg(stream, buf->base, nread);
		free(buf->base);//释放内存
	}
	else if (nread == 0)//OK, but nothing read
	{

	}
	else{

		//ASSERT(uv_last_error(stream->loop).code == UV_EOF);

		if (nread == UV_EOF) {
			printf("PS链接主动断开\n");

		} else if (nread == UV_ECONNRESET) {
			printf("PS链接异常断开\n");

		} else {
			//printf("%s : %s \n", uv_err_name(nread), uv_strerror(nread));

			printf("PS链接异常\n");
			PRINTF_UV_ERROR(nread);
		}
		uv_shutdown_t* req = (uv_shutdown_t*)req_alloc();
		r = uv_shutdown(req, stream, after_shutdown);
		ASSERT(r == 0);
	}
}

void CClientForPS::after_shutdown( uv_shutdown_t* req, int status )
{
	ASSERT(req);
	ASSERT(req->handle);
	uv_close((uv_handle_t*)req->handle, after_close);
	req_free((uv_req_t*)req);
}

void CClientForPS::after_close( uv_handle_t* handle )
{
	uv_tcp_t* client = (uv_tcp_t*)handle;
	ASSERT(client == &(CClientForPS::GetInstance()->m_client));
}

bool CClientForPS::send( uv_stream_t* stream, UINT32 msg_id, google::protobuf::Message* pMessage/*, uv_write_cb write_cb*/ )
{
	ASSERT(uv_is_writable(stream));
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
	r = uv_write(write_req, stream, &ret_buf, 1, after_send);
	buf_free(&ret_buf);

	ASSERT(r == 0);
	if (r != 0)
	{
		PRINTF_UV_ERROR(r);
		return false;
	}
	else{
		printf("CClientForPS::send msgid[%d]  bytes len[%d] \n", msg_id, PACKET_HEAD_LEN + body_len);
		return true;
	}
}

bool CClientForPS::Send( UINT32 msg_id, google::protobuf::Message* pMessage )
{
	return send((uv_stream_t*)(&m_client), msg_id, pMessage);
}


void CClientForPS::after_send( uv_write_t* req, int status )
{
	ASSERT(req != NULL);
	if (status == 0)
	{
		//write_cb_called++;
		printf("CClientForPS::after_send 给PS发送信息成功\n");
	}
	else{
		if (status == UV_EOF) {
			printf("写时, PS链接主动断开\n");

		} else if (status == UV_ECONNRESET) {
			printf("写时, PS链接异常断开\n");

		} else {
			//printf("%s : %s \n", uv_err_name(status), uv_strerror(status));

			printf("写时, PS链接异常\n");
			PRINTF_UV_ERROR(status);
		}

		uv_shutdown_t* shut_r = (uv_shutdown_t*)req_alloc();
		int r = uv_shutdown(shut_r, req->handle, after_shutdown);
		ASSERT(r == 0);
	}

	req_free((uv_req_t*)req);
}

void CClientForPS::after_async_close( uv_handle_t* handle )
{
	ASSERT(handle != NULL);
	//free((uv_async_t*)handle);
	CClientForPS::GetInstance()->DestroyAsync((uv_async_t*)handle);
}

bool CClientForPS::add_message_handle( e_msg_ps_gs msgID, func_message_handle handle )
{
	if (m_mapHandle[msgID]){
		printf("already exsit handle for %u", msgID);
		return false;
	}

	m_mapHandle[msgID] = handle;
	return true;
}

bool CClientForPS::process_msg( uv_stream_t* stream, const char * buf, int len )
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
		ASSERT(msg_id < e_msg_ps_gs::e_msg_ps_gs_max);
		p += sizeof msg_id;

		func_message_handle pMsgHandle = m_mapHandle[msg_id];
		if (pMsgHandle)
		{
			bool r = pMsgHandle(stream, p, msg_len - sizeof(msg_len) - sizeof(msg_id));
		}
		else{
			printf("CClientForPS::process_msg没有找到消息msgid[%d][%s]的处理函数\n", msg_id, e_msg_ps_gs_Name((e_msg_ps_gs)msg_id).c_str());
			//return false;
		}

		p += msg_len - sizeof(msg_len) - sizeof(msg_id);
	}

	return true;
}

bool CClientForPS::handle_msg_paied_req( uv_stream_t* stream, const char * buf, int len )
{
	ps_gs_paied_req *req = new ps_gs_paied_req;
	bool r = req->ParseFromArray(buf, len);
	ASSERT(r);

	//发送命令给 server_for_user线程

	CServerForUser::GetInstance()->SendAsync(Guid(), CServerForUserAsync::EAsyncOP_PS_Paied, req);

	return true;
}
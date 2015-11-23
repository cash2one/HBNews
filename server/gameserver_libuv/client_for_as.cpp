#include "client_for_as.h"
#include "def.h"
#include "net_config.h"
#include "libuv_helper.h"

#include "server_for_user.h"
#include "game_config.h"

bool CClientForAS::Init()
{
	ASSERT(add_message_handle(e_msg_as_gs::e_msg_as_gs_enter_area_req, handle_msg_enter_area_req));
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

	for( std::map<Guid, CUserLoginToken*>::iterator it = m_tokens.begin(); it != m_tokens.end() ; ++it){
		delete it->second;
		it->second = NULL;
	}

	m_tokens.clear();
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

void CClientForAS::async_cb( uv_async_t* handle )
{
	CClientForASASync* async = (CClientForASASync*)handle->data;
	ASSERT(async->op);
	switch (async->op)
	{
	case (int)CClientForASASync::EASyncOP_CheckToken_Req:
		{
			//Guid* token = (Guid*)async->data;
			//CUserLoginToken* userLoginInfo = CClientForAS::GetInstance()->get_user_login_token(*token);
			//delete token;

			typedef struct stSenderData{
				Guid token;
				Guid connect_id;
			}stSenderData;

			stSenderData* sender_data = (stSenderData*)async->data;
			ASSERT(sender_data);

			CServerForUserAsync* asyncOP = new CServerForUserAsync;
			asyncOP->op = CServerForUserAsync::EAsyncOP_Check_Token_Res;
			asyncOP->user_connect_id = sender_data->connect_id;

			CUserLoginToken* userLoginInfo = CClientForAS::GetInstance()->get_user_login_token(sender_data->token);

			delete sender_data;

			if (userLoginInfo)
			{
				asyncOP->user_id = userLoginInfo->user_id;
				//验证成功了 就删掉token！ todo 还要做定时器清理！
				bool r = CClientForAS::GetInstance()->remove_user_login_token(userLoginInfo->token);
				ASSERT(r);
			}
			else{
				asyncOP->user_id = NULL;
			}

			uv_async_t* async = CServerForUser::GetInstance()->CreateAsync();
			async->data = asyncOP;

			int r = uv_async_send(async);
			ASSERT(r == 0);
			if (r != 0)
			{
				PRINTF_UV_ERROR(r);
				delete asyncOP;
				async->data = NULL;
				CServerForUser::GetInstance()->DestroyAsync(async);
			}

		}break;
	}
}

void CClientForAS::run( void* arg )
{
	struct sockaddr_in addr;
	int r;

	CClientForAS* client_for_as = (CClientForAS*)arg;
	ASSERT(client_for_as);

	client_for_as->m_loop = (uv_loop_t*)malloc(sizeof uv_loop_t);
	uv_loop_init(client_for_as->m_loop);

	//ASSERT(0 == uv_ip4_addr("127.0.0.1", 1234, &addr));
	//ASSERT(0 == uv_ip4_addr("211.157.191.149", 1234, &addr));
	//struct sockaddr_in dest = uv_ip4_addr("127.0.0.1", 80);

	//ASSERT(0 == uv_ip4_addr("127.0.0.1", 54321, &addr));
	//ASSERT(0 == uv_ip4_addr("127.0.0.1", 54320, &addr));

	const ClientForASConfig& cfg = GameConfig::GetInstance()->GetClientForASConfig();
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

void CClientForAS::on_server_connection( uv_connect_t* req, int status )
{
	ASSERT(req);
	uv_stream_t* con = req->handle;

	req_free((uv_req_t*)req);
	//ASSERT(status == 0);
	if (status == 0)
	{
		printf("AS服务器连接成功！\n");
		int r = uv_read_start(con, alloc_cb, on_receive);
		if (r == 0) {
			printf("AS服务器连接读启动成功！\n");
		}
		else{
			PRINTF_UV_ERROR(r);
			FATAL("AS服务器 uv_read_start failed \n");
		}
	}
	else{
		//printf("%s:%s", uv_err_name(status), uv_strerror(status));
		PRINTF_UV_ERROR(status);
		FATAL("AS服务器连接失败！ \n");
	}
}

void CClientForAS::on_receive( uv_stream_t* stream, ssize_t nread, const uv_buf_t* buf )
{
	int r;
	//todo net_protect作一些限制处理
	if (nread > 0){
		CClientForAS::GetInstance()->process_msg(stream, buf->base, nread);
		free(buf->base);//释放内存
	}
	else if (nread == 0)//OK, but nothing read
	{

	}
	else{

		//ASSERT(uv_last_error(stream->loop).code == UV_EOF);

		if (nread == UV_EOF) {
			printf("AS链接主动断开\n");

		} else if (nread == UV_ECONNRESET) {
			printf("AS链接异常断开\n");

		} else {
			//printf("%s : %s \n", uv_err_name(nread), uv_strerror(nread));

			printf("AS链接异常\n");
			PRINTF_UV_ERROR(nread);
		}
		uv_shutdown_t* req = (uv_shutdown_t*)req_alloc();
		r = uv_shutdown(req, stream, after_shutdown);
		ASSERT(r == 0);
	}
}

void CClientForAS::after_shutdown( uv_shutdown_t* req, int status )
{
	ASSERT(req);
	ASSERT(req->handle);
	uv_close((uv_handle_t*)req->handle, after_close);
	req_free((uv_req_t*)req);
}

void CClientForAS::after_close( uv_handle_t* handle )
{
	uv_tcp_t* client = (uv_tcp_t*)handle;
	ASSERT(client == &(CClientForAS::GetInstance()->m_client));
}

bool CClientForAS::send( uv_stream_t* stream, UINT32 msg_id, google::protobuf::Message* pMessage/*, uv_write_cb write_cb*/ )
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
		printf("CClientForAS::send msgid[%d]  bytes len[%d] \n", msg_id, PACKET_HEAD_LEN + body_len);
		return true;
	}
}

void CClientForAS::after_send( uv_write_t* req, int status )
{
	ASSERT(req != NULL);
	if (status == 0)
	{
		//write_cb_called++;
		printf("CClientForAS::after_send 给AS发送信息成功\n");
	}
	else{
		if (status == UV_EOF) {
			printf("写时, AS链接主动断开\n");

		} else if (status == UV_ECONNRESET) {
			printf("写时, AS链接异常断开\n");

		} else {
			//printf("%s : %s \n", uv_err_name(status), uv_strerror(status));

			printf("写时, AS链接异常\n");
			PRINTF_UV_ERROR(status);
		}

		uv_shutdown_t* shut_r = (uv_shutdown_t*)req_alloc();
		int r = uv_shutdown(shut_r, req->handle, after_shutdown);
		ASSERT(r == 0);
	}

	req_free((uv_req_t*)req);
}

void CClientForAS::after_async_close( uv_handle_t* handle )
{
	ASSERT(handle != NULL);
	//free((uv_async_t*)handle);
	CClientForAS::GetInstance()->DestroyAsync((uv_async_t*)handle);
}

bool CClientForAS::add_message_handle( e_msg_as_gs msgID, func_message_handle handle )
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
		ASSERT(msg_id < e_msg_as_gs::e_msg_as_gs_max);
		p += sizeof msg_id;

		func_message_handle pMsgHandle = m_mapHandle[msg_id];
		if (pMsgHandle)
		{
			bool r = pMsgHandle(stream, p, msg_len - sizeof(msg_len) - sizeof(msg_id));
		}
		else{
			printf("CClientForAS::process_msg没有找到消息msgid[%d][%s]的处理函数\n", msg_id, e_msg_as_gs_Name((e_msg_as_gs)msg_id).c_str());
			//return false;
		}

		p += msg_len - sizeof(msg_len) - sizeof(msg_id);
	}

	return true;
}

bool CClientForAS::handle_msg_enter_area_req( uv_stream_t* stream, const char * buf, int len )
{
	as_gs_enter_area_req req;
	req.ParseFromArray(buf, len);

	//Guid token = req.token();//注意！这种写法得不到正确的token数据！
	//Guid token = req.token().c_str();注意！这种写法得不到正确的token数据！



	ASSERT(req.mutable_token()->size() == GUID_BYTE_LEN);
	//Guid token(*req.mutable_token());//注意！这种写法得不到正确的token数据！
	Guid token((BYTE*)req.mutable_token()->c_str());
	CUserLoginToken* data = CClientForAS::GetInstance()->get_user_login_token(token);
	ASSERT( data == NULL);

	gs_as_enter_area_res res;
	//res.set_client_id(req.client_id());//注意！这种写法得不到正确的token数据！ as需要通过这个找到客户端链接回信息的
	//ASSERT(req.client_id().size() == GUID_BYTE_LEN);//注意！这种写法得不到正确的token数据！
	//res.set_client_id(req.client_id().c_str(), GUID_BYTE_LEN);//注意！这种写法得不到正确的token数据！ as需要通过这个找到客户端链接回信息的
	
	ASSERT(req.mutable_client_id()->size() == GUID_BYTE_LEN);
	//res.set_client_id(req.mutable_client_id()->c_str(), GUID_BYTE_LEN);//as需要通过这个找到客户端链接回信息的
	res.set_client_id(*(req.mutable_client_id()));//as需要通过这个找到客户端链接回信息的

	if(data != NULL){
		res.set_error_code(e_asgs_errorcode_has_same_token);
		//res.set_token(req.token());//这里不需要设置 吃掉 因为失败了as也不应该把这个信息发给客户端了
	}
	else{
		//添加用户id！！ 作为key！  需要不需要连成一个链表！ 用userid作为key 那些还没有确认userid的链接 我要怎么操作他们？
		bool r = CClientForAS::GetInstance()->add_user_login_token(req.user_id(), token);
		ASSERT( r == true);
		res.set_error_code(e_asgs_errorcode_success);
		//res.set_token(req.token());//注意！这种写法得不到正确的token数据！因为string构造了 而且用了strlen！
		res.set_token(*(req.mutable_token()));
	}
	send(stream, e_msg_gs_as_enter_area_res, &res);
	return true;
}
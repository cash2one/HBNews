#include <stdio.h>

#include "def.h"
#include "net_config.h"
#include "libuv_helper.h"
#include "db_async_command.h"

#include "server_for_user.h"
#include "server_for_gs.h"
#include "area.h"
#include "str.h"
#include "game_config.h"

static int g_max_client_num = 10000;

bool CServerForUser::Start()
{
	int r = uv_thread_create(&m_thread, run, this);
	return r == 0;
}

bool CServerForUser::Join()
{
	int r = uv_thread_join(&m_thread);
	return r == 0;
}

void CServerForUser::run( void* arg )
{
	struct sockaddr_in addr;
	int r;

	CServerForUser* server_for_user = (CServerForUser*)arg;
	ASSERT(server_for_user);

	server_for_user->m_loop = (uv_loop_t*)malloc(sizeof uv_loop_t);
	uv_loop_init(server_for_user->m_loop);

	const ServerForUserConfig& cfg = GameConfig::GetInstance()->GetServerForUserConfig();
	const char * ip = cfg.m_ip.c_str();
	int port = cfg.m_port;

	r = uv_ip4_addr(ip, port, &addr);
	ASSERT(r == 0);

	r = uv_tcp_init(server_for_user->m_loop, &server_for_user->m_server);
	ASSERT(r == 0);

	r = uv_tcp_bind(&server_for_user->m_server, (const struct sockaddr*) &addr, 0);
	ASSERT(r == 0);

	server_for_user->m_server.data = server_for_user;

	r = uv_listen((uv_stream_t*)&server_for_user->m_server, g_max_client_num, on_client_connection);
	ASSERT(r == 0);

	printf("ServerForUser start success! listen %s:%d \n", ip, port);
	r = uv_run(server_for_user->m_loop, UV_RUN_DEFAULT);
	ASSERT(0 && "server exit!");

	uv_loop_close(server_for_user->m_loop);
	free(server_for_user->m_loop);
}

void CServerForUser::on_client_connection( uv_stream_t* stream, int status )
{
	int r;
	struct sockaddr sockname, peername;
	int namelen;

	ASSERT(stream);
	ASSERT(status == 0);
	CServerForUser* server_for_user = (CServerForUser*)stream->data;
	ASSERT(server_for_user);
	ASSERT(stream == (uv_stream_t*)&(server_for_user->m_server));

	uv_tcp_t* handle = new uv_tcp_t;
	ASSERT(handle);
	r = uv_tcp_init(stream->loop, handle);
	ASSERT(r == 0);

	printf("new connect incoming, before accept! \n");
	r = uv_accept(stream, (uv_stream_t*)handle);
	ASSERT(r == 0);

	memset(&sockname, -1, sizeof sockname);  
	namelen = sizeof sockname;  
	r = uv_tcp_getsockname(handle, &sockname, &namelen);  
	//printf("the r is %d.\n", r);
	ASSERT( r == 0);

	char ip[17] = {0};
	struct sockaddr_in sock_addr = *(struct sockaddr_in*) &sockname; 
	//网络字节序转换成主机字符序  
	uv_ip4_name(&sock_addr, (char*)ip, sizeof ip);  
	printf("getsockname   %s : %d \n", ip, sock_addr.sin_port);


	//有连接，可以获得目标的ip和端口  
	namelen = sizeof peername;  
	r = uv_tcp_getpeername(handle, &peername, &namelen);  
	//printf("the r is %d.\n", r);
	ASSERT( r == 0);

	memset(ip, 0, sizeof ip);
	sock_addr = *(struct sockaddr_in*) &peername; 
	//网络字节序转换成主机字符序  
	uv_ip4_name(&sock_addr, (char*)ip, sizeof ip);  
	printf("getpeername  %s : %d \n", ip, sock_addr.sin_port);

	//todo net_protect作一些限制处理

	printf("[%p]new connect accepted! \n", handle);
	r = uv_read_start((uv_stream_t*)handle, alloc_cb, on_receive);
	ASSERT(r == 0);

	CServerForUser::GetInstance()->add_client(handle);
}

void CServerForUser::on_receive( uv_stream_t* stream, ssize_t nread, const uv_buf_t* buf )
{
	int r;
	//todo net_protect作一些限制处理
	if (nread > 0){
		CServerForUser::GetInstance()->process_msg(stream, buf->base, nread);
		free(buf->base);//释放内存
	}
	else if (nread == 0)//OK, but nothing read
	{

	}
	else{

		//ASSERT(uv_last_error(stream->loop).code == UV_EOF);

		if (nread == UV_EOF) {
			printf("[%p]玩家链接主动断开\n", stream);

		} else if (nread == UV_ECONNRESET) {
			printf("[%p]玩家链接异常断开 connection was forcibly closed by the remote host\n", stream);

		} else {
			//printf("%s : %s \n", uv_err_name(nread), uv_strerror(nread));

			printf("[%p]玩家链接异常\n", stream);
			PRINTF_UV_ERROR(nread);
		}
		//uv_shutdown_t* req = (uv_shutdown_t*)req_alloc();
		//r = uv_shutdown(req, stream, after_client_shutdown);
		////ASSERT(r == 0);//可能会出现这种情况
		//if ( r != 0){
		//	printf("并且uv_shutdown失败！准备直接close！！\n");
		//	if(uv_is_active((uv_handle_t*)stream)){
		//		uv_close((uv_handle_t*)stream, after_client_close);
		//	}
		//	else{
		//		after_client_close((uv_handle_t*)stream);
		//	}
		//}

		CServerForUser::GetInstance()->close_client(stream);
	}
}

void CServerForUser::after_client_shutdown( uv_shutdown_t* req, int status )
{
	ASSERT(req);
	ASSERT(req->handle);
	uv_close((uv_handle_t*)req->handle, after_client_close);
	//if(!uv_is_closing((uv_handle_t*)req->handle)){
	//	uv_close((uv_handle_t*)req->handle, after_client_close);
	//}
	//else{
	//	//不然可能会有这种情况 调用了两次此函数 一次uv_close了 一次after_client_close释放了对象 然后uv_close设置end_game队列 在处理的时候就野指针了！
	//	//after_client_close((uv_handle_t*)req->handle);
	//	printf("uv_is_closing");
	//}
	req_free((uv_req_t*)req);
}

void CServerForUser::after_client_close( uv_handle_t* handle )
{
	//uv_tcp_t* client = (uv_tcp_t*)handle;
	//ASSERT(client);
	
	ASSERT(handle);
	CUserContext* ctx = (CUserContext*)handle->data;
	ASSERT(ctx);
	printf("[%p]user[%u] 链接已断开\n", handle, ctx->user_id);
	//delete client;
	ASSERT(CServerForUser::GetInstance()->remove_client(ctx->client_id));
}

bool CServerForUser::send( uv_stream_t* stream, UINT32 msg_id, google::protobuf::Message* pMessage/*, uv_write_cb write_cb*/ )
{
	//printf("[%p]begin CServerForUser::send msgid[%d][%s] \n", stream, msg_id, e_msg_as_c_Name((e_msg_as_c)msg_id).c_str());
	//ASSERT(uv_is_writable(stream));//可能会出现的正常情况！
	if (!uv_is_writable(stream)){
		printf("\n\n\n[%p]CServerForUser::send  uv_is_writable false!!! \n\n\n", stream);
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
	write_req->data = (void*)msg_id;
	r = uv_write(write_req, stream, &ret_buf, 1, after_send);
	buf_free(&ret_buf);

	//ASSERT(r == 0);//客户端进程突然被杀掉的时候会出现这种情况
	if (r != 0)
	{
		PRINTF_UV_ERROR(r);
		return false;
	}
	else{
		printf("[%p]CServerForUser::send msgid[%d][%s]  bytes len[%d] \n", stream, msg_id, e_msg_as_c_Name((e_msg_as_c)msg_id).c_str() , PACKET_HEAD_LEN + body_len);
		return true;
	}
}

void CServerForUser::after_send( uv_write_t* req, int status )
{
	ASSERT(req != NULL);
	if (status == 0)
	{
		//write_cb_called++;
		printf("[%p]CServerForUser::after_send 给user发送信息成功\n", req->handle);

		//会导致libuv客户端退出！
		//UINT32 msg_id = (UINT32)req->data;
		//if ( msg_id == e_msg_as_c_enter_area_res)
		//{
		//	printf("AS->C 发送完e_msg_as_c_enter_area_res, 主动断开玩家的链接 \n");
		//	uv_shutdown_t* shut_r = (uv_shutdown_t*)req_alloc();
		//	int r = uv_shutdown(shut_r, req->handle, after_client_shutdown);
		//	ASSERT(r == 0);
		//}
	}
	else{
		if (status == UV_EOF) {
			printf("[%p]写时, 玩家链接主动断开\n", req->handle);

		} else if (status == UV_ECONNRESET) {
			printf("[%p]写时, 玩家链接异常断开\n", req->handle);

		} else {
			//printf("%s : %s \n", uv_err_name(status), uv_strerror(status));
			printf("[%p]写时, 玩家链接异常\n", req->handle);
			PRINTF_UV_ERROR(status);
		}

		//uv_shutdown_t* shut_r = (uv_shutdown_t*)req_alloc();
		//int r = uv_shutdown(shut_r, req->handle, after_client_shutdown);
		//ASSERT(r == 0);
		//if (r != 0){
		//	if(uv_is_active((uv_handle_t*)req->handle)){
		//		uv_close((uv_handle_t*)req->handle, after_client_close);
		//	}
		//	else{
		//		after_client_close((uv_handle_t*)req->handle);
		//	}
		//}

		CServerForUser::GetInstance()->close_client(req->handle);
	}

	req_free((uv_req_t*)req);
}

bool CServerForUser::process_msg( uv_stream_t* stream, const char * buf, int len )
{
	ASSERT(buf);
	ASSERT(len >= PACKET_HEAD_LEN);//
	int pos = 0;
	int bytes_left = len;


	////如果没有登录 除了login消息 其他消息不处理！
	//stConnectUserData* pUserData = (stConnectUserData*)stream->data;
	//ASSERT(pUserData);
	//UINT32 first_msg_id = *(UINT32*)(buf+4);
	////todo delete 本地测试 暂时跳过登陆验证
	//if (false && !pUserData->iUserID && first_msg_id != e_msg_c_gs::e_msg_c_gs_login_req)//第一个消息必须是注册
	//{
	//	gs_c_error_report rpn;
	//	rpn.set_error_code(EErrorCode_Should_Login_Before_Other);
	//	send(stream, e_msg_gs_c::e_gs_c_error_report, &rpn, write_cb);
	//	return false;
	//}

	char* p = const_cast<char*>(buf);
	while (bytes_left != 0)
	{
		UINT32 msg_len = *(UINT32 *)(p);
		p += sizeof msg_len;

		ASSERT(bytes_left >= msg_len);
		bytes_left -= msg_len;

		UINT32 msg_id = *(UINT32 *)(p);
		ASSERT(msg_id < e_msg_c_as::e_msg_c_as_max);
		p += sizeof msg_id;

		func_message_handle pMsgHandle = m_mapHandle[msg_id];
		if (pMsgHandle)
		{
			bool r = pMsgHandle(stream, p, msg_len - sizeof(msg_len) - sizeof(msg_id));
		}
		else{
			printf("CServerForUser::process_msg没有找到msdid[%d][%s]的消息处理函数\n", msg_id, e_msg_c_as_Name(e_msg_c_as(msg_id)).c_str());
			//return false;
		}

		p += msg_len - sizeof(msg_len) - sizeof(msg_id);
	}

	return true;
}

bool CServerForUser::add_message_handle( e_msg_c_as msgID, func_message_handle handle )
{
	if (m_mapHandle[msgID])
	{
		printf("already exsit handle for %u", msgID);
		return false;
	}

	m_mapHandle[msgID] = handle;
	return true;
}

void CServerForUser::async_cb( uv_async_t* handle )
{
	ASSERT(handle);
	stUserAsyncData* asyncData = (stUserAsyncData*)handle->data;
	ASSERT(asyncData);
	//do something
	switch (asyncData->op)
	{
	case EUserAsyncOP_Register:{
		as_c_register_res* res = (as_c_register_res*)asyncData->data;
		ASSERT(res);
		//if (res->error_code() == e_asc_errorcode_success)
		//{
		//	//res->set_token("123456");
		//}

		uv_tcp_t* client = CServerForUser::GetInstance()->get_client(asyncData->client_id);
		if (client == NULL)
		{
			printf("\n EAsyncOP_Register 回调回来 客户端已经断开！！！\n ");
		}
		else{
			send((uv_stream_t*)client, e_msg_as_c_register_res, res);
		}
		
		delete res;

	}break;

	case EUserAsyncOP_Login:{
		as_c_login_res* res = (as_c_login_res*)asyncData->data;
		//send();
		ASSERT(res);
		uv_tcp_t* client = CServerForUser::GetInstance()->get_client(asyncData->client_id);
		if (client == NULL)
		{
			printf("\n EUserAsyncOP_Login 回调回来 客户端已经断开！！！\n ");
		}
		else{
			if (res->error_code() == e_asc_errorcode_success)
			{
				CUserContext* ctx = (CUserContext*)client->data;
				ASSERT(ctx);
				ctx->stauts = CUserContext::EClientStatus::EClientStatus_Logined;
				ctx->user_id = asyncData->user_id;
				//获取区服列表
				CServerForGS::GetInstance()->GetAreaList(res->mutable_areas());
			}
			send((uv_stream_t*)client, e_msg_as_c_login_res, res);
		}
		delete res;
	}break;

	case EUserAsyncOP_Enter_Area:{

		typedef struct  
		{
			gs_as_enter_area_res * res;
			UINT32 area_id;
		} enter_area_res_info;

		enter_area_res_info* info = (enter_area_res_info*)asyncData->data;
		ASSERT(info);
		//as_gs_enter_area_res* res = info->res;
		ASSERT(info->res);
		uv_tcp_t* client = CServerForUser::GetInstance()->get_client(asyncData->client_id);
		if (client == NULL)
		{
			printf("\n EUserAsyncOP_Enter_Area 回调回来 客户端已经断开！！！\n ");
		}    
		else{
			as_c_enter_area_res res;
			if(info->res->error_code() == e_asgs_errorcode_success){
				res.set_error_code(e_asc_errorcode_success);

				CArea* pArea = CServerForGS::GetInstance()->GetAreaByAreaID(info->area_id);
				ASSERT(pArea);
				char* buf = ANSIToUTF8(pArea->ip.c_str());
				res.set_ip(buf);
				free(buf);
				res.set_port(pArea->port);
				//res.set_token(info->res->token());//注意！ 这种得不到正确数据 因为string拷贝 用的strlen！
				res.set_token(*(info->res->mutable_token()));
			}
			else{
				res.set_error_code(e_asc_errorcode_unkown_error);
			}

			send((uv_stream_t*)client, e_msg_as_c_enter_area_res, &res);

			////todo 不管如何 断开链接！
			////不管gs返回如何 都需要断开客户端链接
			//uv_shutdown_t* req = (uv_shutdown_t*)req_alloc();
			//int r = uv_shutdown(req, (uv_stream_t *)client, after_client_shutdown);
			////ASSERT(r == 0);//可能会出现这种情况
			//if ( r != 0){
			//	printf("as主动关闭c链接！uv_shutdown失败！准备直接close！！\n");
			//	if(uv_is_active((uv_handle_t*)client)){
			//		uv_close((uv_handle_t*)client, after_client_close);
			//	}
			//	else{
			//		after_client_close((uv_handle_t*)client);
			//	}
			//}

			CServerForUser::GetInstance()->close_client((uv_stream_t*)client);
		}
		delete info->res;
		delete info;
     }break;

	}

	delete asyncData;
	uv_close((uv_handle_t*)handle, after_async_close);
}


bool CServerForUser::handle_msg_register( uv_stream_t* stream, const char * buf, int len )
{
	printf("[%p]CServerForUser::handle_msg_register! \n", stream);
	ASSERT(stream && stream->data);

	c_as_register_req st;
	int r = st.ParseFromArray(buf, len);
	ASSERT(r == true);
	if (r)
	{
		CAccountCommand* commond = new CAccountCommand();
		commond->Optype(CAccountCommand::REGISTER);
		commond->Plantform_id(st.plantform_id());
		commond->Plantform_sub_id(st.plantform_sub_id());
		commond->Account(st.account());
		commond->Password(st.password());

		commond->Client_id(((CUserContext*)(stream->data))->client_id);//todo 野指针!

		CAsyncCommandModule::GetInstance()->AddCommand(commond);
		return true;
	}
	else{
		as_c_register_res res;
		res.set_error_code(e_asc_errorcode_unkown_error);
		send(stream, e_msg_as_c_register_res, &res);
		return false;
	}
}

bool CServerForUser::handle_msg_login( uv_stream_t* stream, const char * buf, int len )
{
	printf("[%p]CServerForUser::handle_msg_login! \n", stream);
	ASSERT(stream && stream->data);

	c_as_login_req st;
	bool r = st.ParseFromArray(buf, len);
	ASSERT( r == true);
	if (r)
	{
		CAccountCommand* commond = new CAccountCommand();
		commond->Optype(CAccountCommand::LOGIN);
		commond->Plantform_id(st.plantform_id());
		commond->Plantform_sub_id(st.plantform_sub_id());
		commond->Account(st.account());
		commond->Password(st.password());

		CUserContext* ctx = (CUserContext*)(stream->data);
		ASSERT(ctx);
		commond->Client_id(ctx->client_id);//todo 野指针!

		CAsyncCommandModule::GetInstance()->AddCommand(commond);
	}
	else{
		as_c_login_res res;
		res.set_error_code(e_asc_errorcode_unkown_error);
		send(stream, e_msg_as_c_login_res, &res);
	}

	return true;
}

bool CServerForUser::handle_msg_login_if_no_register( uv_stream_t* stream, const char * buf, int len )
{
	printf("[%p]CServerForUser::handle_msg_login_if_no_register! \n", stream);
	ASSERT(stream && stream->data);

	c_as_login_if_no_register_req st;
	bool r = st.ParseFromArray(buf, len);
	ASSERT( r == true);
	if (r)
	{
		CAccountCommand* commond = new CAccountCommand();
		commond->Optype(CAccountCommand::LOGIN_IF_NO_REGISTER);
		commond->Plantform_id(st.plantform_id());
		commond->Plantform_sub_id(st.plantform_sub_id());
		commond->Account(st.account());
		commond->Password(st.password());

		CUserContext* ctx = (CUserContext*)(stream->data);
		ASSERT(ctx);
		commond->Client_id(ctx->client_id);//todo 野指针!

		CAsyncCommandModule::GetInstance()->AddCommand(commond);
	}
	else{
		as_c_login_if_no_register_res res;
		res.set_error_code(e_asc_errorcode_unkown_error);
		send(stream, e_msg_as_c_login_if_no_register_res, &res);
	}

	return true;
}

Guid CServerForUser::add_client( uv_tcp_t* client )
{
	ASSERT(client);
	Guid key = GetGuidGenerator().newGuid();
	ASSERT(m_clients.find(key) == m_clients.end());
	m_clients[key] = client;

	CUserContext* ctx = new CUserContext;
	ctx->client_id = key;
	ctx->stauts = CUserContext::EClientStatus::EClientStatus_Connected;
	client->data = ctx;

	return key;
}

bool CServerForUser::remove_client( Guid key )
{
	std::map<Guid, uv_tcp_t*>::iterator it =  m_clients.find(key);
	if (it != m_clients.end())
	{
		ASSERT(it->second);
		CUserContext* ctx = (CUserContext*)it->second->data;
		ASSERT(ctx);
		delete ctx;//删除client相关信息
		it->second->data = NULL;
		delete it->second;//删除client
		m_clients.erase(it);
		return true;
	}
	else{
		return false;
	}
}

uv_tcp_t* CServerForUser::get_client( Guid& connect_id )
{
	std::map<Guid, uv_tcp_t*>::iterator it =  m_clients.find(connect_id);
	if (it != m_clients.end())
	{
		return it->second;
	}
	else{
		return NULL;
	}
}

void CServerForUser::close_client( uv_stream_t* client )
{
	uv_shutdown_t* req = (uv_shutdown_t*)req_alloc();
	printf("as主动关闭c链接[%p]！\n", client);
	int r = uv_shutdown(req, client, after_client_shutdown);
	//ASSERT(r == 0);//可能会出现这种情况
	if ( r != 0){
		printf("uv_shutdown失败！！\n");
		//if(uv_is_active((uv_handle_t*)client)){
		//	uv_close((uv_handle_t*)client, after_client_close);
		//}
		//else{
		//	after_client_close((uv_handle_t*)client);
		//}
		
		PRINTF_UV_ERROR(r);
		//ASSERT(false);
	}
	else{
		//printf("uv_shutdown成功！！\n");
	}
}

uv_async_t* CServerForUser::CreateAsync()
{
	ASSERT(m_loop);
	uv_async_t* async = (uv_async_t*)malloc(sizeof uv_async_t);
	ASSERT(async);
	int r = uv_async_init(m_loop, async, async_cb);
	ASSERT(r == 0);
	return async;
}

void CServerForUser::DestroyAsync( uv_async_t* handle )
{
	ASSERT(handle);
	free(handle);
}

void CServerForUser::after_async_close( uv_handle_t* handle )
{
	ASSERT(handle != NULL);
	//free((uv_async_t*)handle);
	CServerForUser::GetInstance()->DestroyAsync((uv_async_t*)handle);
}

bool CServerForUser::handle_msg_enter_area_req( uv_stream_t* stream, const char * buf, int len )
{
	printf("[%p]CServerForUser::handle_msg_enter_area_req! \n", stream);

	c_as_enter_area_req req;
	req.ParseFromArray(buf, len);

	CUserContext* ctx = (CUserContext*)stream->data;
	ASSERT(ctx);

	if (ctx->stauts < CUserContext::EClientStatus::EClientStatus_Logined)
	{
		as_c_enter_area_res res;
		res.set_error_code(e_asc_errorcode_not_logined);
		send(stream, e_msg_as_c_enter_area_res, &res);
		return false;
	}

	CArea* pArea = CServerForGS::GetInstance()->GetAreaByAreaID(req.area_id());
	if (!pArea)
	{
		as_c_enter_area_res res;
		res.set_error_code(e_asc_errorcode_no_this_area);
		send(stream, e_msg_as_c_enter_area_res, &res);
		return false;
	}

	//在gs线程那边确认即可 这里不需要
	////确认改area的gs已经链接上as
	//uv_tcp_t* connect = CServerForGS::GetInstance()->get_client(pArea->client_id);
	//if (!connect)
	//{
	//	as_c_enter_area_res res;
	//	res.set_error_code(e_asc_errorcode_area_closed);
	//	send(stream, e_msg_as_c_enter_area_res, &res);
	//	return false;
	//}

	//res.set_error_code(e_asc_errorcode_success);
	//user线程先发给gs线程， gs线程发给gs  gs收到回复gs线程 gs线程再发给user线程 user线程再发给用户
	//send(stream, as_c_enter_area_res, res);

	
	stGSAsyncData* asyncData = new stGSAsyncData;
	asyncData->op = EGSAsyncOP_Enter_Area_Req;
	asyncData->area_id = pArea->area_id;
	asyncData->client_id = ctx->client_id;//为了回调回来能知道是哪个用户
	asyncData->user_id = ctx->user_id;


	//asyncData->client_id.ToBytes()
	printf("\n 1   \n");

	Guid token = GetGuidGenerator().newGuid();// 生成用来验证登陆的token
	//BYTE* pTokenbuf = new BYTE[GUID_BYTE_LEN];
	//token.ToBytes(pTokenbuf, GUID_BYTE_LEN);
	//asyncData->data = pTokenbuf;
	asyncData->data = new Guid(token);

	uv_async_t* async = CServerForGS::GetInstance()->CreateAsync();
	async->data = asyncData;

	int r = uv_async_send(async);
	ASSERT(r == 0);
	if (r != 0)
	{
		PRINTF_UV_ERROR(r);
		delete (Guid*)asyncData->data;
		delete asyncData;
		async->data = NULL;
		CServerForGS::GetInstance()->DestroyAsync(async);
	}

	return r == 0;
}
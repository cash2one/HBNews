#include "client_for_gs.h"
#include "def.h"
#include "net_config.h"
#include "libuv_helper.h"
//#include "db_async_command.h"
#include "time_helper.h"
bool CClientForGS::Init(std::string ip, UINT32 port)
{
	m_ip = ip;
	m_port = port;
	ASSERT(add_message_handle(e_msg_gs_gm_marquee_res, handle_msg_marquee_res));
	ASSERT(add_message_handle(e_msg_gs_gm_add_item_res, handle_add_item_res));
	ASSERT(add_message_handle(e_msg_gs_gm_mail_res, handle_mail_res));
	ASSERT(add_message_handle(e_msg_gs_gm_arena_mail_res, handle_arena_mail_res));
	return true;
}

bool CClientForGS::Start()
{
	int r = uv_thread_create(&m_thread, run, this);
	return r == 0;
}

bool CClientForGS::Join()
{
	int r = uv_thread_join(&m_thread);
	ASSERT( r == 0) ;
	return true;
}

uv_async_t* CClientForGS::CreateAsync()
{
	ASSERT(m_loop);
	uv_async_t* async = (uv_async_t*)malloc(sizeof uv_async_t);
	ASSERT(async);
	int r = uv_async_init(m_loop, async, async_cb);
	ASSERT(r == 0);
	return async;
}

void CClientForGS::DestroyAsync(uv_async_t* handle)
{
	ASSERT(handle);
	free(handle);
}

void CClientForGS::run( void* arg )
{
	struct sockaddr_in addr;
	int r;

	CClientForGS* client_gs = (CClientForGS*)arg;
	ASSERT(client_gs);

	client_gs->m_loop = (uv_loop_t*)malloc(sizeof uv_loop_t);
	uv_loop_init(client_gs->m_loop);
	client_gs->m_loop->data = client_gs;

	//ASSERT(0 == uv_ip4_addr("127.0.0.1", 1234, &addr));
	//ASSERT(0 == uv_ip4_addr("211.157.191.149", 1234, &addr));
	//struct sockaddr_in dest = uv_ip4_addr("127.0.0.1", 80);

	//ASSERT(0 == uv_ip4_addr("127.0.0.1", 54321, &addr));
	//ASSERT(0 == uv_ip4_addr("127.0.0.1", 54320, &addr));

//#ifdef _LOCAL
//	char * ip = "127.0.0.1";
//#else
//	char * ip = "211.157.191.149";
//#endif
//	int port = 54322;

	//ASSERT(CClientForGS::GetInstance()->ip != "" && CClientForGS::GetInstance()->port);
	//const char* ip = CClientForGS::GetInstance()->ip.c_str();
	//int port = CClientForGS::GetInstance()->port;

	ASSERT(client_gs->m_ip != "" && client_gs->m_port);
	const char* ip = client_gs->m_ip.c_str();
	int port = client_gs->m_port;

	ASSERT(0 == uv_ip4_addr(ip, port, &addr));

	r = uv_tcp_init(client_gs->m_loop, &client_gs->m_client);
	ASSERT(r == 0);

	uv_connect_t* connect = (uv_connect_t*)req_alloc();
	r = uv_tcp_connect(connect, &client_gs->m_client, (const struct sockaddr*)(&addr), on_server_connection);
	ASSERT(r == 0);

	//uv_idle_init(loop, &idle);
	//uv_idle_start(&idle,idle_cb);

	//空转  
	//uv_idle_init(client_gs->m_loop, &client_gs->m_idler);  
	//uv_idle_start(&client_gs->m_idler, on_idle); 
	
	
	r = uv_run(client_gs->m_loop, UV_RUN_DEFAULT);
	//ASSERT(0 && "Blackhole server dropped out of event loop.");

	ASSERT(r == 0);
}

void CClientForGS::on_server_connection( uv_connect_t* req, int status )
{
	ASSERT(req);
	uv_stream_t* con = req->handle;

	req_free((uv_req_t*)req);
	if (status != 0)
	{
		printf("%s:%s", uv_err_name(status), uv_strerror(status));
	}
	ASSERT(status == 0);

	printf("GS服务器连接成功...\n");
	int r = uv_read_start(con, alloc_cb, on_receive);
	if (r) {
		FATAL("uv_read_start failed");
	}

	printf("GS服务器连接读启动成功...\n");
	ASSERT(r == 0);

	create_timer();

	//gm_gs_enter_gs_req enter_gs_req;
	//BYTE token[GUID_BYTE_LEN] = {0};
	//
	////CClientForGS::GetInstance()->token.ToBytes(token, GUID_BYTE_LEN);
	//CClientForGS* client_gs = get_client((uv_handle_t*)con);
	//ASSERT(client_gs);
	//client_gs->token.ToBytes(token, GUID_BYTE_LEN);

	//enter_gs_req.set_token((const char*)token, GUID_BYTE_LEN);

	//bool ret = send(con, e_msg_gs_gm_enter_gs_res, &enter_gs_req);
	//ASSERT(ret);
}

void CClientForGS::on_receive( uv_stream_t* stream, ssize_t nread, const uv_buf_t* buf )
{
	int r;
	//todo net_protect作一些限制处理
	if (nread > 0){
		//CClientForGS::GetInstance()->process_msg(stream, buf->base, nread);

		CClientForGS* client_gs = get_client((uv_handle_t*)stream);
		ASSERT(client_gs);
		client_gs->process_msg(stream, buf->base, nread);
		
		free(buf->base);//释放内存
	}
	else if (nread == 0)//OK, but nothing read
	{

	}
	else{

		//ASSERT(uv_last_error(stream->loop).code == UV_EOF);

		if (nread == UV_EOF) {
			printf("GS主动断开\n");

		} else if (nread == UV_ECONNRESET) {
			printf("GS异常断开\n");

		} else {
			printf("%s : %s \n", uv_err_name(nread), uv_strerror(nread));
		}
		uv_shutdown_t* req = (uv_shutdown_t*)req_alloc();
		r = uv_shutdown(req, stream, after_shutdown);
		ASSERT(r == 0);
	}
}

void CClientForGS::after_shutdown( uv_shutdown_t* req, int status )
{
	ASSERT(req);
	ASSERT(req->handle);
	uv_close((uv_handle_t*)req->handle, after_close);
	req_free((uv_req_t*)req);
}

void CClientForGS::after_close( uv_handle_t* handle )
{
	uv_tcp_t* client = (uv_tcp_t*)handle;
	//ASSERT(client == &(CClientForGS::GetInstance()->m_client));

	CClientForGS* client_gs = get_client(handle);
	ASSERT(client_gs);
	ASSERT( client == &(client_gs->m_client) );
}

bool CClientForGS::send( uv_stream_t* stream, UINT32 msg_id, google::protobuf::Message* pMessage/*, uv_write_cb write_cb*/ )
{
	//ASSERT(uv_is_writable(stream));//可能会出现的正常情况！
	if (!uv_is_writable(stream)){
		printf("\n\n\n[%p]CClientForGS::send  uv_is_writable false!!! \n\n\n", stream);
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
	ASSERT(r == 0);
	if (r != 0)
	{
		PRINTF_UV_ERROR(r);
		return false;
	}
	else{
		printf("CClientForGS::send msgid[%d][%s]  bytes len[%d] \n", msg_id, e_msg_gm_gs_Name((e_msg_gm_gs)msg_id).c_str(), PACKET_HEAD_LEN + body_len);
		return true;
	}
}

void CClientForGS::after_send( uv_write_t* req, int status )
{
	ASSERT(req != NULL);
	if (status == 0)
	{
		//write_cb_called++;
		printf("CClientForGS::after_send 给GS发送信息成功\n");

		//CClientForGS* client_gs = get_client((uv_handle_t*)req->handle);
		//ASSERT(client_gs);
		//UINT32 user_id = client_gs->m_user_info ? client_gs->m_user_info->base_info().user_id() : 0;
		//CLogCommond* commond = new CLogCommond();
		//commond->Init(CLogCommond::ESERVERTYPE_GS, CLogCommond::ELOGTYPE_SEND, UINT32(req->data), user_id, Now());
		//CAsyncCommandModule::GetInstance()->AddCommand(commond);
	}
	else{
		if (status == UV_EOF) {
			printf("写时, GS主动断开\n");

		} else if (status == UV_ECONNRESET) {
			printf("写时, GS异常断开\n");

		} else {
			printf("%s : %s \n", uv_err_name(status), uv_strerror(status));
		}

		uv_shutdown_t* shut_r = (uv_shutdown_t*)req_alloc();
		int r = uv_shutdown(shut_r, req->handle, after_shutdown);
		ASSERT(r == 0);
	}

	req_free((uv_req_t*)req);
}

void CClientForGS::after_async_close( uv_handle_t* handle )
{
	ASSERT(handle != NULL);
	
	//CClientForGS::GetInstance()->DestroyAsync((uv_async_t*)handle);
	CClientForGS* client_gs = get_client(handle);
	ASSERT(client_gs);
	client_gs->DestroyAsync((uv_async_t*)handle);
}

bool CClientForGS::add_message_handle( e_msg_gs_gm msgID, func_message_handle handle )
{
	if (m_mapHandle[msgID])
	{
		printf("already exsit handle for %u", msgID);
		return false;
	}

	m_mapHandle[msgID] = handle;
	return true;
}

bool CClientForGS::process_msg( uv_stream_t* stream, const char * buf, int len )
{
	ASSERT(buf);
	ASSERT(len >= PACKET_HEAD_LEN);//todo  是否必要？

	//if (!m_msg_buff.empty())//存在一个接收不完全的消息！
	//{
	//	BYTE * pData = &m_msg_buff[0];
	//	UINT32 last_msg_len = *((UINT32*)pData);//上一个接收不完全的消息

	//	if( (m_msg_buff.size() + len) >= last_msg_len){//可以拼成一条完整的消息
	//			
	//	}
	//	else{

	//	}
	//}
	char* p = NULL;//遍历buf的工作指针
	int bytes_left = 0;

	if(m_msg_tmp_buf.buf.base){//假如上一个包不完整！

		char* tmp_buf = m_msg_tmp_buf.buf.base;
		UINT32 last_msg_total_len = m_msg_tmp_buf.buf.len;
		UINT32 last_msg_received_len = m_msg_tmp_buf.used_len;

		ASSERT(last_msg_total_len > last_msg_received_len);//不可能等 因为如果相等就已经直接处理了！
		ASSERT(last_msg_total_len >= PACKET_HEAD_LEN);

		UINT32 last_msg_id = *(UINT32*)(tmp_buf + sizeof UINT32);
		ASSERT( last_msg_id < e_msg_gs_gm::e_msg_gs_gm_max && last_msg_id > e_msg_gs_gm::e_msg_gs_gm_min);

		if( (last_msg_received_len + len) >= last_msg_total_len ){//假如包可以完整了
			
			memcpy(tmp_buf + last_msg_received_len, buf, last_msg_total_len - last_msg_received_len);
			
			func_message_handle pMsgHandle = m_mapHandle[last_msg_id];
			if (pMsgHandle){

				//CClientForGS* client_gs = get_client((uv_handle_t*)stream);
				//ASSERT(client_gs);
				//UINT32 user_id = client_gs->m_user_info ? client_gs->m_user_info->base_info().user_id() : 0;
				//CLogCommond* commond = new CLogCommond();
				//commond->Init(CLogCommond::ESERVERTYPE_GS, CLogCommond::ELOGTYPE_RECEIVE,last_msg_id, user_id, Now());
				//CAsyncCommandModule::GetInstance()->AddCommand(commond);

				bool r = pMsgHandle(stream, tmp_buf + PACKET_HEAD_LEN, last_msg_total_len - PACKET_HEAD_LEN);
			}
			else{
				printf("CClientForGS::process_msg没有找到消息msgid[%d][%s]的处理函数\n", last_msg_id, e_msg_gs_gm_Name((e_msg_gs_gm)last_msg_id).c_str());
				//return false;
				ASSERT(false);
			}

			//释放空间
			buf_free(&(m_msg_tmp_buf.buf));
			m_msg_tmp_buf.used_len = 0;

			//设置工作指针到未处理区域
			p = const_cast<char*>(buf + (last_msg_total_len - last_msg_received_len ));//工作指针后移到
			bytes_left = len - (last_msg_total_len - last_msg_received_len );
		}
		else{//还是不足一个包的时候
			memcpy(tmp_buf + last_msg_received_len, buf, len);

			m_msg_tmp_buf.used_len = last_msg_received_len + len;

			printf("CClientForGS::process_msg 消息msgid[%d][%s]包不完整 原长[%u]，已接收[%u] 继续拼包\n", last_msg_id, e_msg_gs_gm_Name((e_msg_gs_gm)last_msg_id).c_str(), m_msg_tmp_buf.buf.len, m_msg_tmp_buf.used_len);
			return true;
		}
	}
	else{//上一个包是完整处理的
		p = const_cast<char*>(buf);
		bytes_left = len;
	}
	
	
	while (bytes_left != 0)
	{
		UINT32 msg_len = *(UINT32 *)(p);
		

		//ASSERT(bytes_left >= msg_len);
		if(bytes_left >= msg_len){
			bytes_left -= msg_len;
	
			p += sizeof msg_len;

			UINT32 msg_id = *(UINT32 *)(p);
			ASSERT(msg_id < e_msg_gs_gm::e_msg_gs_gm_max && msg_id > e_msg_gs_gm::e_msg_gs_gm_min);
			p += sizeof msg_id;

			func_message_handle pMsgHandle = m_mapHandle[msg_id];
			if (pMsgHandle){
				//CClientForGS* client_gs = get_client((uv_handle_t*)stream);
				//ASSERT(client_gs);
				//UINT32 user_id = client_gs->m_user_info ? client_gs->m_user_info->base_info().user_id() : 0;
				//CLogCommond* commond = new CLogCommond();
				//commond->Init(CLogCommond::ESERVERTYPE_GS, CLogCommond::ELOGTYPE_RECEIVE, msg_id, user_id, Now());
				//CAsyncCommandModule::GetInstance()->AddCommand(commond);
				bool r = pMsgHandle(stream, p, msg_len - sizeof(msg_len) - sizeof(msg_id));
			}
			else{
				printf("CClientForGS::process_msg没有找到消息msgid[%d][%s]的处理函数\n", msg_id, e_msg_gs_gm_Name((e_msg_gs_gm)msg_id).c_str());
				//return false;
				ASSERT(false);
			}

			p += msg_len - sizeof(msg_len) - sizeof(msg_id);
		}
		else{
			ASSERT(msg_len <= 16777216);//不超过16M！
			ASSERT(m_msg_tmp_buf.buf.base == NULL && m_msg_tmp_buf.buf.len == 0 && m_msg_tmp_buf.used_len == 0);
			buf_alloc(NULL, msg_len, &(m_msg_tmp_buf.buf));

			memcpy(m_msg_tmp_buf.buf.base, p, bytes_left);
			m_msg_tmp_buf.used_len = bytes_left;
			printf("CClientForGS::process_msg 消息包不完整 原长[%u]，已接收[%u] 后面的需要拼包...\n", msg_len, bytes_left);
			break;//跳出处理！
		}
	}

	return true;
}

bool CClientForGS::Send( UINT32 msg_id, google::protobuf::Message* pMessage )
{
	return send((uv_stream_t*)(&m_client), msg_id, pMessage);
}


bool CClientForGS::SendAsync( UINT32 msg_id, google::protobuf::Message* pMessage )
{
	//这个地方发给db   这里不等db处理完毕 直接回复客户端  db
	CClientForGSASync* asynData = new CClientForGSASync();
	asynData->msg_id = msg_id;
	//asynData->user_connect_id = ctx->ConnectID;//这里不需要设置了 在获取userInfo之后 gs已经把userid和链接 绑定在一起了 不需要直接用链接id 反而容易出错！
	asynData->msg = pMessage;

	uv_async_t* async = CClientForGS::GetInstance()->CreateAsync();
	async->data = asynData;

	int r = uv_async_send(async);
	ASSERT(r == 0);
	if (r != 0)
	{
		PRINTF_UV_ERROR(r);
		//delete db_req;
		delete asynData;
		async->data = NULL;
		CClientForGS::GetInstance()->DestroyAsync(async);
		return false;
	}

	return true;
}

void CClientForGS::async_cb( uv_async_t* handle )
{
	ASSERT(handle);
	CClientForGSASync* async = (CClientForGSASync*)handle->data;
	//ASSERT(async->op);
	//switch (async->op)
	//{
	//	case (int)CClientForGSASync::EASyncOP_Enter_GS_Req:{

	//		as_c_enter_area_res* res = (as_c_enter_area_res*)async->data;

	//		ASSERT(res);
	//		res->mutable_token();
	//		res->ip();
	//		res->port();



	//	}break;
	//}

	ASSERT(e_msg_gs_gm_IsValid(async->msg_id) && async->msg);

	CClientForGS* client_gs = get_client((uv_handle_t*)handle);
	ASSERT(client_gs);
	client_gs->Send(async->msg_id, async->msg);
	delete async->msg;//删除消息结构体


	delete async;
	uv_close((uv_handle_t*)handle, after_async_close);
}

bool CClientForGS::handle_msg_marquee_res( uv_stream_t* stream, const char * buf, int len )
{
	gs_gm_marquee_res res;
	bool r = res.ParseFromArray(buf, len);
	if (r){

		printf("跑马灯结果[%d][%s] \n", res.error_code(), e_gsgm_errorcode_Name((e_gsgm_errorcode)res.error_code()).c_str());
		res.PrintDebugString();
		//gm_gs_equip_evolution_req req;
		//req.set_hero_id(17);
		//req.set_equip_id(321);
		//send(stream, e_msg_gm_gs_equip_evolution_req, &req);

	}
	else{
		PRINTF_PBPARSE_ERROR(e_msg_gs_gm_marquee_res);
		return false;
	}
	ASSERT(r);

	return true;
}

int CClientForGS::create_timer()
{
	int r = uv_timer_init(CClientForGS::GetInstance()->m_loop, &CClientForGS::GetInstance()->m_timer);
	ASSERT(r == 0);
	r = uv_timer_start(&CClientForGS::GetInstance()->m_timer, timer_cb, 1000, 1000);
	ASSERT(r == 0);
	return 0;
}

void CClientForGS::timer_cb( uv_timer_t* handle)
{
	ASSERT(handle);
	ASSERT(1 == uv_is_active((uv_handle_t*)handle));

	ASSERT(handle == &CClientForGS::GetInstance()->m_timer);
	//邮件逻辑移到了GS上
	//time_t tArenaTime = GetTime(9, 0, 0);
	//time_t tNow = Now();

	//if(tArenaTime == tNow)//时间到达上午9点，发送竞技场结算邮件
	//{
	//	gm_gs_arena_mail_req req;
	//	CClientForGS::GetInstance()->Send(e_msg_gs_gm_arena_mail_res, &req);
	//}
}	

bool CClientForGS::handle_add_item_res( uv_stream_t* stream, const char * buf, int len )
{
	gs_gm_add_item_res res;
	bool r = res.ParseFromArray(buf, len);
	if (r){

		printf("添加道具结果[%d][%s] \n", res.error_code(), e_gsgm_errorcode_Name((e_gsgm_errorcode)res.error_code()).c_str());
		res.PrintDebugString();
	}
	else{
		PRINTF_PBPARSE_ERROR(e_msg_gs_gm_add_item_res);
		return false;
	}
	ASSERT(r);

	return true;
}

bool CClientForGS::handle_mail_res( uv_stream_t* stream, const char * buf, int len )
{
	gs_gm_mail_res res;
	bool r = res.ParseFromArray(buf, len);
	if (r){

		printf("发送邮件结果[%d][%s] \n", res.error_code(), e_gsgm_errorcode_Name((e_gsgm_errorcode)res.error_code()).c_str());
		res.PrintDebugString();
	}
	else{
		PRINTF_PBPARSE_ERROR(e_msg_gs_gm_mail_res);
		return false;
	}
	ASSERT(r);

	return true;
}

bool CClientForGS::handle_arena_mail_res( uv_stream_t* stream, const char * buf, int len )
{
	gs_gm_arena_mail_res res;
	bool r = res.ParseFromArray(buf, len);
	if (r){

		printf("发送竞技场邮件[%d][%s] \n", res.error_code(), e_gsgm_errorcode_Name((e_gsgm_errorcode)res.error_code()).c_str());
		res.PrintDebugString();
	}
	else{
		PRINTF_PBPARSE_ERROR(e_msg_gs_gm_arena_mail_res);
		return false;
	}
	ASSERT(r);

	return true;
}

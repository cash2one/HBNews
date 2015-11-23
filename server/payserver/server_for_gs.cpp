#include "server_for_gs.h"
#include "def.h"
#include "net_config.h"
#include "libuv_helper.h"
#include "db_async_command.h"
#include "async_command_module.h"
#include "time_helper.h"
#include "order.h"
#include "db_mgr.h"
#include "game_config.h"

bool CServerForGS::Init()
{
	int r = uv_mutex_init(&m_lock);
	ASSERT(r == 0);

	ASSERT(add_message_handle(e_msg_gs_ps_paied_res, handle_msg_paied_res));

	MYSQL* con = CDBMgr::GetInstance()->GetConnection();
	char sql[512] = {0};
	sprintf_s(sql, sizeof sql, "SELECT * FROM `area_gameserver`");

	r = CDBMgr::Query(con, sql, strlen(sql));
	if (r != 0){
		ASSERT(false);
	}

	int i = 0;
	do{
		MYSQL_RES* res = mysql_store_result( con );
		switch (i++){
		case 0:{//commodity
				   {
					   //检查表结构是否有改变
					   std::string need_fields[] = {"id", "ip"};
					   std::vector<std::string> v_need_fields(need_fields, need_fields + sizeof(need_fields)/sizeof(need_fields[0]));
					   ASSERT(CDBMgr::check_fields(res, v_need_fields));
				   }

				   MYSQL_ROW row;
				   while (row = mysql_fetch_row(res)){
					   int col = 0;//
					   UINT32 id = atoi(row[col++]);
					   std::string ip = row[col++];

					   ASSERT(m_mapIP2Area.find(ip) == m_mapIP2Area.end());

					   CArea* pItem = new CArea;
					   pItem->area_id = id;
					   pItem->ip = ip;

					   m_mapIP2Area[ip] = pItem;
				   }
			   }
			   break;
		}
		mysql_free_result(res);
	}while( !mysql_next_result( con ) );

	ASSERT(m_mapIP2Area.size());
	return true;
}

bool CServerForGS::Start()
{
	int r = uv_thread_create(&m_thread, run, this);
	return r == 0;
}

bool CServerForGS::Join()
{
	int r = uv_thread_join(&m_thread);
	ASSERT( r == 0) ;
	return true;
}

uv_async_t* CServerForGS::CreateAsync()
{
	ASSERT(m_loop);
	uv_async_t* async = (uv_async_t*)malloc(sizeof uv_async_t);
	ASSERT(async);
	int r = uv_async_init(m_loop, async, async_cb);
	ASSERT(r == 0);
	return async;
}

void CServerForGS::DestroyAsync(uv_async_t* handle)
{
	ASSERT(handle);
	free(handle);
}

void CServerForGS::run( void* arg )
{
	struct sockaddr_in addr;
	int r;

	CServerForGS* server_for_gs = (CServerForGS*)arg;
	ASSERT(server_for_gs);

	server_for_gs->m_loop = (uv_loop_t*)malloc(sizeof uv_loop_t);
	uv_loop_init(server_for_gs->m_loop);

	const ServerForGSConfig& cfg = GameConfig::GetInstance()->GetServerForGSConfig();
	const char * ip = cfg.m_ip.c_str();
	int port = cfg.m_port;

	r = uv_ip4_addr(ip, port, &addr);
	ASSERT(r == 0);

	r = uv_tcp_init(server_for_gs->m_loop, &server_for_gs->m_server);
	ASSERT(r == 0);

	r = uv_tcp_bind(&server_for_gs->m_server, (const struct sockaddr*) &addr, 0);
	ASSERT(r == 0);

	server_for_gs->m_server.data = server_for_gs;

	r = uv_listen((uv_stream_t*)&server_for_gs->m_server, 128, on_client_connection);
	ASSERT(r == 0);

	printf("ServerForGS start success! listen %s:%d \n", ip, port);
	r = uv_run(server_for_gs->m_loop, UV_RUN_DEFAULT);
	ASSERT(0 && "server exit!");

	uv_loop_close(server_for_gs->m_loop);
	free(server_for_gs->m_loop);
}

void CServerForGS::on_client_connection( uv_stream_t* stream, int status )
{
	int r;
	struct sockaddr sockname, peername;
	int namelen;

	ASSERT(stream);
	ASSERT(status == 0);
	CServerForGS* server_for_gs = (CServerForGS*)stream->data;
	ASSERT(server_for_gs);
	ASSERT(stream == (uv_stream_t*)&(server_for_gs->m_server));

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

	//printf("new connect accepted! \n");
	printf("GS服务器连接成功...\n");
	r = uv_read_start((uv_stream_t*)handle, alloc_cb, on_receive);
	ASSERT(r == 0);
	printf("GS服务器连接读启动成功...\n");

	bool ret = CServerForGS::GetInstance()->add_client(handle);
	if (!ret){
		uv_shutdown_t* req = (uv_shutdown_t*)req_alloc();
		r = uv_shutdown(req, stream, after_shutdown);
		ASSERT(r == 0);
	}
}

void CServerForGS::on_receive( uv_stream_t* stream, ssize_t nread, const uv_buf_t* buf )
{
	int r;
	//todo net_protect作一些限制处理
	if (nread > 0){
		CServerForGS::GetInstance()->process_msg(stream, buf->base, nread);
		//CServerForGS* client_gs = get_client((uv_handle_t*)stream);
		//ASSERT(client_gs);
		//client_gs->process_msg(stream, buf->base, nread);
		free(buf->base);//释放内存
	}
	else if (nread == 0){//OK, but nothing read
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

void CServerForGS::after_shutdown( uv_shutdown_t* req, int status )
{
	ASSERT(req);
	ASSERT(req->handle);
	uv_close((uv_handle_t*)req->handle, after_close);
	req_free((uv_req_t*)req);
}

void CServerForGS::after_close( uv_handle_t* handle )
{
	uv_tcp_t* client = (uv_tcp_t*)handle;
	//ASSERT(client == &(CServerForGS::GetInstance()->m_client));


	//CServerForGS* client_gs = get_client(handle);
	//ASSERT(client_gs);
	//ASSERT( client == &(client_gs->m_client) );

	bool r = CServerForGS::GetInstance()->remove_client(client);
	//ASSERT(r);//可能是非允许的ip进来的 然后关闭的 所以有可能是么有的！
	delete client;
}

bool CServerForGS::send( uv_stream_t* stream, UINT32 msg_id, google::protobuf::Message* pMessage/*, uv_write_cb write_cb*/ )
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
		printf("CClientForGS::send msgid[%d][%s]  bytes len[%d] \n", msg_id, e_msg_ps_gs_Name((e_msg_ps_gs)msg_id).c_str(), PACKET_HEAD_LEN + body_len);
		return true;
	}
}

void CServerForGS::after_send( uv_write_t* req, int status )
{
	ASSERT(req != NULL);
	if (status == 0){
		//write_cb_called++;
		printf("CServerForGS::after_send 给GS发送信息成功\n");

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

void CServerForGS::after_async_close( uv_handle_t* handle )
{
	ASSERT(handle != NULL);
	//CServerForGS* client_gs = get_client(handle);
	//ASSERT(client_gs);
	//client_gs->DestroyAsync((uv_async_t*)handle);

	CServerForGS::GetInstance()->DestroyAsync((uv_async_t*)handle);
}

bool CServerForGS::add_message_handle( e_msg_gs_ps msgID, func_message_handle handle )
{
	if (m_mapHandle[msgID]){
		printf("already exsit handle for %u", msgID);
		return false;
	}

	m_mapHandle[msgID] = handle;
	return true;
}

bool CServerForGS::process_msg( uv_stream_t* stream, const char * buf, int len )
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
		ASSERT( last_msg_id < e_msg_gs_ps::e_msg_gs_ps_max && last_msg_id > e_msg_gs_ps::e_msg_gs_ps_min);

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
				printf("CClientForGS::process_msg没有找到消息msgid[%d][%s]的处理函数\n", last_msg_id, e_msg_gs_ps_Name((e_msg_gs_ps)last_msg_id).c_str());
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

			printf("CClientForGS::process_msg 消息msgid[%d][%s]包不完整 原长[%u]，已接收[%u] 继续拼包\n", last_msg_id, e_msg_gs_ps_Name((e_msg_gs_ps)last_msg_id).c_str(), m_msg_tmp_buf.buf.len, m_msg_tmp_buf.used_len);
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
			ASSERT(msg_id < e_msg_gs_ps::e_msg_gs_ps_max && msg_id > e_msg_gs_ps::e_msg_gs_ps_min);
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
				printf("CClientForGS::process_msg没有找到消息msgid[%d][%s]的处理函数\n", msg_id, e_msg_gs_ps_Name((e_msg_gs_ps)msg_id).c_str());
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

//bool CServerForGS::Send( UINT32 msg_id, google::protobuf::Message* pMessage )
//{
//	return send((uv_stream_t*)(&m_client), msg_id, pMessage);
//}


bool CServerForGS::SendAsync(UINT16 area_id, UINT32 msg_id, google::protobuf::Message* pMessage )
{
	//这个地方发给db   这里不等db处理完毕 直接回复客户端  db
	CServerForGSASync* asynData = new CServerForGSASync();
	asynData->area_id = area_id;
	asynData->msg_id = msg_id;
	//asynData->user_connect_id = ctx->ConnectID;//这里不需要设置了 在获取userInfo之后 gs已经把userid和链接 绑定在一起了 不需要直接用链接id 反而容易出错！
	asynData->msg = pMessage;

	//uv_async_t* async = CClientForGS::GetInstance()->CreateAsync();
	
	uv_async_t* async = CServerForGS::GetInstance()->CreateAsync();
	async->data = asynData;

	int r = uv_async_send(async);
	ASSERT(r == 0);
	if (r != 0){
		PRINTF_UV_ERROR(r);
		//delete db_req;
		delete asynData;
		async->data = NULL;
		CServerForGS::GetInstance()->DestroyAsync(async);
		return false;
	}

	return true;
}

void CServerForGS::async_cb( uv_async_t* handle )
{
	ASSERT(handle);
	CServerForGSASync* async = (CServerForGSASync*)handle->data;
	ASSERT(async && async->area_id && e_msg_gs_ps_IsValid(async->msg_id) && async->msg);

	//CServerForGS* client_gs = get_client((uv_handle_t*)handle);
	//ASSERT(client_gs);
	//client_gs->Send(async->msg_id, async->msg);

	uv_tcp_t* gs_con = CServerForGS::GetInstance()->GetCon(async->area_id);
	ASSERT(gs_con);

	send((uv_stream_t*)(gs_con), async->msg_id, async->msg);
	delete async->msg;//删除消息结构体
	delete async;
	uv_close((uv_handle_t*)handle, after_async_close);
}

//int CClientForGS::create_timer()
//{
//	int r = uv_timer_init(CClientForGS::GetInstance()->m_loop, &CClientForGS::GetInstance()->m_timer);
//	ASSERT(r == 0);
//	r = uv_timer_start(&CClientForGS::GetInstance()->m_timer, timer_cb, 1000, 1000);
//	ASSERT(r == 0);
//	return 0;
//}

//void CClientForGS::timer_cb( uv_timer_t* handle)
//{
//	ASSERT(handle);
//	ASSERT(1 == uv_is_active((uv_handle_t*)handle));
//
//	ASSERT(handle == &CClientForGS::GetInstance()->m_timer);
//	time_t tArenaTime = GetTime(9, 0, 0);
//	time_t tNow = Now();
//
//	if(tArenaTime == tNow)//时间到达上午9点，发送竞技场结算邮件
//	{
//		//ps_gs_arena_mail_req req;
//		//CClientForGS::GetInstance()->Send(e_msg_gs_ps_arena_mail_res, &req);
//	}
//}	

bool CServerForGS::handle_msg_paied_res( uv_stream_t* stream, const char * buf, int len )
{
	gs_ps_paied_res res;
	bool r = res.ParseFromArray(buf, len);
	if (r){
		printf("GS处理支付结果[%d][%s] \n", res.error_code(), e_psgs_errorcode_Name((e_psgs_errorcode)res.error_code()).c_str());
		res.PrintDebugString();

		ASSERT(!res.order_id().empty());

		COrder::ESTATUS status;
		if (res.error_code() != e_psgs_errorcode_success){
			status = COrder::GS_PROCESS_FAIL;
		}
		else{
			status = COrder::GS_PROCESS_SUCCESS;
		}

		COrderProcessedCommand* c = new COrderProcessedCommand(res.order_id(), status);
		CAsyncCommandModule::GetInstance()->AddCommand(c);
	}
	else{
		PRINTF_PBPARSE_ERROR(e_msg_gs_ps_paied_res);
		return false;
	}
	ASSERT(r);

	return true;
}

uv_tcp_t* CServerForGS::GetCon( UINT16 iAreaID ){
	uv_tcp_t* r = NULL;
	uv_mutex_lock(&m_lock);
	std::map<UINT16, uv_tcp_t*>::iterator it = m_mapAreaID2GSCon.find(iAreaID);
	if (it != m_mapAreaID2GSCon.end()){
		r = it->second;
	}
	uv_mutex_unlock(&m_lock);

	return NULL;
}

bool CServerForGS::AddCon( UINT16 iAreaID, uv_tcp_t* con ){
	ASSERT(iAreaID && con);
	bool r = false;
	uv_mutex_lock(&m_lock);
	if (m_mapAreaID2GSCon.find(iAreaID) == m_mapAreaID2GSCon.end()){
		m_mapAreaID2GSCon[iAreaID] = con;
		r = true;
		printf("area【%u】【%p】链接成功！", iAreaID, con);
	}
	else{
		printf("area【%u】【%p】链接失败 已经有这个区的链接！", iAreaID, con);
	}
	uv_mutex_unlock(&m_lock);
	
	return r;
}

bool CServerForGS::RemoveCon( UINT16 iAreaID ){
	ASSERT(iAreaID);
	bool r = false;
	uv_mutex_lock(&m_lock);
	if (m_mapAreaID2GSCon.find(iAreaID) != m_mapAreaID2GSCon.end()){
		m_mapAreaID2GSCon.erase(iAreaID);
		r = true;
	}
	uv_mutex_unlock(&m_lock);
	printf("area【%u】断开链接！", iAreaID);
	return r;
}

bool CServerForGS::add_client( uv_tcp_t* client )
{
	ASSERT(client);

	struct sockaddr peername;
	//有连接，可以获得目标的ip和端口  
	int namelen = sizeof peername;  
	int r = uv_tcp_getpeername(client, &peername, &namelen);  
	//printf("the r is %d.\n", r);
	ASSERT( r == 0);

	char ip[17] = {0};
	memset(ip, 0, sizeof ip);
	struct sockaddr_in sock_addr = *(struct sockaddr_in*) &peername; 
	//网络字节序转换成主机字符序  
	uv_ip4_name(&sock_addr, (char*)ip, sizeof ip);  
	printf("getpeername  %s : %d \n", ip, sock_addr.sin_port);

	std::map<std::string, CArea*>::iterator it = m_mapIP2Area.find(ip); 
	if ( it == m_mapIP2Area.end()){
		printf("CServerForGS::add_client 链接进来了非配置GS的链接！ip:port  [%s:%d] 后面需要做功能 直接拒绝他们的链接！\n", ip, sock_addr.sin_port);
		return false;//todo 断开链接！
	}

	CArea* pArea = it->second;
	ASSERT(pArea);

	/*
	Guid connect_id = GetGuidGenerator().newGuid();
	ASSERT(m_clients.find(connect_id) == m_clients.end());
	m_clients[connect_id] = client;

	pArea->client_id = connect_id;

	CGSContext* ctx = new CGSContext;
	ctx->client_id = connect_id;
	ctx->area_id = pArea->area_id;
	ctx->stauts = area::EAreaStatus::area_EAreaStatus_EAreaStatus_None;
	client->data = ctx;
	*/

	bool ret = AddCon(pArea->area_id, client);
	ASSERT(ret);

	printf("CServerForGS::add_client 链接进来GS的链接！ip:port  [%s:%d]  areaid[%d] \n", ip, sock_addr.sin_port, pArea->area_id);
	return true;
}
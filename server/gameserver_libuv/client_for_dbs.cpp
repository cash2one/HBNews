#include "client_for_dbs.h"
#include "game_config.h"
#include "libuv_helper.h"
#include "def.h"
#include "net_config.h"

#include "server_for_user.h"

bool CClientForDBS::Init()
{
	ASSERT(add_message_handle(e_msg_dbs_gs_user_info_res, handle_msg_user_info_res));
	ASSERT(add_message_handle(e_msg_dbs_gs_user_info_op_res, handle_msg_user_info_op_res));
	ASSERT(add_message_handle(e_msg_dbs_gs_user_new_mail_res, handle_msg_user_new_mail_res));
	ASSERT(add_message_handle(e_msg_dbs_gs_edit_name_res, handle_msg_edit_name_res));
	ASSERT(add_message_handle(e_msg_dbs_gs_user_new_arena_mail_res, handle_msg_user_new_arena_mail_res));
	ASSERT(add_message_handle(e_msg_dbs_gs_friend_offline_res, handle_msg_friend_offline_res));
	ASSERT(add_message_handle(e_msg_dbs_gs_search_user_res, handle_msg_search_user_res));
	ASSERT(add_message_handle(e_msg_dbs_gs_rank_res, handle_msg_rank_res));
	return true;
}

bool CClientForDBS::Start()
{
	int r = uv_thread_create(&m_thread, run, this);
	return r == 0;
}

bool CClientForDBS::SendAsync(CClientForDBSAsync::EASyncOP eOP, UINT32 user_id, void* data){
	//这个地方发给db   这里不等db处理完毕 直接回复客户端  db
	CClientForDBSAsync* asynData = new CClientForDBSAsync();
	asynData->op = eOP;
	asynData->user_id = user_id;
	//asynData->user_connect_id = ctx->ConnectID;//这里不需要设置了 在获取userInfo之后 gs已经把userid和链接 绑定在一起了 不需要直接用链接id 反而容易出错！
	asynData->data = data;

	uv_async_t* async = CClientForDBS::GetInstance()->CreateAsync();
	async->data = asynData;

	int r = uv_async_send(async);
	ASSERT(r == 0);
	if (r != 0)
	{
		PRINTF_UV_ERROR(r);
		//delete db_req;
		delete asynData;
		async->data = NULL;
		CClientForDBS::GetInstance()->DestroyAsync(async);
		return false;
	}

	return true;
}

uv_async_t* CClientForDBS::CreateAsync()
{
	ASSERT(m_loop);
	uv_async_t* async = (uv_async_t*)malloc(sizeof uv_async_t);
	ASSERT(async);
	int r = uv_async_init(m_loop, async, async_cb);
	ASSERT(r == 0);
	return async;
}

void CClientForDBS::DestroyAsync(uv_async_t* handle)
{
	ASSERT(handle);
	free(handle);
}

void CClientForDBS::async_cb( uv_async_t* handle )
{
	ASSERT(handle);
	CClientForDBSAsync* asyncData = (CClientForDBSAsync*)handle->data;
	ASSERT(asyncData && asyncData->op > CClientForDBSAsync::EASyncOP_NONE);
	switch (asyncData->op)
	{
		case CClientForDBSAsync::EASyncOP_User_Info_Req : {
			gs_dbs_user_info_req req;
			req.set_user_id(asyncData->user_id);

			BYTE client_id_buf[GUID_BYTE_LEN] = {0};
			asyncData->user_connect_id.ToBytes(client_id_buf, GUID_BYTE_LEN);
			req.set_user_connect_id(client_id_buf, GUID_BYTE_LEN);

			send((uv_stream_t*)&(CClientForDBS::GetInstance()->m_client), e_msg_gs_dbs_user_info_req, &req);

		 }break;

		case CClientForDBSAsync::EASyncOP_User_Info_OP_Req : {
			gs_dbs_user_info_op_req* req = (gs_dbs_user_info_op_req*)asyncData->data;
			send((uv_stream_t*)&(CClientForDBS::GetInstance()->m_client), e_msg_gs_dbs_user_info_op_req, req);
			delete req;
		}break;

		case CClientForDBSAsync::EASyncOP_User_Info_New_Create_Req:{

			//信息不要删除 因为已经保存在CUser里了
			db_user_info* new_user_info = (db_user_info*)asyncData->data;

			gs_dbs_user_info_new_create_req req;
			req.set_allocated_user_info(new_user_info);
			send((uv_stream_t*)&(CClientForDBS::GetInstance()->m_client), e_msg_gs_dbs_user_info_new_create_req, &req);
			req.release_user_info();//只赋空 不释放因为已经保存在CUser里了
			
		}break;

		case CClientForDBSAsync::EASyncOP_User_Edit_User_name:{
			gs_dbs_edit_name_req* req = (gs_dbs_edit_name_req*)asyncData->data;
			send((uv_stream_t*)&(CClientForDBS::GetInstance()->m_client), e_msg_gs_dbs_edit_name_req, req);
			delete req;
		}break;
		case CClientForDBSAsync::EASyncOP_User_New_Mail:{
			gs_dbs_user_new_mail_req* req = (gs_dbs_user_new_mail_req*)asyncData->data;
			send((uv_stream_t*)&(CClientForDBS::GetInstance()->m_client), e_msg_gs_dbs_user_new_mail_req, req);
			delete req;
		}break;
		case CClientForDBSAsync::EASyncOP_User_New_Arena_mail:{
			gs_dbs_user_new_arena_mail_req* req = (gs_dbs_user_new_arena_mail_req*)asyncData->data;
			send((uv_stream_t*)&(CClientForDBS::GetInstance()->m_client), e_msg_gs_dbs_user_new_arena_mail_req, req);
			delete req;
		}break;
		case CClientForDBSAsync::EASyncOP_User_Search_User:{
			gs_dbs_search_user_req* req = (gs_dbs_search_user_req*)asyncData->data;
			send((uv_stream_t*)&(CClientForDBS::GetInstance()->m_client), e_msg_gs_dbs_search_user_req, req);
			delete req;
		}break;
		case CClientForDBSAsync::EASyncOP_User_Friend_OP:{
			gs_dbs_user_friend_op_req* req = (gs_dbs_user_friend_op_req*)asyncData->data;
			send((uv_stream_t*)&(CClientForDBS::GetInstance()->m_client), e_msg_gs_dbs_user_friend_op_req, req);
			delete req;
		}break;
		case CClientForDBSAsync::EASyncOP_User_Friend_Offline:{
			gs_dbs_friend_offline_req* req = (gs_dbs_friend_offline_req*)asyncData->data;
			send((uv_stream_t*)&(CClientForDBS::GetInstance()->m_client), e_msg_gs_dbs_friend_offline_req, req);
			delete req;
		}break;
		case CClientForDBSAsync::EASyncOP_User_Rank:{
			gs_dbs_rank_req* req = (gs_dbs_rank_req*)asyncData->data;
			send((uv_stream_t*)&(CClientForDBS::GetInstance()->m_client), e_msg_gs_dbs_rank_req, req);
			delete req;
		}break;
	}

	delete asyncData;
	uv_close((uv_handle_t*)handle, after_async_close);
}

void CClientForDBS::after_async_close( uv_handle_t* handle )
{
	ASSERT(handle != NULL);
	//free((uv_async_t*)handle);
	CClientForDBS::GetInstance()->DestroyAsync((uv_async_t*)handle);
}

bool CClientForDBS::add_message_handle( e_msg_dbs_gs msgID, func_message_handle handle )
{
	if (m_mapHandle[msgID])
	{
		printf("already exsit handle for %u", msgID);
		return false;
	}

	m_mapHandle[msgID] = handle;
	return true;
}


bool CClientForDBS::process_msg( uv_stream_t* stream, const char * buf, int len )
{
	ASSERT(buf);
	ASSERT(len >= PACKET_HEAD_LEN);//
	//int pos = 0;
	//int bytes_left = len;

	//char* p = const_cast<char*>(buf);
	//while (bytes_left != 0)
	//{
	//	UINT32 msg_len = *(UINT32 *)(p);
	//	p += sizeof msg_len;

	//	if (bytes_left < msg_len)
	//	{
	//		printf("bytes_left[%d] < msg_len[%u]", bytes_left, msg_len);
	//	}
	//	ASSERT(bytes_left >= msg_len);
	//	bytes_left -= msg_len;

	//	UINT32 msg_id = *(UINT32 *)(p);
	//	ASSERT(msg_id < e_msg_dbs_gs::e_msg_dbs_gs_max);
	//	p += sizeof msg_id;

	//	func_message_handle pMsgHandle = m_mapHandle[msg_id];
	//	if (pMsgHandle)
	//	{
	//		bool r = pMsgHandle(stream, p, msg_len - sizeof(msg_len) - sizeof(msg_id));
	//	}
	//	else{
	//		printf("CClientForDBS::process_msg没有找到消息msgid[%d][%s]的处理函数\n", msg_id, e_msg_dbs_gs_Name((e_msg_dbs_gs)msg_id).c_str());
	//		//return false;
	//	}

	//	p += msg_len - sizeof(msg_len) - sizeof(msg_id);
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
		ASSERT( last_msg_id < e_msg_dbs_gs::e_msg_dbs_gs_max && last_msg_id > e_msg_dbs_gs::e_msg_dbs_gs_min);

		if( (last_msg_received_len + len) >= last_msg_total_len ){//假如包可以完整了

			memcpy(tmp_buf + last_msg_received_len, buf, last_msg_total_len - last_msg_received_len);

			func_message_handle pMsgHandle = m_mapHandle[last_msg_id];
			if (pMsgHandle)
			{
				bool r = pMsgHandle(stream, tmp_buf + PACKET_HEAD_LEN, last_msg_total_len - PACKET_HEAD_LEN);
			}
			else{
				printf("CClientForDBS::process_msg没有找到消息msgid[%d][%s]的处理函数\n", last_msg_id, e_msg_dbs_gs_Name((e_msg_dbs_gs)last_msg_id).c_str());
				//return false;
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

			printf("CClientForDBS::process_msg 消息msgid[%d][%s]包不完整 原长[%u]，已接收[%u] 继续拼包\n", last_msg_id, e_msg_dbs_gs_Name((e_msg_dbs_gs)last_msg_id).c_str(), m_msg_tmp_buf.buf.len, m_msg_tmp_buf.used_len);
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
			ASSERT(msg_id < e_msg_dbs_gs::e_msg_dbs_gs_max && msg_id > e_msg_dbs_gs::e_msg_dbs_gs_min);
			p += sizeof msg_id;

			func_message_handle pMsgHandle = m_mapHandle[msg_id];
			if (pMsgHandle)
			{
				bool r = pMsgHandle(stream, p, msg_len - sizeof(msg_len) - sizeof(msg_id));
			}
			else{
				printf("CClientForDBS::process_msg没有找到消息msgid[%d][%s]的处理函数\n", msg_id, e_msg_dbs_gs_Name((e_msg_dbs_gs)msg_id).c_str());
				//return false;
			}

			p += msg_len - sizeof(msg_len) - sizeof(msg_id);
		}
		else{
			ASSERT(msg_len <= 16777216);//不超过16M！
			ASSERT(m_msg_tmp_buf.buf.base == NULL && m_msg_tmp_buf.buf.len == 0 && m_msg_tmp_buf.used_len == 0);
			buf_alloc(NULL, msg_len, &(m_msg_tmp_buf.buf));

			memcpy(m_msg_tmp_buf.buf.base, p, bytes_left);
			m_msg_tmp_buf.used_len = bytes_left;
			printf("CClientForDBS::process_msg 消息包不完整 原长[%u]，已接收[%u] 后面的需要拼包...\n", msg_len, bytes_left);
			break;//跳出处理！
		}
	}

	return true;
}


void CClientForDBS::run(void* arg)
{
	struct sockaddr_in addr;
	int r;

	CClientForDBS* client_for_dbs = (CClientForDBS*)arg;
	ASSERT(client_for_dbs);

	client_for_dbs->m_loop = (uv_loop_t*)malloc(sizeof uv_loop_t);
	uv_loop_init(client_for_dbs->m_loop);

	const ClientForDBSConfig& cfg = GameConfig::GetInstance()->GetClientForDBSConfig();
	const char* ip = cfg.m_ip.c_str();
	int port = cfg.m_port;

	ASSERT(0 == uv_ip4_addr(ip, port, &addr));

	r = uv_tcp_init(client_for_dbs->m_loop, &client_for_dbs->m_client);
	ASSERT(r == 0);

	uv_connect_t* connect = (uv_connect_t*)req_alloc();
	r = uv_tcp_connect(connect, &client_for_dbs->m_client, (const struct sockaddr*)(&addr), on_server_connection);
	ASSERT(r == 0);

	r = uv_run(client_for_dbs->m_loop, UV_RUN_DEFAULT);
	ASSERT(r == 0);
}

bool CClientForDBS::Join()
{
	int r = uv_thread_join(&m_thread);
	ASSERT(r == 0);
	return true;
}

bool CClientForDBS::IsDBReqInitialized(gs_dbs_user_info_op_req* db_req)
{
	return db_req->has_user_id() && 
			(db_req->has_base_info() 
			|| db_req->has_sub_task() 
			|| db_req->new_heros_size() 
			|| db_req->heros_base_size() 
			|| db_req->hero_equips_size() 
			|| db_req->hero_skills_size() 
			|| db_req->items_size() 
			|| db_req->op_mails_size()
			|| db_req->shops_size()
			|| db_req->buy_items_size()
			|| db_req->missions_size()
			|| db_req->has_new_arena()
			|| db_req->has_op_arena()
			|| db_req->has_clear_arena_team()
			|| db_req->has_arena_challenge()
			|| db_req->has_sign()
			|| db_req->has_new_sign()
			|| db_req->has_novice_guide()
			|| db_req->has_new_mail()
			|| db_req->op_friends_size()
			|| db_req->has_op_crusade()
			);
}

void CClientForDBS::on_server_connection(uv_connect_t* req, int status)
{
	ASSERT(req);
	uv_stream_t* con = req->handle;
	req_free((uv_req_t*)req);
	if (status != 0)
	{
		printf("%s:%s", uv_err_name(status), uv_strerror(status));
	}
	ASSERT(status == 0);

	printf("DBS服务器连接成功...\n");
	int r = uv_read_start(con, alloc_cb, on_receive);
	if (r) {
		FATAL("uv_read_start failed");
	}

	printf("DBS服务器连接读启动成功...\n");
	
	////test
	//gs_dbs_user_info_req userinfo_req;
	//userinfo_req.set_id(111);
	//
	//send(req->handle, e_msg_gs_dbs_user_info_req, &userinfo_req);

}

void CClientForDBS::on_receive(uv_stream_t* stream, ssize_t nread, const uv_buf_t* buf)
{
	int r;
	if (nread > 0)
	{
		CClientForDBS::GetInstance()->process_msg(stream, buf->base, nread);
		free(buf->base);//释放内存
	}
	else if(nread == 0)
	{

	}
	else
	{
		if (nread == UV_EOF) {
			printf("DBS主动断开\n");

		} else if (nread == UV_ECONNRESET) {
			printf("DBS异常断开\n");

		} else {
			printf("%s : %s \n", uv_err_name(nread), uv_strerror(nread));
		}
		uv_shutdown_t* req = (uv_shutdown_t*)req_alloc();
		r = uv_shutdown(req, stream, after_shutdown);
		ASSERT(r == 0);
	}
}

void CClientForDBS::after_shutdown( uv_shutdown_t* req, int status )
{
	ASSERT(req);
	ASSERT(req->handle);
	uv_close((uv_handle_t*)req->handle, after_close);
	req_free((uv_req_t*)req);
}

void CClientForDBS::after_close( uv_handle_t* handle )
{
	uv_tcp_t* client = (uv_tcp_t*)handle;
	ASSERT(client == &(CClientForDBS::GetInstance()->m_client));
}

bool CClientForDBS::send( uv_stream_t* stream, UINT32 msg_id, google::protobuf::Message* pMessage/*, uv_write_cb write_cb*/ )
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
		printf("CClientForDBS::send msgid[%d][%s]  bytes len[%d] \n", msg_id, e_msg_gs_dbs_Name((e_msg_gs_dbs)msg_id).c_str(),  PACKET_HEAD_LEN + body_len);
		//pMessage->PrintDebugString();
		return true;
	}
}

void CClientForDBS::after_send( uv_write_t* req, int status )
{
	ASSERT(req != NULL);
	if (status == 0)
	{
		//write_cb_called++;
		printf("CClientForDBS::after_send 给DBS发送信息成功\n");
	}
	else{
		if (status == UV_EOF) {
			printf("写时, DBS主动断开\n");

		} else if (status == UV_ECONNRESET) {
			printf("写时, DBS异常断开\n");

		} else {
			printf("%s : %s \n", uv_err_name(status), uv_strerror(status));
		}

		uv_shutdown_t* shut_r = (uv_shutdown_t*)req_alloc();
		int r = uv_shutdown(shut_r, req->handle, after_shutdown);
		ASSERT(r == 0);
	}

	req_free((uv_req_t*)req);
}

bool CClientForDBS::handle_msg_user_info_res(uv_stream_t* stream, const char * buf, int len)
{
	dbs_gs_user_info_res* res = new dbs_gs_user_info_res;
	bool r = res->ParseFromArray(buf, len);
	if (r)
	{
		CServerForUserAsync* asyncData = new CServerForUserAsync();
		asyncData->user_connect_id = (BYTE*)(res->user_connect_id().c_str());
		asyncData->user_id = res->user_id();
		asyncData->op = CServerForUserAsync::EAsyncOP_User_Info_Res;
		asyncData->data = res;

		uv_async_t* async = CServerForUser::GetInstance()->CreateAsync();
		async->data = asyncData;

		int r = uv_async_send(async);
		ASSERT(r == 0);
		if (r != 0)
		{
			PRINTF_UV_ERROR(r);
			delete res;
			delete asyncData;
			async->data = NULL;
			CServerForUser::GetInstance()->DestroyAsync(async);
		}

		return true;
	}
	else//解析不出来就不知道是哪个用户了 所以发送已经没有意义了 这里只能打印错误！
	{
		delete res;
		PRINTF_PBPARSE_ERROR(e_msg_dbs_gs_user_info_res);
		return false;
	}
	
}

bool CClientForDBS::handle_msg_user_new_mail_res(uv_stream_t* stream, const char * buf, int len)
{
	dbs_gs_user_new_mail_res* res = new dbs_gs_user_new_mail_res;
	bool r = res->ParseFromArray(buf, len);
	if (r)
	{
		CServerForUserAsync* asyncData = new CServerForUserAsync;
		//asyncData->user_connect_id = (BYTE*)(res->user_connect_id().c_str());
		//asyncData->user_id = res->user_id();
		asyncData->op = CServerForUserAsync::EAsyncOP_New_Mail_Res;
		asyncData->data = res;

		uv_async_t* async = CServerForUser::GetInstance()->CreateAsync();
		async->data = asyncData;

		int r = uv_async_send(async);
		ASSERT(r == 0);
		if (r != 0)
		{
			PRINTF_UV_ERROR(r);
			delete res;
			delete asyncData;
			async->data = NULL;
			CServerForUser::GetInstance()->DestroyAsync(async);
		}
		return true;
	}
	else{
		delete res;
		PRINTF_PBPARSE_ERROR(e_msg_dbs_gs_user_new_mail_res);
		return false;
	}
}

bool CClientForDBS::handle_msg_user_info_op_res( uv_stream_t* stream, const char * buf, int len )
{
	dbs_gs_user_info_op_res res;
	bool r = res.ParseFromArray(buf, len);
	if (r)
	{
		ASSERT(res.error_code() == e_dbsgs_errorcode_success);
		return true;
	}
	else//解析不出来就不知道是哪个用户了 所以发送已经没有意义了 这里只能打印错误！
	{
		PRINTF_PBPARSE_ERROR(e_msg_dbs_gs_user_info_op_res);
		return false;
	}
}

bool CClientForDBS::handle_msg_edit_name_res(uv_stream_t* stream, const char * buf, int len)
{
	dbs_gs_edit_name_res* res = new dbs_gs_edit_name_res;
	bool r = res->ParseFromArray(buf, len);
	if (r)
	{
		CServerForUserAsync* asyncData = new CServerForUserAsync;
		asyncData->user_id = res->user_id();
		asyncData->op = CServerForUserAsync::EAsyncOP_User_Edit_User_Name_Res;
		asyncData->data = res;

		uv_async_t* async = CServerForUser::GetInstance()->CreateAsync();
		async->data = asyncData;

		int r = uv_async_send(async);
		ASSERT(r == 0);
		if (r != 0)
		{
			PRINTF_UV_ERROR(r);
			delete res;
			delete asyncData;
			asyncData->data = NULL;
			CServerForUser::GetInstance()->DestroyAsync(async);
		}
		return true;
	}
	else{
		PRINTF_PBPARSE_ERROR(e_msg_dbs_gs_edit_name_res);
		delete res;
		return false;
	}
}

bool CClientForDBS::handle_msg_user_new_arena_mail_res( uv_stream_t* stream, const char * buf, int len )
{
	dbs_gs_user_new_arena_mail_res* res = new dbs_gs_user_new_arena_mail_res;
	bool r = res->ParseFromArray(buf, len);
	if (r)
	{
		CServerForUserAsync* asyncData = new CServerForUserAsync;
		asyncData->op = CServerForUserAsync::EAsyncOP_New_Arena_New_Mail_Res;
		asyncData->data = res;

		uv_async_t* async = CServerForUser::GetInstance()->CreateAsync();
		async->data = asyncData;

		int r = uv_async_send(async);
		ASSERT(r == 0);
		if (r != 0)
		{
			PRINTF_UV_ERROR(r);
			delete res;
			delete asyncData;
			asyncData->data = NULL;
			CServerForUser::GetInstance()->DestroyAsync(async);
		}
		return true;
	}
	else{
		PRINTF_PBPARSE_ERROR(e_msg_dbs_gs_user_new_arena_mail_res);
		delete res;
		return false;
	}
}

bool CClientForDBS::handle_msg_friend_offline_res( uv_stream_t* stream, const char * buf, int len )
{
	dbs_gs_friend_offline_res* res = new dbs_gs_friend_offline_res;
	bool r = res->ParseFromArray(buf, len);
	if (r)
	{
		CServerForUserAsync* asyncData = new CServerForUserAsync;
		asyncData->op = CServerForUserAsync::EAsyncOP_Add_Friend_Res;
		asyncData->data = res;

		uv_async_t* async = CServerForUser::GetInstance()->CreateAsync();
		async->data = asyncData;

		int r = uv_async_send(async);
		ASSERT(r == 0);
		if (r != 0)
		{
			PRINTF_UV_ERROR(r);
			delete res;
			delete asyncData;
			asyncData->data = NULL;
			CServerForUser::GetInstance()->DestroyAsync(async);
		}
		return true;
	}
	else{
		PRINTF_PBPARSE_ERROR(e_msg_dbs_gs_friend_offline_res);
		delete res;
		return false;
	}
}

bool CClientForDBS::handle_msg_search_user_res( uv_stream_t* stream, const char * buf, int len )
{
	dbs_gs_search_user_res* res = new dbs_gs_search_user_res;
	bool r = res->ParseFromArray(buf, len);
	if (r)
	{
		CServerForUserAsync* asyncData = new CServerForUserAsync;
		asyncData->user_id = res->user_id();
		asyncData->op = CServerForUserAsync::EAsyncOP_Search_User_Res;
		asyncData->data = res;

		uv_async_t* async = CServerForUser::GetInstance()->CreateAsync();
		async->data = asyncData;

		int r = uv_async_send(async);
		ASSERT(r == 0);
		if (r != 0)
		{
			PRINTF_UV_ERROR(r);
			delete res;
			delete asyncData;
			asyncData->data = NULL;
			CServerForUser::GetInstance()->DestroyAsync(async);
		}
		return true;
	}
	else{
		PRINTF_PBPARSE_ERROR(e_msg_dbs_gs_search_user_res);
		delete res;
		return false;
	}
}

bool CClientForDBS::handle_msg_rank_res( uv_stream_t* stream, const char * buf, int len )
{
	dbs_gs_rank_res* res = new dbs_gs_rank_res;
	bool r = res->ParseFromArray(buf, len);
	if (r)
	{
		CServerForUserAsync* asyncData = new CServerForUserAsync;
		asyncData->op = CServerForUserAsync::EAsyncOP_Rank_Res;
		asyncData->data = res;

		uv_async_t* async = CServerForUser::GetInstance()->CreateAsync();
		async->data = asyncData;

		int r = uv_async_send(async);
		ASSERT(r == 0);
		if (r != 0)
		{
			PRINTF_UV_ERROR(r);
			delete res;
			delete asyncData;
			asyncData->data = NULL;
			CServerForUser::GetInstance()->DestroyAsync(async);
		}
		return true;
	}
	else{
		PRINTF_PBPARSE_ERROR(e_msg_dbs_gs_rank_res);
		delete res;
		return false;
	}
}
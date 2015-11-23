#include "client_for_gs.h"
#include "def.h"
#include "net_config.h"
#include "libuv_helper.h"
#include "db_async_command.h"
#include "time_helper.h"
#include "str.h"

UINT32 CClientForGS::m_connect_client_num = 0;

static bool g_log = false;

bool CClientForGS::Init()
{
	ASSERT(add_message_handle(e_msg_gs_c_enter_gs_res, handle_msg_enter_gs_res));
	ASSERT(add_message_handle(e_msg_gs_c_config_res, handle_msg_config_res));
	ASSERT(add_message_handle(e_msg_gs_c_user_info_res, handle_msg_user_info_res));
	ASSERT(add_message_handle(e_msg_gs_c_start_sub_task_res, handle_msg_start_sub_task_res));
	ASSERT(add_message_handle(e_msg_gs_c_end_sub_task_res, handle_msg_end_sub_task_res));
	ASSERT(add_message_handle(e_msg_gs_c_invoke_hero_res, handle_msg_invoke_hero_res));
	ASSERT(add_message_handle(e_msg_gs_c_leave_gs_res, handle_msg_leave_gs_res));
	ASSERT(add_message_handle(e_msg_gs_c_equip_level_up_res, handle_msg_equip_level_up_res));
	ASSERT(add_message_handle(e_msg_gs_c_equip_evolution_res, handle_msg_equip_evolution_res));
	ASSERT(add_message_handle(e_msg_gs_c_hero_evolution_res, handle_msg_hero_evolution_res));
	ASSERT(add_message_handle(e_msg_gs_c_hero_star_up_res, handle_msg_hero_star_up_res));

	ASSERT(add_message_handle(e_msg_gs_c_mail_ntf, handle_msg_gs_c_mail_ntf));
	ASSERT(add_message_handle(e_msg_gs_c_mail_opt_res, handle_msg_mail_opt_res));
	ASSERT(add_message_handle(e_msg_gs_c_sell_res, handle_msg_sell_res));
	ASSERT(add_message_handle(e_msg_gs_c_fragment_compound_res, handle_msg_fragment_compound_res));

	ASSERT(add_message_handle(e_msg_gs_c_skill_level_up_res, handle_msg_skill_level_up_res));
	ASSERT(add_message_handle(e_msg_gs_c_buy_res, handle_msg_buy_res));
	ASSERT(add_message_handle(e_msg_gs_c_invoke_res, handle_msg_invoke_res));
	ASSERT(add_message_handle(e_msg_gs_c_server_time_res, handle_msg_server_time_res));

	//ASSERT(add_message_handle(e_msg_gs_c_get_activity_task_res, handle_msg_get_activity_task_res));
	ASSERT(add_message_handle(e_msg_gs_c_use_item_res, handle_msg_use_item_res));

	ASSERT(add_message_handle(e_msg_gs_c_shop_info_res, handle_msg_shop_info_res));
	ASSERT(add_message_handle(e_msg_gs_c_shop_buy_item_res, handle_msg_shop_buy_item_res));
	ASSERT(add_message_handle(e_msg_gs_c_shop_refresh_res, handle_msg_shop_refresh_res));
	ASSERT(add_message_handle(e_msg_gs_c_get_mission_reward_res, handle_msg_mission_get_reward_res));
	ASSERT(add_message_handle(e_msg_gs_c_mission_ntf, handle_msg_mission_ntf));
	ASSERT(add_message_handle(e_msg_gs_c_arena_refresh_match_res, handle_msg_arena_refresh_match_res));
	ASSERT(add_message_handle(e_msg_gs_c_arena_start_challenge_res, handle_msg_arena_start_challenge_res));
	ASSERT(add_message_handle(e_msg_gs_c_arena_end_challenge_res, handle_msg_arena_end_challenge_res));
	ASSERT(add_message_handle(e_msg_gs_c_arena_update_team_res, handle_msg_arena_update_team_res));

	ASSERT(add_message_handle(e_msg_gs_c_arena_challenge_reset_cd_res, handle_msg_arena_challenge_reset_cd_res));
	ASSERT(add_message_handle(e_msg_gs_c_arena_challenge_reset_num_res, handle_msg_arena_challenge_reset_num_res));

	ASSERT(add_message_handle(e_msg_gs_c_arena_challenge_ntf, handle_msg_arena_challenge_ntf));
	ASSERT(add_message_handle(e_msg_gs_c_arena_ranks_res, handle_msg_arena_ranks_res));
	ASSERT(add_message_handle(e_msg_gs_c_arena_search_res, handle_msg_arena_search_res));

	ASSERT(add_message_handle(e_msg_gs_c_sign_res, handle_msg_sign_res));
	ASSERT(add_message_handle(e_msg_gs_c_sign_add_res, handle_msg_sign_add_res));
	ASSERT(add_message_handle(e_msg_gs_c_diamond_speed_kill_res, handle_msg_diamond_speed_kill_res));
	ASSERT(add_message_handle(e_msg_gs_c_head_or_frame_change_res, handle_msg_head_or_frame_change_res));
	ASSERT(add_message_handle(e_msg_gs_c_finish_novice_guide_res, handle_msg_finish_novice_guide_res));
	ASSERT(add_message_handle(e_msg_gs_c_edit_player_info_res, handle_msg_edit_player_info_res));
	ASSERT(add_message_handle(e_msg_gs_c_marquee_ntf, handle_msg_marquee_ntf));
	ASSERT(add_message_handle(e_msg_gs_c_search_user_res, handle_msg_search_user_res));
	ASSERT(add_message_handle(e_msg_gs_c_user_friend_op_res, handle_msg_user_friend_op_res));
	ASSERT(add_message_handle(e_msg_gs_c_user_friend_ntf, handle_msg_user_friend_ntf));

	ASSERT(add_message_handle(e_msg_gs_c_json_config_version_res, handle_msg_json_config_version_res));
	ASSERT(add_message_handle(e_msg_gs_c_json_config_res, handle_msg_json_config_res));
	ASSERT(add_message_handle(e_msg_gs_c_chat_res, handle_msg_chat_res));
	ASSERT(add_message_handle(e_msg_gs_c_chat_ntf, handle_msg_chat_ntf));
	ASSERT(add_message_handle(e_msg_gs_c_rank_res, handle_msg_rank_res));
	ASSERT(add_message_handle(e_msg_gs_c_crusade_start_res, handle_msg_crusade_start_res));
	ASSERT(add_message_handle(e_msg_gs_c_crusade_end_res, handle_msg_crusade_end_res));
	ASSERT(add_message_handle(e_msg_gs_c_crusade_hero_life_res, handle_msg_crusade_hero_life_res));
	ASSERT(add_message_handle(e_msg_gs_c_crusade_reset_res, handle_crusade_reset_res));
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

	ASSERT(client_gs->ip != "" && client_gs->port);
	const char* ip = client_gs->ip.c_str();
	int port = client_gs->port;

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

	printf("gs客户端链接数为:[%u]\n", ++m_connect_client_num);

	c_gs_enter_gs_req enter_gs_req;
	BYTE token[GUID_BYTE_LEN] = {0};
	
	//CClientForGS::GetInstance()->token.ToBytes(token, GUID_BYTE_LEN);
	CClientForGS* client_gs = get_client((uv_handle_t*)con);
	ASSERT(client_gs);
	client_gs->token.ToBytes(token, GUID_BYTE_LEN);

	enter_gs_req.set_token((const char*)token, GUID_BYTE_LEN);

	bool ret = send(con, e_msg_gs_c_enter_gs_res, &enter_gs_req);
	ASSERT(ret);

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
		printf("CClientForGS::send msgid[%d][%s]  bytes len[%d] \n", msg_id, e_msg_c_gs_Name((e_msg_c_gs)msg_id).c_str(), PACKET_HEAD_LEN + body_len);
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

		if (g_log){
			CClientForGS* client_gs = get_client((uv_handle_t*)req->handle);
			ASSERT(client_gs);
			UINT32 user_id = client_gs->m_user_info ? client_gs->m_user_info->base_info().user_id() : 0;
			CLogCommond* commond = new CLogCommond();
			commond->Init(CLogCommond::ESERVERTYPE_GS, CLogCommond::ELOGTYPE_SEND, UINT32(req->data), user_id, Now());
			CAsyncCommandModule::GetInstance()->AddCommand(commond);
		}
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

bool CClientForGS::add_message_handle( e_msg_gs_c msgID, func_message_handle handle )
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
		ASSERT( last_msg_id < e_msg_gs_c::e_msg_gs_c_max && last_msg_id > e_msg_gs_c::e_msg_gs_c_min);

		if( (last_msg_received_len + len) >= last_msg_total_len ){//假如包可以完整了
			
			memcpy(tmp_buf + last_msg_received_len, buf, last_msg_total_len - last_msg_received_len);
			
			func_message_handle pMsgHandle = m_mapHandle[last_msg_id];
			if (pMsgHandle){
				bool r = pMsgHandle(stream, tmp_buf + PACKET_HEAD_LEN, last_msg_total_len - PACKET_HEAD_LEN);
				if (g_log){
					CClientForGS* client_gs = get_client((uv_handle_t*)stream);
					ASSERT(client_gs);
					UINT32 user_id = client_gs->m_user_info ? client_gs->m_user_info->base_info().user_id() : 0;
					CLogCommond* commond = new CLogCommond();
					commond->Init(CLogCommond::ESERVERTYPE_GS, CLogCommond::ELOGTYPE_RECEIVE,last_msg_id, user_id, Now());
					CAsyncCommandModule::GetInstance()->AddCommand(commond);
				}
			}
			else{
				printf("CClientForGS::process_msg没有找到消息msgid[%d][%s]的处理函数\n", last_msg_id, e_msg_gs_c_Name((e_msg_gs_c)last_msg_id).c_str());
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

			printf("CClientForGS::process_msg 消息msgid[%d][%s]包不完整 原长[%u]，已接收[%u] 继续拼包\n", last_msg_id, e_msg_gs_c_Name((e_msg_gs_c)last_msg_id).c_str(), m_msg_tmp_buf.buf.len, m_msg_tmp_buf.used_len);
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
			ASSERT(msg_id < e_msg_gs_c::e_msg_gs_c_max && msg_id > e_msg_gs_c::e_msg_gs_c_min);
			p += sizeof msg_id;

			func_message_handle pMsgHandle = m_mapHandle[msg_id];
			if (pMsgHandle){
				bool r = pMsgHandle(stream, p, msg_len - sizeof(msg_len) - sizeof(msg_id));
				if (g_log){
					CClientForGS* client_gs = get_client((uv_handle_t*)stream);
					ASSERT(client_gs);
					UINT32 user_id = client_gs->m_user_info ? client_gs->m_user_info->base_info().user_id() : 0;
					CLogCommond* commond = new CLogCommond();
					commond->Init(CLogCommond::ESERVERTYPE_GS, CLogCommond::ELOGTYPE_RECEIVE, msg_id, user_id, Now());
					CAsyncCommandModule::GetInstance()->AddCommand(commond);
				}
			}
			else{
				printf("CClientForGS::process_msg没有找到消息msgid[%d][%s]的处理函数\n", msg_id, e_msg_gs_c_Name((e_msg_gs_c)msg_id).c_str());
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

void CClientForGS::async_cb( uv_async_t* handle )
{
	ASSERT(handle);
	CClientForGSASync* async = (CClientForGSASync*)handle->data;
	ASSERT(async->op);
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

	delete async;
	uv_close((uv_handle_t*)handle, after_async_close);
}

bool CClientForGS::handle_msg_enter_gs_res( uv_stream_t* stream, const char * buf, int len )
{
	gs_c_enter_gs_res res;
	bool r = res.ParseFromArray(buf, len);
	if (r)
	{
		printf("进入GS结果[%d][%s] \n", res.error_code(), e_gsc_errorcode_Name((e_gsc_errorcode)res.error_code()).c_str());
		//res.PrintDebugString();

		if (res.error_code() == e_gsc_errorcode_success)
		{
			c_gs_config_req req;
			bool r = send(stream, e_msg_c_gs_config_req, &req);
			ASSERT(r);
		}
	}
	else{
		PRINTF_PBPARSE_ERROR(e_msg_gs_c_enter_gs_res);
		return false;
	}
	ASSERT(r);

	return true;
}

#include <fcntl.h>
bool CClientForGS::handle_msg_config_res( uv_stream_t* stream, const char * buf, int len )
{
	gs_c_config_res res;
	bool r = res.ParseFromArray(buf, len);
	if (r)
	{
		printf("获取客户端配置结果[%d][%s] \n", res.error_code(), e_gsc_errorcode_Name((e_gsc_errorcode)res.error_code()).c_str());
		res.DebugString();

		//static uv_fs_t open_req1;
		//static uv_fs_t write_req;
		//static uv_fs_t close_req;
		//r = uv_fs_open(stream->loop, &open_req1, "E:/WH/code/server/Debug/test_file.txt", O_RDWR | O_APPEND, 0, NULL);
		//ASSERT(r >= 0);
		//ASSERT(open_req1.result >= 0);
		//uv_fs_req_cleanup(&open_req1);

		//uv_buf_t iov = uv_buf_init(const_cast<char*>(res.DebugString().c_str()), res.DebugString().size());
		//r = uv_fs_write(stream->loop, &write_req, open_req1.result, &iov, 1, -1, NULL);
		//ASSERT(r >= 0);
		//ASSERT(write_req.result >= 0);
		//uv_fs_req_cleanup(&write_req);

		//r = uv_fs_close(stream->loop, &close_req, open_req1.result, NULL);
		//ASSERT(r == 0);
		//ASSERT(close_req.result == 0);
		//uv_fs_req_cleanup(&close_req);

		if (res.error_code() == e_gsc_errorcode_success)
		{
			//c_gs_user_info_req req;
			//bool r = send(stream, e_msg_c_gs_user_info_req, &req);
			c_gs_json_config_version_req req;
			bool r = send(stream, e_msg_c_gs_json_config_version_req, &req);
			ASSERT(r);
		}
	}
	else{
		PRINTF_PBPARSE_ERROR(e_msg_gs_c_config_res);
		return false;
	}
	ASSERT(r);

	return true;
}

bool CClientForGS::handle_msg_json_config_version_res( uv_stream_t* stream, const char * buf, int len )
{
	gs_c_json_config_version_res res;
	bool r = res.ParseFromArray(buf, len);
	if (r)
	{
		printf("获取客户端json文件版本信息结果[%d][%s] \n", res.error_code(), e_gsc_errorcode_Name((e_gsc_errorcode)res.error_code()).c_str());
		//res.PrintDebugString();

		if (res.error_code() == e_gsc_errorcode_success)
		{
			//c_gs_user_info_req req;
			//bool r = send(stream, e_msg_c_gs_user_info_req, &req);
			
			c_gs_json_config_req req;
			req.add_names("hero_0");
			bool r = send(stream, e_msg_c_gs_json_config_req, &req);
			ASSERT(r);
		}
	}
	else{
		PRINTF_PBPARSE_ERROR(e_msg_gs_c_json_config_version_res);
		return false;
	}
	ASSERT(r);

	return true;
}

bool CClientForGS::handle_msg_json_config_res( uv_stream_t* stream, const char * buf, int len )
{
	gs_c_json_config_res res;
	bool r = res.ParseFromArray(buf, len);
	if (r)
	{
		printf("获取客户端json文件信息结果[%d][%s] \n", res.error_code(), e_gsc_errorcode_Name((e_gsc_errorcode)res.error_code()).c_str());
		//res.PrintDebugString();
		const gs_c_json_config_res_config& c = res.configs(0);


		if (res.error_code() == e_gsc_errorcode_success)
		{
			c_gs_user_info_req req;
			bool r = send(stream, e_msg_c_gs_user_info_req, &req);
			ASSERT(r);
		}
	}
	else{
		PRINTF_PBPARSE_ERROR(e_msg_gs_c_json_config_res);
		return false;
	}
	ASSERT(r);

	return true;
}

bool CClientForGS::handle_msg_user_info_res( uv_stream_t* stream, const char * buf, int len )
{
	//gs_c_user_info_res res;
	CClientForGS* client_gs = get_client((uv_handle_t*)stream);
	ASSERT(client_gs && !client_gs->m_user_info);

	client_gs->m_user_info = new gs_c_user_info_res;
	gs_c_user_info_res &res = *client_gs->m_user_info;
	bool r = res.ParseFromArray(buf, len);
	if (r)
	{
		printf("获取玩家信息结果[%d][%s] \n", res.error_code(), e_gsc_errorcode_Name((e_gsc_errorcode)res.error_code()).c_str());

		if (res.error_code() == e_gsc_errorcode_success)
		{
			//c_gs_config_req req;
			//bool r = send(stream, e_msg_c_gs_config_req, &req);
			//ASSERT(r);

			::google::protobuf::RepeatedPtrField<user_sub_task>::iterator it = res.mutable_sub_tasks()->mutable_items()->begin();
			for (; it != res.mutable_sub_tasks()->mutable_items()->end() ; ++it)
			{
				
			}

			//res.PrintDebugString();
			//res.base_info().PrintDebugString();
			//res.shops().PrintDebugString();

			c_gs_start_sub_task_req req;
			req.set_sub_task_id(2);
			req.add_heros(1);

			
			bool r = send(stream, e_msg_c_gs_start_sub_task_req, &req);
			ASSERT(r);
		}
	}
	else{
		PRINTF_PBPARSE_ERROR(e_msg_gs_c_user_info_res);
		return false;
	}
	ASSERT(r);

	return true;
}

bool CClientForGS::handle_msg_start_sub_task_res( uv_stream_t* stream, const char * buf, int len )
{
	gs_c_start_sub_task_res res;
	bool r = res.ParseFromArray(buf, len);
	if (r)
	{
		printf("开始子任务结果[%d][%s] \n", res.error_code(), e_gsc_errorcode_Name((e_gsc_errorcode)res.error_code()).c_str());

		//res.PrintDebugString();

		c_gs_end_sub_task_req req;
		req.set_is_win(true);
		req.set_star(3);
		bool r = send(stream, e_msg_c_gs_end_sub_task_req, &req);
		ASSERT(r);

	}
	else{
		PRINTF_PBPARSE_ERROR(e_msg_gs_c_start_sub_task_res);
		return false;
	}
	ASSERT(r);

	return true;
}

bool CClientForGS::handle_msg_end_sub_task_res( uv_stream_t* stream, const char * buf, int len )
{
	gs_c_end_sub_task_res res;
	bool r = res.ParseFromArray(buf, len);
	if (r)
	{
		printf("结束子任务结果[%d][%s] \n", res.error_code(), e_gsc_errorcode_Name((e_gsc_errorcode)res.error_code()).c_str());

		//res.PrintDebugString();
		if (res.error_code() == e_gsc_errorcode_success)
		{
	
		}

		c_gs_invoke_hero_req req;
		req.set_invoke_type(c_gs_invoke_hero_req::FRAGMENT);
		req.set_hero_id(15);
		bool r = send(stream, e_msg_c_gs_invoke_hero_req, &req);
		ASSERT(r);
	}
	else{
		PRINTF_PBPARSE_ERROR(e_msg_gs_c_end_sub_task_res);
		return false;
	}
	ASSERT(r);

	return true;
}

bool CClientForGS::handle_msg_invoke_hero_res( uv_stream_t* stream, const char * buf, int len )
{
	gs_c_invoke_hero_res res;
	bool r = res.ParseFromArray(buf, len);
	if (r)
	{
		printf("召唤英雄结果[%d][%s] \n", res.error_code(), e_gsc_errorcode_Name((e_gsc_errorcode)res.error_code()).c_str());

		//res.PrintDebugString();


		c_gs_equip_level_up_req req;
		//req.set_op_type(c_gs_equip_level_up_req::EQUIP1_LEVELN);
		//req.set_hero_id(18);
		//req.set_equip_id(5001);

		req.set_op_type(c_gs_equip_level_up_req::EQUIPN_LEVELN);
		req.set_hero_id(5);
		
		bool r = send(stream, e_msg_c_gs_equip_level_up_req, &req);
		ASSERT(r);

	}
	else{
		PRINTF_PBPARSE_ERROR(e_msg_gs_c_invoke_hero_res);
		return false;
	}
	ASSERT(r);

	return true;
}

bool CClientForGS::handle_msg_equip_level_up_res( uv_stream_t* stream, const char * buf, int len )
{
	gs_c_equip_level_up_res res;
	bool r = res.ParseFromArray(buf, len);
	if (r)
	{
		printf("升级装备结果[%d][%s] \n", res.error_code(), e_gsc_errorcode_Name((e_gsc_errorcode)res.error_code()).c_str());

		//res.PrintDebugString();

		c_gs_equip_evolution_req req;
		req.set_hero_id(17);
		req.set_equip_id(321);
		send(stream, e_msg_c_gs_equip_evolution_req, &req);

	}
	else{
		PRINTF_PBPARSE_ERROR(e_msg_gs_c_equip_level_up_res);
		return false;
	}
	ASSERT(r);

	return true;
}

bool CClientForGS::handle_msg_equip_evolution_res( uv_stream_t* stream, const char * buf, int len )
{
	gs_c_equip_evolution_res res;
	bool r = res.ParseFromArray(buf, len);
	if (r)
	{
		printf("进化装备结果[%d][%s] \n", res.error_code(), e_gsc_errorcode_Name((e_gsc_errorcode)res.error_code()).c_str());
		//res.PrintDebugString();

		c_gs_hero_evolution_req req;
		req.set_hero_id(17);
		send(stream, e_msg_c_gs_hero_evolution_req, &req);
	}
	else{
		PRINTF_PBPARSE_ERROR(e_msg_gs_c_equip_evolution_res);
		return false;
	}
	ASSERT(r);

	return true;
}

bool CClientForGS::handle_msg_hero_evolution_res( uv_stream_t* stream, const char * buf, int len )
{
	gs_c_hero_evolution_res res;
	bool r = res.ParseFromArray(buf, len);
	if (r)
	{
		printf("英雄突破结果[%d][%s] \n", res.error_code(), e_gsc_errorcode_Name((e_gsc_errorcode)res.error_code()).c_str());

		//res.PrintDebugString();

		{
			c_gs_sell_req req2;

			{
				struct_game_element* pEle = req2.add_items();
				pEle->set_id(89);
				pEle->set_num(100);
				pEle->set_num_type(struct_game_element::DECREASE);
				pEle->set_type(struct_game_element::EELEMENTTYPE_ITEM);
			}

			send(stream, e_msg_c_gs_sell_req, &req2);
		}

	}
	else{
		PRINTF_PBPARSE_ERROR(e_msg_gs_c_invoke_hero_res);
		return false;
	}
	ASSERT(r);

	return true;
}


bool CClientForGS::handle_msg_sell_res( uv_stream_t* stream, const char * buf, int len )
{
	gs_c_sell_res res;
	bool r = res.ParseFromArray(buf, len);
	if (r)
	{
		printf("出售结果[%d][%s]\n", res.error_code(), e_gsc_errorcode_Name((e_gsc_errorcode)res.error_code()).c_str());
		//res.PrintDebugString();
		
		
		c_gs_fragment_compound_req req;
		req.set_compound_id(13);
		send(stream, e_msg_c_gs_fragment_compound_req, &req);

		/*
		c_gs_hero_star_up_req req;
		req.set_hero_id(17);
		send(stream, e_msg_c_gs_hero_star_up_req, &req);
		*/
	}
	else{
		PRINTF_PBPARSE_ERROR(e_msg_gs_c_sell_res);
		return false;
	}
	ASSERT(r);
	return true;
}	

bool CClientForGS::handle_msg_fragment_compound_res(uv_stream_t* stream, const char * buf, int len )
{
	gs_c_fragment_compound_res res;
	bool r = res.ParseFromArray(buf, len);
	if (r)
	{
		printf("合成结果[%d][%s]\n", res.error_code(), e_gsc_errorcode_Name((e_gsc_errorcode)res.error_code()).c_str());
		//res.PrintDebugString();

		c_gs_hero_star_up_req req;
		req.set_hero_id(17);
		send(stream, e_msg_c_gs_hero_star_up_req, &req);
	}
	else
	{
		PRINTF_PBPARSE_ERROR(e_msg_gs_c_fragment_compound_res);
		return false;
	}
	ASSERT(r);
	return true;
}

bool CClientForGS::handle_msg_hero_star_up_res(uv_stream_t* stream, const char * buf, int len )
{
	gs_c_hero_star_up_res res;
	bool r = res.ParseFromArray(buf, len);
	if (r)
	{
		printf("英雄升星结果[%d][%s]\n", res.error_code(), e_gsc_errorcode_Name((e_gsc_errorcode)res.error_code()).c_str());
		//res.PrintDebugString();

		c_gs_skill_level_up_req req;
		req.set_hero_id(17);
		req.set_skill_type(user_hero_skill_e_skill_type_AOYI);
		req.set_level(1);

		send(stream, e_msg_c_gs_skill_level_up_req, &req);
	}
	else
	{
		PRINTF_PBPARSE_ERROR(e_msg_gs_c_hero_star_up_res);
		return false;
	}
	ASSERT(r);
	return true;
}

bool CClientForGS::handle_msg_skill_level_up_res( uv_stream_t* stream, const char * buf, int len )
{
	gs_c_skill_level_up_res res;
	bool r = res.ParseFromArray(buf, len);
	if (r)
	{
		printf("技能升级结果[%d][%s]\n", res.error_code(), e_gsc_errorcode_Name((e_gsc_errorcode)res.error_code()).c_str());
		//res.PrintDebugString();

		c_gs_mail_opt_req req;
		req.set_mail_opt(c_gs_mail_opt_req::READ1);
		req.add_mail_id_arr(267);
		send(stream, e_msg_c_gs_mail_opt_req, &req);
		
		/*
		c_gs_invoke_req req;
		req.set_invoke_type(c_gs_invoke_req::GOLD_FREE);
		send(stream, e_msg_c_gs_invoke_req, &req);

		req.set_invoke_type(c_gs_invoke_req::GOLD_1);
		send(stream, e_msg_c_gs_invoke_req, &req);

		req.set_invoke_type(c_gs_invoke_req::GOLD_N);
		send(stream, e_msg_c_gs_invoke_req, &req);

		req.set_invoke_type(c_gs_invoke_req::DIAMOND_FREE);
		send(stream, e_msg_c_gs_invoke_req, &req);

		req.set_invoke_type(c_gs_invoke_req::DIAMOND_1);
		send(stream, e_msg_c_gs_invoke_req, &req);

		req.set_invoke_type(c_gs_invoke_req::DIAMOND_N);
		send(stream, e_msg_c_gs_invoke_req, &req);


		c_gs_buy_req req_buy;
		req_buy.set_buy_type(struct_game_element::EELEMENTTYPE_SKILL_POINT);
		send(stream, e_msg_c_gs_buy_req, &req_buy);
		*/
	}
	else
	{
		PRINTF_PBPARSE_ERROR(e_msg_gs_c_skill_level_up_res);
		return false;
	}
	ASSERT(r);
	return true;
}

bool CClientForGS::handle_msg_mail_opt_res( uv_stream_t* stream, const char * buf, int len )
{
	gs_c_mail_opt_res  res;
	bool r = res.ParseFromArray(buf, len);
	if (r)
	{
		printf("邮件操作(领取全部邮件)[%d][%s] \n", res.error_code(), e_gsc_errorcode_Name((e_gsc_errorcode)res.error_code()).c_str());
		//res.PrintDebugString();
		
		c_gs_invoke_req req;
		req.set_invoke_type(c_gs_invoke_req::GOLD_FREE);
		send(stream, e_msg_c_gs_invoke_req, &req);

		req.set_invoke_type(c_gs_invoke_req::GOLD_1);
		send(stream, e_msg_c_gs_invoke_req, &req);

		req.set_invoke_type(c_gs_invoke_req::GOLD_N);
		send(stream, e_msg_c_gs_invoke_req, &req);

		req.set_invoke_type(c_gs_invoke_req::DIAMOND_FREE);
		send(stream, e_msg_c_gs_invoke_req, &req);

		req.set_invoke_type(c_gs_invoke_req::DIAMOND_1);
		send(stream, e_msg_c_gs_invoke_req, &req);

		req.set_invoke_type(c_gs_invoke_req::DIAMOND_N);
		send(stream, e_msg_c_gs_invoke_req, &req);

		req.set_invoke_type(c_gs_invoke_req::VIP_DIAMOND_1);
		send(stream, e_msg_c_gs_invoke_req, &req);

		req.set_invoke_type(c_gs_invoke_req::VIP_DIAMOND_N);
		send(stream, e_msg_c_gs_invoke_req, &req);

		req.set_invoke_type(c_gs_invoke_req::FRIEND_POINT);
		req.set_invoke_num(10);
		send(stream, e_msg_c_gs_invoke_req, &req);

		/*c_gs_buy_req req_buy;
		req_buy.set_buy_type(struct_game_element::EELEMENTTYPE_SKILL_POINT);
		send(stream, e_msg_c_gs_buy_req, &req_buy);*/

		c_gs_buy_req req_buy2;
		req_buy2.set_buy_type(struct_game_element::EELEMENTTYPE_SUB_TASK_ENTER_NUM);
		req_buy2.set_element_id(1);
		send(stream, e_msg_c_gs_buy_req, &req_buy2);
	}
	else{
		PRINTF_PBPARSE_ERROR(e_msg_gs_c_end_sub_task_res);
		return false;
	}
	ASSERT(r);
	return true;
}
bool CClientForGS::handle_msg_invoke_res( uv_stream_t* stream, const char * buf, int len )
{
	gs_c_invoke_res res;
	bool r = res.ParseFromArray(buf, len);
	if (r)
	{
		printf("祭坛召唤结果[%d][%s]\n", res.error_code(), e_gsc_errorcode_Name((e_gsc_errorcode)res.error_code()).c_str());
		//res.PrintDebugString();

		
	}
	else
	{
		PRINTF_PBPARSE_ERROR(e_msg_gs_c_skill_level_up_res);
		return false;
	}
	ASSERT(r);
	return true;
}
bool CClientForGS::handle_msg_buy_res(uv_stream_t* stream, const char * buf, int len )
{
	gs_c_buy_res res;
	bool r = res.ParseFromArray(buf, len);
	if (r)
	{
		printf("购买技能点结果[%d][%s]\n", res.error_code(), e_gsc_errorcode_Name((e_gsc_errorcode)res.error_code()).c_str());
		//res.PrintDebugString();
		
		c_gs_server_time_req req;
		send(stream, e_msg_c_gs_server_time_req, &req);
	}
	else
	{
		PRINTF_PBPARSE_ERROR(e_msg_gs_c_buy_res);
		return false;
	}
	ASSERT(r);
	return true;
}

bool CClientForGS::handle_msg_server_time_res(uv_stream_t* stream, const char * buf, int len)
{
	gs_c_server_time_res res;
	bool r = res.ParseFromArray(buf, len);
	if (r)
	{
		printf("请求服务器时间结果[%d][%s]\n", res.error_code(), e_gsc_errorcode_Name((e_gsc_errorcode)res.error_code()).c_str());
		//res.PrintDebugString();

		c_gs_use_item_req req;

		////使用扫荡券
		//req.set_item_id(601);
		//req.set_num(10);
		//req.set_target_id(1);

		//使用经验药水
		req.set_item_id(601);
		req.set_num(10);
		req.set_target_id(2);

		send(stream, e_msg_c_gs_use_item_req, &req);
	}
	else
	{
		PRINTF_PBPARSE_ERROR(e_msg_gs_c_server_time_res);
		return false;
	}
	ASSERT(r);
	return true;
}

//bool CClientForGS::handle_msg_get_activity_task_res( uv_stream_t* stream, const char * buf, int len )
//{
//	gs_c_get_activity_task_res res;
//	bool r = res.ParseFromArray(buf, len);
//	if (r)
//	{
//		printf("请求有效活动列表结果[%d][%s]\n", res.error_code(), e_gsc_errorcode_Name((e_gsc_errorcode)res.error_code()).c_str());
//		res.PrintDebugString();
//
//		c_gs_leave_gs_req req;
//		send(stream, e_msg_c_gs_leave_gs_req, &req);
//	}
//	else
//	{
//		PRINTF_PBPARSE_ERROR(e_msg_gs_c_get_activity_task_res);
//		return false;
//	}
//}

bool CClientForGS::handle_msg_use_item_res(uv_stream_t* stream, const char * buf, int len)
{
	gs_c_use_item_res res;
	bool r = res.ParseFromArray(buf, len);
	if (r)
	{
		printf("使用道具结果[%d][%s]\n", res.error_code(), e_gsc_errorcode_Name((e_gsc_errorcode)res.error_code()).c_str());
		res.PrintDebugString();

		c_gs_shop_info_req req;
		req.set_pull_type(c_gs_shop_info_req::ALL);
		send(stream, e_msg_c_gs_shop_info_req, &req);

		//c_gs_shop_info_req req2;
		//req2.set_pull_type(c_gs_shop_info_req::ONE);
		//req2.set_shop_type(user_shop::COMMON);
		//send(stream, e_msg_c_gs_shop_info_req, &req2);
	}
	else
	{
		PRINTF_PBPARSE_ERROR(e_msg_gs_c_use_item_res);
		return false;
	}
	ASSERT(r);
	return true;
}

bool CClientForGS::handle_msg_shop_info_res(uv_stream_t* stream, const char * buf, int len)
{
	gs_c_shop_info_res res;
	bool r = res.ParseFromArray(buf, len);
	if (r)
	{
		printf("获取商店信息[%d][%s]\n", res.error_code(), e_gsc_errorcode_Name((e_gsc_errorcode)res.error_code()).c_str());
		//res.PrintDebugString();

		c_gs_shop_buy_item_req req;
		req.set_shop_type(user_shop::COMMON);
		req.set_pos(0);
		send(stream, e_msg_c_gs_shop_buy_item_req, &req);
	}
	else
	{
		PRINTF_PBPARSE_ERROR(e_msg_gs_c_use_item_res);
		return false;
	}
	ASSERT(r);
	return true;
}
bool CClientForGS::handle_msg_shop_buy_item_res(uv_stream_t* stream, const char * buf, int len)
{
	gs_c_shop_buy_item_res res;
	bool r = res.ParseFromArray(buf, len);
	if (r)
	{
		printf("购买商店道具结果[%d][%s]\n", res.error_code(), e_gsc_errorcode_Name((e_gsc_errorcode)res.error_code()).c_str());
		//res.PrintDebugString();

		c_gs_shop_refresh_req req;
		req.set_shop_type(user_shop::COMMON);
		send(stream, e_msg_c_gs_shop_refresh_req, &req);
	}
	else
	{
		PRINTF_PBPARSE_ERROR(e_msg_gs_c_use_item_res);
		return false;
	}
	ASSERT(r);
	return true;
}

bool CClientForGS::handle_msg_shop_refresh_res(uv_stream_t* stream, const char * buf, int len)
{
	gs_c_shop_refresh_res res;
	bool r = res.ParseFromArray(buf, len);
	if (r)
	{
		printf("刷新商店结果[%d][%s]\n", res.error_code(), e_gsc_errorcode_Name((e_gsc_errorcode)res.error_code()).c_str());
		//res.PrintDebugString();

		c_gs_get_mission_reward_req req;
		req.set_mission_id(1);
		send(stream, e_msg_c_gs_get_mission_reward_req, &req);
	}
	else
	{
		PRINTF_PBPARSE_ERROR(e_msg_gs_c_use_item_res);
		return false;
	}
	ASSERT(r);
	return true;
}


bool CClientForGS::handle_msg_mission_get_reward_res(uv_stream_t* stream, const char * buf, int len)
{
	gs_c_get_mission_reward_res  res;
	bool r = res.ParseFromArray(buf, len);
	if (r)
	{
		printf("任务领取结果[%d][%s]\n", res.error_code(), e_gsc_errorcode_Name((e_gsc_errorcode)res.error_code()).c_str());
		//res.PrintDebugString();

		//c_gs_leave_gs_req req;
		//send(stream, e_msg_c_gs_leave_gs_req, &req);

		//return true;

		c_gs_arena_refresh_match_req req;
		send(stream, e_msg_c_gs_arena_refresh_match_req, &req);
	}
	else
	{
		PRINTF_PBPARSE_ERROR(e_msg_gs_c_get_mission_reward_res);
		return false;
	}
	ASSERT(r);
	return true;
}

bool CClientForGS::handle_msg_arena_refresh_match_res(uv_stream_t* stream, const char * buf, int len)
{
	gs_c_arena_refresh_match_res res;
	bool r = res.ParseFromArray(buf, len);
	if (r)
	{
		printf("刷新竞技场对手结果[%d][%s]\n", res.error_code(), e_gsc_errorcode_Name((e_gsc_errorcode)res.error_code()).c_str());
		//res.PrintDebugString();
		
		c_gs_arena_start_challenge_req req;
		req.add_heros(1);
		//req.add_heros(16);
		//req.set_heros(0, 1);
		req.set_defender_user_id(931);
		//req.set_defender_user_id(41);
		req.set_power(1000);
		req.set_defender_power(10000);

		send(stream, e_msg_c_gs_arena_start_challenge_req, &req);
	}
	else
	{
		PRINTF_PBPARSE_ERROR(e_msg_gs_c_arena_refresh_match_res);
		return false;
	}
	ASSERT(r);
	return true;
}

bool CClientForGS::handle_msg_arena_start_challenge_res(uv_stream_t* stream, const char * buf, int len)
{
	gs_c_arena_start_challenge_res res;
	bool r = res.ParseFromArray(buf, len);
	if (r)
	{
		printf("竞技场开始战斗结果[%d][%s]\n", res.error_code(), e_gsc_errorcode_Name((e_gsc_errorcode)res.error_code()).c_str());
		//res.PrintDebugString();

		c_gs_arena_end_challenge_req req;
		req.set_is_win(true);
		send(stream, e_msg_c_gs_arena_end_challenge_req, &req);
	}
	else
	{
		PRINTF_PBPARSE_ERROR(e_msg_gs_c_arena_start_challenge_res);
		return false;
	}
	ASSERT(r);
	return true;
}

bool CClientForGS::handle_msg_arena_end_challenge_res(uv_stream_t* stream, const char * buf, int len)
{
	gs_c_arena_end_challenge_res res;
	bool r = res.ParseFromArray(buf, len);
	if (r)
	{
		printf("竞技场结束战斗结果[%d][%s]\n", res.error_code(), e_gsc_errorcode_Name((e_gsc_errorcode)res.error_code()).c_str());
		//res.PrintDebugString();

		c_gs_arena_update_team_req req;
		req.add_heros(2);
		send(stream, e_msg_c_gs_arena_update_team_req, &req);
	}
	else
	{
		PRINTF_PBPARSE_ERROR(e_msg_gs_c_arena_end_challenge_res);
		return false;
	}
	ASSERT(r);
	return true;
}

bool CClientForGS::handle_msg_arena_update_team_res(uv_stream_t* stream, const char * buf, int len)
{
	gs_c_arena_update_team_res res;
	bool r = res.ParseFromArray(buf, len);
	if (r)
	{
		printf("竞技场改变防守阵容结果[%d][%s]\n", res.error_code(), e_gsc_errorcode_Name((e_gsc_errorcode)res.error_code()).c_str());
		//res.PrintDebugString();

		//c_gs_leave_gs_req req;
		//send(stream, e_msg_c_gs_leave_gs_req, &req);
		c_gs_arena_challenge_reset_cd_req req;
		send(stream, e_msg_c_gs_arena_challenge_reset_cd_req, &req);
	}
	else
	{
		PRINTF_PBPARSE_ERROR(e_msg_gs_c_arena_update_team_res);
		return false;
	}
	ASSERT(r);
	return true;
}

bool CClientForGS::handle_msg_arena_challenge_reset_cd_res( uv_stream_t* stream, const char * buf, int len )
{
	gs_c_arena_challenge_reset_cd_res res;
	bool r = res.ParseFromArray(buf, len);
	if (r)
	{
		printf("竞技场重置挑战cd结果[%d][%s]\n", res.error_code(), e_gsc_errorcode_Name((e_gsc_errorcode)res.error_code()).c_str());
		//res.PrintDebugString();

		//c_gs_leave_gs_req req;
		//send(stream, e_msg_c_gs_leave_gs_req, &req);
		c_gs_arena_challenge_reset_num_req req;
		send(stream, e_msg_c_gs_arena_challenge_reset_num_req, &req);
	}
	else
	{
		PRINTF_PBPARSE_ERROR(e_msg_gs_c_arena_challenge_reset_cd_res);
		return false;
	}
	ASSERT(r);
	return true;
}

bool CClientForGS::handle_msg_arena_challenge_reset_num_res( uv_stream_t* stream, const char * buf, int len )
{
	gs_c_arena_challenge_reset_num_res res;
	bool r = res.ParseFromArray(buf, len);
	if (r)
	{
		printf("竞技场重置挑战次数结果[%d][%s]\n", res.error_code(), e_gsc_errorcode_Name((e_gsc_errorcode)res.error_code()).c_str());
		//res.PrintDebugString();

		c_gs_arena_ranks_req req;
		req.set_page_num(0);
		send(stream, e_msg_c_gs_arena_ranks_req, &req);

		//c_gs_leave_gs_req req;
		//send(stream, e_msg_c_gs_leave_gs_req, &req);
	}
	else
	{
		PRINTF_PBPARSE_ERROR(e_msg_gs_c_arena_challenge_reset_num_res);
		return false;
	}
	ASSERT(r);
	return true;
}

bool CClientForGS::handle_msg_arena_ranks_res( uv_stream_t* stream, const char * buf, int len )
{
	gs_c_arena_challenge_reset_num_res res;
	bool r = res.ParseFromArray(buf, len);
	if (r)
	{
		printf("竞技场获取排行榜结果[%d][%s]\n", res.error_code(), e_gsc_errorcode_Name((e_gsc_errorcode)res.error_code()).c_str());
		//res.PrintDebugString();

		c_gs_arena_search_req req;
		req.set_user_id(1);
		send(stream, e_msg_c_gs_arena_search_req, &req);
	}
	else
	{
		PRINTF_PBPARSE_ERROR(e_msg_gs_c_arena_challenge_reset_num_res);
		return false;
	}
	ASSERT(r);
	return true;
}

bool CClientForGS::handle_msg_arena_search_res( uv_stream_t* stream, const char * buf, int len )
{
	gs_c_arena_search_res res;
	bool r = res.ParseFromArray(buf, len);
	if (r)
	{
		printf("竞技场排行榜搜索结果[%d][%s]\n", res.error_code(), e_gsc_errorcode_Name((e_gsc_errorcode)res.error_code()).c_str());
		//res.PrintDebugString();

		//c_gs_leave_gs_req req;
		//send(stream, e_msg_c_gs_leave_gs_req, &req);

		//签到请求
		c_gs_sign_req req;
		//req.set_sign_type(c_gs_sign_req::SIGN_TYPE_1);
		//send(stream, e_msg_c_gs_sign_req, &req);

		req.set_sign_type(c_gs_sign_req::SIGN_TYPE_N);
		req.set_reward_type(sign_n_reward::SIGN_N_REWARD_TYPE_FIRST);
		send(stream, e_msg_c_gs_sign_req, &req);
	}
	else
	{
		PRINTF_PBPARSE_ERROR(e_msg_gs_c_arena_search_res);
		return false;
	}
	ASSERT(r);
	return true;
}

bool CClientForGS::handle_msg_arena_challenge_ntf( uv_stream_t* stream, const char * buf, int len )
{
	gs_c_arena_challenge_ntf ntf;
	bool r = ntf.ParseFromArray(buf, len);
	if (r)
	{
		printf("收到竞技场日志通知\n");
		//ntf.PrintDebugString();
	}
	else
	{
		PRINTF_PBPARSE_ERROR(e_msg_gs_c_arena_challenge_ntf);
		return false;
	}
	ASSERT(r);
	return true;
}

bool CClientForGS::handle_msg_sign_res( uv_stream_t* stream, const char * buf, int len )
{
	gs_c_sign_res res;
	bool r = res.ParseFromArray(buf, len);
	if (r)
	{
		printf("签到结果[%d][%s]\n", res.error_code(), e_gsc_errorcode_Name((e_gsc_errorcode)res.error_code()).c_str());
		res.PrintDebugString();

		c_gs_sign_add_req req;
		send(stream, e_msg_c_gs_sign_add_req, &req);

	}
	else
	{
		PRINTF_PBPARSE_ERROR(e_msg_gs_c_sign_res);
		return false;
	}
	ASSERT(r);
	return true;
}

bool CClientForGS::handle_msg_sign_add_res( uv_stream_t* stream, const char * buf, int len )
{
	gs_c_sign_add_res res;
	bool r = res.ParseFromArray(buf, len);
	if (r)
	{
		printf("补签结果[%d][%s]\n", res.error_code(), e_gsc_errorcode_Name((e_gsc_errorcode)res.error_code()).c_str());
		res.PrintDebugString();

		c_gs_diamond_speed_kill_req req;
		req.set_num(200);
		req.set_sub_task_id(2);
		send(stream, e_msg_c_gs_diamond_speed_kill_req, &req);
	}
	else
	{
		PRINTF_PBPARSE_ERROR(e_msg_gs_c_sign_add_res);
		return false;
	}
	ASSERT(r);
	return true;
}

bool CClientForGS::handle_msg_diamond_speed_kill_res(uv_stream_t* stream, const char * buf, int len )
{
	gs_c_diamond_speed_kill_res res;
	bool r = res.ParseFromArray(buf, len);
	if (r)
	{
		printf("钻石扫荡结果[%d][%s] \n", res.error_code(), e_gsc_errorcode_Name((e_gsc_errorcode)res.error_code()).c_str());
		res.PrintDebugString();

		c_gs_edit_player_info_req req;
		//char* name = ANSIToUTF8("AAAA");//客户端传的就是utf8
		req.set_signature("aaabbbccc");
		send(stream, e_msg_c_gs_edit_player_info_req, &req);
	}
	else
	{
		PRINTF_PBPARSE_ERROR(e_msg_gs_c_diamond_speed_kill_res);
		return false;
	}
	ASSERT(r);

	return true;
}

bool CClientForGS::handle_msg_edit_player_info_res(uv_stream_t* stream, const char * buf, int len )
{
	gs_c_edit_player_info_res res;
	bool r = res.ParseFromArray(buf, len);
	if (r)
	{
		printf("编辑玩家信息结果[%d][%s] \n", res.error_code(), e_gsc_errorcode_Name((e_gsc_errorcode)res.error_code()).c_str());
		c_gs_finish_novice_guide_req req;
		req.set_guide_id(12);
		send(stream, e_msg_c_gs_finish_novice_guide_req, &req);
	}
	else{
		PRINTF_PBPARSE_ERROR(e_msg_gs_c_edit_player_info_res);
		return false;
	}
	ASSERT(r);
	return true;
}

bool CClientForGS::handle_msg_head_or_frame_change_res(uv_stream_t* stream, const char * buf, int len )
{
	gs_c_head_or_frame_change_res res;
	bool r = res.ParseFromArray(buf, len);
	if (r)
	{
		printf("更改头像或头像框结果[%d][%s] \n", res.error_code(), e_gsc_errorcode_Name((e_gsc_errorcode)res.error_code()).c_str());
		//res.PrintDebugString();
		c_gs_finish_novice_guide_req req;
		req.set_guide_id(12);
		send(stream, e_msg_c_gs_finish_novice_guide_req, &req);
	}
	else
	{
		PRINTF_PBPARSE_ERROR(e_msg_gs_c_head_or_frame_change_res);
		return false;
	}
	ASSERT(r);

	return true;
}

bool CClientForGS::handle_msg_finish_novice_guide_res(uv_stream_t* stream, const char * buf, int len )
{
	gs_c_finish_novice_guide_res res;
	bool r = res.ParseFromArray(buf, len);
	if (r)
	{
		printf("新手引导结果[%d][%s] \n", res.error_code(), e_gsc_errorcode_Name((e_gsc_errorcode)res.error_code()).c_str());
		res.PrintDebugString();

		c_gs_search_user_req req;
		req.set_user_id(2009);

		send(stream, e_msg_c_gs_search_user_req, &req);
	}
	else
	{
		PRINTF_PBPARSE_ERROR(e_msg_gs_c_finish_novice_guide_res);
		return false;
	}
	ASSERT(r);

	return true;

}

bool CClientForGS::handle_msg_search_user_res(uv_stream_t* stream, const char * buf, int len )
{
	gs_c_search_user_res res;
	bool r = res.ParseFromArray(buf, len);
	if (r)
	{
		printf("搜索玩家结果[%d][%s]\n",  res.error_code(), e_gsc_errorcode_Name((e_gsc_errorcode)res.error_code()).c_str());
		res.PrintDebugString();

		//添加该玩家为好友
		c_gs_user_friend_op_req req;
		req.set_op_type(c_gs_user_friend_op_req::GIVE_ELEMENT1);
		req.set_user_id(2020);
		send(stream, e_msg_c_gs_user_friend_op_req, &req);
	}
	else{
		PRINTF_PBPARSE_ERROR(e_msg_gs_c_search_user_res);
		return false;
	}
	ASSERT(r);
	return true;
}

bool CClientForGS::handle_msg_user_friend_op_res( uv_stream_t* stream, const char * buf, int len )
{
	gs_c_user_friend_op_res res;
	bool r = res.ParseFromArray(buf, len);
	if (r)
	{
		//聊天
		c_gs_chat_req chat_req;
		chat_req.set_chat_type(c_gs_chat_req::WORLD);
		chat_req.set_word("世界聊天");
		send(stream, e_msg_c_gs_chat_req, &chat_req);

		chat_req.set_chat_type(c_gs_chat_req::GUILD);
		chat_req.set_word("帮会聊天");
		send(stream, e_msg_c_gs_chat_req, &chat_req);

		chat_req.set_chat_type(c_gs_chat_req::WHISPER);
		chat_req.set_to_id(2020);
		chat_req.set_word("私聊 id");
		send(stream, e_msg_c_gs_chat_req, &chat_req);

		chat_req.set_chat_type(c_gs_chat_req::WHISPER);
		chat_req.clear_to_id();
		chat_req.set_to_name("world herod");
		chat_req.set_word("私聊 名字");
		send(stream, e_msg_c_gs_chat_req, &chat_req);

		switch(res.op_type()){
			case c_gs_user_friend_op_req::ADD_FRIEND:{
				printf("申请好友结果[%d][%s]\n",  res.error_code(), e_gsc_errorcode_Name((e_gsc_errorcode)res.error_code()).c_str());
				res.PrintDebugString();

				//赠送物品
				c_gs_user_friend_op_req req;
				req.set_op_type(c_gs_user_friend_op_req::GET_ELEMENT1);
				req.set_user_id(2007);
				//send(stream, e_msg_c_gs_user_friend_op_req, &req);

				
													 }break;
			case c_gs_user_friend_op_req::GIVE_UP_APPLY1:{
				printf("取消申请好友结果[%d][%s]\n",  res.error_code(), e_gsc_errorcode_Name((e_gsc_errorcode)res.error_code()).c_str());
				res.PrintDebugString();
														 }
		}
	}
	ASSERT(r);
	return true;
}


bool CClientForGS::handle_msg_leave_gs_res( uv_stream_t* stream, const char * buf, int len ){
	gs_c_leave_gs_res res;
	bool r = res.ParseFromArray(buf, len);
	if (r)
	{
		printf("离开GS结果[%d][%s] \n", res.error_code(), e_gsc_errorcode_Name((e_gsc_errorcode)res.error_code()).c_str());
		//res.PrintDebugString();
	}
	else{
		PRINTF_PBPARSE_ERROR(e_msg_gs_c_leave_gs_res);
		return false;
	}
	ASSERT(r);

	return true;
}

bool CClientForGS::handle_msg_gs_c_mail_ntf( uv_stream_t* stream, const char * buf, int len )
{
	gs_c_mail_ntf_array ntf;
	bool r = ntf.ParseFromArray(buf, len);
	if (r)
	{
		printf("收到邮件通知\n");
		ntf.PrintDebugString();

	}
	else
	{
		PRINTF_PBPARSE_ERROR(e_msg_gs_c_mail_ntf);
		return false;
	}
	ASSERT(r);
	return true;
}

bool CClientForGS::handle_msg_mission_ntf( uv_stream_t* stream, const char * buf, int len )
{
	gs_c_mission_ntf_array ntf;
	bool r = ntf.ParseFromArray(buf, len);
	if (r)
	{
		printf("收到任务通知\n");
		//ntf.PrintDebugString();
	}
	else
	{
		PRINTF_PBPARSE_ERROR(e_msg_gs_c_mission_ntf);
		return false;
	}
	ASSERT(r);
	return true;
}

bool CClientForGS::handle_msg_marquee_ntf( uv_stream_t* stream, const char * buf, int len )
{
	gs_c_marquee_ntf ntf;
	bool r = ntf.ParseFromArray(buf, len);
	if (r)
	{
		printf("收到跑马灯通知\n");
		ntf.PrintDebugString();
	}
	else
	{
		PRINTF_PBPARSE_ERROR(e_msg_gs_c_marquee_ntf);
		return false;
	}
	ASSERT(r);
	return true;
}

bool CClientForGS::handle_msg_user_friend_ntf( uv_stream_t* stream, const char * buf, int len )
{
	gs_c_friend_ntf ntf;
	bool r = ntf.ParseFromArray(buf, len);
	if (r)
	{
		printf("收到好友通知\n");
		ntf.PrintDebugString();

		c_gs_user_friend_op_req req;
		req.set_op_type(c_gs_user_friend_op_req::AFFIRM_REFUSE1);
		req.set_user_id(2021);
		send(stream, e_msg_c_gs_user_friend_op_req, &req);
	}
	else
	{
		PRINTF_PBPARSE_ERROR(e_msg_gs_c_user_friend_ntf);
		return false;
	}
	ASSERT(r);
	return true;
}

bool CClientForGS::handle_msg_chat_ntf( uv_stream_t* stream, const char * buf, int len )
{
	gs_c_chat_ntf ntf;
	bool r = ntf.ParseFromArray(buf, len);
	if (r)
	{
		printf("收到聊天通知\n");
		ntf.PrintDebugString();
	}
	else
	{
		PRINTF_PBPARSE_ERROR(e_msg_gs_c_chat_ntf);
		return false;
	}
	ASSERT(r);
	return true;
}

bool CClientForGS::handle_msg_chat_res( uv_stream_t* stream, const char * buf, int len )
{
	gs_c_chat_res res;
	bool r = res.ParseFromArray(buf, len);
	if (r)
	{
		printf("聊天结果[%d][%s]\n",  res.error_code(), e_gsc_errorcode_Name((e_gsc_errorcode)res.error_code()).c_str());
		res.PrintDebugString();

		c_gs_rank_req req;
		req.set_rank_type(c_gs_rank_req::ARENA);
		send(stream, e_msg_c_gs_rank_req, &req);
	}
	else{
		PRINTF_PBPARSE_ERROR(e_msg_gs_c_chat_res);
		return false;
	}
	ASSERT(r);
	return true;
}

bool CClientForGS::handle_msg_rank_res( uv_stream_t* stream, const char * buf, int len )
{
	gs_c_rank_res res;
	bool r = res.ParseFromArray(buf, len);
	if (r)
	{
		printf("获取排行结果[%d][%s]\n",  res.error_code(), e_gsc_errorcode_Name((e_gsc_errorcode)res.error_code()).c_str());
		res.PrintDebugString();

		/*c_gs_fight_power_req req;
		req.set_fight_power(10000);
		send(stream, e_msg_c_gs_fight_power_req, &req);*/
		
		c_gs_crusade_start_req req;
		req.set_wave(1);
		req.add_enemy_heros(1);
		req.add_enemy_heros(2);
		req.add_enemy_heros(3);
		req.add_enemy_heros(4);
		req.add_enemy_heros(5);

		req.add_me_heros(4);
		req.add_me_heros(7);
		req.add_me_heros(10);

		send(stream, e_msg_c_gs_crusade_start_req, &req);

	}
	else{
		PRINTF_PBPARSE_ERROR(e_msg_gs_c_rank_res);
		return false;
	}
	ASSERT(r);
	return true;
}

bool CClientForGS::handle_msg_crusade_start_res( uv_stream_t* stream, const char * buf, int len )
{
	gs_c_crusade_start_res res;
	bool r = res.ParseFromArray(buf, len);
	if (r)
	{
		printf("开始远征结果[%d][%s]\n",  res.error_code(), e_gsc_errorcode_Name((e_gsc_errorcode)res.error_code()).c_str());
		res.PrintDebugString();

		/*c_gs_fight_power_req req;
		req.set_fight_power(10000);
		send(stream, e_msg_c_gs_fight_power_req, &req);*/

		c_gs_crusade_end_req req;
		req.set_is_win(false);
		user_crusade::crusade_hero* hero =  req.add_me_heros();
		hero->set_hero_id(4);
		hero->set_hp(0);
		hero->set_ao_yi(10);
		hero->set_bi_sha(10);

		hero = req.add_me_heros();
		hero->set_hero_id(7);
		hero->set_hp(0);
		hero->set_ao_yi(20);
		hero->set_bi_sha(20);

		hero = req.add_me_heros();
		hero->set_hero_id(10);
		hero->set_hp(0);
		hero->set_ao_yi(30);
		hero->set_bi_sha(30);

		//输了需要发送敌人信息
		hero = req.add_enemy_heros();
		hero->set_hero_id(100);
		hero->set_hp(100);
		hero->set_ao_yi(200);
		hero->set_bi_sha(300);
		
		hero = req.add_enemy_heros();
		hero->set_hero_id(101);
		hero->set_hp(100);
		hero->set_ao_yi(200);
		hero->set_bi_sha(300);
		
		hero = req.add_enemy_heros();
		hero->set_hero_id(102);
		hero->set_hp(100);
		hero->set_ao_yi(200);
		hero->set_bi_sha(300);

		send(stream, e_msg_c_gs_crusade_end_req, &req);

	}
	else{
		PRINTF_PBPARSE_ERROR(e_msg_gs_c_crusade_start_res);
		return false;
	}
	ASSERT(r);
	return true;
}

bool CClientForGS::handle_msg_crusade_end_res( uv_stream_t* stream, const char * buf, int len )
{
	gs_c_crusade_start_res res;
	bool r = res.ParseFromArray(buf, len);
	if (r)
	{
		printf("结束远征结果[%d][%s]\n",  res.error_code(), e_gsc_errorcode_Name((e_gsc_errorcode)res.error_code()).c_str());
		res.PrintDebugString();

		c_gs_crusade_hero_life_req req;
		req.set_hero_id(4);

		send(stream, e_msg_c_gs_crusade_hero_life_req, &req);

	}
	else{
		PRINTF_PBPARSE_ERROR(e_msg_gs_c_crusade_end_res);
		return false;
	}
	ASSERT(r);
	return true;
}

bool CClientForGS::handle_msg_crusade_hero_life_res( uv_stream_t* stream, const char * buf, int len )
{
	gs_c_crusade_hero_life_res res;
	bool r = res.ParseFromArray(buf, len);
	if (r)
	{
		printf("英雄复活结果结果[%d][%s]\n",  res.error_code(), e_gsc_errorcode_Name((e_gsc_errorcode)res.error_code()).c_str());
		res.PrintDebugString();

		c_gs_crusade_reset_req req;

		send(stream, e_msg_c_gs_crusade_reset_req, &req);

	}
	else{
		PRINTF_PBPARSE_ERROR(e_msg_gs_c_crusade_hero_life_res);
		return false;
	}
	ASSERT(r);
	return true;
}

bool CClientForGS::handle_crusade_reset_res( uv_stream_t* stream, const char * buf, int len )
{
	gs_c_crusade_reset_res res;
	bool r = res.ParseFromArray(buf, len);
	if (r)
	{
		printf("重置远征[%d][%s]\n",  res.error_code(), e_gsc_errorcode_Name((e_gsc_errorcode)res.error_code()).c_str());
		res.PrintDebugString();
	}
	else{
		PRINTF_PBPARSE_ERROR(e_msg_gs_c_crusade_reset_res);
		return false;
	}
	ASSERT(r);
	return true;
}
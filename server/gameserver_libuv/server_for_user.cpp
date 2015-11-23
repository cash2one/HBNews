#include <stdio.h>

#include "def.h"
#include "net_config.h"
#include "libuv_helper.h"
#include "time_helper.h"
#include "str.h"
//#include "db_async.h"

#include "server_for_user.h"
#include "client_for_as.h"
#include "client_for_dbs.h"
#include "client_for_ps.h"

#include "game_config.h"
#include "client_config_mgr.h"

#include "client_id.h"

#include "user_mgr.h"
#include "task_mgr.h"
#include "hero_mgr.h"
#include "item_mgr.h"
#include "equip_mgr.h"
#include "skill_mgr.h"
#include "vip_mgr.h"
#include "invoke_mgr.h"
#include "sys_setting_mgr.h"
#include "burst_mgr.h"
#include "arena.h"
#include "mail_mgr.h"
#include "mission_mgr.h"
#include "marquee_mgr.h"
#include "rank_mgr.h"
#include "server_for_gm.h"

static int g_max_client_num = 10000;



bool CServerForUser::Init()
{
	ASSERT(add_message_handle(e_msg_c_gs_enter_gs_req, handle_msg_enter_gs_req));
	ASSERT(add_message_handle(e_msg_c_gs_config_req, handle_msg_config_req));
	ASSERT(add_message_handle(e_msg_c_gs_user_info_req, handle_msg_user_info_req));
	ASSERT(add_message_handle(e_msg_c_gs_start_sub_task_req, handle_msg_start_sub_task_req));
	ASSERT(add_message_handle(e_msg_c_gs_end_sub_task_req, handle_msg_end_sub_task_req));
	ASSERT(add_message_handle(e_msg_c_gs_invoke_hero_req, handle_msg_invoke_hero_req));
	ASSERT(add_message_handle(e_msg_c_gs_leave_gs_req, handle_msg_leave_gs_req));
	ASSERT(add_message_handle(e_msg_c_gs_equip_level_up_req, handle_msg_equip_level_up_req));
	ASSERT(add_message_handle(e_msg_c_gs_equip_evolution_req, handle_msg_equip_evolution_req));
	ASSERT(add_message_handle(e_msg_c_gs_sell_req, handle_msg_sell_req));
	ASSERT(add_message_handle(e_msg_c_gs_fragment_compound_req, handle_msg_fragment_compound_req));
	ASSERT(add_message_handle(e_msg_c_gs_mail_opt_req, handle_msg_mail_opt_req));
	ASSERT(add_message_handle(e_msg_c_gs_hero_evolution_req, handle_msg_hero_evolution_req));

	ASSERT(add_message_handle(e_msg_c_gs_hero_star_up_req, handle_msg_hero_star_up_req));
	ASSERT(add_message_handle(e_msg_c_gs_skill_level_up_req, handle_msg_skill_level_up_req));
	ASSERT(add_message_handle(e_msg_c_gs_buy_req, handle_msg_buy_req));
	ASSERT(add_message_handle(e_msg_c_gs_invoke_req, handle_msg_invoke_req));
	ASSERT(add_message_handle(e_msg_c_gs_server_time_req, handle_msg_server_time_req));
	//ASSERT(add_message_handle(e_msg_c_gs_get_activity_task_req, handle_msg_get_activity_task_req));
	ASSERT(add_message_handle(e_msg_c_gs_use_item_req, handle_msg_use_item_req));
	ASSERT(add_message_handle(e_msg_c_gs_diamond_speed_kill_req, handle_msg_diamond_speed_kill_req));
	//�̵����Э��
	ASSERT(add_message_handle(e_msg_c_gs_shop_buy_item_req, handle_msg_shop_buy_item_req));
	ASSERT(add_message_handle(e_msg_c_gs_shop_refresh_req, handle_msg_shop_refresh_req));
	ASSERT(add_message_handle(e_msg_c_gs_shop_info_req, handle_msg_shop_info_req));
	ASSERT(add_message_handle(e_msg_c_gs_get_mission_reward_req, handle_msg_get_mission_reward_req));
	// ���������Э��
	ASSERT(add_message_handle(e_msg_c_gs_arena_refresh_match_req, handle_msg_arena_refresh_match_req));
	ASSERT(add_message_handle(e_msg_c_gs_arena_update_team_req, handle_msg_arena_update_team_req));
	ASSERT(add_message_handle(e_msg_c_gs_arena_start_challenge_req, handle_msg_arena_start_challenge_req));
	ASSERT(add_message_handle(e_msg_c_gs_arena_end_challenge_req, handle_msg_arena_end_challenge_req));
	ASSERT(add_message_handle(e_msg_c_gs_arena_challenge_reset_cd_req, handle_msg_arena_challenge_reset_cd_req));
	ASSERT(add_message_handle(e_msg_c_gs_arena_challenge_reset_num_req, handle_msg_arena_challenge_reset_num_req));
	ASSERT(add_message_handle(e_msg_c_gs_arena_ranks_req, handle_msg_arena_ranks_req));
	//ǩ��
	ASSERT(add_message_handle(e_msg_c_gs_sign_req, handle_msg_sign_req));
	ASSERT(add_message_handle(e_msg_c_gs_sign_add_req, handle_msg_sign_add_req));
	ASSERT(add_message_handle(e_msg_c_gs_arena_search_req, handle_msg_arena_search_req));
	//ͷ��ͷ���
	ASSERT(add_message_handle(e_msg_c_gs_head_or_frame_change_req, handle_msg_head_or_frame_change_req));
	//��������
	ASSERT(add_message_handle(e_msg_c_gs_finish_novice_guide_req, handle_msg_finish_novice_guide_req));
	//�༭�����Ϣ
	ASSERT(add_message_handle(e_msg_c_gs_edit_player_info_req, handle_msg_edit_player_info_req));

	//�汾��Ϣ �ͻ���������Ϣ
	ASSERT(add_message_handle(e_msg_c_gs_json_config_version_req, handle_msg_json_config_version_req));
	ASSERT(add_message_handle(e_msg_c_gs_json_config_req, handle_msg_json_config_req));
	//����
	ASSERT(add_message_handle(e_msg_c_gs_user_friend_op_req, handle_msg_user_friend_op_req));
	ASSERT(add_message_handle(e_msg_c_gs_search_user_req, handle_msg_search_user_req));
	
	//����
	ASSERT(add_message_handle(e_msg_c_gs_chat_req, handle_msg_chat_req));
	
	//����
	ASSERT(add_message_handle(e_msg_c_gs_fight_power_req, handle_msg_fight_power_req));
	ASSERT(add_message_handle(e_msg_c_gs_rank_req, handle_msg_rank_req));

	//Զ��
	ASSERT(add_message_handle(e_msg_c_gs_crusade_start_req, handle_msg_crusade_start_req));
	ASSERT(add_message_handle(e_msg_c_gs_crusade_end_req, handle_msg_crusade_end_req));
	ASSERT(add_message_handle(e_msg_c_gs_crusade_hero_life_req, handle_msg_crusade_hero_life_req));
	ASSERT(add_message_handle(e_msg_c_gs_crusade_reset_req, handle_msg_crusade_reset_req));
	ASSERT(add_message_handle(e_msg_c_gs_crusade_promote_award_req, handle_msg_crusade_promote_award_req));
	ASSERT(add_message_handle(e_msg_c_gs_crusade_buff_refresh_req, handle_msg_crusade_buff_refresh_req));

	return true;
}

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

	int send_buffer_size = 1024*1024*4;//4M�ķ��Ϳռ䣡
	r = uv_send_buffer_size((uv_handle_t*)&server_for_user->m_server, &send_buffer_size);
	ASSERT(r == 0);

	int ret_send_buffer_size = 0;
	r = uv_send_buffer_size((uv_handle_t*)&server_for_user->m_server, &ret_send_buffer_size);
	ASSERT(r == 0);

	server_for_user->m_server.data = server_for_user;
	
	create_timer();

	r = uv_listen((uv_stream_t*)&server_for_user->m_server, g_max_client_num, on_client_connection);
	ASSERT(r == 0);
	printf("ServerForUser listen %s:%d  success !\n", ip, port);

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
	//�����ֽ���ת���������ַ���  
	uv_ip4_name(&sock_addr, (char*)ip, sizeof ip);  
	printf("getsockname   %s : %d \n", ip, sock_addr.sin_port);


	//�����ӣ����Ի��Ŀ���ip�Ͷ˿�  
	namelen = sizeof peername;  
	r = uv_tcp_getpeername(handle, &peername, &namelen);  
	//printf("the r is %d.\n", r);
	ASSERT( r == 0);

	memset(ip, 0, sizeof ip);
	sock_addr = *(struct sockaddr_in*) &peername; 
	//�����ֽ���ת���������ַ���  
	uv_ip4_name(&sock_addr, (char*)ip, sizeof ip);  
	printf("getpeername  %s : %d \n", ip, sock_addr.sin_port);

	//todo net_protect��һЩ���ƴ���

	printf("new connect accepted! \n");
	r = uv_read_start((uv_stream_t*)handle, alloc_cb, on_receive);
	ASSERT(r == 0);

	CServerForUser::GetInstance()->add_client(handle);
}

void CServerForUser::on_receive( uv_stream_t* stream, ssize_t nread, const uv_buf_t* buf )
{
	int r;
	//todo net_protect��һЩ���ƴ���
	if (nread > 0){
		CServerForUser::GetInstance()->process_msg(stream, buf->base, nread);
		free(buf->base);//�ͷ��ڴ�
	}
	else if (nread == 0)//OK, but nothing read
	{

	}
	else{

		//ASSERT(uv_last_error(stream->loop).code == UV_EOF);

		if (nread == UV_EOF) {
			printf("������������Ͽ�\n");

		} else if (nread == UV_ECONNRESET) {
			printf("��������쳣�Ͽ�\n");

		} else {
			//printf("%s : %s \n", uv_err_name(nread), uv_strerror(nread));
			printf("��������쳣\n");
			PRINTF_UV_ERROR(nread);
		}

		shutdown_client(stream);
	}
}

void CServerForUser::shutdown_client( uv_stream_t* stream )
{
	uv_shutdown_t* req = (uv_shutdown_t*)req_alloc();
	int r = uv_shutdown(req, stream, after_client_shutdown);//����� ��
	//ASSERT(r == 0);
	if( r!= 0){
		printf("������� uv_shutdown�쳣 !!! \n");
		req_free((uv_req_t*)req);

		///uv_close((uv_handle_t*)stream, after_client_close);//����� ��assert 0
		//after_client_close((uv_handle_t*)stream);//delete uv_tcp_t*  end game error!

		CUserConnectContext* ctx = CServerForUser::GetInstance()->get_context_on_client((uv_tcp_t*)stream);
		if (!ctx)
		{
			printf("\n\n!!! ����û��ctx����\n\n");
			//todo�� Ҫ��Ҫ
			return ;
		}

		printf("\n\n!!! ���һ�����ctx����\n\n");

		bool ret = CServerForUser::GetInstance()->remove_client(ctx->ConnectID, false);
		//bool ret = CServerForUser::GetInstance()->remove_client(ctx->ConnectID);//�����ͷ��ڴ�ᵼ�±�ĵط������õ� �������
		if (!ret)//û���ҵ���ʱ��
		{
			printf("\n\n!!! remove_client����ʧ�ܣ���\n\n");
			ret = CServerForUser::GetInstance()->detach_context_on_client((uv_tcp_t*)stream);
			ASSERT(ret);
		}
		else{
			printf("\n\n!!! remove_client���ӳɹ�����\n\n");
		}
	}
	else{
		printf("������� uv_shutdown�ɹ��� !!! \n");
	}
}

void CServerForUser::after_client_shutdown( uv_shutdown_t* req, int status )
{
	ASSERT(req);
	ASSERT(req->handle);
	uv_close((uv_handle_t*)req->handle, after_client_close);
	req_free((uv_req_t*)req);
}

void CServerForUser::after_client_close( uv_handle_t* handle )
{
	//uv_tcp_t* client = (uv_tcp_t*)handle;
	//ASSERT(client);

	ASSERT(handle);
	//CUserConnectContext* ctx = (CUserConnectContext*)handle->data;
	CUserConnectContext* ctx = CServerForUser::GetInstance()->get_context_on_client((uv_tcp_t*)handle);
		
	//ASSERT(ctx);
	//����ط� ��onreceive��һ�� shutdown����tcp���Ӳ�����endgame���� ���ǻ�û�����ü�����after_client_shutdown-> after_client_close->remove_client
	//Ȼ������ڶ����ֵ�����onreceive ���ʱ����uv_shutdown��ʧ�ܵ� Ȼ��ֱ�ӵ���remove_client
	//����ÿ�ζ�Ҫ�ж�ctx�Ƿ�OK
	if(ctx){
		//delete client;
		//ASSERT(CServerForUser::GetInstance()->remove_client(ctx->ConnectID));
		bool r = CServerForUser::GetInstance()->remove_client(ctx->ConnectID);
		if(!r){
			printf("after_client_close���ӶϿ���������ʧ�ܣ�after_client_close-> CServerForUser::GetInstance()->remove_client����\n");
		}
		printf("after_client_close���ӶϿ���������ɹ����ͷ����������� ����ɾ��tcp_t������\n");
	}
	else{
		delete (uv_tcp_t*)handle;//�ܵ����� ˵��֮ǰremove_client��ʱ���ͷ�tcp����Ϊlibuv�ڲ����ܻ����õ��� �������һ�ν�����ʱ����ͷŵ�
		printf("after_client_close ������û�������ģ�����\n");
	}
}

bool CServerForUser::send( uv_stream_t* stream, UINT32 msg_id, google::protobuf::Message* pMessage/*, uv_write_cb write_cb*/ )
{
	ASSERT(uv_is_writable(stream));
	if(!uv_is_writable(stream)){
		printf("uv_is_writable false!!! CServerForUser start send failed! msgid[%d][%s] \n", msg_id, e_msg_gs_c_Name((e_msg_gs_c)msg_id).c_str());
		return false;
	}

	ASSERT(msg_id > e_msg_gs_c_min && msg_id < e_msg_gs_c_max);
	uv_buf_t ret_buf;
	UINT32 body_len = pMessage->ByteSize();
	//UINT32 head_len = sizeof(UINT32) + sizeof(UINT32);//4ֱ�ӵİ����� + 4�ֽڵ���Ϣid
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

	//ASSERT(r == 0);//�ͻ��˿����ڷ�������û�з��͵�ʱ��Ͽ������ӣ�
	if (r != 0)
	{
		PRINTF_UV_ERROR(r);
		printf("CServerForUser start send failed! msgid[%d][%s]  bytes len[%d] \n", msg_id, e_msg_gs_c_Name((e_msg_gs_c)msg_id).c_str(), PACKET_HEAD_LEN + body_len);
		return false;
	}
	else{
		printf("CServerForUser start send success! msgid[%d][%s]  bytes len[%d] \n", msg_id, e_msg_gs_c_Name((e_msg_gs_c)msg_id).c_str(), PACKET_HEAD_LEN + body_len);
		//pMessage->PrintDebugString();
		return true;
	}
}

bool CServerForUser::send( uv_stream_t* stream, BYTE* buf, UINT32 buf_len )
{
	ASSERT(uv_is_writable(stream));
	ASSERT(buf && buf_len >= PACKET_HEAD_LEN);

	uv_buf_t ret_buf;
	ret_buf.base = (char*)buf;
	ret_buf.len = buf_len;
	
	UINT32 msg_id = *(((UINT32*)buf)+1);
	ASSERT(msg_id > e_msg_gs_c_min && msg_id < e_msg_gs_c_max);

	uv_write_t* write_req = (uv_write_t*)req_alloc();
	
	int r = uv_write(write_req, stream, &ret_buf, 1, after_send);
	ASSERT(r == 0);
	if (r != 0)
	{
		//printf("%s:%s", uv_err_name(r), uv_strerror(r));
		PRINTF_UV_ERROR(r);
	}
	else{
		printf("CServerForUser::send2  msgid[%d]  bytes len[%d] \n", msg_id, buf_len);
	}

	return true;
}
void CServerForUser::after_send( uv_write_t* req, int status )
{
	ASSERT(req != NULL);
	if (status == 0)
	{
		//write_cb_called++;
		UINT32 msg_id = (UINT32)(req->data);
		printf("CServerForUser::after_send ��user������Ϣmsgid[%d][%s]�ɹ�\n", msg_id, e_msg_gs_c_Name((e_msg_gs_c)msg_id).c_str());
	}
	else{
		if (status == UV_EOF) {
			printf("дʱ, ������������Ͽ�\n");

		} else if (status == UV_ECONNRESET) {
			printf("дʱ, ��������쳣�Ͽ�\n");

		} else {
			//printf("%s : %s \n", uv_err_name(status), uv_strerror(status));
			printf("дʱ, ��������쳣\n");
			PRINTF_UV_ERROR(status);
		}

		uv_shutdown_t* shut_r = (uv_shutdown_t*)req_alloc();
		int r = uv_shutdown(shut_r, req->handle, after_client_shutdown);
		ASSERT(r == 0);
	}

	req_free((uv_req_t*)req);
}

//void CServerForUser::timer_cb(uv_timer_t* handle) {
//	//printf("\n timer_cb!!\n");
//
//	ASSERT(handle != NULL);
//	ASSERT(1 == uv_is_active((uv_handle_t*) handle));
//
//	ASSERT(handle == &CServerForUser::GetInstance()->m_timer);
//	//repeat_cb_called++;
//	//if (repeat_cb_called == 5) {
//	//	uv_close((uv_handle_t*)handle, repeat_close_cb);
//	//}
//
//	UINT32 now = Now();
//	CArena::GetInstance()->OnTime(now);
//}

bool CServerForUser::process_msg( uv_stream_t* stream, const char * buf, int len )
{
	ASSERT(buf);
	ASSERT(len >= PACKET_HEAD_LEN);//
	int pos = 0;
	int bytes_left = len;


	////���û�е�¼ ����login��Ϣ ������Ϣ������
	//stConnectUserData* pUserData = (stConnectUserData*)stream->data;
	//ASSERT(pUserData);
	//UINT32 first_msg_id = *(UINT32*)(buf+4);
	////todo delete ���ز��� ��ʱ������½��֤
	//if (false && !pUserData->iUserID && first_msg_id != e_msg_c_gs::e_msg_c_gs_login_req)//��һ����Ϣ������ע��
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
		ASSERT(msg_id < e_msg_c_gs_max);
		p += sizeof msg_id;

		func_message_handle pMsgHandle = m_mapHandle[msg_id];
		if (pMsgHandle)
		{
			bool r = pMsgHandle(stream, p, msg_len - sizeof(msg_len) - sizeof(msg_id));
		}
		else{
			printf("CServerForUser::process_msgû���ҵ�msdid[%d][%s]����Ϣ������\n", msg_id, e_msg_c_gs_Name(e_msg_c_gs(msg_id)).c_str());
			//return false;
		}

		p += msg_len - sizeof(msg_len) - sizeof(msg_id);
	}

	return true;
}

bool CServerForUser::add_message_handle( e_msg_c_gs msgID, func_message_handle handle )
{
	if (m_mapHandle[msgID])
	{
		printf("already exsit handle for msgid[%u][%s] \n", msgID, e_msg_c_gs_Name(msgID));
		return false;
	}

	m_mapHandle[msgID] = handle;
	return true;
}


CUserConnectContext* CServerForUser::attach_context_on_client( uv_tcp_t* client ){
	CUserConnectContext* ctx = new CUserConnectContext;
	ASSERT(ctx);
	ctx->iUserID = NULL;
	ctx->stauts = CUserConnectContext::EUserConnectStatus_Connected;
	client->data = ctx;
	return ctx;
}

CUserConnectContext* CServerForUser::get_context_on_client( uv_tcp_t* client ){
	ASSERT(client);
	CUserConnectContext* ctx = (CUserConnectContext* )client->data;
	//ASSERT(ctx);
	return ctx;
}

bool CServerForUser::detach_context_on_client( uv_tcp_t* client ){
	ASSERT(client);
	CUserConnectContext* ctx = (CUserConnectContext* )client->data;
	ASSERT(ctx);

	ctx->iUserID = NULL;
	ctx->stauts = CUserConnectContext::EUserConnectStatus_None;

	delete ctx;
	client->data = NULL;

	return true;
}

bool CServerForUser::add_client( uv_tcp_t* client )
{
	ASSERT(client);
	//CUserConnectContext* ctx = (CUserConnectContext* )client->data;
	//ASSERT(ctx && ctx->iUserID && ctx->EUserConnectStatus_Logined);
	//ASSERT(m_clients.find(ctx->iUserID) == m_clients.end());
	//m_clients[ctx->iUserID] = client;

	Guid connect_id = GetGuidGenerator().newGuid();
	ASSERT(m_clients.find(connect_id) == m_clients.end());

	CUserConnectContext* ctx = attach_context_on_client(client);
	ASSERT(ctx);
	ctx->ConnectID = connect_id;

	m_clients[connect_id] = client;

	return true;
}

bool CServerForUser::remove_client( Guid& connect_id , bool free_tcp_t/* = true*/)
{
	//std::map<UINT32, uv_tcp_t*>::iterator it =  m_clients.find(key);
	//if (it != m_clients.end())
	//{
	//	ASSERT(it->second);
	//	//CUserContext* ctx = (CUserContext*)it->second->data;
	//	//ASSERT(ctx);
	//	//delete ctx;//ɾ��client�����Ϣ
	//	//it->second->data = NULL;

	//	ASSERT(it->second->data == NULL);//ȷ��context�Ѿ��ͷ�

	//	delete it->second;//ɾ��client
	//	m_clients.erase(it);
	//	return true;
	//}
	//else{
	//	return false;
	//}

	std::map<Guid, uv_tcp_t*>::iterator it =  m_clients.find(connect_id);
	if (it != m_clients.end())
	{
		ASSERT(it->second);
		
		//>>>�����������Ѿ���ʼ��������� �ͽ��
		CUserConnectContext* ctx = (CUserConnectContext*)it->second->data;
		ASSERT(ctx);
		//delete ctx;//ɾ��client�����Ϣ
		//it->second->data = NULL;

		if (ctx->iUserID){//�����id��ʱ��
			ASSERT(ctx->stauts == CUserConnectContext::EUserConnectStatus_Logined);
			CUser * pUser = CUserMgr::GetInstance()->GetUserByUserID(ctx->iUserID);
			if(pUser){
				ASSERT(pUser->GetConnect() == (uv_stream_t*)it->second);
				pUser->OnLeave();
				//pUser->UnIntertwineConnect((uv_stream_t*)it->second);
			}
		}
		//<<<

		detach_context_on_client(it->second);

		if(free_tcp_t){
			delete it->second;//ɾ��client
		}
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
	ASSERT(handle != NULL);
	free((uv_async_t*)handle);
}

void CServerForUser::after_async_close( uv_handle_t* handle )
{
	ASSERT(handle != NULL);
	//free((uv_async_t*)handle);
	CServerForUser::GetInstance()->DestroyAsync((uv_async_t*)handle);
}

void CServerForUser::async_cb( uv_async_t* handle )
{
	ASSERT(handle);
	CServerForUserAsync* asyncData = (CServerForUserAsync*)handle->data;
	ASSERT(asyncData);
	//do something
	switch (asyncData->op)
	{
		case CServerForUserAsync::EAsyncOP_Check_Token_Res:{

			uv_tcp_t* con =  CServerForUser::GetInstance()->get_client(asyncData->user_connect_id);
			if (con)//�ص����� �ͻ������ӻ���
			{
				gs_c_enter_gs_res res;
				if (asyncData->user_id)//��֤��½token�ɹ� ȷʵ������û� 
				{
					//�жϵ�ǰ�û��Ƿ��Ѿ������������� ����Ѿ����� �����ߣ�������ô�ģ�!
					CUser* pUser = CUserMgr::GetInstance()->GetUserByUserID(asyncData->user_id);
					if(pUser){

						if(pUser->HasConnect()){
							//һ��������ͬһ������  ����ͬһ������ token��һ�������ɾ���� �ڶ��ε���Ҳ���ᵽ���λ����
							ASSERT( pUser->GetConnect() != (uv_stream_t*)con);
							UINT32 Switch_Allow_Same_User_Enter = 0;
							bool r = CSysSettingMgr::GetInstance()->GetIntValue("Switch_Allow_Same_User_Enter", Switch_Allow_Same_User_Enter);
							ASSERT(r);

							if (!Switch_Allow_Same_User_Enter){
								printf("���[%u]��Ϣ��ǰ���ڱ�����[%p]ʹ�ã�����[%p]������enter_gs, ���ҶϿ������ӣ�\n", pUser->UserID(), pUser->GetConnect(), con);
								res.set_error_code(e_gsc_errorcode_user_using_by_another_connect);
								
								//���� Ȼ��ر�
								bool r = send((uv_stream_t*)con, e_msg_gs_c_enter_gs_res, &res);
								ASSERT(r);

								shutdown_client((uv_stream_t*)con);

								goto END;
							}
							else{// (todo!!����ǰһ����� �ѱ���������) Ȼ�� �Ͽ��ͻ�������
								printf("���[%u]��Ϣ��ǰ���ڱ�����[%p]ʹ�ã�����[%p]enter_gs, ���ҶϿ�������[%p]��\n", pUser->UserID(), pUser->GetConnect(), con, pUser->GetConnect());
								CUserConnectContext* ctx_pre = CServerForUser::GetInstance()->get_context_on_client((uv_tcp_t*)pUser->GetConnect());
								ASSERT(ctx_pre && ctx_pre->iUserID);
								ctx_pre->iUserID = 0;//Ϊ��shutdown�ɵ����ӵ�ʱ����������Ϣ
								//ctx->ConnectID = NULL;//
								ctx_pre->stauts = CUserConnectContext::EUserConnectStatus_Connected;//

								shutdown_client(pUser->GetConnect());

								CUserConnectContext* ctx_new = CServerForUser::GetInstance()->get_context_on_client(con);
								ctx_new->iUserID = asyncData->user_id;
								ctx_new->stauts = CUserConnectContext::EUserConnectStatus_Logined;

								pUser->IntertwineConnect((uv_stream_t*)con);

								res.set_error_code(e_gsc_errorcode_success);
								//res.set_proto_version(e_gsc_proto_version(2));
								res.set_proto_version(GSC_VERSION);
								res.set_server_timestamp(Now());
							}
						}
						else{//�û���Ϣ���� ���ǵ�ǰû������ ��ô����ֱ�Ӱ�
							
							pUser->IntertwineConnect((uv_stream_t*)con);

							//��ô�ҵ�����û��������أ������봫connect_id
							CUserConnectContext* ctx = (CUserConnectContext*)con->data;
							ASSERT(ctx);
							ctx->iUserID = asyncData->user_id;
							ctx->stauts = CUserConnectContext::EUserConnectStatus_Logined;

							res.set_error_code(e_gsc_errorcode_success);
							//res.set_proto_version(e_gsc_proto_version(2));
							res.set_proto_version(GSC_VERSION);
							res.set_server_timestamp(Now());
						}
					}
					else{
						//��ô�ҵ�����û��������أ������봫connect_id
						CUserConnectContext* ctx = (CUserConnectContext*)con->data;
						ASSERT(ctx);
						ctx->iUserID = asyncData->user_id;
						ctx->stauts = CUserConnectContext::EUserConnectStatus_Logined;

						res.set_error_code(e_gsc_errorcode_success);
						//res.set_proto_version(e_gsc_proto_version(2));
						res.set_proto_version(GSC_VERSION);
						res.set_server_timestamp(Now());
					}
				}
				else{//token��֤ʧ�� û�е�½
					res.set_error_code(e_gsc_errorcode_not_logined);
				}

				bool r = send((uv_stream_t*)con, e_msg_gs_c_enter_gs_res, &res);
				ASSERT(r);
			}
			else{//�ص����� �ͻ��������Ѿ�������
				printf("CServerForUser::async_cb �ص����� �ͻ��������Ѿ������ˣ� \n");
			}
		}break;

	case CServerForUserAsync::EAsyncOP_User_Info_Res:{

			uv_tcp_t* con =  CServerForUser::GetInstance()->get_client(asyncData->user_connect_id);
			dbs_gs_user_info_res* db_res = (dbs_gs_user_info_res*)asyncData->data;
			ASSERT(db_res);
			//bool should_delete_db_res = true;//Ĭ����Ҫɾ���� ֻ�д��������Ϣ֮�� ����������Ͽ��Բ�ɾ����
			if (con)//�ص����� �ͻ������ӻ���
			{
				gs_c_user_info_res res;

				//���������Ϣ�һ�ȡ�ɹ�
				if (db_res->error_code() == e_dbsgs_errorcode_success)
				{
					printf("gs��ȡdb�ϵ����[%u]��Ϣ�ɹ�\n", db_res->user_id());
					ASSERT(db_res->has_user_info());//����ʹ��mutable�ж� ��Ϊ������ʱ���ڲ���new��
					//should_delete_db_res = false;//db_res��ŵ�������� �������ﲻҪɾ����  delete userҲ���ͷŵ���

					//��� db_user_info����
					db_user_info* user_info = db_res->release_user_info();
				
					//����û���Ϣ��������
					CUser* pUser = new CUser;

					gs_dbs_user_info_op_req *db_req = new gs_dbs_user_info_op_req;
					db_req->set_user_id(user_info->user_base_info().user_id());

					//db_res��ɾ�� �������������
					if(!pUser->Init(user_info, *db_req)){
						printf("��ʼ�� �ִ��û� id[%u] CUser����ʧ�ܣ�\n", db_res->user_id());
						res.set_error_code(e_gsc_errorcode_unknown_error);
						delete pUser;//���������ͷ�db_res
						delete db_req;
					}
					else{
						if (!CUserMgr::GetInstance()->AddUser(pUser))
						{
							printf("CUserMgr��ӽ��û�id[%u]��Ϣʧ�� �Ѿ��д��û���\n", pUser->UserID());
							res.set_error_code(e_gsc_errorcode_unknown_error);
							delete pUser;//���������ͷ�db_res

						}
						else{
							pUser->IntertwineConnect((uv_stream_t*)con);

							

							bool r = pUser->Fill_gs_c_user_info_res(&res, *db_req);
							ASSERT(r);
							printf("CUserMgr��ӽ��û�id[%u]��Ϣ�ɹ�\n", pUser->UserID());
							res.set_error_code(e_gsc_errorcode_success);

							SendDBReq(db_req);//�޸�db��Ϣ
															

						}
						
					}
				}
				else if(db_res->error_code() == e_dbsgs_errorcode_user_info_not_exsit){//�����ڴ������Ϣ ��������ע��� ��û���½�������� ����Ҫ�½�
					printf("gs��ȡdb�ϵ����ʧ�ܣ� ���[%u]��Ϣ������ �����û�׼���½������Ϣ��gs �����͵�dbs��\n", db_res->user_id());
					ASSERT(!db_res->has_user_info());//����ʹ��mutable�ж� ��Ϊ������ʱ���ڲ���new��
					//should_delete_db_res = false;//db_res��ŵ�������� �������ﲻҪɾ���� delete userҲ���ͷŵ���

					//�½��û���Ϣ
					db_user_info* new_user_info = new db_user_info;
					ASSERT(new_user_info);
					bool r = CUserMgr::InitNewUser(new_user_info, db_res->user_id());
					ASSERT(r);

					//����û���Ϣ��������
					CUser* pUser =  new CUser;

					//�����ʼ�Ƹ� Ҳ���ܷ������� ��Ϊ��ʱ������û�кã�
					//r = pUser->GiveInitPlayerBelongings();
					//ASSERT(r);

					//��ɾ�� �������������
					gs_dbs_user_info_op_req *db_req = new gs_dbs_user_info_op_req;
					db_req->set_user_id(db_res->user_id());
					if (!pUser->Init(new_user_info, *db_req))
					{
						printf("��ʼ�� �½��û� id[%u] CUser����ʧ�ܣ�\n", db_res->user_id());
						res.set_error_code(e_gsc_errorcode_unknown_error);
						delete pUser;//���������ͷ�new_user_info
						delete db_req;
					}
					else{
						if (!CUserMgr::GetInstance()->AddUser(pUser))
						{
							printf("CUserMgr��ӽ��û�id[%u]��Ϣʧ�� �Ѿ��д��û���\n", pUser->UserID());
							res.set_error_code(e_gsc_errorcode_unknown_error);
							delete pUser;//���������ͷ�new_user_info
						}
						else{
							//������
							r = pUser->IntertwineConnect((uv_stream_t*)con);
							ASSERT(r);

							//�����ʼ�Ƹ� ���ܷ������� ����û�г�ʼ����������Ϣ����
							//r = pUser->GiveInitPlayerBelongings();
							//ASSERT(r);

							//�����ͻ��˵���Ϣ
							

							r = pUser->Fill_gs_c_user_info_res(&res, *db_req);
							ASSERT(r);

							printf("CUserMgr��ӽ��û�id[%u]��Ϣ�ɹ�\n", pUser->UserID());
							res.set_error_code(e_gsc_errorcode_success);

							r = CClientForDBS::GetInstance()->SendAsync(CClientForDBSAsync::EASyncOP_User_Info_New_Create_Req, pUser->UserID(), new_user_info);
							if (!r)
							{
								printf("GS ����̸߳�db�߳� �����½��������Ϣ[%u]���첽����ʧ�ܣ�GS����ȻҪ������CUser�ڴ���Ϣ��\n", pUser->UserID());
							}

							//���͸�����Ϣ
							SendDBReq(db_req);

						}//usermgr ���user����ɹ�
					}//user�����ʼ���ɹ�
				}//�����Ϣ������ �½������Ϣ
				else{
					res.set_error_code(e_gsc_errorcode_user_info_not_exsit_or_has_error);
					//e_gsc_errorcode_unknown_error
				}
				
				bool r = send((uv_stream_t*)con, e_msg_gs_c_user_info_res, &res);
				ASSERT(r);
			}
			else{//�ص����� �ͻ��������Ѿ�������
				printf("CServerForUser::async_cb �ص����� �ͻ��������Ѿ������ˣ� \n");
			}

			//if (should_delete_db_res)
			//{
			delete db_res;//NULLָ��deleteҲ�ǰ�ȫ��
			//}

		}break;
		case CServerForUserAsync::EAsyncOP_User_Edit_User_Name_Res:{
			CUser* pUser = CUserMgr::GetInstance()->GetUserByUserID(asyncData->user_id);
			if(pUser)
			{
				dbs_gs_edit_name_res* db_res = (dbs_gs_edit_name_res*)asyncData->data;
				ASSERT(db_res);

				gs_c_edit_player_info_res c_res;
				if(e_dbsgs_errorcode_success == db_res->error_code()){
					pUser->Name(db_res->name());
					c_res.set_error_code(e_gsc_errorcode_success);
					c_res.set_name(db_res->name());
				}
				else if(e_dbsgs_errorcode_user_edit_same_name == db_res->error_code()){
					c_res.set_error_code(e_gsc_errorcode_edit_player_info_name_same);	
				}
				else{
					c_res.set_error_code(e_gsc_errorcode_unknown_error);
				}
				pUser->Send(e_msg_gs_c_edit_player_info_res, &c_res);
			}
			else
			{
				printf("CServerForUser::async_cb �ص����� �ͻ��������Ѿ������ˣ� \n");
			}

		}break;
		//����gm�߳��ϲ��� ����user�߳��ϲ��� ��Ϊ���ܻ��õ�user������Ϣ��
		case CServerForUserAsync::EAsyncOP_GM_Add_Item:{
		
			gm_gs_add_item_req* gm_req = (gm_gs_add_item_req*)asyncData->data;
			ASSERT(gm_req);
			
			gs_gm_add_item_res* res = new gs_gm_add_item_res;
			CUser* pUser = CUserMgr::GetInstance()->GetUserByUserID(gm_req->user_id());
			if (pUser){//�������ʱ
				stGameElement ele(EELEMENTTYPE_ITEM, gm_req->item_id(), 0, stGameElement::INCREASE, gm_req->num());
				GS_C_NTF ntf;
				gs_dbs_user_info_op_req* db_req = new gs_dbs_user_info_op_req;
				pUser->UpdateGameElement(ele, *db_req, *ntf.mutable_element_ntf()->mutable_eles(), ntf);
				pUser->SendNtf(ntf);
				SendDBReq(db_req);
				res->set_error_code(e_gsgm_errorcode_success);
			}
			else{
				res->set_error_code(e_gsgm_errorcode_user_not_online);
			}
			delete gm_req;
			CServerForGM::GetInstance()->SendAsync(asyncData->user_connect_id, stServerForGMAsync::EAsyncOP_Add_Item_Res, res);
		}break;

//--- ����Ϊgm������������
//����gm�߳��ϲ��� ����user�߳��ϲ��� ��Ϊ���ܻ��õ�user������Ϣ��
		case CServerForUserAsync::EAsyncOP_GM_Marquee:{

			gm_gs_marquee_req* gm_req = (gm_gs_marquee_req*)asyncData->data;
			ASSERT(gm_req);

			bool r = CMarqueeMgr::GetInstance()->SendMarqueeToAllUsers((gs_c_marquee_ntf::MARQUEE_TYPE)gm_req->type(), gm_req->content().c_str());

			delete gm_req;

			gs_gm_marquee_res* gm_res = new gs_gm_marquee_res;
			gm_res->set_error_code( r ? e_gsgm_errorcode_success : e_gsgm_errorcode_unkown_error);
			CServerForGM::GetInstance()->SendAsync(asyncData->user_connect_id, stServerForGMAsync::EAsyncOP_Marquee_Res, gm_res);
		}break;

		case CServerForUserAsync::EAsyncOP_GM_Mail:{

			gm_gs_mail_req* gm_req = (gm_gs_mail_req*)asyncData->data;
			ASSERT(gm_req);

			//bool r = false;
			bool r = CMailMgr::GetInstance()->SendMailToUsers(gm_req);

			delete gm_req;

			gs_gm_mail_res* gm_res = new gs_gm_mail_res;
			gm_res->set_error_code( r ? e_gsgm_errorcode_success : e_gsgm_errorcode_unkown_error);
			CServerForGM::GetInstance()->SendAsync(asyncData->user_connect_id, stServerForGMAsync::EAsyncOP_Mail_Res, gm_res);
		}break;
		case CServerForUserAsync::EAsyncOP_New_Mail_Res:
			{
				dbs_gs_user_new_mail_res* new_mail_res = (dbs_gs_user_new_mail_res*)asyncData->data;
				ASSERT(new_mail_res);
				CMailMgr::GetInstance()->HandleNewMailRes(*new_mail_res);
				delete new_mail_res;
				new_mail_res = NULL;
			}
			break;
		case CServerForUserAsync::EAsyncOP_GM_Arena_Mail:
			{
				gm_gs_arena_mail_req* gm_req = (gm_gs_arena_mail_req*)asyncData->data;
				ASSERT(gm_req);
				bool r = CArena::GetInstance()->OnTime(Now());
				delete gm_req;

				gs_gm_arena_mail_res* res = new gs_gm_arena_mail_res;
				res->set_error_code(r ? e_gsgm_errorcode_success : e_gsgm_errorcode_unkown_error);
				CServerForGM::GetInstance()->SendAsync(asyncData->user_connect_id, stServerForGMAsync::EAsyncOP_Arena_Mail_Res, res);
			}
			break;
		case CServerForUserAsync::EAsyncOP_New_Arena_New_Mail_Res:
			{
				dbs_gs_user_new_arena_mail_res* res = (dbs_gs_user_new_arena_mail_res*)asyncData->data;
				ASSERT(res);
				CMailMgr::GetInstance()->HandleNewArenaMailRes(*res);
				delete res;
			}
			break;
		case CServerForUserAsync::EAsyncOP_Search_User_Res:{
				dbs_gs_search_user_res* res = (dbs_gs_search_user_res*)asyncData->data;
				ASSERT(res);
				UINT32 iUserID = res->user_id();
				CUser* pUser = CUserMgr::GetInstance()->GetUserByUserID(iUserID);
				if (pUser)
				{
					gs_c_search_user_res c_res;
					if(e_dbsgs_errorcode_user_info_not_exsit == res->error_code())
					{
						c_res.set_error_code(e_gsc_errorcode_friend_search_user_not_exist);
					}
					else if(e_dbsgs_errorcode_success == res->error_code()){
						c_res.set_error_code(e_gsc_errorcode_success);
						c_res.mutable_base_info()->CopyFrom(res->base_info());
						//����������Ϣ�洢����(�ȿͻ�����Ӻ��ѣ�ֱ�ӷ���id���)
						pUser->SearchUserRet(res->base_info());
					}
					else{
						c_res.set_error_code(e_gsc_errorcode_unknown_error);
					}
					pUser->Send(e_msg_gs_c_search_user_res, &c_res);
				}
				else{
					printf("EAsyncOP_Search_User_Res���ݿⷵ���û�id %d��������", iUserID);
				}
				delete res;
			}break;
		case CServerForUserAsync::EAsyncOP_Add_Friend_Res:{
			dbs_gs_friend_offline_res* res = (dbs_gs_friend_offline_res*)asyncData->data;
			ASSERT(res);
			UINT32 iUserID = res->user_id();
			CUser* pUser = CUserMgr::GetInstance()->GetUserByUserID(iUserID);
			if (pUser)
			{
				gs_dbs_friend_offline_req::OP_TYPE op_type = res->op_type();
				switch (op_type)
				{
				case gs_dbs_friend_offline_req::ADD_FRIEND:
					{
						gs_c_user_friend_op_res c_res;
						c_res.set_op_type(c_gs_user_friend_op_req::ADD_FRIEND);
						if(e_dbsgs_errorcode_success == res->error_code()){
							pUser->AddFriend(res->info());
							c_res.set_error_code(e_gsc_errorcode_success);
							c_res.mutable_base_info()->CopyFrom(res->info());
						}
						else{
							c_res.set_error_code(e_gsc_errorcode_friend_affirm_num_reach_max_num);
						}
						pUser->Send(e_msg_gs_c_user_friend_op_res, &c_res);
					}
					break;
				case gs_dbs_friend_offline_req::AFFIRM_AGREE1:
				case gs_dbs_friend_offline_req::AFFIRM_AGREEN:{
					gs_c_user_friend_op_res c_res;

					if(gs_dbs_friend_offline_req::AFFIRM_AGREE1 == op_type){
						c_res.set_op_type(c_gs_user_friend_op_req::AFFIRM_AGREE1);
					}
					else if(gs_dbs_friend_offline_req::AFFIRM_AGREEN == op_type){
						c_res.set_op_type(c_gs_user_friend_op_req::AFFIRM_AGREEN);
					}

					if (e_dbsgs_errorcode_success == res->error_code()){
						pUser->AffirmFriend(res->affirm_ids());
						c_res.set_error_code(e_gsc_errorcode_success);
						if(gs_dbs_friend_offline_req::AFFIRM_AGREE1 == op_type){
							c_res.set_op_type(c_gs_user_friend_op_req::AFFIRM_AGREE1);
							c_res.set_user_id(res->affirm_ids(0));
						}
					}
					else if(e_dbsgs_errorcode_friend_num_reach_max == res->error_code()){
						c_res.set_error_code(e_gsc_errorcode_friend_friend_friend_num_reach_max);
					}
					else{
						c_res.set_error_code(e_gsc_errorcode_unknown_error);
					}
					pUser->Send(e_msg_gs_c_user_friend_op_res, &c_res);
															  }break;
				}
				
			}
			delete res;
			res = NULL;
		}break;


		//---- pay_server����ش���
		case CServerForUserAsync::EAsyncOP_PS_Paied:{

			ps_gs_paied_req* req = (ps_gs_paied_req*)asyncData->data;
			ASSERT(req);

			std::vector<UINT32> users;
			users.push_back(req->user_id());

			std::map<std::string, CValue> params;
			params["string"].SetStringValue("��ֵ�ɹ�O(��_��)O");

			std::vector<stGameElement> attaches;
			for (::google::protobuf::RepeatedPtrField< ::ps_gs_paied_req_game_element >::const_iterator it = req->eles().begin(); it != req->eles().end(); ++it){
				stGameElement ele;
				ele.eType = (EELEMENTTYPE)it->type();
				ele.iID = it->id();
				ele.iSubID = 0;
				ele.eNumType = stGameElement::INCREASE;
				ele.iNum = it->num();

				bool r = IsElementValid(ele);
				ASSERT(r);
				attaches.push_back(ele);
			}

			bool r = CMailMgr::GetInstance()->SendMailToUsers(MAIL_TYPE::MAIL_TYPE_SYS_NOTICE, &users, NULL, &params, &attaches);
			ASSERT(r);

			delete req;

			e_psgs_errorcode ret = e_psgs_errorcode_unkown_error;
			if (r){
				ret = e_psgs_errorcode_success;
			}

			gs_ps_paied_res* res = new gs_ps_paied_res;
			res->set_error_code(ret);
			res->set_order_id(req->order_id());

			r = CClientForPS::GetInstance()->SendAsync(e_msg_gs_ps_paied_res, res);
			ASSERT(r);
		}break;
		case CServerForUserAsync::EAsyncOP_Rank_Res:{
			dbs_gs_rank_res* res = (dbs_gs_rank_res*)asyncData->data;
			ASSERT(res);
			if(e_dbsgs_errorcode_success == res->error_code())
				CRankMgr::GetInstance()->UpdateRank(*res);
			else{
				printf("�����ݿ���ȡ�������а���Ϣʧ�ܣ�");
			}
			delete res;
		}break;
	}

END:
	delete asyncData;
	uv_close((uv_handle_t*)handle, after_async_close);
}

void CServerForUser::SendAsync(Guid& sender_client_id, CServerForUserAsync::EAsyncOP eAsyncOP, google::protobuf::Message* data )
{
	CServerForUserAsync* asyncData = new CServerForUserAsync();
	asyncData->op = eAsyncOP;
	asyncData->user_connect_id = sender_client_id;
	asyncData->data = data;

	uv_async_t* async = CServerForUser::GetInstance()->CreateAsync();
	async->data = asyncData;

	int r = uv_async_send(async);
	ASSERT(r == 0);
	if (r != 0)
	{
		PRINTF_UV_ERROR(r);
		delete data;
		delete asyncData;
		async->data = NULL;
		CServerForUser::GetInstance()->DestroyAsync(async);
	}
}

bool CServerForUser::check_logined( uv_stream_t* stream )
{
	ASSERT(stream);
	CUserConnectContext* ctx = (CUserConnectContext*)stream->data;
	ASSERT(ctx);
	if (ctx->stauts >= CUserConnectContext::EUserConnectStatus_Logined)
	{
		return true;
	}
	else{
		return false;
	}
}

e_gsc_errorcode CServerForUser::check( uv_stream_t* stream, e_msg_c_gs req_msg_id, google::protobuf::Message* pInReq, const void* buf, int len, CUser* &pOutUser )
{
	ASSERT(stream);

	//e_msg_c_gs req_msg_id = e_msg_c_gs_leave_gs_req;
	//e_msg_gs_c res_msg_id = e_msg_gs_c_leave_gs_res;
	//c_gs_leave_gs_req req;
	//gs_c_leave_gs_res res;

	bool logined = check_logined(stream);
	if (!logined)
	{
		//pInRes->set_error_code(e_gsc_errorcode_not_logined);
		//send(stream, res_msg_id, &res);
		return e_gsc_errorcode_not_logined;
	}

	bool r = pInReq->ParseFromArray(buf, len);
	if (!r)
	{
		PRINTF_PBPARSE_ERROR(req_msg_id);
		//res.set_error_code(e_gsc_errorcode_unknown_error);
		//send(stream, res_msg_id, &res);
		return e_gsc_errorcode_unknown_error;
	}

	CUserConnectContext* ctx = (CUserConnectContext*)stream->data;
	ASSERT(ctx && ctx->iUserID);
	CUser* pUser = CUserMgr::GetInstance()->GetUserByUserID(ctx->iUserID);
	pOutUser = pUser;
	if (!pUser)
	{
		PRINTF_PROCMSG_ERROR(req_msg_id);
		//res.set_error_code(e_gsc_errorcode_not_get_user_info);
		//send(stream, res_msg_id, &res);
		return e_gsc_errorcode_not_get_user_info;
	}

	return e_gsc_errorcode_success;
}

bool CServerForUser::handle_msg_enter_gs_req( uv_stream_t* stream, const char * buf, int len )
{
	c_gs_enter_gs_req req;
	bool r = req.ParseFromArray(buf, len);
	if (r)
	{
		CUserConnectContext* ctx = (CUserConnectContext*)stream->data;
		ASSERT(ctx);

		////�жϵ�½״̬ ��ʱ�϶�û�е�¼ ��û���ж�token�أ���
		//if (ctx->stauts >= ctx->EUserConnectStatus_Logined){//���� client_for_as�߳� �ж�token����
		//	//Guid* token = new Guid(req.token());
		//	//asyncOP->data = token;
		//}
		//else{
		//	gs_c_enter_gs_res res;
		//	res.set_error_code(e_gsc_errorcode_not_logined);
		//	send(stream, e_msg_gs_c_enter_gs_res, &res);
		//	return false;
		//}

		typedef struct stSenderData{
			Guid token;
			Guid connect_id;
		}stSenderData;
		stSenderData* data = new stSenderData;
		//data->token = req.token();//ע�� ����д���ò�����ȷ��token���ݣ�
		data->token = (BYTE*)(req.mutable_token()->c_str());
		data->connect_id = ctx->ConnectID;

		CClientForASASync* asyncOP = new CClientForASASync;
		asyncOP->op = CClientForASASync::EASyncOP_CheckToken_Req;
		asyncOP->data = data;

		uv_async_t* async = CClientForAS::GetInstance()->CreateAsync();
		async->data = asyncOP;

		int r = uv_async_send(async);
		ASSERT(r == 0);
		if (r != 0)
		{
			PRINTF_UV_ERROR(r);
			delete data;
			delete asyncOP;
			async->data = NULL;
			CClientForAS::GetInstance()->DestroyAsync(async);
		}

	}
	else{
		PRINTF_PBPARSE_ERROR(e_msg_c_gs_enter_gs_req);
		gs_c_enter_gs_res res;
		res.set_error_code(e_gsc_errorcode_unknown_error);
		send(stream, e_msg_gs_c_enter_gs_res, &res);
		return false;
	}

	return true;
}

bool CServerForUser::handle_msg_config_req( uv_stream_t* stream, const char * buf, int len )
{

	bool logined = check_logined(stream);
	if (!logined)
	{
		gs_c_config_res res;
		res.set_error_code(e_gsc_errorcode_not_logined);
		send(stream, e_msg_gs_c_config_res, &res);
		return false;
	}

	c_gs_config_req req;
	bool r = req.ParseFromArray(buf, len);
	if (!r)
	{
		PRINTF_PBPARSE_ERROR(e_msg_c_gs_config_req);
		gs_c_config_res res;
		res.set_error_code(e_gsc_errorcode_unknown_error);
		send(stream, e_msg_gs_c_config_res, &res);
		return false;
	}

	BYTE* bin = NULL;
	UINT32 bin_len = CClientConfigMgr::GetInstance()->get_gs_c_config_bin(bin);
	return send(stream, bin, bin_len);
}

bool CServerForUser::handle_msg_user_info_req( uv_stream_t* stream, const char * buf, int len )
{
	bool logined = check_logined(stream);
	if (!logined)
	{
		gs_c_user_info_res res;
		res.set_error_code(e_gsc_errorcode_not_logined);
		send(stream, e_msg_gs_c_user_info_res, &res);
		return false;
	}

	c_gs_user_info_req req;
	bool ret = req.ParseFromArray(buf, len);
	if (!ret)
	{
		PRINTF_PBPARSE_ERROR(e_msg_c_gs_user_info_req);
		gs_c_user_info_res res;
		res.set_error_code(e_gsc_errorcode_unknown_error);
		send(stream, e_msg_gs_c_user_info_res, &res);
		return false;
	}

	CUserConnectContext* ctx = (CUserConnectContext*)stream->data;
	ASSERT(ctx && ctx->iUserID);

	CUser* pUser = CUserMgr::GetInstance()->GetUserByUserID(ctx->iUserID);
	//todo �������Ӧ�ò������ ��Ϊ������ʱ������������һ���ϵ� �������usermgr�е�user����
	//�ϵ������ӵ�Ȼ������뵽��������Ĵ��� ��ʹ�´�ͬһ���û��������ӽ���
	//�ж�ͬһ�� UserID ���ܶ�����ӣ��� ��ǰһ���û��������� ��������û���Ϣ�Ƿ�Ҫɾ���� ���Բ�ɾ�������õģ�
	if (pUser)
	{
		////�ж������Ƿ���� 
		//if(pUser->HasConnect()){
		//	//����Ѿ����� �����ߣ�������ô�ģ�!
		//	//ASSERT(false && "�û��Ѿ����� �����ߣ�������ô�ģ�!\n");
		//	if( pUser->GetConnect() != stream){
		//		gs_c_user_info_res res;
		//		res.set_error_code(e_gsc_errorcode_user_using_by_another_connect);
		//		send(stream, e_msg_gs_c_user_info_res, &res);
		//		printf("���[%u]��Ϣ��ǰ���ڱ�����[%p]ʹ�ã�����[%p]��ȡ�����Ϣʧ��\n", pUser->UserID(), pUser->GetConnect(), stream);

		//		//����stream��״̬Ϊδ��¼ ����UserID��
		//		ctx->stauts = CUserConnectContext::EUserConnectStatus_Connected;
		//		ctx->iUserID = NULL;//����ط������õĻ� �����ӶϿ� ��ȥ���CUser�����������Ϣ Ȼ���ֲ���� ���±���
		//		return false;
		//	}
		//}
		//else{
		//	pUser->IntertwineConnect((uv_stream_t*)stream);
		//}

		gs_c_user_info_res res;
		
		gs_dbs_user_info_op_req *db_req = new gs_dbs_user_info_op_req;
		db_req->set_user_id(pUser->UserID());
		bool r = pUser->Fill_gs_c_user_info_res(&res, *db_req);
		ASSERT(r);

		res.set_error_code(e_gsc_errorcode_success);
		send(stream, e_msg_gs_c_user_info_res, &res);
		printf("CUserMgr�д��û�id[%u]�Ѿ�����, ���췵���û���Ϣ��\n", pUser->UserID());

		SendDBReq(db_req);
		return true;
	}
	
	CClientForDBSAsync* asynData = new CClientForDBSAsync();
	asynData->op = CClientForDBSAsync::EASyncOP_User_Info_Req;
	asynData->user_id = ctx->iUserID;
	asynData->user_connect_id = ctx->ConnectID;

	uv_async_t* async = CClientForDBS::GetInstance()->CreateAsync();
	async->data = asynData;

	int r = uv_async_send(async);
	ASSERT(r == 0);
	if (r != 0)
	{
		PRINTF_UV_ERROR(r);
		delete asynData;
		async->data = NULL;
		CClientForDBS::GetInstance()->DestroyAsync(async);
	}

	return r == 0;
}

bool CServerForUser::handle_msg_start_sub_task_req( uv_stream_t* stream, const char * buf, int len )
{
	e_msg_c_gs req_msg_id = e_msg_c_gs_start_sub_task_req;
	e_msg_gs_c res_msg_id = e_msg_gs_c_start_sub_task_res;
	c_gs_start_sub_task_req req;
	gs_c_start_sub_task_res res;

	CUser* pUser = NULL;
	e_gsc_errorcode error_code = check(stream, req_msg_id, &req, buf, len, pUser);
	if (error_code != e_gsc_errorcode_success)
	{
		res.set_error_code(error_code);
		send(stream, res_msg_id, &res);
		return false;
	}
	ASSERT(pUser);

	//�жϳ���Ӣ�۲���Ϊ�գ�
	if (req.heros().size() == 0)
	{
		res.set_error_code(e_gsc_errorcode_op_not_support);
		send(stream, res_msg_id, &res);
		return false;
	}

	//�ж������������ЩӢ��
	for (::google::protobuf::RepeatedField< ::google::protobuf::uint32 >::const_iterator it = req.heros().begin(); it != req.heros().end() ; ++it){
		if (!pUser->GetHero(*it)){
			res.set_error_code(e_gsc_errorcode_op_not_support);
			send(stream, res_msg_id, &res);
			return false;
		}
	}
	
	//���ö���
	pUser->UpdateTeam(req.heros());//�����ö��� ������Ҫ�ж��Ƿ�����������
	bool r = pUser->StartFight(req.sub_task_id());
	if (!r)
	{
		//��ն��飡
		pUser->ClearTeam();
	}
	
	send(stream, e_msg_gs_c_start_sub_task_res, &(pUser->get_start_sub_task_res()));

	return r;
}

bool CServerForUser::handle_msg_end_sub_task_req( uv_stream_t* stream, const char * buf, int len )
{
	e_msg_c_gs req_msg_id = e_msg_c_gs_end_sub_task_req;
	e_msg_gs_c res_msg_id = e_msg_gs_c_end_sub_task_res;
	c_gs_end_sub_task_req req;
	gs_c_end_sub_task_res res;

	CUser* pUser = NULL;
	e_gsc_errorcode error_code = check(stream, req_msg_id, &req, buf, len, pUser);
	if (error_code != e_gsc_errorcode_success)
	{
		res.set_error_code(error_code);
		send(stream, res_msg_id, &res);
		return false;
	}
	ASSERT(pUser);

	gs_c_start_sub_task_res& start_sub_task_info = pUser->get_start_sub_task_res();
	if (start_sub_task_info.error_code() != e_gsc_errorcode_success)// == min ����.  û���Ѿ����������
	{
		//PRINTF_PROCMSG_ERROR(e_msg_c_gs_end_sub_task_req);
		res.set_error_code(e_gsc_errorcode_cannot_end_sub_task);
		send(stream, e_msg_gs_c_end_sub_task_res, &res);
		return false;
	}
	
	//todo �Ƿ񳬹���ʱ�����ƣ���
	/*//ȥ��ʱ���ж� ��Ϊ�ͻ��˿��ܻ�������ͣ
	UINT32 timeLimit = CTaskMgr::GetInstance()->GetSubTaskTimeLimit(start_sub_task_info.sub_task_id());
	UINT32 timeUsed = diffTime(Now(), start_sub_task_info.enter_time());
	if ( timeUsed > timeLimit)
	{
		//todo ��ʱ��ȥ����
		printf("����ʱ �������ʧ�ܣ�������ʱ�ȷŹ��� timeUsed[%u] > timeLimit[%u]", timeUsed, timeLimit);
		res.set_error_code(e_gsc_errorcode_sub_task_time_out);
		send(stream, e_msg_gs_c_end_sub_task_res, &res);
		return false;
	}
	*/

	//����ɹ����ͻ���
	res.set_error_code(e_gsc_errorcode_success);
	
	gs_dbs_user_info_op_req* db_req = NULL;
	GS_C_NTF ntf;

	if(req.is_win()){
		//todo �����Ǽ�

		//UINT32 star =  3;
		//ASSERT(req.has_star());
		if (!req.has_star())
		{
			res.set_error_code(e_gsc_errorcode_cannot_end_sub_task);
			send(stream, e_msg_gs_c_end_sub_task_res, &res);
			return false;
		}

		//�Ѿ�ʹ�ù� ��ָ�������״̬Ϊ���ɹ�����ֹ��ε���end ��β�����
		start_sub_task_info.set_error_code(e_gsc_errorcode_min);

		stSubTask* SubTaskCfg = CTaskMgr::GetInstance()->GetSubTaskBySubTaskID(start_sub_task_info.sub_task_id());
		if (!SubTaskCfg)
		{
			res.set_error_code(e_gsc_errorcode_cannot_end_sub_task);
			send(stream, e_msg_gs_c_end_sub_task_res, &res);
			return false;
		}

		CTask* pTask = CTaskMgr::GetInstance()->GetTaskByTaskID(SubTaskCfg->iTaskID);
		if (!pTask)
		{
			res.set_error_code(e_gsc_errorcode_cannot_end_sub_task);
			send(stream, e_msg_gs_c_end_sub_task_res, &res);
			return false;
		}

		//��db����Ϣ
		db_req = new gs_dbs_user_info_op_req;
		db_req->set_user_id(pUser->UserID());

		//�۳�ʣ�������
		int spend_health = SubTaskCfg->iSpendHealth - SUBTASK_START_SPEND_HEALTH;//
		ASSERT( spend_health >= 0 );
		if (spend_health > 0){//��Ҫ��ʣ�µ�����ʱ
			bool isFull2NotFull = false;
			bool r = pUser->DecreaseHealth(spend_health, &isFull2NotFull);
			ASSERT(r);//CanStartFight�������ж�
			if (!r)
			{
				res.set_error_code(e_gsc_errorcode_cannot_end_sub_task);
				send(stream, e_msg_gs_c_end_sub_task_res, &res);
				delete db_req;
				return false;
			}

			//c��db�޸��������ֵ
			struct_game_element* ele = res.add_output();
			ele->set_type(struct_game_element::EELEMENTTYPE_HEALTH);
			ele->set_id(0);
			if (isFull2NotFull)
			{
				ele->set_sub_id(pUser->GetHealthLastUpdateTime());//����ͻ��˱������������Զ��ظ����ֵ���� �ڿ�ȥ�������������� ����֮�� ��ʹ�����﷢��������ʱ��Ϊ����ʱ���
			}
			else{
				ele->set_sub_id(0);
			}
			ele->set_num_type(struct_game_element::DECREASE);
			ele->set_num(spend_health);

			//�޸�db����ֵ
			gs_dbs_user_info_op_req_struct_op_base_info* base_info = db_req->mutable_base_info();
			base_info->set_op_type(gs_dbs_user_info_op_req::Update);
			base_info->set_health_cur(pUser->GetHealth());
			base_info->set_health_last_update_timestamp(pUser->GetHealthLastUpdateTime());
		}

		bool isTaskDone_before = pUser->IsTaskDone(pTask->iTaskID);
		bool isTaskDone_after = false;

		//����gs�ڴ����������Ϣ
		db_user_sub_task st;
		st.set_sub_task_id(start_sub_task_info.sub_task_id());
		st.set_star(req.star());//todo�� �Ǽ����㣡
		st.set_last_enter_timestamp(start_sub_task_info.enter_time());
		pUser->AddOrUpdateSubTask(st, db_req);
		if (!isTaskDone_before){//֮ǰû����ɵ�ʱ�� ��������
			isTaskDone_after = pUser->IsTaskDone(pTask->iTaskID);
		}
		
		//�б����ʱ��
		if (start_sub_task_info.output_size() != 0){
			bool r = pUser->UpdateGameElement(start_sub_task_info.output(), *db_req, *(res.mutable_output()), ntf, res.mutable_heros());
			ASSERT(r);
		}

		//���������ʱ������⽱��
		if(!isTaskDone_before && isTaskDone_after){
			bool r = pUser->UpdateGameElement(pTask->prizes, *db_req, *(res.mutable_task_complete_prize()), ntf, res.mutable_heros());
			ASSERT(r);
		}

		if (pTask->eType == CTask::ETASKTYPE_MAIN || pTask->eType == CTask::ETASKTYPE_BRANCH){
			bool r = pUser->OnShopRandomOpen(pTask->eType, *db_req, *res.mutable_shop());
			if (!r){//���û�п��� �̵� ���ͷ��̵�
				res.set_allocated_shop(NULL);
			}
		}
		
		//�ȷ��ظ�
		send(stream, e_msg_gs_c_end_sub_task_res, &res);

		pUser->TriggerMission(EEventType_Common_FB_All_Through, EEventSubType(SubTaskCfg->iSubTaskID), *db_req, ntf);//ͨ�ظ���
		
		CMapPoint* point = CMapMgr::GetInstance()->GetMapPointByID(pTask->iMapPointID);
		ASSERT(point);
		if (point)
		{
			if((UINT32)EArea_Huaxia == point->iChapterID){//����
				pUser->TriggerMission(EEventType_Complete_Chapter_FB, EEventSubType_HuaXia,  *db_req, ntf);//����ض��½ڸ���(����)
				if (3 == req.star())//����
				{
					if(1 == point->bType && (CTask::ETASKTYPE_MAIN  == pTask->eType || CTask::ETASKTYPE_BRANCH == pTask->eType))//��㣨���ߺ�֧�ߣ�
						pUser->TriggerMission(EEventType_Complete_Chapter_FB_Three_Star, EEventSubType_HuaXia, *db_req, ntf);//��������(�������ߺ�֧��)
				}
			}
		}

		if (3 == req.star())
		{
			pUser->TriggerMission(EEventType_Common_FB_All_Through_Three_Star, EEventSubType(SubTaskCfg->iSubTaskID), *db_req, ntf);//����ͨ�ظ���
		}
		switch(pTask->eType)
		{
		case CTask::ETASKTYPE_MAIN:
			{
				pUser->TriggerMission(EEventType_Mission_Main, EEventSubType_None, *db_req, ntf);//ͨ������
				
			}
			break;
		case CTask::ETASKTYPE_BRANCH://֧��
			pUser->TriggerMission(EEventType_Mission_Free, EEventSubType_None, *db_req, ntf);//ͨ��ֱ��
			break;
		case CTask::ETASKTYPE_ACTIVITY://�
		case CTask::ETASKTYPE_EXP:
		case CTask::ETASKTYPE_GOLD:
			pUser->TriggerMission(EEventType_Activity, EEventSubType_None, *db_req, ntf);//ͨ�ػ
			break;	
		}
	}
	else{
		send(stream, e_msg_gs_c_end_sub_task_res, &res);
		
		db_req = new gs_dbs_user_info_op_req;
		db_req->set_user_id(pUser->UserID());

		pUser->TriggerMission(EEventType_FB_Fail, EEventSubType_None, *db_req, ntf);//ͨ�ظ���ʧ��
	}
	
	_send_gs_c_ntf(ntf, stream);
	SendDBReq(db_req);

	return true;
}

bool CServerForUser::handle_msg_invoke_hero_req( uv_stream_t* stream, const char * buf, int len )
{
	e_msg_c_gs req_msg_id = e_msg_c_gs_invoke_hero_req;
	e_msg_gs_c res_msg_id = e_msg_gs_c_invoke_hero_res;
	c_gs_invoke_hero_req req;
	gs_c_invoke_hero_res res;

	CUser* pUser = NULL;
	e_gsc_errorcode error_code = check(stream, req_msg_id, &req, buf, len, pUser);
	if (error_code != e_gsc_errorcode_success)
	{
		res.set_error_code(error_code);
		send(stream, res_msg_id, &res);
		return false;
	}

	ASSERT(pUser);

	//todo 1.�����жϣ�

	//todo 2.���ģ�

	//db_user_hero_array new_heros;
	//���Ӣ��
	if(req.invoke_type() == c_gs_invoke_hero_req::FREE){
		res.set_error_code(e_gsc_errorcode_op_not_support);
		send(stream, e_msg_gs_c_invoke_hero_res, &res);
		return false;
	}
	else if(req.invoke_type() == c_gs_invoke_hero_req::GOLD){
		res.set_error_code(e_gsc_errorcode_op_not_support);
		send(stream, e_msg_gs_c_invoke_hero_res, &res);
		return false;
	}
	else if(req.invoke_type() == c_gs_invoke_hero_req::DIAMOND){   
		res.set_error_code(e_gsc_errorcode_op_not_support);
		send(stream, e_msg_gs_c_invoke_hero_res, &res);
		return false;
	}
	else if(req.invoke_type() == c_gs_invoke_hero_req::FRAGMENT){
		
		CItemCfg* item = NULL;
		UINT32 hero_id = 0;
		if (req.has_hero_id())
		{
			item = CItemMgr::GetInstance()->GetFragmentItemByHeroID(req.hero_id());
			hero_id = req.hero_id();
		}
		else if (req.has_item_id())
		{
			item = CItemMgr::GetInstance()->GetItemCfgByItemID(req.item_id());
			ASSERT(item->m_use_output.size() == 1 && item->m_use_output[0] && item->m_use_output[0]->eType == EELEMENTTYPE_HERO);
			hero_id = item->m_use_output[0]->iID;
		}

		//ASSERT(item && item->m_eUseType == CItemCfg::HERO_FRAGMENT);
		if (!item || item->m_eUseType != CItemCfg::HERO_FRAGMENT)
		{
			res.set_error_code(e_gsc_errorcode_op_not_support);
			send(stream, e_msg_gs_c_invoke_hero_res, &res);
			return false;
		}

		if (pUser->GetHero(hero_id))
		{
			res.set_error_code(e_gsc_errorcode_op_not_support);
			send(stream, e_msg_gs_c_invoke_hero_res, &res);
			return false;
		}

		CHeroCfg* hero_cfg = CHeroMgr::GetInstance()->GetHeroByHeroID(hero_id);
		ASSERT(hero_cfg);
	
		//�����Ƿ�����
		stHeroStarCfg* need = CHeroMgr::GetInstance()->GetStarNeed(hero_cfg->iStar);
		ASSERT(need);
		if (!need)
		{
			res.set_error_code(e_gsc_errorcode_op_not_support);
			send(stream, e_msg_gs_c_invoke_hero_res, &res);
			return false;
		}

		UINT32 need_num = need->iFragmentNum;
		bool r = pUser->IsItemEnough(item->m_iID, need_num);
		if (!r)
		{
			res.set_error_code(e_gsc_errorcode_no_enough_consume);
			send(stream, res_msg_id, &res);
			return false;
		}

		gs_dbs_user_info_op_req* db_req = new gs_dbs_user_info_op_req;
		db_req->set_user_id(pUser->UserID());

		//���� ����Ƭ
		GS_C_NTF ntf;

		struct_game_element consume;
		consume.set_type(struct_game_element::EELEMENTTYPE_ITEM);
		consume.set_id(item->m_iID);
		consume.set_num_type(struct_game_element::DECREASE);
		consume.set_num(need_num);
		r = pUser->UpdateGameElement(consume, *db_req, *res.mutable_consume(), ntf);
		ASSERT(r);

		//������
		
		r = pUser->UpdateGameElement(item->m_use_output, *db_req, *res.mutable_output(), ntf, res.mutable_heros());
		ASSERT(r);

		//���͸��ͻ���
		res.set_error_code(e_gsc_errorcode_success);
		send(stream, e_msg_gs_c_invoke_hero_res, &res);
		
		//�����
		if(CMarqueeMgr::GetInstance()->CanTriggerMarquee(gs_c_marquee_ntf::MARQUEE_INVOKE_HERO, hero_cfg->iStar)){
			std::map<std::string, CValue> values;
			CValue& vUserName = values["user_name"];
			vUserName.SetStringValue(pUser->Name().c_str());
			
			CValue& vHeroID = values["hero_id"];
			vHeroID.SetIntValue(hero_cfg->iHeroID);
			CMarqueeMgr::GetInstance()->SendMarqueeToAllUsers(gs_c_marquee_ntf::MARQUEE_INVOKE_HERO, values );
		}

		_send_gs_c_ntf(ntf, stream);
		//���͸�DBS
		SendDBReq(db_req);
	}

	return false;
}

bool CServerForUser::handle_msg_leave_gs_req( uv_stream_t* stream, const char * buf, int len )
{
	ASSERT(stream);

	e_msg_c_gs req_msg_id = e_msg_c_gs_leave_gs_req;
	e_msg_gs_c res_msg_id = e_msg_gs_c_leave_gs_res;
	c_gs_leave_gs_req req;
	gs_c_leave_gs_res res;

	CUser* pUser = NULL;
	e_gsc_errorcode error_code = check(stream, req_msg_id, &req, buf, len, pUser);
	if (error_code != e_gsc_errorcode_success)
	{
		res.set_error_code(error_code);
		send(stream, res_msg_id, &res);
		return false;
	}

	ASSERT(pUser);

	res.set_error_code(e_gsc_errorcode_success);
	send(stream, res_msg_id, &res);

	//1. ���������û���Ϣ
	pUser->OnLeave();

	//2. ����������ϵ�����
	uv_shutdown_t* shut_r = (uv_shutdown_t*)req_alloc();
	int ret = uv_shutdown(shut_r, stream, after_client_shutdown);
	ASSERT(ret == 0);

	////2. ���������Ϣ
	//CUserMgr::GetInstance()->RemoveUser(pUser->UserID());
	//
	////3. �ͷ�����ڴ�
	//delete pUser;

	return true;
}

bool CServerForUser::handle_msg_equip_level_up_req( uv_stream_t* stream, const char * buf, int len )
{
	e_msg_c_gs req_msg_id = e_msg_c_gs_equip_level_up_req;
	e_msg_gs_c res_msg_id = e_msg_gs_c_equip_level_up_res;
	c_gs_equip_level_up_req req;
	gs_c_equip_level_up_res res;

	CUser* pUser = NULL;
	e_gsc_errorcode error_code = check(stream, req_msg_id, &req, buf, len, pUser);
	if (error_code != e_gsc_errorcode_success)
	{
		res.set_error_code(error_code);
		send(stream, res_msg_id, &res);
		return false;
	}

	ASSERT(pUser);

	db_user_hero* hero = pUser->GetHero(req.hero_id());
	if (!hero)
	{
		res.set_error_code(e_gsc_errorcode_op_not_support);
		send(stream, res_msg_id, &res);
		return false;
	}

	UINT32 hero_level = hero->level();
	UINT32 user_gold = pUser->GetGold();

	if (req.op_type() == c_gs_equip_level_up_req::EQUIP1_LEVEL)
	{
		res.set_error_code(e_gsc_errorcode_op_not_support);
		send(stream, res_msg_id, &res);
		return false;
	}
	else if (req.op_type() == c_gs_equip_level_up_req::EQUIP1_LEVELN)
	{
		//�����ж�װ���Ƿ����
		db_user_hero_equip* equip = pUser->GetHeroEquip(req.hero_id(), req.equip_id());
		if (!equip)
		{
			res.set_error_code(e_gsc_errorcode_op_not_support);
			send(stream, res_msg_id, &res);
			return false;
		}
	
		//��������
		UINT32 need_gold = 0;
		UINT32 can_up_level = 0;
		UINT32 equip_level = equip->level();
		CEquipMgr::GetInstance()->LevelUpByEquipID(equip->equip_id(), equip_level, hero_level, user_gold, &need_gold, &can_up_level);

		if (!can_up_level)
		{
			res.set_error_code(e_gsc_errorcode_no_enough_consume_or_reach_max);
			send(stream, res_msg_id, &res);
			return false;
		}

		//����
		ASSERT(user_gold > need_gold);
		//1.�޸�gs����
		pUser->SetGold(user_gold - need_gold);

		//2.�޸�dbs����
		gs_dbs_user_info_op_req* db_req = new gs_dbs_user_info_op_req;
		db_req->set_user_id(pUser->UserID());
		gs_dbs_user_info_op_req_struct_op_base_info* db_op_base_info = db_req->mutable_base_info();
		db_op_base_info->set_op_type(gs_dbs_user_info_op_req::Update);
		db_op_base_info->set_gold(user_gold - need_gold);

		//3.���ͻ��˵���Ӧ
		struct_game_element* c_consume = res.add_consume();
		c_consume->set_type(struct_game_element::EELEMENTTYPE_GOLD);
		c_consume->set_id(0);
		c_consume->set_num_type(struct_game_element::DECREASE);
		c_consume->set_num(need_gold);
		
		//����Ч��
		ASSERT((equip_level + can_up_level) <= hero_level);
		//1.�޸�gs����
		equip->set_level(equip_level + can_up_level);

		//2.�޸�dbs����
		gs_dbs_user_info_op_req_struct_op_hero_equip* db_op_equip = db_req->add_hero_equips();
		db_op_equip->set_op_type(gs_dbs_user_info_op_req::Update);
		db_op_equip->set_hero_id(req.hero_id());
		db_op_equip->set_equip_id(equip->equip_id());
		db_op_equip->set_level(equip_level + can_up_level);

		//3.���ͻ��˵���Ӧ
		user_hero_equip* c_equip = res.add_equips();
		c_equip->set_equip_id(equip->equip_id());
		c_equip->set_level(equip_level + can_up_level);

		//���Ϳͻ���
		res.set_error_code(e_gsc_errorcode_success);
		res.set_hero_id(req.hero_id());
		send(stream, res_msg_id, &res);
		
		GS_C_NTF ntf;
		pUser->TriggerMission(EEventType_Equip_Level_Up, EEventSubType_None, *db_req, ntf);
		_send_gs_c_ntf(ntf, stream);
		//����db
		bool r = CClientForDBS::GetInstance()->SendAsync(CClientForDBSAsync::EASyncOP_User_Info_OP_Req, pUser->UserID(), db_req);
		//ASSERT(r);
		if (!r)
		{
			printf("EQUIP1_LEVELNװ���������ݿ���������͸�db�߳�ʧ�ܣ��� \n");
			delete db_req;
		}

	}
	else if (req.op_type() == c_gs_equip_level_up_req::EQUIPN_LEVELN)
	{
		//���Ȼ�ȡ����װ��
		::google::protobuf::RepeatedPtrField< ::db_user_hero_equip >* equips = pUser->GetHeroEquips(req.hero_id());
		if (!equips || equips->size() == 0)
		{
			res.set_error_code(e_gsc_errorcode_op_not_support);
			send(stream, res_msg_id, &res);
			return false;
		}

		std::vector<stEquipLevelUpInOut> equipInOut;
		stEquipLevelUpInOut tmp = {0};
		
		for ( ::google::protobuf::RepeatedPtrField< ::db_user_hero_equip >::const_iterator it = equips->begin(); it != equips->end() ; ++it)
		{
			tmp.iEquipID = it->equip_id();
			tmp.iLevel = it->level();
			tmp.iGold = 0;
			tmp.iLevelUpNum = 0;
			equipInOut.push_back(tmp);
		}

		//��������
		UINT32 need_gold = CEquipMgr::GetInstance()->LevelUp(hero_level, user_gold, equipInOut);
		if ( 0 == need_gold)
		{
			res.set_error_code(e_gsc_errorcode_no_enough_consume_or_reach_max);
			send(stream, res_msg_id, &res);
			return false;
		}

		//һ. ����
		ASSERT(user_gold > need_gold);
		//1.�޸�gs����
		pUser->SetGold(user_gold - need_gold);

		//2.�޸�dbs����
		gs_dbs_user_info_op_req* db_req = new gs_dbs_user_info_op_req;
		db_req->set_user_id(pUser->UserID());
		gs_dbs_user_info_op_req_struct_op_base_info* db_op_base_info = db_req->mutable_base_info();
		db_op_base_info->set_op_type(gs_dbs_user_info_op_req::Update);
		db_op_base_info->set_gold(user_gold - need_gold);

		//3.���ͻ��˵���Ӧ
		struct_game_element* c_consume = res.add_consume();
		c_consume->set_type(struct_game_element::EELEMENTTYPE_GOLD);
		c_consume->set_id(0);
		c_consume->set_num_type(struct_game_element::DECREASE);
		c_consume->set_num(need_gold);
		
		//��.����Ч��
		UINT32 iUpNum = 0;//������װ����Ŀ
		for ( std::vector<stEquipLevelUpInOut>::iterator itRet = equipInOut.begin(); itRet != equipInOut.end(); ++itRet)
		{
			if (0 == itRet->iLevelUpNum)
			{
				ASSERT(itRet->iGold == 0);
				continue;
			}

			db_user_hero_equip* equip = pUser->GetHeroEquip(req.hero_id(), itRet->iEquipID);
			ASSERT(equip && equip->level() == itRet->iLevel);
			ASSERT((equip->level() + itRet->iLevelUpNum) <= hero_level);
			UINT32 target_level = equip->level() + itRet->iLevelUpNum;
			//1.�޸�gs����
			equip->set_level(target_level);

			//2.�޸�dbs����
			gs_dbs_user_info_op_req_struct_op_hero_equip* db_op_equip = db_req->add_hero_equips();
			db_op_equip->set_op_type(gs_dbs_user_info_op_req::Update);
			db_op_equip->set_hero_id(req.hero_id());
			db_op_equip->set_equip_id(equip->equip_id());
			db_op_equip->set_level(target_level);

			//3.���ͻ��˵���Ӧ
			user_hero_equip* c_equip = res.add_equips();
			c_equip->set_equip_id(equip->equip_id());
			c_equip->set_level(target_level);
			iUpNum++;
		}
		
		//���Ϳͻ���
		res.set_error_code(e_gsc_errorcode_success);
		res.set_hero_id(req.hero_id());
		send(stream, res_msg_id, &res);
		
		GS_C_NTF ntf;
		pUser->TriggerMission(EEventType_Equip_Level_Up, EEventSubType_None, *db_req, ntf, iUpNum);
		_send_gs_c_ntf(ntf, stream);
		//����db
		bool r = CClientForDBS::GetInstance()->SendAsync(CClientForDBSAsync::EASyncOP_User_Info_OP_Req, pUser->UserID(), db_req);
		//ASSERT(r);
		if (!r)
		{
			printf("EQUIPN_LEVELNװ���������ݿ���������͸�db�߳�ʧ�ܣ��� \n");
			delete db_req;
		}
	}
	else{
		res.set_error_code(e_gsc_errorcode_op_not_support);
		send(stream, res_msg_id, &res);
		return false;
	}

	return true;
}

bool CServerForUser::handle_msg_equip_evolution_req(uv_stream_t* stream, const char * buf, int len){
	e_msg_c_gs req_msg_id = e_msg_c_gs_equip_evolution_req;
	e_msg_gs_c res_msg_id = e_msg_gs_c_equip_evolution_res;
	c_gs_equip_evolution_req req;
	gs_c_equip_evolution_res res;

	CUser* pUser = NULL;
	e_gsc_errorcode error_code = check(stream, req_msg_id, &req, buf, len, pUser);
	if (error_code != e_gsc_errorcode_success)
	{
		res.set_error_code(error_code);
		send(stream, res_msg_id, &res);
		return false;
	}
	ASSERT(pUser);

	//1.�ж���������Ƿ��д�װ��
	db_user_hero* hero = pUser->GetHero(req.hero_id());
	db_user_hero_equip* equip = pUser->GetHeroEquip(req.hero_id(), req.equip_id());
	if (!hero || !equip)
	{
		res.set_error_code(e_gsc_errorcode_op_not_support);
		send(stream, res_msg_id, &res);
		return false;
	}

	//2.�ж�װ���Ƿ��н�������
	CEquipCfg* equip_cfg = CEquipMgr::GetInstance()->GetEquipCfgByEquipID(req.equip_id());

	if (!equip_cfg || !equip_cfg->iEvolutionEquipID || equip_cfg->eQuality > hero->quality() )
	{
		res.set_error_code(e_gsc_errorcode_op_not_support);
		send(stream, res_msg_id, &res);
		return false;
	}

	CEquipCfg* target_equip_cfg = CEquipMgr::GetInstance()->GetEquipCfgByEquipID(equip_cfg->iEvolutionEquipID);
	//if ( equip_cfg->eQuality > hero->quality() 
	//	//|| target_equip_cfg->eQuality > (hero->quality() + 1)//���������Ʒ�������� �ͻ������������� Ҳ������
	//	)
	//{
	//	ASSERT(target_equip_cfg->eQuality == (hero->quality()+1));
	//	res.set_error_code(e_gsc_errorcode_op_not_support);
	//	send(stream, res_msg_id, &res);
	//	return false;
	//}

	//3.�ж������Ƿ�����
	ASSERT(equip_cfg->eQuality == hero->quality());//�����������ô Դװ��Ʒ�ʴ�ʱһ������Ӣ��Ʒ�� �����Ĺ�ϵ
	ASSERT(target_equip_cfg->eQuality == (hero->quality()+1));//Ŀ��װ��Ʒ�ʴ�ʱһ������Ӣ��Ʒ��+1
	ASSERT(equip_cfg->eEquipType == target_equip_cfg->eEquipType);//ͬ���͵�װ��

		//����Ƿ�����
	UINT32 need_gold = CEquipMgr::GetInstance()->EvolutionToQualityGold(equip_cfg->eQuality, equip_cfg->eEquipType);
	UINT32 user_gold = pUser->GetGold();
	if (user_gold < need_gold)
	{
		res.set_error_code(e_gsc_errorcode_gold_not_enough);
		send(stream, res_msg_id, &res);
		return false;
	}

		//�����Ƿ�����
	bool r = pUser->HasItems(equip_cfg->vEvolutionConsume);
	if (!r)
	{
		res.set_error_code(e_gsc_errorcode_no_enough_consume);
		send(stream, res_msg_id, &res);
		return false;
	}
	
	//1.�������
	pUser->SetGold(user_gold - need_gold);

	gs_dbs_user_info_op_req* db_req = new gs_dbs_user_info_op_req;
	db_req->set_user_id(pUser->UserID());
	gs_dbs_user_info_op_req_struct_op_base_info* base_op = db_req->mutable_base_info();
	base_op->set_op_type(gs_dbs_user_info_op_req::Update);
	base_op->set_gold(user_gold - need_gold);

	struct_game_element* consume = res.add_consume();
	consume->set_type(struct_game_element::EELEMENTTYPE_GOLD);
	consume->set_id(0);
	consume->set_num_type(struct_game_element::DECREASE);
	consume->set_num(need_gold);

	//2.��������
	/*
	for ( std::vector<stGameElement*>::iterator it = equip_cfg->vEvolutionInput.begin() ; it != equip_cfg->vEvolutionInput.end(); ++it)
	{
		stGameElement* input = *it;
		ASSERT(input);
		struct_game_element* db_consume = db_req->add_items();
		db_consume->set_type((struct_game_element::EELEMENTTYPE)input->eType);
		db_consume->set_id(input->iID);
		db_consume->set_sub_id(input->iSubID);
		db_consume->set_num_type(struct_game_element::DECREASE);
		db_consume->set_num(input->iNum);

		struct_game_element* c_consume = res.add_consume();
		c_consume->CopyFrom(*db_consume);
	}
	*/

	GS_C_NTF ntf;
	r = pUser->UpdateGameElement(equip_cfg->vEvolutionConsume, *db_req, *res.mutable_consume(), ntf);
	ASSERT(r);

	//3.��Ϊ�������װ����
	equip->set_equip_id(equip_cfg->iEvolutionEquipID);
		
	gs_dbs_user_info_op_req_struct_op_hero_equip* equip_op = db_req->add_hero_equips();
	equip_op->set_op_type(gs_dbs_user_info_op_req::Update);
	equip_op->set_hero_id(req.hero_id());
	equip_op->set_equip_id(req.equip_id());
	equip_op->set_evolution_equip_id(equip_cfg->iEvolutionEquipID);

	res.set_error_code(e_gsc_errorcode_success);
	res.set_hero_id(req.hero_id());
	res.set_equip_id(req.equip_id());
	res.set_evolution_equip_id(equip_cfg->iEvolutionEquipID);

	//���͸��ͻ���
	r = send(stream, res_msg_id, &res);
	//ASSERT(r);

	_send_gs_c_ntf(ntf, stream);
	//���͸�db
	SendDBReq(db_req);

	return true;
}

bool CServerForUser::handle_msg_sell_req(uv_stream_t* stream, const char * buf, int len)
{
	e_msg_c_gs req_msg_id = e_msg_c_gs_sell_req;
	e_msg_gs_c res_msg_id = e_msg_gs_c_sell_res;

	c_gs_sell_req req;
	gs_c_sell_res res;

	CUser* pUser = NULL;
	e_gsc_errorcode error_code = check(stream, req_msg_id, &req, buf, len, pUser);
	if (error_code != e_gsc_errorcode_success)
	{
		res.set_error_code(error_code);
		send(stream, res_msg_id, &res);
		return false;
	}
	
	ASSERT(pUser);

	//��֤������Ʒ
	::google::protobuf::RepeatedPtrField< ::struct_game_element >::iterator it = req.mutable_items()->begin();
	for (; it != req.mutable_items()->end(); ++it)
	{
		ASSERT(it->type() == struct_game_element::EELEMENTTYPE_ITEM);
		ASSERT(it->num_type() == struct_game_element::DECREASE);
	
		if (it->type() != struct_game_element::EELEMENTTYPE_ITEM || it->num_type() != struct_game_element::DECREASE)
		{
			res.set_error_code(e_gsc_errorcode_op_not_support);
			send(stream, res_msg_id, &res);
			return false;
		}
	}

	//�ж���Ʒ�Ƿ�����
	if(!pUser->IsItemsEnough(req.mutable_items()))
	{
		res.set_error_code(e_gsc_errorcode_not_enough_item);
		send(stream, res_msg_id, &res);
		return false;
	}
	
	//�����ܽ��ֵ
	UINT32 iGold = 0;
	/*
	::google::protobuf::RepeatedPtrField< ::struct_game_element >::iterator iter = req.mutable_items()->begin();
	for (; iter != req.mutable_items()->end(); ++iter)
	{
		CItemCfg* pItem = CItemMgr::GetInstance()->GetItemCfgByItemID(iter->id());
		if(!pItem)
		{
			res.set_error_code(e_gsc_errorcode_unknown_error);
			send(stream, res_msg_id, &res);
			return false;
		}
		else
		{
			iGold += pItem->m_iSellGold * iter->num();
		}
	}
	*/

	iGold = CItemMgr::GetInstance()->GetSellGold(req.items());
	if ( -1 == iGold)
	{
		res.set_error_code(e_gsc_errorcode_op_not_support);
		send(stream, res_msg_id, &res);
		return false;
	}

	//���ĵ���Ʒ
	gs_dbs_user_info_op_req* pDBReq = new gs_dbs_user_info_op_req;
	pDBReq->set_user_id(pUser->UserID());
	/*
	::google::protobuf::RepeatedPtrField< ::struct_game_element >::iterator it = req.mutable_items()->begin();
	for (; it != req.mutable_items()->end(); ++it)
	{
		it->set_num_type(struct_game_element::DECREASE);
		it->set_type(struct_game_element::EELEMENTTYPE_ITEM);
		pUser->UpdateGameElement(*it, *pDBReq, *res.mutable_eles());
	}
	*/

	GS_C_NTF ntf;
	bool r = pUser->UpdateGameElement(req.items(), *pDBReq, *res.mutable_eles(), ntf);
	ASSERT(r);

	//��õĽ��
	struct_game_element eleGold;
	eleGold.set_id(0);
	eleGold.set_num(iGold);
	eleGold.set_type(struct_game_element::EELEMENTTYPE_GOLD);
	eleGold.set_num_type(struct_game_element::INCREASE);
	r = pUser->UpdateGameElement(eleGold, *pDBReq, *res.mutable_eles(), ntf);
	ASSERT(r);
	/*
	//c res
	res.add_eles()->CopyFrom(eleGold);
	*/

	res.set_error_code(e_gsc_errorcode_success);
	send(stream, e_msg_gs_c_sell_res, &res);

	_send_gs_c_ntf(ntf, stream);

	SendDBReq(pDBReq);

	return true;
}

bool CServerForUser::handle_msg_fragment_compound_req(uv_stream_t* stream, const char * buf, int len)
{
	e_msg_c_gs req_msg_id = e_msg_c_gs_fragment_compound_req;
	e_msg_gs_c res_msg_id = e_msg_gs_c_fragment_compound_res;

	c_gs_fragment_compound_req req;
	gs_c_fragment_compound_res res;

	CUser* pUser = NULL;
	e_gsc_errorcode error_code = check(stream, req_msg_id, &req, buf, len, pUser);
	if (error_code != e_gsc_errorcode_success)
	{
		res.set_error_code(error_code);
		send(stream, res_msg_id, &res);
		return false;
	}

	UINT32 iComID = req.compound_id();
	CItemCfg* pCfg = CItemMgr::GetInstance()->GetItemCfgByItemID(iComID);
	if (!pCfg)
	{
		res.set_error_code(e_gsc_errorcode_unknown_error);
		send(stream, res_msg_id, &res);
		return false;
	}
	//��ȡ�ϳ�����Ҫ����Ʒ
	std::vector<stGameElement*>& vecEle = pCfg->m_compose_input;


	if(!pUser->IsConsumeEnough(vecEle)){
		res.set_error_code(e_gsc_errorcode_no_enough_consume);
		send(stream, res_msg_id, &res);
		return false;
	}
	
	gs_dbs_user_info_op_req* db_req = new gs_dbs_user_info_op_req;
	db_req->set_user_id(pUser->UserID());
	GS_C_NTF ntf;
	//����ͬ��(���ĵ���Ƭ�ͽ��)

	bool r = pUser->UpdateGameElement(vecEle, *db_req, *res.mutable_eles(), ntf);
	ASSERT(r);


	//����ͬ��(�ϳɵ���Ʒ)
	struct_game_element ele;
	ele.set_id(iComID);
	ele.set_num(1);
	ele.set_num_type(struct_game_element::INCREASE);
	ele.set_type(struct_game_element::EELEMENTTYPE_ITEM);


	r = pUser->UpdateGameElement(ele, *db_req, *res.mutable_eles(), ntf);
	ASSERT(r);

	res.set_error_code(e_gsc_errorcode_success);
	send(stream, e_msg_gs_c_fragment_compound_res, &res);

	_send_gs_c_ntf(ntf, stream);
	SendDBReq(db_req);

	return true;
}

bool CServerForUser::handle_msg_mail_opt_req(uv_stream_t* stream, const char * buf, int len)
{
	e_msg_c_gs req_msg_id = e_msg_c_gs_mail_opt_req;
	e_msg_gs_c res_msg_id = e_msg_gs_c_mail_opt_res;
	c_gs_mail_opt_req req;
	gs_c_mail_opt_res res;

	CUser* pUser = NULL;
	e_gsc_errorcode error_code = check(stream, req_msg_id, &req, buf, len, pUser);
	if (error_code != e_gsc_errorcode_success)
	{
		res.set_error_code(error_code);
		send(stream, res_msg_id, &res);
		return false;
	}
	
	//�����ʼ���Ϣ
	gs_dbs_user_info_op_req* db_req  = new gs_dbs_user_info_op_req;
	db_req->set_user_id(pUser->UserID());
	GS_C_NTF ntf;
	pUser->DelOutOfDateMail(db_req, ntf);
	
	std::vector<UINT32> vecMailid;
	::google::protobuf::RepeatedField< ::google::protobuf::uint32 >::iterator iter = req.mutable_mail_id_arr()->begin();
	for (; iter != req.mutable_mail_id_arr()->end(); ++iter)
	{
		vecMailid.push_back(*iter);
	}

	switch(req.mail_opt())
	{
	case c_gs_mail_opt_req::DEL:
		{
			if (!vecMailid.size() || !pUser->HasMails(vecMailid))
			{
				res.set_error_code(e_gsc_errorcode_op_not_support);
				break;
			}
			
			ASSERT(pUser->DelMails(vecMailid, db_req, &res));
			res.set_error_code(e_gsc_errorcode_success);
		}
		break;
	case c_gs_mail_opt_req::READ1:
		{
			if (vecMailid.size() != 1 || !pUser->HasMails(vecMailid))
			{
				res.set_error_code(e_gsc_errorcode_op_not_support);
				break;
			}
			
			ASSERT(db_req);
			ASSERT(pUser->SetMailReaded(vecMailid[0], db_req, &res));

			res.set_error_code(e_gsc_errorcode_success);
		}
		break;
	case c_gs_mail_opt_req::GET_ATTACH_AND_DEL1:
		{
			if (vecMailid.size() != 1 || !pUser->HasMails(vecMailid))
			{
				res.set_error_code(e_gsc_errorcode_op_not_support);
				break;
			}

			if (!pUser->IsMailsHasAttachs(vecMailid))
			{
				res.set_error_code(e_gsc_errorcode_op_not_support);
				break;
			}
			
			pUser->GetMailAttach(vecMailid[0], &res, db_req, ntf);
			ntf.release_mail_ntf_arr();//��ȡ��ֹ�ظ������ʼ�

			if (!res.has_error_code())
				res.set_error_code(e_gsc_errorcode_success);
		}
		break;
	case c_gs_mail_opt_req::GET_ATTACH_AND_DEL_ALL://ȫ����ȡ
		{
			//��ȡ����ȡ���ʼ�id
			std::vector<UINT32> vecMailID = pUser->GetAllHasAttachMailIDS();
			if (!vecMailID.size())
			{
				res.set_error_code(e_gsc_errorcode_not_exist_attach_mail);
				break;
			}
			
			ASSERT(db_req);
			GS_C_NTF ntf;
			for(UINT32 i = 0; i < vecMailID.size(); ++i)
			{
				pUser->GetMailAttach(vecMailID[i], &res, db_req, ntf);
			}
			ntf.release_mail_ntf_arr();
			if(!res.has_error_code())
				res.set_error_code(e_gsc_errorcode_success);
		}
		break;
	default:
		ASSERT(false && "�Ƿ����ʼ���������");
		break;
	}
	

	send(stream, e_msg_gs_c_mail_opt_res, &res);
	_send_gs_c_ntf(ntf, stream);
	SendDBReq(db_req);
	
	
	return true;
}

bool CServerForUser::handle_msg_hero_evolution_req( uv_stream_t* stream, const char * buf, int len )
{
	e_msg_c_gs req_msg_id = e_msg_c_gs_hero_evolution_req;
	e_msg_gs_c res_msg_id = e_msg_gs_c_hero_evolution_res;
	c_gs_hero_evolution_req req;
	gs_c_hero_evolution_res res;

	CUser* pUser = NULL;
	e_gsc_errorcode error_code = check(stream, req_msg_id, &req, buf, len, pUser);
	if (error_code != e_gsc_errorcode_success)
	{
		res.set_error_code(error_code);
		send(stream, res_msg_id, &res);
		return false;
	}
	ASSERT(pUser);

	//1.�ж���������Ƿ��д�Ӣ��
	db_user_hero* hero = pUser->GetHero(req.hero_id());
	if (!hero)//
	{
		res.set_error_code(e_gsc_errorcode_op_not_support);
		send(stream, res_msg_id, &res);
		return false;
	}

	UINT32 old_quality = hero->quality();
	if(old_quality >= (EQUALITY_MAX-1)){
		res.set_error_code(e_gsc_errorcode_op_not_support);
		send(stream, res_msg_id, &res);
		return false;
	}

	//��ȡװ��
	::google::protobuf::RepeatedPtrField< ::db_user_hero_equip >* equips = pUser->GetHeroEquips(req.hero_id());
	if (!equips || equips->size() == 0)//����ж� Ҳ�����ж���Ӣ���Ƿ���ڣ�
	{
		res.set_error_code(e_gsc_errorcode_op_not_support);
		send(stream, res_msg_id, &res);
		return false;
	}

	stHeroQualityCfg*  need = CHeroMgr::GetInstance()->GetQualityCfg((EQUALITY)(old_quality+1));
	if (!need)
	{
		res.set_error_code(e_gsc_errorcode_op_not_support);
		send(stream, res_msg_id, &res);
		return false;
	}

	if(pUser->GetGold() < need->iGold){
		res.set_error_code(e_gsc_errorcode_gold_not_enough);
		send(stream, res_msg_id, &res);
		return false;
	}

	if(pUser->GetLevel() < need->iNeedLevel){
		res.set_error_code(e_gsc_errorcode_level_not_enough);
		send(stream, res_msg_id, &res);
		return false;
	}

	//�ж�װ��Ʒ��
	for (::google::protobuf::RepeatedPtrField< ::db_user_hero_equip >::iterator it = equips->begin(); it != equips->end(); ++it)
	{
		EQUALITY eCurQuality = CEquipMgr::GetInstance()->GetQuality(it->equip_id());
		if (eCurQuality <=  old_quality)//װ��Ʒ�ʱ���Ҫ hero quality + 1���ܽ���
		{
			res.set_error_code(e_gsc_errorcode_equip_quality_not_enough);
			send(stream, res_msg_id, &res);
			return false;
		}
	}

	//1. �۽��
	pUser->SetGold(pUser->GetGold() - need->iGold);

	gs_dbs_user_info_op_req* db_req = new gs_dbs_user_info_op_req;
	db_req->set_user_id(pUser->UserID());
	gs_dbs_user_info_op_req_struct_op_base_info* base_op = db_req->mutable_base_info();
	base_op->set_op_type(gs_dbs_user_info_op_req::Update);
	base_op->set_gold(pUser->GetGold() - need->iGold);
	
	struct_game_element* c_consume = res.add_consume();
	c_consume->set_type(struct_game_element::EELEMENTTYPE_GOLD);
	c_consume->set_id(0);
	c_consume->set_num_type(struct_game_element::DECREASE);
	c_consume->set_num(need->iGold);

	gs_dbs_user_info_op_req_struct_op_hero_base* hero_base = db_req->add_heros_base();
	hero_base->set_op_type(gs_dbs_user_info_op_req::Update);
	hero_base->set_hero_id(req.hero_id());
	hero_base->set_quality(old_quality+1);

	//2. Ӣ��Ʒ������
	hero->set_quality(old_quality+1);
	
	//3. Ʒ������֮���Ƿ����¿��ļ��ܣ�
	stHeroQualityCfg* old_quality_cfg = CHeroMgr::GetInstance()->GetQualityCfg(EQUALITY(old_quality));
	stHeroQualityCfg* new_quality_cfg = CHeroMgr::GetInstance()->GetQualityCfg(EQUALITY(old_quality+1));
	ASSERT(old_quality_cfg && new_quality_cfg);
	if (old_quality_cfg->iPassiveSkillNum != new_quality_cfg->iPassiveSkillNum)//���¿��ļ���ʱ
	{
		for(int i =  old_quality_cfg->iPassiveSkillNum ;  i < new_quality_cfg->iPassiveSkillNum ; ++i){
			user_hero_skill::e_skill_type skill_type;

			switch (i)
			{
			case 0:{
						skill_type = user_hero_skill::PASSIVE1;
				   }break;
			case 1:{
						skill_type = user_hero_skill::PASSIVE2;
				   }break;
			case 2:{
						skill_type = user_hero_skill::PASSIVE3;
				   }break;
			}

			//����gs�ڴ�
			db_user_hero_skill* skill = pUser->GetHeroSkill(req.hero_id(), skill_type);
			ASSERT(skill);
			skill->set_level(1);//�¿��ŵ����ó�ʼ�ȼ�1 û����ʱ��1

			//�ͻ���
			user_hero_skill* c_skill = res.add_new_skills();
			c_skill->set_type(skill_type);
			c_skill->set_level(1);

			//����db
			gs_dbs_user_info_op_req::struct_op_hero_skill* db_skill = db_req->add_hero_skills();
			db_skill->set_op_type(gs_dbs_user_info_op_req::Update);
			db_skill->set_hero_id(req.hero_id());
			db_skill->set_type(skill_type);
			db_skill->set_level(1);//
		}
	}

	res.set_hero_id(req.hero_id());
	res.set_error_code(e_gsc_errorcode_success);
	send(stream, res_msg_id, &res);
	
	//����ƣ�
	if(CMarqueeMgr::GetInstance()->CanTriggerMarquee(gs_c_marquee_ntf::MARQUEE_HERO_EVOLUTION, old_quality+1)){
		std::map<std::string, CValue> values;
		CValue& vUserName = values["user_name"];
		vUserName.SetStringValue(pUser->Name().c_str());

		CHeroCfg* pCfg = CHeroMgr::GetInstance()->GetHeroByHeroID(req.hero_id());
		ASSERT(pCfg);
		
		CValue& vHeroID = values["hero_id"];
		vHeroID.SetIntValue(pCfg->iHeroID);

		CMarqueeMgr::GetInstance()->SendMarqueeToAllUsers(gs_c_marquee_ntf::MARQUEE_HERO_EVOLUTION, values, old_quality+1);
	}

	bool r = CClientForDBS::GetInstance()->SendAsync(CClientForDBSAsync::EASyncOP_User_Info_OP_Req, pUser->UserID(), db_req);
	if (!r)
	{
		delete db_req;
		return false;
	}
	return true;
}

bool CServerForUser::handle_msg_hero_star_up_req( uv_stream_t* stream, const char * buf, int len )
{
	e_msg_c_gs req_msg_id = e_msg_c_gs_hero_star_up_req;
	e_msg_gs_c res_msg_id = e_msg_gs_c_hero_star_up_res;
	c_gs_hero_star_up_req req;
	gs_c_hero_star_up_res res;

	CUser* pUser = NULL;
	e_gsc_errorcode error_code = check(stream, req_msg_id, &req, buf, len, pUser);
	if (error_code != e_gsc_errorcode_success)
	{
		res.set_error_code(error_code);
		send(stream, res_msg_id, &res);
		return false;
	}
	ASSERT(pUser);

	//1.�ж���������Ƿ��д�Ӣ��
	db_user_hero* hero = pUser->GetHero(req.hero_id());
	if (!hero || hero->star() >= MAXNUM_STAR)//
	{
		res.set_error_code(e_gsc_errorcode_op_not_support);
		send(stream, res_msg_id, &res);
		return false;
	}

	stHeroStarCfg*  from = CHeroMgr::GetInstance()->GetStarNeed(hero->star());
	stHeroStarCfg*  to = CHeroMgr::GetInstance()->GetStarNeed(hero->star()+1);
	if (!from || !to)
	{
		res.set_error_code(e_gsc_errorcode_op_not_support);
		send(stream, res_msg_id, &res);
		return false;
	}

	UINT32 needNum = to->iFragmentNum - from->iFragmentNum;
	CItemCfg* item_cfg = CItemMgr::GetInstance()->GetFragmentItemByHeroID(req.hero_id());
	if (!item_cfg)
	{
		res.set_error_code(e_gsc_errorcode_op_not_support);
		send(stream, res_msg_id, &res);
		return false;
	}

	if (!pUser->IsItemEnough(item_cfg->m_iID, needNum))
	{
		res.set_error_code(e_gsc_errorcode_not_enough_item);
		send(stream, res_msg_id, &res);
		return false;
	}
	
	gs_dbs_user_info_op_req* db_req = new gs_dbs_user_info_op_req;
	db_req->set_user_id(pUser->UserID());

//�۵���
	//����db
	//struct_game_element* consume = db_req->add_items();
	//consume->set_type(struct_game_element::EELEMENTTYPE_ITEM);
	//consume->set_id(item_cfg->m_iID);
	//consume->set_num_type(struct_game_element::DECREASE);
	//consume->set_num(needNum);

	
	/*
	//�ͻ���
	struct_game_element* c_consume = res.add_consume();
	c_consume->set_type(struct_game_element::EELEMENTTYPE_ITEM);
	c_consume->set_id(item_cfg->m_iID);
	c_consume->set_num_type(struct_game_element::DECREASE);
	c_consume->set_num(needNum);
	*/

	struct_game_element consume;
	consume.set_type(struct_game_element::EELEMENTTYPE_ITEM);
	consume.set_id(item_cfg->m_iID);
	consume.set_num_type(struct_game_element::DECREASE);
	consume.set_num(needNum);

	//����gs����ڴ� �� db������Ϣ
	GS_C_NTF ntf;
	bool r = pUser->UpdateGameElement(consume, *db_req, *res.mutable_consume(), ntf);
	ASSERT(r);

//��������Ǽ�
	//����db
	gs_dbs_user_info_op_req_struct_op_hero_base* db_hero = db_req->add_heros_base();
	db_hero->set_op_type(gs_dbs_user_info_op_req::Update);
	db_hero->set_hero_id(req.hero_id());
	db_hero->set_star(hero->star() + 1);

	//GS����������� ����ǰ��db���þ����������ˣ� ����gs����ڴ�
	hero->set_star(hero->star() + 1);

	res.set_hero_id(req.hero_id());
	res.set_error_code(e_gsc_errorcode_success);
	send(stream, res_msg_id, &res);

	//�����
	if(CMarqueeMgr::GetInstance()->CanTriggerMarquee(gs_c_marquee_ntf::MARQUEE_HERO_STAR_UP, hero->star())){
		std::map<std::string, CValue> values;
		CValue& vUserName = values["user_name"];
		vUserName.SetStringValue(pUser->Name().c_str());

		CHeroCfg* pCfg = CHeroMgr::GetInstance()->GetHeroByHeroID(req.hero_id());
		ASSERT(pCfg);
		
		CValue& vHeroID = values["hero_id"];
		vHeroID.SetIntValue(pCfg->iHeroID);

		CMarqueeMgr::GetInstance()->SendMarqueeToAllUsers(gs_c_marquee_ntf::MARQUEE_HERO_STAR_UP, values, hero->star());
	}

	_send_gs_c_ntf(ntf, stream);
	r = CClientForDBS::GetInstance()->SendAsync(CClientForDBSAsync::EASyncOP_User_Info_OP_Req, pUser->UserID(), db_req);
	if (!r)
	{
		delete db_req;
		return false;
	}
	return true;
}

bool CServerForUser::handle_msg_skill_level_up_req( uv_stream_t* stream, const char * buf, int len )
{
	e_msg_c_gs req_msg_id = e_msg_c_gs_skill_level_up_req;
	e_msg_gs_c res_msg_id = e_msg_gs_c_skill_level_up_res;
	c_gs_skill_level_up_req req;
	gs_c_skill_level_up_res res;

	CUser* pUser = NULL;
	e_gsc_errorcode error_code = check(stream, req_msg_id, &req, buf, len, pUser);
	if (error_code != e_gsc_errorcode_success)
	{
		res.set_error_code(error_code);
		send(stream, res_msg_id, &res);
		return false;
	}
	ASSERT(pUser);

	//1.�ж���������Ƿ��д�Ӣ��
	db_user_hero* hero = pUser->GetHero(req.hero_id());
	if (!hero)//
	{
		res.set_error_code(e_gsc_errorcode_op_not_support);
		send(stream, res_msg_id, &res);
		return false;
	}

	//�Ҽ��� ���ҿ�������
	db_user_hero_skill* skill = pUser->GetHeroSkill(req.hero_id(), req.skill_type());
	if (!skill || skill->level() == 0 || (skill->level() + req.level()) > MAXNUM_SKILLLEVEL || skill->level() >= hero->level())
	{//����δ���ţ�=0�� ���� �������ȼ� ���� Ӣ�۵ȼ����� ����������
		res.set_error_code(e_gsc_errorcode_op_not_support);
		send(stream, res_msg_id, &res);
		return false;
	}

	//�жϵ���
	UINT32 cur_value = pUser->GetSkillPoint();
	if(cur_value < req.level()){//��������
		res.set_error_code(e_gsc_errorcode_op_not_support);
		send(stream, res_msg_id, &res);
		return false;
	}

	//�жϽ������
	UINT32 need_gold = 0;
	for (UINT32 i = skill->level() ; i < (skill->level() + req.level()) ; ++i)
	{
		stSkillLevelNeed* need = CSkillMgr::GetInstance()->GetLevelNeed(i);
		ASSERT(need);
		need_gold += need->arrNeedGold[req.skill_type()];
	}

	if (pUser->GetGold() < need_gold)
	{
		res.set_error_code(e_gsc_errorcode_gold_not_enough);
		send(stream, res_msg_id, &res);
		return false;
	}

	gs_dbs_user_info_op_req* db_req = new gs_dbs_user_info_op_req;
	db_req->set_user_id(pUser->UserID());

//����
	//�ͻ���
	struct_game_element* c_consume = res.add_consume();
	c_consume->set_type(struct_game_element::EELEMENTTYPE_SKILL_POINT);
	c_consume->set_id(0);
	c_consume->set_num_type(struct_game_element::DECREASE);
	c_consume->set_num(req.level());

	//����gs����ڴ� �� db������Ϣ
	//bool r = pUser->UpdateGameElement(c_consume, db_req);
	bool isFull2NotFull = false;
	bool r = pUser->DecreaseSkillPoint(req.level(), &isFull2NotFull);
	ASSERT(r);

	if (isFull2NotFull)
	{
		c_consume->set_sub_id(pUser->GetSkillPointLastUpdateTime());//����ͻ��˱���ֵ�����Զ��ظ����ֵ���� �ڿ�ȥ����������ֵ ����֮�� ��ʹ�����﷢��������ʱ��Ϊ����ʱ���
	}
	else{
		c_consume->set_sub_id(0);
	}

	c_consume = res.add_consume();
	c_consume->set_type(struct_game_element::EELEMENTTYPE_GOLD);
	c_consume->set_id(0);
	c_consume->set_num_type(struct_game_element::DECREASE);
	c_consume->set_num(need_gold);


	gs_dbs_user_info_op_req::struct_op_base_info* db_base = db_req->mutable_base_info();
	db_base->set_op_type(gs_dbs_user_info_op_req::Update);
	db_base->set_skill_point(pUser->GetSkillPoint());
	db_base->set_skill_point_update_timestamp(pUser->GetSkillPointLastUpdateTime());
	db_base->set_gold(pUser->GetGold()-need_gold);

	pUser->SetGold(pUser->GetGold()-need_gold);
//����
	//����gs��skill����
	skill->set_level(skill->level()+req.level());

	//����db
	gs_dbs_user_info_op_req::struct_op_hero_skill* db_skill = db_req->add_hero_skills();
	db_skill->set_op_type(gs_dbs_user_info_op_req::Update);
	db_skill->set_hero_id(req.hero_id());
	db_skill->set_type(req.skill_type());
	db_skill->set_level(skill->level());///ǰ���Ѿ����£�

	res.set_error_code(e_gsc_errorcode_success);
	res.set_hero_id(req.hero_id());
	res.set_skill_type(req.skill_type());
	res.set_level(req.level());
	send(stream, res_msg_id, &res);
	
	GS_C_NTF ntf;
	pUser->TriggerMission(EEventType_Skill_Level_Up, EEventSubType_None, *db_req, ntf);
	_send_gs_c_ntf(ntf, stream);

	r = CClientForDBS::GetInstance()->SendAsync(CClientForDBSAsync::EASyncOP_User_Info_OP_Req, pUser->UserID(), db_req);
	if (!r)
	{
		delete db_req;
		return false;
	}
	return true;
}

bool CServerForUser::handle_msg_buy_req(uv_stream_t* stream, const char * buf, int len)
{
	DECLEAR_IN_HANDLE_FUNCTION(buy);
	CUser* pUser = NULL;
	e_gsc_errorcode error_code = check(stream, req_msg_id, &req, buf, len, pUser);
	if (error_code != e_gsc_errorcode_success)
	{
		res.set_error_code(error_code);
		send(stream, res_msg_id, &res);
		return false;
	}
	ASSERT(pUser);
	
	CVipCfg* pVipCfg = CVipMgr::GetInstance()->GetVipCfgByVipLevel(pUser->GetVipLevel());
	ASSERT(pVipCfg);
	gs_dbs_user_info_op_req* db_req = NULL;
	GS_C_NTF ntf;

	struct_game_element::EELEMENTTYPE eBuyType = req.buy_type();
	UINT32 iCostDiamond = 0;
	switch (eBuyType)
	{
	case struct_game_element::EELEMENTTYPE_GOLD:
		{
			UINT32 iUsedTimes = pUser->GetEventTodayValue(EEventType_Buy_Gold, EEventSubType_None);
			ASSERT(iUsedTimes <= pVipCfg->m_buy_num_gold);

			if(pVipCfg->m_buy_num_gold <= iUsedTimes){
				res.set_error_code(e_gsc_errorcode_not_enough_times);
				send(stream, res_msg_id, &res);
				return false;
			}

			UINT32 iDiamond = pUser->GetDiamond();
			iCostDiamond = CVipMgr::GetInstance()->GetCostDiamond(EBUY_GOLD, iUsedTimes + 1);
			if(iDiamond < iCostDiamond)
			{
				res.set_error_code(e_gsc_errorcode_not_enough_diamond);
				send(stream, res_msg_id, &res);
				return false;
			}

			UINT32 iCoinOrign;
			ASSERT(CSysSettingMgr::GetInstance()->GetIntValue("Buy_Coin_Default", iCoinOrign));
			UINT32 iCoinGrow;
			ASSERT(CSysSettingMgr::GetInstance()->GetIntValue("Buy_Coin_Grow", iCoinGrow));

			db_req = new gs_dbs_user_info_op_req;
			db_req->set_user_id(pUser->UserID());
			UINT32 iGetGold = iCoinOrign + iCoinGrow * iUsedTimes;

			{//�ڴ�ͬ��
				pUser->DecreaseDiamond(iCostDiamond);
				pUser->IncreaseGold(iGetGold);
				//pUser->UpdateEventInfo//�˴����ø��£���Ϊ�����TriggerMission������ڴ�����ݿ�
			}
			db_req->mutable_base_info()->set_gold(pUser->GetGold());
			{//c res
				res.set_error_code(e_gsc_errorcode_success);
				res.set_buy_type(struct_game_element::EELEMENTTYPE_GOLD);
				struct_game_element* pOutput = res.mutable_output();
				pOutput->set_id(0);
				pOutput->set_num(iGetGold);
				pOutput->set_num_type(struct_game_element::INCREASE);
				pOutput->set_type(struct_game_element::EELEMENTTYPE_GOLD);

				res.set_buy_used_times(iUsedTimes + 1);
			}
			pUser->TriggerMission(EEventType_Buy_Gold, EEventSubType_None, *db_req, ntf);
		}
		break;
	case struct_game_element::EELEMENTTYPE_HEALTH:
		{
			//�ж��Ƿ�ﵽ����
			UINT32 iCurHealth = pUser->GetHealth();
			UINT32 iMaxHealth = CUserMgr::GetMaxHealthPro();
			if (iCurHealth >= iMaxHealth)
			{
				res.set_error_code(e_gsc_errorcode_health_reach_max);
				send(stream, res_msg_id, &res);
				return false;
			}
			
			//�жϹ������
			UINT32 iUsedTimesHealth = pUser->GetEventTodayValue(EEventType_Buy_Health, EEventSubType_None);
			UINT32 iDiamond = pUser->GetDiamond();
			iCostDiamond = CVipMgr::GetInstance()->GetCostDiamond(EBUY_HEALTH, iUsedTimesHealth + 1);


			if(pVipCfg->m_buy_num_health > iUsedTimesHealth)
			{
				if(iDiamond < iCostDiamond)
				{
					res.set_error_code(e_gsc_errorcode_not_enough_diamond);
					send(stream, res_msg_id, &res);
					return false;
				}
			}
			else
			{
				res.set_error_code(e_gsc_errorcode_not_enough_times);
				send(stream, res_msg_id, &res);
				return false;
			}
			
			db_req = new gs_dbs_user_info_op_req;
			db_req->set_user_id(pUser->UserID());
			db_req->mutable_base_info()->set_health_cur(iCurHealth);
			{//�ڴ�ͬ��
				pUser->DecreaseDiamond(iCostDiamond);
				UINT32 iHealth = 0;
				ASSERT(CSysSettingMgr::GetInstance()->GetIntValue("Buy_Health_Point", iHealth));
				iCurHealth = pUser->IncreaseHealth(iHealth);
				pUser->UpdateEventInfo(EEventType_Buy_Health, EEventSubType_None, *db_req, Now(), 1);

			}

			{//c res
				res.set_error_code(e_gsc_errorcode_success);
				res.set_buy_type(struct_game_element::EELEMENTTYPE_HEALTH);
				struct_game_element* pOutput = res.mutable_output();
				pOutput->set_id(0);
				pOutput->set_num(iCurHealth);
				pOutput->set_num_type(struct_game_element::TOTAL);
				pOutput->set_type(struct_game_element::EELEMENTTYPE_HEALTH);
				res.set_buy_used_times(iUsedTimesHealth + 1);
			}
		}
		break;
	case struct_game_element::EELEMENTTYPE_SKILL_POINT:
		{
			//�鿴�Ƿ񵽴ﹺ������(���ܵ�Ϊ0)
			UINT32 iCurSkill = pUser->GetSkillPoint();
			if (iCurSkill != 0)
			{
				res.set_error_code(e_gsc_errorcode_op_not_support);
				send(stream, res_msg_id, &res);
				return false;
			}

			//�鿴vip�ȼ��Ƿ�����
			UINT32 iVipLevel = 0;
			ASSERT(CSysSettingMgr::GetInstance()->GetIntValue("Vip_Level_Function_Buy_Skill_Point", iVipLevel));

			if(pUser->GetVipLevel() < iVipLevel)
			{
				res.set_error_code(e_gsc_errorcode_vip_level_not_reach);
				send(stream, res_msg_id, &res);
				return false;
			}

			UINT32 iUsedTimesSkill = pUser->GetEventTodayValue(EEventType_Buy_Skill_Point, EEventSubType_None);
			//���ܵ㹺������ж�
			CVipCfg* pCfg = CVipMgr::GetInstance()->GetVipCfgByVipLevel(pUser->GetVipLevel());
			ASSERT(pCfg);
			if (!pCfg)
			{
				res.set_error_code(e_gsc_errorcode_op_not_support);
				send(stream, res_msg_id, &res);
				return false;
			}

			if(iUsedTimesSkill >= pCfg->m_buy_skill_point){
				res.set_error_code(e_gsc_errorcode_not_enough_times);
				send(stream, res_msg_id, &res);
				return false;
			}

			UINT32 iDiamond = pUser->GetDiamond();
			iCostDiamond = CVipMgr::GetInstance()->GetCostDiamond(EBUY_SKILL, iUsedTimesSkill + 1);

			if(iDiamond < iCostDiamond)
			{
				res.set_error_code(e_gsc_errorcode_not_enough_diamond);
				send(stream, res_msg_id, &res);
				return false;
			}
			
			db_req = new gs_dbs_user_info_op_req;
			db_req->set_user_id(pUser->UserID());
			{//�ڴ�ͬ��
				pUser->DecreaseDiamond(iCostDiamond);
				UINT32 iSkill = 0;
				ASSERT(CSysSettingMgr::GetInstance()->GetIntValue("Buy_Skill_Point", iSkill));
				pUser->IncreaseSkillPoint(iSkill);
				pUser->UpdateEventInfo(EEventType_Buy_Skill_Point, EEventSubType_None, *db_req, Now(), 1);
			}

			db_req->mutable_base_info()->set_skill_point(pUser->GetSkillPoint());

			{//c res
				res.set_error_code(e_gsc_errorcode_success);

				res.set_buy_type(struct_game_element::EELEMENTTYPE_SKILL_POINT);

				struct_game_element* pOutput = res.mutable_output();
				pOutput->set_id(0);
				UINT32 iSkill = 0;
				ASSERT(CSysSettingMgr::GetInstance()->GetIntValue("Buy_Skill_Point", iSkill));
				pOutput->set_num(iSkill);
				pOutput->set_num_type(struct_game_element::INCREASE);
				pOutput->set_type(struct_game_element::EELEMENTTYPE_SKILL_POINT);

				res.set_buy_used_times(iUsedTimesSkill + 1);

			}
		}
		break;
	case struct_game_element::EELEMENTTYPE_ITEM:
		{
		/*	req.*/
		}
		break;
	case struct_game_element::EELEMENTTYPE_SUB_TASK_ENTER_NUM:
		{
			if (!req.has_element_id()){
				res.set_error_code(e_gsc_errorcode_unknown_error);
				send(stream, res_msg_id, &res);
				return false;
			}
			db_req = new gs_dbs_user_info_op_req;
			db_req->set_user_id(pUser->UserID());
			e_gsc_errorcode r = pUser->ResetSubTaskEnterNum(pVipCfg->m_buy_num_instance, req.element_id(), res, *db_req, iCostDiamond);
			res.set_error_code(r);
			if (r != e_gsc_errorcode_success){
				send(stream, res_msg_id, &res);
				delete db_req;
				return false;
			}
		}
		break;
	default:
		{
			res.set_error_code(e_gsc_errorcode_op_not_support);
			send(stream, res_msg_id, &res);
			return false;
		}
	}

	struct_game_element* pConEle = res.add_consume();
	pConEle->set_id(0);
	pConEle->set_num(iCostDiamond);
	pConEle->set_num_type(struct_game_element::DECREASE);
	pConEle->set_type(struct_game_element::EELEMENTTYPE_DIAMOND);
	
	db_req->mutable_base_info()->set_op_type(gs_dbs_user_info_op_req::Update);
	db_req->mutable_base_info()->set_diamond(pUser->GetDiamond());

	_send_gs_c_ntf(ntf, stream);
	bool r = CClientForDBS::GetInstance()->SendAsync(CClientForDBSAsync::EASyncOP_User_Info_OP_Req, pUser->UserID(), db_req);
	if (!r)
	{
		delete db_req;
		return false;
	}

	send(stream, res_msg_id, &res);

	return true;
}

bool CServerForUser::handle_msg_invoke_req( uv_stream_t* stream, const char * buf, int len )
{
	e_msg_c_gs req_msg_id = e_msg_c_gs_invoke_req;
	e_msg_gs_c res_msg_id = e_msg_gs_c_invoke_res;
	c_gs_invoke_req req;
	gs_c_invoke_res res;

	CUser* pUser = NULL;
	e_gsc_errorcode error_code = check(stream, req_msg_id, &req, buf, len, pUser);
	if (error_code != e_gsc_errorcode_success){
		res.set_error_code(error_code);
		send(stream, res_msg_id, &res);
		return false;
	}
	ASSERT(pUser);

	/*
	GOLD_FREE = 1;	//�����ѵ����ٻ�
	GOLD_1 = 2;	//��ҵ����ٻ�
	GOLD_N = 3;	//��������ٻ�
	DIAMOND_FREE = 4;	//��ʯ��ѵ����ٻ�
	DIAMOND_1 = 5;	//��ʯ�����ٻ�
	DIAMOND_N = 6;	//��ʯ�����ٻ�
	*/

	gs_dbs_user_info_op_req* db_req = new gs_dbs_user_info_op_req;
	db_req->set_user_id(pUser->UserID());

	bool isFirstTime = false;
	EInvokeType invoke_type = EInvokeType_MIN;
	EEventSubType event_sub_type = EEventSubType_Min;
	UINT32 invoke_num = 1;//�ٻ��Ĵ��� ֻ���������Բ�����1
	switch (req.invoke_type())
	{
		case c_gs_invoke_req::GOLD_FREE:
			invoke_type = EInvokeType_GOLD_FREE;
			//free���Ը���ʱ������ж��Ƿ��ǵ�һ�� ���� 1������ô�а취֪���� ���Զ���event����type
			event_sub_type = EEventSubType_Invoke_GOLD_FREE;
			isFirstTime = ( 0 == pUser->GetEventTotalValue(EEventType_Invoke, event_sub_type));
			break;
		case c_gs_invoke_req::GOLD_1:
			invoke_type = EInvokeType_GOLD_1;
			event_sub_type = EEventSubType_Invoke_GOLD_1;
			isFirstTime = ( 0 == pUser->GetEventTotalValue(EEventType_Invoke, event_sub_type));
			break;
		case c_gs_invoke_req::GOLD_N:
			isFirstTime = pUser->isFirstInvokeGoldN();
			invoke_type = EInvokeType_GOLD_N;
			break;
		case c_gs_invoke_req::DIAMOND_FREE:
			invoke_type = EInvokeType_DIAMOND_FREE;
			event_sub_type = EEventSubType_Invoke_DIAMOND_FREE;
			isFirstTime = ( 0 == pUser->GetEventTotalValue(EEventType_Invoke, event_sub_type));
			break;
		case c_gs_invoke_req::DIAMOND_1:
			invoke_type = EInvokeType_DIAMOND_1;
			event_sub_type = EEventSubType_Invoke_DIAMOND_1;
			isFirstTime = ( 0 == pUser->GetEventTotalValue(EEventType_Invoke, event_sub_type));
			break;
		case c_gs_invoke_req::DIAMOND_N:
			isFirstTime = pUser->isFirstInvokeDiamondN();
			invoke_type = EInvokeType_DIAMOND_N;
			break;
		case c_gs_invoke_req::VIP_DIAMOND_1:{

			UINT32 value;
			bool r = CSysSettingMgr::GetInstance()->GetIntValue("Vip_Level_Function_Invoke_Vip", value);
			ASSERT(r);
			if(!r){
				res.set_error_code(e_gsc_errorcode_unknown_error);
				send(stream, res_msg_id, &res);
				delete db_req;
				return false;
			}

			if (pUser->GetVipLevel() < value){
				res.set_error_code(e_gsc_errorcode_vip_level_not_reach);
				send(stream, res_msg_id, &res);
				delete db_req;
				return false;
			}

			invoke_type = EInvokeType_VIP_DIAMOND_1;
			isFirstTime = false;//����Ҫ�״��߼�
		}	break;
		case c_gs_invoke_req::VIP_DIAMOND_N:{
			UINT32 value;
			bool r = CSysSettingMgr::GetInstance()->GetIntValue("Vip_Level_Function_Invoke_Vip", value);
			ASSERT(r);
			if(!r){
				res.set_error_code(e_gsc_errorcode_unknown_error);
				send(stream, res_msg_id, &res);
				delete db_req;
				return false;
			}

			if (pUser->GetVipLevel() < value){
				res.set_error_code(e_gsc_errorcode_vip_level_not_reach);
				send(stream, res_msg_id, &res);
				delete db_req;
				return false;
			}

			invoke_type = EInvokeType_VIP_DIAMOND_N;
			isFirstTime = false;//����Ҫ�״��߼�
		}	break;
		case c_gs_invoke_req::FRIEND_POINT : {
			invoke_type = EInvokeType_FRIEND_POINT;
			isFirstTime = false;//����Ҫ�״��߼�

			if (req.has_invoke_num()){
				invoke_num = req.invoke_num();
				ASSERT(invoke_num > 0);
			}

		} break;

		default:
			res.set_error_code(e_gsc_errorcode_op_not_support);
			send(stream, res_msg_id, &res);
			delete db_req;
			return false;
	}

	vector<stGameElement> output;
	vector<stGameElement> consume;
	bool r = CInvokeMgr::GetInstance()->Invoke(invoke_type, invoke_num, output, consume, isFirstTime);
	ASSERT(r);
	if(!r){
		res.set_error_code(e_gsc_errorcode_unknown_error);
		send(stream, res_msg_id, &res);
		delete db_req;
		return false;
	}

	if (!pUser->IsConsumeEnough(consume)){
		res.set_error_code(e_gsc_errorcode_no_enough_consume);
		send(stream, res_msg_id, &res);
		delete db_req;
		return false;
	}

	GS_C_NTF ntf;
	//����ٻ�ʱ �۳�����
	if(invoke_type == EInvokeType_GOLD_FREE){
		bool r = pUser->DecreaseInvokeGoldFreeLeftNum(1);
		if (!r){
			res.set_error_code(e_gsc_errorcode_op_not_support);
			send(stream, res_msg_id, &res);
			delete db_req;
			return false;
		}

		struct_game_element* c_consume = res.add_consume();
		c_consume->set_type(struct_game_element::EELEMENTTYPE_INVOKE_GOLD_FREE);
		c_consume->set_id(0);
		c_consume->set_sub_id(pUser->GetInvokeGoldFreeUpdateTime());
		c_consume->set_num_type(struct_game_element::DECREASE);
		c_consume->set_num(1);

		gs_dbs_user_info_op_req::struct_op_base_info* db_op_base = db_req->mutable_base_info();
		db_op_base->set_op_type(gs_dbs_user_info_op_req::Update);
		db_op_base->set_invoke_gold_free_left_num(pUser->GetInvokeGoldFreeLeftNum());
		db_op_base->set_invoke_gold_free_update_timestamp(pUser->GetInvokeGoldFreeUpdateTime());
	}
	else if (invoke_type == EInvokeType_DIAMOND_FREE){
		bool isFull2NotFull = false;
		bool r = pUser->DecreaseInvokeDiamondFreeLeftNum(1, &isFull2NotFull);
		if (!r){
			res.set_error_code(e_gsc_errorcode_op_not_support);
			send(stream, res_msg_id, &res);
			delete db_req;
			return false;
		}

		struct_game_element* c_consume = res.add_consume();
		c_consume->set_type(struct_game_element::EELEMENTTYPE_INVOKE_DIAMOND_FREE);
		c_consume->set_id(0);
		if (isFull2NotFull){
			c_consume->set_sub_id(pUser->GetInvokeDiamondFreeUpdateTime());
		}
		else{
			c_consume->set_sub_id(0);
		}

		c_consume->set_num_type(struct_game_element::DECREASE);
		c_consume->set_num(1);

		gs_dbs_user_info_op_req::struct_op_base_info* db_op_base = db_req->mutable_base_info();
		db_op_base->set_op_type(gs_dbs_user_info_op_req::Update);
		db_op_base->set_invoke_diamond_free_left_num(pUser->GetInvokeDiamondFreeLeftNum());
		db_op_base->set_invoke_diamond_free_update_timestamp(pUser->GetInvokeDiamondFreeUpdateTime());
	}
	else{//������ٻ�ʱ �۳�����
		bool r = pUser->UpdateGameElement(consume, *db_req, *res.mutable_consume(), ntf);
		ASSERT(r);
		if (r){//�㹻�۵�ʱ�� ǰ���Ѿ������㹻���ж���
			if (invoke_type == EInvokeType_GOLD_N){
				//���� db �� gs�� ����ٻ��ܴ���
				r = pUser->IncreaseInvokeGoldNTotalNum(*db_req);
				ASSERT(r);
			}
			else if (invoke_type == EInvokeType_DIAMOND_N){
				//���� db �� gs�� ����ٻ��ܴ���
				r = pUser->IncreaseInvokeDiamondNTotalNum(*db_req);
				ASSERT(r);
			}
			//�����������Ҫ����
		}
		else{
			res.set_error_code(e_gsc_errorcode_no_enough_consume);
			send(stream, res_msg_id, &res);
			delete db_req;
			return false;
		}
	}

	//����ƴ���
	for (vector<stGameElement>::iterator it = output.begin(); it != output.end(); ++it){
		//���û�и�Ӣ�� �����ٻ�����Ӣ��
		if(EELEMENTTYPE_HERO == it->eType && !pUser->GetHero(it->iID)){
			CHeroCfg* pCfg = CHeroMgr::GetInstance()->GetHeroByHeroID(it->iID);
			ASSERT(pCfg);
			if (CMarqueeMgr::GetInstance()->CanTriggerMarquee(gs_c_marquee_ntf::MARQUEE_INVOKE, pCfg->iStar)){
				std::map<std::string, CValue> values;

				CValue& vUserName = values["user_name"];
				vUserName.SetStringValue(pUser->Name().c_str());

				CValue& vHeroID = values["hero_id"];
				vHeroID.SetIntValue(pCfg->iHeroID);
				CMarqueeMgr::GetInstance()->SendMarqueeToAllUsers(gs_c_marquee_ntf::MARQUEE_INVOKE, values);
			}
		}
	}

	//��ȡ����
	r = pUser->UpdateGameElement(output, *db_req, *res.mutable_ouput(), ntf, res.mutable_heros());
	ASSERT(r);
	
	if (event_sub_type == EEventSubType_Invoke_GOLD_FREE
		||event_sub_type == EEventSubType_Invoke_GOLD_1
		||event_sub_type == EEventSubType_Invoke_DIAMOND_FREE
		||event_sub_type == EEventSubType_Invoke_DIAMOND_1
		){//����N���Ѿ��洢����user���� ʵ����freeҲ���Դ�user�����ж� ���Ǻ���׼��ͳһ�Ƶ��¼���
		pUser->UpdateEventInfo(EEventType_Invoke, event_sub_type, *db_req, Now(), 1);
	}
	
	res.set_error_code(e_gsc_errorcode_success);
	send(stream, res_msg_id, &res);
	pUser->TriggerMission(EEventType_Invoke, EEventSubType_None, *db_req, ntf);
	_send_gs_c_ntf(ntf, stream);

	r = CClientForDBS::GetInstance()->SendAsync(CClientForDBSAsync::EASyncOP_User_Info_OP_Req, pUser->UserID(), db_req);
	if (!r){
		delete db_req;
		return false;
	}

	return true;
}

bool CServerForUser::handle_msg_server_time_req(uv_stream_t* stream, const char * buf, int len)
{
	e_msg_c_gs req_msg_id = e_msg_c_gs_server_time_req;
	e_msg_gs_c res_msg_id = e_msg_gs_c_server_time_res;
	c_gs_server_time_req req;
	gs_c_server_time_res res;

	CUser* pUser = NULL;
	e_gsc_errorcode error_code = check(stream, req_msg_id, &req, buf, len, pUser);
	if (error_code != e_gsc_errorcode_success)
	{
		res.set_error_code(error_code);
		send(stream, res_msg_id, &res);
		return false;
	}
	ASSERT(pUser);
	
	res.set_server_time(Now());
	res.set_error_code(e_gsc_errorcode_success);
	send(stream, res_msg_id, &res);

	return true;
}

void CServerForUser::SendDBReq( gs_dbs_user_info_op_req* db_req )
{
	ASSERT(db_req);
	if (CClientForDBS::GetInstance()->IsDBReqInitialized(db_req)){
		bool r = CClientForDBS::GetInstance()->SendAsync(CClientForDBSAsync::EASyncOP_User_Info_OP_Req, db_req->user_id(), db_req);
		if (!r){
			delete db_req;
			printf("GS ����̸߳�db�߳� �����޸���һ�����Ϣ[%u]���첽����ʧ�ܣ�GS����ȻҪ������CUser�ڴ���Ϣ��\n", db_req->user_id());
		}
	}
	else{
		printf("_send_db_req failed!! IsDBReqInitialized is false!\n");
		delete db_req;
	}

}



bool CServerForUser::_send_gs_c_ntf(GS_C_NTF& ntf,  uv_stream_t* stream)
{
	ASSERT(stream);
	CHECK_NULL_RETURN_FALSE(stream);
	

	if(ntf.is_mail_ntf_arr_init())
	{
		send(stream, e_msg_gs_c_mail_ntf, ntf.mutable_mail_ntf_arr());
	}

	if(ntf.is_mission_ntf_arr_init())
	{
		send(stream, e_msg_gs_c_mission_ntf,ntf.mutable_mis_ntf_arr());
	}
	return true;
}

//bool CServerForUser::handle_msg_get_activity_task_req( uv_stream_t* stream, const char * buf, int len )
//{
//	DECLEAR_IN_HANDLE_FUNCTION(get_activity_task);
//
//	CUser* pUser = NULL;
//	e_gsc_errorcode error_code = check(stream, req_msg_id, &req, buf, len, pUser);
//	if (error_code != e_gsc_errorcode_success)
//	{
//		res.set_error_code(error_code);
//		send(stream, res_msg_id, &res);
//		return false;
//	}
//	ASSERT(pUser);
//
//	std::set<UINT32> validActivityTaskIDs;
//	CTaskMgr::GetInstance()->GetShowActivityTask(validActivityTaskIDs);
//	res.set_error_code(e_gsc_errorcode_success);
//	for (std::set<UINT32>::iterator it = validActivityTaskIDs.begin() ; it != validActivityTaskIDs.end(); ++it)
//	{
//		res.mutable_tasks()->Add(*it);
//	}
//	
//	send(stream, res_msg_id, &res);
//
//	return true;
//}

bool CServerForUser::handle_msg_diamond_speed_kill_req( uv_stream_t* stream, const char * buf, int len )
{
	DECLEAR_IN_HANDLE_FUNCTION(diamond_speed_kill);
	CUser* pUser = NULL;
	e_gsc_errorcode error_code = check(stream, req_msg_id, &req, buf, len, pUser);
	if (error_code != e_gsc_errorcode_success)
	{
		res.set_error_code(error_code);
		send(stream, res_msg_id, &res);
		return false;
	}
	ASSERT(pUser);

	if (!req.num() || !req.sub_task_id())
	{
		res.set_error_code(e_gsc_errorcode_unknown_error);
		send(stream, res_msg_id, &res);
		return false;
	}
	
	UINT32 Speed_Kill_Limit = 0;
	bool r = CSysSettingMgr::GetInstance()->GetIntValue("Speed_Kill_Limit", Speed_Kill_Limit);
	ASSERT(r);
	if (!r){
		res.set_error_code(e_gsc_errorcode_unknown_error);
		send(stream, res_msg_id, &res);
		return false;
	}
	
	UINT32 Speed_Kill_Diamond_Per = 0;
	r = CSysSettingMgr::GetInstance()->GetIntValue("Speed_Kill_Diamond_Per", Speed_Kill_Diamond_Per);
	ASSERT(r);
	if (!r)
	{
		res.set_error_code(e_gsc_errorcode_unknown_error);
		send(stream, res_msg_id, &res);
		return false;
	}

	if (req.num() % Speed_Kill_Diamond_Per)//�������ʯ��������ȷ
	{
		res.set_error_code(e_gsc_errorcode_unknown_error);
		send(stream, res_msg_id, &res);
		return false;
	}
	
	if (req.num() > 1)
	{
		//�ж�vip�Ƿ�����
		if(!CVipMgr::GetInstance()->VipLevelSatisfy(EVip_Level_Function_Speed_Kill_N, pUser->GetVipLevel())){
			res.set_error_code(e_gsc_errorcode_vip_level_not_reach);
			send(stream, res_msg_id, &res);
			return false;
		}
	}

	if (req.num() > Speed_Kill_Limit * Speed_Kill_Diamond_Per)//�����������ɨ����������ʯ
	{
		res.set_error_code(e_gsc_errorcode_speed_kill_exceed_limit);
		send(stream, res_msg_id, &res);
		return false;
	}

	if(pUser->GetDiamond() < req.num())//��ʯ����
	{
		res.set_error_code(e_gsc_errorcode_not_enough_diamond);
		send(stream, res_msg_id, &res);
		return false;
	}
	
	gs_dbs_user_info_op_req* db_req = new gs_dbs_user_info_op_req;
	db_req->set_user_id(pUser->UserID());
	GS_C_NTF ntf;
	struct_game_element req_ele;
	req_ele.set_type(struct_game_element::EELEMENTTYPE_DIAMOND);
	req_ele.set_id(0);
	req_ele.set_num_type(struct_game_element::DECREASE);
	req_ele.set_num(req.num());

	e_gsc_errorcode errorcode = pUser->SpeedKillSubTaskByDiamond(req.sub_task_id(), req.num() / Speed_Kill_Diamond_Per, res, req_ele, *db_req, ntf);
	res.set_error_code(errorcode);

	send(stream, res_msg_id, &res);
	
	//�����ɾ�����
	if (e_gsc_errorcode_success == errorcode)
	{
		pUser->TriggerMission(EEventType_Complete_Chapter_FB, EEventSubType_HuaXia, *db_req, ntf, req.num() / Speed_Kill_Diamond_Per);

		stSubTask* SubTaskCfg = CTaskMgr::GetInstance()->GetSubTaskBySubTaskID(req.sub_task_id());
		if (SubTaskCfg)
		{
			CTask* pTask = CTaskMgr::GetInstance()->GetTaskByTaskID(SubTaskCfg->iTaskID);
			if (pTask)
			{
				switch(pTask->eType)
				{
				case CTask::ETASKTYPE_MAIN:
					{
						pUser->TriggerMission(EEventType_Mission_Main, EEventSubType_None, *db_req, ntf, req.num());//ͨ������

					}
					break;
				case CTask::ETASKTYPE_BRANCH://֧��
					pUser->TriggerMission(EEventType_Mission_Free, EEventSubType_None, *db_req, ntf, req.num());//ͨ��֧��(��Ӣ)
					break;
				case CTask::ETASKTYPE_ACTIVITY://�
				case CTask::ETASKTYPE_EXP:
				case CTask::ETASKTYPE_GOLD:
					pUser->TriggerMission(EEventType_Activity, EEventSubType_None, *db_req, ntf, req.num());//ͨ�ػ
					break;	
				}

			}
		}
	}

	_send_gs_c_ntf(ntf, stream);
	SendDBReq(db_req);
	return true;
}


bool CServerForUser::handle_msg_use_item_req( uv_stream_t* stream, const char * buf, int len )
{
		DECLEAR_IN_HANDLE_FUNCTION(use_item);
	
		CUser* pUser = NULL;
		e_gsc_errorcode error_code = check(stream, req_msg_id, &req, buf, len, pUser);
		if (error_code != e_gsc_errorcode_success)
		{
			res.set_error_code(error_code);
			send(stream, res_msg_id, &res);
			return false;
		}
		ASSERT(pUser);

		if (!req.item_id() || !req.num()){
			res.set_error_code(e_gsc_errorcode_unknown_error);
			send(stream, res_msg_id, &res);
			return false;
		}

		CItemCfg* cfg = CItemMgr::GetInstance()->GetItemCfgByItemID(req.item_id());
		//ASSERT(cfg);
		if (!cfg){//�������ò�����
			res.set_error_code(e_gsc_errorcode_unknown_error);
			send(stream, res_msg_id, &res);
			return false;
		}

		if(!pUser->IsItemEnough(req.item_id(), req.num())){//���߲���
			res.set_error_code(e_gsc_errorcode_not_enough_item);
			send(stream, res_msg_id, &res);
			return false;
		}

		//db������һ�����Ϣ
		gs_dbs_user_info_op_req* db_req = NULL;
		GS_C_NTF ntf;

		switch (req.item_id())
		{
		case 601:{//ɨ��ȯ

				//ɨ������
				//1 ��������Ǽ�  ��
				//2 vip�ȼ� ��ͬ��vip�ȼ� ��ͬ����������ɨ��   ��һ ���� vip�ȼ���һ�� ÿ�����Ҳ��һ����
				//3.�ǻ���� <--- �κ����񶼿��� ֻҪvip	��
				//4.����		��
				//5.ʣ����ս���� ��
				//6.ɨ��ȯ��������	��
				//7.�����ڵ������ɨ������ ��
				//8.��ڼ�Ҫ�������

				UINT32 Speed_Kill_Limit = 0;
				bool r = CSysSettingMgr::GetInstance()->GetIntValue("Speed_Kill_Limit", Speed_Kill_Limit);
				if (!r){
					res.set_error_code(e_gsc_errorcode_unknown_error);
					send(stream, res_msg_id, &res);
					return false;
				}

				if (req.num() > 1)
				{
					//�ж�vip�Ƿ�����
					if(!CVipMgr::GetInstance()->VipLevelSatisfy(EVip_Level_Function_Speed_Kill_N, pUser->GetVipLevel())){
						res.set_error_code(e_gsc_errorcode_vip_level_not_reach);
						send(stream, res_msg_id, &res);
						return false;
					}
				}

				//�����ڵ������ɨ������
				if (req.num() > Speed_Kill_Limit){
					res.set_error_code(e_gsc_errorcode_speed_kill_exceed_limit);
					send(stream, res_msg_id, &res);
					return false;
				}
				struct_game_element consume;
				consume.set_type(struct_game_element::EELEMENTTYPE_ITEM);
				consume.set_id(req.item_id());
				consume.set_sub_id(0);
				consume.set_num_type(struct_game_element::DECREASE);
				consume.set_num(req.num());
				
				db_req = new gs_dbs_user_info_op_req;
				db_req->set_user_id(pUser->UserID());
				e_gsc_errorcode errorcode = pUser->SpeedKillSubTaskByItem(req.target_id(), req.num(), res, consume, *db_req, ntf);
				res.set_error_code(errorcode);
				if (e_gsc_errorcode_success == errorcode)
				{//�����ɾ�����
					pUser->TriggerMission(EEventType_Complete_Chapter_FB, EEventSubType_HuaXia, *db_req, ntf, req.num());

					stSubTask* SubTaskCfg = CTaskMgr::GetInstance()->GetSubTaskBySubTaskID(req.target_id());
					if (SubTaskCfg)
					{
						CTask* pTask = CTaskMgr::GetInstance()->GetTaskByTaskID(SubTaskCfg->iTaskID);
						if (pTask)
						{
							switch(pTask->eType)
							{
							case CTask::ETASKTYPE_MAIN:
								{
									pUser->TriggerMission(EEventType_Mission_Main, EEventSubType_None, *db_req, ntf, req.num());//ͨ������

								}
								break;
							case CTask::ETASKTYPE_BRANCH://֧��
								pUser->TriggerMission(EEventType_Mission_Free, EEventSubType_None, *db_req, ntf, req.num());//ͨ��֧��(��Ӣ)
								break;
							case CTask::ETASKTYPE_ACTIVITY://�
							case CTask::ETASKTYPE_EXP:
							case CTask::ETASKTYPE_GOLD:
								pUser->TriggerMission(EEventType_Activity, EEventSubType_None, *db_req, ntf, req.num());//ͨ�ػ
								break;	
							}

						}
					}
				}
				
				
				/*stSubTask* sub_task = CTaskMgr::GetInstance()->GetSubTaskBySubTaskID(req.target_id());
				if(!sub_task){//������������
					res.set_error_code(e_gsc_errorcode_unknown_error);
					send(stream, res_msg_id, &res);
					return false;
				}

				CTask* task = CTaskMgr::GetInstance()->GetTaskByTaskID(sub_task->iTaskID);
				if(!task || task->bHidden){//��⸸������� ����û�����أ�
					res.set_error_code(e_gsc_errorcode_unknown_error);
					send(stream, res_msg_id, &res);
					return false;
				}

				//�ж�vip ����ɨ��ʱ��ͬ��������Ƶ�������� if else��˳��
				switch (task->eType)
				{
				case CTask::ETASKTYPE_MAIN:
					{
						UINT32 Vip_Level_Function_Speed_Kill_Main_Task = -1;
						bool r = CSysSettingMgr::GetInstance()->GetIntValue("Vip_Level_Function_Speed_Kill_Main_Task", Vip_Level_Function_Speed_Kill_Main_Task);
						ASSERT(r);
						if (pUser->GetVipLevel() < Vip_Level_Function_Speed_Kill_Main_Task){
							res.set_error_code(e_gsc_errorcode_vip_level_not_reach);
							send(stream, res_msg_id, &res);
							return false;
						}
					}
					break;
				case CTask::ETASKTYPE_BRANCH:
					{
						UINT32 Vip_Level_Function_Speed_Kill_Free_Task = -1;
						bool r = CSysSettingMgr::GetInstance()->GetIntValue("Vip_Level_Function_Speed_Kill_Free_Task", Vip_Level_Function_Speed_Kill_Free_Task);
						ASSERT(r);
						if (pUser->GetVipLevel() < Vip_Level_Function_Speed_Kill_Free_Task){
							res.set_error_code(e_gsc_errorcode_vip_level_not_reach);
							send(stream, res_msg_id, &res);
							return false;
						}
					}
					break;
				case CTask::ETASKTYPE_ACTIVITY:
				case CTask::ETASKTYPE_EXP:
				case CTask::ETASKTYPE_GOLD:
					{
						UINT32 Vip_Level_Function_Speed_Kill_Activity_Task = -1;
						bool r = CSysSettingMgr::GetInstance()->GetIntValue("Vip_Level_Function_Speed_Kill_Activity_Task", Vip_Level_Function_Speed_Kill_Activity_Task);
						ASSERT(r);
						if (pUser->GetVipLevel() < Vip_Level_Function_Speed_Kill_Activity_Task){
							res.set_error_code(e_gsc_errorcode_vip_level_not_reach);
							send(stream, res_msg_id, &res);
							return false;
						}
					}
					break;
				default:
					ASSERT(false && "��֧�ֵ���������");
					res.set_error_code(e_gsc_errorcode_unknown_error);
					send(stream, res_msg_id, &res);
					return false;
				}
				
				//�ж��Ǽ�
				db_user_sub_task* pSubTask = pUser->GetSubTask(req.target_id());
				if (!pSubTask || pSubTask->star() < 3){//�Ǽ�����
					res.set_error_code(e_gsc_errorcode_speed_kill_not_enough_star);
					send(stream, res_msg_id, &res);
					return false;
				}

				//�ж�����
				UINT32 need_health = sub_task->iSpendHealth * req.num();
				if(pUser->GetHealth() < need_health){
					res.set_error_code(e_gsc_errorcode_no_enough_health);
					send(stream, res_msg_id, &res);
					return false;
				}
			
				//�ж�ʣ��������
				if(pUser->GetSubTaskLeftEnterNum(sub_task->iSubTaskID) < req.num()){
					res.set_error_code(e_gsc_errorcode_speed_kill_not_enough_left_enter_num);
					send(stream, res_msg_id, &res);
					return false;
				}

//������
				bool isFull2NotFull = false;
				r = pUser->DecreaseHealth(need_health, &isFull2NotFull);
				ASSERT(r);

				struct_game_element* ele = res.add_consume();
				ele->set_type(struct_game_element::EELEMENTTYPE_HEALTH);
				ele->set_id(0);
				if (isFull2NotFull)
				{
					ele->set_sub_id(pUser->GetHealthLastUpdateTime());//����ͻ��˱������������Զ��ظ����ֵ���� �ڿ�ȥ�������������� ����֮�� ��ʹ�����﷢��������ʱ��Ϊ����ʱ���
				}
				else{
					ele->set_sub_id(0);
				}
				ele->set_num_type(struct_game_element::DECREASE);
				ele->set_num(need_health);


				//m_start_sub_task_res.set_health_last_update_timestamp(_get_health_last_update_time());//�Ƶ� ���� element��sub_id�ϡ� ����ͻ��˱������������Զ��ظ����ֵ���� �ڿ�ȥ�������������� ����֮�� ��ʹ�����﷢��������ʱ��Ϊ����ʱ���

				//db������һ�����Ϣ
				db_req = new gs_dbs_user_info_op_req;
				db_req->set_user_id(pUser->UserID());
				//�޸�db������Ϣ
				gs_dbs_user_info_op_req_struct_op_base_info* base_info = db_req->mutable_base_info();
				base_info->set_op_type(gs_dbs_user_info_op_req::Update);
				base_info->set_health_cur(pUser->GetHealth());
				base_info->set_health_last_update_timestamp(pUser->GetHealthLastUpdateTime());

//�۵���
				struct_game_element consume;
				consume.set_type(struct_game_element::EELEMENTTYPE_ITEM);
				consume.set_id(req.item_id());
				consume.set_sub_id(0);
				consume.set_num_type(struct_game_element::DECREASE);
				consume.set_num(req.num());

				r = pUser->UpdateGameElement(consume, *db_req, *res.mutable_consume(), ntf);
				ASSERT(r);

//�ӵ���
				bool hasRandomOpenShop = false;
				for (UINT32 i = 0 ; i < req.num() ; ++i){
					gs_c_use_item_res::struct_game_element_array* eles_arr = res.add_items_arr();//����ɨ���Ľ��, ����˴�û�в��� ��ô�����Ǹ��յļ� �ͻ��˿�����ʾ��
					
					std::vector<stGameElement> bursts;
					bool r = CBurstMgr::GetInstance()->GetBurst(sub_task->iAutoBattleBurstPacketID, bursts);
					ASSERT(r);
					if (!r){
						res.set_error_code(e_gsc_errorcode_unknown_error);
						send(stream, res_msg_id, &res);
						return false;
					}

					if (sub_task->iExtraPacketID && IsConditionsSatisfy(sub_task->ExtraPacketCondition, pUser->UserID())){
						r = CBurstMgr::GetInstance()->GetBurst(sub_task->iExtraPacketID, bursts);
						ASSERT(r);
						if (!r){
							res.set_error_code(e_gsc_errorcode_unknown_error);
							send(stream, res_msg_id, &res);
							return false;
						}
					}

					r = pUser->UpdateGameElement(bursts, *db_req, *(eles_arr->mutable_items()), ntf);
					ASSERT(r);

					if (!hasRandomOpenShop && (task->eType == CTask::ETASKTYPE_MAIN || task->eType == CTask::ETASKTYPE_BRANCH)){
						bool r = pUser->OnShopRandomOpen(task->eType, *db_req, *res.mutable_shop());
						if (!r){//���û�п��� �̵� ���ͷ��̵�
							res.set_allocated_shop(NULL);
						}
						else{
							hasRandomOpenShop = true;//�����˾Ͳ���������� û�����ż��������
						}
					}
				}

				res.set_error_code(e_gsc_errorcode_success);*/
			}
			break;
		case 501:	//����ҩˮ��΢����
		case 502:	//����ҩˮ��С��
		case 503:	//����ҩˮ���У�
		case 504:	//����ҩˮ����
		case 505:	//����ҩˮ������
			{//����ҩˮ��Ӣ��ʹ�ã�
				UINT32 hero_id = req.target_id();
				db_user_hero* hero = pUser->GetHero(hero_id);
				//�ж�Ӣ�۵ľ�������û��
				if (!hero){
					res.set_error_code(e_gsc_errorcode_no_this_hero);
					send(stream, res_msg_id, &res);
					return false;
				}

				if (CHeroMgr::GetInstance()->IsExpFull(hero->level(), hero->cur_exp(), pUser->GetLevel())){
					res.set_error_code(e_gsc_errorcode_hero_exp_full);
					send(stream, res_msg_id, &res);
					return false;
				}

				//db������һ�����Ϣ
				db_req = new gs_dbs_user_info_op_req;
				db_req->set_user_id(pUser->UserID());

				//�۵���
				struct_game_element consume;
				consume.set_type(struct_game_element::EELEMENTTYPE_ITEM);
				consume.set_id(req.item_id());
				consume.set_sub_id(0);
				consume.set_num_type(struct_game_element::DECREASE);
				consume.set_num(req.num());

				bool r = pUser->UpdateGameElement(consume, *db_req, *res.mutable_consume(), ntf);
				ASSERT(r);

				//�����߲���
				struct_game_element_array* eles_arr = res.add_items_arr();//���ν��
				for (UINT32 i = 0 ; i < req.num() ; ++i){
					//bool r = pUser->UpdateGameElement(cfg->m_use_output, *db_req, *(eles_arr->mutable_items()), client_ntf);
					//ASSERT(r);

					//����Ҫ����ָ��Ӣ��
					for( std::vector<stGameElement*>::iterator it = cfg->m_use_output.begin(); it != cfg->m_use_output.end() ; ++it){
						stGameElement* st = *it;
						ASSERT(st);
						if (NULL == st){
							delete db_req;
							res.set_error_code(e_gsc_errorcode_unknown_error);
							send(stream, res_msg_id, &res);
							return false;
						}

						struct_game_element ele;
						bool r = st->Fill(ele);
						ASSERT(r);
						if (!r){
							delete db_req;
							res.set_error_code(e_gsc_errorcode_unknown_error);
							send(stream, res_msg_id, &res);
							return false;
						}

						//Ӣ�۾���ʱ��
						if(st->eType == EELEMENTTYPE_EXP && st->iID == 2){
							ele.set_sub_id(hero_id);//����Ӣ��id
						}
					
						r = pUser->UpdateGameElement(ele, *db_req, *(eles_arr->mutable_items()), ntf);
						ASSERT(r);
						if (!r){
							delete db_req;
							res.set_error_code(e_gsc_errorcode_unknown_error);
							send(stream, res_msg_id, &res);
							return false;
						}
					}
				}

				res.set_error_code(e_gsc_errorcode_success);
			}
			break;
		case 801://����ҩ��(������ʹ��)
			{
				//�ж���ҵ������Ƿ�����
				if(pUser->IsHealthFull())
				{
					res.set_error_code(e_gsc_errorcode_health_reach_max);
					send(stream, res_msg_id, &res);
					return false;
				}
				
				
				ASSERT(cfg->m_use_output.size() == 1);
				if (cfg->m_use_output.size() != 1)
				{
					res.set_error_code(e_gsc_errorcode_unknown_error);
					send(stream, res_msg_id, &res);
					return false;
				}
				ASSERT((*cfg->m_use_output.begin())->eType == EELEMENTTYPE_HEALTH);
				if ((*cfg->m_use_output.begin())->eType != EELEMENTTYPE_HEALTH)
				{
					res.set_error_code(e_gsc_errorcode_unknown_error);
					send(stream, res_msg_id, &res);
					return false;
				}

				UINT iCfgHealth = (*cfg->m_use_output.begin())->iNum;//����ҩ����õ�����ֵ

				//������ʹ�õĸ���
				UINT32 iMaxHealthPro = CUserMgr::GetMaxHealthPro();

				UINT32 iCanUse = 1;
				for (; iCanUse < req.num() + 1;)
				{
					if (pUser->GetHealth() + iCanUse * iCfgHealth > iMaxHealthPro)
					{
						break;
					}

					if (++iCanUse >= req.num() + 1)
					{
						iCanUse--;
						break;
					}
				}
				
				//�۳�����
				struct_game_element consume;
				consume.set_type(struct_game_element::EELEMENTTYPE_ITEM);
				consume.set_id(req.item_id());
				consume.set_sub_id(0);
				consume.set_num_type(struct_game_element::DECREASE);
				consume.set_num(iCanUse);

				db_req = new gs_dbs_user_info_op_req;
				db_req->set_user_id(pUser->UserID());

				bool r = pUser->UpdateGameElement(consume, *db_req, *res.mutable_consume(), ntf);
				ASSERT(r);
				
				//����Ҽ�����
				struct_game_element_array* eles_arr = res.add_items_arr();
				struct_game_element* ele = eles_arr->add_items();

				UINT32 iPreHealth = pUser->GetHealth();
				UINT32 iCurHealth = pUser->IncreaseHealth(iCfgHealth * iCanUse);
				ele->set_type(struct_game_element::EELEMENTTYPE_HEALTH);
				ele->set_id(0);
				ele->set_num_type(struct_game_element::INCREASE);
				ele->set_num(iCurHealth - iPreHealth);
				res.set_error_code(e_gsc_errorcode_success);

				db_req->mutable_base_info()->set_op_type(gs_dbs_user_info_op_req::Update);
				db_req->mutable_base_info()->set_health_cur(pUser->GetHealth());
				db_req->mutable_base_info()->set_health_last_update_timestamp(pUser->GetHealthLastUpdateTime());

				//for(UINT32 i = 0; i < iCanUse; ++i){
				//	stGameElement* st = *cfg->m_use_output.begin();
				//	ASSERT(st);
				//	if (NULL == st){
				//		delete db_req;
				//		res.set_error_code(e_gsc_errorcode_unknown_error);
				//		send(stream, res_msg_id, &res);
				//		return false;
				//	}

				//	struct_game_element ele;
				//	bool r = st->Fill(ele);
				//	ASSERT(r);
				//	if (!r){
				//		delete db_req;
				//		res.set_error_code(e_gsc_errorcode_unknown_error);
				//		send(stream, res_msg_id, &res);
				//		return false;
				//	}
				//		
				//	pUser->IncreaseHealth(iCfgHealth);

					/*r = pUser->UpdateGameElement(ele, *db_req, *(eles_arr->mutable_items()), ntf);
					ASSERT(r);
					if (!r){
						delete db_req;
						res.set_error_code(e_gsc_errorcode_unknown_error);
						send(stream, res_msg_id, &res);
						return false;
					}*/
	/*			}*/
				
			}
			break;
		default:
			//ASSERT(false && "���߲�֧��ʹ�ã�");
			res.set_error_code(e_gsc_errorcode_item_can_not_use);
			send(stream, res_msg_id, &res);
			return false;
			break;
		}

		//�ȷ��ظ����ͻ���
		send(stream, res_msg_id, &res);

		//����������ʼ� �ͷ��ʼ�
		_send_gs_c_ntf(ntf, stream);

		if (db_req){
			bool ret = CClientForDBS::GetInstance()->SendAsync(CClientForDBSAsync::EASyncOP_User_Info_OP_Req, pUser->UserID(), db_req);
			if (!ret){
				delete db_req;
			}
		}
		
		return true;
}

bool CServerForUser::handle_msg_shop_info_req( uv_stream_t* stream, const char * buf, int len )
{
	DECLEAR_IN_HANDLE_FUNCTION(shop_info);
	CUser* pUser = NULL;
	e_gsc_errorcode error_code = check(stream, req_msg_id, &req, buf, len, pUser);
	if (error_code != e_gsc_errorcode_success)
	{
		res.set_error_code(error_code);
		send(stream, res_msg_id, &res);
		return false;
	}
	ASSERT(pUser);

	gs_dbs_user_info_op_req* db_req = new gs_dbs_user_info_op_req;
	db_req->set_user_id(pUser->UserID());

	bool r = pUser->GetShopInfo(req, *db_req, res);
	if (!r){
		send(stream, res_msg_id, &res);
		return false;
	}

	send(stream, res_msg_id, &res);

	SendDBReq(db_req);
	return true;
}

bool CServerForUser::handle_msg_shop_buy_item_req( uv_stream_t* stream, const char * buf, int len )
{
	DECLEAR_IN_HANDLE_FUNCTION(shop_buy_item);

	CUser* pUser = NULL;
	e_gsc_errorcode error_code = check(stream, req_msg_id, &req, buf, len, pUser);
	if (error_code != e_gsc_errorcode_success)
	{
		res.set_error_code(error_code);
		send(stream, res_msg_id, &res);
		return false;
	}
	ASSERT(pUser);

	gs_dbs_user_info_op_req* db_req = new gs_dbs_user_info_op_req;
	db_req->set_user_id(pUser->UserID());

	GS_C_NTF ntf;

	bool r = pUser->ShopBuyItem((CShop::ESHOPTYPE)req.shop_type(), req.pos(), *db_req, res, ntf);
	if (!r){
		send(stream, res_msg_id, &res);
		return false;
	}

	send(stream, res_msg_id, &res);

	_send_gs_c_ntf(ntf, stream);
	SendDBReq(db_req);

	return true;
}

bool CServerForUser::handle_msg_shop_refresh_req( uv_stream_t* stream, const char * buf, int len )
{
	DECLEAR_IN_HANDLE_FUNCTION(shop_refresh);
	CUser* pUser = NULL;
	e_gsc_errorcode error_code = check(stream, req_msg_id, &req, buf, len, pUser);
	if (error_code != e_gsc_errorcode_success)
	{
		res.set_error_code(error_code);
		send(stream, res_msg_id, &res);
		return false;
	}
	ASSERT(pUser);

	gs_dbs_user_info_op_req* db_req = new gs_dbs_user_info_op_req;
	db_req->set_user_id(pUser->UserID());

	GS_C_NTF ntf;

	bool r = pUser->ShopRefresh((CShop::ESHOPTYPE)req.shop_type(), *db_req, res, ntf);
	if (!r){
		send(stream, res_msg_id, &res);
		return false;
	}

	send(stream, res_msg_id, &res);

	_send_gs_c_ntf(ntf, stream);
	
	SendDBReq(db_req);
	return true;
}

bool CServerForUser::handle_msg_arena_refresh_match_req( uv_stream_t* stream, const char * buf, int len )
{
	DECLEAR_IN_HANDLE_FUNCTION(arena_refresh_match);
	CUser* pUser = NULL;
	e_gsc_errorcode error_code = check(stream, req_msg_id, &req, buf, len, pUser);
	if (error_code != e_gsc_errorcode_success)
	{
		res.set_error_code(error_code);
		send(stream, res_msg_id, &res);
		return false;
	}
	ASSERT(pUser);

	//����Ҫdb������
	bool r = pUser->ArenaRefreshMatch(res);
	send(stream, res_msg_id, &res);
	return r;
}

bool CServerForUser::handle_msg_arena_update_team_req( uv_stream_t* stream, const char * buf, int len )
{
	DECLEAR_IN_HANDLE_FUNCTION(arena_update_team);
	CUser* pUser = NULL;
	e_gsc_errorcode error_code = check(stream, req_msg_id, &req, buf, len, pUser);
	if (error_code != e_gsc_errorcode_success)
	{
		res.set_error_code(error_code);
		send(stream, res_msg_id, &res);
		return false;
	}
	ASSERT(pUser);

	gs_dbs_user_info_op_req* db_req = new gs_dbs_user_info_op_req;
	db_req->set_user_id(pUser->UserID());

	bool r = pUser->ArenaUpdateTeam(req, *db_req, res);
	send(stream, res_msg_id, &res);
	SendDBReq(db_req);

	return r;
}

bool CServerForUser::handle_msg_get_mission_reward_req(uv_stream_t* stream, const char * buf, int len)
{
	DECLEAR_IN_HANDLE_FUNCTION(get_mission_reward);
	CUser* pUser = NULL;
	e_gsc_errorcode error_code = check(stream, req_msg_id, &req, buf, len, pUser);
	if (error_code != e_gsc_errorcode_success)
	{
		res.set_error_code(error_code);
		send(stream, res_msg_id, &res);
		return false;
	}
	ASSERT(pUser);

	UINT32 iMissionId = req.mission_id();

	CMission* pMission = CMissionMgr::GetInstance()->GetMissionByMissionID(iMissionId);
	ASSERT(pMission);
	if (!pMission)
	{
		res.set_error_code(e_gsc_errorcode_op_not_support);
		send(stream, res_msg_id, &res);
		return false;
	}
	
	gs_dbs_user_info_op_req* db_req = NULL;
	
	GS_C_NTF ntf;
	UINT32 now = Now();

	switch(pMission->eType)
	{
	case EMISSIONTYPE_MONTH_CARD_REWARD://�¿�
		{
			ASSERT(pMission->iMisChainId == 0);
			if(pMission->iMisChainId != 0)
			{
				return false;
			}
			//�ж��Ƿ����¿���ȡ����
			if (!(iMissionId == MONTH_CARD_REWARD_MISSION1_ID || iMissionId == MONTH_CARD_REWARD_MISSION2_ID))
			{
				res.set_error_code(e_gsc_errorcode_op_not_support);
				send(stream, res_msg_id, &res);
				return false;
			}
			
			//�ж��¿��Ƿ���Ч
			if(pUser->GetMissionMonthCardStatus(iMissionId) == MONTH_CARD_BUY)
			{
				res.set_error_code(e_gsc_errorcode_mission_reward_condition_not_satisfy);
				send(stream, res_msg_id, &res);
				return false;
			}

			//�жϵ����Ƿ���ȡ��
			bool r = pUser->IsMissionAlreadyGetReward(iMissionId);
			if (r)
			{
				res.set_error_code(e_gsc_errorcode_mission_reward_condition_not_satisfy);
				send(stream, res_msg_id, &res);
				return false;
			}
			
			db_req = new gs_dbs_user_info_op_req;
			db_req->set_user_id(pUser->UserID());
			pUser->GetMissionReward(iMissionId, *db_req, res, ntf);
			
			
		}
		break;
	case EMISSIONTYPE_HEALTH_LUNCH:
	case EMISSIONTYPE_HEALTH_DINNER:
	case EMISSIONTYPE_HEALTH_NIGHT:
		{
			ASSERT(pMission->iMisChainId > 0);
			if(pMission->iMisChainId <= 0)
			{
				return false;
			}
			
			if(pUser->IsMissionAlreadyGetReward(iMissionId))
			{
				res.set_error_code(e_gsc_errorcode_mission_already_get_reward);
				send(stream, res_msg_id, &res);
				return false;
			}

			//�жϽ�������
			if(!IsConditionsSatisfy(pMission->enterCondition, pUser->UserID()))
			{
				res.set_error_code(e_gsc_errorcode_mission_reward_condition_not_satisfy);
				send(stream, res_msg_id, &res);
				return false;
			}

			//�ж���ȡ����
			if(!IsConditionsSatisfy(pMission->finishCondition, pUser->UserID(), true, pMission->eType))
			{
				res.set_error_code(e_gsc_errorcode_mission_reward_condition_not_satisfy);
				send(stream, res_msg_id, &res);
				return false;
			}
			
			//�ж���������Ƿ����� 
			if(pUser->IsHealthFull())
			{
				res.set_error_code(e_gsc_errorcode_health_reach_max);
				send(stream, res_msg_id, &res);
				return false;
			}
			db_req = new gs_dbs_user_info_op_req;
			db_req->set_user_id(pUser->UserID());
			pUser->GetMissionReward(iMissionId, *db_req, res, ntf);
		}
		break;
	case EMISSIONTYPE_ACHIEVEMENT:
	case EMISSIONTYPE_DAILY:		
		{
			//�жϸ������Ƿ���ȡ��
			bool r = pUser->IsMissionAlreadyGetReward(iMissionId);
			if (r)
			{
				res.set_error_code(e_gsc_errorcode_mission_already_get_reward);
				send(stream, res_msg_id, &res);
				return false;
			}
			
			//�жϽ�������
			if(!IsConditionsSatisfy(pMission->enterCondition, pUser->UserID()))
			{
				res.set_error_code(e_gsc_errorcode_mission_reward_condition_not_satisfy);
				send(stream, res_msg_id, &res);
				return false;
			}

			//�ж���ȡ����
			if(!IsConditionsSatisfy(pMission->finishCondition, pUser->UserID(), true, pMission->eType))
			{
				res.set_error_code(e_gsc_errorcode_mission_reward_condition_not_satisfy);
				send(stream, res_msg_id, &res);
				return false;
			}
			
			db_req = new gs_dbs_user_info_op_req;
			db_req->set_user_id(pUser->UserID());
			pUser->GetMissionReward(iMissionId, *db_req, res, ntf);
		}
		break;
	default:
		{
			res.set_error_code(e_gsc_errorcode_unknown_error);
			send(stream, res_msg_id, &res);
		}
		return false;
	}
	res.set_error_code(e_gsc_errorcode_success);
	send(stream, res_msg_id, &res);
	if(db_req){
		SendDBReq(db_req);
	}
	
	_send_gs_c_ntf(ntf, stream);
	return true;
}

bool CServerForUser::handle_msg_arena_start_challenge_req( uv_stream_t* stream, const char * buf, int len )
{
	DECLEAR_IN_HANDLE_FUNCTION(arena_start_challenge);
	CUser* pUser = NULL;
	e_gsc_errorcode error_code = check(stream, req_msg_id, &req, buf, len, pUser);
	if (error_code != e_gsc_errorcode_success)
	{
		res.set_error_code(error_code);
		send(stream, res_msg_id, &res);
		return false;
	}
	ASSERT(pUser);

	gs_dbs_user_info_op_req* db_req = new gs_dbs_user_info_op_req;
	db_req->set_user_id(pUser->UserID());

	bool r = pUser->ArenaStartChallenge(req, *db_req, res);
	
	GS_C_NTF ntf;
	pUser->TriggerMission(EEventType_Arena_Challenge, EEventSubType_None, *db_req, ntf);//������Ӯ���㴥������

	send(stream, res_msg_id, &res);
	_send_gs_c_ntf(ntf, stream);
	SendDBReq(db_req);

	return r;
}

bool CServerForUser::handle_msg_arena_end_challenge_req( uv_stream_t* stream, const char * buf, int len )
{
	DECLEAR_IN_HANDLE_FUNCTION(arena_end_challenge);
	CUser* pUser = NULL;
	e_gsc_errorcode error_code = check(stream, req_msg_id, &req, buf, len, pUser);
	if (error_code != e_gsc_errorcode_success){
		res.set_error_code(error_code);
		send(stream, res_msg_id, &res);
		return false;
	}
	ASSERT(pUser);

	gs_dbs_user_info_op_req* db_req = new gs_dbs_user_info_op_req;
	db_req->set_user_id(pUser->UserID());

	GS_C_NTF ntf;
	gs_c_arena_challenge_ntf attacker_ntf;
	gs_c_arena_challenge_ntf defender_ntf;

	bool r = pUser->ArenaEndChallenge(req, *db_req, res, ntf, attacker_ntf, defender_ntf);
	send(stream, res_msg_id, &res);
	SendDBReq(db_req);
	_send_gs_c_ntf(ntf, stream);

	//��֪ͨ ���������Ҳ���� Ҳ��֪ͨ
	UINT32 iDefenderUserID = 0;
	if(attacker_ntf.IsInitialized()){
		ASSERT(defender_ntf.IsInitialized());
		send(stream, e_msg_gs_c_arena_challenge_ntf, &attacker_ntf);
		iDefenderUserID = attacker_ntf.ele().match_user_id();
	}

	if (iDefenderUserID && defender_ntf.IsInitialized()){
		CUser* pDefender = CUserMgr::GetInstance()->GetUserByUserID(iDefenderUserID);
		
		if (pDefender){
			uv_stream_t* con = pDefender->GetConnect();
			ASSERT(con);
			send(con, e_msg_gs_c_arena_challenge_ntf, &defender_ntf);
		}
		//ASSERT(pDefender);
	}
	
	return r;
}

bool CServerForUser::handle_msg_arena_challenge_reset_cd_req( uv_stream_t* stream, const char * buf, int len ){
	DECLEAR_IN_HANDLE_FUNCTION(arena_challenge_reset_cd);
	CUser* pUser = NULL;
	e_gsc_errorcode error_code = check(stream, req_msg_id, &req, buf, len, pUser);
	if (error_code != e_gsc_errorcode_success){
		res.set_error_code(error_code);
		send(stream, res_msg_id, &res);
		return false;
	}
	ASSERT(pUser);

	gs_dbs_user_info_op_req* db_req = new gs_dbs_user_info_op_req;
	db_req->set_user_id(pUser->UserID());

	bool r = pUser->ArenaResetChallengeCD(req, *db_req, res);
	send(stream, res_msg_id, &res);
	SendDBReq(db_req);
	return r;
}

bool CServerForUser::handle_msg_arena_challenge_reset_num_req( uv_stream_t* stream, const char * buf, int len ){
	DECLEAR_IN_HANDLE_FUNCTION(arena_challenge_reset_num);
	CUser* pUser = NULL;
	e_gsc_errorcode error_code = check(stream, req_msg_id, &req, buf, len, pUser);
	if (error_code != e_gsc_errorcode_success){
		res.set_error_code(error_code);
		send(stream, res_msg_id, &res);
		return false;
	}
	ASSERT(pUser);

	gs_dbs_user_info_op_req* db_req = new gs_dbs_user_info_op_req;
	db_req->set_user_id(pUser->UserID());

	bool r = pUser->ArenaResetChallengeNum(req, *db_req, res);
	send(stream, res_msg_id, &res);
	SendDBReq(db_req);
	return r;
}

bool CServerForUser::handle_msg_arena_ranks_req( uv_stream_t* stream, const char * buf, int len )
{
	DECLEAR_IN_HANDLE_FUNCTION(arena_ranks);
	CUser* pUser = NULL;
	e_gsc_errorcode error_code = check(stream, req_msg_id, &req, buf, len, pUser);
	if (error_code != e_gsc_errorcode_success){
		res.set_error_code(error_code);
		send(stream, res_msg_id, &res);
		return false;
	}
	ASSERT(pUser);

	//gs_dbs_user_info_op_req* db_req = new gs_dbs_user_info_op_req;
	//db_req->set_user_id(pUser->UserID());

	bool r = pUser->ArenaRanks(req, res);
	send(stream, res_msg_id, &res);
	//_send_db_req(db_req);
	return r;
}

bool CServerForUser::handle_msg_arena_search_req( uv_stream_t* stream, const char * buf, int len )
{
	DECLEAR_IN_HANDLE_FUNCTION(arena_search);
	CUser* pUser = NULL;
	e_gsc_errorcode error_code = check(stream, req_msg_id, &req, buf, len, pUser);
	if (error_code != e_gsc_errorcode_success){
		res.set_error_code(error_code);
		send(stream, res_msg_id, &res);
		return false;
	}
	ASSERT(pUser);

	//gs_dbs_user_info_op_req* db_req = new gs_dbs_user_info_op_req;
	//db_req->set_user_id(pUser->UserID());

	bool r = pUser->ArenaSearch(req, res);
	send(stream, res_msg_id, &res);
	//_send_db_req(db_req);
	return r;
}

bool CServerForUser::handle_msg_sign_req( uv_stream_t* stream, const char * buf, int len )
{
	DECLEAR_IN_HANDLE_FUNCTION(sign);
	CUser* pUser = NULL;
	e_gsc_errorcode error_code = check(stream, req_msg_id, &req, buf, len, pUser);
	if (error_code != e_gsc_errorcode_success)
	{
		res.set_error_code(error_code);
		send(stream, res_msg_id, &res);
		return false;
	}
	ASSERT(pUser);

	c_gs_sign_req::SIGN_TYPE sign_type = req.sign_type();

	gs_dbs_user_info_op_req* db_req = new gs_dbs_user_info_op_req;
	db_req->set_user_id(pUser->UserID());
	
	GS_C_NTF ntf;
	switch (sign_type)
	{
	case c_gs_sign_req::SIGN_TYPE_1:
		{
			e_gsc_errorcode error_code = pUser->Sign_1(*db_req, res, ntf);

			res.set_error_code(error_code);
			send(stream, res_msg_id, &res);
			
			SendDBReq(db_req);

			_send_gs_c_ntf(ntf, stream);
		}
		break;
	case c_gs_sign_req::SIGN_TYPE_N:
		{
			ASSERT(req.has_reward_type());
			ASSERT(req.reward_type() > sign_n_reward::MIN && req.reward_type() < sign_n_reward::MAX);
			if (!req.has_reward_type())
			{
				res.set_error_code(e_gsc_errorcode_unknown_error);
				send(stream, res_msg_id, &res);
				return false;
			}

			e_gsc_errorcode error_code = pUser->GetSignNReward(req.reward_type(), *db_req, res, ntf);
			res.set_error_code(error_code);
			send(stream, res_msg_id, &res);

			SendDBReq(db_req);

			_send_gs_c_ntf(ntf, stream);

		}
		break;
	default:
		ASSERT(false);
		res.set_error_code(e_gsc_errorcode_unknown_error);
		send(stream, res_msg_id, &res);
		return false;
	}
	return true;
}


bool CServerForUser::handle_msg_sign_add_req( uv_stream_t* stream, const char * buf, int len )
{
	DECLEAR_IN_HANDLE_FUNCTION(sign_add);
	CUser* pUser = NULL;
	e_gsc_errorcode error_code = check(stream, req_msg_id, &req, buf, len, pUser);
	if (error_code != e_gsc_errorcode_success)
	{
		res.set_error_code(error_code);
		send(stream, res_msg_id, &res);
		return false;
	}
	ASSERT(pUser);
	
	gs_dbs_user_info_op_req* db_req = new gs_dbs_user_info_op_req;
	db_req->set_user_id(pUser->UserID());
	GS_C_NTF ntf;
	error_code = pUser->AddSign(*db_req, res, ntf);
	res.set_error_code(error_code);
	send(stream, res_msg_id, &res);

	SendDBReq(db_req);
	_send_gs_c_ntf(ntf, stream);
	return true;
}

bool CServerForUser::handle_msg_head_or_frame_change_req(uv_stream_t* stream, const char * buf, int len)
{
	DECLEAR_IN_HANDLE_FUNCTION(head_or_frame_change);
	CUser* pUser = NULL;
	e_gsc_errorcode error_code = check(stream, req_msg_id, &req, buf, len, pUser);
	if (error_code != e_gsc_errorcode_success)
	{
		res.set_error_code(error_code);
		send(stream, res_msg_id, &res);
		return false;
	}
	ASSERT(pUser);
	gs_dbs_user_info_op_req* db_req = new gs_dbs_user_info_op_req;
	db_req->set_user_id(pUser->UserID());
	pUser->SetHeadOrHeadFrame(req.change_type(), req.target_id(), *db_req, res);
	send(stream, e_msg_gs_c_head_or_frame_change_res, &res);
	SendDBReq(db_req);
	return true;
}

bool CServerForUser::handle_msg_finish_novice_guide_req(uv_stream_t* stream, const char * buf, int len)
{
	DECLEAR_IN_HANDLE_FUNCTION(finish_novice_guide);
	CUser* pUser = NULL;
	e_gsc_errorcode error_code = check(stream, req_msg_id, &req, buf, len, pUser);
	if (error_code != e_gsc_errorcode_success)
	{
		res.set_error_code(error_code);
		send(stream, res_msg_id, &res);
		return false;
	}
	ASSERT(pUser);
	
	gs_dbs_user_info_op_req* db_req = new gs_dbs_user_info_op_req;
	db_req->set_user_id(pUser->UserID());
	
	GS_C_NTF ntf;
	pUser->FinishNoviceGuide(req.guide_id(), *db_req, res, ntf);
	send(stream, e_msg_gs_c_finish_novice_guide_res, &res);
	_send_gs_c_ntf(ntf, stream);
	SendDBReq(db_req);
	return true;
}

bool CServerForUser::handle_msg_edit_player_info_req(uv_stream_t* stream, const char * buf, int len)
{
	DECLEAR_IN_HANDLE_FUNCTION(edit_player_info);
	CUser* pUser = NULL;
	e_gsc_errorcode error_code = check(stream, req_msg_id, &req, buf, len, pUser);
	if (error_code != e_gsc_errorcode_success)
	{
		res.set_error_code(error_code);
		send(stream, res_msg_id, &res);
		return false;
	}
	ASSERT(pUser);

	if (!req.has_name() && !req.has_head_id() && !req.has_head_frame_id() && !req.has_signature())
	{
		res.set_error_code(e_gsc_errorcode_unknown_error);
		send(stream, e_msg_gs_c_edit_player_info_res, &res);
	}

	gs_dbs_user_info_op_req* db_req = new gs_dbs_user_info_op_req;
	db_req->set_user_id(pUser->UserID());
	
	gs_dbs_edit_name_req* name_req = new gs_dbs_edit_name_req;

	pUser->EditPlayerInfo(req, *db_req, *name_req, res);
	
	if (!req.has_name() || (req.has_name() && res.error_code() != e_gsc_errorcode_success))//���Ǳ༭���֣������Ǳ༭���֣���������gs��߼��������(������󳤶ȣ������дʻ�)
	{
		send(stream, e_msg_gs_c_edit_player_info_res, &res);
	}
	
	if (req.has_name() && e_gsc_errorcode_success == res.error_code())
	{
		gs_dbs_edit_name_req* name_req = new gs_dbs_edit_name_req;
		name_req->set_user_id(pUser->UserID());
		name_req->set_name(req.name());
		CClientForDBS::GetInstance()->SendAsync(CClientForDBSAsync::EASyncOP_User_Edit_User_name, pUser->UserID(), name_req);
	}

	SendDBReq(db_req);
	return true;

}

bool CServerForUser::BroadCast(UINT32 msg_id, google::protobuf::Message* pMessage, std::vector<UINT32>* includeUsers /*= NULL*/, std::vector<UINT32>* excludeUsers /*= NULL*/)
{
	if(includeUsers){
		ASSERT(!excludeUsers);
		for (std::vector<UINT32>::iterator it = includeUsers->begin(); it != includeUsers->end(); ++it)
		{
			CUser* pUser = CUserMgr::GetInstance()->GetUserByUserID(*it);
			if (pUser)
			{
				pUser->Send(msg_id, pMessage);
			}
		}
	}
	else{
		std::map<UINT32, CUser*>* allUsers = CUserMgr::GetInstance()->GetOnlineUsers();
		ASSERT(allUsers);

		for (std::map<UINT32, CUser*>::iterator it = allUsers->begin(); it != allUsers->end(); ++it)
		{
			ASSERT(it->second);
			if(excludeUsers){
				if(std::find(excludeUsers->begin(), excludeUsers->end(), it->first) != excludeUsers->end())
					continue;
			}
			if (it->second)
			{
				it->second->Send(msg_id, pMessage);
			}
		}
	}
	return true;
}

bool CServerForUser::handle_msg_json_config_version_req( uv_stream_t* stream, const char * buf, int len )
{
	bool logined = check_logined(stream);
	if (!logined)
	{
		gs_c_json_config_version_res res;
		res.set_error_code(e_gsc_errorcode_not_logined);
		send(stream, e_msg_gs_c_json_config_version_res, &res);
		return false;
	}

	c_gs_json_config_version_req req;
	bool r = req.ParseFromArray(buf, len);
	if (!r)
	{
		PRINTF_PBPARSE_ERROR(e_msg_c_gs_json_config_version_req);
		gs_c_json_config_version_res res;
		res.set_error_code(e_gsc_errorcode_unknown_error);
		send(stream, e_msg_gs_c_json_config_version_res, &res);
		return false;
	}

	BYTE* bin = NULL;
	UINT32 bin_len = CClientConfigMgr::GetInstance()->get_gs_c_json_config_version_bin(bin);
	return send(stream, bin, bin_len);
}

bool CServerForUser::handle_msg_json_config_req( uv_stream_t* stream, const char * buf, int len ){
	bool logined = check_logined(stream);
	if (!logined)
	{
		gs_c_json_config_res res;
		res.set_error_code(e_gsc_errorcode_not_logined);
		send(stream, e_msg_gs_c_json_config_res, &res);
		return false;
	}

	c_gs_json_config_req req;
	bool r = req.ParseFromArray(buf, len);
	if (!r)
	{
		PRINTF_PBPARSE_ERROR(e_msg_c_gs_json_config_req);
		gs_c_json_config_res res;
		res.set_error_code(e_gsc_errorcode_unknown_error);
		send(stream, e_msg_gs_c_json_config_res, &res);
		return false;
	}

	return CClientConfigMgr::GetInstance()->handle_json_config_req(stream, req);
}

bool CServerForUser::handle_msg_user_friend_op_req( uv_stream_t* stream, const char * buf, int len )
{
	DECLEAR_IN_HANDLE_FUNCTION(user_friend_op);
	CUser* pUser = NULL;
	e_gsc_errorcode error_code = check(stream, req_msg_id, &req, buf, len, pUser);
	if (error_code != e_gsc_errorcode_success)
	{
		res.set_error_code(error_code);
		send(stream, res_msg_id, &res);
		return false;
	}
	ASSERT(pUser);
	gs_dbs_user_info_op_req* db_req = new gs_dbs_user_info_op_req;
	db_req->set_user_id(pUser->UserID());
	GS_C_NTF ntf;
	pUser->UserFriendOP(req, res, *db_req, ntf);
	res.set_op_type(req.op_type());

	if (res.IsInitialized())
	{
		send(stream, e_msg_gs_c_user_friend_op_res, &res);
	}

	SendDBReq(db_req);
	pUser->SendNtf(ntf);
	return true;
}

bool CServerForUser::handle_msg_search_user_req( uv_stream_t* stream, const char * buf, int len )
{
	DECLEAR_IN_HANDLE_FUNCTION(search_user);
	CUser* pUser = NULL;
	e_gsc_errorcode error_code = check(stream, req_msg_id, &req, buf, len, pUser);
	if (error_code != e_gsc_errorcode_success)
	{
		res.set_error_code(error_code);
		send(stream, res_msg_id, &res);
		return false;
	}
	ASSERT(pUser);
	
	pUser->SearchUser(req, res);

	if (res.IsInitialized()) 
	{
		send(stream, e_msg_gs_c_search_user_res, &res);
	}else{
		gs_dbs_search_user_req* db_req = new gs_dbs_search_user_req;
		db_req->set_user_id(pUser->UserID());

		if (req.has_user_id()){
			db_req->set_search_id(req.user_id());
		}
		
		if (req.has_name()){
			db_req->set_search_name(req.name());
		}
		CClientForDBS::GetInstance()->SendAsync(CClientForDBSAsync::EASyncOP_User_Search_User, pUser->UserID(), db_req);
	}
	return true;
}

bool CServerForUser::handle_msg_chat_req( uv_stream_t* stream, const char * buf, int len )
{
	DECLEAR_IN_HANDLE_FUNCTION(chat);
	CUser* pUser = NULL;
	e_gsc_errorcode error_code = check(stream, req_msg_id, &req, buf, len, pUser);
	if (error_code != e_gsc_errorcode_success)
	{
		res.set_error_code(error_code);
		send(stream, res_msg_id, &res);
		return false;
	}
	ASSERT(pUser);

	pUser->Chat(req, res);
	send(stream, e_msg_gs_c_chat_res, &res);
	return true;
}

bool CServerForUser::handle_msg_rank_req( uv_stream_t* stream, const char * buf, int len )
{
	DECLEAR_IN_HANDLE_FUNCTION(rank);
	CUser* pUser = NULL;
	e_gsc_errorcode error_code = check(stream, req_msg_id, &req, buf, len, pUser);
	if (error_code != e_gsc_errorcode_success)
	{
		res.set_error_code(error_code);
		send(stream, res_msg_id, &res);
		return false;
	}
	ASSERT(pUser);
	
	if (c_gs_rank_req::RANK_TYPE_IsValid(req.rank_type()))
	{
		bool r = CRankMgr::GetInstance()->GetRank(pUser->UserID(), req.rank_type(), res);
		ASSERT(r);
	}
	else{
		res.set_error_code(e_gsc_errorcode_unknown_error);
	}
	send(stream, e_msg_gs_c_rank_res, &res);
	return true;
}

bool CServerForUser::handle_msg_fight_power_req( uv_stream_t* stream, const char * buf, int len )
{
	DECLEAR_IN_HANDLE_FUNCTION(fight_power);
	CUser* pUser = NULL;
	e_gsc_errorcode error_code = check(stream, req_msg_id, &req, buf, len, pUser);
	if (error_code != e_gsc_errorcode_success)
	{
		res.set_error_code(error_code);
		send(stream, res_msg_id, &res);
		return false;
	}
	ASSERT(pUser);

	gs_dbs_user_info_op_req* db_req = new gs_dbs_user_info_op_req;
	db_req->set_user_id(pUser->UserID());
	gs_dbs_user_info_op_req::struct_op_base_info* base_info = db_req->mutable_base_info();
	base_info->set_op_type(gs_dbs_user_info_op_req::Update);
	base_info->set_fight_power(req.fight_power());
	SendDBReq(db_req);
	return true;
}

bool CServerForUser::handle_msg_crusade_start_req( uv_stream_t* stream, const char * buf, int len )
{
	DECLEAR_IN_HANDLE_FUNCTION(crusade_start);
	CUser* pUser = NULL;
	e_gsc_errorcode error_code = check(stream, req_msg_id, &req, buf, len, pUser);
	if (error_code != e_gsc_errorcode_success)
	{
		res.set_error_code(error_code);
		send(stream, res_msg_id, &res);
		return false;
	}
	ASSERT(pUser);

	//�жϵȼ�
	UINT32 iCLevel = 0;
	bool r = CSysSettingMgr::GetInstance()->GetIntValue("User_Level_Function_Crusade", iCLevel);
	ASSERT(r);

	if (iCLevel > pUser->GetLevel())
	{
		res.set_error_code(e_gsc_errorcode_level_not_enough);
		send(stream, e_msg_gs_c_crusade_start_res, &res);
		return false;
	}

	pUser->CrusadeStart(req, res);
	
	send(stream, e_msg_gs_c_crusade_start_res, &res);
	return true;
}

bool CServerForUser::handle_msg_crusade_end_req( uv_stream_t* stream, const char * buf, int len )
{
	DECLEAR_IN_HANDLE_FUNCTION(crusade_end);
	CUser* pUser = NULL;
	e_gsc_errorcode error_code = check(stream, req_msg_id, &req, buf, len, pUser);
	if (error_code != e_gsc_errorcode_success)
	{
		res.set_error_code(error_code);
		send(stream, res_msg_id, &res);
		return false;
	}
	ASSERT(pUser);
	
	gs_dbs_user_info_op_req* db_req = new gs_dbs_user_info_op_req;
	db_req->set_user_id(pUser->UserID());
	GS_C_NTF ntf;
	pUser->CrusadeEnd(req, res, *db_req, ntf);
	send(stream, e_msg_gs_c_crusade_end_res, &res);
	SendDBReq(db_req);
	pUser->SendNtf(ntf);

	return true;
}

bool CServerForUser::handle_msg_crusade_hero_life_req( uv_stream_t* stream, const char * buf, int len )
{
	DECLEAR_IN_HANDLE_FUNCTION(crusade_hero_life);
	CUser* pUser = NULL;
	e_gsc_errorcode error_code = check(stream, req_msg_id, &req, buf, len, pUser);
	if (error_code != e_gsc_errorcode_success)
	{
		res.set_error_code(error_code);
		send(stream, res_msg_id, &res);
		return false;
	}
	ASSERT(pUser);

	gs_dbs_user_info_op_req* db_req = new gs_dbs_user_info_op_req;
	db_req->set_user_id(pUser->UserID());
	GS_C_NTF ntf;
	pUser->CrusadeHeroLife(req, res, *db_req, ntf);
	send(stream, e_msg_gs_c_crusade_hero_life_res, &res);
	pUser->SendNtf(ntf);
	SendDBReq(db_req);
	return true;
}

bool CServerForUser::handle_msg_crusade_reset_req( uv_stream_t* stream, const char * buf, int len )
{
	DECLEAR_IN_HANDLE_FUNCTION(crusade_reset);
	CUser* pUser = NULL;
	e_gsc_errorcode error_code = check(stream, req_msg_id, &req, buf, len, pUser);
	if (error_code != e_gsc_errorcode_success)
	{
		res.set_error_code(error_code);
		send(stream, res_msg_id, &res);
		return false;
	}
	ASSERT(pUser);

	gs_dbs_user_info_op_req* db_req = new gs_dbs_user_info_op_req;
	db_req->set_user_id(pUser->UserID());
	pUser->CrusadeReset(req, res, *db_req);
	SendDBReq(db_req);
	send(stream, e_msg_gs_c_crusade_reset_res, &res);

	return true;
}

bool CServerForUser::handle_msg_crusade_promote_award_req( uv_stream_t* stream, const char * buf, int len )
{
	DECLEAR_IN_HANDLE_FUNCTION(crusade_promote_award);
	CUser* pUser = NULL;
	e_gsc_errorcode error_code = check(stream, req_msg_id, &req, buf, len, pUser);
	if (error_code != e_gsc_errorcode_success)
	{
		res.set_error_code(error_code);
		send(stream, res_msg_id, &res);
		return false;
	}
	ASSERT(pUser);

	gs_dbs_user_info_op_req* db_req = new gs_dbs_user_info_op_req;
	db_req->set_user_id(pUser->UserID());
	pUser->CrusadePromoteAward(req, res, *db_req);
	SendDBReq(db_req);
	send(stream, e_msg_gs_c_crusade_promote_award_res, &res);

	return true;
}

bool CServerForUser::handle_msg_crusade_buff_refresh_req( uv_stream_t* stream, const char * buf, int len )
{
	DECLEAR_IN_HANDLE_FUNCTION(crusade_buff_refresh);
	CUser* pUser = NULL;
	e_gsc_errorcode error_code = check(stream, req_msg_id, &req, buf, len, pUser);
	if (error_code != e_gsc_errorcode_success)
	{
		res.set_error_code(error_code);
		send(stream, res_msg_id, &res);
		return false;
	}
	ASSERT(pUser);

	gs_dbs_user_info_op_req* db_req = new gs_dbs_user_info_op_req;
	db_req->set_user_id(pUser->UserID());
	GS_C_NTF ntf;
	pUser->CrusadeBuffRefresh(req, res, *db_req, ntf);

	send(stream, e_msg_gs_c_crusade_buff_refresh_res, &res);

	SendDBReq(db_req);
	pUser->SendNtf(ntf);

	return true;
}

int CServerForUser::create_timer()
{
	int r = uv_timer_init(CServerForUser::GetInstance()->m_loop, &CServerForUser::GetInstance()->m_timer);
	ASSERT(r == 0);
	r = uv_timer_start(&CServerForUser::GetInstance()->m_timer, timer_cb, 1000, 1000);
	ASSERT(r == 0);
	return 0;
}

void CServerForUser::timer_cb( uv_timer_t* handle)
{
	ASSERT(handle);
	ASSERT(1 == uv_is_active((uv_handle_t*)handle));

	ASSERT(handle == &CServerForUser::GetInstance()->m_timer);
	UINT32 iArenaTime = 0;
	bool r = CSysSettingMgr::GetInstance()->GetIntValue("Arena_Auto_Prize_Time", iArenaTime);
	ASSERT(r && iArenaTime);

	UINT32 iSecond = GetSecondsOfDay(Now());

	if(iArenaTime == iSecond)//ʱ�䵽������9�㣬���;����������ʼ�
	{
		CArena::GetInstance()->OnTime(Now());
	}
	CRankMgr::GetInstance()->OnTime(Now());
}



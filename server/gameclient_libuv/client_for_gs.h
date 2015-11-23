#ifndef client_for_gs_h__
#define client_for_gs_h__

#include <string>
#include "uv.h"
//#include "libuv_helper.h"

#include <google/protobuf/message.h>
#include "def.h"
//#include "guid.h"
#include "guid_generator.h"

#include "gs_c_proto.pb.h"



class CClientForGSASync{
public:
	enum EASyncOP{
		EASyncOP_None,
		EASyncOP_Enter_GS_Req,
	};

	EASyncOP op;
	void* data;
};

class CClientForGS
{
public:

	typedef bool (*func_message_handle) (uv_stream_t* stream, const char * buf, int len);

	//static CClientForGS* GetInstance(){
	//	static CClientForGS singleton;
	//	return &singleton;
	//}

	CClientForGS():	m_loop(NULL){
		m_user_info = NULL;
		memset(m_mapHandle, 0, sizeof m_mapHandle);
	};

	bool Init();

	//�ȵ�����
	bool Join();

	//ֹͣ�ڲ�uv_runѭ�� �Ӷ���ֹ�߳�
	//bool Stop();

	//������Ϣ
	//bool Send(UINT32 msg_id, google::protobuf::Message* pMessage);

	//���ⲿ�̸߳��������̷߳�����Ϣʹ�õ�
	uv_async_t* CreateAsync();
	void DestroyAsync(uv_async_t* handle);
private:

	//// ��ģ������� ������CClientForAS�����token֮�����������
	//��������gs���߳�
	bool Start();

	static void run(void* arg);

	//��ת��ֹ�˳� ���ҿ�����һЩ��
	static void on_idle(uv_idle_t* handle){
		static UINT64 counter = 0;
		++counter;  
		printf("the counter is:%ld\n", counter);
		//uv_idle_stop(&idler);   //ֹͣ��ת  
	}

	//�������
	static void on_server_connection(uv_connect_t* req, int status);
	static void on_receive(uv_stream_t* stream, ssize_t nread, const uv_buf_t* buf);
	static void after_shutdown(uv_shutdown_t* req, int status);
	static void after_close(uv_handle_t* handle);
	static bool send( uv_stream_t* stream, UINT32 msg_id, google::protobuf::Message* pMessage/*, uv_write_cb write_cb*/);
	static void after_send(uv_write_t* req, int status);

	//������ģ���߳̽���
	static void async_cb(uv_async_t* handle);
	static void after_async_close(uv_handle_t* handle);

	//���ݾ����ȡ�����Ŀͻ��˶���
	static CClientForGS* get_client(uv_handle_t* handle){
		ASSERT(handle && handle->loop);
		return (CClientForGS*)(handle->loop->data);
	}

	//��Ϣ����
	bool process_msg( uv_stream_t* stream, const char * buf, int len );
	bool add_message_handle( e_msg_gs_c msgID, func_message_handle handle );

	static bool handle_msg_enter_gs_res( uv_stream_t* stream, const char * buf, int len );
	static bool handle_msg_config_res( uv_stream_t* stream, const char * buf, int len );
	static bool handle_msg_user_info_res( uv_stream_t* stream, const char * buf, int len );
	static bool handle_msg_start_sub_task_res( uv_stream_t* stream, const char * buf, int len );
	static bool handle_msg_end_sub_task_res( uv_stream_t* stream, const char * buf, int len );
	static bool handle_msg_invoke_hero_res( uv_stream_t* stream, const char * buf, int len );
	static bool handle_msg_leave_gs_res( uv_stream_t* stream, const char * buf, int len );
	static bool handle_msg_equip_level_up_res( uv_stream_t* stream, const char * buf, int len );

	static bool handle_msg_gs_c_mail_ntf( uv_stream_t* stream, const char * buf, int len );
	static bool handle_msg_mail_opt_res( uv_stream_t* stream, const char * buf, int len );
	static bool handle_msg_equip_evolution_res( uv_stream_t* stream, const char * buf, int len );
	static bool handle_msg_hero_evolution_res( uv_stream_t* stream, const char * buf, int len );
	static bool handle_msg_hero_star_up_res( uv_stream_t* stream, const char * buf, int len );

	static bool handle_msg_sell_res( uv_stream_t* stream, const char * buf, int len );
	static bool handle_msg_fragment_compound_res(uv_stream_t* stream, const char * buf, int len );

	static bool handle_msg_skill_level_up_res(uv_stream_t* stream, const char * buf, int len );

	static bool handle_msg_buy_res(uv_stream_t* stream, const char * buf, int len );
	static bool handle_msg_invoke_res( uv_stream_t* stream, const char * buf, int len );
	static bool handle_msg_server_time_res(uv_stream_t* stream, const char * buf, int len);
	//static bool handle_msg_get_activity_task_res(uv_stream_t* stream, const char * buf, int len);
	static bool handle_msg_use_item_res(uv_stream_t* stream, const char * buf, int len);
	
	static bool handle_msg_shop_info_res(uv_stream_t* stream, const char * buf, int len);
	static bool handle_msg_shop_buy_item_res(uv_stream_t* stream, const char * buf, int len);
	static bool handle_msg_shop_refresh_res(uv_stream_t* stream, const char * buf, int len);
	static bool handle_msg_mission_get_reward_res(uv_stream_t* stream, const char * buf, int len);
	static bool handle_msg_mission_ntf(uv_stream_t* stream, const char * buf, int len);
	//���������
	static bool handle_msg_arena_refresh_match_res(uv_stream_t* stream, const char * buf, int len);
	static bool handle_msg_arena_start_challenge_res(uv_stream_t* stream, const char * buf, int len);
	static bool handle_msg_arena_end_challenge_res(uv_stream_t* stream, const char * buf, int len);
	static bool handle_msg_arena_update_team_res(uv_stream_t* stream, const char * buf, int len);

	static bool handle_msg_arena_challenge_reset_cd_res(uv_stream_t* stream, const char * buf, int len);
	static bool handle_msg_arena_challenge_reset_num_res(uv_stream_t* stream, const char * buf, int len);
	static bool handle_msg_arena_challenge_ntf( uv_stream_t* stream, const char * buf, int len );
	static bool handle_msg_arena_ranks_res( uv_stream_t* stream, const char * buf, int len );
	static bool handle_msg_arena_search_res( uv_stream_t* stream, const char * buf, int len );
	static bool handle_msg_sign_res( uv_stream_t* stream, const char * buf, int len );
	static bool handle_msg_sign_add_res( uv_stream_t* stream, const char * buf, int len );
	static bool handle_msg_diamond_speed_kill_res(uv_stream_t* stream, const char * buf, int len );
	static bool handle_msg_head_or_frame_change_res(uv_stream_t* stream, const char * buf, int len );
	static bool handle_msg_finish_novice_guide_res(uv_stream_t* stream, const char * buf, int len );
	static bool handle_msg_edit_player_info_res(uv_stream_t* stream, const char * buf, int len );
	static bool handle_msg_marquee_ntf(uv_stream_t* stream, const char * buf, int len );

	static bool handle_msg_json_config_version_res(uv_stream_t* stream, const char * buf, int len );
	static bool handle_msg_json_config_res(uv_stream_t* stream, const char * buf, int len );
	static bool handle_msg_chat_res(uv_stream_t* stream, const char * buf, int len );
	
	static bool handle_msg_search_user_res(uv_stream_t* stream, const char * buf, int len );
	static bool handle_msg_user_friend_op_res(uv_stream_t* stream, const char * buf, int len );
	static bool handle_msg_user_friend_ntf(uv_stream_t* stream, const char * buf, int len );

	static bool handle_msg_chat_ntf(uv_stream_t* stream, const char * buf, int len );
	static bool handle_msg_rank_res(uv_stream_t* stream, const char * buf, int len );
	static bool handle_msg_crusade_start_res(uv_stream_t* stream, const char * buf, int len );
	static bool handle_msg_crusade_end_res(uv_stream_t* stream, const char * buf, int len );
	static bool handle_msg_crusade_hero_life_res(uv_stream_t* stream, const char * buf, int len );
	static bool handle_crusade_reset_res(uv_stream_t* stream, const char * buf, int len );
	//�������������Ż� ����Ϊ��Ϣ�ţ�
	//std::map<unsigned int, func_message_handle> m_mapHandle;
	func_message_handle m_mapHandle[e_msg_gs_c::e_msg_gs_c_max];

	static UINT32 m_connect_client_num;//�Ѿ������ϵĿͻ�������

	uv_thread_t m_thread;
	uv_tcp_t m_client;
	uv_idle_t m_idler;    //��תhandle ��ֹ�޻ʱ�˳�
	uv_loop_t* m_loop;

	std::string ip;	//GS��IP
	int port;		//GS�Ķ˿�
	Guid token;		//AS�����ͻ��˵ĵ�½��֤token

	//std::vector<BYTE> m_msg_buff;
	//BYTE* m_msg_tmp_buf;


	gs_c_user_info_res* m_user_info;
	typedef struct stMsgTmpBuf
	{
		uv_buf_t buf;
		ULONG used_len;

		stMsgTmpBuf(){
			buf.base = 0;
			buf.len = 0;
			used_len = 0;
		}

	} stMsgTmpBuf;

	stMsgTmpBuf m_msg_tmp_buf;//��һ����Ϣ��������ܲ���ȫ����Ϣ

	friend class CClientForAS;// ��ģ������� ������CClientForAS�����token֮�����������
};

#endif // client_for_gs_h__
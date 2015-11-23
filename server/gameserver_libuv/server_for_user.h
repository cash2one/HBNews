#ifndef server_for_user_h__
#define server_for_user_h__

#include "uv.h"
//#include "libuv_helper.h"

#include <google/protobuf/message.h>
#include "gs_c_proto.pb.h"
#include "dbs_gs_proto.pb.h"
#include "def.h"
//#include "guid.h"
#include "guid_generator.h"
#include "config.h"
class CUser;

class CUserConnectContext{
public:
	enum EUserConnectStatus{
		EUserConnectStatus_None = 0 , //初始是无状态的
		EUserConnectStatus_Connected = 1, //只连接进来了
		EUserConnectStatus_Logined = 2,	//登陆成功 ： as发给gs token， 客户端发来的token 两者一致 判断成功
	};

	CUserConnectContext():stauts(EUserConnectStatus_None){

	}

	Guid ConnectID;	//链接ID
	UINT32 iUserID;		//玩家用户ID
	EUserConnectStatus stauts;
};

//玩家线程开放给外部的异步操作数据
struct CServerForUserAsync{

	//玩家线程开放给外部的异步操作
	enum EAsyncOP{
		EAsyncOP_None = 0,
		EAsyncOP_Check_Token_Res = 1,//选区时 gs线程发给user线程
		EAsyncOP_User_Info_Res = 2,
		EAsyncOP_New_Mail_Res = 3,
		EAsyncOP_User_Edit_User_Name_Res = 4,
		EAsyncOP_New_Arena_New_Mail_Res = 5,
		EAsyncOP_Search_User_Res = 6,
		EAsyncOP_Add_Friend_Res = 7,
		EAsyncOP_Rank_Res = 8,
		//---------以上gm命令-----
		EAsyncOP_GM_Add_Item = 1001,	//gm给玩家增加道具
		EAsyncOP_GM_Marquee = 1002,		//gm跑马灯
		EAsyncOP_GM_Mail = 1003,		//gm邮件命令
		EAsyncOP_GM_Arena_Mail = 1004,	//竞技场定时发邮件

		//以下ps服务器使用
		EAsyncOP_PS_Paied = 2001,//ps通知gs玩家已支付 要给东西
	};

	EAsyncOP op;

	//for as
	Guid user_connect_id;//玩家链接ID ( 或者 gm客户端链接id)， 回调到线程这里来了 为了让线程知道是哪个链接的 好回消息 
	UINT32 user_id;//玩家用户ID
	void* data;

	//for gm  todo union

};

class CServerForUser
{
public:

	typedef bool (*func_message_handle) (uv_stream_t* stream, const char * buf, int len);

	static CServerForUser* GetInstance(){
		static CServerForUser singleton;
		return &singleton;
	}

	bool Init();

	//启动玩家连接服务线程
	bool Start();

	//等到结束
	bool Join();

	//给外部线程给本网络线程发送消息使用的
	//不要使用这两个函数！ 准备移到protect！
	uv_async_t* CreateAsync();
	void DestroyAsync(uv_async_t* handle);

	void SendAsync(Guid& sender_client_id, CServerForUserAsync::EAsyncOP eAsyncOP, google::protobuf::Message* data);

	static bool send( uv_stream_t* stream, UINT32 msg_id, google::protobuf::Message* pMessage/*, uv_write_cb write_cb*/);
	static bool send( uv_stream_t* stream, BYTE* buf, UINT32 buf_len);

	static void SendDBReq( gs_dbs_user_info_op_req* db_req );
	/************************************************************************/
	/* 
	includeUsers:指定发送的玩家，如果发给全服在线玩家 该参数不用设置
	excludeUsers:发给全服在线玩家中排除的玩家
	*/
	/************************************************************************/
	static bool BroadCast(UINT32 msg_id, google::protobuf::Message* pMessage, std::vector<UINT32>* includeUsers = NULL, std::vector<UINT32>* excludeUsers = NULL);
protected:

	//发送消息
	//bool Send(UINT32 GSID, UINT32 msg_id, google::protobuf::Message* pMessage);

	//bool BroadCast();

	static void run(void* arg);

	//空转防止退出 并且可以做一些事
	static void on_idle(uv_idle_t* handle){
		static UINT64 counter = 0;
		++counter;  
		printf("the counter is:%ld\n", counter);
		//uv_idle_stop(&idler);   //停止空转  
	}

	//客户端相关
	static void on_client_connection(uv_stream_t* stream, int status);
	static void on_receive(uv_stream_t* stream, ssize_t nread, const uv_buf_t* buf);
	static void after_client_shutdown(uv_shutdown_t* req, int status);
	static void after_client_close(uv_handle_t* handle);
	
	static void after_send(uv_write_t* req, int status);
	static void shutdown_client(uv_stream_t*  stream);
	
	//与其他模块线程交互
	static void async_cb(uv_async_t* handle);
	static void after_async_close(uv_handle_t* handle);
	
	
private:

	CServerForUser():m_loop(NULL){
		memset(m_mapHandle, 0, sizeof m_mapHandle);
	};

	//此线程自己使用 所以不需要线程安全


	CUserConnectContext* attach_context_on_client( uv_tcp_t* client );
	CUserConnectContext* get_context_on_client( uv_tcp_t* client );
	bool detach_context_on_client( uv_tcp_t* client );

	bool add_client(uv_tcp_t* client);
	bool remove_client(Guid& connect_id, bool free_tcp_t = true);
	uv_tcp_t* get_client( Guid& connect_id );
	bool add_message_handle( e_msg_c_gs msgID, func_message_handle handle );
	static bool check_logined(uv_stream_t* stream);
	bool process_msg( uv_stream_t* stream, const char * buf, int len );

	/*static void timer_cb(uv_timer_t* handle);*/

	//消息处理函数
	static e_gsc_errorcode check( uv_stream_t* stream, e_msg_c_gs req_msg_id, google::protobuf::Message* pInReq, const void* buf, int len, CUser* &pOutUser);

	static bool handle_msg_enter_gs_req(uv_stream_t* stream, const char * buf, int len);
	static bool handle_msg_leave_gs_req(uv_stream_t* stream, const char * buf, int len);
	static bool handle_msg_config_req(uv_stream_t* stream, const char * buf, int len);
	static bool handle_msg_user_info_req(uv_stream_t* stream, const char * buf, int len);
	static bool handle_msg_start_sub_task_req(uv_stream_t* stream, const char * buf, int len);
	static bool handle_msg_sell_req(uv_stream_t* stream, const char * buf, int len);
	static bool handle_msg_fragment_compound_req(uv_stream_t* stream, const char * buf, int len);
	static bool handle_msg_mail_opt_req(uv_stream_t* stream, const char * buf, int len);
	static bool handle_msg_end_sub_task_req( uv_stream_t* stream, const char * buf, int len );
	static bool handle_msg_invoke_hero_req( uv_stream_t* stream, const char * buf, int len );
	static bool handle_msg_equip_level_up_req( uv_stream_t* stream, const char * buf, int len );
	static bool handle_msg_equip_evolution_req(uv_stream_t* stream, const char * buf, int len);
	static bool handle_msg_hero_evolution_req(uv_stream_t* stream, const char * buf, int len);
	static bool handle_msg_hero_star_up_req(uv_stream_t* stream, const char * buf, int len);
	static bool handle_msg_buy_req(uv_stream_t* stream, const char * buf, int len);

	static bool handle_msg_skill_level_up_req(uv_stream_t* stream, const char * buf, int len);
	static bool handle_msg_invoke_req(uv_stream_t* stream, const char * buf, int len);
	static bool handle_msg_server_time_req(uv_stream_t* stream, const char * buf, int len);

	//static bool handle_msg_get_activity_task_req(uv_stream_t* stream, const char * buf, int len);
	static bool handle_msg_use_item_req(uv_stream_t* stream, const char * buf, int len);
	static bool handle_msg_diamond_speed_kill_req(uv_stream_t* stream, const char * buf, int len);
	static bool handle_msg_shop_buy_item_req(uv_stream_t* stream, const char * buf, int len);
	static bool handle_msg_shop_refresh_req(uv_stream_t* stream, const char * buf, int len);
	static bool handle_msg_shop_info_req( uv_stream_t* stream, const char * buf, int len );
	static bool handle_msg_get_mission_reward_req(uv_stream_t* stream, const char * buf, int len );

	static bool handle_msg_arena_refresh_match_req(uv_stream_t* stream, const char * buf, int len);
	static bool handle_msg_arena_update_team_req(uv_stream_t* stream, const char * buf, int len);
	static bool handle_msg_arena_start_challenge_req(uv_stream_t* stream, const char * buf, int len);
	static bool handle_msg_arena_end_challenge_req(uv_stream_t* stream, const char * buf, int len);
	static bool handle_msg_arena_challenge_reset_cd_req(uv_stream_t* stream, const char * buf, int len);
	static bool handle_msg_arena_challenge_reset_num_req(uv_stream_t* stream, const char * buf, int len);
	//static bool handle_msg_arena_log_req(uv_stream_t* stream, const char * buf, int len);
	static bool handle_msg_arena_ranks_req(uv_stream_t* stream, const char * buf, int len);
	static bool handle_msg_arena_search_req(uv_stream_t* stream, const char * buf, int len);
	static bool handle_msg_sign_req(uv_stream_t* stream, const char * buf, int len);
	static bool handle_msg_sign_add_req(uv_stream_t* stream, const char * buf, int len);
	
	static bool handle_msg_head_or_frame_change_req(uv_stream_t* stream, const char * buf, int len);
	static bool handle_msg_finish_novice_guide_req(uv_stream_t* stream, const char * buf, int len);
	static bool handle_msg_edit_player_info_req(uv_stream_t* stream, const char * buf, int len);
	static bool _send_gs_c_ntf(GS_C_NTF& ntf, uv_stream_t* stream);

	//获取客户端配置信息相关
	static bool handle_msg_json_config_version_req(uv_stream_t* stream, const char * buf, int len);
	static bool handle_msg_json_config_req(uv_stream_t* stream, const char * buf, int len);
	static bool handle_msg_user_friend_op_req(uv_stream_t* stream, const char * buf, int len);
	static bool handle_msg_search_user_req(uv_stream_t* stream, const char * buf, int len);
	static bool handle_msg_chat_req(uv_stream_t* stream, const char * buf, int len);
	static bool handle_msg_fight_power_req(uv_stream_t* stream, const char * buf, int len);
	static bool handle_msg_rank_req(uv_stream_t* stream, const char * buf, int len);
	static bool handle_msg_crusade_start_req(uv_stream_t* stream, const char * buf, int len);
	static bool handle_msg_crusade_end_req(uv_stream_t* stream, const char * buf, int len);
	static bool handle_msg_crusade_hero_life_req(uv_stream_t* stream, const char * buf, int len);
	static bool handle_msg_crusade_reset_req(uv_stream_t* stream, const char * buf, int len);
	static bool handle_msg_crusade_promote_award_req(uv_stream_t* stream, const char * buf, int len);
	static bool handle_msg_crusade_buff_refresh_req(uv_stream_t* stream, const char * buf, int len);
	
	static int create_timer();
	static void timer_cb(uv_timer_t* handle);
	//可以用数组来优化 索引为消息号！
	//std::map<unsigned int, func_message_handle> m_mapHandle;
	func_message_handle m_mapHandle[e_msg_c_gs_max];

	uv_thread_t m_thread;
	uv_tcp_t m_server;
	uv_loop_t* m_loop;
	uv_timer_t m_timer;//定时器

	//std::map<UINT32, uv_tcp_t*> m_clients;//因为不能不管那些还么有确认user_id的链接！
	std::map<Guid, uv_tcp_t*> m_clients;
};
#endif // server_for_user_h__
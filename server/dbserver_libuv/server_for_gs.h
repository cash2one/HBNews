#ifndef server_for_gs_h__
#define server_for_gs_h__

#include "uv.h"

#include <google/protobuf/message.h>
#include "def.h"
#include "guid_generator.h"
#include "dbs_gs_proto.pb.h"
#include "db_sync_operator.h"

class CGSConnectContext
{
public:
	Guid ConnectID;	//链接id
};

//gs线程的异步数据
struct stGSAsyncData{
	//gs线程开放给外部的异步操作
	enum EGSAsyncOP{
		EGSAsyncOP_None = 0,
		EGSAsyncOP_User_Info_Res = 1,
		EGSAsyncOP_User_Edit_User_Name_Res = 2,//编辑玩家名字
		EGSAsyncOP_User_Search_User = 3,	//搜索玩家信息
		EGSAsyncOP_User_Friend_OffLine_Res = 4,
		EGSAsyncOP_Rank_res = 5,
	};

	EGSAsyncOP op;	//操作类型
	Guid gs_connect_id;	//dbs上 发起该请求所属的GS链接id
	Guid user_connect_id;//gs上 玩家的的链接id
	UINT32 user_id;//发起该请求的玩家id
	void* data;
};

class CServerForGS
{
public:

	typedef bool (*func_message_handle) (uv_stream_t* stream, const char * buf, int len);

	static CServerForGS* GetInstance(){
		static CServerForGS singleton;
		return &singleton;
	}

	bool Init();
	//启动gs连接服务线程
	bool Start();
	//等到结束
	bool Join();

	//给外部线程给本网络线程发送消息使用的
	uv_async_t* CreateAsync();
	void DestroyAsync(uv_async_t* handle);

private:

	CServerForGS():	m_loop(NULL){
	};


	//bool BroadCast();

	static void run(void* arg);

	//客户端相关
	static void on_client_connection(uv_stream_t* stream, int status);
	static void on_receive(uv_stream_t* stream, ssize_t nread, const uv_buf_t* buf);
	static void after_client_shutdown(uv_shutdown_t* req, int status);
	static void after_client_close(uv_handle_t* handle);
	static bool send( uv_stream_t* stream, UINT32 msg_id, google::protobuf::Message* pMessage/*, uv_write_cb write_cb*/);
	static void after_send(uv_write_t* req, int status);

	//与其他模块线程交互
	static void async_cb(uv_async_t* handle);
	static void after_async_close(uv_handle_t* handle);


	CGSConnectContext* attach_context_on_client(uv_tcp_t* client);
	bool detach_context_on_client(uv_tcp_t* client);
	bool add_client(uv_tcp_t* client);
	bool remove_client(const Guid& key);
	uv_tcp_t* get_client(const Guid& key);


	//消息处理
	bool add_message_handle(e_msg_gs_dbs msgID, func_message_handle handle );
	bool process_msg( uv_stream_t* stream, const char * buf, int len );
	static bool handle_msg_user_info_req(uv_stream_t* stream, const char * buf, int len);
	static bool handle_msg_user_info_op_req(uv_stream_t* stream, const char * buf, int len);
	static bool handle_msg_user_info_new_create_req(uv_stream_t* stream, const char * buf, int len);
	static bool handle_msg_edit_name_req(uv_stream_t* stream, const char * buf, int len);
	static bool handle_msg_user_new_mail_req(uv_stream_t* stream, const char * buf, int len);
	static bool handle_msg_user_new_arena_mail_req(uv_stream_t* stream, const char * buf, int len);
	static bool handle_msg_user_friend_op_req(uv_stream_t* stream, const char * buf, int len);
	static bool handle_msg_search_user_req(uv_stream_t* stream, const char * buf, int len);
	static bool handle_msg_friend_offline_req(uv_stream_t* stream, const char * buf, int len);
	static bool handle_msg_rank_req(uv_stream_t* stream, const char * buf, int len);
	func_message_handle m_mapHandle[e_msg_gs_dbs_max];

	uv_tcp_t m_server;
	uv_thread_t m_thread;	
	uv_loop_t* m_loop;

	std::map<Guid, uv_tcp_t*> m_clients;

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

	stMsgTmpBuf m_msg_tmp_buf;//上一个消息过大而接受不完全的消息
};

#endif // server_for_gs_h__
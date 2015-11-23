#ifndef client_for_dbs_h__
#define client_for_dbs_h__

#include <google/protobuf/message.h>
#include "uv.h"
#include "dbs_gs_proto.pb.h"


#include "def.h"
#include "guid.h"

class CClientForDBSAsync{
public:
	enum EASyncOP{
		EASyncOP_NONE = 0,
		EASyncOP_User_Info_Req = 1,		//拉取玩家信息
		EASyncOP_User_Info_OP_Req = 2,	//操作玩家信息
		EASyncOP_User_Info_New_Create_Req = 3,//新建新玩家信息
		EASyncOP_User_Edit_User_name = 5,	//编辑玩家姓名
		EASyncOP_User_New_Mail = 6,		//新建邮件
		EASyncOP_User_New_Arena_mail = 7,//竞技场邮件
		EASyncOP_User_Search_User = 8,	//搜索玩家
		EASyncOP_User_Friend_OP = 9,			//好友操作
		EASyncOP_User_Friend_Offline = 10,	//添加好友(不在线的)
		EASyncOP_User_Rank = 11,			//排行榜
	};
	EASyncOP op;
	UINT32 user_id;//玩家id
	Guid user_connect_id;//server for user里面的玩家链接id
	void* data;
};

class CClientForDBS
{
public:
	typedef bool (*func_message_handle) (uv_stream_t* stream, const char * buf, int len);

	static CClientForDBS* GetInstance(){
		static CClientForDBS singleton;
		return &singleton;
	}

	bool Init();
	
	//启动链接dbs线程
	bool Start();

	bool Join();

	//给外部线程调用给本网络线程发送消息接口
	uv_async_t* CreateAsync();
	void DestroyAsync(uv_async_t* handle);
	bool SendAsync(CClientForDBSAsync::EASyncOP eOP, UINT32 user_id, void* data);

	bool IsDBReqInitialized(gs_dbs_user_info_op_req* db_req);
private:
	CClientForDBS():m_loop(NULL){};

	static void run(void* arg);

	//链接相关
	static void on_server_connection(uv_connect_t* req, int status);
	static void on_receive(uv_stream_t* stream, ssize_t nread, const uv_buf_t* buf);
	static void after_shutdown(uv_shutdown_t* req, int status);
	static void after_close(uv_handle_t* handle);
	static bool send( uv_stream_t* stream, UINT32 msg_id, google::protobuf::Message* pMessage/*, uv_write_cb write_cb*/);
	static void after_send(uv_write_t* req, int status);
	
	//消息相关
	bool process_msg( uv_stream_t* stream, const char * buf, int len );
	bool add_message_handle( e_msg_dbs_gs msgID, func_message_handle handle );

	static bool handle_msg_user_info_res(uv_stream_t* stream, const char * buf, int len);
	static bool handle_msg_user_info_op_res(uv_stream_t* stream, const char * buf, int len);
	static bool handle_msg_user_new_mail_res(uv_stream_t* stream, const char * buf, int len);
	static bool handle_msg_edit_name_res(uv_stream_t* stream, const char * buf, int len);
	static bool handle_msg_user_new_arena_mail_res(uv_stream_t* stream, const char * buf, int len);
	static bool handle_msg_friend_offline_res(uv_stream_t* stream, const char * buf, int len);
	static bool handle_msg_search_user_res(uv_stream_t* stream, const char * buf, int len);
	static bool handle_msg_rank_res(uv_stream_t* stream, const char * buf, int len);
	//与其他模块线程交互
	static void async_cb(uv_async_t* handle);
	static void after_async_close(uv_handle_t* handle);

	func_message_handle m_mapHandle[e_msg_dbs_gs_max];

	uv_thread_t m_thread;	
	uv_loop_t* m_loop;
	uv_tcp_t  m_client;

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

#endif
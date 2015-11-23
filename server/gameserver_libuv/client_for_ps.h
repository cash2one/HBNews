#ifndef client_for_ps_h__
#define client_for_ps_h__

#include "uv.h"
//#include "libuv_helper.h"

#include <google/protobuf/message.h>
#include "def.h"

#include "ps_gs_proto.pb.h"

class CClientForPSASync{
public:
	enum EASyncOP{
		EASyncOP_None
	};

	//EASyncOP op;
	UINT32 msg_id;
	google::protobuf::Message* msg;
};

class CClientForPS
{
public:

	typedef bool (*func_message_handle) (uv_stream_t* stream, const char * buf, int len);


	static CClientForPS* GetInstance(){
		static CClientForPS singleton;
		return &singleton;
	}

	bool Init();

	//启动链接as的线程
	bool Start();

	//等到结束
	bool Join();

	bool SendAsync( UINT32 msg_id, google::protobuf::Message* pMessage );
private:

	CClientForPS():	m_loop(NULL){
		memset(m_mapHandle, 0, sizeof m_mapHandle);
	};

	//发送消息
	//bool Send(UINT32 msg_id, google::protobuf::Message* pMessage);

	//给外部线程给本网络线程发送消息使用的
	uv_async_t* CreateAsync();
	void DestroyAsync(uv_async_t* handle);

	bool Send( UINT32 msg_id, google::protobuf::Message* pMessage );

	static void run(void* arg);

	//链接相关
	static void on_server_connection(uv_connect_t* req, int status);
	static void on_receive(uv_stream_t* stream, ssize_t nread, const uv_buf_t* buf);
	static void after_shutdown(uv_shutdown_t* req, int status);
	static void after_close(uv_handle_t* handle);
	static bool send( uv_stream_t* stream, UINT32 msg_id, google::protobuf::Message* pMessage/*, uv_write_cb write_cb*/);
	static void after_send(uv_write_t* req, int status);

	//与其他模块线程交互
	static void async_cb(uv_async_t* handle);
	static void after_async_close(uv_handle_t* handle);

	//消息处理
	bool process_msg( uv_stream_t* stream, const char * buf, int len );
	bool add_message_handle( e_msg_ps_gs msgID, func_message_handle handle );
	static bool handle_msg_paied_req( uv_stream_t* stream, const char * buf, int len );


	//void OnTime();

	//可以用数组来优化 索引为消息号！
	//std::map<unsigned int, func_message_handle> m_mapHandle;
	func_message_handle m_mapHandle[e_msg_ps_gs_max];

	uv_thread_t m_thread;
	uv_tcp_t m_client;
	uv_loop_t* m_loop;

	//单线程访问 定时器也是走内部线程 所以不需要加锁 做线程安全
	//uv_mutex_t m_tokens_lock;//todo 
	//std::map<Guid, CUserLoginToken*> m_tokens;//保存enter area时 由as发过来的token  todo会定时清理!
};

#endif // client_for_ps_h__
#ifndef client_for_as_h__
#define client_for_as_h__

#include "uv.h"
//#include "libuv_helper.h"

#include <google/protobuf/message.h>
#include "def.h"
//#include "guid.h"
#include "guid_generator.h"

#include "as_c_proto.pb.h"


class CClientForASASync{
public:
	enum EASyncOP{
		EASyncOP_None,
		EASyncOP_CheckToken_Req,
	};

	EASyncOP op;
	void* data;
};

class CClientForAS
{
public:

	typedef bool (*func_message_handle) (uv_stream_t* stream, const char * buf, int len);


	//static CClientForAS* GetInstance(){
	//	static CClientForAS singleton;
	//	return &singleton;
	//}

	CClientForAS():	m_loop(NULL){
		m_user_id = 0;
		memset(m_mapHandle, 0, sizeof m_mapHandle);
	};

	bool Init();

	//启动链接as的线程
	bool Start();

	//等到结束
	bool Join();

	//发送消息
	bool Send(UINT32 msg_id, google::protobuf::Message* pMessage);

	//给外部线程给本网络线程发送消息使用的
	uv_async_t* CreateAsync();
	void DestroyAsync(uv_async_t* handle);

	void SetUserPwd(std::string &usr, std::string &pwd){
		m_user_name = usr;
		m_password = pwd;
	}
private:

	static void run(void* arg);

	//空转防止退出 并且可以做一些事
	static void on_idle(uv_idle_t* handle){
		static UINT64 counter = 0;
		++counter;
		printf("the counter is:%ld\n", counter);
		//uv_idle_stop(&idler);   //停止空转
	}

	//链接相关
	static void on_server_connection(uv_connect_t* req, int status);
	static void on_receive(uv_stream_t* stream, ssize_t nread, const uv_buf_t* buf);
	static void after_shutdown(uv_shutdown_t* req, int status);
	static void after_close(uv_handle_t* handle);
	static bool send( uv_stream_t* stream, UINT32 msg_id, google::protobuf::Message* pMessage/*, uv_write_cb write_cb*/);
	static void after_send(uv_write_t* req, int status);
	static void close_client( uv_stream_t* client );

	//与其他模块线程交互
	static void async_cb(uv_async_t* handle);
	static void after_async_close(uv_handle_t* handle);

	//根据句柄获取所属的客户端对象
	static CClientForAS* get_client(uv_handle_t* handle){
		ASSERT(handle && handle->loop);
		return (CClientForAS*)(handle->loop->data);
	}

	//消息处理
	bool process_msg( uv_stream_t* stream, const char * buf, int len );
	bool add_message_handle( e_msg_as_c msgID, func_message_handle handle );

	static bool handle_msg_register_res( uv_stream_t* stream, const char * buf, int len );
	static bool handle_msg_login_res( uv_stream_t* stream, const char * buf, int len );
	static bool handle_msg_enter_area_res( uv_stream_t* stream, const char * buf, int len );
	static bool handle_msg_login_if_no_register_res( uv_stream_t* stream, const char * buf, int len );

	//可以用数组来优化 索引为消息号！
	//std::map<unsigned int, func_message_handle> m_mapHandle;
	func_message_handle m_mapHandle[e_msg_as_c::e_msg_as_c_max];

	static UINT32 m_connect_client_num;//已经链接上的客户端数量

	std::string m_user_name;
	std::string m_password;
	UINT32 m_user_id;

	uv_thread_t m_thread;
	uv_tcp_t m_client;
	uv_idle_t m_idler;    //空转handle 防止无活动时退出
	uv_loop_t* m_loop;
};

#endif // client_for_as_h__
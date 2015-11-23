#ifndef server_for_user_h__
#define server_for_user_h__

#include "uv.h"
//#include "libuv_helper.h"

#include <google/protobuf/message.h>
#include "as_c_proto.pb.h"
#include "def.h"
//#include "guid.h"
#include "guid_generator.h"

//玩家线程开放给外部的异步操作
enum EUserAsyncOP{
	EUserAsyncOP_None = 0,
	EUserAsyncOP_Login,		//登陆时时 数据库线程发给user线程
	EUserAsyncOP_Register,	//注册时 数据库线程发给user线程
	EUserAsyncOP_Enter_Area,//选区时 gs线程发给user线程
};

//玩家线程开放给外部的异步操作数据
struct stUserAsyncData{
	EUserAsyncOP op;
	Guid client_id;//玩家链接id
	UINT32 user_id;//玩家用户ID
	void* data;
};

class CUserContext{
public:
	enum EClientStatus{
		EClientStatus_None = 0 , //初始是无状态的
		EClientStatus_Connected = 1, //只连接进来了
		EClientStatus_Logined = 2,	//登陆成功了！
		EClientStatus_AreaChosed = 3, //已经通知选的区的gs token了 此时as 可以主动断开和c的连接
	};

	CUserContext():stauts(EClientStatus_None){
		user_id = 0;
	}

	Guid client_id;		//玩家链接的标识
	UINT32 user_id;//玩家用户ID
	EClientStatus stauts;
};

class CServerForUser
{
public:

	typedef bool (*func_message_handle) (uv_stream_t* stream, const char * buf, int len);

	static CServerForUser* GetInstance(){
		static CServerForUser singleton;
		return &singleton;
	}

	bool Init()
	{
		ASSERT(add_message_handle(e_msg_c_as::e_msg_c_as_register_req, handle_msg_register));
		ASSERT(add_message_handle(e_msg_c_as::e_msg_c_as_login_req, handle_msg_login));
		ASSERT(add_message_handle(e_msg_c_as::e_msg_c_as_login_if_no_register_req, handle_msg_login_if_no_register));

		ASSERT(add_message_handle(e_msg_c_as::e_msg_c_as_enter_area_req, handle_msg_enter_area_req));
		ASSERT(add_message_handle(e_msg_c_as::e_msg_c_as_logout_req, handle_msg_logout));
		return true;
	}

	//启动玩家连接服务线程
	bool Start();

	//等到结束
	bool Join();

	//给外部线程给本网络线程发送消息使用的
	uv_async_t* CreateAsync();
	void DestroyAsync(uv_async_t* handle);
private:
	
	//发送消息
	//bool Send(UINT32 GSID, UINT32 msg_id, google::protobuf::Message* pMessage);

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

	CServerForUser():m_loop(NULL){
		//m_loop = NULL;
		memset(m_mapHandle, 0, sizeof m_mapHandle);
	};

	//此线程自己使用 所以不需要线程安全
	Guid add_client(uv_tcp_t* client);
	bool remove_client(Guid key);
	uv_tcp_t* get_client(Guid& key);

	//断开链接！
	void close_client(uv_stream_t* client);

	bool process_msg( uv_stream_t* stream, const char * buf, int len );
	bool add_message_handle( e_msg_c_as msgID, func_message_handle handle );
	static bool handle_msg_register(uv_stream_t* stream, const char * buf, int len);
	static bool handle_msg_login(uv_stream_t* stream, const char * buf, int len);
	static bool handle_msg_enter_area_req(uv_stream_t* stream, const char * buf, int len);
	static bool handle_msg_logout(uv_stream_t* stream, const char * buf, int len){
		/*
		printf("call logout!");

		static char str[] = "response logout : hello client!";
		write_reqs.buf.base = str;
		write_reqs.buf.len = strlen(str);
		write_req* w = &write_reqs;
		int r = uv_write(&w->req, stream, &w->buf, 1, write_cb);
		ASSERT(r == 0);
		*/
		return false;
	}

	static bool handle_msg_login_if_no_register(uv_stream_t* stream, const char * buf, int len);
	

	//可以用数组来优化 索引为消息号！
	//std::map<unsigned int, func_message_handle> m_mapHandle;
	func_message_handle m_mapHandle[e_msg_c_as::e_msg_c_as_max];

	uv_thread_t m_thread;
	uv_tcp_t m_server;
	uv_loop_t* m_loop;

	std::map<Guid, uv_tcp_t*> m_clients;
};

#endif // server_for_user_h__
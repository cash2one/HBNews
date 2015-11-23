#ifndef server_for_gs_h__
#define server_for_gs_h__

#include "uv.h"
//#include "libuv_helper.h"

#include <google/protobuf/message.h>
#include "def.h"
//#include "guid.h"
#include "guid_generator.h"

#include "as_gs_proto.pb.h"

class CUserLoginToken 
{
public:
	Guid token;
	UINT32 user_id;
	double receive_time;
};

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


	static CClientForAS* GetInstance(){
		static CClientForAS singleton;
		return &singleton;
	}

	bool Init();

	//启动链接as的线程
	bool Start();

	//等到结束
	bool Join();

	//给外部线程给本网络线程发送消息使用的
	uv_async_t* CreateAsync();
	void DestroyAsync(uv_async_t* handle);
private:

	CClientForAS():	m_loop(NULL){
		memset(m_mapHandle, 0, sizeof m_mapHandle);
	};

	//发送消息
	//bool Send(UINT32 msg_id, google::protobuf::Message* pMessage);


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
	bool add_message_handle( e_msg_as_gs msgID, func_message_handle handle );
	static bool handle_msg_enter_area_req( uv_stream_t* stream, const char * buf, int len );

	CUserLoginToken* get_user_login_token(Guid& token){
		std::map<Guid, CUserLoginToken*>::iterator it = m_tokens.find(token);
		if (it != m_tokens.end())
		{
			return it->second;
		}
		else{
			return NULL;
		}
	}

	bool add_user_login_token(UINT32 user_id, Guid& token){
		ASSERT(m_tokens.find(token) == m_tokens.end());

		double current = NULL;
		int r = uv_uptime(&current);
		ASSERT(r == 0);

		CUserLoginToken* data = new CUserLoginToken;
		data->token = token;
		data->user_id = user_id;
		data->receive_time = current;

		m_tokens[token] = data;

		return true;
	}

	bool remove_user_login_token(Guid& token){
		std::map<Guid, CUserLoginToken*>::iterator it = m_tokens.find(token);
		if (it != m_tokens.end())
		{
			delete it->second;
			m_tokens.erase(it);
			return true;
		}
		else{
			return false;
		}
	}

	void OnTime();

	//可以用数组来优化 索引为消息号！
	//std::map<unsigned int, func_message_handle> m_mapHandle;
	func_message_handle m_mapHandle[e_msg_as_gs_max];

	uv_thread_t m_thread;
	uv_tcp_t m_client;
	uv_loop_t* m_loop;

	//单线程访问 定时器也是走内部线程 所以不需要加锁 做线程安全
	//uv_mutex_t m_tokens_lock;//todo 
	std::map<Guid, CUserLoginToken*> m_tokens;//保存enter area时 由as发过来的token  todo会定时清理!
};

#endif // server_for_gs_h__
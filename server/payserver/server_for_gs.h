#ifndef server_for_gs_h__
#define server_for_gs_h__

#include <string>
#include "uv.h"
//#include "libuv_helper.h"

#include <google/protobuf/message.h>
#include "def.h"
//#include "guid.h"
//#include "guid_generator.h"

#include "ps_gs_proto.pb.h"

class CServerForGSASync{
public:
	enum EASyncOP{
		EASyncOP_None
	};

	//EASyncOP op;
	UINT16 area_id;
	UINT32 msg_id;
	google::protobuf::Message* msg;
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

	//// 此模块的启动 必须由CClientForAS获得了token之后才能启动！
	//启动链接gs的线程
	bool Start();

	//等到结束
	bool Join();

	//停止内部uv_run循环 从而终止线程
	//bool Stop();

	//外部线程给网络线程发消息
	static bool SendAsync(UINT16 area_id, UINT32 msg_id, google::protobuf::Message* pMessage );

private:

	CServerForGS():	m_loop(NULL){
		//m_user_info = NULL;
		memset(m_mapHandle, 0, sizeof m_mapHandle);
	};

	//发送消息 只能本线程！！
	//bool Send(UINT32 msg_id, google::protobuf::Message* pMessage);

	//给外部线程给本网络线程发送消息使用的
	uv_async_t* CreateAsync();
	void DestroyAsync(uv_async_t* handle);

	static void run(void* arg);

	//空转防止退出 并且可以做一些事
	static void on_idle(uv_idle_t* handle){
		static UINT64 counter = 0;
		++counter;  
		printf("the counter is:%ld\n", counter);
		//uv_idle_stop(&idler);   //停止空转  
	}

	//链接相关
	static void on_client_connection( uv_stream_t* stream, int status );
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
	bool add_message_handle( e_msg_gs_ps msgID, func_message_handle handle );

	static bool handle_msg_paied_res( uv_stream_t* stream, const char * buf, int len );
	
	//static int create_timer();
	//static void timer_cb(uv_timer_t* handle);
	//可以用数组来优化 索引为消息号！
	//std::map<unsigned int, func_message_handle> m_mapHandle;
	func_message_handle m_mapHandle[e_msg_gs_ps::e_msg_gs_ps_max];

	uv_thread_t m_thread;
	//uv_tcp_t m_client;
	//uv_idle_t m_idler;    //空转handle 防止无活动时退出
	uv_loop_t* m_loop;
	//uv_timer_t m_timer;//定时器

	uv_tcp_t m_server;

	//std::vector<BYTE> m_msg_buff;
	//BYTE* m_msg_tmp_buf;

	struct CArea{
		UINT32	area_id;
		//std::string	name;
		std::string	ip;
		//UINT32	port;
		//bool	isHidden;

		//>>> 动态数据
		//Guid client_id;		//各个GS的连接id 没有链接时 为默认构造的 GUID()
		//bool status ;//服务器运行状况

		CArea(){
			area_id = 0;
			ip.clear();
		}
	};

	//UINT32 m_area_id;
	//std::string m_ip;	//GS的IP
	//UINT32 m_port;		//GS的端口
	uv_tcp_t* GetCon( UINT16 iAreaID );
	bool AddCon( UINT16 iAreaID, uv_tcp_t* con );
	bool RemoveCon( UINT16 iAreaID );
	bool add_client(uv_tcp_t* client );
	bool remove_client(uv_tcp_t* client ){
		bool r = NULL;
		uv_mutex_lock(&m_lock);
		for (std::map<UINT16, uv_tcp_t*>::iterator it = m_mapAreaID2GSCon.begin(); it != m_mapAreaID2GSCon.end(); ++it){
			if (it->second == client){
				m_mapAreaID2GSCon.erase(it);
				r = true;
				break;
			}
		}
		uv_mutex_unlock(&m_lock);
		return r;
	}

	//根据句柄获取所属的客户端对象
	//static CServerForGS* get_client(uv_handle_t* handle){
	//	ASSERT(handle && handle->loop);
	//	return (CServerForGS*)(handle->loop->data);
	//}

	uv_mutex_t m_lock;
	std::map<UINT16, uv_tcp_t*> m_mapAreaID2GSCon;//areaid -> gs con
	std::map<std::string, CArea*> m_mapIP2Area;//ip -> area 为了判断来的是哪个区！
	
	typedef struct stMsgTmpBuf{
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
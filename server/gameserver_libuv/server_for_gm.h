#ifndef server_for_gm_h__
#define server_for_gm_h__

#include "uv.h"
//#include "libuv_helper.h"

#include <google/protobuf/message.h>
#include "gs_gm_proto.pb.h"
#include "def.h"
//#include "guid.h"
#include "guid_generator.h"

#include "as_gs_proto.pb.h"

struct CArea;

class CGMContext{
public:
	Guid client_id;
	//area::EAreaStatus stauts;
};

//gs线程的异步数据
struct stServerForGMAsync{
	//gs线程开放给外部的异步操作
	enum EAsyncOP{
		EAsyncOP_None = 0,
		EAsyncOP_Marquee_Res,	//user线程 向gm线程 发送跑马灯操作的响应消息
		EAsyncOP_Mail_Res,		//邮箱响应
		EAsyncOP_Add_Item_Res,	//添加道具
		EAsyncOP_Arena_Mail_Res,//竞技场结算邮件
	};

	EAsyncOP op;	//操作类型
	Guid client_id;//发起该请求玩家链接id
	//UINT32 user_id;//发起该请求的玩家id
	//UINT32 area_id;//所请求的area
	void* data;
};

class CServerForGM
{
public:

	typedef bool (*func_message_handle) (uv_stream_t* stream, const char * buf, int len);

	static CServerForGM* GetInstance(){
		static CServerForGM singleton;
		return &singleton;
	}

	bool Init();
	//启动gs连接服务线程
	bool Start();
	//等到结束
	bool Join();

	void SendAsync( Guid& sender_client_id, stServerForGMAsync::EAsyncOP eAsyncOP, google::protobuf::Message* data );

	//bool GetAreaList(area_list* areas);
	//CArea* GetAreaByAreaID(UINT32 area_id);

private:

	CServerForGM():	m_loop(NULL){
		memset(m_mapHandle, 0, sizeof m_mapHandle);
	};

	//给外部线程给本网络线程发送消息使用的
	uv_async_t* CreateAsync();
	void DestroyAsync(uv_async_t* handle);

	//发送消息
	bool Send(UINT32 area_id, UINT32 msg_id, google::protobuf::Message* pMessage);
	bool Send(Guid client_id, UINT32 msg_id, google::protobuf::Message* pMessage);

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

	//此线程自己使用 所以不需要线程安全
	Guid add_client(uv_tcp_t* client);
	bool remove_client(Guid key);
	uv_tcp_t* get_client(Guid& key);
	uv_tcp_t* get_client(UINT32 area_id);

	static CGMContext* get_context(uv_stream_t* client){
		ASSERT(client);
		CGMContext* ctx = (CGMContext*)(client->data);
		ASSERT(ctx);
		return ctx;
	}

	//消息处理
	bool add_message_handle( e_msg_gm_gs msgID, func_message_handle handle );
	bool process_msg( uv_stream_t* stream, const char * buf, int len );
	static bool handle_msg_marquee_req(uv_stream_t* stream, const char * buf, int len);
	static bool handle_msg_add_item_req( uv_stream_t* stream, const char * buf, int len );
	static bool handle_msg_mail_req( uv_stream_t* stream, const char * buf, int len );
	static bool handle_msg_arena_mail_req( uv_stream_t* stream, const char * buf, int len );

	//可以用数组来优化 索引为消息号！
	//std::map<unsigned int, func_message_handle> m_mapHandle;
	func_message_handle m_mapHandle[e_msg_gm_gs::e_msg_gm_gs_max];

	uv_thread_t m_thread;
	uv_tcp_t m_server;
	uv_loop_t* m_loop;

	std::map<Guid, uv_tcp_t*> m_clients;//所有的链接

	//area_list m_areas;//原始数据, 也为了可以直接构造 而不作额外的工作 直接发送给客户端
	//std::map<UINT32, CArea*> m_mapAreaID2Area;//全部的区服信息

	//要求 不同区服GS必须是不同IP！！
	//std::map<std::string, CArea*> m_mapIP2Area;//全部的区服信息 ip到区信息的映射，为了链接进来时知道是哪个区服！ 
};

#endif // server_for_gm_h__
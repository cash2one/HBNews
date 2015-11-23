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

//gs�̵߳��첽����
struct stServerForGMAsync{
	//gs�߳̿��Ÿ��ⲿ���첽����
	enum EAsyncOP{
		EAsyncOP_None = 0,
		EAsyncOP_Marquee_Res,	//user�߳� ��gm�߳� ��������Ʋ�������Ӧ��Ϣ
		EAsyncOP_Mail_Res,		//������Ӧ
		EAsyncOP_Add_Item_Res,	//��ӵ���
		EAsyncOP_Arena_Mail_Res,//�����������ʼ�
	};

	EAsyncOP op;	//��������
	Guid client_id;//����������������id
	//UINT32 user_id;//�������������id
	//UINT32 area_id;//�������area
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
	//����gs���ӷ����߳�
	bool Start();
	//�ȵ�����
	bool Join();

	void SendAsync( Guid& sender_client_id, stServerForGMAsync::EAsyncOP eAsyncOP, google::protobuf::Message* data );

	//bool GetAreaList(area_list* areas);
	//CArea* GetAreaByAreaID(UINT32 area_id);

private:

	CServerForGM():	m_loop(NULL){
		memset(m_mapHandle, 0, sizeof m_mapHandle);
	};

	//���ⲿ�̸߳��������̷߳�����Ϣʹ�õ�
	uv_async_t* CreateAsync();
	void DestroyAsync(uv_async_t* handle);

	//������Ϣ
	bool Send(UINT32 area_id, UINT32 msg_id, google::protobuf::Message* pMessage);
	bool Send(Guid client_id, UINT32 msg_id, google::protobuf::Message* pMessage);

	//bool BroadCast();

	static void run(void* arg);

	//�ͻ������
	static void on_client_connection(uv_stream_t* stream, int status);
	static void on_receive(uv_stream_t* stream, ssize_t nread, const uv_buf_t* buf);
	static void after_client_shutdown(uv_shutdown_t* req, int status);
	static void after_client_close(uv_handle_t* handle);
	static bool send( uv_stream_t* stream, UINT32 msg_id, google::protobuf::Message* pMessage/*, uv_write_cb write_cb*/);
	static void after_send(uv_write_t* req, int status);

	//������ģ���߳̽���
	static void async_cb(uv_async_t* handle);
	static void after_async_close(uv_handle_t* handle);

	//���߳��Լ�ʹ�� ���Բ���Ҫ�̰߳�ȫ
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

	//��Ϣ����
	bool add_message_handle( e_msg_gm_gs msgID, func_message_handle handle );
	bool process_msg( uv_stream_t* stream, const char * buf, int len );
	static bool handle_msg_marquee_req(uv_stream_t* stream, const char * buf, int len);
	static bool handle_msg_add_item_req( uv_stream_t* stream, const char * buf, int len );
	static bool handle_msg_mail_req( uv_stream_t* stream, const char * buf, int len );
	static bool handle_msg_arena_mail_req( uv_stream_t* stream, const char * buf, int len );

	//�������������Ż� ����Ϊ��Ϣ�ţ�
	//std::map<unsigned int, func_message_handle> m_mapHandle;
	func_message_handle m_mapHandle[e_msg_gm_gs::e_msg_gm_gs_max];

	uv_thread_t m_thread;
	uv_tcp_t m_server;
	uv_loop_t* m_loop;

	std::map<Guid, uv_tcp_t*> m_clients;//���е�����

	//area_list m_areas;//ԭʼ����, ҲΪ�˿���ֱ�ӹ��� ����������Ĺ��� ֱ�ӷ��͸��ͻ���
	//std::map<UINT32, CArea*> m_mapAreaID2Area;//ȫ����������Ϣ

	//Ҫ�� ��ͬ����GS�����ǲ�ͬIP����
	//std::map<std::string, CArea*> m_mapIP2Area;//ȫ����������Ϣ ip������Ϣ��ӳ�䣬Ϊ�����ӽ���ʱ֪�����ĸ������� 
};

#endif // server_for_gm_h__
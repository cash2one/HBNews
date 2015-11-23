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

	//��������as���߳�
	bool Start();

	//�ȵ�����
	bool Join();

	bool SendAsync( UINT32 msg_id, google::protobuf::Message* pMessage );
private:

	CClientForPS():	m_loop(NULL){
		memset(m_mapHandle, 0, sizeof m_mapHandle);
	};

	//������Ϣ
	//bool Send(UINT32 msg_id, google::protobuf::Message* pMessage);

	//���ⲿ�̸߳��������̷߳�����Ϣʹ�õ�
	uv_async_t* CreateAsync();
	void DestroyAsync(uv_async_t* handle);

	bool Send( UINT32 msg_id, google::protobuf::Message* pMessage );

	static void run(void* arg);

	//�������
	static void on_server_connection(uv_connect_t* req, int status);
	static void on_receive(uv_stream_t* stream, ssize_t nread, const uv_buf_t* buf);
	static void after_shutdown(uv_shutdown_t* req, int status);
	static void after_close(uv_handle_t* handle);
	static bool send( uv_stream_t* stream, UINT32 msg_id, google::protobuf::Message* pMessage/*, uv_write_cb write_cb*/);
	static void after_send(uv_write_t* req, int status);

	//������ģ���߳̽���
	static void async_cb(uv_async_t* handle);
	static void after_async_close(uv_handle_t* handle);

	//��Ϣ����
	bool process_msg( uv_stream_t* stream, const char * buf, int len );
	bool add_message_handle( e_msg_ps_gs msgID, func_message_handle handle );
	static bool handle_msg_paied_req( uv_stream_t* stream, const char * buf, int len );


	//void OnTime();

	//�������������Ż� ����Ϊ��Ϣ�ţ�
	//std::map<unsigned int, func_message_handle> m_mapHandle;
	func_message_handle m_mapHandle[e_msg_ps_gs_max];

	uv_thread_t m_thread;
	uv_tcp_t m_client;
	uv_loop_t* m_loop;

	//���̷߳��� ��ʱ��Ҳ�����ڲ��߳� ���Բ���Ҫ���� ���̰߳�ȫ
	//uv_mutex_t m_tokens_lock;//todo 
	//std::map<Guid, CUserLoginToken*> m_tokens;//����enter areaʱ ��as��������token  todo�ᶨʱ����!
};

#endif // client_for_ps_h__
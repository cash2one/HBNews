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

	//��������as���߳�
	bool Start();

	//�ȵ�����
	bool Join();

	//������Ϣ
	bool Send(UINT32 msg_id, google::protobuf::Message* pMessage);

	//���ⲿ�̸߳��������̷߳�����Ϣʹ�õ�
	uv_async_t* CreateAsync();
	void DestroyAsync(uv_async_t* handle);

	void SetUserPwd(std::string &usr, std::string &pwd){
		m_user_name = usr;
		m_password = pwd;
	}
private:

	static void run(void* arg);

	//��ת��ֹ�˳� ���ҿ�����һЩ��
	static void on_idle(uv_idle_t* handle){
		static UINT64 counter = 0;
		++counter;
		printf("the counter is:%ld\n", counter);
		//uv_idle_stop(&idler);   //ֹͣ��ת
	}

	//�������
	static void on_server_connection(uv_connect_t* req, int status);
	static void on_receive(uv_stream_t* stream, ssize_t nread, const uv_buf_t* buf);
	static void after_shutdown(uv_shutdown_t* req, int status);
	static void after_close(uv_handle_t* handle);
	static bool send( uv_stream_t* stream, UINT32 msg_id, google::protobuf::Message* pMessage/*, uv_write_cb write_cb*/);
	static void after_send(uv_write_t* req, int status);
	static void close_client( uv_stream_t* client );

	//������ģ���߳̽���
	static void async_cb(uv_async_t* handle);
	static void after_async_close(uv_handle_t* handle);

	//���ݾ����ȡ�����Ŀͻ��˶���
	static CClientForAS* get_client(uv_handle_t* handle){
		ASSERT(handle && handle->loop);
		return (CClientForAS*)(handle->loop->data);
	}

	//��Ϣ����
	bool process_msg( uv_stream_t* stream, const char * buf, int len );
	bool add_message_handle( e_msg_as_c msgID, func_message_handle handle );

	static bool handle_msg_register_res( uv_stream_t* stream, const char * buf, int len );
	static bool handle_msg_login_res( uv_stream_t* stream, const char * buf, int len );
	static bool handle_msg_enter_area_res( uv_stream_t* stream, const char * buf, int len );
	static bool handle_msg_login_if_no_register_res( uv_stream_t* stream, const char * buf, int len );

	//�������������Ż� ����Ϊ��Ϣ�ţ�
	//std::map<unsigned int, func_message_handle> m_mapHandle;
	func_message_handle m_mapHandle[e_msg_as_c::e_msg_as_c_max];

	static UINT32 m_connect_client_num;//�Ѿ������ϵĿͻ�������

	std::string m_user_name;
	std::string m_password;
	UINT32 m_user_id;

	uv_thread_t m_thread;
	uv_tcp_t m_client;
	uv_idle_t m_idler;    //��תhandle ��ֹ�޻ʱ�˳�
	uv_loop_t* m_loop;
};

#endif // client_for_as_h__
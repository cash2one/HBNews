#ifndef server_for_gs_h__
#define server_for_gs_h__

#include "uv.h"

#include <google/protobuf/message.h>
#include "def.h"
#include "guid_generator.h"
#include "dbs_gs_proto.pb.h"
#include "db_sync_operator.h"

class CGSConnectContext
{
public:
	Guid ConnectID;	//����id
};

//gs�̵߳��첽����
struct stGSAsyncData{
	//gs�߳̿��Ÿ��ⲿ���첽����
	enum EGSAsyncOP{
		EGSAsyncOP_None = 0,
		EGSAsyncOP_User_Info_Res = 1,
		EGSAsyncOP_User_Edit_User_Name_Res = 2,//�༭�������
		EGSAsyncOP_User_Search_User = 3,	//���������Ϣ
		EGSAsyncOP_User_Friend_OffLine_Res = 4,
		EGSAsyncOP_Rank_res = 5,
	};

	EGSAsyncOP op;	//��������
	Guid gs_connect_id;	//dbs�� ���������������GS����id
	Guid user_connect_id;//gs�� ��ҵĵ�����id
	UINT32 user_id;//�������������id
	void* data;
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
	//����gs���ӷ����߳�
	bool Start();
	//�ȵ�����
	bool Join();

	//���ⲿ�̸߳��������̷߳�����Ϣʹ�õ�
	uv_async_t* CreateAsync();
	void DestroyAsync(uv_async_t* handle);

private:

	CServerForGS():	m_loop(NULL){
	};


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


	CGSConnectContext* attach_context_on_client(uv_tcp_t* client);
	bool detach_context_on_client(uv_tcp_t* client);
	bool add_client(uv_tcp_t* client);
	bool remove_client(const Guid& key);
	uv_tcp_t* get_client(const Guid& key);


	//��Ϣ����
	bool add_message_handle(e_msg_gs_dbs msgID, func_message_handle handle );
	bool process_msg( uv_stream_t* stream, const char * buf, int len );
	static bool handle_msg_user_info_req(uv_stream_t* stream, const char * buf, int len);
	static bool handle_msg_user_info_op_req(uv_stream_t* stream, const char * buf, int len);
	static bool handle_msg_user_info_new_create_req(uv_stream_t* stream, const char * buf, int len);
	static bool handle_msg_edit_name_req(uv_stream_t* stream, const char * buf, int len);
	static bool handle_msg_user_new_mail_req(uv_stream_t* stream, const char * buf, int len);
	static bool handle_msg_user_new_arena_mail_req(uv_stream_t* stream, const char * buf, int len);
	static bool handle_msg_user_friend_op_req(uv_stream_t* stream, const char * buf, int len);
	static bool handle_msg_search_user_req(uv_stream_t* stream, const char * buf, int len);
	static bool handle_msg_friend_offline_req(uv_stream_t* stream, const char * buf, int len);
	static bool handle_msg_rank_req(uv_stream_t* stream, const char * buf, int len);
	func_message_handle m_mapHandle[e_msg_gs_dbs_max];

	uv_tcp_t m_server;
	uv_thread_t m_thread;	
	uv_loop_t* m_loop;

	std::map<Guid, uv_tcp_t*> m_clients;

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

	stMsgTmpBuf m_msg_tmp_buf;//��һ����Ϣ��������ܲ���ȫ����Ϣ
};

#endif // server_for_gs_h__
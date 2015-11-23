
#include <stdio.h>
#include "uv.h"
#include "as_c_proto.pb.h"
#include "libuv_helper.h"

#define  PACKET_HEAD_LEN 8

/* Have our own assert, so we are sure it does not get optimized away in
* a release build.
*/
#define ASSERT(expr)                                      \
	do {                                                     \
	if (!(expr)) {                                          \
	fprintf(stderr,                                       \
	"Assertion failed in %s on line %d: %s\n",    \
	__FILE__,                                     \
	__LINE__,                                     \
#expr);                                       \
	abort();                                              \
	}                                                       \
	} while (0)


/* Die with fatal error. */
#define FATAL(msg)                                        \
	do {                                                    \
	fprintf(stderr,                                       \
	"Fatal error in %s on line %d: %s\n",         \
	__FILE__,                                     \
	__LINE__,                                     \
	msg);                                         \
	fflush(stderr);                                       \
	abort();                                              \
	} while (0)


typedef struct {
	uv_tcp_t handle;
	uv_shutdown_t shutdown_req;
} conn_rec;



typedef struct {
	uv_write_t req;
	uv_buf_t buf;
} write_req;
static write_req write_reqs;

static uv_shutdown_t shutdown_req;

uv_loop_t * loop;




static int connect_cb_called;
static int write_cb_called;
static int close_cb_called;


static void close_cb(uv_handle_t* handle) {
	close_cb_called++;
}



static void alloc_cb(uv_handle_t* handle, 
					 size_t suggested_size, 
					 uv_buf_t* buf) {
						 //static char sbuf[65536];
						 //return uv_buf_init(buf, sizeof buf);

						 buf->base = (char*)malloc(suggested_size);
						 buf->len = suggested_size;
}

static void write_cb(uv_write_t* req, int status) {
	ASSERT(req != NULL);
	ASSERT(status == 0);
	write_cb_called++;
	printf("客户端给服务器发送信息成功\n");
}


bool send( uv_stream_t* stream, UINT32 msg_id, google::protobuf::Message* pMessage, uv_write_cb write_cb )
{
	ASSERT(uv_is_writable(stream));
	uv_buf_t ret_buf;
	UINT32 body_len = pMessage->ByteSize();
	//UINT32 head_len = sizeof(UINT32) + sizeof(UINT32);//4直接的包长度 + 4字节的消息id
	buf_alloc((uv_handle_t*)stream, PACKET_HEAD_LEN + body_len, &ret_buf);

	ASSERT(ret_buf.base && ret_buf.len);
	*((UINT32*)(ret_buf.base)) = PACKET_HEAD_LEN + body_len;
	*((UINT32*)(ret_buf.base)+1) = msg_id;
	INT32 r = pMessage->SerializeToArray(ret_buf.base + PACKET_HEAD_LEN, body_len);
	ASSERT(r);

	uv_write_t* write_req = (uv_write_t*)req_alloc();
	printf("send msg[%d]  bytes len[%d] \n", msg_id, pMessage->ByteSize() + sizeof(UINT32) + sizeof(UINT32));
	r = uv_write(write_req, stream, &ret_buf, 1, write_cb);
	buf_free(&ret_buf);
	
	if (r != 0)
	{
		PRINTF_UV_ERROR(r);
	}
	ASSERT(r == 0);

	return true;
}

bool process_msg( uv_stream_t* stream, const char * buf, int len )
{
	ASSERT(buf);
	ASSERT(len >= PACKET_HEAD_LEN);//
	int pos = 0;
	int bytes_left = len;

	char* p = const_cast<char*>(buf);
	while (bytes_left != 0)
	{
		UINT32 msg_len = *(UINT32 *)(p);
		p += sizeof msg_len;

		ASSERT(bytes_left >= msg_len);
		bytes_left -= msg_len;

		UINT32 msg_id = *(UINT32 *)(p);
		//ASSERT(msg_id < e_msg_as_gs::e_msg_as_gs_max);
		p += sizeof msg_id;

		//func_message_handle pMsgHandle = m_mapHandle[msg_id];
		//if (pMsgHandle)
		//{
		//	bool r = pMsgHandle(stream, p, msg_len - sizeof(msg_len) - sizeof(msg_id));
		//}
		//else{
		//	printf("CClientForAS::process_msg没有找到消息msgid[%d][%s]的处理函数\n", msg_id, e_msg_as_gs_Name((e_msg_as_gs)msg_id).c_str());
		//	//return false;
		//}

		if (msg_id == e_msg_as_c::e_msg_as_c_register_res)
		{
			as_c_register_res res;
			bool r = res.ParseFromArray(p, msg_len - sizeof(msg_len) - sizeof(msg_id));
			ASSERT(r);
			if (res.error_code() != e_asc_errorcode_success)
			{
				printf("msg[%d][%s] error[%d][%s]", msg_id, e_msg_as_c_Name((e_msg_as_c)msg_id).c_str(),  res.error_code(), e_asc_errorcode_Name((e_asc_errorcode)res.error_code()).c_str());
			}
		}

		if (msg_id == e_msg_as_c::e_msg_as_c_login_res)
		{
			as_c_login_res res;
			bool r = res.ParseFromArray(p, msg_len - sizeof(msg_len) - sizeof(msg_id));
			ASSERT(r);
			if (res.error_code() != e_asc_errorcode_success)
			{
				printf("msg[%d][%s] error[%d][%s]", msg_id, e_msg_as_c_Name((e_msg_as_c)msg_id).c_str(),  res.error_code(), e_asc_errorcode_Name((e_asc_errorcode)res.error_code()).c_str());
			}
			else{
				const area_list& areas = res.areas();
				c_as_enter_area_req req;

				const area& a = areas.areas(0);
				req.set_area_id(a.area_id());
				
				send(stream, e_msg_c_as_enter_area_req, &req, write_cb);
			}
		}

		if (msg_id == e_msg_as_c::e_msg_as_c_enter_area_res)
		{
			as_c_enter_area_res res;
			bool r = res.ParseFromArray(p, msg_len - sizeof(msg_len) - sizeof(msg_id));
			ASSERT(r);
			if (res.error_code() != e_asc_errorcode_success)
			{
				printf("msg[%d][%s] error[%d][%s]", msg_id, e_msg_as_c_Name((e_msg_as_c)msg_id).c_str(),  res.error_code(), e_asc_errorcode_Name((e_asc_errorcode)res.error_code()).c_str());
			}
		}

		p += msg_len - sizeof(msg_len) - sizeof(msg_id);
	}

	return true;
}

static void read_cb(uv_stream_t* stream, ssize_t nread, const uv_buf_t* buf) {

	static int count = 0;
	printf("客户端收到消息！\n");
	if (nread >= 0){

		//int r;
		++count;
		//buf->base[nread] = 0;
		//printf("%s, %d, %d \n", buf->base, nread, count);

		/*
		static char str[] = "hello server!";
		write_reqs.buf.base = str;
		write_reqs.buf.len = strlen(str);
		write_req* w = &write_reqs;
		int r = uv_write(&w->req, stream, &w->buf, 1, write_cb);
		ASSERT(r == 0);
		*/

		process_msg(stream, buf->base, nread);

		return;
	}
	//ASSERT(uv_last_error(stream->loop).code == UV_EOF);
}




static void shutdown_cb(uv_shutdown_t* req, int status) {
	//ASSERT(req->handle == (uv_stream_t*)&tcp_client);
	ASSERT(req->handle->write_queue_size == 0);

	uv_close((uv_handle_t*)req->handle, close_cb);
	//free(write_reqs);

	//shutdown_cb_called++;
}




bool hasConnected = false;
uv_tcp_t g_socket ;
static void connect_cb(uv_connect_t* req, int status) {

	if (status != 0)
	{
		//printf("%s:%s", uv_err_name(status), uv_strerror(status));
		PRINTF_UV_ERROR(status);
	}
	ASSERT(status == 0);

	hasConnected = true;
	connect_cb_called++;
	//uv_close((uv_handle_t*)req->handle, close_cb);

	//int r = uv_read_start((uv_stream_t*)&req->handle, alloc_cb, read_cb);
	//ASSERT(r == 0);

	//ASSERT(req->handle == (uv_stream_t*)&tcp_client);

	/*
	static char str[] = " hello server!";
	write_reqs.buf.base = str;
	write_reqs.buf.len = strlen(str);
	str[0] = 0 ;
	write_req* w = &write_reqs;
	int r = uv_write(&w->req, req->handle, &w->buf, 1, write_cb);
	ASSERT(r == 0);
	*/

	//r = uv_shutdown(&shutdown_req, req->handle, shutdown_cb);
	//ASSERT(r == 0);

	//r = uv_read_start((uv_stream_t*)&req->handle, alloc_cb, read_cb);
	//ASSERT(r == 0);

	printf("服务器连接成功...\n");
	int r = uv_read_start(req->handle, alloc_cb, read_cb);
	if (r) {
		FATAL("uv_read_start failed");
	}
	printf("服务器连接读启动成功...\n");


	c_as_register_req reg_req;
	reg_req.set_plantform_id(1);
	reg_req.set_plantform_sub_id(2);
	reg_req.set_account("nini");
	reg_req.set_password("111111");

	send(req->handle, e_msg_c_as_register_req, &reg_req, write_cb);


	c_as_login_req login_req;
	login_req.set_plantform_id(1);
	login_req.set_plantform_sub_id(2);
	login_req.set_account("nini");
	login_req.set_password("111111");

	send(req->handle, e_msg_c_as_login_req, &login_req, write_cb);

	ASSERT(r == 0);
}




static void idle_cb(uv_idle_t* handle) {
	//if (++ticks == NUM_TICKS)
	//	uv_idle_stop(handle);

	printf("空闲函数执行\n");
	if(hasConnected){
		int msgType;
		scanf("%d", &msgType);

		static char str[1024] = {0};
		write_reqs.buf.base = str;
		write_reqs.buf.len = sizeof(str);
		*(int*)str = msgType ;
		write_req* w = &write_reqs;
		int r = uv_write(&w->req, (uv_stream_t *)(&g_socket), &w->buf, 1, write_cb);
		ASSERT(r == 0);
	}
}

int main1(){

	uv_idle_t idle;
	struct sockaddr_in addr;
	uv_loop_t* loop;
	int r;

	loop = uv_default_loop();
	//ASSERT(0 == uv_ip4_addr("127.0.0.1", 1234, &addr));
	//ASSERT(0 == uv_ip4_addr("211.157.191.149", 1234, &addr));
	//struct sockaddr_in dest = uv_ip4_addr("127.0.0.1", 80);

	//ASSERT(0 == uv_ip4_addr("127.0.0.1", 54321, &addr));

	//ASSERT(0 == uv_ip4_addr("127.0.0.1", 54320, &addr));

#ifdef _LOCAL
	char* ip = "127.0.0.1";
#else
	char* ip = "211.157.191.149";
#endif
	int port = 54320;
	ASSERT(0 == uv_ip4_addr(ip, port, &addr));

	r = uv_tcp_init(loop, &g_socket);
	ASSERT(r == 0);

	uv_connect_t connect;
	r = uv_tcp_connect(&connect, &g_socket, (const struct sockaddr*)(&addr), connect_cb);
	ASSERT(r == 0);

	//uv_idle_init(loop, &idle);
	//uv_idle_start(&idle,idle_cb);

	r = uv_run(loop, UV_RUN_DEFAULT);
	//ASSERT(0 && "Blackhole server dropped out of event loop.");

	return 0;

}
#include "libuv_helper.h"

//#include "str.h"
//#include "net_config.h"


/*
* Request allocator
*/
typedef struct req_list_s {
	union uv_any_req uv_req;
	struct req_list_s* next;
} req_list_t;

static req_list_t* req_freelist = NULL;

static uv_mutex_t req_list_mutex;
static int r = uv_mutex_init(&req_list_mutex);

//todo 线程安全！ todo 不用锁 而是每个线程封装一个req工厂对象
/*static*/ uv_req_t* req_alloc(void) {
	ASSERT(r == 0);
	req_list_t* req = NULL;

	uv_mutex_lock(&req_list_mutex);
	req = req_freelist;
	if (req != NULL) {
		req_freelist = req->next;
		//return (uv_req_t*) req;
	}
	else{
		req = (req_list_t*) malloc(sizeof *req);
	}
	uv_mutex_unlock(&req_list_mutex);
	return (uv_req_t*) req;
}

//todo 线程安全  todo 不用锁 而是每个线程封装一个req工厂对象
/*static*/ void req_free(uv_req_t* uv_req) {
	ASSERT(r == 0);
	uv_mutex_lock(&req_list_mutex);
	req_list_t* req = (req_list_t*) uv_req;
	req->next = req_freelist;
	req_freelist = req;
	uv_mutex_unlock(&req_list_mutex);
}


/*
* Buffer allocator
*/

typedef struct buf_list_s {
	uv_buf_t uv_buf_t;
	struct buf_list_s* next;
} buf_list_t;


static buf_list_t* buf_freelist = NULL;


void buf_alloc(uv_handle_t* handle, size_t size, uv_buf_t* buf) {
	buf->base = (char*)malloc(size);
	buf->len = size;
}

void buf_free(uv_buf_t* buf) {
	free(buf->base);
	buf->base = NULL;
	buf->len = 0;
}

void alloc_cb(uv_handle_t* handle, 
			  size_t suggested_size, 
			  uv_buf_t* buf) {
				  //static char sbuf[65536];
				  //return uv_buf_init(buf, sizeof buf);

				  buf->base = (char*)malloc(suggested_size);
				  buf->len = suggested_size;
}

//void write_cb(uv_write_t* req, int status) {
//	ASSERT(req != NULL);
//	//ASSERT(status == 0);
//	
//	conn_rec* conn;
//	if (status == 0)
//	{
//		//write_cb_called++;
//		printf("服务器给客户端发送信息成功\n");
//	}
//	else{
//		if (status == UV_EOF) {
//			printf("客户端主动断开\n");
//
//		} else if (status == UV_ECONNRESET) {
//			printf("客户端异常断开\n");
//
//		} else {
//			printf("%s : %s \n", uv_err_name(status), uv_strerror(status));
//		}
//
//		conn = container_of(req->handle, conn_rec, handle);
//		int r = uv_shutdown(&conn->shutdown_req, req->handle, shutdown_cb);
//		ASSERT(r == 0);
//	}
//
//	req_free((uv_req_t*)req);
//
//}

//void connection_cb(uv_stream_t* stream, int status) {
//	conn_rec* conn;
//	int r;
//	struct sockaddr sockname, peername;
//	int namelen;
//
//	ASSERT(status == 0);
//	ASSERT(stream == (uv_stream_t*)&tcp_server);
//
//	conn = (conn_rec*)malloc(sizeof *conn);
//	ASSERT(conn != NULL);
//
//	r = uv_tcp_init(stream->loop, &conn->handle);
//	ASSERT(r == 0);
//
//	printf("new connect incoming, before accept! \n");
//	r = uv_accept(stream, (uv_stream_t*)&conn->handle);
//	ASSERT(r == 0);
//
//	memset(&sockname, -1, sizeof sockname);  
//	namelen = sizeof sockname;  
//	r = uv_tcp_getsockname(&(conn->handle), &sockname, &namelen);  
//	//printf("the r is %d.\n", r);
//	ASSERT( r == 0);
//
//	char ip[17] = {0};
//	struct sockaddr_in sock_addr = *(struct sockaddr_in*) &sockname; 
//	//网络字节序转换成主机字符序  
//	uv_ip4_name(&sock_addr, (char*)ip, sizeof ip);  
//	printf("getsockname   %s : %d \n", ip, sock_addr.sin_port);
//
//
//	//有连接，可以获得目标的ip和端口  
//	namelen = sizeof peername;  
//	r = uv_tcp_getpeername(&(conn->handle), &peername, &namelen);  
//	//printf("the r is %d.\n", r);
//	ASSERT( r == 0);
//
//	memset(ip, 0, sizeof ip);
//	sock_addr = *(struct sockaddr_in*) &peername; 
//	//网络字节序转换成主机字符序  
//	uv_ip4_name(&sock_addr, (char*)ip, sizeof ip);  
//	printf("getpeername  %s : %d \n", ip, sock_addr.sin_port);
//
//	//todo net_protect作一些限制处理
//
//	printf("new connect accepted! \n");
//	r = uv_read_start((uv_stream_t*)&conn->handle, alloc_cb, read_cb);
//	ASSERT(r == 0);
//
//	stConnectUserData* userdata = new stConnectUserData();
//	memset(userdata, 0 , sizeof stConnectUserData);
//	conn->handle.data = userdata;
//
//
//	////为什么服务器这个时候不能写
//	//uv_write_t* req = (uv_write_t*)malloc(sizeof *req);
//	//uv_buf_t buf = uv_buf_init("hello baozi!",sizeof("hello baozi!"));
//	//r = uv_write(req, stream, &buf, 1, NULL);
//	//if (r != 0) {
//	//	printf("%s:%s", uv_err_name(r), uv_strerror(r));
//	//	FATAL("uv_write failed");
//	//}
//	//
//	////uv_async_send()
//
//	//uv_timer_t *timer = new uv_timer_t;  
//	//r = uv_timer_init(uv_default_loop(), timer);  
//	//assert(r == 0);  
//
//	//assert(!uv_is_active((uv_handle_t *) timer));  
//	//assert(!uv_is_closing((uv_handle_t *) timer));  
//	////printf("start %d\n", time(NULL));
//	//timer->data = stream;
//	//r = uv_timer_start(timer, timer_cb, 5 * 1000, 0);  
//
//}

//void read_cb(uv_stream_t* stream, ssize_t nread, const uv_buf_t* buf) {
//	conn_rec* conn;
//	int r;
//
//	static int count = 0;
//
//	//todo net_protect作一些限制处理
//	if (nread > 0){
//		++count;
//
//		CMessageHandlerMgr::GetInstance()->process_msg(stream, buf->base, nread);
//		free(buf->base);//释放内存
//
//		//just for client test
//		stConnectUserData* pConData = (stConnectUserData*)stream->data;
//		ASSERT(pConData);
//		if (!pConData->timer[0])
//		{
//			uv_timer_t *timer = new uv_timer_t;  
//			r = uv_timer_init(uv_default_loop(), timer);  
//			assert(r == 0);  
//
//			assert(!uv_is_active((uv_handle_t *) timer));  
//			assert(!uv_is_closing((uv_handle_t *) timer));  
//			//printf("start %d\n", time(NULL));
//			timer->data = stream;
//			pConData->timer[0] = timer;
//			r = uv_timer_start(timer, timer_cb, 5 * 1000, 5 * 1000);
//			assert(r == 0);
//
//
//			timer = new uv_timer_t;  
//			r = uv_timer_init(uv_default_loop(), timer);  
//			assert(r == 0);  
//
//			assert(!uv_is_active((uv_handle_t *) timer));  
//			assert(!uv_is_closing((uv_handle_t *) timer));  
//			//printf("start %d\n", time(NULL));
//			timer->data = stream;
//			pConData->timer[1] = timer;
//			r = uv_timer_start(timer, timer_cb2, 3 * 1000, 3 * 1000);
//			assert(r == 0);
//
//		}
//
//	}
//	else if (nread == 0)//OK, but nothing read
//	{
//
//	}
//	else{
//
//		//ASSERT(uv_last_error(stream->loop).code == UV_EOF);
//
//		if (nread == UV_EOF) {
//			printf("客户端主动断开\n");
//
//		} else if (nread == UV_ECONNRESET) {
//			printf("客户端异常断开\n");
//
//		} else {
//			printf("%s : %s \n", uv_err_name(nread), uv_strerror(nread));
//		}
//
//		conn = container_of(stream, conn_rec, handle);
//		r = uv_shutdown(&conn->shutdown_req, stream, shutdown_cb);
//		ASSERT(r == 0);
//	}
//}

//void shutdown_cb(uv_shutdown_t* req, int status) {
//	conn_rec* conn = container_of(req, conn_rec, shutdown_req);
//	uv_close((uv_handle_t*)&conn->handle, close_cb);
//}
//
//void close_cb(uv_handle_t* handle) {
//	conn_rec* conn = container_of(handle, conn_rec, handle);
//
//	stConnectUserData* pUserData = (stConnectUserData*)handle->data;
//	ASSERT(pUserData);
//	if (pUserData->iUserID)
//	{
//		CUser* pUser = CUserMgr::GetInstance()->GetUserByUserID(pUserData->iUserID);
//		ASSERT(pUser);
//		pUser->UnIntertwineConnect((uv_stream_t*)handle);
//	}
//
//	if(pUserData->timer[0]){
//		//uv_timer_stop(pUserData->timer);
//		////delete pUserData->timer;//todo todo todo！ 删除定时器 有内存野指针重写的bug！！！
//		//pUserData->timer = NULL;
//
//		uv_timer_stop(pUserData->timer[0]);
//		uv_timer_stop(pUserData->timer[1]);
//		pUserData->timer[0] = NULL;
//		pUserData->timer[1] = NULL;
//		// todo  delete timer对象
//	}
//	
//	delete pUserData;
//	free(conn);
//}


//bool send( uv_stream_t* stream, UINT32 msg_id, google::protobuf::Message* pMessage, uv_write_cb write_cb){
//	 ASSERT(uv_is_writable(stream));
//	 uv_buf_t ret_buf;
//	 UINT32 body_len = pMessage->ByteSize();
//	 //UINT32 head_len = sizeof(UINT32) + sizeof(UINT32);//4直接的包长度 + 4字节的消息id
//	 buf_alloc((uv_handle_t*)stream, PACKET_HEAD_LEN + body_len, &ret_buf);
//
//	 ASSERT(ret_buf.base && ret_buf.len);
//	 *((UINT32*)(ret_buf.base)) = PACKET_HEAD_LEN + body_len;
//	 *((UINT32*)(ret_buf.base)+1) = msg_id;
//	 INT32 r = pMessage->SerializeToArray(ret_buf.base + PACKET_HEAD_LEN, body_len);
//	 ASSERT(r);
//
//	 uv_write_t* write_req = (uv_write_t*)req_alloc();
//	 printf("send msg[%d]  bytes len[%d] \n", msg_id, pMessage->ByteSize() + sizeof(UINT32) + sizeof(UINT32));
//	 r = uv_write(write_req, stream, &ret_buf, 1, write_cb);
//
//	 if (r != 0)
//	 {
//		 printf("%s:%s", uv_err_name(r), uv_strerror(r));
//	 }
//
//	 ASSERT(r == 0);
//	 buf_free(&ret_buf);
//
//	 return true;
// }

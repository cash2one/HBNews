#ifndef _LIBUV_HELPER_H_
#define _LIBUV_HELPER_H_

#pragma once

#include "uv.h"

#include "def.h"

//printf("%s:%s", uv_err_name(r), uv_strerror(r));
//#define TRACE_CMH_1 (printf("%s(%d)-<%s>: ",__FILE__, __LINE__, __FUNCTION__), printf)  
#define PRINTF_UV_ERROR(code)  printf("%s(%d)-<%s>: %s:%s \n",__FILE__, __LINE__, __FUNCTION__, uv_err_name(code), uv_strerror(code));


#define DECLEAR_IN_HANDLE_FUNCTION(do_what) \
	e_msg_c_gs req_msg_id = e_msg_c_gs_##do_what##_req;\
	e_msg_gs_c res_msg_id = e_msg_gs_c_##do_what##_res;\
	c_gs_##do_what##_req req;\
	gs_c_##do_what##_res res;\
\


//struct stConnectUserData 
//{
//	UINT32 iUserID;
//	uv_timer_t * timer[2];
//};
//
//
//typedef struct {
//	uv_tcp_t handle;
//	uv_shutdown_t shutdown_req;
//} conn_rec;
//
//
//static uv_tcp_t tcp_server;
//uv_tcp_t& get_tcp_server();


/*
* Request allocator
*/
/*static*/ uv_req_t* req_alloc(void);
/*static*/ void req_free(uv_req_t* uv_req);

/*
* Buffer allocator
*/
/*static*/ void buf_alloc(uv_handle_t* handle, size_t size, uv_buf_t* buf);
/*static*/ void buf_free( uv_buf_t* buf);

//void connection_cb(uv_stream_t* stream, int status);
void alloc_cb(uv_handle_t* handle, size_t suggested_size, uv_buf_t* buf);
//void write_cb(uv_write_t* req, int status);
//void read_cb(uv_stream_t* stream, ssize_t nread, const uv_buf_t* buf);
//void shutdown_cb(uv_shutdown_t* req, int status);
//void close_cb(uv_handle_t* handle);


//bool send( uv_stream_t* stream, UINT32 msg_id, google::protobuf::Message* pMessage, uv_write_cb write_cb);


#endif
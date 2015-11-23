#ifndef _GS_C_MESSAGE_HANDLE_HEADER_
#define _GS_C_MESSAGE_HANDLE_HEADER_

#ifdef RAKNET

bool HandleMsgRegisterOrLogin(unsigned char* data, unsigned int len);
bool HandleMsg_GS_C_Req_Open_Chapter_Map(unsigned char* data, unsigned int len);

#else //ifdef LIBUV

#include "libuv_helper.h"


static void write_cb(uv_write_t* req, int status) {
	ASSERT(req != NULL);
	ASSERT(status == 0);
	//write_cb_called++;
	printf("服务器给客户端发送信息成功\n");
}


bool handle_msg_login(uv_stream_t* stream, const char * buf, int len){

	printf("call login!");

	static char str[] = "response login : hello client!";
	write_reqs.buf.base = str;
	write_reqs.buf.len = strlen(str);
	write_req* w = &write_reqs;


	int r = uv_write(&w->req, stream, &w->buf, 1, write_cb);
	ASSERT(r == 0);

	return false;
}

bool handle_msg_logout(uv_stream_t* stream, const char * buf, int len){
	printf("call logout!");

	static char str[] = "response logout : hello client!";
	write_reqs.buf.base = str;
	write_reqs.buf.len = strlen(str);
	write_req* w = &write_reqs;
	int r = uv_write(&w->req, stream, &w->buf, 1, write_cb);
	ASSERT(r == 0);

	return false;
}

#endif

#endif

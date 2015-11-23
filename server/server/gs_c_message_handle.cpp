//#include <string>
//#include <sstream> 

#ifdef RAKNET
#include "RakNetTypes.h"
#include "RakPeerInterface.h"
#else //ifdef LIBUV

#include "uv.h"

#endif

#include "gs_c_message_define.h"
#include "gs_c_message_handle.h"
#include "gs_c_net_mgr.h"

#include "gs_c_msg_body.pb.h"

#include "user_task_mgr.h"


#ifdef RAKNET
bool HandleMsgRegisterOrLogin( unsigned char* data, unsigned int len )
{
	GS_C_Register_Or_Login st;
	st.ParseFromArray(data, len);


	//CNetMgr::GetInstance()->getRakPeerInterface()->Send()

	return true;
}

bool HandleMsg_GS_C_Req_Open_Chapter_Map( unsigned char* data, unsigned int len )
{
	GS_C_Req_Open_Chapter_Map st;
	st.ParseFromArray(data, len);
	UINT32 iUserID = CNetMgr::GetInstance()->GetCurPacketUserID();

	GS_C_Res_Open_Chapter_Map stRet;
	CUserTaskMgr::GetInstance()->OnGS_C_Req_Open_Chapter_Map(iUserID, st.chapter_id(), stRet);

	//CNetMgr::GetInstance()->getRakPeerInterface()->Send()
	return true;
}
#else //ifdef LIBUV

bool handle_msg_login(uv_stream_t* stream, const char * buf, int len){

	printf("call login!");

	gs_c_login_req st;
	st.ParseFromArray(buf, len);

	/*
	static char str[] = "response login : hello client!";
	write_reqs.buf.base = str;
	write_reqs.buf.len = strlen(str);
	write_req* w = &write_reqs;
	*/

	gs_c_login_res ret;
	ret.set_user_id(1);
	ret.set_token("hello world!");
	ret.set_error_code(0);

	int len = ret.ByteSize();
	uv_buf_t * ret_buf = NULL;
	buf_alloc(stream, len, ret_buf);
	ret.SerializeToArray(ret_buf, ret_buf->base);
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
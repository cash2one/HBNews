/************************************************************************/
/* 

for : 消息及处理函数管理器
author ： baomin
data : 20141230

可以考虑用protobuf
注意对齐为1
*/
/************************************************************************/

#ifndef _GS_C_MESSAGE_HANDLE_MGR_HEADER_
#define _GS_C_MESSAGE_HANDLE_MGR_HEADER_

#include <map>

#ifdef RAKNET
#include "RakNetTypes.h"
#else //ifdef LIBUV
#endif

#include "gs_c_message_define.h"

class CMessageHandlerMgr{

#ifndef RAKNET
	typedef bool (*func_message_handle)(unsigned char* data, unsigned int len);
#else  //ifdef LIBUV
	typedef bool (*func_message_handle) (uv_stream_t* stream, const char * buf, int len);
#endif
	
public:

	//todo 单例工具
	inline static CMessageHandlerMgr* GetInstance(){
		static CMessageHandlerMgr singleton;
		return &singleton;
	}

	bool Init();

	//todo 由协议工具生成和调用
	bool AddMessageHandle(BMMsg msgID, func_message_handle handle);

#ifdef RAKNET
	bool ProcessMessage(RakNet::Packet* pPacket);
#else //ifdef LIBUV
	bool process_msg(uv_stream_t* stream, const char * buf, int len){

		func_message_handle pMsgHandle = m_mapHandle[*((char*)buf)];
		if (pMsgHandle)
		{
			return pMsgHandle(stream, buf, len);
		}
		else{
			printf("没有找到消息处理函数\n");
			return false;
		}

	} 
#endif

private:
	CMessageHandlerMgr(){
		Init();
	}

	//可以用数组来优化 索引为消息号！
	//std::map<unsigned int, func_message_handle> m_mapHandle;
	func_message_handle m_mapHandle[BMMsg::MsgMAX];
};

#endif
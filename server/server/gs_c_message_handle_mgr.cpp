

#ifdef RAKNET
#include "RakNetTypes.h"
#include "RakString.h"
#include "BitStream.h"
#else //ifdef LIBUV
#endif

#include <sstream> 
#include <string.h>

#include "gs_c_message_handle_mgr.h"
#include "gs_c_message_handle.h"

bool CMessageHandlerMgr::AddMessageHandle( BMMsg msgID, func_message_handle handle )
{
	/*
	std::map<unsigned int, func_message_handle>::iterator it = m_mapHandle.find(msgID);
	if(it != m_mapHandle.end()){
		//todo log server
		printf("already exsit handle for %u", msgID);
		return false;
	}
	m_mapHandle.insert(std::make_pair<unsigned int, func_message_handle>(msgID, handle));
	*/

	if (m_mapHandle[msgID])
	{
		printf("already exsit handle for %u", msgID);
		return false;
	}
	
	m_mapHandle[msgID] = handle;
	return true;
}

#ifdef RAKNET
bool CMessageHandlerMgr::ProcessMessage( RakNet::Packet* pPacket)
{
	unsigned int msgID = *((unsigned int*)(pPacket->data + sizeof(RakNet::MessageID)));
	unsigned char* data = pPacket->data + sizeof(RakNet::MessageID) + sizeof(unsigned int);
	unsigned int len = pPacket->length - sizeof(RakNet::MessageID) - sizeof(unsigned int);

	//assert(msgID > BMMsg::MsgMin && msgID < BMMsg::MsgMAX);//不能因为客户端的错误数据 而导致服务器崩溃
	if (msgID > BMMsg::MsgMin && msgID < BMMsg::MsgMAX && m_mapHandle[msgID])
	{
		return (*(m_mapHandle[msgID]))(data, len);
	}
	else{
		printf("no message handle for %u", msgID);
		return false;
	}
}
#else //ifdef LIBUV
#endif

bool CMessageHandlerMgr::Init()
{
	//todo assert false!
	//也可以直接m_mapHandle[X] = handle  但是这种方式 检测不到重复添加的问题
	bool a = AddMessageHandle(BMMsg::MsgRegisterOrLogin, HandleMsgRegisterOrLogin);
	a = AddMessageHandle(BMMsg::MSG_GS_C_Req_Open_Chapter_Map, HandleMsg_GS_C_Req_Open_Chapter_Map);
	return true;
}

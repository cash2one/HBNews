/************************************************************************/
/* 

for : gameserver 和 client之间的数据结构
author ： baomin
data : 20141230

可以考虑用protobuf
注意对齐为1
*/
/************************************************************************/


#ifndef _GS_C_MESSAGE_DEFINE_HEADER_
#define _GS_C_MESSAGE_DEFINE_HEADER_


enum BMMsg{
	MsgMin = 0,//必须为0  从0开始 方便为了funcmap数组存储
	MsgRegisterOrLogin,

	MSG_GS_C_Req_Open_Chapter_Map,
	MsgMAX
};

#endif

/************************************************************************/
/* 

for : gameserver �� client֮������ݽṹ
author �� baomin
data : 20141230

���Կ�����protobuf
ע�����Ϊ1
*/
/************************************************************************/


#ifndef _GS_C_MESSAGE_DEFINE_HEADER_
#define _GS_C_MESSAGE_DEFINE_HEADER_


enum BMMsg{
	MsgMin = 0,//����Ϊ0  ��0��ʼ ����Ϊ��funcmap����洢
	MsgRegisterOrLogin,

	MSG_GS_C_Req_Open_Chapter_Map,
	MsgMAX
};

#endif

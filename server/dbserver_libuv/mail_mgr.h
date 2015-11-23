#ifndef _mail_mgr_h__
#define _mail_mgr_h__
#pragma once
#include <map>
#include "dbs_gs_proto.pb.h"
#include "def.h"
struct stAddMail  
{
	db_user_mail mail;	//�ʼ�����
	::google::protobuf::RepeatedField< ::google::protobuf::uint32 > user_ids;	//�û�id
	::google::protobuf::RepeatedField< ::google::protobuf::uint32 > mail_ids;	//�ʼ�id
	::google::protobuf::RepeatedField< ::google::protobuf::uint32 > mail_attach_ids;//�ʼ�����id
	bool all;	//true:��ʾȫ���ʼ� false:��ʾ���ض��˷����ʼ�
};
class CMailMgr
{
public:
	enum EIDType
	{
		eIdType_Min,
		eIdType_Mail,			//�ʼ�
		eIdType_MailAttach,		//�ʼ�����
		eIdType_Max,
	};
	

	static CMailMgr* GetInstance(){
		static CMailMgr singleton;
		return &singleton;
	}

	bool Init();
	
	UINT32 CreateId(EIDType eType);
	//�������id, iCreateNum:��Ҫ����������,iBeginId:��ʼid, iEndId:����id
	void CreateId(EIDType eType, UINT32 iCreateNum, UINT32& iBeginId, UINT32& iEndId);

	void UnReceivedToReceived(db_user_info* user_info);
	
	void HandleNewMailReq( gs_dbs_user_new_mail_req& req, dbs_gs_user_new_mail_res& res);

	void HandleDBOPNewMailReq(gs_dbs_user_info_op_req& req, dbs_gs_user_new_mail_res& res);

	void HandleNewArenaMailReq(gs_dbs_user_new_arena_mail_req& req, dbs_gs_user_new_arena_mail_res& res);
	
private:
	void _fill_mail_ids(db_user_mail* mail, stAddMail* st_mail, dbs_gs_user_new_mail_res& res);
	CMailMgr(void){};
	std::map<EIDType, UINT32> m_mapId;
};

#endif
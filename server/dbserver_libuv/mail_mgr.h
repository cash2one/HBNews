#ifndef _mail_mgr_h__
#define _mail_mgr_h__
#pragma once
#include <map>
#include "dbs_gs_proto.pb.h"
#include "def.h"
struct stAddMail  
{
	db_user_mail mail;	//邮件主体
	::google::protobuf::RepeatedField< ::google::protobuf::uint32 > user_ids;	//用户id
	::google::protobuf::RepeatedField< ::google::protobuf::uint32 > mail_ids;	//邮件id
	::google::protobuf::RepeatedField< ::google::protobuf::uint32 > mail_attach_ids;//邮件附件id
	bool all;	//true:表示全服邮件 false:表示给特定人发的邮件
};
class CMailMgr
{
public:
	enum EIDType
	{
		eIdType_Min,
		eIdType_Mail,			//邮件
		eIdType_MailAttach,		//邮件附件
		eIdType_Max,
	};
	

	static CMailMgr* GetInstance(){
		static CMailMgr singleton;
		return &singleton;
	}

	bool Init();
	
	UINT32 CreateId(EIDType eType);
	//创建多个id, iCreateNum:需要创建的数量,iBeginId:起始id, iEndId:结束id
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
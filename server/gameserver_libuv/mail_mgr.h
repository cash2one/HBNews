#ifndef mail_mgr_h__
#define mail_mgr_h__
#pragma once

#include <map>
#include "def.h"
#include "gs_c_proto.pb.h"
#include "dbs_gs_proto.pb.h"
#include "sys_setting_mgr.h"
#include "config.h"
#include "user.h"
enum EIDType
{
	eIdType_Min,
	eIdType_Mail,			//邮件
	eIdType_MailAttach,		//邮件附件
	eIdType_Max,
};

//enum eSendType
//{
//	eSendType_AllUsers,			//所有玩家
//	eSendType_SpecificUsers,	//特定玩家	
//};

class gm_gs_mail_req;
class CMailMgr
{
public:
	static CMailMgr* GetInstance(){
		static CMailMgr singleton;
		return &singleton;
	}
	
	void HandleNewMailRes(dbs_gs_user_new_mail_res res);
	
	void HandleNewArenaMailRes(dbs_gs_user_new_arena_mail_res res);
	//根据邮件类型检查邮件参数设置是否正确
	bool CheckValueCorrect( MAIL_TYPE eMailType, const char* strParams_utf8);
	bool CheckValueCorrect( MAIL_TYPE eMailType, std::map<std::string, CValue>* mapParams);
	bool SendMailToUsers( MAIL_TYPE eMailType, std::vector<UINT32>* vTargetUsers, const char* strParams_utf8, std::map<std::string, CValue>* mapParams , std::vector<stGameElement>* vAttachs);
	bool SendMailToUsers(const gm_gs_mail_req* req);
	bool Fill_Arena_Mail_Req(MAIL_TYPE eMailType, db_user_mail& mail, const char* strParams_utf8, std::map<std::string, CValue>* mapParams, std::vector<stGameElement>* vAttachs = NULL);
private:
	CMailMgr() {}
	bool _fill_db_mail_req(MAIL_TYPE eMailType, gs_dbs_user_new_mail_req& db_mail_req, std::vector<UINT32>* vecTargetUserIDs, const char* strParams_utf8, std::map<std::string, CValue>* mapParams, std::vector<stGameElement>* vAttachs = NULL);
	std::map<EIDType, UINT32> m_mapId;
};


#endif
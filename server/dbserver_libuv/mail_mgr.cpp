#include "mail_mgr.h"
#include "db_mgr.h"
#include "user_mgr.h"
#include "db_async_command.h"
bool CMailMgr::Init()
{
	MYSQL* pCon = CDBMgr::GetInstance()->GetConnection();
	CHAR sql[1024] =  {0};
	//删除过期邮件
	UINT32 iCode = CDBMgr::Query(pCon, "CALL del_out_of_date_mail();", strlen("CALL del_out_of_date_mail();"));
	if (0 != iCode){   //非0查询失败  
		printf("query failed! [%s] [%d] [%s] \n", "CALL del_out_of_date_mail();", mysql_errno(pCon), mysql_error(pCon));
		return false;
	}

	sprintf_s(sql, sizeof(sql), 
		"SELECT MAX(`mail_id`) FROM `user_mail`;"
		"SELECT MAX(`id`) FROM user_mail_attach;");
	iCode = CDBMgr::Query(pCon, sql, strlen(sql));
	if (0 != iCode)   //非0查询失败  
	{
		printf("query failed! [%s] [%d] [%s] \n", sql, mysql_errno(pCon), mysql_error(pCon));
		return false;
	}

	int i = 0 ;
	do 
	{
		MYSQL_RES* pRes = mysql_store_result(pCon); 
		switch (i++)
		{
		case 0:
			{
				MYSQL_ROW row;
				if (row = mysql_fetch_row(pRes))
				{
					if (0 == row[0])
					{
						m_mapId[eIdType_Mail] = 0;
						break;
					}
					m_mapId[eIdType_Mail] = atoi(row[0]);
				}

			}
			break;
		case 1:
			{
				MYSQL_ROW row;
				if (row = mysql_fetch_row(pRes))
				{
					if (0 == row[0])
					{
						m_mapId[eIdType_MailAttach] = 0;
						break;
					}
					m_mapId[eIdType_MailAttach] = atoi(row[0]);
				}
			}
			break;
		}
		mysql_free_result(pRes);
	} while (!mysql_next_result(pCon));
	return true;
}

void CMailMgr::UnReceivedToReceived( db_user_info* user_info )
{
	if (!user_info)
	{
		return;
	}
	if(!user_info->has_mails_unreceived())
		return;
	db_user_mail_array* mail_unreceive = user_info->mutable_mails_unreceived();
	::google::protobuf::RepeatedPtrField< ::db_user_mail >::iterator it = mail_unreceive->mutable_mails()->begin();

	for(; it != mail_unreceive->mutable_mails()->end(); ++it)
	{
		it->set_mail_id(CreateId(eIdType_Mail));

		if (it->attachs_size())
		{
			::google::protobuf::RepeatedPtrField< ::db_mail_attach >::iterator iter_att = it->mutable_attachs()->begin();
			for (; iter_att != it->mutable_attachs()->end(); ++iter_att)
			{
				iter_att->set_attach_id(CreateId(eIdType_MailAttach));
			}
		}
		
		//将邮件加到玩家已接收
		db_user_mail* pMail = user_info->mutable_mails_received()->add_mails();
		pMail->CopyFrom(*it);
	}
	user_info->set_allocated_mails_unreceived(NULL);
}

void CMailMgr::HandleNewMailReq( gs_dbs_user_new_mail_req& req, dbs_gs_user_new_mail_res& res)
{
	stAddMail* st_mail = new stAddMail;
	db_user_mail* new_mail = req.mutable_mail();
	st_mail->mail.CopyFrom(req.mail());
	res.mutable_mail()->CopyFrom(req.mail());
	if(req.send_user_ids_size())//给特定人添加邮件
	{
		res.mutable_user_ids()->CopyFrom(req.send_user_ids());
		st_mail->user_ids.CopyFrom(req.send_user_ids());
		st_mail->all = false;
		//1.查看这些玩家是否在线，如果在线直接同步
		::google::protobuf::RepeatedField< ::google::protobuf::uint32 >::iterator it = req.mutable_send_user_ids()->begin();
		for(; it != req.mutable_send_user_ids()->end(); ++it){
			
			_fill_mail_ids(new_mail, st_mail, res);

			db_user_info* pUser = CUserMgr::GetInstance()->GetUserInfo(*it);
			if (pUser)//在线同步
			{
				gs_dbs_user_info_op_req db_req;
				db_req.set_user_id(pUser->mutable_user_base_info()->user_id());
				gs_dbs_user_info_op_req::struct_op_mail* op_mail =  db_req.add_op_mails();
				op_mail->set_op_type(gs_dbs_user_info_op_req::Add);
				op_mail->mutable_mail()->CopyFrom(*new_mail);
				CUserMgr::GetInstance()->Update(&db_req);
			}
		}
	}
	else//给所有人添加
	{
		st_mail->all = true;
		//1.给在线的所有玩家同步内存
		const std::map<UINT32, db_user_info*>& users = CUserMgr::GetInstance()->GetAllUsers();
		for (std::map<UINT32, db_user_info*>::const_iterator it = users.begin(); it != users.end(); ++it)
		{
			res.add_user_ids(it->first);
			st_mail->user_ids.Add(it->first);
			_fill_mail_ids(new_mail, st_mail, res);

			gs_dbs_user_info_op_req db_req;
			db_req.set_user_id(it->first);
			gs_dbs_user_info_op_req::struct_op_mail* op_mail =  db_req.add_op_mails();
			op_mail->set_op_type(gs_dbs_user_info_op_req::Add);
			op_mail->mutable_mail()->CopyFrom(*new_mail);
			CUserMgr::GetInstance()->Update(&db_req);
		}
		//2.在线的插入到已接收表中
		//3.给在线的插入到数据库中

	}	
	res.set_error_code(e_dbsgs_errorcode_success);
	CUserCommand* pCommand = new CUserCommand;
	pCommand->Optype(CUserCommand::OP_MAIL);
	//pCommand->Gs_connect_id(ctx->ConnectID); 
	pCommand->Data(st_mail);//保持对象

	CAsyncCommandModule::GetInstance()->AddCommand(pCommand);
}


void CMailMgr::_fill_mail_ids(db_user_mail* mail, stAddMail* st_mail, dbs_gs_user_new_mail_res& res){
	UINT32 iMailID = CreateId(eIdType_Mail);
	mail->set_mail_id(iMailID);
	st_mail->mail_ids.Add(iMailID);
	res.add_mail_ids(iMailID);

	if (mail->attachs_size())
	{
		::google::protobuf::RepeatedPtrField< ::db_mail_attach >::iterator it = mail->mutable_attachs()->begin();
		for (; it != mail->mutable_attachs()->end(); ++it)
		{
			UINT32 iMailAttID;
			iMailAttID = CreateId(eIdType_MailAttach);
			st_mail->mail_attach_ids.Add(iMailAttID);
			it->set_attach_id(iMailAttID);
			res.add_mail_attach_ids(iMailAttID);
		}
	}
}

UINT32 CMailMgr::CreateId(EIDType eType)
{
	ASSERT(eType > eIdType_Min && eType < eIdType_Max);
	return ++m_mapId[eType];	
}

void CMailMgr::CreateId(EIDType eType, UINT32 iCreateNum, UINT32& iBeginId, UINT32& iEndId)
{
	ASSERT(eType > eIdType_Min && eType < eIdType_Max);
	iBeginId = m_mapId[eType] + 1;
	m_mapId[eType] = iEndId = m_mapId[eType] + iCreateNum;
}

void CMailMgr::HandleDBOPNewMailReq( gs_dbs_user_info_op_req& req, dbs_gs_user_new_mail_res& res )
{
	gs_dbs_user_new_mail_req mail_req;
	mail_req.add_send_user_ids(req.user_id());
	mail_req.mutable_mail()->CopyFrom(*req.mutable_new_mail()->mutable_mail());
	req.set_allocated_new_mail(NULL);
	HandleNewMailReq(mail_req, res);
}

void CMailMgr::HandleNewArenaMailReq( gs_dbs_user_new_arena_mail_req& req, dbs_gs_user_new_arena_mail_res& res )
{
	::google::protobuf::RepeatedPtrField< ::arena_mail >::iterator iter = req.mutable_mails()->begin();
	for (; iter != req.mutable_mails()->end(); ++iter)
	{
		UINT32 iUserID = iter->user_id();
		db_user_mail* mail = iter->mutable_mail();

		UINT32 iMailID = CreateId(eIdType_Mail);
		mail->set_mail_id(iMailID);

		if (mail->attachs_size())
		{
			::google::protobuf::RepeatedPtrField< ::db_mail_attach >::iterator it = mail->mutable_attachs()->begin();
			for (; it != mail->mutable_attachs()->end(); ++it)
			{
				UINT32 iMailAttID;
				iMailAttID = CreateId(eIdType_MailAttach);
				it->set_attach_id(iMailAttID);
			}
		}

		//如果在线同步
		db_user_info* pUser = CUserMgr::GetInstance()->GetUserInfo(iUserID);
		if (pUser)
		{
			gs_dbs_user_info_op_req db_req;
			db_req.set_user_id(pUser->mutable_user_base_info()->user_id());
			gs_dbs_user_info_op_req::struct_op_mail* op_mail =  db_req.add_op_mails();
			op_mail->set_op_type(gs_dbs_user_info_op_req::Add);
			op_mail->mutable_mail()->CopyFrom(*mail);
			CUserMgr::GetInstance()->Update(&db_req);
		}
	}
	res.set_error_code(e_dbsgs_errorcode_success);
	res.mutable_mails()->CopyFrom(req.mails());
	CUserCommand* pCommand = new CUserCommand;
	pCommand->Optype(CUserCommand::OP_ARENA_MAIL);
	//pCommand->Gs_connect_id(ctx->ConnectID); 
	pCommand->Data(&req);//保持对象

	CAsyncCommandModule::GetInstance()->AddCommand(pCommand);
}

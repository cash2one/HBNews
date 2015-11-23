#include "mail_mgr.h"
#include "db_mgr.h"
#include "client_for_dbs.h"
#include "user.h"
#include "user_mgr.h"
#include "str.h"
#include "server_for_user.h"
#include "gs_gm_proto.pb.h"

bool CMailMgr::SendMailToUsers( MAIL_TYPE eMailType, std::vector<UINT32>* vTargetUsers, const char* strParams_utf8, std::map<std::string, CValue>* mapParams , std::vector<stGameElement>* vAttachs )
{
	CHECK_FALSE_RETURN_FALSE(MAIL_TYPE_IsValid(eMailType));

	if(strParams_utf8){
		ASSERT(mapParams == NULL);
		CHECK_FALSE_RETURN_FALSE(CheckValueCorrect(eMailType, strParams_utf8));
	}
	else{
		ASSERT(strParams_utf8 == NULL);
		CHECK_FALSE_RETURN_FALSE(CheckValueCorrect(eMailType, mapParams));
	}
	
	gs_dbs_user_new_mail_req* db_mail_req = new gs_dbs_user_new_mail_req;
	_fill_db_mail_req(eMailType, *db_mail_req, vTargetUsers, strParams_utf8, mapParams, vAttachs);
	CClientForDBS::GetInstance()->SendAsync(CClientForDBSAsync::EASyncOP_User_New_Mail, 0, db_mail_req);

	return true;
}

bool CMailMgr::SendMailToUsers(const gm_gs_mail_req* req)
{
	ASSERT(req);
	MAIL_TYPE eMailType = (MAIL_TYPE)req->type();
	
	std::vector<stGameElement> vAttatchs;
	for (::google::protobuf::RepeatedPtrField< ::game_element >::const_iterator it = req->elements().begin(); it != req->elements().end(); ++it){
		vAttatchs.push_back(stGameElement((EELEMENTTYPE)it->type(), it->id(), NULL, stGameElement::INCREASE, it->num()));
	}

	std::vector<UINT32> vUsers;
	for ( ::google::protobuf::RepeatedField< ::google::protobuf::uint32 >::const_iterator it = req->users().begin() ; it != req->users().end() ; ++it){
		vUsers.push_back(*it);
	}

	return  SendMailToUsers(eMailType, &vUsers, req->content().c_str(), NULL, &vAttatchs);
}


bool CMailMgr::CheckValueCorrect( MAIL_TYPE eMailType, std::map<std::string, CValue>* mapParams )
{
	switch(eMailType)
	{
	case MAIL_TYPE_SYS_NOTICE:
		{
			ASSERT(mapParams);
			CHECK_NULL_RETURN_FALSE(mapParams);
			ASSERT(mapParams->size() == 1);
			if (mapParams->size() != 1)
			{
				return false;
			}
			ASSERT(mapParams->find("string") != mapParams->end());
			CHECK_FALSE_RETURN_FALSE(mapParams->find("string") != mapParams->end());
		}
		break;
	case MAIL_TYPE_RECHARGE_SUCCESS:
	case MAIL_TYPE_VIP_LEVEL_UP:
	case MAIL_TYPE_ARENA_TOP_RANK:
		{
			ASSERT(mapParams);
			CHECK_NULL_RETURN_FALSE(mapParams);
			ASSERT(mapParams->size() == 1);
			if (mapParams->size() != 1)
			{
				return false;
			}
			ASSERT(mapParams->find("num1") != mapParams->end());
		}
		break;
	case MAIL_TYPE_ARENA_RANK_REWARD:
		{
			ASSERT(mapParams);
			CHECK_NULL_RETURN_FALSE(mapParams);
			ASSERT(mapParams->size() == 2);
			if (mapParams->size() != 2)
			{
				return false;
			}
			ASSERT(mapParams->find("num1") != mapParams->end());
			ASSERT(mapParams->find("num2") != mapParams->end());
			CHECK_FALSE_RETURN_FALSE(mapParams->find("num1") != mapParams->end());
			CHECK_FALSE_RETURN_FALSE(mapParams->find("num2") != mapParams->end());
		}
		break;
	case MAIL_TYPE_OVERLAP_ITEMS:
		break;
	default:
		ASSERT(false);
		return false;
	}
	return true;
}

//只作参数个数的检查
bool CMailMgr::CheckValueCorrect( MAIL_TYPE eMailType, const char* strParams_utf8)
{
	//if (!content){
	//	return false;
	//}

	std::vector<std::string> vParams;
	split_string(std::string(strParams_utf8), vParams, ",");
	switch(eMailType)
	{
	case MAIL_TYPE_SYS_NOTICE:
		{
			//ASSERT(strParams);
			//CHECK_NULL_RETURN_FALSE(strParams);
			ASSERT(vParams.size() == 1);
			CHECK_FALSE_RETURN_FALSE(vParams.size() == 1);
		}
		break;
	case MAIL_TYPE_RECHARGE_SUCCESS:
	case MAIL_TYPE_VIP_LEVEL_UP:
	case MAIL_TYPE_ARENA_TOP_RANK:
		{
			//ASSERT(values);
			//CHECK_NULL_RETURN_FALSE(values);
			ASSERT(vParams.size() == 1);
			if (vParams.size() != 1)
			{
				return false;
			}
			//ASSERT(values->find("num1") != values->end());
		}
		break;
	case MAIL_TYPE_ARENA_RANK_REWARD:
		{
			//ASSERT(values);
			//CHECK_NULL_RETURN_FALSE(values);
			ASSERT(vParams.size() == 2);
			if (vParams.size() != 2)
			{
				return false;
			}
			//ASSERT(values->find("num1") != values->end());
			//ASSERT(values->find("num2") != values->end());
		}
		break;
	case MAIL_TYPE_OVERLAP_ITEMS:
		break;
	default:
		ASSERT(false);
		return false;
	}
	return true;
}

bool CMailMgr::_fill_db_mail_req( MAIL_TYPE eMailType, gs_dbs_user_new_mail_req& db_mail_req, std::vector<UINT32>* vecTargetUserIDs, const char* strParams_utf8, std::map<std::string, CValue>* mapParams, std::vector<stGameElement>* vAttachs /*= NULL*/ )
{
	db_user_mail* user_mail = db_mail_req.mutable_mail();
	user_mail->set_mail_id(0);
	user_mail->set_send_time(Now());
	user_mail->set_type(eMailType);
	
	char buf[1024] = {0};
	if (strParams_utf8){
		
		ASSERT(mapParams == NULL);
		
		char* ansiParams = NULL;
		switch(eMailType)
		{
		case MAIL_TYPE_SYS_NOTICE:
			{
				ansiParams = UTF8ToANSI(strParams_utf8);
				sprintf_s(buf, "5&%s", ansiParams);
				free(ansiParams);
			}break;
		default:
			ASSERT(false && "不支持类型");//注意新加时注意需要拼接字符串
			break;
		}
	}
	else{
		ASSERT(strParams_utf8 == NULL);
		switch(eMailType)
		{
		case MAIL_TYPE_SYS_NOTICE:
			{
				CHECK_FALSE_RETURN_FALSE(mapParams && mapParams->size() == 1);
				sprintf_s(buf, sizeof(buf), "5&%s", (*mapParams)["string"].GetStringValue());
			}
			break;
		case MAIL_TYPE_RECHARGE_SUCCESS:
		case MAIL_TYPE_VIP_LEVEL_UP:
		case MAIL_TYPE_ARENA_TOP_RANK:
			{
				CHECK_FALSE_RETURN_FALSE(mapParams && mapParams->size() == 1);
				sprintf_s(buf, sizeof(buf), "2&%d", (*mapParams)["num1"].GetIntValue());
			}
			break;
		case MAIL_TYPE_ARENA_RANK_REWARD:
			{
				CHECK_FALSE_RETURN_FALSE(mapParams && mapParams->size() == 2);
				sprintf_s(buf, sizeof(buf), "3&%d|||2&%d", (*mapParams)["num1"].GetIntValue(), (*mapParams)["num2"].GetIntValue());
			}
			break;
		case MAIL_TYPE_OVERLAP_ITEMS:
			{
				sprintf_s(buf, sizeof(buf), "4");
			}
		default:
			ASSERT(false);
			return false;
		}
	}
	
	char* content = ANSIToUTF8(buf);
	user_mail->set_content(content);
	free(content);

	user_mail->set_status(MAIL_STATUS_NOT_READED);
	if(vAttachs && vAttachs->size())
	{
		for(size_t i = 0; i < vAttachs->size(); ++i)
		{
			db_mail_attach* pAtt = user_mail->add_attachs();
			pAtt->set_attach_id(0);
			struct_game_element* pEle = pAtt->mutable_element();
			pEle->set_id((*vAttachs)[i].iID);
			pEle->set_num((*vAttachs)[i].iNum);
			pEle->set_type(struct_game_element::EELEMENTTYPE((*vAttachs)[i].eType));
			pEle->set_num_type(struct_game_element::INCREASE);
		}
	}
	
	if(vecTargetUserIDs && vecTargetUserIDs->size())
	{
		for (size_t i = 0; i < vecTargetUserIDs->size(); ++i)
		{
			db_mail_req.add_send_user_ids((*vecTargetUserIDs)[i]);
		}
	}
	return true;
}

void CMailMgr::HandleNewMailRes( dbs_gs_user_new_mail_res res )
{
	if (res.error_code() == e_dbsgs_errorcode_success)
	{
		::google::protobuf::RepeatedField< ::google::protobuf::uint32 >::iterator it_user_id = res.mutable_user_ids()->begin();
		::google::protobuf::RepeatedField< ::google::protobuf::uint32 >::iterator it_mail_id = res.mutable_mail_ids()->begin();
		::google::protobuf::RepeatedField< ::google::protobuf::uint32 >::iterator it_attach_id = res.mutable_mail_attach_ids()->begin();
		db_user_mail* mail = res.mutable_mail();
		for (; it_user_id != res.mutable_user_ids()->end(); ++it_user_id)
		{
			for (; it_mail_id != res.mutable_mail_ids()->end(); )
			{
				mail->set_mail_id(*it_mail_id);
				for (;it_attach_id != res.mutable_mail_attach_ids()->end();)
				{
					if(mail->attachs_size()){
						::google::protobuf::RepeatedPtrField< ::db_mail_attach >::iterator it_attach = mail->mutable_attachs()->begin();
						for (; it_attach != mail->mutable_attachs()->end(); ++it_attach)
						{
							it_attach->set_attach_id(*it_attach_id);
							it_attach_id++;
						}
					}
					break;
				}
				++it_mail_id;
				break;
			}
			//查看玩家是否在线，如果在线给玩家发送邮件
			CUser* pUser = CUserMgr::GetInstance()->GetUserByUserID(*it_user_id);
			if (pUser)
			{
				GS_C_NTF ntf;
				gs_dbs_user_info_op_req * db_req = new gs_dbs_user_info_op_req;
				pUser->AddMail(*mail, *db_req, ntf);
				CServerForUser::GetInstance()->SendDBReq(db_req);
				pUser->SendNtf(ntf);
			}
		}
	}
	else{
		printf("数据库添加新邮件返回失败了！！！");
	}
	
}

bool CMailMgr::Fill_Arena_Mail_Req( MAIL_TYPE eMailType, db_user_mail& user_mail, const char* strParams_utf8, std::map<std::string, CValue>* mapParams, std::vector<stGameElement>* vAttachs /*= NULL*/ )
{
	user_mail.set_mail_id(0);
	user_mail.set_send_time(Now());
	user_mail.set_type(eMailType);

	if (strParams_utf8){
		ASSERT(mapParams == NULL);
		user_mail.set_content(strParams_utf8);
	}
	else{
		ASSERT(strParams_utf8 == NULL);
		char buf[128] = {0};
		CHECK_FALSE_RETURN_FALSE(mapParams && mapParams->size() == 2);
		sprintf_s(buf, sizeof(buf), "3&%d|||2&%d", (*mapParams)["num1"].GetIntValue(), (*mapParams)["num2"].GetIntValue());
		char* pContent = ANSIToUTF8(buf);
		user_mail.set_content(pContent);
		free(pContent);
	}

	user_mail.set_status(MAIL_STATUS_NOT_READED);
	if(vAttachs && vAttachs->size())
	{
		for(size_t i = 0; i < vAttachs->size(); ++i)
		{
			db_mail_attach* pAtt = user_mail.add_attachs();
			pAtt->set_attach_id(0);
			struct_game_element* pEle = pAtt->mutable_element();
			pEle->set_id((*vAttachs)[i].iID);
			pEle->set_num((*vAttachs)[i].iNum);
			pEle->set_type(struct_game_element::EELEMENTTYPE((*vAttachs)[i].eType));
			pEle->set_num_type(struct_game_element::INCREASE);
		}
	}
	return true;
}

void CMailMgr::HandleNewArenaMailRes( dbs_gs_user_new_arena_mail_res res )
{
	if (res.error_code() == e_dbsgs_errorcode_success)
	{
		::google::protobuf::RepeatedPtrField< ::arena_mail >::iterator it = res.mutable_mails()->begin();
		for (; it != res.mutable_mails()->end(); ++it)
		{
			UINT32 iUserID = it->user_id();
			db_user_mail* user_mail = it->mutable_mail();

			//查看玩家是否在线，如果在线给玩家发送邮件
			CUser* pUser = CUserMgr::GetInstance()->GetUserByUserID(iUserID);
			if (pUser)
			{
				GS_C_NTF ntf;
				gs_dbs_user_info_op_req * db_req = new gs_dbs_user_info_op_req;
				pUser->AddMail(*user_mail, *db_req, ntf);
				CServerForUser::GetInstance()->SendDBReq(db_req);
				pUser->SendNtf(ntf);
			}
		}
	}
	else{
		printf("数据库添加竞技场邮件返回失败了！！！");
	}
}

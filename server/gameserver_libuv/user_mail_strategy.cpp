#include "user_mail_strategy.h"
#include "config.h"
#include "mail_mgr.h"
#include "time_helper.h"
#include "str.h"
#include "sys_setting_mgr.h"
#include "user.h"
bool CUserMailStrategy::Init(CUser* pUser, db_user_mail_array* pReceMailArr, gs_dbs_user_info_op_req& db_req)
{
	ASSERT(pUser && pReceMailArr);
	m_pUser = pUser;
	m_DBMails = pReceMailArr;
	
	//过期邮件时机目前有三个(gs启动时，玩家上线存储过程中，玩家操作邮件时)

	::google::protobuf::RepeatedPtrField< ::db_user_mail >::iterator iter = pReceMailArr->mutable_mails()->begin();
	for (; iter != pReceMailArr->mutable_mails()->end(); ++iter)
	{
		ASSERT(m_mapMails.find(iter->mail_id()) == m_mapMails.end());
		m_mapMails[iter->mail_id()] = &*iter;
	}

	//此处需要删除玩家超过数量的邮件(添加全服邮件的时候可能超过玩家邮件的数量)
	UINT32 iMaxNum = 0;
	bool r = CSysSettingMgr::GetInstance()->GetIntValue("Mail_Max_Num", iMaxNum);
	ASSERT(r);
	while(m_DBMails->mails_size() > iMaxNum){
		GS_C_NTF ntf;//此处就不用发给客户端了(后面填充信息的时候直接获取)
		DelOldestMail(db_req, ntf);
	}
	return true;
}

bool CUserMailStrategy::CreateMail(MAIL_TYPE eMailType, gs_dbs_user_info_op_req& db_req, const char* strParams /*= NULL*/, std::map<std::string, CValue>* mapParams/* = NULL*/, std::vector<stGameElement*>* pvecstGameEle /*= NULL*/ ) 
{
	if (strParams)
	{
		CMailMgr::GetInstance()->CheckValueCorrect(eMailType, strParams);
	}

	if (mapParams)
	{
		CMailMgr::GetInstance()->CheckValueCorrect(eMailType, mapParams);
	}

	db_user_mail* user_mail = db_req.mutable_new_mail()->mutable_mail();
	user_mail->set_mail_id(0);
	user_mail->set_send_time(Now());
	user_mail->set_type(eMailType);
	
	//!现在发给客户端的格式需要拼接字符串，类似跑马灯
	if (strParams)//直接传以,逗号隔开的内容
	{
		ASSERT(false);
		user_mail->set_content(strParams);
	}
	else//自己拼接
	{
		char buf[1024] = {0};
		
		switch(eMailType)
		{
		case MAIL_TYPE_SYS_NOTICE:
			{	
				sprintf_s(buf, sizeof(buf), "5&%s", (*mapParams)["string"].GetStringValue());
			}
			break;
		case MAIL_TYPE_RECHARGE_SUCCESS:
		case MAIL_TYPE_VIP_LEVEL_UP:
		case MAIL_TYPE_ARENA_TOP_RANK:
			{
				sprintf_s(buf, sizeof(buf), "2&%d", (*mapParams)["num1"].GetIntValue());
			}
			break;
		case MAIL_TYPE_ARENA_RANK_REWARD:
			{
				sprintf_s(buf, sizeof(buf), "3&%d|||2&%d", (*mapParams)["num1"].GetIntValue(), (*mapParams)["num2"].GetIntValue());
			}
			break;
		case MAIL_TYPE_OVERLAP_ITEMS:
			{
				sprintf_s(buf, sizeof(buf), "4");
			}
			break;
		case MAIL_TYPE_PRE_MONTH_ADD_SIGN:
		case MAIL_TYPE_MAIL_GUIDE:
			break;
		default:
			ASSERT(false);
			return false;
		}
		char* mail_content = ANSIToUTF8(buf);
		user_mail->set_content(mail_content);
		free(mail_content);
	}

	user_mail->set_status(MAIL_STATUS_NOT_READED);
	
	//附件处理
	if (pvecstGameEle)
	{
		for(size_t i = 0; i < pvecstGameEle->size(); ++i)
		{
			db_mail_attach* pAtt = user_mail->add_attachs();
			pAtt->set_attach_id(0);
			struct_game_element* pEle = pAtt->mutable_element();
			pEle->set_id((*pvecstGameEle)[i]->iID);
			pEle->set_num((*pvecstGameEle)[i]->iNum);
			pEle->set_type(struct_game_element::EELEMENTTYPE((*pvecstGameEle)[i]->eType));
			pEle->set_num_type(struct_game_element::INCREASE);
		}
	}

	return true;
}

bool CUserMailStrategy::CreateMail(MAIL_TYPE eMailType, gs_dbs_user_info_op_req& db_req, 
								   const char* strParam/* = NULL*/, std::map<std::string, CValue>* values/* = NULL*/, std::vector<stGameElement>* vecstGameEle/* = NULL*/){
	if (vecstGameEle)
	{
		std::vector<stGameElement*> vecpstGameEle;
		for (std::vector<stGameElement>::iterator it = vecstGameEle->begin(); it != vecstGameEle->end(); ++it)
		{
			vecpstGameEle.push_back(&*it);
		}
		return CreateMail(eMailType, db_req, strParam, values, &vecpstGameEle);
	}
	else
	{
		return CreateMail(eMailType, db_req, strParam, values, (std::vector<stGameElement*>*)NULL);
	}
	
}

void CUserMailStrategy::AddMail( db_user_mail& db_mail, gs_dbs_user_info_op_req& req, GS_C_NTF& ntf)
{
	//1.查看邮件是否满了，如果满了将时间最早的删除
	UINT32 iMaxMailNum = 0;
	bool r = CSysSettingMgr::GetInstance()->GetIntValue("Mail_Max_Num", iMaxMailNum);
	ASSERT(r);
	ASSERT(m_DBMails->mails_size() <= iMaxMailNum);
	if(m_DBMails->mails_size() == iMaxMailNum){
		//获取发送时间最早的一封邮件
		DelOldestMail(req, ntf);
	}
	
	//客户端通知
	gs_c_mail_ntf* mail_ntf =  ntf.mutable_mail_ntf_arr()->add_mails();
	mail_ntf->set_type(gs_c_mail_ntf::ADD);

	user_mail* c_mail = mail_ntf->mutable_ele();
	c_mail->set_type(db_mail.type());
	c_mail->set_mail_id(db_mail.mail_id());
	c_mail->set_send_time(db_mail.send_time());
	c_mail->set_type(db_mail.type());
	if (db_mail.has_content())
	{
		c_mail->set_content(db_mail.content());
	}
	c_mail->set_status(db_mail.status());
	if(db_mail.attachs_size()){
		::google::protobuf::RepeatedPtrField< ::db_mail_attach >::iterator itDBEle = db_mail.mutable_attachs()->begin();
		for (; itDBEle != db_mail.mutable_attachs()->end(); ++itDBEle){
			struct_game_element* pResEle = c_mail->add_elements();
			pResEle->CopyFrom(*itDBEle->mutable_element());
		}
	}


	//玩家添加邮件
	db_user_mail* mail = m_DBMails->add_mails();
	mail->CopyFrom(db_mail);
	m_mapMails[mail->mail_id()] = mail;
}


bool CUserMailStrategy::DelMail(UINT32 iMailId, ::google::protobuf::RepeatedPtrField< ::gs_dbs_user_info_op_req_struct_op_mail >* pRepMail, ::google::protobuf::Message* pClientRes /*= NULL*/, bool isOutOfDate /*= false*/)
{
	for (int i = 0; i < m_DBMails->mails_size(); ++i)
	{
		if (iMailId == m_DBMails->mutable_mails(i)->mail_id())
		{
			ITER_MAIL iter = m_mapMails.find(iMailId);
			ASSERT(iter != m_mapMails.end());
			m_mapMails.erase(iter);

			m_DBMails->mutable_mails()->SwapElements(i, m_DBMails->mails_size() - 1);
			m_DBMails->mutable_mails()->RemoveLast();
			
			{//dbreq
				gs_dbs_user_info_op_req_struct_op_mail* pMail = pRepMail->Add();
				pMail->set_op_type(gs_dbs_user_info_op_req::Delete);
				pMail->set_mail_id(iMailId);
			}
			
			if(isOutOfDate)
			{
				if(pClientRes)
				{
					gs_c_mail_ntf* pNtf = ((gs_c_mail_ntf_array*)pClientRes)->add_mails();
					pNtf->set_type(gs_c_mail_ntf::DEL);
					pNtf->set_mail_id(iMailId);
				}
				
			}
			else
			{
				if(pClientRes)
				{
					gs_c_mail_opt_res::mail_update* pMailUpdate = ((gs_c_mail_opt_res*)pClientRes)->add_mail_update_arr();
					pMailUpdate->set_mail_id(iMailId);
					pMailUpdate->set_mail_status(MAIL_STATUS_DELED);
				}
				
			}
			return true;
		}
	}
	return false;
}

bool CUserMailStrategy::HasMail(UINT32 iMailID)
{
	ITER_MAIL iter = m_mapMails.find(iMailID);
	return iter != m_mapMails.end();
}

bool CUserMailStrategy::HasMails(std::vector<UINT32>& vecMailId)
{
	for (size_t i = 0; i < vecMailId.size(); ++i)
	{
		if (!HasMail(vecMailId[i]))
		{
			return false;
		}
	}
	return true;
}


bool CUserMailStrategy::DelMails(std::vector<UINT32>& vecMailId, gs_dbs_user_info_op_req* pDBReq, gs_c_mail_opt_res* pClientRes)
{
	for(size_t i = 0; i < vecMailId.size(); ++i)
	{
		if (!DelMail(vecMailId[i], pDBReq->mutable_op_mails(), pClientRes))
		{
			return false;
		}
	}
	return true;
}


bool CUserMailStrategy::SetMailReaded(UINT32 iMailId, gs_dbs_user_info_op_req* db_req, gs_c_mail_opt_res* pMailRes)
{
	ITER_MAIL iter = m_mapMails.find(iMailId);
	if (iter == m_mapMails.end())
	{
		pMailRes->set_error_code(e_gsc_errorcode_unknown_error);
		return false;
	}
	iter->second->set_status(MAIL_STATUS_READED);
	gs_dbs_user_info_op_req_struct_op_mail* pMail = db_req->add_op_mails();

	pMail->set_mail_id(iMailId);
	pMail->set_op_type(gs_dbs_user_info_op_req::Update);

	{
		gs_c_mail_opt_res::mail_update* pMailUpdate = pMailRes->add_mail_update_arr();
		pMailUpdate->set_mail_id(iMailId);
		pMailUpdate->set_mail_status(MAIL_STATUS_READED);
	}
	
	return true;
}


MAIL_STATUS CUserMailStrategy::GetMailStatus(UINT32 iMailId)
{
	ITER_MAIL iter = m_mapMails.find(iMailId);
	ASSERT(iter != m_mapMails.end());
	return iter->second->status();
}

bool CUserMailStrategy::IsMailsHasAttachs(std::vector<UINT32>& vecMailId)
{
	for(size_t i = 0; i < vecMailId.size(); ++i)
	{
		ITER_MAIL iter = m_mapMails.find(vecMailId[i]);
		if (iter == m_mapMails.end() || 
			(iter != m_mapMails.end() && iter->second->attachs_size() == 0))
		{
			return false;
		}
	}
	return true;
}

std::vector<UINT32> CUserMailStrategy::GetAllHasAttachMailIDS()
{
	std::vector<UINT32> vecMailID;
	ITER_MAIL iter = m_mapMails.begin();
	for (; iter != m_mapMails.end(); ++iter)
	{
		if (iter->second->attachs_size())
		{
			vecMailID.push_back(iter->second->mail_id());
		}
	}
	return vecMailID;
}

bool CUserMailStrategy::GetExistMailWithAttach(gs_dbs_user_info_op_req::struct_new_mail* new_mail, db_user_mail*& pDBMail)
{
	ASSERT(new_mail);
	if (new_mail->mutable_mail()->type() == MAIL_TYPE_OVERLAP_ITEMS)
	{
		pDBMail = new_mail->mutable_mail();
		return true;
	}
	return false;
}

bool CUserMailStrategy::AddAttachTo(const std::vector<struct_game_element*>& pvecGameEle, db_user_mail* pDBMail)
{
	ASSERT(pDBMail);
	for (size_t i = 0; i < pvecGameEle.size(); ++i)
	{
		AddAttachTo(*pvecGameEle[i], pDBMail);
	}
	return true;
}

bool CUserMailStrategy::AddAttachTo(const std::vector<struct_game_element*>& pvecGameEle, user_mail* pUserMail)
{
	ASSERT(pUserMail);
	for (size_t i = 0; i < pvecGameEle.size(); ++i)
	{
		AddAttachTo(*pvecGameEle[i], pUserMail);
	}
	return true;
}


bool CUserMailStrategy::AddAttachTo(const struct_game_element& gameEle, db_user_mail* pDBMail)
{
	ASSERT(pDBMail);
	
	::google::protobuf::RepeatedPtrField< ::db_mail_attach >::iterator iter = pDBMail->mutable_attachs()->begin();
	for (; iter != pDBMail->mutable_attachs()->end(); ++iter)
	{
		if (iter->mutable_element()->type() == gameEle.type())
		{
			if (gameEle.type() != struct_game_element::EELEMENTTYPE_ITEM || iter->mutable_element()->id() == gameEle.id())//如果已经存在添加类型的附件，直接设置数量
			{
				iter->mutable_element()->set_num(iter->mutable_element()->num() + gameEle.num());
				return true;
			}
		}
	}

	db_mail_attach* pAttach = pDBMail->add_attachs();
	pAttach->set_attach_id(0);
	pAttach->mutable_element()->CopyFrom(gameEle);

	return true;
}

bool CUserMailStrategy::AddAttachTo(const struct_game_element& gameEle, user_mail* pUserMail)
{
	ASSERT(pUserMail);
	::google::protobuf::RepeatedPtrField< ::struct_game_element >::iterator iter = pUserMail->mutable_elements()->begin();
	for (; iter != pUserMail->mutable_elements()->end(); ++iter)
	{
		if (iter->type() == gameEle.type())
		{
			if (gameEle.type() != struct_game_element::EELEMENTTYPE_ITEM || iter->id() == gameEle.id())//非道具类型直接叠加
			{
				iter->set_num(iter->num() + gameEle.num());
				return true;
			}
		}
	}
	struct_game_element* pEle = pUserMail->add_elements();
	pEle->CopyFrom(gameEle);
	return true;
}

void CUserMailStrategy::DelOutOfDateMail(::google::protobuf::RepeatedPtrField< ::gs_dbs_user_info_op_req_struct_op_mail >* pRepMail, gs_c_mail_ntf_array* pMailNtf, std::vector<struct_game_element>& vecEle)
{
	ASSERT(pRepMail && pMailNtf);
	ITER_MAIL iter = m_mapMails.begin();

	UINT32 iMailUnReadValidTime = 0;
	ASSERT(CSysSettingMgr::GetInstance()->GetIntValue("Mail_UnReaded_Mail_Valid_Time", iMailUnReadValidTime));
	UINT32 iMailReadValidTime = 0;
	ASSERT(CSysSettingMgr::GetInstance()->GetIntValue("Mail_Readed_Mail_Valid_Time", iMailReadValidTime));

	for (; iter != m_mapMails.end(); )
	{
		db_user_mail* pMail = iter->second;
		MAIL_STATUS status =  pMail->status();
		if ((status == MAIL_STATUS_READED && pMail->send_time() + iMailReadValidTime < Now()) ||
			(status == MAIL_STATUS_NOT_READED && pMail->send_time() + iMailUnReadValidTime < Now()))
		{
			ITER_MAIL iterNext = ++iter;
			--iter;
			//获取邮件附件
			std::vector<struct_game_element*> vecpGameEle = _get_mail_attach(iter->second->mail_id());
			for (size_t i = 0; i < vecpGameEle.size(); ++i)
			{
				vecEle.push_back(*vecpGameEle[i]);
			}
			ASSERT(DelMail(pMail->mail_id(), pRepMail, pMailNtf, true));
			iter = iterNext;
		}
		else
		{
			++iter;
		}
	}
}

bool CUserMailStrategy::GetMailAttach(UINT32 iMailID, gs_c_mail_opt_res* pRes, gs_dbs_user_info_op_req* db_req, GS_C_NTF& client_arr)
{
	std::vector<struct_game_element*> vecMailEle = _get_mail_attach(iMailID);
	MAIL_STATUS mailStatus = GetMailStatus(iMailID);

	if (m_pUser->CanAddElements(vecMailEle))
	{
		for (size_t i = 0; i < vecMailEle.size(); ++i)
		{
			m_pUser->UpdateGameElement(*vecMailEle[i], *db_req, *pRes->mutable_output_arr(), client_arr);
		}
		std::vector<UINT32> vecDel;
		vecDel.push_back(iMailID);
		DelMails(vecDel, db_req, pRes);
		return true;
	}	
	else
	{
		pRes->set_error_code(e_gsc_errorcode_not_enough_bag_space);//物品满了

		//领取邮件后该邮件为已读状态
		if(mailStatus != MAIL_STATUS_READED)
		{
			ASSERT(SetMailReaded(iMailID, db_req, pRes));
			return true;
		}
		return false;
	}
}

std::vector<struct_game_element*> CUserMailStrategy::_get_mail_attach(UINT32 iMailID)
{
	std::vector<struct_game_element*> vecEle;
	ITER_MAIL iterMail = m_mapMails.find(iMailID);
	if (iterMail != m_mapMails.end())
	{
		::google::protobuf::RepeatedPtrField< ::db_mail_attach >::iterator iter = iterMail->second->mutable_attachs()->begin();
		for (; iter != iterMail->second->mutable_attachs()->end(); ++iter)
		{
			vecEle.push_back(iter->mutable_element());
		}
	}
	return vecEle;
}


void CUserMailStrategy::DelOldestMail(gs_dbs_user_info_op_req& req, GS_C_NTF& ntf )
{
	UINT32 iOldest = 0;
	for (int i = 1; i < m_DBMails->mails_size(); ++i)
	{
		if (m_DBMails->mails(i).send_time() < m_DBMails->mails(i - 1).send_time())
		{
			iOldest = i;
		}
	}
	
	ITER_MAIL iter = m_mapMails.find(m_DBMails->mutable_mails(iOldest)->mail_id());
	ASSERT(iter != m_mapMails.end());
	
	//给客户端删除邮件通知
	gs_c_mail_ntf* mail_ntf = ntf.mutable_mail_ntf_arr()->add_mails();
	mail_ntf->set_type(gs_c_mail_ntf::DEL);
	mail_ntf->set_mail_id(iter->second->mail_id());
	
	//邮件附件通知客户端
	::google::protobuf::RepeatedPtrField< ::db_mail_attach >::iterator iterAtt = iter->second->mutable_attachs()->begin();
	for (; iterAtt != iter->second->mutable_attachs()->end(); ++iterAtt)
	{
		GS_C_NTF ntf2;	//如果产生邮件，此邮件就不用发了
		m_pUser->UpdateGameElement(*iterAtt->mutable_element(), req, *ntf.mutable_mail_ntf_arr()->mutable_eles(), ntf2);//附件给玩家加上(道具玩家，数据都在此加上了)
	}

	//数据库添加删除命令(邮件)
	gs_dbs_user_info_op_req::struct_op_mail* pMail = req.add_op_mails();
	pMail->set_op_type(gs_dbs_user_info_op_req::Delete);
	pMail->set_mail_id(iter->second->mail_id());

	//玩家删除邮件
	m_mapMails.erase(iter);

	m_DBMails->mutable_mails()->SwapElements(iOldest, m_DBMails->mails_size() - 1);
	m_DBMails->mutable_mails()->RemoveLast();
}

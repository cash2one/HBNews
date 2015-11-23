#include "friend_mgr.h"
#include "user_mgr.h"
#include "sys_setting_mgr.h"
#include "def.h"
#include "client_for_dbs.h"
#define SEND_DB_FRIEND_REQ(user_id, op_req)  CClientForDBS::GetInstance()->SendAsync(CClientForDBSAsync::EASyncOP_User_Friend_OP, user_id, op_req)

bool CFriendMgr::Init()
{
	const char* str = NULL;
	bool r = CSysSettingMgr::GetInstance()->GetStringValue("Friend_Give", str);

	CHECK_NULL_RETURN_FALSE(r && str);
	r = stGameElement::Parse(str, m_friend_give, stGameElement::DECREASE);
	CHECK_FALSE_RETURN_FALSE(r);

	ASSERT(1 == m_friend_give.size() && m_friend_give[0].eType == EELEMENTTYPE_HEALTH);

	r = stGameElement::Parse(str, m_friend_get, stGameElement::INCREASE);
	CHECK_FALSE_RETURN_FALSE(r);

	r = CSysSettingMgr::GetInstance()->GetStringValue("Friend_Give_Back", str);
	CHECK_NULL_RETURN_FALSE(r && str);

	r = stGameElement::Parse(str, m_friend_give_back, stGameElement::INCREASE);
	CHECK_FALSE_RETURN_FALSE(r);
	return true;
}


void CFriendMgr::GiveElementToFriend(UINT32 meID, UINT32 iFriendID, GS_C_NTF& ntf)
{
	CUser* pUser = CUserMgr::GetInstance()->GetUserByUserID(iFriendID);
	if (pUser)
	{
		GS_C_NTF ntf;
		pUser->FriendGiveElement(meID, ntf);
		pUser->SendNtf(ntf);
	}
	gs_dbs_user_friend_op_req* op_req = new gs_dbs_user_friend_op_req;
	op_req->set_op_type(gs_dbs_user_friend_op_req::FRIEND_GIVE_ELEMENT);
	op_req->set_user_id(iFriendID);
	op_req->set_friend_user_id(meID);
	SEND_DB_FRIEND_REQ(meID, op_req);
}

void CFriendMgr::GiveUpApply( UINT32 meID, UINT32 iFriendID, GS_C_NTF& ntf )
{
	CUser* pUser = CUserMgr::GetInstance()->GetUserByUserID(iFriendID);
	if (pUser)
	{
		GS_C_NTF ntf;
		pUser->FriendGiveUpApply(meID, ntf);
		pUser->SendNtf(ntf);
	}
	gs_dbs_user_friend_op_req* op_req = new gs_dbs_user_friend_op_req;
	op_req->set_op_type(gs_dbs_user_friend_op_req::FRIEND_GIVE_UP_APPLY);
	op_req->set_user_id(iFriendID);
	op_req->set_friend_user_id(meID);
	SEND_DB_FRIEND_REQ(meID, op_req);
}

void CFriendMgr::AffirmAgree( UINT32 meID, UINT32 iFriendID, GS_C_NTF& ntf )
{
	CUser* pUser = CUserMgr::GetInstance()->GetUserByUserID(iFriendID);
	ASSERT(pUser);
	if (pUser)
	{
		GS_C_NTF ntf;
		pUser->FriendAffirmAgree(meID, ntf);
		pUser->SendNtf(ntf);
	}
	gs_dbs_user_friend_op_req* op_req = new gs_dbs_user_friend_op_req;
	op_req->set_op_type(gs_dbs_user_friend_op_req::FRIEND_AFFIRM_AGREE);
	op_req->set_user_id(iFriendID);
	op_req->set_friend_user_id(meID);
	SEND_DB_FRIEND_REQ(meID, op_req);
}

void CFriendMgr::AffirmRefuse( UINT32 meID, UINT32 iFriendID, GS_C_NTF& ntf )
{
	CUser* pUser = CUserMgr::GetInstance()->GetUserByUserID(iFriendID);
	if (pUser)
	{
		GS_C_NTF ntf;
		pUser->FriendAffirmRefuse(meID, ntf);
		pUser->SendNtf(ntf);
	}
	gs_dbs_user_friend_op_req* op_req = new gs_dbs_user_friend_op_req;
	op_req->set_op_type(gs_dbs_user_friend_op_req::FRIEND_AFFIRM_REFUSE);
	op_req->set_user_id(iFriendID);
	op_req->set_friend_user_id(meID);
	SEND_DB_FRIEND_REQ(meID, op_req);
}

void CFriendMgr::DelFriend(UINT32 meID, UINT32 iFriendID, GS_C_NTF& ntf)
{
	CUser* pUser = CUserMgr::GetInstance()->GetUserByUserID(iFriendID);
	if (pUser)
	{
		GS_C_NTF ntf;
		pUser->FriendDelFriend(meID, ntf);
		pUser->SendNtf(ntf);
	}
	gs_dbs_user_friend_op_req* op_req = new gs_dbs_user_friend_op_req;
	op_req->set_op_type(gs_dbs_user_friend_op_req::FRIEND_DEL_FRIEND);
	op_req->set_user_id(iFriendID);
	op_req->set_friend_user_id(meID);
	SEND_DB_FRIEND_REQ(meID, op_req);
}


//添加好友在线调用此接口
void CFriendMgr::AddFriend(UINT32 meID, UINT32 iFriendID, GS_C_NTF& ntf, const friend_base_info& base_info)
{
	CUser* pUser = CUserMgr::GetInstance()->GetUserByUserID(iFriendID);
	ASSERT(pUser);
	if (pUser)
	{
		GS_C_NTF ntf;
		pUser->FriendAddFriend(meID, ntf, base_info);
		pUser->SendNtf(ntf);
	}
	gs_dbs_user_friend_op_req* op_req = new gs_dbs_user_friend_op_req;
	op_req->set_op_type(gs_dbs_user_friend_op_req::FRIEND_ADD_FRIEND);
	op_req->set_user_id(iFriendID);
	op_req->set_friend_user_id(meID);
	op_req->mutable_base_info()->CopyFrom(base_info);
	SEND_DB_FRIEND_REQ(meID, op_req);
}

bool CFriendMgr::IsFriendFull( UINT32 inFriendNum )
{
	UINT32 iFriendNum = 0;
	bool r = CSysSettingMgr::GetInstance()->GetIntValue("Friend_Max_Num", iFriendNum);
	ASSERT(r && iFriendNum);
	ASSERT(inFriendNum <= iFriendNum);
	return iFriendNum == inFriendNum;
}
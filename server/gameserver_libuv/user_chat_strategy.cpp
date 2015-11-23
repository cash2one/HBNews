#include "user_chat_strategy.h"
#include "server_for_user.h"
#include "user.h"
#include "user_mgr.h"
bool CUserChatStrategy::Init(CUser* pUser){
	CHECK_NULL_RETURN_FALSE(pUser);
	m_pUser = pUser;
	return true;
}

void CUserChatStrategy::Chat( c_gs_chat_req& req, gs_c_chat_res& res )
{
	switch(req.chat_type()){
		case c_gs_chat_req::WORLD:{
			gs_c_chat_ntf ntf;
			ntf.set_chat_type(req.chat_type());
			ntf.set_user_id(m_pUser->UserID());
			ntf.set_vip(m_pUser->GetVipLevel());
			ntf.set_name(m_pUser->Name());
			ntf.set_head_id(m_pUser->GetHeadID());
			ntf.set_head_frame_id(m_pUser->GetHeadFrameID());
			ntf.set_word(req.word());
			std::vector<UINT32> exUserIDs;
			exUserIDs.push_back(m_pUser->UserID());
			CServerForUser::BroadCast(e_msg_gs_c_chat_ntf, &ntf, NULL, &exUserIDs);
			res.set_error_code(e_gsc_errorcode_success);
								  }break;
		case c_gs_chat_req::GUILD:{
			res.set_error_code(e_gsc_errorcode_unknown_error);
								  }break;
		case c_gs_chat_req::WHISPER:{
			if(!req.has_to_id() && !req.has_to_name()){
				res.set_error_code(e_gsc_errorcode_unknown_error);
				return;
			}
			CUser* pUser = NULL;
			std::map<UINT32, CUser*>* mapUsers = CUserMgr::GetInstance()->GetOnlineUsers();
			ASSERT(mapUsers);
			if (req.has_to_id())
			{
				std::map<UINT32, CUser*>::iterator it = mapUsers->find(req.to_id());
				if (it != mapUsers->end())
				{
					pUser = it->second;
				}
			}else{
				if (req.has_to_name())
				{
					std::map<UINT32, CUser*>::iterator it = mapUsers->begin();
					for (; it != mapUsers->end(); ++it)
					{
						if (0 == strcmp(it->second->Name().c_str(), req.to_name().c_str()))
						{
							pUser = it->second;
							break;
						}
					}
				}
			}
			

			if (!pUser)
			{
				res.set_error_code(e_gsc_errorcode_chat_user_not_online_or_exist);
				return;
			}
			GS_C_NTF ntf;
			gs_c_chat_ntf* chat_ntf = ntf.mutable_chat_ntf();
			chat_ntf->set_chat_type(req.chat_type());
			chat_ntf->set_user_id(m_pUser->UserID());
			chat_ntf->set_vip(m_pUser->GetVipLevel());
			chat_ntf->set_name(m_pUser->Name());
			chat_ntf->set_head_id(m_pUser->GetHeadID());
			chat_ntf->set_head_frame_id(m_pUser->GetHeadFrameID());
			chat_ntf->set_word(req.word());
			pUser->SendNtf(ntf);
			res.set_error_code(e_gsc_errorcode_success);
									}break;
	}
}
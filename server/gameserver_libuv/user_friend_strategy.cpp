#include "user_friend_strategy.h"
#include "sys_setting_mgr.h"
#include "def.h"
#include "time_helper.h"
#include "user.h"
#include "friend_mgr.h"
#include "user_mgr.h"
#include "client_for_dbs.h"
bool CUserFriendStrategy::Init(CUser* pUser, db_user_friend* fri)
{
	ASSERT(fri);
	CHECK_NULL_RETURN_FALSE(fri && pUser);
	m_pUser = pUser;
	m_db_user_friend = fri;

	//�����б�
	if(fri->friends_size())
	{
		::google::protobuf::RepeatedPtrField< ::db_user_friend_friend_info >::iterator it = fri->mutable_friends()->begin();
		for (; it != fri->mutable_friends()->end(); ++it)
		{
			db_user_friend::friend_info* fri = &(*it);
			ASSERT(m_mapFriends.find(fri->mutable_base_info()->user_id()) == m_mapFriends.end());
			m_mapFriends[fri->mutable_base_info()->user_id()] = fri;
		}
	}

	//�����б�
	if (fri->applies_size())
	{
		::google::protobuf::RepeatedPtrField< ::friend_base_info >::iterator it = fri->mutable_applies()->begin();
		for (; it != fri->mutable_applies()->end(); ++it)
		{
			friend_base_info* apply = &(*it);
			ASSERT(m_mapApply.find(apply->user_id()) == m_mapApply.end());
			m_mapApply[apply->user_id()] = apply;
		}
	}

	//��ȷ���б�
	if (fri->confirms_size())
	{
		::google::protobuf::RepeatedPtrField< ::friend_base_info >::iterator it = fri->mutable_confirms()->begin();
		for (; it != fri->mutable_confirms()->end(); ++it)
		{
			friend_base_info* confirm = &(*it);
			ASSERT(m_mapConfirms.find(confirm->user_id()) == m_mapConfirms.end());
			m_mapConfirms[confirm->user_id()] = confirm;
		}
	}

	//�Ƽ��б�
	if (fri->recommends_size())
	{
		::google::protobuf::RepeatedPtrField< ::friend_base_info >::iterator it = fri->mutable_recommends()->begin();
		for (; it != fri->mutable_recommends()->end(); ++it)
		{
			friend_base_info* recommend = &(*it);
			ASSERT(m_mapRecommends.find(recommend->user_id()) == m_mapRecommends.end());
			m_mapRecommends[recommend->user_id()] = recommend;
		}
	}
	return true;
}


void CUserFriendStrategy::UserFriendOP(const c_gs_user_friend_op_req& op_req, gs_c_user_friend_op_res& res, gs_dbs_user_info_op_req& db_req, GS_C_NTF& ntf)
{
	c_gs_user_friend_op_req::OP_TYPE op_type = op_req.op_type();
	switch (op_type)
	{
	case c_gs_user_friend_op_req::GET_ELEMENT1:
		{
			//�鿴�����Ƿ�ﵽ����
			if(m_pUser->IsHealthFull()){
				res.set_error_code(e_gsc_errorcode_health_reach_max);
				return;
			}

			ASSERT(op_req.has_user_id());
			if (!op_req.has_user_id())
			{
				res.set_error_code(e_gsc_errorcode_unknown_error);
				return;
			}
			UINT32 iUserID = op_req.user_id();
			//ASSERT(m_mapFriends.find(iUserID) != m_mapFriends.end());
			if (m_mapFriends.find(iUserID) == m_mapFriends.end())
			{
				res.set_error_code(e_gsc_errorcode_unknown_error);
				return;
			}
			db_user_friend::friend_info* fri = m_mapFriends[iUserID];
			if (!isSameDay(Now(), fri->friend_give_timestamp(), CSysSettingMgr::GetInstance()->Global_Reset_Time))
			{
				res.set_error_code(e_gsc_errorcode_unknown_error);
				return;
			}
			m_pUser->UpdateGameElement(CFriendMgr::GetInstance()->GetFriendGet(), db_req, *res.mutable_eles(), ntf);
			//�޸���������ʱ��
			fri->set_friend_give_timestamp(0);
			//db_req
			gs_dbs_user_info_op_req::struct_op_friend* op_friend = db_req.add_op_friends();
			op_friend->set_op_type(gs_dbs_user_info_op_req::struct_op_friend::GET_ELEMENT1);
			op_friend->set_friend_id(iUserID);
			//c
			res.set_user_id(op_req.user_id());
		}break;
	case c_gs_user_friend_op_req::GET_ELEMENTN:
		{
			if(m_pUser->IsHealthFull()){
				res.set_error_code(e_gsc_errorcode_health_reach_max);
				return;
			}

			std::map<UINT32, db_user_friend::friend_info*>::iterator it = m_mapFriends.begin();
			for (; it != m_mapFriends.end(); ++it)
			{
				if (isSameDay(Now(), it->second->friend_give_timestamp(), CSysSettingMgr::GetInstance()->Global_Reset_Time))
				{
					m_pUser->UpdateGameElement(CFriendMgr::GetInstance()->GetFriendGet(), db_req, *res.mutable_eles(), ntf);
					it->second->set_friend_give_timestamp(0);
				}
			}
			//db_req
			gs_dbs_user_info_op_req::struct_op_friend* op_friend = db_req.add_op_friends();
			op_friend->set_op_type(gs_dbs_user_info_op_req::struct_op_friend::GET_ELEMENTN);
			//c
		}break;
	case c_gs_user_friend_op_req::GIVE_ELEMENT1:
		{
			//ASSERT(op_req.has_user_id());
			if (!op_req.has_user_id())
			{
				res.set_error_code(e_gsc_errorcode_unknown_error);
				return;
			}
			UINT32 iUserID = op_req.user_id();
			//ASSERT(m_mapFriends.find(iUserID) != m_mapFriends.end());
			if (m_mapFriends.find(iUserID) == m_mapFriends.end())
			{
				res.set_error_code(e_gsc_errorcode_unknown_error);
				return;
			}

			//�鿴�����Ƿ����͹�
			db_user_friend::friend_info* fri = m_mapFriends[iUserID];
			if (isSameDay(Now(), fri->me_give_timestamp(), CSysSettingMgr::GetInstance()->Global_Reset_Time))
			{
				res.set_error_code(e_gsc_errorcode_friend_already_give_element_today);
				return;
			}

			//�ж���������
			UINT32 iCurHealth = m_pUser->GetHealth();
			if (iCurHealth < CFriendMgr::GetInstance()->GetFriendGive().iNum)
			{
				res.set_error_code(e_gsc_errorcode_no_enough_health);
				return;
			}

			fri->set_me_give_timestamp(Now());

			//�۳���Ʒ
			//m_pUser->UpdateGameElement(CFriendMgr::GetInstance()->GetFriendGive(), db_req, *res.mutable_eles(), ntf);
			//��÷�����Ʒ
			m_pUser->UpdateGameElement(CFriendMgr::GetInstance()->GetFriendGiveBack(), db_req, *res.mutable_eles(), ntf);

			//�������֪ͨ��������������Ʒ
			CFriendMgr::GetInstance()->GiveElementToFriend(m_pUser->UserID(), iUserID, ntf);
			//db_req
			gs_dbs_user_info_op_req::struct_op_friend* op_friend = db_req.add_op_friends();
			op_friend->set_op_type(gs_dbs_user_info_op_req::struct_op_friend::GIVE_ELEMENT1);
			op_friend->set_friend_id(iUserID);
			//c
			res.set_user_id(iUserID);
		}break;
	case c_gs_user_friend_op_req::GIVE_ELEMENTN:
		{
			//��Ʒ�Ƿ����
			UINT32 iNum = 0;
			std::map<UINT32, db_user_friend::friend_info*>::iterator it = m_mapFriends.begin();
			for (; it != m_mapFriends.end(); ++it){
				if (!isSameDay(Now(), it->second->me_give_timestamp(), CSysSettingMgr::GetInstance()->Global_Reset_Time)){
					iNum++;
				}
			}

			if(m_pUser->GetHealth() < CFriendMgr::GetInstance()->GetFriendGive().iNum * iNum){
				res.set_error_code(e_gsc_errorcode_no_enough_health);
				return;
			}

			it = m_mapFriends.begin();
			for (; it != m_mapFriends.end(); ++it){
				if (!isSameDay(Now(), it->second->me_give_timestamp(), CSysSettingMgr::GetInstance()->Global_Reset_Time)){
					//�۳���Ʒ
					m_pUser->UpdateGameElement(CFriendMgr::GetInstance()->GetFriendGive(), db_req, *res.mutable_eles(), ntf);
					//��÷�����Ʒ
					m_pUser->UpdateGameElement(CFriendMgr::GetInstance()->GetFriendGiveBack(), db_req, *res.mutable_eles(), ntf);
					it->second->set_me_give_timestamp(Now());
					CFriendMgr::GetInstance()->GiveElementToFriend(m_pUser->UserID(), it->first, ntf);
				}
			}
			//db_req
			gs_dbs_user_info_op_req::struct_op_friend* op_friend = db_req.add_op_friends();
			op_friend->set_op_type(gs_dbs_user_info_op_req::struct_op_friend::GIVE_ELEMENTN);
			//c
		}break;
	case c_gs_user_friend_op_req::GIVE_UP_APPLY1:
		{
			//ASSERT(op_req.has_user_id());
			if (!op_req.has_user_id())
			{
				res.set_error_code(e_gsc_errorcode_unknown_error);
				return;
			}
			std::map<UINT32, friend_base_info*>::iterator it = m_mapApply.find(op_req.user_id());
			//ASSERT(it != m_mapApply.end());
			if (it == m_mapApply.end())
			{
				res.set_error_code(e_gsc_errorcode_unknown_error);
				return;
			}

			//�ڴ�ɾ��
			_del_user_apply(op_req.user_id());
			m_mapApply.erase(it);
			//�����һ���������֪ͨ���Ա�Ӵ�ȷ���б���ɾ��
			CFriendMgr::GetInstance()->GiveUpApply(m_pUser->UserID(), op_req.user_id(), ntf);
			//db_req;
			gs_dbs_user_info_op_req::struct_op_friend* op_friend = db_req.add_op_friends();
			op_friend->set_op_type(gs_dbs_user_info_op_req::struct_op_friend::GIVE_UP_APPLY1);
			op_friend->set_friend_id(op_req.user_id());
			//c
			res.set_user_id(op_req.user_id());
		}break;
	case c_gs_user_friend_op_req::GIVE_UP_APPLYN:
		{
			std::map<UINT32, friend_base_info*>::iterator it = m_mapApply.begin();
			for (; it != m_mapApply.end(); ++it)
			{
				CFriendMgr::GetInstance()->GiveUpApply(m_pUser->UserID(), it->first, ntf);
			}
			//�ڴ�ɾ��
			_del_user_apply(0);
			m_mapApply.clear();
			//db_req
			gs_dbs_user_info_op_req::struct_op_friend* op_friend = db_req.add_op_friends();
			op_friend->set_op_type(gs_dbs_user_info_op_req::struct_op_friend::GIVE_UP_APPLYN);
			//c
		}break;
	case c_gs_user_friend_op_req::AFFIRM_AGREE1:
		{
			//�鿴��������
			if (_is_friend_num_reach_max())
			{
				res.set_error_code(e_gsc_errorcode_friend_me_friend_num_reach_max);
				return;
			}

			//ASSERT(op_req.has_user_id());
			if (!op_req.has_user_id())
			{
				res.set_error_code(e_gsc_errorcode_unknown_error);
				return;
			}
			
			std::map<UINT32, friend_base_info*>::iterator it = m_mapConfirms.find(op_req.user_id());
			//ASSERT(it != m_mapConfirms.end());
			if (it == m_mapConfirms.end())
			{
				res.set_error_code(e_gsc_errorcode_unknown_error);
				return;
			}
			
			//�鿴�Է���������(����������޾Ͳ���ͬ��)
			UINT32 iFriendID = op_req.user_id();
			CUser* pUser = CUserMgr::GetInstance()->GetUserByUserID(iFriendID);
			if (pUser)
			{
				UINT32 iCurFriendNum = pUser->GetFriendNum();
				if (CFriendMgr::GetInstance()->IsFriendFull(iCurFriendNum))
				{
					res.set_error_code(e_gsc_errorcode_friend_friend_friend_num_reach_max);
					return;
				}

				//�ڴ����(�Ӵ�ȷ���б������б�)
				db_user_friend::friend_info* fri = m_db_user_friend->add_friends();
				fri->set_friend_give_timestamp(0);
				fri->set_me_give_timestamp(0);
				fri->mutable_base_info()->CopyFrom(*it->second);
				
				m_mapFriends[fri->mutable_base_info()->user_id()] = fri;

				_del_user_confirm(op_req.user_id());
				m_mapConfirms.erase(it);
				//���������ߣ�֪ͨ��������б���ɾ��
				CFriendMgr::GetInstance()->AffirmAgree(m_pUser->UserID(), op_req.user_id(), ntf);
				//db_req
				gs_dbs_user_info_op_req::struct_op_friend* op_friend =  db_req.add_op_friends();
				op_friend->set_op_type(gs_dbs_user_info_op_req::struct_op_friend::AFFIRM_AGREE1);
				op_friend->set_friend_id(op_req.user_id());
				//c
				res.set_user_id(op_req.user_id());
			}
			else{
				gs_dbs_friend_offline_req* req = new gs_dbs_friend_offline_req;
				req->set_op_type(gs_dbs_friend_offline_req::AFFIRM_AGREE1);
				req->set_user_id(m_pUser->UserID());
				req->add_affirm_ids(op_req.user_id());
				CClientForDBS::GetInstance()->SendAsync(CClientForDBSAsync::EASyncOP_User_Friend_Offline, m_pUser->UserID(), req);
				return;
			}
			
		}break;
	case c_gs_user_friend_op_req::AFFIRM_AGREEN:
		{
			//�鿴������Ŀ�Ƿ�ᳬ������
			UINT32 iMaxFriNum = 0;
			bool r = CSysSettingMgr::GetInstance()->GetIntValue("Friend_Max_Num", iMaxFriNum);
			ASSERT(r && iMaxFriNum);
			if(m_mapConfirms.size() + m_mapFriends.size() > iMaxFriNum){
				res.set_error_code(e_gsc_errorcode_friend_me_friend_num_reach_max);
				return;				
			}
			
			if(m_mapConfirms.empty()){
				res.set_error_code(e_gsc_errorcode_unknown_error);
				return;
			}

			//���������ߣ����������б��Ƿ�����
			bool isAllOnline = true;
			std::map<UINT32, friend_base_info*>::iterator it = m_mapConfirms.begin();
			for (; it != m_mapConfirms.end(); ++it)
			{
				CUser* pUser = CUserMgr::GetInstance()->GetUserByUserID(it->first);
				if (pUser)
				{
					if(CFriendMgr::GetInstance()->IsFriendFull(pUser->GetFriendNum())){
						res.set_error_code(e_gsc_errorcode_friend_friend_friend_num_reach_max);
						return;
					}
				}
				else{
					isAllOnline = false;
				}
			}
			
			if (isAllOnline)
			{
				std::map<UINT32, friend_base_info*>::iterator it = m_mapConfirms.begin();
				for (; it != m_mapConfirms.end(); ++it)
				{
					CUser* pUser = CUserMgr::GetInstance()->GetUserByUserID(it->first);
					ASSERT(pUser);
					CFriendMgr::GetInstance()->AffirmAgree(m_pUser->UserID(), it->second->user_id(), ntf);

					db_user_friend::friend_info* fri = m_db_user_friend->add_friends();
					fri->set_friend_give_timestamp(0);
					fri->set_me_give_timestamp(0);
					fri->mutable_base_info()->CopyFrom(*it->second);
					m_mapFriends[it->first] = fri;
				}
				//�ڴ�ɾ��
				_del_user_confirm(0);
				m_mapConfirms.clear();
				//db_req
				gs_dbs_user_info_op_req::struct_op_friend* op_friend =  db_req.add_op_friends();
				op_friend->set_op_type(gs_dbs_user_info_op_req::struct_op_friend::AFFIRM_AGREEN);
				//c
			}
			else{//����ȫ�����߾���gs_dbs_friend_offline_req
				std::map<UINT32, friend_base_info*>::iterator it = m_mapConfirms.begin();
				gs_dbs_friend_offline_req* req = new gs_dbs_friend_offline_req;
				req->set_op_type(gs_dbs_friend_offline_req::AFFIRM_AGREEN);
				req->set_user_id(m_pUser->UserID());
				for (; it != m_mapConfirms.end(); ++it)
				{
					req->add_affirm_ids(it->first);
				}
				CClientForDBS::GetInstance()->SendAsync(CClientForDBSAsync::EASyncOP_User_Friend_Offline, m_pUser->UserID(), req);
				return;
			}
			
		}break;
	case c_gs_user_friend_op_req::AFFIRM_REFUSE1:
		{
			//ASSERT(op_req.has_user_id());
			if (!op_req.has_user_id())
			{
				res.set_error_code(e_gsc_errorcode_unknown_error);
				return;
			}
			std::map<UINT32, friend_base_info*>::iterator it = m_mapConfirms.find(op_req.user_id());
			//ASSERT(it != m_mapConfirms.end());
			if (it == m_mapConfirms.end())
			{
				res.set_error_code(e_gsc_errorcode_unknown_error);
				return;
			}
			//�ڴ����
			_del_user_confirm(op_req.user_id());
			m_mapConfirms.erase(it);

			CFriendMgr::GetInstance()->AffirmRefuse(m_pUser->UserID(), op_req.user_id(), ntf);
			//db_req
			gs_dbs_user_info_op_req::struct_op_friend* op_friend = db_req.add_op_friends();
			op_friend->set_op_type(gs_dbs_user_info_op_req::struct_op_friend::AFFIRM_REFUSE1);
			op_friend->set_friend_id(op_req.user_id());
			//c
			res.set_user_id(op_req.user_id());

		}break;
	case c_gs_user_friend_op_req::AFFIRM_REFUSEN:
		{
			std::map<UINT32, friend_base_info*>::iterator it = m_mapConfirms.begin();
			for (; it != m_mapConfirms.end(); ++it)
			{
				CFriendMgr::GetInstance()->AffirmRefuse(m_pUser->UserID(), it->first, ntf);
			}
			//�ڴ����
			_del_user_confirm(0);
			m_mapConfirms.clear();
			
			//db_req
			gs_dbs_user_info_op_req::struct_op_friend* op_friend = db_req.add_op_friends();
			op_friend->set_op_type(gs_dbs_user_info_op_req::struct_op_friend::AFFIRM_REFUSEN);
			//c
		}break;
	case c_gs_user_friend_op_req::ADD_FRIEND:
		{
			//����������
			if (_is_friend_num_reach_max())
			{
				res.set_error_code(e_gsc_errorcode_friend_me_friend_num_reach_max);
				return;
			}

			//�ͻ����Ƿ�����������id
			//ASSERT(op_req.has_user_id());
			if (!op_req.has_user_id())
			{
				res.set_error_code(e_gsc_errorcode_unknown_error);
				return;
			}
			//�Ƿ�ﵽ�����������
			if(_is_friend_apply_num_reach_max())
			{
				res.set_error_code(e_gsc_errorcode_friend_apply_num_reach_max);
				return;
			}
			
			if (op_req.user_id() == m_pUser->UserID())
			{
				res.set_error_code(e_gsc_errorcode_unknown_error);
				return;
			}
			//�鿴�Ƿ��Ѿ�����ҵ������б���
			std::map<UINT32, friend_base_info*>::iterator it = m_mapApply.find(op_req.user_id());
			if(it != m_mapApply.end())
			{
				res.set_error_code(e_gsc_errorcode_friend_already_in_apply_list);
				return;
			}

			//�鿴�Ƿ��Ѿ���Ϊ��ҵ�����
			std::map<UINT32, db_user_friend::friend_info*>::iterator it_fri = m_mapFriends.find(op_req.user_id());
			//ASSERT(it_fri == m_mapFriends.end());
			if (it_fri != m_mapFriends.end())
			{
				res.set_error_code(e_gsc_errorcode_friend_already_be_friend);
				return;
			}
			
			//�ҵĻ�����Ϣ
			friend_base_info me;
			me.set_user_id(m_pUser->UserID());
			me.set_head_id(m_pUser->GetHeadID());
			me.set_head_frame_id(m_pUser->GetHeadFrameID());
			me.set_name(m_pUser->Name());
			me.set_level(m_pUser->GetLevel());
			me.set_vip(m_pUser->GetVipLevel());
			CArenaContestant* pArena = CArena::GetInstance()->GetArenaContestantByUserID(m_pUser->UserID());
			if(pArena){
				me.set_arena_rank(pArena->m_rank);
			}else{
				me.set_arena_rank(0);
			}
			
			me.set_last_login_time(Now());

			//�Է���ȷ���б��Ƿ񳬹��������
			CUser* pUser = CUserMgr::GetInstance()->GetUserByUserID(op_req.user_id());
			if (pUser){//�ú�������
				UINT32 iAffirm = pUser->GetFriendAffirmNum();
				UINT32 Friend_Confirm_Max_Num = 0;

				bool r = CSysSettingMgr::GetInstance()->GetIntValue("Friend_Confirm_Max_Num", Friend_Confirm_Max_Num);
				ASSERT(r && Friend_Confirm_Max_Num);
				
				ASSERT(iAffirm <= Friend_Confirm_Max_Num);
				if (iAffirm == Friend_Confirm_Max_Num)
				{
					res.set_error_code(e_gsc_errorcode_friend_affirm_num_reach_max_num);
					return;
				}

				//�ڴ����(������Ƽ��б�ֱ�Ӵ��Ƽ��б���ɾ��)
				friend_base_info add_fri; 
				r = _get_add_friend_info(op_req.user_id(), db_req, add_fri);
				ASSERT(r);//���߿϶��ܻ����Ϣ��
				//ASSERT(add_fri);
				if (!r)
				{
					res.set_error_code(e_gsc_errorcode_unknown_error);
					return;
				}
				//���뵽�������б���
				friend_base_info* fri = m_db_user_friend->mutable_applies()->Add();
				fri->CopyFrom(add_fri);
				m_mapApply[fri->user_id()] = fri;
				
				_del_user_recommend(add_fri.user_id());
				//�Է����ߣ���Ҫ֪ͨ����뵽��ȷ���б���
				CFriendMgr::GetInstance()->AddFriend(m_pUser->UserID(), fri->user_id(), ntf, me);

				//db_req
				gs_dbs_user_info_op_req::struct_op_friend* op_friend = db_req.add_op_friends();
				op_friend->set_op_type(gs_dbs_user_info_op_req::struct_op_friend::ADD_FRIEND_GS_ONLINE);
				op_friend->set_friend_id(fri->user_id());
				op_friend->mutable_base_info()->CopyFrom(*fri);
				//c
				res.set_op_type(op_req.op_type());
				res.mutable_base_info()->CopyFrom(*fri);
			}else{//�����ߣ�db�Ǳ��ж�����Է���ȷ���б�����ֱ��ͬ����������˷��ش����뵽GS GS�ٽ�������ؿͻ���
				gs_dbs_friend_offline_req* req = new gs_dbs_friend_offline_req;
				req->set_user_id(m_pUser->UserID());
				friend_base_info info;
				bool r = _get_add_friend_info(op_req.user_id(), db_req, info);
				if(r){
					req->mutable_friend_info()->CopyFrom(info);
				}else{//����������Ϣ����Ӻ����п��ܻ�ȡ���������Ϣ
					req->set_friend_id(op_req.user_id());
				}
				
				_del_user_recommend(op_req.user_id());
				req->mutable_me_info()->CopyFrom(me);
				req->set_op_type(gs_dbs_friend_offline_req::ADD_FRIEND);
				CClientForDBS::GetInstance()->SendAsync(CClientForDBSAsync::EASyncOP_User_Friend_Offline, m_pUser->UserID(), req);
				return;
			}
			
		}break;
	case c_gs_user_friend_op_req::DEL_FRIEND:
		{
			//ASSERT(op_req.has_user_id());
			if (!op_req.has_user_id())
			{
				res.set_error_code(e_gsc_errorcode_unknown_error);
				return;
			}

			UINT32 iDelNum = m_pUser->GetEventTodayValue(EEventType_Del_Friend, EEventSubType_None);//����ɾ��������Ŀ
			UINT32 iDelCfgNum = 0;
			bool r = CSysSettingMgr::GetInstance()->GetIntValue("Friend_Del_Max_Num_Per_Day", iDelCfgNum);
			ASSERT(r && iDelCfgNum);
			if (iDelNum >= iDelCfgNum) 
			{
				res.set_error_code(e_gsc_errorcode_friend_del_friend_num_reach_max);
				return;
			}

			std::map<UINT32, db_user_friend::friend_info*>::iterator it = m_mapFriends.find(op_req.user_id());
			ASSERT(it != m_mapFriends.end());
			if (it == m_mapFriends.end())
			{
				res.set_error_code(e_gsc_errorcode_unknown_error);
				return;
			}
			//�ڴ�ɾ��
			_del_user_friend(op_req.user_id());
			m_mapFriends.erase(it);

			CFriendMgr::GetInstance()->DelFriend(m_pUser->UserID(), op_req.user_id(), ntf);


			//db_req
			m_pUser->UpdateEventInfo(EEventType_Del_Friend, EEventSubType_None, db_req, Now(), 1);
			gs_dbs_user_info_op_req::struct_op_friend* op_friend = db_req.add_op_friends();
			op_friend->set_op_type(gs_dbs_user_info_op_req::struct_op_friend::DEL_FRIEND);
			op_friend->set_friend_id(op_req.user_id());
			//c
			res.set_op_type(op_req.op_type());
			res.set_user_id(op_req.user_id());
		}break;
	default:
		res.set_error_code(e_gsc_errorcode_unknown_error);
		return;
	}
	res.set_error_code(e_gsc_errorcode_success);
}

bool CUserFriendStrategy::FriendGiveElement( UINT32 iFriendID, GS_C_NTF& ntf)
{
	std::map<UINT32, db_user_friend::friend_info*>::iterator it = m_mapFriends.find(iFriendID);
	ASSERT(it != m_mapFriends.end());
	if (it == m_mapFriends.end())
	{
		return false;
	}
	it->second->set_friend_give_timestamp(Now());
	gs_c_friend_ntf* friend_ntf = ntf.mutable_friend_ntf();
	friend_ntf->set_op_type(gs_c_friend_ntf::FRIEND_GIVE_ELEMENT);
	friend_ntf->set_friend_id(iFriendID);
	return true;
}

bool CUserFriendStrategy::FriendGiveUpApply( UINT32 iFriendID, GS_C_NTF& ntf )
{
	//��Ҫ�Ӵ�ȷ���б���ɾ��
	std::map<UINT32, friend_base_info*>::iterator it = m_mapConfirms.find(iFriendID);
	ASSERT(it != m_mapConfirms.end());
	if (it == m_mapConfirms.end())
	{
		return false;
	}
	_del_user_confirm(iFriendID);
	m_mapConfirms.erase(it);
	gs_c_friend_ntf* friend_ntf = ntf.mutable_friend_ntf();
	friend_ntf->set_op_type(gs_c_friend_ntf::FRIEND_GIVE_UP_APPLY);
	friend_ntf->set_friend_id(iFriendID);
	return true;
}

bool CUserFriendStrategy::FriendAffirmAgree( UINT32 iFriendID, GS_C_NTF& ntf )
{
	ASSERT(!_is_friend_num_reach_max());
	if (_is_friend_num_reach_max())
	{
		return false;
	}

	std::map<UINT32, friend_base_info*>::iterator it = m_mapApply.find(iFriendID);
	ASSERT(it != m_mapApply.end());
	if (it == m_mapApply.end())
	{
		return false;
	}
	
	//���뵽�����б���
	db_user_friend::friend_info* fri = m_db_user_friend->add_friends();
	fri->mutable_base_info()->CopyFrom(*it->second);
	fri->set_friend_give_timestamp(0);
	fri->set_me_give_timestamp(0);
	m_mapFriends[fri->mutable_base_info()->user_id()] = fri;

	//�������б���ɾ��
	_del_user_apply(iFriendID);
	m_mapApply.erase(it);
	
	gs_c_friend_ntf* friend_ntf = ntf.mutable_friend_ntf();
	friend_ntf->set_op_type(gs_c_friend_ntf::FRIEND_AFFIRM_AGREE);
	friend_ntf->set_friend_id(iFriendID);
	return true;
}

bool CUserFriendStrategy::FriendAffirmRefuse( UINT32 iFriendID, GS_C_NTF& ntf )
{
	std::map<UINT32, friend_base_info*>::iterator it = m_mapApply.find(iFriendID);
	ASSERT(it != m_mapApply.end());
	if (it == m_mapApply.end())
	{
		return false;
	}
	_del_user_apply(iFriendID);
	m_mapApply.erase(it);

	gs_c_friend_ntf* friend_ntf = ntf.mutable_friend_ntf();
	friend_ntf->set_op_type(gs_c_friend_ntf::FRIEND_AFFIRM_REFUSE);
	friend_ntf->set_friend_id(iFriendID);
	return true;
}


void CUserFriendStrategy::FriendAddFriend( UINT32 iFriendID, GS_C_NTF& ntf, const friend_base_info& base_info )
{
	//���뵽��ȷ���б���
	friend_base_info* confirm = m_db_user_friend->add_confirms();
	confirm->CopyFrom(base_info);
	m_mapConfirms[confirm->user_id()] = confirm;

	gs_c_friend_ntf* friend_ntf = ntf.mutable_friend_ntf();
	friend_ntf->set_op_type(gs_c_friend_ntf::FRIEND_ADD_FRIEND);
	friend_ntf->mutable_base_info()->CopyFrom(base_info);

}

bool CUserFriendStrategy::FriendDelFriend(UINT32 iFriendID, GS_C_NTF& ntf)
{
	std::map<UINT32, db_user_friend::friend_info*>::iterator it = m_mapFriends.find(iFriendID);
	ASSERT(it != m_mapFriends.end());
	if(it == m_mapFriends.end())
	{
		return false;
	}
	_del_user_friend(iFriendID);
	m_mapFriends.erase(it);

	gs_c_friend_ntf* friend_ntf = ntf.mutable_friend_ntf();
	friend_ntf->set_op_type(gs_c_friend_ntf::FRIEND_DEL_FRIEND);
	friend_ntf->set_friend_id(iFriendID);
	return true;
}




UINT32 CUserFriendStrategy::GetFriendAffirmNum()
{
	return m_mapConfirms.size();
}


void CUserFriendStrategy::_del_user_apply( UINT32 iUserID /*= 0*/ )
{
	if (iUserID)
	{
		for(int i = 0; i < m_db_user_friend->applies_size(); ++i){
			if (m_db_user_friend->mutable_applies(i)->user_id() == iUserID)
			{
				m_db_user_friend->mutable_applies()->SwapElements(i, m_db_user_friend->applies_size() - 1);
				m_db_user_friend->mutable_applies()->RemoveLast();
				break;
			}
		}
	}
	else
	{
		m_db_user_friend->clear_applies();//�÷��Ƿ���ȷ
	}

}

void CUserFriendStrategy::_del_user_confirm( UINT32 iUserID /*= 0*/ )
{
	if (iUserID)
	{
		for(int i = 0; i < m_db_user_friend->confirms_size(); ++i){
			if (m_db_user_friend->mutable_confirms(i)->user_id() == iUserID)
			{
				m_db_user_friend->mutable_confirms()->SwapElements(i, m_db_user_friend->confirms_size() - 1);
				m_db_user_friend->mutable_confirms()->RemoveLast();
				break;
			}
		}
	}
	else{
		m_db_user_friend->clear_confirms();//�÷���֪����ȷ		
	}
}

void CUserFriendStrategy::_del_user_recommend(UINT32 iUserID){
	std::map<UINT32, friend_base_info*>::iterator it = m_mapRecommends.find(iUserID);
	if (it != m_mapRecommends.end())
	{
		for(int i = 0; i < m_db_user_friend->recommends_size(); ++i){
			if (m_db_user_friend->mutable_recommends(i)->user_id() == iUserID)
			{
				m_db_user_friend->mutable_recommends()->SwapElements(i, m_db_user_friend->recommends_size() - 1);
				m_db_user_friend->mutable_recommends()->RemoveLast();
				break;
			}
		}
		m_mapRecommends.erase(it);
	}
}

bool CUserFriendStrategy::_is_friend_num_reach_max()
{
	UINT32 iMaxFriNum = 0;
	bool r = CSysSettingMgr::GetInstance()->GetIntValue("Friend_Max_Num", iMaxFriNum);
	ASSERT(r && iMaxFriNum);
	ASSERT(m_mapFriends.size() <= iMaxFriNum);
	return  m_mapFriends.size() == iMaxFriNum;
}

bool CUserFriendStrategy::_is_friend_apply_num_reach_max()
{
	UINT32 iMaxFriApplyNum = 0;
	bool r = CSysSettingMgr::GetInstance()->GetIntValue("Friend_Invite_Max_Num", iMaxFriApplyNum);
	ASSERT(r && iMaxFriApplyNum);
	ASSERT(m_mapApply.size() <= iMaxFriApplyNum);
	return m_mapApply.size() == iMaxFriApplyNum;
}

bool CUserFriendStrategy::_get_add_friend_info(UINT32 iUserID, gs_dbs_user_info_op_req& db_req, friend_base_info& base_info)
{
	//���Ȳ鿴�Ƿ����Ƽ��б���
	std::map<UINT32, friend_base_info*>::iterator it = m_mapRecommends.find(iUserID);
	if (it != m_mapRecommends.end())
	{
		gs_dbs_user_info_op_req::struct_op_friend* op_friend = db_req.add_op_friends();
		op_friend->set_op_type(gs_dbs_user_info_op_req::struct_op_friend::DEL_RECOMMEND_FRIEND);
		op_friend->set_friend_id(it->first);
		base_info = *it->second;
		return true;
	}

	if (m_temp_search_info && m_temp_search_info->user_id() == iUserID)
	{
		base_info = *m_temp_search_info;
		return true;
	}
	
	CUser* pUser = CUserMgr::GetInstance()->GetUserByUserID(iUserID);
	if (pUser)
	{
		base_info = pUser->GetBaseInfo();
		return true;
	}
	return false;
}

void CUserFriendStrategy::_del_user_friend(UINT32 iUserID)
{
	for(int i = 0; i < m_db_user_friend->friends_size(); ++i){
		if (m_db_user_friend->mutable_friends(i)->mutable_base_info()->user_id() == iUserID)
		{
			m_db_user_friend->mutable_friends()->SwapElements(i, m_db_user_friend->friends_size() - 1);
			m_db_user_friend->mutable_friends()->RemoveLast();
			break;
		}
	}
}

void CUserFriendStrategy::AddFriend( const friend_base_info& rbase_info )
{
	friend_base_info* base_info = m_db_user_friend->add_applies();
	base_info->CopyFrom(rbase_info);
	m_mapApply[base_info->user_id()] = base_info;
}

void CUserFriendStrategy::GetFriendInfo(user_friend* c_friend)
{
	::google::protobuf::RepeatedPtrField< ::db_user_friend_friend_info >::iterator it = m_db_user_friend->mutable_friends()->begin();		
	for (; it != m_db_user_friend->mutable_friends()->end(); ++it)
	{
		user_friend::friend_info* fri = c_friend->add_friends();
		if(isSameDay(Now(), it->friend_give_timestamp(), CSysSettingMgr::GetInstance()->Global_Reset_Time))
			fri->set_is_friend_give(true);
		else{
			fri->set_is_friend_give(false);
		}

		if(isSameDay(Now(), it->me_give_timestamp(), CSysSettingMgr::GetInstance()->Global_Reset_Time))
			fri->set_is_me_give(true);
		else{
			fri->set_is_me_give(false);
		}
		fri->mutable_base_info()->CopyFrom(it->base_info());
	}
	
	//�Ƿ������ж�
	::google::protobuf::RepeatedPtrField< ::friend_base_info >::const_iterator it_1 = m_db_user_friend->applies().begin();
	for (; it_1 != m_db_user_friend->applies().end(); ++it_1)
	{
		friend_base_info* base_info = c_friend->add_applies();
		base_info->CopyFrom(*it_1);
		UINT32 iUserID = it_1->user_id();
		if(CUserMgr::GetInstance()->GetUserByUserID(base_info->user_id())){
			base_info->set_last_login_time(0);
		}
	}

	::google::protobuf::RepeatedPtrField< ::friend_base_info >::const_iterator it_2 = m_db_user_friend->confirms().begin();
	for (; it_2 != m_db_user_friend->confirms().end(); ++it_2)
	{
		friend_base_info* base_info = c_friend->add_confirms();
		base_info->CopyFrom(*it_2);
		UINT32 iUserID = it_2->user_id();
		if(CUserMgr::GetInstance()->GetUserByUserID(base_info->user_id())){
			base_info->set_last_login_time(0);
		}
	}
	
	//����Ƽ��б���GS��map�����ȡ5���û�
	UINT32 iNeedNum = 5 - m_db_user_friend->recommends_size();
	if (iNeedNum > 0)
	{
		std::map<UINT32, CUser*>* pMapAllUsers = CUserMgr::GetInstance()->GetOnlineUsers();
		ASSERT(pMapAllUsers);

		if (pMapAllUsers->size())
		{
			for (std::map<UINT32, CUser*>::iterator it = pMapAllUsers->begin(); it != pMapAllUsers->end(); ++it)
			{
				if(_is_commend_satisfy(it->first)){
					iNeedNum--;
					friend_base_info* base_info = m_db_user_friend->add_recommends();
					CUser* pUser = it->second;
					ASSERT(pUser);
					*base_info = pUser->GetBaseInfo();
					m_mapRecommends[base_info->user_id()] = base_info;
					if (0 == iNeedNum)
					{
						break;
					}
				}
			}
		}
	}

	::google::protobuf::RepeatedPtrField< ::friend_base_info >::const_iterator it_3 = m_db_user_friend->recommends().begin();
	for (; it_3 != m_db_user_friend->recommends().end(); ++it_3)
	{
		friend_base_info* base_info = c_friend->add_recommends();
		base_info->CopyFrom(*it_3);
		UINT32 iUserID = it_3->user_id();
		if(CUserMgr::GetInstance()->GetUserByUserID(base_info->user_id())){
			base_info->set_last_login_time(0);
		}
	}
}

void CUserFriendStrategy::SearchUserRet( const friend_base_info& base_info )
{
	if(!m_temp_search_info)
		m_temp_search_info = new friend_base_info;
	else{
		m_temp_search_info->Clear();
	}
	
	m_temp_search_info->CopyFrom(base_info);
}

UINT32 CUserFriendStrategy::GetFriendNum()
{
	return m_mapFriends.size();
}

void CUserFriendStrategy::AffirmFriend( const ::google::protobuf::RepeatedField< ::google::protobuf::uint32 >& friend_ids )
{
	::google::protobuf::RepeatedField< ::google::protobuf::uint32 >::const_iterator iter = friend_ids.begin();
	for (; iter != friend_ids.end(); ++iter)
	{
		std::map<UINT32, friend_base_info*>::iterator it = m_mapConfirms.find(*iter);
		ASSERT(it != m_mapConfirms.end());
		//������뵽�����б�
		db_user_friend::friend_info* fri = m_db_user_friend->add_friends();
		fri->set_friend_give_timestamp(0);
		fri->set_me_give_timestamp(0);
		fri->mutable_base_info()->CopyFrom(*it->second);
		m_mapFriends[it->first] = fri;

		CUser* pUser = CUserMgr::GetInstance()->GetUserByUserID(*iter);
		if (pUser)
		{
			GS_C_NTF ntf;
			pUser->FriendAffirmAgree(m_pUser->UserID(), ntf);
			pUser->SendNtf(ntf);
		}

		_del_user_confirm(it->first);
		m_mapConfirms.erase(it);	
	}
}

bool CUserFriendStrategy::_is_commend_satisfy(UINT32 iUserID)
{
	if (iUserID == m_pUser->UserID())//��ұ���
	{
		return false;
	}

	//����Һ��� || �������б��� || �ڴ�ȷ���б��� || ���Ƽ��б���
	if(m_mapFriends.find(iUserID) != m_mapFriends.end() ||
		m_mapApply.find(iUserID) != m_mapApply.end() ||
		m_mapConfirms.find(iUserID) != m_mapConfirms.end() ||
		m_mapRecommends.find(iUserID) != m_mapRecommends.end()){
			return false;
	}
	return true;
}
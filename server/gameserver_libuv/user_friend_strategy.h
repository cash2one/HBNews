#pragma once
#ifndef user_friend_strategy_h__
#define user_friend_strategy_h__
#include <map>
#include <vector>
#include "dbs_gs_proto.pb.h"
#include "def.h"
#include "config.h"
class CUser;
class IUserFriendStrategy{
public:
	virtual ~IUserFriendStrategy() {};
	virtual bool Init(CUser* pUser, db_user_friend* fri) = 0;
	virtual void UserFriendOP(const c_gs_user_friend_op_req& op_req, gs_c_user_friend_op_res& res, gs_dbs_user_info_op_req& db_req, GS_C_NTF& ntf) = 0;
	virtual bool FriendGiveElement(UINT32 iFriendID, GS_C_NTF& ntf) = 0;
	virtual bool FriendGiveUpApply(UINT32 iFriendID, GS_C_NTF& ntf) = 0;
	virtual bool FriendAffirmAgree(UINT32 iFriendID, GS_C_NTF& ntf) = 0;
	virtual bool FriendAffirmRefuse(UINT32 iFriendID, GS_C_NTF& ntf) = 0;
	virtual bool FriendDelFriend(UINT32 iFriendID, GS_C_NTF& ntf) = 0;
	virtual void FriendAddFriend(UINT32 iFriendID, GS_C_NTF& ntf, const friend_base_info& base_info) = 0;
	virtual UINT32 GetFriendAffirmNum() = 0;
	virtual void AddFriend(const friend_base_info& rbase_info) = 0;
	virtual void GetFriendInfo(user_friend* c_friend) = 0;
	virtual void SearchUserRet(const friend_base_info& base_info) = 0;
	virtual UINT32 GetFriendNum() = 0;
	virtual void AffirmFriend(const ::google::protobuf::RepeatedField< ::google::protobuf::uint32 >& friend_ids) = 0;
};

class CUserFriendStrategy : public IUserFriendStrategy
{
public:
	~CUserFriendStrategy(){
		SAFE_DEL(m_temp_search_info);
	}
	CUserFriendStrategy(void):m_temp_search_info(NULL){};
	virtual bool Init(CUser* pUser, db_user_friend* fri);
	virtual void UserFriendOP(const c_gs_user_friend_op_req& op_req, gs_c_user_friend_op_res& res, gs_dbs_user_info_op_req& db_req, GS_C_NTF& ntf);

	virtual bool FriendGiveElement(UINT32 iFriendID, GS_C_NTF& ntf);
	virtual bool FriendGiveUpApply(UINT32 iFriendID, GS_C_NTF& ntf);
	virtual bool FriendAffirmAgree(UINT32 iFriendID, GS_C_NTF& ntf);
	virtual bool FriendAffirmRefuse(UINT32 iFriendID, GS_C_NTF& ntf);
	virtual bool FriendDelFriend(UINT32 iFriendID, GS_C_NTF& ntf);
	virtual UINT32 GetFriendAffirmNum();
	virtual void FriendAddFriend(UINT32 iFriendID, GS_C_NTF& ntf, const friend_base_info& base_info);
	virtual void AddFriend(const friend_base_info& rbase_info);
	virtual void GetFriendInfo(user_friend* c_friend);
	virtual void SearchUserRet(const friend_base_info& base_info);
	virtual UINT32 GetFriendNum();
	virtual void AffirmFriend(const ::google::protobuf::RepeatedField< ::google::protobuf::uint32 >& friend_ids);

private:
	std::map<UINT32, db_user_friend::friend_info*> m_mapFriends;			//好友列表
	std::map<UINT32, friend_base_info*> m_mapApply;			//申请列表
	std::map<UINT32, friend_base_info*> m_mapConfirms;		//确认列表
	std::map<UINT32, friend_base_info*> m_mapRecommends;	//推荐列表

	

	CUser*						m_pUser;
	db_user_friend*				m_db_user_friend;
	
	friend_base_info* m_temp_search_info;	//搜索玩家保存的临时信息
	void _del_user_apply(UINT32 iUserID = 0);//如果iUserID = 0表示全部删除
	void _del_user_confirm(UINT32 iUserID = 0);
	void _del_user_friend(UINT32 iUserID);
	void _del_user_recommend(UINT32 iUserID);
	bool _is_friend_num_reach_max();
	bool _is_friend_apply_num_reach_max();
	bool _get_add_friend_info(UINT32 iUserID, gs_dbs_user_info_op_req& db_req, friend_base_info& base_info);
	bool _is_commend_satisfy(UINT32 iUserID);
};

#endif
#pragma once
#include "dbs_gs_proto.pb.h"
#include "gs_c_proto.pb.h"
#include "config.h"
#ifndef friend_mgr_h__
#define friend_mgr_h__
class CFriendMgr
{
public:
	bool Init();
	static CFriendMgr* GetInstance()
	{
		static CFriendMgr singleton;
		return &singleton;
	}

	const stGameElement& GetFriendGive(){return m_friend_give[0];}
	const stGameElement& GetFriendGet(){return m_friend_get[0];}
	const stGameElement& GetFriendGiveBack(){return m_friend_give_back[0];}

	//给物品给好友 对方要通知
	void GiveElementToFriend(UINT32 meID, UINT32 iFriendID, GS_C_NTF& ntf);

	//取消申请   对方待确认列表中删除
	void GiveUpApply(UINT32 meID, UINT32 iFriendID, GS_C_NTF& ntf);

	//申请得到确认 对方申请列表中删除
	void AffirmAgree(UINT32 meID, UINT32 iFriendID, GS_C_NTF& ntf);

	//申请被拒绝 对方从申请列表中删除
	void AffirmRefuse(UINT32 meID, UINT32 iFriendID, GS_C_NTF& ntf);

	//删除好友 对方也从好友列表中删除
	void DelFriend(UINT32 meID, UINT32 iFriendID, GS_C_NTF& ntf);

	//添加好友 对方加入到其待确认列表中
	void AddFriend(UINT32 meID, UINT32 iFriendID, GS_C_NTF& ntf, const friend_base_info& base_info);

	bool IsFriendFull(UINT32 inFriendNum);
private:
	CFriendMgr(void){};

	std::vector<stGameElement> m_friend_give;								//赠送物品
	std::vector<stGameElement> m_friend_get;								//得到物品
	std::vector<stGameElement> m_friend_give_back;							//回赠物品
	
};
#endif
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

	//����Ʒ������ �Է�Ҫ֪ͨ
	void GiveElementToFriend(UINT32 meID, UINT32 iFriendID, GS_C_NTF& ntf);

	//ȡ������   �Է���ȷ���б���ɾ��
	void GiveUpApply(UINT32 meID, UINT32 iFriendID, GS_C_NTF& ntf);

	//����õ�ȷ�� �Է������б���ɾ��
	void AffirmAgree(UINT32 meID, UINT32 iFriendID, GS_C_NTF& ntf);

	//���뱻�ܾ� �Է��������б���ɾ��
	void AffirmRefuse(UINT32 meID, UINT32 iFriendID, GS_C_NTF& ntf);

	//ɾ������ �Է�Ҳ�Ӻ����б���ɾ��
	void DelFriend(UINT32 meID, UINT32 iFriendID, GS_C_NTF& ntf);

	//��Ӻ��� �Է����뵽���ȷ���б���
	void AddFriend(UINT32 meID, UINT32 iFriendID, GS_C_NTF& ntf, const friend_base_info& base_info);

	bool IsFriendFull(UINT32 inFriendNum);
private:
	CFriendMgr(void){};

	std::vector<stGameElement> m_friend_give;								//������Ʒ
	std::vector<stGameElement> m_friend_get;								//�õ���Ʒ
	std::vector<stGameElement> m_friend_give_back;							//������Ʒ
	
};
#endif
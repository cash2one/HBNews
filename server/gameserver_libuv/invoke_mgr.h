#ifndef invoke_mgr_h__
#define invoke_mgr_h__

#pragma once

#include <vector>
#include <map>
#include "def.h"
#include "config.h"
#include "condition.h"

enum EInvokeType{
	/*
	EInvokeType_MIN = 0,
	EInvokeType_GOLD_FREE = 1,		// ��ѽ�ҵ��γ�ȡ
	EInvokeType_GOLD_1 = 2,			//��ҵ��γ�ȡ��������Ľ�ң�
	EInvokeType_GOLD_FIRST_N = 3,	//���ʮ�����״γ�ȡ
	EInvokeType_GOLD_N = 4,			//���ʮ������ͨ��ȡ
	EInvokeType_DIAMOND_FREE = 5,	//��ͨ�����ʯ��ȡ
	EInvokeType_DIAMOND_1 = 6,		//��ͨ��ʯ��ȡ�������������ʯ��
	EInvokeType_DIAMOND_FIRST_N = 7,	//��ʯʮ�����״γ�ȡ
	EInvokeType_DIAMOND_N = 8,		//��ʯʮ������ͨ��ȡ
	
	EInvokeType_MAX = 9
	//9 ������������صĳ�ȡ�����ܻ���2������ʱδ����;10 �������л�ϻϵͳ����ϻϵͳ����س�ȡ����ϻϵͳ�ĳ�ȡ������Ҫһ�����������������������ʱ;
	*/

	EInvokeType_MIN = 0,
	EInvokeType_GOLD_FREE = 1,		// ��ѽ�ҵ��γ�ȡ
	EInvokeType_GOLD_1 = 2,			//��ҵ��γ�ȡ��������Ľ�ң�
	EInvokeType_GOLD_N = 3,			//���ʮ����ȡ
	EInvokeType_DIAMOND_FREE = 4,	//��ͨ�����ʯ��ȡ
	EInvokeType_DIAMOND_1 = 5,		//��ͨ��ʯ��ȡ�������������ʯ��
	EInvokeType_DIAMOND_N = 6,		//��ʯʮ����ȡ
	EInvokeType_VIP_DIAMOND_1 = 7,	//VIP��ʯ�ٻ�
	EInvokeType_VIP_DIAMOND_N = 8,	//VIP��ʯ�ٻ�
	//7 ������������صĳ�ȡ�����ܻ���2������ʱδ����;
	//8 �������л�ϻϵͳ����ϻϵͳ����س�ȡ����ϻϵͳ�ĳ�ȡ������Ҫһ�����������������������ʱ;
	EInvokeType_FRIEND_POINT = 9,	//������ȡ
	EInvokeType_MAX = 10
};

struct stInvokeCfg{
	UINT32 iPacketID;		//��ͨ�����ID
	UINT32 iFirstPacketID;	//����ʹ��ʱ�İ�ID
	UINT32 iActivityPacketID;		//������ID
	ICondition* ActivityConditions[MAXNUM_CONDITION];	//�ʱ������

	//bool IsConditionsSatisfy(UINT32 iUserID){
	//	return _IsConditionsSatisfy(ActivityConditions, iUserID);
	//}
};

class CInvokeMgr{

public:
	//todo ��������
	inline static CInvokeMgr* GetInstance(){
		static CInvokeMgr singleton;
		return &singleton;
	}

	bool Init();

	bool UnInit();

	//invoke_numֻ����������ʱ�������
	bool Invoke(EInvokeType invoke_type, UINT32 invoke_num, std::vector<stGameElement>& output, std::vector<stGameElement>& consume, bool isFirstTime = false);

private:
	stInvokeCfg* m_invokeType2Cfg[EInvokeType_MAX];
};

#endif // invoke_mgr_h__

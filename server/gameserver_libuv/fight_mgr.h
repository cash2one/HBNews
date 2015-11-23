#ifndef fight_mgr_h__
#define fight_mgr_h__

#pragma once;

#include <map>
#include <vector>
#include "config.h"
#include "def.h"

struct stBattleElement
{
	EELEMENTTYPE	eElementType;	//�����������͡� ����С�� boss  Ӣ��  ��������
	UINT32  iElementSubType;	//������ ��ѡ
	UINT32	iElementID;		//��������ID
	UINT8	iElementPos;	//��������λ��
};
//ս������
struct stBattle{
	UINT32 iBattleID;
	/*
	vector<UINT32>	m_vecSceneObjectType;	//�����������͡� ����С�� boss  Ӣ��  ��������
	vector<UINT32>	m_vecSceneObjectID;		//��������ID
	vector<UINT8>	m_vecSceneObjectPos;	//��������λ��
	*/

	//����˳������С��С
	//EELEMENTTYPE	m_vecSceneObjectType[MAXNUM_TASKBATTLE];	//�����������͡� ����С�� boss  Ӣ��  ��������
	//UINT8	m_vecSceneObjectPos[MAXNUM_TASKBATTLE];	//��������λ��
	//UINT32	m_vecSceneObjectID[MAXNUM_TASKBATTLE];		//��������ID

	
	//����������Ҫ��
	//stBattleElement elements[MAXNUM_TASKBATTLEPOS];//һ��ս���еĹ��boss�ȵĲ���
};

//struct stBattleControl{
//	UINT32 iBattleID;		//ս��ID
//	//UINT32 iDifficulty;		//�Ѷ�ϵ�� ����������Ҫ
//	UINT32 iTimeLimit;		//ʱ������  ��Ϊ��λ
//};

class CFightMgr{
public:
	//todo ��������
	inline static CFightMgr* GetInstance(){
		static CFightMgr singleton;
		return &singleton;
	}

	bool Init();

	bool UnInit();

	stBattle* GetBattleByID(UINT32 iBattleID);

	bool AddBattle(stBattle* pBattle);

private:

	CFightMgr(){}

	std::map<UINT32, stBattle*> m_mapBattleID2Battle;
};

#endif // fight_mgr_h__
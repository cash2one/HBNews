#ifndef __HERO_MGR_H__
#define __HERO_MGR_H__

#include <map>

//��������mapǰ�� �ᵼ��һ���﷨���� Ϊʲô��
#include "config.h"
#include "def.h"



struct CSkill{
	UINT32 iSkillID;
};

struct CHero{
	//���ֺ������ȵ� ����������Ҫ
	UINT32	iHeroID;	//Ӣ��ID
	EArea	eArea;		//����
	UINT8	iStar;		//�Ǽ�
	EOccupation	eOccupation;	//ְҵ	
	ENatureProperty eNatureProperty;	//��Ȼ����
	UINT32	arrProperty[EProperty_MAX];	//Ӣ������
	CSkill	arrSkill[MAXNUM_HEROSKILL];	//Ӣ�ۼ����б�
};


class CHeroMgr{

public:
	//todo ��������
	inline static CHeroMgr* GetInstance(){
		static CHeroMgr singleton;
		return &singleton;
	}

	bool Init(){

	}

	bool UnInit();

	CHero* GetHeroByHeroID(UINT32 iHeroID);

private:
	std::map<UINT32, CHero*> m_mapHeroID2Hero;
};

#endif


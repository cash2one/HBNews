#ifndef __HERO_MGR_H__
#define __HERO_MGR_H__

#include <string>
#include <map>

//��������mapǰ�� �ᵼ��һ���﷨���� Ϊʲô��
#include "config.h"
#include "def.h"

#include "dbs_gs_proto.pb.h"


//struct stSkill{
//	UINT32 iSkillID;
//};
//
//struct stEquip{
//	UINT32 iEquipID;
//};

struct CHeroCfg{
	//���ֺ������ȵ� ����������Ҫ
	UINT32	iHeroID;	//Ӣ��ID
	std::string	strName;	//����
	EArea	eArea;		//����
	//ENatureProperty eNatureProperty;	//��Ȼ����
	EOccupation	eOccupation;	//ְҵ	
	UINT8	iStar;		//�Ǽ�
	EQUALITY	eQuality;	//Ʒ��	
	
	//UINT32	arrProperty[EProperty_MAX];	//Ӣ������	//����������Ҫ

	//��ɱ�Ͱ��� д���ڿͻ��� ��ͨ������������ �������ñȽ���� û��һ����׼id
	//��������������Ҫ�����������ã� ��Щ���Ǵ��Ŀͻ���  ������Ҫ��ҵ� �������ܵĵȼ���Ϣ��
	UINT32	arrPassiveSkill[MAXNUM_HEROPASSIVESKILL];	//Ӣ�۱�������ID�б�
	//UINT32	iPassiveSkill1;
	//UINT32	iPassiveSkill2;
	//UINT32	iPassiveSkill3;

	UINT32 arrEquip[EEQUIPTYPE_MAX];	//Ӣ��װ���б�
};

struct stHeroLevelNeed{
	UINT32 iExpForNextLevel;//��������һ������Ҫ�ľ���ֵ
};
struct stHeroStarCfg{
	UINT32 iFragmentNum;//���ǵ��˵ȼ�����Ҫ����Ƭ����  ���޵�1 �����ٻ����ģ�
	UINT32 iToFragmentNum;//�������һ��Ӣ��ʱ���ٻ�ȡ��ͬ��Ӣ����ת��Ϊ����Ƭ����
};

struct stHeroQualityCfg{
	UINT32 iNeedLevel;	// ����һ��Ʒ��������Ʒ����Ҫ��Ӣ�۵ȼ�
	UINT32 iGold;		//����һ��Ʒ��������Ʒ����Ҫ�Ľ��
	UINT8 iPassiveSkillNum;	//��Ʒ���µ�Ӣ��ӵ�еı������ܸ��� ��Ʒ�ʵ�Ӣ�ۣ����ܿ������� ����ʱ ���ձ�������˳�򿪷ţ� ���翪��2������ �򿪷ŵ�ʽ��������1 ��������2
};

class CUser;
class db_user_hero;
class user_hero;

class CHeroMgr{

public:
	//todo ��������
	inline static CHeroMgr* GetInstance(){
		static CHeroMgr singleton;
		return &singleton;
	}

	bool Init();

	bool UnInit();

	CHeroCfg* GetHeroByHeroID(UINT32 iHeroID);
	stHeroLevelNeed* GetNextLevelNeed(UINT32 iCurLevel);
	stHeroStarCfg* GetStarNeed(UINT32 iToStar);
	stHeroQualityCfg* GetQualityCfg(EQUALITY eToQuality);

	//Ӣ�۵ȼ����޵�����ҵȼ�����
	UINT32 GetHeroMaxLevel();

	UINT32 GetHeroMaxQuality();

	UINT32 GetHeroMaxStar();

	//û������ ��ΪӢ��װ���Ƕ�̬�� ����ֻ�ܻ�ȡ��ʼװ��
	//UINT32 GetHeroEquipID(UINT32 iHeroID, EEQUIPTYPE eEquipType);
	bool UpdateHeroSkillByQuality(CUser* pUser, UINT32 hero_id, EQUALITY quality_from, EQUALITY quality_to);

	//������Ӣ����Ϣ
	bool NewHero(UINT32 hero_id, db_user_hero* db_hero_new, user_hero* c_hero_new );

	//�ж�Ӣ���Ƿ�ȼ��Ѿ����ˣ�
	bool IsLevelFull(UINT32 iCurLevel, UINT32 iLevelLimit);
	//�ж�Ӣ���Ƿ���ֵ����
	bool IsExpFull(UINT32 iCurLevel, UINT32 iCurExp, UINT32 iLevelLimit);

	gs_dbs_user_info_op_req::struct_op_hero_base* GetHeroInDBReq(::google::protobuf::RepeatedPtrField< ::gs_dbs_user_info_op_req::struct_op_hero_base >& db_heros, UINT32 iHeroID, bool create_if_not_exist = true);
private:

	CHeroMgr(){}

	std::map<UINT32, CHeroCfg*> m_mapHeroID2Hero;		//hero id -> hero
	std::map<UINT32, stHeroLevelNeed*> m_mapLevel2NextLevelNeedExp;	//level -> next level exp
	std::map<UINT32, stHeroStarCfg*> m_mapStar2FragmentNeedNum;	//star -> need fragment num
	stHeroQualityCfg m_arrQualityNeed[EQUALITY_MAX];
};

#endif


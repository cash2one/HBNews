#ifndef __HERO_MGR_H__
#define __HERO_MGR_H__

#include <string>
#include <map>

//如果这个在map前面 会导致一堆语法错误 为什么？
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
	//名字和描述等等 服务器不需要
	UINT32	iHeroID;	//英雄ID
	std::string	strName;	//名字
	EArea	eArea;		//国家
	//ENatureProperty eNatureProperty;	//自然属性
	EOccupation	eOccupation;	//职业	
	UINT8	iStar;		//星级
	EQUALITY	eQuality;	//品质	
	
	//UINT32	arrProperty[EProperty_MAX];	//英雄属性	//服务器不需要

	//必杀和奥义 写死在客户端 不通过服务器配置 而且配置比较奇怪 没有一个标准id
	//！！！服务器不要主动技能配置！ 这些都是纯的客户端  但是需要玩家的 主动技能的等级信息！
	UINT32	arrPassiveSkill[MAXNUM_HEROPASSIVESKILL];	//英雄被动技能ID列表
	//UINT32	iPassiveSkill1;
	//UINT32	iPassiveSkill2;
	//UINT32	iPassiveSkill3;

	UINT32 arrEquip[EEQUIPTYPE_MAX];	//英雄装备列表
};

struct stHeroLevelNeed{
	UINT32 iExpForNextLevel;//升级到下一集所需要的经验值
};
struct stHeroStarCfg{
	UINT32 iFragmentNum;//升星到此等级所需要的碎片数量  从无到1 就是召唤消耗！
	UINT32 iToFragmentNum;//玩家已有一样英雄时，再获取到同样英雄则转化为的碎片个数
};

struct stHeroQualityCfg{
	UINT32 iNeedLevel;	// 从上一个品质升到该品质需要的英雄等级
	UINT32 iGold;		//从上一个品质升到该品质需要的金币
	UINT8 iPassiveSkillNum;	//此品质下的英雄拥有的被动技能个数 此品质的英雄，技能开的数量 开放时 按照被动技能顺序开放， 比如开放2个技能 则开放的式被动技能1 被动技能2
};

class CUser;
class db_user_hero;
class user_hero;

class CHeroMgr{

public:
	//todo 单例工具
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

	//英雄等级上限等于玩家等级上限
	UINT32 GetHeroMaxLevel();

	UINT32 GetHeroMaxQuality();

	UINT32 GetHeroMaxStar();

	//没有意义 因为英雄装备是动态的 这里只能获取初始装备
	//UINT32 GetHeroEquipID(UINT32 iHeroID, EEQUIPTYPE eEquipType);
	bool UpdateHeroSkillByQuality(CUser* pUser, UINT32 hero_id, EQUALITY quality_from, EQUALITY quality_to);

	//创建新英雄信息
	bool NewHero(UINT32 hero_id, db_user_hero* db_hero_new, user_hero* c_hero_new );

	//判断英雄是否等级已经满了！
	bool IsLevelFull(UINT32 iCurLevel, UINT32 iLevelLimit);
	//判断英雄是否经验值已满
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


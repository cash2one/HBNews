#ifndef user_hero_strategy_h__
#define user_hero_strategy_h__

#include <map>
#include "def.h"
#include "config.h"

//class db_user_hero_array;
//class db_user_hero;
//class db_user_hero_equip;
//class db_user_hero_skill;

#include "dbs_gs_proto.pb.h"

//数据指针全是外部数据 这里只是保存一个指针 不负责删除
class IUserHeroStrategy{

public:
	virtual ~IUserHeroStrategy(){}

	virtual bool Init(db_user_hero_array* hero_info) = 0;

	//英雄操作 涵盖英雄基本信息 装备 技能等所有操作
	virtual bool Add(db_user_hero* hero) = 0;
	virtual db_user_hero* Get(UINT32 hero_id) = 0;
	virtual bool Del(UINT32 hero_id) = 0;
	virtual bool Update(db_user_hero* hero) = 0;

	//英雄装备操作
	virtual ::google::protobuf::RepeatedPtrField< ::db_user_hero_equip >* GetEquips(UINT32 hero_id) = 0;
	virtual db_user_hero_equip* GetEquip(UINT32 hero_id, UINT32 equip_id) = 0;
	virtual db_user_hero_equip* GetEquip(UINT32 hero_id, EEQUIPTYPE equip_type) = 0;
	virtual bool UpdateEquip(db_user_hero_equip* equip) = 0;

	virtual db_user_hero_skill* GetSkill(UINT32 hero_id, UINT32 skill_type) = 0;

	virtual UINT32 IncreaseExp(const UINT32 hero_id, const UINT32 add_exp, const UINT32 level_limit, UINT32& up_level_num, UINT32& new_cur_exp) = 0;
	//virtual bool IncreaseExp(const UINT32 hero_id, const UINT32 add_exp, const UINT32 level_limit, gs_dbs_user_info_op_req_struct_op_hero_base& db_hero_base, ::google::protobuf::RepeatedPtrField< ::struct_game_element >& c_eles ) = 0;
	virtual UINT32 IncreaseExp(const UINT32 hero_id, const UINT32 add_exp, const UINT32 level_limit, ::google::protobuf::RepeatedPtrField< ::gs_dbs_user_info_op_req_struct_op_hero_base >& db_heros, ::google::protobuf::RepeatedPtrField< ::struct_game_element >& c_eles ) = 0;

	virtual bool IsUserHeroEquipEvolutionNeed(EELEMENTTYPE eType, UINT32 iID) = 0;

	virtual UINT32 GetHeroNum() = 0;
};

//数据指针全是外部数据 这里只是保存一个指针 不负责删除
class CUserHeroStrategy : public IUserHeroStrategy{

public:
	virtual bool Init(db_user_hero_array* hero_info);
	virtual bool Add(db_user_hero* hero);
	virtual db_user_hero* Get(UINT32 hero_id){
		std::map<UINT32, db_user_hero*>::iterator it = mMapHeroID2Hero.find(hero_id);
		if ( it != mMapHeroID2Hero.end())
		{
			return it->second;
		}
		return NULL;
	}
	virtual bool Del(UINT32 hero_id){
	
		return false;
	}
	virtual bool Update(db_user_hero* hero){
		return false;
	}
	virtual ::google::protobuf::RepeatedPtrField< ::db_user_hero_equip >* GetEquips(UINT32 hero_id);

	virtual db_user_hero_equip* GetEquip(UINT32 hero_id, UINT32 equip_id);

	virtual db_user_hero_equip* GetEquip(UINT32 hero_id, EEQUIPTYPE equip_type);

	virtual bool UpdateEquip(db_user_hero_equip* equip){
		return false;
	}


	virtual db_user_hero_skill* GetSkill(UINT32 hero_id, UINT32 skill_type);

	virtual UINT32 IncreaseExp(const UINT32 hero_id, const UINT32 add_exp, const UINT32 level_limit, UINT32& up_level_num, UINT32& new_cur_exp);
	//virtual bool IncreaseExp(const UINT32 hero_id, const UINT32 add_exp, const UINT32 level_limit, gs_dbs_user_info_op_req_struct_op_hero_base& db_hero_base, ::google::protobuf::RepeatedPtrField< ::struct_game_element >& c_eles );
	virtual UINT32 IncreaseExp(const UINT32 hero_id, const UINT32 add_exp, const UINT32 level_limit, ::google::protobuf::RepeatedPtrField< ::gs_dbs_user_info_op_req_struct_op_hero_base >& db_heros, ::google::protobuf::RepeatedPtrField< ::struct_game_element >& c_eles );
	
	virtual UINT32 GetHeroNum();

	//判断是否是英雄装备进化所需要的东西
	virtual bool IsUserHeroEquipEvolutionNeed(EELEMENTTYPE eType, UINT32 iID){
		
		return false;
	}
private:
	
	std::map<UINT32, db_user_hero*> mMapHeroID2Hero;			//存在user上面  析构的时候不要释放这里！

	//用索引到hero就足够了！ 然后再查询！
	//std::map<std::pair<UINT32, UINT32>, db_user_hero_equip*> mMap2Equip;	//为了装备操作  <hero-id,equipid> 快速索引到装备
	//std::map<std::pair<UINT32, UINT32>, db_user_hero_skill*> mMap2PassiveSkill;	//为了技能操作  <hero-id,skillid> 快速索引到被动技能
	////奥义和主动技能 直接通过上面索引到db_user_info来操作
};
#endif // user_hero_strategy_h__
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

//����ָ��ȫ���ⲿ���� ����ֻ�Ǳ���һ��ָ�� ������ɾ��
class IUserHeroStrategy{

public:
	virtual ~IUserHeroStrategy(){}

	virtual bool Init(db_user_hero_array* hero_info) = 0;

	//Ӣ�۲��� ����Ӣ�ۻ�����Ϣ װ�� ���ܵ����в���
	virtual bool Add(db_user_hero* hero) = 0;
	virtual db_user_hero* Get(UINT32 hero_id) = 0;
	virtual bool Del(UINT32 hero_id) = 0;
	virtual bool Update(db_user_hero* hero) = 0;

	//Ӣ��װ������
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

//����ָ��ȫ���ⲿ���� ����ֻ�Ǳ���һ��ָ�� ������ɾ��
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

	//�ж��Ƿ���Ӣ��װ����������Ҫ�Ķ���
	virtual bool IsUserHeroEquipEvolutionNeed(EELEMENTTYPE eType, UINT32 iID){
		
		return false;
	}
private:
	
	std::map<UINT32, db_user_hero*> mMapHeroID2Hero;			//����user����  ������ʱ��Ҫ�ͷ����

	//��������hero���㹻�ˣ� Ȼ���ٲ�ѯ��
	//std::map<std::pair<UINT32, UINT32>, db_user_hero_equip*> mMap2Equip;	//Ϊ��װ������  <hero-id,equipid> ����������װ��
	//std::map<std::pair<UINT32, UINT32>, db_user_hero_skill*> mMap2PassiveSkill;	//Ϊ�˼��ܲ���  <hero-id,skillid> ������������������
	////������������� ֱ��ͨ������������db_user_info������
};
#endif // user_hero_strategy_h__
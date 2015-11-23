#pragma once
#ifndef user_crusade_strategy_h__
#define user_crusade_strategy_h__
#include "dbs_gs_proto.pb.h"
#include "config.h"
class CUser;
class IUserCrusadeStrategy{
public:
	virtual ~IUserCrusadeStrategy(void){}
	virtual bool Init(CUser* pUser, db_user_info& db_info) = 0;
	virtual void GetCrusadeInfo(gs_c_user_info_res& res) = 0;
	virtual void CrusadeStart(c_gs_crusade_start_req& req, gs_c_crusade_start_res& res, db_user_info& db_info) = 0;
	virtual void CrusadeEnd(c_gs_crusade_end_req& req, gs_c_crusade_end_res& res, gs_dbs_user_info_op_req& db_req, GS_C_NTF& ntf) = 0;
	virtual void CrusadeHeroLife(c_gs_crusade_hero_life_req& req, gs_c_crusade_hero_life_res& res, gs_dbs_user_info_op_req& db_req, GS_C_NTF& ntf) = 0;
	virtual void CrusadeReset(c_gs_crusade_reset_req& req, gs_c_crusade_reset_res& res, gs_dbs_user_info_op_req& db_req) = 0;
	virtual void CrusadePromoteAward(c_gs_crusade_promote_award_req& req, gs_c_crusade_promote_award_res& res, gs_dbs_user_info_op_req& db_req) = 0;
	virtual void CrusadeBuffRefresh(c_gs_crusade_buff_refresh_req& req, gs_c_crusade_buff_refresh_res& res, gs_dbs_user_info_op_req& db_req, GS_C_NTF& ntf) = 0;
};

class CUserCrusadeStrategy : public IUserCrusadeStrategy
{
public:
	CUserCrusadeStrategy():m_db_user_crusade(NULL), m_pUser(NULL), m_has_crusade(false), m_is_fighting(false){}
	virtual bool Init(CUser* pUser, db_user_info& db_info);
	virtual void GetCrusadeInfo(gs_c_user_info_res& res);
	virtual void CrusadeStart(c_gs_crusade_start_req& req, gs_c_crusade_start_res& res, db_user_info& db_info);
	virtual void CrusadeEnd(c_gs_crusade_end_req& req, gs_c_crusade_end_res& res, gs_dbs_user_info_op_req& db_req, GS_C_NTF& ntf);
	virtual void CrusadeHeroLife(c_gs_crusade_hero_life_req& req, gs_c_crusade_hero_life_res& res, gs_dbs_user_info_op_req& db_req, GS_C_NTF& ntf);
	virtual void CrusadeReset(c_gs_crusade_reset_req& req, gs_c_crusade_reset_res& res, gs_dbs_user_info_op_req& db_req);
	virtual void CrusadePromoteAward(c_gs_crusade_promote_award_req& req, gs_c_crusade_promote_award_res& res, gs_dbs_user_info_op_req& db_req);
	virtual void CrusadeBuffRefresh(c_gs_crusade_buff_refresh_req& req, gs_c_crusade_buff_refresh_res& res, gs_dbs_user_info_op_req& db_req, GS_C_NTF& ntf);


private:
	CUser* m_pUser;																										
	db_user_crusade* m_db_user_crusade;
	//std::map<UINT32, user_crusade::crusade_hero*> m_enemy_heros;
	std::map<UINT32, user_crusade::crusade_hero*> m_map_me_heros;
	bool m_has_crusade;
	bool m_is_fighting;	//是否在战斗中


	void _add_or_update_me_heros(const ::google::protobuf::RepeatedPtrField< ::user_crusade_crusade_hero >& me_heros);
	void _del_me_hero(UINT32 iHeroID);
	void _clear_crusade(gs_dbs_user_info_op_req& db_req);
};

#endif

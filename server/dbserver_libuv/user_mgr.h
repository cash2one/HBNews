#ifndef user_mgr_h__
#define user_mgr_h__

#include <map>

#include "dbs_gs_proto.pb.h"

#include "def.h"

class CUserMgr{

public:
	//todo 单例工具
	inline static CUserMgr* GetInstance(){
		static CUserMgr singleton;
		return &singleton;
	}

	db_user_info* GetUserInfo(UINT32 iUserID){
		std::map<UINT32, db_user_info*>::iterator it = m_mapUserID2User.find(iUserID);
		if (it != m_mapUserID2User.end())
		{
			return it->second;
		}

		return NULL;
	}

	bool Add(UINT32 iUserID, db_user_info* info){
		ASSERT(iUserID && info);
		ASSERT(m_mapUserID2User.find(iUserID) == m_mapUserID2User.end());

		if (!iUserID || !info || m_mapUserID2User.find(iUserID) != m_mapUserID2User.end())
		{
			return false;
		}

		m_mapUserID2User[iUserID] = info;

		return true;
	}

	bool Remove(UINT32 iUserID){
		ASSERT(iUserID);
		ASSERT(m_mapUserID2User.find(iUserID) != m_mapUserID2User.end());
		delete m_mapUserID2User[iUserID];
		m_mapUserID2User.erase(iUserID);
		return true;
	}
	
	const std::map<UINT32, db_user_info*>& GetAllUsers()
	{
		return m_mapUserID2User;
	}
	
	db_user_hero* GetHero(UINT32 iUserID, UINT32 iHeroID);
	db_user_hero_equip* GetEquip(UINT32 iUserID, UINT32 iHeroID, UINT32 iEquipID);
	db_user_hero_skill* GetSkill( UINT32 iUserID, UINT32 iHeroID, user_hero_skill_e_skill_type eSkillType );
	struct_game_element* GetItem(UINT32 iUserID, UINT32 iItemID);
	db_user_mail*        GetMail(UINT32 iUserID, UINT32 iMailID);
	db_user_mission_event* GetMissionEvent(UINT32 iUserID, db_user_mission_event::MISSION_EVENT_TYPE event_type, UINT32 event_sub_type);
	db_user_mission_chain* GetMissionChain(UINT32 iUserID, UINT32 iChainID);
	//db_user_shop* GetShop(UINT32 iUserID, user_shop::ESHOPTYPE eShopType);
	//db_user_shop* GetShopItem(UINT32 iUserID, user_shop::ESHOPTYPE eShopType, UINT32 iPos);
	//db_user_shop* GetShopBuy(UINT32 iUserID, user_shop::ESHOPTYPE eShopType);

	//如果玩家在线则同步好友相关信息
	void UpdateFriend(gs_dbs_user_friend_op_req& friend_req);

	bool Update(gs_dbs_user_info_op_req* req){
		// todo！ 更新内存信息！

		ASSERT(req);
		bool r = false;

		db_user_info* user_info = GetUserInfo(req->user_id());
		ASSERT(user_info);

		if(req->has_base_info()){
			r = _op_base_info(req->user_id(), req->mutable_base_info());
			ASSERT(r);
		}

		if(req->has_sub_task()){
			r = _op_sub_task(req->user_id(), req->mutable_sub_task());
			ASSERT(r);
		}

		//game_elements操作
		for (::google::protobuf::RepeatedPtrField< ::struct_game_element >::const_iterator iter = req->items().begin(); iter != req->items().end(); ++iter){
			r = _op_item(req->user_id(), *iter);
			ASSERT(r);
		}
		
		//邮件操作
		for (::google::protobuf::RepeatedPtrField< ::gs_dbs_user_info_op_req_struct_op_mail >::const_iterator iter = req->op_mails().begin(); iter != req->op_mails().end(); ++iter){
			r = _op_mail(req->user_id(), *iter);
			ASSERT(r);
		}

		//新建英雄操作
		
		for ( ::google::protobuf::RepeatedPtrField< ::gs_dbs_user_info_op_req_struct_new_hero >::const_iterator it = req->new_heros().begin(); it != req->new_heros().end() ; ++it){
			r = _new_hero(req->user_id(), *it);
			ASSERT(r);
		}
		
		//更新英雄信息
		for ( ::google::protobuf::RepeatedPtrField< ::gs_dbs_user_info_op_req_struct_op_hero_base >::const_iterator it = req->heros_base().begin(); it != req->heros_base().end() ; ++it){
			r = _op_hero_base(req->user_id(), *it);
			ASSERT(r);
		}
		
		//更新英雄装备信息
		for ( ::google::protobuf::RepeatedPtrField< ::gs_dbs_user_info_op_req_struct_op_hero_equip >::const_iterator it = req->hero_equips().begin() ; it != req->hero_equips().end() ; ++it){
			r = _op_hero_equip(req->user_id(), *it);
			ASSERT(r);
		}
		
		//更新英雄技能信息
		for ( ::google::protobuf::RepeatedPtrField< ::gs_dbs_user_info_op_req_struct_op_hero_skill >::const_iterator it = req->hero_skills().begin(); it != req->hero_skills().end() ; ++it){
			r = _op_hero_skill(req->user_id(), *it);
			ASSERT(r);
		}
		
		//商店创建或者刷新 追加或者更新
		for ( ::google::protobuf::RepeatedPtrField< ::gs_dbs_user_info_op_req::struct_op_shop >::const_iterator it = req->shops().begin(); it != req->shops().end() ; ++it){
			//r = _op_shop(req->user_id(), *it);
			//ASSERT(r);

			bool has = false;
			for ( ::google::protobuf::RepeatedPtrField< ::db_user_shop >::iterator itIn = user_info->mutable_shops()->mutable_items()->begin(); itIn != user_info->mutable_shops()->mutable_items()->end() ; ++itIn){
				if(itIn->shop_type() == it->shop().shop_type()){
					//user_info = &*itIn;
					itIn->CopyFrom(it->shop());
					has = true;
					break;
				}
			}
			
			if (!has){
				db_user_shop* new_shop = user_info->mutable_shops()->add_items();
				new_shop->CopyFrom(it->shop());
			}
		}

		//商店道具购买相关更新 改为已售
		for ( ::google::protobuf::RepeatedPtrField< ::gs_dbs_user_info_op_req::struct_op_shop_item >::const_iterator it = req->shop_items().begin() ; it != req->shop_items().end() ; ++it){
			//r = _op_shop_item(req->user_id(), *it);
			//ASSERT(r);

			bool has = false;
			for ( ::google::protobuf::RepeatedPtrField< ::db_user_shop >::iterator itShop = user_info->mutable_shops()->mutable_items()->begin(); itShop != user_info->mutable_shops()->mutable_items()->end() ; ++itShop){
				if(itShop->shop_type() == it->shop_type()){
					for (::google::protobuf::RepeatedPtrField< ::db_user_shop_item >::iterator itItem = itShop->mutable_items()->begin(); itItem != itShop->mutable_items()->end(); ++itItem){
						if (itItem->pos() == it->item().pos()){
							ASSERT(!itItem->is_sold() && it->item().is_sold());//之前没有售出 而现在一定是已售出
							
							//itItem->set_is_sold(true);//设置已售出 这就够了 但是为了以后的可能的变化 改变了其他的信息
							itItem->CopyFrom(it->item());
							
							has = true;
							goto FIND_ITEM;
						}
						
					}
					
				}
			}
FIND_ITEM:
			ASSERT(has);
		}

		//更新购买信息 追加或者更新
		for ( ::google::protobuf::RepeatedPtrField< ::db_user_shop_buy >::const_iterator it = req->buy_items().begin(); it != req->buy_items().end() ; ++it){
			//r = _op_shop_buy(req->user_id(), *it);
			//ASSERT(r);

			bool has = false;
			for (::google::protobuf::RepeatedPtrField< ::db_user_shop_buy >::iterator itShopBuy = user_info->mutable_buy_items()->mutable_items()->begin(); itShopBuy != user_info->mutable_buy_items()->mutable_items()->end() ; ++itShopBuy){
				if (itShopBuy->element_type() == it->element_type() && itShopBuy->element_id() == it->element_id()){
					itShopBuy->CopyFrom(*it);
					has = true;
				}
			}

			if (!has){
				db_user_shop_buy* new_shop_buy = user_info->mutable_buy_items()->add_items();
				new_shop_buy->CopyFrom(*it);
			}
		}
		
		//任务信息更新
		for(::google::protobuf::RepeatedPtrField< gs_dbs_user_info_op_req::struct_op_mission >::const_iterator it =  req->missions().begin(); it != req->missions().end(); ++it)
		{
			r = _op_mission(req->user_id(), *it);
			ASSERT(r);
		}

#define SET_FIELD_IF_HAS(ptr_src, ref_to, field_name) if(ref_to.has_##field_name##()) ptr_src->set_##field_name##(ref_to.##field_name##());
#define SET_FIELD(ptr_src, ref_to, field_name) ptr_src->set_##field_name##(ref_to.##field_name##());
		
		//修改竞技场出战英雄
		if (req->has_new_arena()){
			ASSERT(!user_info->has_arena());
			if (user_info->has_arena()){
				return false;
			}

			ASSERT(req->new_arena().IsInitialized());
			if (!req->new_arena().IsInitialized()){
				return false;
			}
			//db_user_arena &pArena = user_info->arena();//没有的话这里可以创建的！
			//user_info->mutable_arena()->mutable_heros()->CopyFrom(req->arena().heros());

			db_user_arena* pArena = user_info->mutable_arena();//没有的话这里可以创建的！
			//pArena->set_rank(req->arena().rank());
			//pArena->set_score(req->arena().score());//竞技场积分
			//pArena->set_last_challenge_time(req->arena().last_challenge_time());//上一次的挑战时间
			//pArena->set_last_day_challenge_num(req->arena().last_day_challenge_num());//last_challenge_time当天已经挑战的次数
			//pArena->set_history_highest_rank(req->arena().history_highest_rank());//历史最高排名
			//pArena->mutable_heros()->CopyFrom(req->arena().heros());

			SET_FIELD(pArena, req->new_arena(), rank);
			SET_FIELD(pArena, req->new_arena(), score);
			SET_FIELD(pArena, req->new_arena(), last_challenge_time);
			SET_FIELD(pArena, req->new_arena(), last_day_challenge_num);
			SET_FIELD(pArena, req->new_arena(), history_highest_rank);
			SET_FIELD(pArena, req->new_arena(), last_time_buy_arena_challenge);
			SET_FIELD(pArena, req->new_arena(), last_day_buy_arena_challenge_num);
			pArena->mutable_heros()->CopyFrom(req->new_arena().heros());
		}

		//新增加或者修改玩家竞技场信息 rank的添加在这里 修改不在这里！因此不要设置
		if(req->has_op_arena()){
			ASSERT(user_info->has_arena());
			if (!user_info->has_arena()){
				return false;
			}

			db_user_arena* pArena = user_info->mutable_arena();//没有的话这里可以创建的！
			//pArena->set_rank(req->arena().rank());
			//pArena->set_score(req->arena().score());//竞技场积分
			//pArena->set_last_challenge_time(req->arena().last_challenge_time());//上一次的挑战时间
			//pArena->set_last_day_challenge_num(req->arena().last_day_challenge_num());//last_challenge_time当天已经挑战的次数
			//pArena->set_history_highest_rank(req->arena().history_highest_rank());//历史最高排名
			//pArena->mutable_heros()->CopyFrom(req->arena().heros());
			////repeated db_user_arena_log	logs = 5;//竞技场日志 最多十条

			SET_FIELD_IF_HAS(pArena, req->op_arena(), rank);
			SET_FIELD_IF_HAS(pArena, req->op_arena(), score);
			SET_FIELD_IF_HAS(pArena, req->op_arena(), last_challenge_time);
			SET_FIELD_IF_HAS(pArena, req->op_arena(), last_day_challenge_num);
			SET_FIELD_IF_HAS(pArena, req->op_arena(), history_highest_rank);
			SET_FIELD_IF_HAS(pArena, req->op_arena(), last_time_buy_arena_challenge);
			SET_FIELD_IF_HAS(pArena, req->op_arena(), last_day_buy_arena_challenge_num);
			if (req->op_arena().heros_size() > 0 ){
					pArena->mutable_heros()->CopyFrom(req->op_arena().heros());
			}
		}

		//竞技场挑战结果 只负责添加日志 和 被挑战者的排名
		if (req->has_arena_challenge()){
			//optional uint32 score = 9;//获胜时的积分 目前用不到

			ASSERT(user_info->has_arena());//以前就有 或者 前面已经创建好！
			db_user_arena* pArena = user_info->mutable_arena();//没有的话这里可以创建的！
			//repeated db_user_arena_log	logs = 5;//竞技场日志 最多十条

			const gs_dbs_user_info_op_req::struct_op_arena_challenge &op_arena_challenge = req->arena_challenge();
			ASSERT(req->user_id() == op_arena_challenge.attacker_user_id());
			if (req->user_id() != op_arena_challenge.attacker_user_id()){
				ASSERT(false);
				printf("error! 只有主动进攻者才会发竞技场操作信息！\n");
				return false;
			}
			
			db_user_arena_log* pLog = pArena->add_logs();
			pLog->set_is_attacker_win(op_arena_challenge.is_win());//挑战者是否获胜
			pLog->set_time(op_arena_challenge.time());//挑战发起时间
			pLog->set_is_me_attacker(true);
			pLog->set_me_rank(op_arena_challenge.attacker_rank());

			pLog->set_match_user_id(op_arena_challenge.defender_user_id());
			pLog->set_match_rank(op_arena_challenge.defender_rank());
			pLog->set_match_power(op_arena_challenge.defender_power());
			pLog->set_match_name(op_arena_challenge.defender_name());
			pLog->set_match_level(op_arena_challenge.defender_level());
			pLog->mutable_match_heros()->CopyFrom(op_arena_challenge.defender_heros());
			
			
			db_user_info* pDefender = GetUserInfo(req->arena_challenge().defender_user_id());
			if (pDefender){
				ASSERT(pDefender->has_arena());//一定有竞技场信息 不然不能被挑战的
				if (req->arena_challenge().is_win()){
					pDefender->mutable_arena()->set_rank(req->arena_challenge().attacker_rank());
				}

				db_user_arena_log* pDefLog =  pDefender->mutable_arena()->add_logs();
				//pDefLog->CopyFrom(*pLog);

				pDefLog->set_is_attacker_win(op_arena_challenge.is_win());//挑战者是否获胜
				pDefLog->set_time(op_arena_challenge.time());//挑战发起时间
				pDefLog->set_is_me_attacker(false);
				pDefLog->set_me_rank(req->arena_challenge().defender_rank());

				pDefLog->set_match_user_id(op_arena_challenge.attacker_user_id());
				pDefLog->set_match_rank(op_arena_challenge.attacker_rank());
				pDefLog->set_match_power(op_arena_challenge.attacker_power());
				pDefLog->set_match_name(op_arena_challenge.attacker_name());
				pDefLog->set_match_level(op_arena_challenge.attacker_level());
				pDefLog->mutable_match_heros()->CopyFrom(op_arena_challenge.attacker_heros());
			}
		}
		
		if (req->has_new_sign()){
			if (!user_info->has_sign()){//新用户已经有签到信息无需更新
				ASSERT(req->new_sign().IsInitialized());
				if (!req->new_sign().IsInitialized()){
					return false;
				}

				db_user_sign* pSign = user_info->mutable_sign();
				pSign->set_sign_1_times(req->new_sign().sign_1_times());
				pSign->set_sign_n_reward_status(req->new_sign().sign_n_reward_status());
				pSign->set_last_sign_time(req->new_sign().last_sign_time());
				pSign->set_add_signed_times(req->new_sign().add_signed_times());
			}
		}

		//签到
		if (req->has_sign()){
			r = _op_sign(req->user_id(), req->sign());
			ASSERT(r);
		}
		
		//新手引导
		if (req->has_novice_guide())
		{
			r = _op_novice_guide(req->user_id(), req->novice_guide());
			ASSERT(r);
		}
		
		//好友信息
		for(::google::protobuf::RepeatedPtrField< ::gs_dbs_user_info_op_req_struct_op_friend >::const_iterator it = req->op_friends().begin(); it != req->op_friends().end(); ++it){
			r = _op_friend(req->user_id(), *it);
			ASSERT(r);
		}
		
		//远征
		if (req->has_op_crusade())
		{
			r = _op_crusade(req->user_id(), req->op_crusade());
			ASSERT(r);
		}


		return false;
	}

private:
	
	bool _op_base_info( UINT32 iUserID, gs_dbs_user_info_op_req_struct_op_base_info* op );

	bool _op_sub_task( UINT32 iUserID, gs_dbs_user_info_op_req_struct_op_sub_task* op_sub_task );

	bool _op_item(UINT32 iUserID, const struct_game_element& op_game_ele);
	
	bool _op_mail(UINT32 iUserID, const gs_dbs_user_info_op_req_struct_op_mail& op_mail);
	
	bool _op_mission(UINT32 iUserID, const gs_dbs_user_info_op_req_struct_op_mission& op_mission);

	bool _op_sign(UINT32 iUserID, const gs_dbs_user_info_op_req_struct_op_sign& op_sign);

	bool _op_novice_guide(UINT32 iUserID, const gs_dbs_user_info_op_req::struct_op_novice_guide& novice_guide);

	bool _op_friend(UINT32 iUserID, const gs_dbs_user_info_op_req::struct_op_friend& op_friend);

	bool _op_crusade(UINT32 iUserID, const gs_dbs_user_info_op_req::struct_op_crusade& op_crusade);

	db_user_friend::friend_info* _get_friend(UINT32 iUserID, UINT32 iFriend);
	bool _del_user_apply(UINT32 iUserID, UINT32 iFriendID);
	bool _del_user_affirm(UINT32 iUserID, UINT32 iFriendID);
	bool _del_user_friend(UINT32 iUserID, UINT32 iFriendID);
	bool _affirm_friend(UINT32 iUserID, UINT32 iFriendID);
	bool _friend_affirm_friend(UINT32 iUserID, UINT32 iFriendID);
	bool _del_user_recommend(UINT32 iUserID, UINT32 iFriendID);
	//英雄相关操作
	bool _new_hero(UINT32 iUserID, const gs_dbs_user_info_op_req_struct_new_hero& new_hero);
	bool _op_hero_base(UINT32 iUserID, const gs_dbs_user_info_op_req_struct_op_hero_base& op_hero_base);
	bool _op_hero_equip(UINT32 iUserID, const gs_dbs_user_info_op_req_struct_op_hero_equip& op_hero_equip);
	bool _op_hero_skill(UINT32 iUserID, const gs_dbs_user_info_op_req_struct_op_hero_skill& op_hero_skill);

	user_crusade::crusade_hero* _get_crusade_me_hero(UINT32 iUserID, UINT32 iMeHeroID);
	//商店相关操作
	//bool _op_shop(UINT32 iUserID, const gs_dbs_user_info_op_req::struct_op_shop& op_shop);
	//bool _op_shop_item(UINT32 iUserID, const gs_dbs_user_info_op_req::struct_op_shop_item& op_shop_item);
	//bool _op_shop_buy(UINT32 iUserID, const db_user_shop_buy& op_shop_buy);
	
	std::map<UINT32, db_user_info*> m_mapUserID2User;//在线 todo gs上的离线操作
	std::map<UINT32, db_user_info*> m_mapUserID2UserDelayOffline;//todo 下线缓存列表 数据暂时放着
};

#endif // user_mgr_h__
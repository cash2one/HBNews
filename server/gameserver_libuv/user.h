#ifndef __USER_H__
#define __USER_H__

#pragma once

#include <set>

#include "memory.h"
#include "uv.h"

#include "config.h"
#include "def.h"

#include "gs_c_proto.pb.h"
#include "dbs_gs_proto.pb.h"
//#include "db_user_bag.pb.h"
//#include "db_user_task.pb.h"
//#include "db_user_hero.pb.h"
//#include "db_user_friend.pb.h"

#include "user_task_strategy.h"
#include "user_hero_strategy.h"
#include "item_bag.h"
#include "user_mail_strategy.h"
#include "time_helper.h"

#include "shop_mgr.h"
#include "user_shop_strategy.h"
#include "user_mission_strategy.h"
#include "user_arena_strategy.h"
#include "user_sign_strategy.h"
#include "user_novice_guide_strategy.h"
#include "user_friend_strategy.h"
#include "user_chat_strategy.h"
#include "user_crusade_strategy.h"
class CHeroTeam{
public:

	CHeroTeam():m_curTeamIdx(0){
		ClearTeam();
	}

	void GetCurTeamHero(std::set<UINT32>& heros){
		for (int i = 0; i < MAXNUM_HEROINTEAM ; ++i)
		{
			UINT32 hero_id = m_Teams[m_curTeamIdx][i];
			if (hero_id)
			{
				heros.insert(hero_id);
			}
		}
	}

	UINT32 UpdateTeam(UINT8 iTeamIdx, UINT32 (&heros)[MAXNUM_HEROINTEAM]);

	bool UpdateTeam(const ::google::protobuf::RepeatedField< ::google::protobuf::uint32 >& heros){
		ASSERT(MAXNUM_TEAM >= 0);
		ASSERT(heros.size() <= MAXNUM_HEROINTEAM);

		int i = 0;
		for (::google::protobuf::RepeatedField< ::google::protobuf::uint32 >::const_iterator it = heros.begin() ; it != heros.end(); ++it)
		{
			m_Teams[m_curTeamIdx][i++] = *it;
		}

		return true;
	}

	void ClearTeam(){
		memset(m_Teams, 0, sizeof(m_Teams));
	}
private:
	UINT8 m_curTeamIdx;
	UINT32 m_Teams[MAXNUM_TEAM][MAXNUM_HEROINTEAM];//每个队伍存储的是HeroID, 每种英雄玩家最多只有一个

	friend class CCondition_Occupation;
	friend class CTaskEnterCondition_NatureProperty;
	friend class CCondition_Area;
};

class CUser{

public:

	CUser(){
		m_connect = NULL;
		m_db_user_info = NULL;
		m_taskStrategy = NULL;
		m_heroStrategy = NULL;
		m_pItemBag = NULL;
		m_shopStrategy = NULL;
		m_userMailStrategy = NULL;
		m_arenaStrategy = NULL;
		m_usersignstrategy = NULL;
		m_userFriendStrategy = NULL;
		m_userChatStrategy = NULL;

	}

	~CUser(){

		Unit();
	
		m_connect = NULL;
		m_db_user_info = NULL;
		m_shopStrategy = NULL;
		m_pItemBag = NULL;
		m_taskStrategy = NULL;
		m_heroStrategy = NULL;
		m_userMailStrategy = NULL;
		m_arenaStrategy = NULL;
		m_usersignstrategy = NULL;
		m_userNoviceGuideStrategy = NULL;
		m_userFriendStrategy = NULL;
		m_userChatStrategy = NULL;
	}

	bool Init(db_user_info* db_data, gs_dbs_user_info_op_req& db_req){
		ASSERT(db_data);
		m_db_user_info = db_data;

		//初始化任务信息
		m_taskStrategy = new CUserTaskStrategy;
		//if (m_db_user_info->has_sub_tasks())//没有的时候创建！否则策略添加不了子任务
		{
			bool r = m_taskStrategy->Init(this, m_db_user_info->mutable_sub_tasks());
			if (!r)
			{
				return false;
			}
		}

		//初始化背包信息
		m_pItemBag	= new CItemBag;
		//if (m_db_user_info->has_items())
		{		
			bool r = m_pItemBag->Init(m_db_user_info->mutable_items());
			if (!r)
			{
				return false;
			}
		}

		//初始化英雄信息
		m_heroStrategy = new CUserHeroStrategy();
		//if (m_db_user_info->has_heros())//没有的时候创建！否则策略添加不了英雄
		{
			bool r = m_heroStrategy->Init(m_db_user_info->mutable_heros());
			if (!r){
				return false;
			}
		}
		
		m_userMailStrategy = new CUserMailStrategy;
		bool r = m_userMailStrategy->Init(this, m_db_user_info->mutable_mails_received(), db_req);
		ASSERT(r);
		if (!r)
		{
			return false;
		}
		//初始化商店物品信息
		//初始化商店购买记录
		m_shopStrategy = new CUserShopStrategy();
		r = m_shopStrategy->Init(this, m_db_user_info->mutable_shops()->items(), m_db_user_info->mutable_buy_items()->items());
		ASSERT(r);
		
		//初始化任务信息
		m_userMissionStrategy = new CUserMissionStrategy;
		r = m_userMissionStrategy->Init(this, m_db_user_info->mutable_mission_events(), m_db_user_info->mutable_mission_chains(), db_req);
		ASSERT(r);
		
		m_arenaStrategy = new CUserArenaStrategy();
		r = m_arenaStrategy->Init(this, *m_db_user_info);
		ASSERT(r);
		
		m_usersignstrategy = new CUserSignStrategy;
		r = m_usersignstrategy->Init(this, *m_db_user_info, db_req);
		ASSERT(r);
		
		m_userNoviceGuideStrategy = new CUserNoviceGuideStrategy;
		r = m_userNoviceGuideStrategy->Init(this, m_db_user_info->mutable_guide());
		
		m_userFriendStrategy = new CUserFriendStrategy;
		r = m_userFriendStrategy->Init(this, m_db_user_info->mutable_user_friend());
		ASSERT(r);

		m_userChatStrategy = new CUserChatStrategy;
		r = m_userChatStrategy->Init(this);
		ASSERT(r);
		
		m_userCrusadeStrategy = new CUserCrusadeStrategy;
		r = m_userCrusadeStrategy->Init(this, *m_db_user_info);
		ASSERT(r);
		return true;	
	}

	void Unit(){
		delete m_userNoviceGuideStrategy;
		delete m_usersignstrategy;
		delete m_userMissionStrategy;
		delete m_arenaStrategy;
		delete m_pItemBag;
		delete m_heroStrategy;
		delete m_taskStrategy;
		delete m_userMailStrategy;
		delete m_shopStrategy;
		delete m_userFriendStrategy;
		delete m_userChatStrategy;
		delete m_db_user_info;
	}
	
	void PrintInfo(){
		ASSERT(m_db_user_info);
		m_db_user_info->PrintDebugString();
	}

	bool HasConnect(){
		return m_connect != NULL;
	}
	uv_stream_t* GetConnect(){
		return m_connect;
	}
	bool IntertwineConnect(uv_stream_t* connect);
	bool UnIntertwineConnect(uv_stream_t* connect);
	bool Send(UINT32 msg_id, google::protobuf::Message* pMessage);
	void SendNtf(GS_C_NTF& ntf);
	
	////玩家一段时间内 不响应时调用， 并不一定只是在下线时调用
	bool OnLeave();

	UINT32 UserID() const { 
		ASSERT(m_db_user_info);
		return m_db_user_info->user_base_info().user_id(); 
	}

	std::string Name() const{
		ASSERT(m_db_user_info);
		return m_db_user_info->user_base_info().name();
	}

	void Name(const std::string& name){
		ASSERT(m_db_user_info);
		return m_db_user_info->mutable_user_base_info()->set_name(name);
	}

	UINT32 GetMaxHealth();
	UINT32 GetHealth();
	UINT32 GetHealthLastUpdateTime(){
			return _get_health_last_update_time();
	}
	//todo 外部增加 查看体力增加规则！
	UINT32 IncreaseHealth(UINT32 iHealth);
	bool DecreaseHealth(UINT32 iHealth, bool* isFull2NotFull);
	bool IsHealthFull();
	UINT32 GetMaxSkillPoint(UINT32 iVipLevel);
	UINT32 GetSkillPoint();
	UINT32 IncreaseSkillPoint(UINT32 iSkillPoint);
	bool DecreaseSkillPoint(UINT32 iSkillPoint, bool* isFull2NotFull);

	//某些handle里面 客户端需要知道上一次的充值时间
	//注意 调用此函数之前 需要先调用了GetSkillPoint 否则获取的时间 可能是旧时间！
	UINT32 GetSkillPointLastUpdateTime(){
		return _get_skill_point_last_update_time();
	}
	
	void IncreaseDiamond(UINT32 iDiamond){
		_set_diamond(_get_diamond() + iDiamond);
	}

	bool DecreaseDiamond(UINT32 iDiamond){
		ASSERT(_get_diamond() >= iDiamond);
		if (_get_diamond() < iDiamond){
			return false;
		}
		_set_diamond(_get_diamond() - iDiamond);
		return true;
	}
	
	void IncreaseGold(UINT32 iGold){
		_set_gold(_get_gold() + iGold);
	}

	UINT32 GetExp(){
		return _get_exp();
	}

	//添加玩家经验值
	//UINT32 IncreaseExp(const UINT32 add_exp, UINT32& up_level_num, UINT32& new_cur_exp);

	UINT32 IncreaseExp(const UINT32 add_exp, gs_dbs_user_info_op_req& db_req, ::google::protobuf::RepeatedPtrField<struct_game_element>& c_rep_game_ele, GS_C_NTF& gs_c_ntf);

	//bool IncreaseHeroExp(UINT32 hero_id, UINT32 add_exp, UINT32& up_level_num, UINT32& left_exp ){
	//	ASSERT(m_heroStrategy);
	//	//是否升级
	//	return m_heroStrategy->IncreaseExp(hero_id, add_exp, GetLevel(), up_level_num, left_exp);
	//	//是否开放被动技能 不开放技能的 技能是升级品质的时候才会开
	//}

	//是否首次金币十连抽
	bool isFirstInvokeGoldN();
	//是否首次钻石十连抽
	bool isFirstInvokeDiamondN();
	bool IncreaseInvokeGoldNTotalNum(gs_dbs_user_info_op_req& db_req){
		_set_invoke_gold_n_total_num(_get_invoke_gold_n_total_num() + 1);
		gs_dbs_user_info_op_req_struct_op_base_info* db_op_base = db_req.mutable_base_info();
		db_op_base->set_op_type(gs_dbs_user_info_op_req::Update);
		db_op_base->set_invoke_gold_n_total_num(_get_invoke_gold_n_total_num());
		return true;
	}

	bool IncreaseInvokeDiamondNTotalNum(gs_dbs_user_info_op_req& db_req){
		_set_invoke_diamond_n_total_num(_get_invoke_diamond_n_total_num() + 1);
		gs_dbs_user_info_op_req_struct_op_base_info* db_op_base = db_req.mutable_base_info();
		db_op_base->set_op_type(gs_dbs_user_info_op_req::Update);
		db_op_base->set_invoke_diamond_n_total_num(_get_invoke_diamond_n_total_num());

		return true;
	}

	//获取金币免费抽取剩余使用次数
	UINT32 GetInvokeGoldFreeLeftNum();
	//减少金币免费抽取剩余使用次数
	bool DecreaseInvokeGoldFreeLeftNum(UINT32 iPoint);
	//获取钻石免费抽取剩余使用次数
	UINT32 GetInvokeDiamondFreeLeftNum();
	//减少钻石免费抽取剩余使用次数
	bool DecreaseInvokeDiamondFreeLeftNum(UINT32 iPoint, bool* isFull2NotFull);
	UINT32 GetInvokeGoldFreeUpdateTime();
	UINT32 GetInvokeDiamondFreeUpdateTime();
	
	UINT32 GetLevel(){
		return _get_level();
	}

	UINT32 GetGold(){
		return _get_gold();
	}

	void SetGold(UINT32 gold){
		_set_gold(gold);
	}
	
	UINT32 GetVipLevel(){
		return _get_vip_level();
	}
	
	UINT32 GetDiamond(){
		return _get_diamond();
	}

	UINT32 GetFriendPoint(){
		return _get_friend_point();
	}

	UINT32 GetHeadID(){
		return _get_head_id();
	}
	
	UINT32 GetHeadFrameID(){
		return _get_head_frame_id();
	}
	
	const std::string& GetSignature(){
		return _get_signature();
	}

	IItem* GetItem(UINT32 iItemID){
		return m_pItemBag->GetItem(iItemID);
	}
	bool CanAddItems(std::vector<stGameElement*>& vecEles){
		return m_pItemBag->CanAddItems(vecEles);
	}
	bool CanAddElements(std::vector<struct_game_element*>& vecEle){
		for (size_t i = 0; i < vecEle.size(); ++i)
		{
			if(vecEle[i]->type() == struct_game_element::EELEMENTTYPE_ITEM)
			{	
				if(!m_pItemBag->CanAddItem(vecEle[i]))
					return false;
			}
		}
		return true;
	}

	bool HasItems(std::vector<stGameElement*>& vecEles){
		return m_pItemBag->HasItems(vecEles);
	}
	bool IsItemsEnough(::google::protobuf::RepeatedPtrField< ::struct_game_element >* pRepeatedEle){
		return m_pItemBag->IsItemsEnough(pRepeatedEle);
	}
	bool IsItemEnough(UINT32 iItemID, UINT32 iNum){
		return m_pItemBag->IsItemEnough(iItemID, iNum);
	}
	bool IsConsumeEnough(std::vector<stGameElement>& vecEles);
	bool IsConsumeEnough( std::vector<stGameElement*>& vecEles );
	bool IsConsumeEnough(EELEMENTTYPE eType, UINT32 iID, UINT32 iNum);
	void AddItem(const struct_game_element& gameEle, ::google::protobuf::RepeatedPtrField< ::struct_game_element >* pRepeatedEle, ::google::protobuf::RepeatedPtrField<::struct_game_element>* p_c_rep_game_ele, UINT32& iActualAddNum){
		ASSERT(m_pItemBag);
		m_pItemBag->AddItem(gameEle, pRepeatedEle, p_c_rep_game_ele, iActualAddNum);
	}
	//这个函数不能用 因为新玩家背包还没有初始好 设置没用
	//新注册的玩家 给予初始财富 只调用一次！
	//bool GiveInitPlayerBelongings();

	bool DelMails(std::vector<UINT32>& vecMailID, gs_dbs_user_info_op_req* pDBReq, gs_c_mail_opt_res* pClientRes){
		return m_userMailStrategy->DelMails(vecMailID, pDBReq, pClientRes);
	}
	
	bool HasMails(std::vector<UINT32>& vecMailID){
		return m_userMailStrategy->HasMails(vecMailID);
	}

	bool SetMailReaded(UINT32 iMailID, gs_dbs_user_info_op_req* pDBMsg, gs_c_mail_opt_res* pClientMsg){
		return m_userMailStrategy->SetMailReaded(iMailID, pDBMsg, pClientMsg);
	}
	
	MAIL_STATUS GetMailStatus(UINT32 iMailID){
		return m_userMailStrategy->GetMailStatus(iMailID);
	}

	bool IsMailsHasAttachs(std::vector<UINT32>& vecMailId){
		return m_userMailStrategy->IsMailsHasAttachs(vecMailId);
	}
		
	//获取所有有附件的邮件id
	std::vector<UINT32> GetAllHasAttachMailIDS(){
		return m_userMailStrategy->GetAllHasAttachMailIDS();
	}

	void DelOutOfDateMail(gs_dbs_user_info_op_req* db_req, GS_C_NTF& ntf);

	void AddMail(db_user_mail& user_mail, gs_dbs_user_info_op_req& req, GS_C_NTF& ntf){
		ASSERT(m_userMailStrategy);
		m_userMailStrategy->AddMail(user_mail, req, ntf);
	}
	//处理一封邮件的附件信息，返回值为数据库中该邮件信息是否需要更新(领取是否意味邮件就读过了)
	bool GetMailAttach(UINT32 iMailID, gs_c_mail_opt_res* pRes, gs_dbs_user_info_op_req* db_req, GS_C_NTF& client_arr){
		ASSERT(m_userMailStrategy && pRes && db_req);
		return m_userMailStrategy->GetMailAttach(iMailID,  pRes, db_req, client_arr);
	}
	
	bool CreateMail(MAIL_TYPE eMailType, gs_dbs_user_info_op_req& db_req, 
		const char* strParams = NULL, std::map<std::string, CValue>* mapParams = NULL, std::vector<stGameElement>* vecstGameEle = NULL){
			ASSERT(m_userMailStrategy);
			return m_userMailStrategy->CreateMail(eMailType, db_req, strParams, mapParams, vecstGameEle);
	}
	

	bool CreateMail(MAIL_TYPE eMailType, gs_dbs_user_info_op_req& db_req, 
		const char* strParams = NULL, std::map<std::string, CValue>* mapParams = NULL, std::vector<stGameElement*>* vecstGameEle = NULL){
		ASSERT(m_userMailStrategy);
		return m_userMailStrategy->CreateMail(eMailType, db_req, strParams, mapParams, vecstGameEle);
	}
	
	bool CreateMail(MAIL_TYPE eMailType, gs_dbs_user_info_op_req& db_req, 
		const char* strParams = NULL, std::map<std::string, CValue>* mapParams = NULL, stGameElement* pstGameEle = NULL){
			ASSERT(m_userMailStrategy);
			if (pstGameEle)
			{
				std::vector<stGameElement*> vecGameEle;
				vecGameEle.push_back(pstGameEle);
				return CreateMail(eMailType, db_req, strParams, mapParams, &vecGameEle);
			}
			else
			{
				return CreateMail(eMailType, db_req, strParams, mapParams, (std::vector<stGameElement*>*)NULL);
			}
	}

	//获取玩家的章节信息， 判断是否主线任务都过了
	e_gsc_errorcode GetChapterInfo(stUserChapterInfo& userChapterInfo){
		ASSERT(m_taskStrategy);
		return m_taskStrategy->GetChapterInfo(userChapterInfo);
	}

	db_user_sub_task* GetSubTask(UINT32 iSubTaskID){
		ASSERT(m_taskStrategy);
		return m_taskStrategy->GetSubTask(iSubTaskID);
	}

	bool AddOrUpdateSubTask(const db_user_sub_task& sub_task, gs_dbs_user_info_op_req* db_req){
		ASSERT(m_taskStrategy);
		return m_taskStrategy->AddOrUpdateSubTask(sub_task, db_req);
	}

	bool IsSubTaskDone(UINT32 iSubTaskID){
		ASSERT(m_taskStrategy);
		return m_taskStrategy->IsSubTaskDone(iSubTaskID);
	}

	bool IsTaskDone(UINT32 iTaskID){
		ASSERT(m_taskStrategy);
		return m_taskStrategy->IsTaskDone(iTaskID);
	}

	e_gsc_errorcode SpeedKillSubTaskByItem(UINT32 iSubTaskID, UINT32 iTimes, gs_c_use_item_res& res, struct_game_element& req_consume_ele,  gs_dbs_user_info_op_req& db_req, GS_C_NTF& ntf){
		ASSERT(m_taskStrategy);
		bool isShopOpen = false;
		e_gsc_errorcode errorcode = m_taskStrategy->SpeedKillSubTask(iSubTaskID, iTimes, *res.mutable_consume(), *res.mutable_items_arr(), *res.mutable_shop(), req_consume_ele, db_req, ntf, isShopOpen);
		if (!isShopOpen)
		{
			res.set_allocated_shop(NULL);
		}
		return errorcode;
	}
	
	e_gsc_errorcode SpeedKillSubTaskByDiamond(UINT32 iSubTaskID, UINT32 iTimes, gs_c_diamond_speed_kill_res& res, struct_game_element& req_consume_ele,  gs_dbs_user_info_op_req& db_req, GS_C_NTF& ntf){
		ASSERT(m_taskStrategy);
		bool isShopOpen = false;
		e_gsc_errorcode errorcode = m_taskStrategy->SpeedKillSubTask(iSubTaskID, iTimes, *res.mutable_consume(), *res.mutable_items_arr(), *res.mutable_shop(), req_consume_ele, db_req, ntf, isShopOpen);
		if (!isShopOpen)
		{
			res.set_allocated_shop(NULL);
		}
		return errorcode;
	}

	UINT32 GetSubTaskLeftEnterNum(const UINT32 iSubTaskID){
		ASSERT(m_taskStrategy);
		return m_taskStrategy->GetSubTaskLeftEnterNum(iSubTaskID, GetVipLevel());
	}

	bool CanStartFight(UINT32 iSubTaskID){
		ASSERT(m_taskStrategy);
		return m_taskStrategy->CanStartFight(this, iSubTaskID);
	}

	bool GetMapPointCanVisit(UINT16 iChapterID, std::set<UINT32> &retPoints, UINT32& curMainTaskPointID){
		ASSERT(m_taskStrategy);
		return m_taskStrategy->GetMapPointCanVisit(iChapterID, retPoints, curMainTaskPointID);	
	}

	bool CanVisitMapPoint(UINT32 iMapPointID){
		ASSERT(m_taskStrategy);
		return m_taskStrategy->CanVisitMapPoint(iMapPointID);
	}
	
	db_user_hero* GetHero(UINT32 hero_id){
		ASSERT(m_heroStrategy);
		return m_heroStrategy->Get(hero_id);
	}

	//内部复制
	bool AddNewHero(const db_user_hero& hero, GS_C_NTF& gs_c_ntf, gs_dbs_user_info_op_req& db_req);

	db_user_hero_equip* GetHeroEquip(UINT32 hero_id, UINT32 equip_id){
		ASSERT(m_heroStrategy);
		return m_heroStrategy->GetEquip(hero_id, equip_id);
	}

	::google::protobuf::RepeatedPtrField< ::db_user_hero_equip >*  GetHeroEquips(UINT32 hero_id){
		ASSERT(m_heroStrategy);
		return m_heroStrategy->GetEquips(hero_id);
	}
	

	db_user_hero_skill* GetHeroSkill(UINT32 hero_id, UINT32 skill_type){
		ASSERT(m_heroStrategy);
		return m_heroStrategy->GetSkill(hero_id, skill_type);
	}

	bool IsUserHeroEquipEvolutionNeed(EELEMENTTYPE eType, UINT32 iID){
		ASSERT(m_heroStrategy);
		return m_heroStrategy->IsUserHeroEquipEvolutionNeed(eType, iID);
	}
	
	UINT32 GetHeroNum(){
		ASSERT(m_heroStrategy);
		return m_heroStrategy->GetHeroNum();
	}

	bool UpdateGameElement(const struct_game_element& ele, gs_dbs_user_info_op_req& db_req, ::google::protobuf::RepeatedPtrField<struct_game_element>& c_rep_game_ele, GS_C_NTF& gs_c_ntf, ::google::protobuf::RepeatedPtrField<user_hero>* c_heros = NULL);
	bool UpdateGameElement(const ::google::protobuf::RepeatedPtrField<struct_game_element>& rep_ele, gs_dbs_user_info_op_req& db_req, ::google::protobuf::RepeatedPtrField<struct_game_element>& c_rep_game_ele, GS_C_NTF& gs_c_ntf, ::google::protobuf::RepeatedPtrField<user_hero>* c_heros = NULL);

	bool UpdateGameElement(const stGameElement& ele, gs_dbs_user_info_op_req& db_req, ::google::protobuf::RepeatedPtrField<struct_game_element>& c_rep_game_ele, GS_C_NTF& gs_c_ntf, ::google::protobuf::RepeatedPtrField<user_hero>* c_heros = NULL);
	bool UpdateGameElement(const std::vector<stGameElement*> &eles, gs_dbs_user_info_op_req& db_req, ::google::protobuf::RepeatedPtrField<struct_game_element>& c_rep_game_ele, GS_C_NTF& gs_c_ntf, ::google::protobuf::RepeatedPtrField<user_hero>* c_heros = NULL);
	bool UpdateGameElement(const std::vector<stGameElement> &eles, gs_dbs_user_info_op_req& db_req, ::google::protobuf::RepeatedPtrField<struct_game_element>& c_rep_game_ele, GS_C_NTF& gs_c_ntf, ::google::protobuf::RepeatedPtrField<user_hero>* c_heros = NULL);
	
	//bool IsShopOpenForever(CShop::ESHOPTYPE eShopType){
	//	ASSERT(m_shopStrategy);
	//	return m_shopStrategy->IsShopOpenForever(eShopType);
	//}

	bool GetShopInfo(const c_gs_shop_info_req &req, gs_dbs_user_info_op_req &db_req, gs_c_shop_info_res &c_res ){
		ASSERT(m_shopStrategy);
		e_gsc_errorcode r = m_shopStrategy->GetShopInfo(req.pull_type(), req.shop_type(), db_req, *c_res.mutable_shops());
		c_res.set_error_code(r);
		return r== e_gsc_errorcode_success;
	}

	bool ShopBuyItem(CShop::ESHOPTYPE eShopType, UINT32 iPos, gs_dbs_user_info_op_req &db_req, gs_c_shop_buy_item_res &c_res, GS_C_NTF &ntf){
		ASSERT(m_shopStrategy);
		return m_shopStrategy->Buy(eShopType, iPos, db_req, c_res, ntf);
	}

	bool ShopRefresh(CShop::ESHOPTYPE eShopType, gs_dbs_user_info_op_req &db_req, gs_c_shop_refresh_res &c_res, GS_C_NTF& ntf){
		ASSERT(m_shopStrategy);
		return m_shopStrategy->ManualRefreshShop(eShopType, db_req, c_res, ntf);
	}

	bool OnShopRandomOpen(CTask::ETASKTYPE eTaskType, gs_dbs_user_info_op_req &db_req, user_shop &c_shop){
		ASSERT(m_shopStrategy);
		return m_shopStrategy->OnRandomOpen(eTaskType, db_req, c_shop);
	}

	//竞技场
	bool ArenaRefreshMatch(gs_c_arena_refresh_match_res &c_res){
		ASSERT(m_arenaStrategy);
		return m_arenaStrategy->RefreshMatches(c_res);
	}

	bool ArenaUpdateTeam(const c_gs_arena_update_team_req &req, gs_dbs_user_info_op_req &db_req, gs_c_arena_update_team_res &c_res){
		ASSERT(m_arenaStrategy /*&& m_arenaStrategy->HasArenaData()*/);
		if(m_arenaStrategy->HasArenaData()){
			return m_arenaStrategy->UpdateTeam(req, db_req, c_res);
		}
		else{
			c_res.set_error_code(e_gsc_errorcode_unknown_error);
			return false;
		}
	}

	bool ArenaStartChallenge( const c_gs_arena_start_challenge_req& req, gs_dbs_user_info_op_req &db_req, gs_c_arena_start_challenge_res &res ){
		ASSERT(m_arenaStrategy);
		return m_arenaStrategy->StartChallenge(req, db_req, res);
	}

	bool ArenaEndChallenge( const c_gs_arena_end_challenge_req& req, gs_dbs_user_info_op_req &db_req, gs_c_arena_end_challenge_res &res, GS_C_NTF& ntf, gs_c_arena_challenge_ntf &attacker_ntf, gs_c_arena_challenge_ntf &defender_ntf ){
		ASSERT(m_arenaStrategy/* && m_arenaStrategy->HasArenaData()*/);
		if (m_arenaStrategy->HasArenaData()){
			return m_arenaStrategy->EndChallenge(req, db_req, res, ntf, attacker_ntf, defender_ntf);
		}
		else{
			res.set_error_code(e_gsc_errorcode_unknown_error);
			return false;
		}
	}

	bool ArenaResetChallengeCD(const c_gs_arena_challenge_reset_cd_req& req, gs_dbs_user_info_op_req &db_req, gs_c_arena_challenge_reset_cd_res &res){
		ASSERT(m_arenaStrategy /*&& m_arenaStrategy->HasArenaData()*/);
		if (m_arenaStrategy->HasArenaData()){
			return m_arenaStrategy->ResetChallengeCD(req, db_req, res);
		}
		else{
			res.set_error_code(e_gsc_errorcode_unknown_error);
			return false;
		}
	}

	bool ArenaResetChallengeNum(const c_gs_arena_challenge_reset_num_req& req, gs_dbs_user_info_op_req &db_req, gs_c_arena_challenge_reset_num_res &res){
		ASSERT(m_arenaStrategy /*&& m_arenaStrategy->HasArenaData()*/);
		if (m_arenaStrategy->HasArenaData()){
			return m_arenaStrategy->ResetChallengeNum(req, db_req, res);
		}
		else{
			res.set_error_code(e_gsc_errorcode_unknown_error);
			return false;
		}
	}

	bool ArenaRanks(const c_gs_arena_ranks_req& req, gs_c_arena_ranks_res &res){
		ASSERT(m_arenaStrategy);
		return m_arenaStrategy->Ranks(req, res);
	}

	bool ArenaSearch(const c_gs_arena_search_req& req, gs_c_arena_search_res &res){
		ASSERT(m_arenaStrategy);
		return m_arenaStrategy->Search(req, res);
	}

	//增加竞技场积分
	void ArenaAddScore(UINT32 iScore){
		ASSERT(m_arenaStrategy && m_arenaStrategy->HasArenaData());
		return m_arenaStrategy->AddScore(iScore);
	}

	bool ArenaDecreaseScore(UINT32 iScore){
		ASSERT(m_arenaStrategy && m_arenaStrategy->HasArenaData());
		return m_arenaStrategy->DecreaseScore(iScore);
	}

	UINT32 ArenaGetScore(){
		ASSERT(m_arenaStrategy && m_arenaStrategy->HasArenaData());
		return m_arenaStrategy->GetScore();
	}

	//玩家是否在线 在线的话 需要给玩家发通知
	//如果不在线就不发通知了
	void ArenaLeave(bool bUserIsOnline = false);

	//副本进入战斗 调用这个函数之前已经做过判断 开始战斗
	bool StartFight(UINT32 iSubTaskID);

	bool Fill_gs_c_user_info_res(gs_c_user_info_res* res, gs_dbs_user_info_op_req &db_req);

	gs_c_start_sub_task_res& get_start_sub_task_res() { return m_start_sub_task_res; }
	
	bool UpdateTeam(const ::google::protobuf::RepeatedField< ::google::protobuf::uint32 >& heros){
		return m_hero_team.UpdateTeam(heros);
	}

	void ClearTeam(){
		m_hero_team.ClearTeam();
	}
	
	UINT32 GetMissionEventValueByEventType(EEventType eEventType, EEventSubType eEventSubType, EMISSIONTYPE eMissionType){
		ASSERT(m_userMissionStrategy);
		return m_userMissionStrategy->GetMissionEventValueByEventType(eEventType, eEventSubType, eMissionType);
	}
	
	//任务是否已经领取过奖励(EMISSIONTYPE_DAILY,EMISSIONTYPE_ACHIEVEMENT,EMISSIONTYPE_MONTH_CARD_REWARD有效)
	bool IsMissionAlreadyGetReward(UINT32 iMissionID){
		ASSERT(m_userMissionStrategy);
		return m_userMissionStrategy->IsMissionAlreadyGetReward(iMissionID);
	}

	MONTH_CARD_STATUS GetMissionMonthCardStatus(UINT32 iMissionId){
		ASSERT(m_userMissionStrategy);
		return m_userMissionStrategy->GetMissionMonthCardStatus(iMissionId);
	}

	void GetMissionReward(UINT32 iMissionId, gs_dbs_user_info_op_req& db_req, gs_c_get_mission_reward_res& res, GS_C_NTF& ntf){
		ASSERT(m_userMissionStrategy);
		m_userMissionStrategy->GetMissionReward(iMissionId, db_req, res, ntf);
	}

	void TriggerMission(EEventType eEventType, EEventSubType eEventSubType, gs_dbs_user_info_op_req& db_req, GS_C_NTF& ntf, UINT32 iValue = 1){
		ASSERT(m_userMissionStrategy);
		m_userMissionStrategy->TriggerMission(eEventType, eEventSubType, db_req, ntf, iValue);
	}
	
	UINT32 GetEventTodayValue(EEventType eEventType, EEventSubType eEventSubType){
		ASSERT(m_userMissionStrategy);
		return m_userMissionStrategy->GetEventTodayValue(eEventType, eEventSubType);
	}

	//add by baomin for invoke
	UINT32 GetEventTotalValue(EEventType eEventType, EEventSubType eEventSubType){
		ASSERT(m_userMissionStrategy);
		return m_userMissionStrategy->GetEventTotalValue(eEventType, eEventSubType);
	}
	//只更新db和内存， 之前的invoke_sub_none还是保留  相当于祭坛信息保存了两份数据 todo 优化掉！！
	void UpdateEventInfo(EEventType eEventType, EEventSubType eEventSubType, gs_dbs_user_info_op_req& db_req, UINT32 iTriggerTime, UINT32 iValue){
		ASSERT(m_userMissionStrategy);
		m_userMissionStrategy->UpdateEventInfo(eEventType, eEventSubType, db_req, iTriggerTime, iValue);
	}
	
	void ClearEvent(EEventType eEventType, EEventSubType eEventSubType, gs_dbs_user_info_op_req& db_req){
		ASSERT(m_userMissionStrategy);
		m_userMissionStrategy->ClearEvent(eEventType, eEventSubType, db_req);
	}

	//触发玩家统计
	void TriggerUserStatistics(EEventType eEventType, gs_dbs_user_info_op_req& db_req, UINT32 iCurID);
	
	bool TriggerGuidMail(UINT32 iLastLevel, UINT32 iNowLevle, gs_dbs_user_info_op_req& db_req){
		ASSERT(m_userNoviceGuideStrategy);
		return m_userNoviceGuideStrategy->TriggerGuidMail(iLastLevel, iNowLevle, db_req);
	}

	e_gsc_errorcode Sign_1(gs_dbs_user_info_op_req& db_req, gs_c_sign_res& res, GS_C_NTF& ntf){
		ASSERT(m_usersignstrategy);
		return m_usersignstrategy->Sign_1(db_req, res, ntf);
	}

	e_gsc_errorcode GetSignNReward(sign_n_reward::SIGN_N_REWARD_TYPE eType, gs_dbs_user_info_op_req& db_req, gs_c_sign_res& res,  GS_C_NTF& ntf){
		ASSERT(m_usersignstrategy);
		return m_usersignstrategy->GetSignNReward(eType, db_req, res, ntf);
	}
	
	e_gsc_errorcode AddSign(gs_dbs_user_info_op_req& db_req, gs_c_sign_add_res& res, GS_C_NTF& ntf){
		ASSERT(m_usersignstrategy);
		return 	m_usersignstrategy->AddSign(db_req, res, ntf);
	}
	
	void SetHeadOrHeadFrame(HEAD_CHANGE_TYPE change_type, UINT32 iTargetID, gs_dbs_user_info_op_req& db_req, gs_c_head_or_frame_change_res& res);
	
	void FinishNoviceGuide(UINT32 iGuideID, gs_dbs_user_info_op_req& db_req, gs_c_finish_novice_guide_res& res, GS_C_NTF& ntf){
		ASSERT(m_userNoviceGuideStrategy);
		m_userNoviceGuideStrategy->FinishNoviceGuide(iGuideID, db_req, res, ntf);
	}

	void EditPlayerInfo(const c_gs_edit_player_info_req& req, gs_dbs_user_info_op_req& db_req, gs_dbs_edit_name_req& name_req, gs_c_edit_player_info_res& res);

	/************************************************************************/
	/* 
	重置子任务的挑战次数，最大重置次数由vip配置决定， 已重置次数存储在具体的子任务上
	这里不给db和c_res 钻石信息 由调用方给
	*/
	/************************************************************************/
	e_gsc_errorcode ResetSubTaskEnterNum(UINT32 cfg_buy_num_instance, UINT32 sub_task_id, gs_c_buy_res &c_res, gs_dbs_user_info_op_req& db_req, UINT32& rCostDiamond);

	void UserFriendOP(const c_gs_user_friend_op_req& op_req, gs_c_user_friend_op_res& res, gs_dbs_user_info_op_req& db_req, GS_C_NTF& ntf){
		ASSERT(m_userFriendStrategy);
		m_userFriendStrategy->UserFriendOP(op_req, res, db_req, ntf);
	}
	
	void FriendGiveElement(UINT32 iFriendID, GS_C_NTF& ntf){
		ASSERT(m_userFriendStrategy);
		m_userFriendStrategy->FriendGiveElement(iFriendID, ntf);
	}

	void FriendGiveUpApply(UINT32 iFriendID, GS_C_NTF& ntf){
		ASSERT(m_userFriendStrategy);
		m_userFriendStrategy->FriendGiveUpApply(iFriendID, ntf);
	}

	void FriendAffirmAgree(UINT32 iFriendID, GS_C_NTF& ntf){
		ASSERT(m_userFriendStrategy);
		m_userFriendStrategy->FriendAffirmAgree(iFriendID, ntf);
	}

	void FriendAffirmRefuse(UINT32 iFriendID, GS_C_NTF& ntf){
		ASSERT(m_userFriendStrategy);
		m_userFriendStrategy->FriendAffirmRefuse(iFriendID, ntf);
	}
	UINT32 GetFriendAffirmNum(){
		ASSERT(m_userFriendStrategy);
		return m_userFriendStrategy->GetFriendAffirmNum();
	}

	void FriendAddFriend(UINT32 iFriendID, GS_C_NTF& ntf, const friend_base_info& base_info){
		ASSERT(m_userFriendStrategy);
		m_userFriendStrategy->FriendAddFriend(iFriendID, ntf, base_info);
	}
	
	//将其加入到其申请列表中
	void AddFriend(const friend_base_info& base_info){
		ASSERT(m_userFriendStrategy);
		m_userFriendStrategy->AddFriend(base_info);
	}

	void FriendDelFriend(UINT32 iFriendID, GS_C_NTF& ntf){
		ASSERT(m_userFriendStrategy);
		m_userFriendStrategy->FriendDelFriend(iFriendID, ntf);
	}

	void SearchUserRet(const friend_base_info& base_info){
		ASSERT(m_userFriendStrategy);
		m_userFriendStrategy->SearchUserRet(base_info);
	}
	
	void SearchUser( c_gs_search_user_req& req, gs_c_search_user_res& res );

	UINT32 GetFriendNum(){
		ASSERT(m_userFriendStrategy);
		return m_userFriendStrategy->GetFriendNum();
	}

	void AffirmFriend(const ::google::protobuf::RepeatedField< ::google::protobuf::uint32 >& friend_ids){
		ASSERT(m_userFriendStrategy);
		return m_userFriendStrategy->AffirmFriend(friend_ids);
	}
	
	friend_base_info GetBaseInfo();

	void Chat(c_gs_chat_req& req, gs_c_chat_res& res){
		ASSERT(m_userChatStrategy);
		m_userChatStrategy->Chat(req, res);
	}
	
	void CrusadeStart(c_gs_crusade_start_req& req, gs_c_crusade_start_res& res){
		ASSERT(m_userCrusadeStrategy);
		m_userCrusadeStrategy->CrusadeStart(req, res, *m_db_user_info);
	}
	
	void CrusadeEnd(c_gs_crusade_end_req& req, gs_c_crusade_end_res& res, gs_dbs_user_info_op_req& db_req, GS_C_NTF& ntf){
		ASSERT(m_userCrusadeStrategy);
		m_userCrusadeStrategy->CrusadeEnd(req, res, db_req, ntf);
	}
	
	void CrusadeHeroLife(c_gs_crusade_hero_life_req& req, gs_c_crusade_hero_life_res& res, gs_dbs_user_info_op_req& db_req, GS_C_NTF& ntf){
		ASSERT(m_userCrusadeStrategy);
		m_userCrusadeStrategy->CrusadeHeroLife(req, res, db_req, ntf);
	}

	void CrusadeReset(c_gs_crusade_reset_req& req, gs_c_crusade_reset_res& res, gs_dbs_user_info_op_req& db_req){
		ASSERT(m_userCrusadeStrategy);
		m_userCrusadeStrategy->CrusadeReset(req, res, db_req);
	}

	void CrusadePromoteAward(c_gs_crusade_promote_award_req& req, gs_c_crusade_promote_award_res& res, gs_dbs_user_info_op_req& db_req){
		ASSERT(m_userCrusadeStrategy);
		m_userCrusadeStrategy->CrusadePromoteAward(req, res, db_req);
	}

	void CrusadeBuffRefresh(c_gs_crusade_buff_refresh_req& req, gs_c_crusade_buff_refresh_res& res, gs_dbs_user_info_op_req& db_req, GS_C_NTF& ntf){
		ASSERT(m_userCrusadeStrategy);
		m_userCrusadeStrategy->CrusadeBuffRefresh(req, res, db_req, ntf);
	}
private:

	UINT32 _get_health(){
		ASSERT(m_db_user_info);
		return m_db_user_info->mutable_user_base_info()->health_cur();
	}

	void _set_health(UINT32 health){
		ASSERT(m_db_user_info);
		m_db_user_info->mutable_user_base_info()->set_health_cur(health);
	}

	UINT32 _get_health_last_update_time(){
		ASSERT(m_db_user_info);
		return m_db_user_info->mutable_user_base_info()->health_last_update_timestamp();
	}

	void _set_health_last_update_time(UINT32 timestamp){
		ASSERT(m_db_user_info);
		m_db_user_info->mutable_user_base_info()->set_health_last_update_timestamp(timestamp);
	}

	UINT32 _get_skill_point(){
		ASSERT(m_db_user_info);
		return m_db_user_info->mutable_user_base_info()->skill_point();
	}

	void _set_skill_point(UINT32 health){
		ASSERT(m_db_user_info);
		m_db_user_info->mutable_user_base_info()->set_skill_point(health);
	}

	UINT32 _get_skill_point_last_update_time(){
		ASSERT(m_db_user_info);
		return m_db_user_info->mutable_user_base_info()->skill_point_last_update_timestamp();
	}

	void _set_skill_point_last_update_time(UINT32 timestamp){
		ASSERT(m_db_user_info);
		m_db_user_info->mutable_user_base_info()->set_skill_point_last_update_timestamp(timestamp);
	}

	//上一次金币免费召唤当天已经使用的次数
	UINT32 _get_invoke_gold_free_left_num(){
		ASSERT(m_db_user_info);
		return m_db_user_info->mutable_user_base_info()->invoke_gold_free_left_num();
	}

	void _set_invoke_gold_free_left_num(UINT32 num){
		ASSERT(m_db_user_info);
		m_db_user_info->mutable_user_base_info()->set_invoke_gold_free_left_num(num);
	}

	//上一次金币免费召唤的时间
	UINT32 _get_invoke_gold_free_update_timestamp(){
		ASSERT(m_db_user_info);
		return m_db_user_info->mutable_user_base_info()->invoke_gold_free_update_timestamp();
	}

	UINT32 _get_invoke_gold_n_total_num(){
		ASSERT(m_db_user_info);
		return m_db_user_info->mutable_user_base_info()->invoke_gold_n_total_num();
	}

	void _set_invoke_gold_n_total_num(UINT32 total_num){
		ASSERT(m_db_user_info);
		m_db_user_info->mutable_user_base_info()->set_invoke_gold_n_total_num(total_num);
	}

	void _set_invoke_gold_free_update_timestamp(UINT32 timestamp){
		ASSERT(m_db_user_info);
		m_db_user_info->mutable_user_base_info()->set_invoke_gold_free_update_timestamp(timestamp);
	}

	//上一次钻石免费召唤当天已经使用的次数
	UINT32 _get_invoke_diamond_free_left_num(){
		ASSERT(m_db_user_info);
		return m_db_user_info->mutable_user_base_info()->invoke_diamond_free_left_num();
	}

	void _set_invoke_diamond_free_left_num(UINT32 num){
		ASSERT(m_db_user_info);
		m_db_user_info->mutable_user_base_info()->set_invoke_diamond_free_left_num(num);
	}

	//上一次金币免费召唤的时间
	UINT32 _get_invoke_diamond_free_update_timestamp(){
		ASSERT(m_db_user_info);
		return m_db_user_info->mutable_user_base_info()->invoke_diamond_free_update_timestamp();
	}

	void _set_invoke_diamond_free_update_timestamp(UINT32 timestamp){
		ASSERT(m_db_user_info);
		m_db_user_info->mutable_user_base_info()->set_invoke_diamond_free_update_timestamp(timestamp);
	}

	UINT32 _get_invoke_diamond_n_total_num(){
		ASSERT(m_db_user_info);
		return m_db_user_info->mutable_user_base_info()->invoke_diamond_n_total_num();
	}

	void _set_invoke_diamond_n_total_num(UINT32 total_num){
		ASSERT(m_db_user_info);
		m_db_user_info->mutable_user_base_info()->set_invoke_diamond_n_total_num(total_num);
	}


	UINT32 _get_level(){
		ASSERT(m_db_user_info);
		return m_db_user_info->mutable_user_base_info()->level();
	}

	void _set_level(UINT32 level){
		ASSERT(m_db_user_info);
		m_db_user_info->mutable_user_base_info()->set_level(level);
	}

	UINT32 _get_gold(){
		ASSERT(m_db_user_info);
		return m_db_user_info->mutable_user_base_info()->gold();
	}
	
	UINT32 _get_vip_level(){
		ASSERT(m_db_user_info);
		return m_db_user_info->mutable_user_base_info()->vip();
	}

	UINT32 _get_diamond(){
		ASSERT(m_db_user_info);
		return m_db_user_info->mutable_user_base_info()->diamond();
	}
	
	UINT32 _get_head_id(){
		ASSERT(m_db_user_info);
		return m_db_user_info->mutable_user_base_info()->head_id();
	}

	UINT32 _get_head_frame_id(){
		ASSERT(m_db_user_info);
		return m_db_user_info->mutable_user_base_info()->head_frame_id();
	}
	
	const std::string& _get_signature(){
		ASSERT(m_db_user_info);
		return m_db_user_info->mutable_user_base_info()->signature();
	}

	void _set_diamond(UINT32 diamond){
		ASSERT(m_db_user_info);
		m_db_user_info->mutable_user_base_info()->set_diamond(diamond);
	}

	void _set_gold(UINT32 gold){
		ASSERT(m_db_user_info);
		m_db_user_info->mutable_user_base_info()->set_gold(gold);
	}
	
	void _set_friend_point(UINT32 iFriendPoint){
		ASSERT(m_db_user_info);
		m_db_user_info->mutable_user_base_info()->set_friend_point(iFriendPoint);
	}
	
	UINT32 _get_friend_point(){
		ASSERT(m_db_user_info);
		return m_db_user_info->mutable_user_base_info()->friend_point();
	}

	void _set_crusade_point(UINT32 iCrusadePoint){
		ASSERT(m_db_user_info);
		m_db_user_info->mutable_user_base_info()->set_crusade_point(iCrusadePoint);
	}

	UINT32 _get_crusade_point(){
		ASSERT(m_db_user_info);
		return m_db_user_info->mutable_user_base_info()->crusade_point();
	}

	UINT32 _get_exp(){
		ASSERT(m_db_user_info);
		return m_db_user_info->mutable_user_base_info()->exp();
	}
	
	void _set_exp(UINT32 iExp){
		ASSERT(m_db_user_info);
		m_db_user_info->mutable_user_base_info()->set_exp(iExp);
	}

	void _update_element_item_total(const struct_game_element& ele, gs_dbs_user_info_op_req* pDBReq, ::google::protobuf::RepeatedPtrField<struct_game_element>* p_c_rep_game_ele);
	void _update_element_item_increase(const struct_game_element& ele, gs_dbs_user_info_op_req* pDBReq, ::google::protobuf::RepeatedPtrField<struct_game_element>* p_c_rep_game_ele);
	bool _update_element_item_decrease(const struct_game_element& ele, gs_dbs_user_info_op_req* pDBReq, ::google::protobuf::RepeatedPtrField<struct_game_element>* p_c_rep_game_ele);
	//bool _update_game_element(std::vector<stGameElement>& eles);
	void _add_item_to_mail(struct_game_element& gameEle, gs_dbs_user_info_op_req* pDBReq, ::google::protobuf::RepeatedPtrField<struct_game_element>* p_c_rep_game_ele);
	
	db_user_info* m_db_user_info;	//玩家db数据

	gs_c_start_sub_task_res m_start_sub_task_res;//暂存的战斗信息

	CHeroTeam m_hero_team;//队伍信息 gs_c_start_sub_task_req的时候进行更新 并且在给予英雄经验的时候 也是据此 假如EELEMENTTYPE_HERO_EXP上面没有指定hero_id就是根据出阵的英雄队伍的英雄来！

	IUserTaskStrategy* m_taskStrategy;
	IUserHeroStrategy* m_heroStrategy;
	IUserMailStrategy* m_userMailStrategy;	//邮箱
	IBag*		m_pItemBag;		//道具背包(包括碎片普通道具)
	CUserShopStrategy* m_shopStrategy;
	IUserMissionStrategy* m_userMissionStrategy;
	CUserArenaStrategy* m_arenaStrategy;
	IUserSignStrategy* m_usersignstrategy;
	IUserNoviceGuideStrategy* m_userNoviceGuideStrategy;
	IUserFriendStrategy* m_userFriendStrategy;
	IUserChatStrategy* m_userChatStrategy;

	IUserCrusadeStrategy* m_userCrusadeStrategy;
	uv_stream_t* m_connect;	//客户端连接

	//friend class ITaskEnterCondition;//用基类 子类访问不了
	friend class CCondition_Occupation;
	friend class CTaskEnterCondition_NatureProperty;
	friend class CCondition_Area;
};

#endif
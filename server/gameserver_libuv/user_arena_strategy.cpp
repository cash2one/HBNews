#include "user_arena_strategy.h"
#include "user.h"
#include "sys_setting_mgr.h"
#include "arena.h"
#include "vip_mgr.h"
#include "marquee_mgr.h"
bool CUserArenaStrategy::Init( CUser* pUser, const db_user_info& db_user)
{
	ASSERT(pUser);
	m_pUser = pUser;
	//初始化玩家的一些竞技场信息
	//比如三个对手 并且还要发给客户端的！

	////从竞技场中获取玩家信息
	//CArenaContestant* me = CArena::GetInstance()->GetArenaContestantByUserID(pUser->UserID());
	//if(!me){
	//	ASSERT(db_user.has_arena() == false);//没有竞技场信息的
	//	if(db_user.has_arena()){
	//		return false;
	//	}

	//	m_rank = 0;
	//	m_score = 0;
	//	m_history_highest_rank = 0;
	//	m_last_challenge_time = 0;
	//	m_last_day_challenge_num = 0;
	//	m_last_time_buy_arena_challenge = 0;//上次购买挑战次数的时间
	//	m_last_day_buy_arena_challenge_num = 0;//上次当日购买挑战次数的次数，上限在sys_vip表
	//}
	//else{//有竞技场信息
	//	ASSERT(db_user.has_arena() && db_user.arena().IsInitialized());
	//	ASSERT(db_user.arena().logs_size() <= MAXNUM_ARENA_LOG);
	//	//有竞技场信息 并且日志条数符合
	//	if(!db_user.has_arena() || !db_user.arena().IsInitialized() || db_user.arena().logs_size() > MAXNUM_ARENA_LOG){
	//		ASSERT(false);
	//		return false;
	//	}

	//	m_rank = me->m_rank;
	//	m_score = db_user.arena().score();
	//	m_last_challenge_time = db_user.arena().last_challenge_time();
	//	m_last_day_challenge_num = db_user.arena().last_day_challenge_num();
	//	m_history_highest_rank = db_user.arena().history_highest_rank();
	//	m_last_time_buy_arena_challenge = db_user.arena().last_time_buy_arena_challenge();//上次购买挑战次数的时间
	//	m_last_day_buy_arena_challenge_num = db_user.arena().last_day_buy_arena_challenge_num();//上次当日购买挑战次数的次数，上限在sys_vip表
	//	for ( ::google::protobuf::RepeatedPtrField< ::db_user_arena_log >::const_iterator it = db_user.arena().logs().begin() ; it != db_user.arena().logs().end(); ++it){
	//		CArenaLog* pLog = new CArenaLog;
	//		ASSERT(pLog);
	//		bool r = pLog->CopyFrom(*it);
	//		ASSERT(r);
	//		m_log.push_back(pLog);
	//	}
	//}

	if (!db_user.has_arena()){
		m_has_arena_data = false;

		m_rank = 0;
		m_score = 0;
		m_history_highest_rank = 0;
		m_last_challenge_time = 0;
		m_last_day_challenge_num = 0;
		m_last_time_buy_arena_challenge = 0;//上次购买挑战次数的时间
		m_last_day_buy_arena_challenge_num = 0;//上次当日购买挑战次数的次数，上限在sys_vip表

		CArenaContestant* me = CArena::GetInstance()->GetArenaContestantByUserID(pUser->UserID());
		ASSERT(me == NULL);
		if (me != NULL){
			return false;
		}
	}
	else{
		ASSERT(db_user.arena().IsInitialized());
		//ASSERT(db_user.arena().logs_size() <= MAXNUM_ARENA_LOG);
		//有竞技场信息 并且日志条数符合
		if(!db_user.arena().IsInitialized() /*|| db_user.arena().logs_size() > MAXNUM_ARENA_LOG*/){
			ASSERT(false);
			return false;
		}

		m_rank = db_user.arena().rank();

		if (m_rank != 0){
			CArenaContestant* me = CArena::GetInstance()->GetArenaContestantInRankList(m_rank);
			ASSERT(me);
			if (!me){
				return false;
			}
		}
		m_score = db_user.arena().score();
		m_last_challenge_time = db_user.arena().last_challenge_time();
		m_last_day_challenge_num = db_user.arena().last_day_challenge_num();
		m_history_highest_rank = db_user.arena().history_highest_rank();
		m_last_time_buy_arena_challenge = db_user.arena().last_time_buy_arena_challenge();//上次购买挑战次数的时间
		m_last_day_buy_arena_challenge_num = db_user.arena().last_day_buy_arena_challenge_num();//上次当日购买挑战次数的次数，上限在sys_vip表
		for ( ::google::protobuf::RepeatedPtrField< ::db_user_arena_log >::const_iterator it = db_user.arena().logs().begin() ; it != db_user.arena().logs().end(); ++it){
			CArenaLog* pLog = new CArenaLog;
			ASSERT(pLog);
			bool r = pLog->CopyFrom(*it);
			ASSERT(r);
			m_log.push_back(pLog);
		}
		m_has_arena_data = true;
	}

	return true;
}

bool CUserArenaStrategy::RefreshMatches( gs_c_arena_refresh_match_res &res )
{
	e_gsc_errorcode e = GetMatches(*res.mutable_matchs());
	res.set_error_code(e);
	return e == e_gsc_errorcode_success;
}

bool CUserArenaStrategy::UpdateTeam( const c_gs_arena_update_team_req &req, gs_dbs_user_info_op_req &db_req, gs_c_arena_update_team_res &c_res )
{
	if (!m_has_arena_data){
		c_res.set_error_code(e_gsc_errorcode_unknown_error);
		return false;
	}

	//数量判断
	if (req.heros_size() > MAXNUM_HEROINTEAM){
		c_res.set_error_code(e_gsc_errorcode_unknown_error);
		return false;
	}

	//重复性判断
	//会判断英雄存在
	for ( ::google::protobuf::RepeatedField< ::google::protobuf::uint32 >::const_iterator it = req.heros().begin() ; it != req.heros().end() ; ++it){
		UINT32 hero_id = *it;
		if (!hero_id){
			c_res.set_error_code(e_gsc_errorcode_unknown_error);
			return false;
		}
		for ( ::google::protobuf::RepeatedField< ::google::protobuf::uint32 >::const_iterator itIn = it+1 ; itIn != req.heros().end() ; ++itIn){
			if (*itIn == hero_id){
				c_res.set_error_code(e_gsc_errorcode_unknown_error);
				return false;
			}
		}
	}

	std::set<db_user_hero*> heros;
	if(req.heros_size() == 0){
		gs_dbs_user_info_op_req::struct_clear_arena_team* db_team = db_req.mutable_clear_arena_team();
	}
	else{
		gs_dbs_user_info_op_req::struct_op_arena* db_team = db_req.mutable_op_arena();
		db_team->set_op_type(gs_dbs_user_info_op_req::Update);
		//会判断英雄存在
		for ( ::google::protobuf::RepeatedField< ::google::protobuf::uint32 >::const_iterator it = req.heros().begin() ; it != req.heros().end() ; ++it){
			UINT32 hero_id = *it;
			if (!hero_id){
				db_req.set_allocated_op_arena(NULL);
				c_res.set_error_code(e_gsc_errorcode_unknown_error);
				return false;
			}

			db_user_hero* hero = m_pUser->GetHero(hero_id);
			if (!hero){
				db_req.set_allocated_op_arena(NULL);
				c_res.set_error_code(e_gsc_errorcode_unknown_error);
				return false;
			}

			//更新db
			db_team->add_heros(hero_id);
			heros.insert(hero);
		}

		if (db_team->heros_size() == 0){//没有有效英雄的时候 操作
			db_req.set_allocated_op_arena(NULL);
		}
	}
	
	// 英雄都在可以放心 更新gs 竞技场内存数据
	bool r = CArena::GetInstance()->UpdateTeam(m_pUser->UserID(), heros);
	if (!r){
		c_res.set_error_code(e_gsc_errorcode_unknown_error);
		return false;
	}

	c_res.set_error_code(e_gsc_errorcode_success);
	return true;
}

UINT32 CUserArenaStrategy::GetChallengeNum(){
	if (!isSameDay(Now(), m_last_challenge_time, CSysSettingMgr::GetInstance()->Global_Reset_Time)){
		m_last_day_challenge_num = 0;
	}
	return m_last_day_challenge_num;
}

bool CUserArenaStrategy::hasLeftChallengeNum(){
	return GetChallengeNum() < CSysSettingMgr::GetInstance()->Arena_Challenge_Num;
}

//bool CUserArenaStrategy::IsLeftChallengeNumFull(){
//	return GetChallengeNum() == 0;
//}


UINT32 CUserArenaStrategy::GetLeftChallengeNum()
{
	UINT32 num = GetChallengeNum();
	ASSERT(CSysSettingMgr::GetInstance()->Arena_Challenge_Num >= num);
	return CSysSettingMgr::GetInstance()->Arena_Challenge_Num - num;
}

bool CUserArenaStrategy::IncreaseChallengeNum(UINT32 now/*, gs_dbs_user_info_op_req &db_req*/)
{
	if (!hasLeftChallengeNum()){
		return false;
	}

	if (!isSameDay(now, m_last_challenge_time, CSysSettingMgr::GetInstance()->Global_Reset_Time)){
		m_last_day_challenge_num = 1;
	}
	else{
		++m_last_day_challenge_num;
	}

	m_last_challenge_time = now;

	//放在end的时候做 因为这时候 数据库可能还没有记录！
	//db_req.mutable_arena()->set_op_type(gs_dbs_user_info_op_req::Add_Or_Update);
	//db_req.mutable_arena()->set_last_day_challenge_num(m_last_day_challenge_num);
	//db_req.mutable_arena()->set_last_challenge_time(now);

	return true;
}

bool CUserArenaStrategy::StartChallenge( const c_gs_arena_start_challenge_req& req, gs_dbs_user_info_op_req &db_req, gs_c_arena_start_challenge_res &res )
{
	ASSERT(m_pUser);
	if (!m_pUser){
		res.set_error_code(e_gsc_errorcode_unknown_error);
		return false;
	}

	if( m_pUser->GetLevel() < CSysSettingMgr::GetInstance()->User_Level_Function_Arena){
		res.set_error_code(e_gsc_errorcode_level_not_enough);
		return false;
	}

	if ( m_cache_matches.find(req.defender_user_id()) == m_cache_matches.end()){
		res.set_error_code(e_gsc_errorcode_arena_cannot_fight_this_contestant);
		return false;
	}
	
	//判断不是自己打自己
	if (m_pUser->UserID() == req.defender_user_id()){
		res.set_error_code(e_gsc_errorcode_unknown_error);
		return false;
	}
	
	//判断挑战次数
	if (!hasLeftChallengeNum()){
		res.set_error_code(e_gsc_errorcode_arena_no_enough_challenge_num);
		return false;
	}

	UINT32 now = Now();
	//判断是否在cd
	if (diffTime(now, m_last_challenge_time) < CSysSettingMgr::GetInstance()->Arena_Challenge_CD){
		res.set_error_code(e_gsc_errorcode_arena_challenge_in_cd);
		return false;
	}

	CArenaContestant* pMe = CArena::GetInstance()->GetArenaContestantByUserID(m_pUser->UserID());
	if ( pMe && pMe->IsFighting()){//判断自己是否在战斗状态
		res.set_error_code(e_gsc_errorcode_arena_i_am_fighting);
		return false;
	}

	//判断是否在竞技场榜单上
	CArenaContestant* pMatch = CArena::GetInstance()->GetArenaContestantByUserID(req.defender_user_id());
	if ( !pMatch || !pMatch->IsInRankList()){
		res.set_error_code(e_gsc_errorcode_unknown_error);
		return false;
	}

	//判断对手是否已经在战斗状态
	if (pMatch->IsFighting()){
		res.set_error_code(e_gsc_errorcode_arena_match_is_fighting);
		return false;
	}

	//主动挑战但是对手排名却比自己低！ ( 非也->排名已经改变时 排名和玩家对不上) 给予新的对手数据
	if ( m_rank !=0 && m_rank <= pMatch->m_rank){//没有打过竞技场 没有竞技场信息 则rank默认为0
		res.set_error_code(e_gsc_errorcode_arena_contestant_rank_changed);

		//获取新的对手
		e_gsc_errorcode e = GetMatches(*res.mutable_matchs());
		if ( e != e_gsc_errorcode_success){
			res.set_error_code(e);
			return false;
		}

		return false;
	}

	//判断有这些英雄
	std::set<db_user_hero*> heros;
	ASSERT(m_pUser);
	for (  ::google::protobuf::RepeatedField< ::google::protobuf::uint32 >::const_iterator it = req.heros().begin(); it != req.heros().end() ; ++it ){
		db_user_hero* pHero = m_pUser->GetHero(*it);
		if (!pHero){
			res.set_error_code( e_gsc_errorcode_no_this_hero);
			return false;
		}

		heros.insert(pHero);
	}

	//如果自己没有竞技场记录 则新建
	if ( !pMe ){//没有竞技场记录
		//res.set_error_code(e_gsc_errorcode_unknown_error);
		//return false;
		//pMe = CArena::GetInstance()->AddNewContestant(m_pUser->UserID());
		pMe = new CArenaContestant(m_pUser->UserID(), m_pUser->Name(), m_pUser->GetLevel(), m_pUser->GetHeadID(), m_pUser->GetHeadFrameID());
		ASSERT(pMe);
		//pMe->m_user_id = m_pUser->UserID();
		//pMe->m_name = m_pUser->Name();
		//pMe->m_level = m_pUser->GetLevel();

		bool r = pMe->UpdateTeam(heros);
		if (!r){
			delete pMe;
			res.set_error_code(e_gsc_errorcode_unknown_error);
			return false;
		}

		r = CArena::GetInstance()->AddNewContestant(pMe);
		if (!r){
			delete pMe;
			res.set_error_code(e_gsc_errorcode_unknown_error);
			return false;
		}

		if (!m_has_arena_data){
			ASSERT(m_last_challenge_time == 0);
			//db新建放在这里做 db更新操作放在end的时候做！
			//更新或者添加新的记录 在结束的时候才给db 结束有主动结束 和 超时检查结束
			gs_dbs_user_info_op_req::struct_new_arena* db_new_arena = db_req.mutable_new_arena();
			db_new_arena->set_rank(0);
			db_new_arena->set_score(0);
			db_new_arena->set_last_challenge_time(now);	//上一次的挑战时间
			db_new_arena->set_last_day_challenge_num(1);	//last_challenge_time当天已经挑战的次数
			db_new_arena->set_history_highest_rank(0);	//历史最高排名 0表示正无穷 还没有排名
			db_new_arena->mutable_heros()->CopyFrom(req.heros());//防守英雄id， 全量更新
			db_new_arena->set_last_time_buy_arena_challenge(0);
			db_new_arena->set_last_day_buy_arena_challenge_num(0);
			
			m_has_arena_data = true;
		}
	}

	//减少今日可挑战次数, 和最后一次的挑战时间
	bool r = IncreaseChallengeNum(now/*, db_req*/);
	if(!r){
		res.set_error_code(e_gsc_errorcode_arena_no_enough_challenge_num);
		return false;
	}

	//设置两个人不可挑战 和 挑战开始时间！
	//////pMe->is_fighting = true;
	//////pMe->fight_start_time = now;
	//////pMatch->is_fighting = true;
	//////pMatch->fight_start_time = now;
	////pMatch->Fighting(now);
	////pMe->Fighting(now);
	//pMatch->BeFighted(pMe);

	r = pMe->Fight(now, req.power(), req.defender_power(), pMatch);
	ASSERT(r);

	res.set_last_challenge_time(m_last_challenge_time);
	res.set_last_day_challenge_num(GetChallengeNum());
	res.set_error_code(e_gsc_errorcode_success);
	return true;
}

bool CUserArenaStrategy::EndChallenge( const c_gs_arena_end_challenge_req& req, gs_dbs_user_info_op_req &db_req, gs_c_arena_end_challenge_res &res, GS_C_NTF& ntf, gs_c_arena_challenge_ntf &attacker_ntf, gs_c_arena_challenge_ntf &defender_ntf)
{
	res.set_error_code(e_gsc_errorcode_unknown_error);
	//如果自己没有竞技场记录 则新建
	CArenaContestant* pMe = CArena::GetInstance()->GetArenaContestantByUserID(m_pUser->UserID());
	UINT32 now = Now();
	if ( !pMe || !pMe->IsFighting() || !pMe->m_fighting_defender){//没有竞技场记录, 不在战斗状态， 不是主动攻击者 都不能请求结束
		res.set_error_code(e_gsc_errorcode_unknown_error);
		return false;
	}

	//判断是否有竞技场数据
	CArenaContestant* pMatch = pMe->m_fighting_defender;
	//对手存在 对手排名有效 对手正在战斗中 对手是防守者
	if ( !pMatch || !pMatch->IsInRankList() || !pMatch->IsFighting() || pMatch->m_fighting_defender){
		res.set_error_code(e_gsc_errorcode_unknown_error);
		return false;
	}

	//主动挑战但是对手排名却比自己低！ ( 非也->排名已经改变时 排名和玩家对不上) 给予新的对手数据
	if ( m_rank !=0 && m_rank <= pMatch->m_rank){//没有打过竞技场 没有竞技场信息 则rank默认为0
		//res.set_error_code(e_gsc_errorcode_arena_contestant_rank_changed);
		res.set_error_code(e_gsc_errorcode_unknown_error);
		return false;
	}

	bool is_win = req.is_win();
	//判断超时时间 如果超时了 也是输了！
	if (diffTime(now, pMe->m_fight_start_time) >= CSysSettingMgr::GetInstance()->Arena_Fight_Time_Out){
		is_win = false;
	}

	UINT32 iChallengerRank = pMe->m_rank;
	UINT32 iDefenderRank = pMatch->m_rank;

	//利用这个更新挑战者的信息 
	gs_dbs_user_info_op_req::struct_op_arena* db_arena = db_req.mutable_op_arena();
	db_arena->set_op_type(gs_dbs_user_info_op_req::Update);
	ASSERT(m_last_challenge_time == pMe->m_fight_start_time);//如果是重新登陆 再来调用end的时候 这里会断言出来！！
	//不需要去计算是否需要重置
	db_arena->set_last_challenge_time(pMe->m_fight_start_time);	//上一次的挑战时间
	db_arena->set_last_day_challenge_num(m_last_day_challenge_num);	//last_challenge_time当天已经挑战的次数 不需要计算时间
	//db_arena->set_score(m_score);
	//db_arena->mutable_heros()->CopyFrom(req.heros());//防守英雄id， 全量更新
	//bool r = pMe->GetHeros(*db_arena->mutable_heros());
	//ASSERT(r);

	if (is_win){//获胜的时候
		bool r = CArena::GetInstance()->Swap(pMe->m_user_id, pMe->m_rank, pMatch->m_rank);
		if (!r){
			res.set_error_code(e_gsc_errorcode_unknown_error);
			return false;
		}
		res.set_rank(pMe->m_rank);//新的排名
		m_rank = pMe->m_rank;
		db_arena->set_rank(pMe->m_rank);
		
		//历史排名提高时奖励
		//if (/* pMe->m_rank == 0 || */pMe->m_rank < m_history_highest_rank){
		if (m_history_highest_rank == 0 || pMe->m_rank < m_history_highest_rank){
			UINT32 old_history_highest_rank = m_history_highest_rank;
			m_history_highest_rank = pMe->m_rank;
			db_arena->set_history_highest_rank(m_history_highest_rank);	//历史最高排名 0表示正无穷 还没有排名

			//todo 给奖励
			std::vector<stGameElement> output;
			r = CArena::GetInstance()->GetHistoryRankRisePrize(old_history_highest_rank, m_history_highest_rank, output);
			if (!r){
				res.set_error_code(e_gsc_errorcode_unknown_error);
				return false;
			}

			//r = m_pUser->UpdateGameElement(output, db_req, *res.mutable_output(), mail_ntf);
			//if (!r){
			//	res.set_error_code(e_gsc_errorcode_unknown_error);
			//	return false;
			//}

			//发送奖励邮件
			std::map<std::string, CValue> values;
			values["num1"].SetIntValue(m_history_highest_rank);
			r = m_pUser->CreateMail(MAIL_TYPE_ARENA_TOP_RANK, db_req, NULL, &values, &output);
			if (!r){
				res.set_error_code(e_gsc_errorcode_unknown_error);
				return false;
			}
		}

		//竞技场第一名成就任务触发
		if (1 == m_rank)
		{
			m_pUser->TriggerMission(EEventType_Arena_Rank_Top, EEventSubType_None, db_req, ntf);
		}

		//跑马灯(前三名)
		std::vector<UINT32>* vecRank = CMarqueeMgr::GetInstance()->GetTriggerValue(gs_c_marquee_ntf::MARQUEE_ARENA_CHALLENGE);
		ASSERT(vecRank && vecRank->size());
		for (std::vector<UINT32>::iterator it = vecRank->begin(); it != vecRank->end(); ++it)
		{
			std::vector<UINT32>::iterator itf = std::find(vecRank->begin(), vecRank->end(), m_rank);
			if (itf != vecRank->end())
			{
				std::map<std::string, CValue> values;

				CValue& value = values["user_name"];
				value.SetStringValue(m_pUser->Name().c_str()); 

				CMarqueeMgr::GetInstance()->SendMarqueeToAllUsers(gs_c_marquee_ntf::MARQUEE_ARENA_CHALLENGE, values, m_rank);
			}
		}
	}
	else{//主动认输或者超时获胜判输！
		//暂时没啥要做的 后续如果策划要补损失或者防守奖励的话 这里可以做
		res.set_rank(pMe->m_rank);//还是旧的排名
	}

	//------- 注意 上面把玩家和对手的信息已经更新了---------

//利用这个添加日志信息 和 如果需要更新卫冕者的排名
	gs_dbs_user_info_op_req::struct_op_arena_challenge* db_log = db_req.mutable_arena_challenge();
	ASSERT(db_log);
	bool r = MakeLog(is_win, pMe, iChallengerRank, pMatch, iDefenderRank, *db_log, attacker_ntf, defender_ntf);
	ASSERT(r);
	CHECK_FALSE_RETURN_FALSE(r);

	//最后 清理战斗状态 前面要用到这些战斗信息！
	r = pMe->EndFight();
	ASSERT(r);
	CHECK_FALSE_RETURN_FALSE(r);

	r = m_pUser->UpdateGameElement(CArena::GetInstance()->GetEndFightPrize(is_win), db_req, *res.mutable_output(), ntf);
	ASSERT(r);
	CHECK_FALSE_RETURN_FALSE(r);

	res.set_error_code(e_gsc_errorcode_success);
	return true;
}

bool CUserArenaStrategy::MakeLog(bool isAttackerWin, CArenaContestant* pAttacker, UINT32 iChallengerRank, CArenaContestant* pDefender, UINT32 iDefenderRank, gs_dbs_user_info_op_req::struct_op_arena_challenge& db_log, gs_c_arena_challenge_ntf &attacker_ntf, gs_c_arena_challenge_ntf &defender_ntf){
	ASSERT(pAttacker && pDefender && pAttacker->m_fighting_defender == pDefender);
	if (!pAttacker || !pDefender || pAttacker->m_fighting_defender != pDefender){
		ASSERT(false);
		return false;
	}

	db_log.set_time(pAttacker->m_fight_start_time);//挑战发起时间
	db_log.set_is_win(isAttackerWin);//挑战者是否获胜

	attacker_ntf.mutable_ele()->set_is_attacker_win(isAttackerWin);
	attacker_ntf.mutable_ele()->set_time(pAttacker->m_fight_start_time);
	attacker_ntf.mutable_ele()->set_is_me_attacker(true);
	attacker_ntf.mutable_ele()->set_match_user_id(pDefender->m_user_id);
	attacker_ntf.mutable_ele()->set_match_power(pAttacker->m_fighting_power_defender);
	attacker_ntf.mutable_ele()->set_match_name(pDefender->m_name);
	attacker_ntf.mutable_ele()->set_match_level(pDefender->m_level);

	defender_ntf.mutable_ele()->set_is_attacker_win(isAttackerWin);
	defender_ntf.mutable_ele()->set_time(pAttacker->m_fight_start_time);
	defender_ntf.mutable_ele()->set_is_me_attacker(false);
	defender_ntf.mutable_ele()->set_match_user_id(pAttacker->m_user_id);
	defender_ntf.mutable_ele()->set_match_power(pAttacker->m_fighting_power_me);
	defender_ntf.mutable_ele()->set_match_name(pAttacker->m_name);
	defender_ntf.mutable_ele()->set_match_level(pAttacker->m_level);

	if (!isAttackerWin){
		db_log.set_attacker_rank(iChallengerRank);//挑战者排名
		db_log.set_defender_rank(iDefenderRank);//防守者排名

		attacker_ntf.mutable_ele()->set_me_rank(iChallengerRank);
		attacker_ntf.mutable_ele()->set_match_rank(iDefenderRank);

		defender_ntf.mutable_ele()->set_me_rank(iDefenderRank);
		defender_ntf.mutable_ele()->set_match_rank(iChallengerRank);
	}
	else{//排名已经调换过来 因为可能 0  vs  103  变成  0 -> 103   103-> m_cur_rank_len
		db_log.set_attacker_rank(pDefender->m_rank);//挑战者排名 如果挑战者之前为0 赢的时候 就交换m_cur_rank_len给防守者
		db_log.set_defender_rank(pAttacker->m_rank);//防守者排名  

		attacker_ntf.mutable_ele()->set_me_rank(pDefender->m_rank);
		attacker_ntf.mutable_ele()->set_match_rank(pAttacker->m_rank);

		defender_ntf.mutable_ele()->set_me_rank(pAttacker->m_rank);
		defender_ntf.mutable_ele()->set_match_rank(pDefender->m_rank);
	}

	db_log.set_attacker_user_id(pAttacker->m_user_id);
	db_log.set_attacker_power(pAttacker->m_fighting_power_me);
	db_log.set_attacker_name(pAttacker->m_name);
	db_log.set_attacker_level(pAttacker->m_level);
	pAttacker->GetLogHeros(*db_log.mutable_attacker_heros(), *defender_ntf.mutable_ele()->mutable_match_heros());

	db_log.set_defender_user_id(pDefender->m_user_id);
	db_log.set_defender_power(pAttacker->m_fighting_power_defender);
	db_log.set_defender_name(pDefender->m_name);
	db_log.set_defender_level(pDefender->m_level);
	pDefender->GetLogHeros(*db_log.mutable_defender_heros(), *attacker_ntf.mutable_ele()->mutable_match_heros());

	//>>>user_arena用到的
	//required uint32 attacker_last_day_challenge_num = 9;
	//以下为获胜时才用到的值
	//db_log->set_score = 10;//获胜时的积分 目前不获得积分
	//db_log->set_score(0);
	//获胜并且历史排名有提高时才用到 不需要在这里做 sql里面做
	//db_log->set_history_highest_rank = 11;//

	return true;
}

bool CUserArenaStrategy::GetArenaInfo( gs_c_user_info_res& res )
{
	ASSERT(m_pUser);
	CArenaContestant* pMe = CArena::GetInstance()->GetArenaContestantByUserID(m_pUser->UserID());
	if (pMe){//玩家有竞技场信息
		user_arena* c_arena = res.mutable_arena();
		c_arena->set_rank(pMe->m_rank);//排名
		c_arena->set_score(m_score);//竞技场积分
		c_arena->set_last_challenge_time(m_last_challenge_time);//上一次的挑战时间
		c_arena->set_last_day_challenge_num(GetChallengeNum());//last_challenge_time当天已经挑战的次数
		c_arena->set_history_highest_rank(m_history_highest_rank);//历史最高排名
		c_arena->set_last_time_buy_arena_challenge(m_last_time_buy_arena_challenge);
		c_arena->set_last_day_buy_arena_challenge_num(GetChallengeBuyNum(Now()));

		//防守英雄
		pMe->GetHeros(*c_arena->mutable_heros());

		//日志
		for (std::vector<CArenaLog*>::iterator it = m_log.begin(); it != m_log.end(); ++it){
			CArenaLog* p = *it;
			if (p){
				user_arena_log* c_log = c_arena->add_log();
				bool r = p->Fill(*c_log);
				ASSERT(r);
			}
		}

		//对手
		//ASSERT(pMe->m_rank == m_rank);
		if (pMe->m_rank != m_rank){//会出现这种特殊情况：玩家登陆时，db那边从mysql获取到数据之后  gs这边正好已经被人打掉了排名，结果出现不一致的情况
			m_rank = pMe->m_rank;//此时以竞技场排名为准！
		}

		e_gsc_errorcode r = GetMatches(*c_arena->mutable_matchs());
		ASSERT(r == e_gsc_errorcode_success);
		if ( r != e_gsc_errorcode_success){
			return false;
		}
	}

	return true;
}

bool CUserArenaStrategy::ResetChallengeCD( const c_gs_arena_challenge_reset_cd_req& req, gs_dbs_user_info_op_req &db_req, gs_c_arena_challenge_reset_cd_res &res ){
	ASSERT(m_pUser);
	//判断如果不在cd 就不需要刷新
	UINT32 now = Now();
	if (diffTime(now, m_last_challenge_time) >= CSysSettingMgr::GetInstance()->Arena_Fight_Time_Out){
		res.set_error_code(e_gsc_errorcode_arena_challenge_not_in_cd);
		return false;
	}

	UINT32 num = CSysSettingMgr::GetInstance()->Arena_Challenge_CD_Reset_Consume_Diamond;
	//消耗
	if(m_pUser->GetDiamond() < num){
		res.set_error_code(e_gsc_errorcode_not_enough_diamond);
		return false;
	}

	CArenaContestant* pMe = CArena::GetInstance()->GetArenaContestantByUserID(m_pUser->UserID());
	if (!pMe){//玩家有竞技场信息
		res.set_error_code(e_gsc_errorcode_unknown_error);
		return false;
	}

	bool r = m_pUser->DecreaseDiamond(num);
	if (!r){
		res.set_error_code(e_gsc_errorcode_not_enough_diamond);
		return false;
	}

	m_last_challenge_time = now - CSysSettingMgr::GetInstance()->Arena_Fight_Time_Out - 1;//多减一个
	
	//更新时间
	res.set_last_challenge_time(m_last_challenge_time);
	gs_dbs_user_info_op_req::struct_op_arena* db_op_arena = db_req.mutable_op_arena();
	db_op_arena->set_op_type(gs_dbs_user_info_op_req::Update);
	db_op_arena->set_last_challenge_time(m_last_challenge_time);

	//更新钻石
	struct_game_element* c_consume = res.add_consume();
	c_consume->set_type(struct_game_element::EELEMENTTYPE_DIAMOND);
	c_consume->set_id(0);
	c_consume->set_sub_id(0);
	c_consume->set_num_type(struct_game_element::DECREASE);
	c_consume->set_num(num);

	gs_dbs_user_info_op_req::struct_op_base_info* db_base_info = db_req.mutable_base_info();
	db_base_info->set_op_type(gs_dbs_user_info_op_req::Update);
	db_base_info->set_diamond(m_pUser->GetDiamond());

	res.set_error_code(e_gsc_errorcode_success);
	return true;
}

bool CUserArenaStrategy::ResetChallengeNum( const c_gs_arena_challenge_reset_num_req& req, gs_dbs_user_info_op_req &db_req, gs_c_arena_challenge_reset_num_res &res ){
	ASSERT(m_pUser);
	
	if(hasLeftChallengeNum()){
		res.set_error_code(e_gsc_errorcode_arena_challenge_num_not_zero);
		return false;
	}
	
	UINT32 now = Now();
	UINT32 num  = CVipMgr::GetInstance()->GetCostDiamond(EBUY_ARENA_CHALLENGE_NUM, GetChallengeBuyNum(now) + 1);
	//消耗
	if(m_pUser->GetDiamond() < num){
		res.set_error_code(e_gsc_errorcode_not_enough_diamond);
		return false;
	}

	if (!hasLeftChallengeBuyNum(now)){
		res.set_error_code(e_gsc_errorcode_arena_challenge_not_enough_buy_num);
		return false;
	}

	CArenaContestant* pMe = CArena::GetInstance()->GetArenaContestantByUserID(m_pUser->UserID());
	if (!pMe){//玩家有竞技场信息
		res.set_error_code(e_gsc_errorcode_unknown_error);
		return false;
	}

	bool r = m_pUser->DecreaseDiamond(num);
	if (!r){
		res.set_error_code(e_gsc_errorcode_not_enough_diamond);
		return false;
	}
	
	if (!IncreaseChallengeBuyNum(now)){
		res.set_error_code(e_gsc_errorcode_arena_challenge_not_enough_buy_num);
		return false;
	}

	//更新钻石
	struct_game_element* c_consume = res.add_consume();
	c_consume->set_type(struct_game_element::EELEMENTTYPE_DIAMOND);
	c_consume->set_id(0);
	c_consume->set_sub_id(0);
	c_consume->set_num_type(struct_game_element::DECREASE);
	c_consume->set_num(num);

	gs_dbs_user_info_op_req::struct_op_base_info* db_base_info = db_req.mutable_base_info();
	db_base_info->set_op_type(gs_dbs_user_info_op_req::Update);
	db_base_info->set_diamond(m_pUser->GetDiamond());

	//更新挑战信息
	gs_dbs_user_info_op_req::struct_op_arena* db_op_arena = db_req.mutable_op_arena();
	db_op_arena->set_op_type(gs_dbs_user_info_op_req::Update);
	db_op_arena->set_last_day_buy_arena_challenge_num(m_last_day_buy_arena_challenge_num);
	db_op_arena->set_last_time_buy_arena_challenge(m_last_time_buy_arena_challenge);
	db_op_arena->set_last_challenge_time(m_last_challenge_time);
	db_op_arena->set_last_day_challenge_num(m_last_day_challenge_num);

	res.set_last_day_buy_arena_challenge_num(m_last_day_buy_arena_challenge_num);
	res.set_last_time_buy_arena_challenge(m_last_time_buy_arena_challenge);
	res.set_last_challenge_time(m_last_challenge_time);
	res.set_last_day_challenge_num(m_last_day_challenge_num);

	res.set_error_code(e_gsc_errorcode_success);
	return true;
}

bool CUserArenaStrategy::Ranks( const c_gs_arena_ranks_req& req, gs_c_arena_ranks_res &res )
{
	e_gsc_errorcode r = CArena::GetInstance()->GetRanks(req.page_num(), *res.mutable_ranks());
	res.set_error_code(r);
	if ( r != e_gsc_errorcode_success){
		return false;
	}

	return true;
}

bool CUserArenaStrategy::Search(const c_gs_arena_search_req& req, gs_c_arena_search_res &res){
	UINT32 user_id = 0;
	std::string user_name = "";
	if (req.has_user_id()){
		user_id = req.user_id();
	}

	if (req.has_user_name()){
		user_name = req.user_name();
	}

	e_gsc_errorcode r = CArena::GetInstance()->Search(user_id, user_name, res);
	res.set_error_code(r);
	if ( r != e_gsc_errorcode_success){
		return false;
	}

	return true;
}

bool CUserArenaStrategy::hasLeftChallengeBuyNum( UINT32 now )
{
	ASSERT(m_pUser);
	CVipCfg* cfg = CVipMgr::GetInstance()->GetVipCfgByVipLevel(m_pUser->GetVipLevel());
	if (!cfg){
		return false;
	}

	if (!isSameDay(now, m_last_time_buy_arena_challenge, CSysSettingMgr::GetInstance()->Global_Reset_Time)){
		m_last_day_buy_arena_challenge_num = 0;
	}

	return cfg->m_buy_num_arena > m_last_day_buy_arena_challenge_num;
}

bool CUserArenaStrategy::IncreaseChallengeBuyNum( UINT32 now )
{
	ASSERT(m_pUser);
	if (!hasLeftChallengeBuyNum(now)){
		return false;
	}

	++m_last_day_buy_arena_challenge_num;
	m_last_time_buy_arena_challenge = now;

	//会重置挑战信息
	m_last_challenge_time = now - CSysSettingMgr::GetInstance()->Arena_Challenge_CD - 1;
	m_last_day_challenge_num = 0;
	return true;
}

UINT32 CUserArenaStrategy::GetChallengeBuyNum(UINT32 now){
	if (!isSameDay(now, m_last_time_buy_arena_challenge, CSysSettingMgr::GetInstance()->Global_Reset_Time)){
		m_last_day_buy_arena_challenge_num = 0;
	}

	return m_last_day_buy_arena_challenge_num;
}

//bool CUserArenaStrategy::LeaveArena()
//{
//	ASSERT(m_pUser);
//	CHECK_NULL_RETURN_FALSE(m_pUser);
//	//未结束的竞技场判负！
//	c_gs_arena_end_challenge_req req;
//	req.set_is_win(false);
//
//	gs_dbs_user_info_op_req* db_req = new gs_dbs_user_info_op_req;
//	db_req->set_user_id(m_pUser->UserID());
//
//	gs_c_arena_end_challenge_res res;
//	GS_C_NTF ntf;
//	gs_c_arena_challenge_ntf attacker_ntf;
//	gs_c_arena_challenge_ntf defender_ntf;
//	bool r = m_pUser->ArenaEndChallenge(req, *db_req, res, ntf, attacker_ntf, defender_ntf);
//	//send(stream, e_msg_gs_c_arena_end_challenge_res, &res);
//	CServerForUser::GetInstance()->SendDBReq(db_req);
//
//	//if (mail_ntf.mails_size() > 0){
//	//	send(stream, e_msg_gs_c_mail_ntf, &mail_ntf);
//	//}
//	//如果对手在线就给对手发日志
//	UINT32 iDefenderUserID = 0;
//	if(attacker_ntf.IsInitialized()){
//		ASSERT(defender_ntf.IsInitialized());
//		iDefenderUserID = attacker_ntf.ele().match_user_id();
//	}
//
//	if (iDefenderUserID && defender_ntf.IsInitialized()){
//		CUser* pDefender = CUserMgr::GetInstance()->GetUserByUserID(iDefenderUserID);
//
//		if (pDefender){
//			uv_stream_t* con = pDefender->GetConnect();
//			ASSERT(con);
//			CServerForUser::GetInstance()->send(con, e_msg_gs_c_arena_challenge_ntf, &defender_ntf);
//		}
//		//ASSERT(pDefender);
//	}
//
//	return true;
//}
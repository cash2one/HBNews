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
	//��ʼ����ҵ�һЩ��������Ϣ
	//������������ ���һ�Ҫ�����ͻ��˵ģ�

	////�Ӿ������л�ȡ�����Ϣ
	//CArenaContestant* me = CArena::GetInstance()->GetArenaContestantByUserID(pUser->UserID());
	//if(!me){
	//	ASSERT(db_user.has_arena() == false);//û�о�������Ϣ��
	//	if(db_user.has_arena()){
	//		return false;
	//	}

	//	m_rank = 0;
	//	m_score = 0;
	//	m_history_highest_rank = 0;
	//	m_last_challenge_time = 0;
	//	m_last_day_challenge_num = 0;
	//	m_last_time_buy_arena_challenge = 0;//�ϴι�����ս������ʱ��
	//	m_last_day_buy_arena_challenge_num = 0;//�ϴε��չ�����ս�����Ĵ�����������sys_vip��
	//}
	//else{//�о�������Ϣ
	//	ASSERT(db_user.has_arena() && db_user.arena().IsInitialized());
	//	ASSERT(db_user.arena().logs_size() <= MAXNUM_ARENA_LOG);
	//	//�о�������Ϣ ������־��������
	//	if(!db_user.has_arena() || !db_user.arena().IsInitialized() || db_user.arena().logs_size() > MAXNUM_ARENA_LOG){
	//		ASSERT(false);
	//		return false;
	//	}

	//	m_rank = me->m_rank;
	//	m_score = db_user.arena().score();
	//	m_last_challenge_time = db_user.arena().last_challenge_time();
	//	m_last_day_challenge_num = db_user.arena().last_day_challenge_num();
	//	m_history_highest_rank = db_user.arena().history_highest_rank();
	//	m_last_time_buy_arena_challenge = db_user.arena().last_time_buy_arena_challenge();//�ϴι�����ս������ʱ��
	//	m_last_day_buy_arena_challenge_num = db_user.arena().last_day_buy_arena_challenge_num();//�ϴε��չ�����ս�����Ĵ�����������sys_vip��
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
		m_last_time_buy_arena_challenge = 0;//�ϴι�����ս������ʱ��
		m_last_day_buy_arena_challenge_num = 0;//�ϴε��չ�����ս�����Ĵ�����������sys_vip��

		CArenaContestant* me = CArena::GetInstance()->GetArenaContestantByUserID(pUser->UserID());
		ASSERT(me == NULL);
		if (me != NULL){
			return false;
		}
	}
	else{
		ASSERT(db_user.arena().IsInitialized());
		//ASSERT(db_user.arena().logs_size() <= MAXNUM_ARENA_LOG);
		//�о�������Ϣ ������־��������
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
		m_last_time_buy_arena_challenge = db_user.arena().last_time_buy_arena_challenge();//�ϴι�����ս������ʱ��
		m_last_day_buy_arena_challenge_num = db_user.arena().last_day_buy_arena_challenge_num();//�ϴε��չ�����ս�����Ĵ�����������sys_vip��
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

	//�����ж�
	if (req.heros_size() > MAXNUM_HEROINTEAM){
		c_res.set_error_code(e_gsc_errorcode_unknown_error);
		return false;
	}

	//�ظ����ж�
	//���ж�Ӣ�۴���
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
		//���ж�Ӣ�۴���
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

			//����db
			db_team->add_heros(hero_id);
			heros.insert(hero);
		}

		if (db_team->heros_size() == 0){//û����ЧӢ�۵�ʱ�� ����
			db_req.set_allocated_op_arena(NULL);
		}
	}
	
	// Ӣ�۶��ڿ��Է��� ����gs �������ڴ�����
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

	//����end��ʱ���� ��Ϊ��ʱ�� ���ݿ���ܻ�û�м�¼��
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
	
	//�жϲ����Լ����Լ�
	if (m_pUser->UserID() == req.defender_user_id()){
		res.set_error_code(e_gsc_errorcode_unknown_error);
		return false;
	}
	
	//�ж���ս����
	if (!hasLeftChallengeNum()){
		res.set_error_code(e_gsc_errorcode_arena_no_enough_challenge_num);
		return false;
	}

	UINT32 now = Now();
	//�ж��Ƿ���cd
	if (diffTime(now, m_last_challenge_time) < CSysSettingMgr::GetInstance()->Arena_Challenge_CD){
		res.set_error_code(e_gsc_errorcode_arena_challenge_in_cd);
		return false;
	}

	CArenaContestant* pMe = CArena::GetInstance()->GetArenaContestantByUserID(m_pUser->UserID());
	if ( pMe && pMe->IsFighting()){//�ж��Լ��Ƿ���ս��״̬
		res.set_error_code(e_gsc_errorcode_arena_i_am_fighting);
		return false;
	}

	//�ж��Ƿ��ھ���������
	CArenaContestant* pMatch = CArena::GetInstance()->GetArenaContestantByUserID(req.defender_user_id());
	if ( !pMatch || !pMatch->IsInRankList()){
		res.set_error_code(e_gsc_errorcode_unknown_error);
		return false;
	}

	//�ж϶����Ƿ��Ѿ���ս��״̬
	if (pMatch->IsFighting()){
		res.set_error_code(e_gsc_errorcode_arena_match_is_fighting);
		return false;
	}

	//������ս���Ƕ�������ȴ���Լ��ͣ� ( ��Ҳ->�����Ѿ��ı�ʱ ��������ҶԲ���) �����µĶ�������
	if ( m_rank !=0 && m_rank <= pMatch->m_rank){//û�д�������� û�о�������Ϣ ��rankĬ��Ϊ0
		res.set_error_code(e_gsc_errorcode_arena_contestant_rank_changed);

		//��ȡ�µĶ���
		e_gsc_errorcode e = GetMatches(*res.mutable_matchs());
		if ( e != e_gsc_errorcode_success){
			res.set_error_code(e);
			return false;
		}

		return false;
	}

	//�ж�����ЩӢ��
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

	//����Լ�û�о�������¼ ���½�
	if ( !pMe ){//û�о�������¼
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
			//db�½����������� db���²�������end��ʱ������
			//���»�������µļ�¼ �ڽ�����ʱ��Ÿ�db �������������� �� ��ʱ������
			gs_dbs_user_info_op_req::struct_new_arena* db_new_arena = db_req.mutable_new_arena();
			db_new_arena->set_rank(0);
			db_new_arena->set_score(0);
			db_new_arena->set_last_challenge_time(now);	//��һ�ε���սʱ��
			db_new_arena->set_last_day_challenge_num(1);	//last_challenge_time�����Ѿ���ս�Ĵ���
			db_new_arena->set_history_highest_rank(0);	//��ʷ������� 0��ʾ������ ��û������
			db_new_arena->mutable_heros()->CopyFrom(req.heros());//����Ӣ��id�� ȫ������
			db_new_arena->set_last_time_buy_arena_challenge(0);
			db_new_arena->set_last_day_buy_arena_challenge_num(0);
			
			m_has_arena_data = true;
		}
	}

	//���ٽ��տ���ս����, �����һ�ε���սʱ��
	bool r = IncreaseChallengeNum(now/*, db_req*/);
	if(!r){
		res.set_error_code(e_gsc_errorcode_arena_no_enough_challenge_num);
		return false;
	}

	//���������˲�����ս �� ��ս��ʼʱ�䣡
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
	//����Լ�û�о�������¼ ���½�
	CArenaContestant* pMe = CArena::GetInstance()->GetArenaContestantByUserID(m_pUser->UserID());
	UINT32 now = Now();
	if ( !pMe || !pMe->IsFighting() || !pMe->m_fighting_defender){//û�о�������¼, ����ս��״̬�� �������������� �������������
		res.set_error_code(e_gsc_errorcode_unknown_error);
		return false;
	}

	//�ж��Ƿ��о���������
	CArenaContestant* pMatch = pMe->m_fighting_defender;
	//���ִ��� ����������Ч ��������ս���� �����Ƿ�����
	if ( !pMatch || !pMatch->IsInRankList() || !pMatch->IsFighting() || pMatch->m_fighting_defender){
		res.set_error_code(e_gsc_errorcode_unknown_error);
		return false;
	}

	//������ս���Ƕ�������ȴ���Լ��ͣ� ( ��Ҳ->�����Ѿ��ı�ʱ ��������ҶԲ���) �����µĶ�������
	if ( m_rank !=0 && m_rank <= pMatch->m_rank){//û�д�������� û�о�������Ϣ ��rankĬ��Ϊ0
		//res.set_error_code(e_gsc_errorcode_arena_contestant_rank_changed);
		res.set_error_code(e_gsc_errorcode_unknown_error);
		return false;
	}

	bool is_win = req.is_win();
	//�жϳ�ʱʱ�� �����ʱ�� Ҳ�����ˣ�
	if (diffTime(now, pMe->m_fight_start_time) >= CSysSettingMgr::GetInstance()->Arena_Fight_Time_Out){
		is_win = false;
	}

	UINT32 iChallengerRank = pMe->m_rank;
	UINT32 iDefenderRank = pMatch->m_rank;

	//�������������ս�ߵ���Ϣ 
	gs_dbs_user_info_op_req::struct_op_arena* db_arena = db_req.mutable_op_arena();
	db_arena->set_op_type(gs_dbs_user_info_op_req::Update);
	ASSERT(m_last_challenge_time == pMe->m_fight_start_time);//��������µ�½ ��������end��ʱ�� �������Գ�������
	//����Ҫȥ�����Ƿ���Ҫ����
	db_arena->set_last_challenge_time(pMe->m_fight_start_time);	//��һ�ε���սʱ��
	db_arena->set_last_day_challenge_num(m_last_day_challenge_num);	//last_challenge_time�����Ѿ���ս�Ĵ��� ����Ҫ����ʱ��
	//db_arena->set_score(m_score);
	//db_arena->mutable_heros()->CopyFrom(req.heros());//����Ӣ��id�� ȫ������
	//bool r = pMe->GetHeros(*db_arena->mutable_heros());
	//ASSERT(r);

	if (is_win){//��ʤ��ʱ��
		bool r = CArena::GetInstance()->Swap(pMe->m_user_id, pMe->m_rank, pMatch->m_rank);
		if (!r){
			res.set_error_code(e_gsc_errorcode_unknown_error);
			return false;
		}
		res.set_rank(pMe->m_rank);//�µ�����
		m_rank = pMe->m_rank;
		db_arena->set_rank(pMe->m_rank);
		
		//��ʷ�������ʱ����
		//if (/* pMe->m_rank == 0 || */pMe->m_rank < m_history_highest_rank){
		if (m_history_highest_rank == 0 || pMe->m_rank < m_history_highest_rank){
			UINT32 old_history_highest_rank = m_history_highest_rank;
			m_history_highest_rank = pMe->m_rank;
			db_arena->set_history_highest_rank(m_history_highest_rank);	//��ʷ������� 0��ʾ������ ��û������

			//todo ������
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

			//���ͽ����ʼ�
			std::map<std::string, CValue> values;
			values["num1"].SetIntValue(m_history_highest_rank);
			r = m_pUser->CreateMail(MAIL_TYPE_ARENA_TOP_RANK, db_req, NULL, &values, &output);
			if (!r){
				res.set_error_code(e_gsc_errorcode_unknown_error);
				return false;
			}
		}

		//��������һ���ɾ����񴥷�
		if (1 == m_rank)
		{
			m_pUser->TriggerMission(EEventType_Arena_Rank_Top, EEventSubType_None, db_req, ntf);
		}

		//�����(ǰ����)
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
	else{//����������߳�ʱ��ʤ���䣡
		//��ʱûɶҪ���� ��������߻�Ҫ����ʧ���߷��ؽ����Ļ� ���������
		res.set_rank(pMe->m_rank);//���Ǿɵ�����
	}

	//------- ע�� �������ҺͶ��ֵ���Ϣ�Ѿ�������---------

//������������־��Ϣ �� �����Ҫ���������ߵ�����
	gs_dbs_user_info_op_req::struct_op_arena_challenge* db_log = db_req.mutable_arena_challenge();
	ASSERT(db_log);
	bool r = MakeLog(is_win, pMe, iChallengerRank, pMatch, iDefenderRank, *db_log, attacker_ntf, defender_ntf);
	ASSERT(r);
	CHECK_FALSE_RETURN_FALSE(r);

	//��� ����ս��״̬ ǰ��Ҫ�õ���Щս����Ϣ��
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

	db_log.set_time(pAttacker->m_fight_start_time);//��ս����ʱ��
	db_log.set_is_win(isAttackerWin);//��ս���Ƿ��ʤ

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
		db_log.set_attacker_rank(iChallengerRank);//��ս������
		db_log.set_defender_rank(iDefenderRank);//����������

		attacker_ntf.mutable_ele()->set_me_rank(iChallengerRank);
		attacker_ntf.mutable_ele()->set_match_rank(iDefenderRank);

		defender_ntf.mutable_ele()->set_me_rank(iDefenderRank);
		defender_ntf.mutable_ele()->set_match_rank(iChallengerRank);
	}
	else{//�����Ѿ��������� ��Ϊ���� 0  vs  103  ���  0 -> 103   103-> m_cur_rank_len
		db_log.set_attacker_rank(pDefender->m_rank);//��ս������ �����ս��֮ǰΪ0 Ӯ��ʱ�� �ͽ���m_cur_rank_len��������
		db_log.set_defender_rank(pAttacker->m_rank);//����������  

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

	//>>>user_arena�õ���
	//required uint32 attacker_last_day_challenge_num = 9;
	//����Ϊ��ʤʱ���õ���ֵ
	//db_log->set_score = 10;//��ʤʱ�Ļ��� Ŀǰ����û���
	//db_log->set_score(0);
	//��ʤ������ʷ���������ʱ���õ� ����Ҫ�������� sql������
	//db_log->set_history_highest_rank = 11;//

	return true;
}

bool CUserArenaStrategy::GetArenaInfo( gs_c_user_info_res& res )
{
	ASSERT(m_pUser);
	CArenaContestant* pMe = CArena::GetInstance()->GetArenaContestantByUserID(m_pUser->UserID());
	if (pMe){//����о�������Ϣ
		user_arena* c_arena = res.mutable_arena();
		c_arena->set_rank(pMe->m_rank);//����
		c_arena->set_score(m_score);//����������
		c_arena->set_last_challenge_time(m_last_challenge_time);//��һ�ε���սʱ��
		c_arena->set_last_day_challenge_num(GetChallengeNum());//last_challenge_time�����Ѿ���ս�Ĵ���
		c_arena->set_history_highest_rank(m_history_highest_rank);//��ʷ�������
		c_arena->set_last_time_buy_arena_challenge(m_last_time_buy_arena_challenge);
		c_arena->set_last_day_buy_arena_challenge_num(GetChallengeBuyNum(Now()));

		//����Ӣ��
		pMe->GetHeros(*c_arena->mutable_heros());

		//��־
		for (std::vector<CArenaLog*>::iterator it = m_log.begin(); it != m_log.end(); ++it){
			CArenaLog* p = *it;
			if (p){
				user_arena_log* c_log = c_arena->add_log();
				bool r = p->Fill(*c_log);
				ASSERT(r);
			}
		}

		//����
		//ASSERT(pMe->m_rank == m_rank);
		if (pMe->m_rank != m_rank){//��������������������ҵ�½ʱ��db�Ǳߴ�mysql��ȡ������֮��  gs��������Ѿ����˴����������������ֲ�һ�µ����
			m_rank = pMe->m_rank;//��ʱ�Ծ���������Ϊ׼��
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
	//�ж��������cd �Ͳ���Ҫˢ��
	UINT32 now = Now();
	if (diffTime(now, m_last_challenge_time) >= CSysSettingMgr::GetInstance()->Arena_Fight_Time_Out){
		res.set_error_code(e_gsc_errorcode_arena_challenge_not_in_cd);
		return false;
	}

	UINT32 num = CSysSettingMgr::GetInstance()->Arena_Challenge_CD_Reset_Consume_Diamond;
	//����
	if(m_pUser->GetDiamond() < num){
		res.set_error_code(e_gsc_errorcode_not_enough_diamond);
		return false;
	}

	CArenaContestant* pMe = CArena::GetInstance()->GetArenaContestantByUserID(m_pUser->UserID());
	if (!pMe){//����о�������Ϣ
		res.set_error_code(e_gsc_errorcode_unknown_error);
		return false;
	}

	bool r = m_pUser->DecreaseDiamond(num);
	if (!r){
		res.set_error_code(e_gsc_errorcode_not_enough_diamond);
		return false;
	}

	m_last_challenge_time = now - CSysSettingMgr::GetInstance()->Arena_Fight_Time_Out - 1;//���һ��
	
	//����ʱ��
	res.set_last_challenge_time(m_last_challenge_time);
	gs_dbs_user_info_op_req::struct_op_arena* db_op_arena = db_req.mutable_op_arena();
	db_op_arena->set_op_type(gs_dbs_user_info_op_req::Update);
	db_op_arena->set_last_challenge_time(m_last_challenge_time);

	//������ʯ
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
	//����
	if(m_pUser->GetDiamond() < num){
		res.set_error_code(e_gsc_errorcode_not_enough_diamond);
		return false;
	}

	if (!hasLeftChallengeBuyNum(now)){
		res.set_error_code(e_gsc_errorcode_arena_challenge_not_enough_buy_num);
		return false;
	}

	CArenaContestant* pMe = CArena::GetInstance()->GetArenaContestantByUserID(m_pUser->UserID());
	if (!pMe){//����о�������Ϣ
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

	//������ʯ
	struct_game_element* c_consume = res.add_consume();
	c_consume->set_type(struct_game_element::EELEMENTTYPE_DIAMOND);
	c_consume->set_id(0);
	c_consume->set_sub_id(0);
	c_consume->set_num_type(struct_game_element::DECREASE);
	c_consume->set_num(num);

	gs_dbs_user_info_op_req::struct_op_base_info* db_base_info = db_req.mutable_base_info();
	db_base_info->set_op_type(gs_dbs_user_info_op_req::Update);
	db_base_info->set_diamond(m_pUser->GetDiamond());

	//������ս��Ϣ
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

	//��������ս��Ϣ
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
//	//δ�����ľ������и���
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
//	//����������߾͸����ַ���־
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
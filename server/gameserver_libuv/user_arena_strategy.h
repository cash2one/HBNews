#ifndef user_arena_strategy_h__
#define user_arena_strategy_h__

#include "arena.h"

class CUser;
class CUserArenaStrategy{
public:
	bool Init( CUser* pUser, const db_user_info& db_user);

	UINT32 GetRank(){
		return m_rank;
	}

	void SetRank(UINT32 new_rank){
		m_rank = new_rank;
	}

	bool RefreshMatches(gs_c_arena_refresh_match_res &res);
	bool UpdateTeam( const c_gs_arena_update_team_req &req, gs_dbs_user_info_op_req &db_req, gs_c_arena_update_team_res &c_res);
	bool StartChallenge(const c_gs_arena_start_challenge_req& req, gs_dbs_user_info_op_req &db_req, gs_c_arena_start_challenge_res &res);
	bool EndChallenge(const c_gs_arena_end_challenge_req& req, gs_dbs_user_info_op_req &db_req, gs_c_arena_end_challenge_res &res, GS_C_NTF& ntf, gs_c_arena_challenge_ntf &attacker_ntf, gs_c_arena_challenge_ntf &defender_ntf);
	bool ResetChallengeCD(const c_gs_arena_challenge_reset_cd_req& req, gs_dbs_user_info_op_req &db_req, gs_c_arena_challenge_reset_cd_res &res);
	bool ResetChallengeNum(const c_gs_arena_challenge_reset_num_req& req, gs_dbs_user_info_op_req &db_req, gs_c_arena_challenge_reset_num_res &res);
	
	bool Ranks(const c_gs_arena_ranks_req& req, gs_c_arena_ranks_res &res);
	//bool Fill(user_arena &c_arena){
	//	ASSERT(m_pUser);
	//	c_arena.
	//}

	bool Search(const c_gs_arena_search_req& req, gs_c_arena_search_res &res);

	bool GetArenaInfo(gs_c_user_info_res& res);

	//增加竞技场积分
	void AddScore(UINT32 iScore){
		m_score += iScore;
	}

	UINT32 GetScore(){
		return m_score;
	}

	bool DecreaseScore(UINT32 iScore){
		if (m_score < iScore){
			return false;
		}

		m_score -= iScore;
		return true;
	}

	//db中是否有竞技场数据
	bool HasArenaData(){
		return m_has_arena_data;
	}

	//挑战方被动立场 如超时
	//bool LeaveArena();
private:

	e_gsc_errorcode GetMatches(::google::protobuf::RepeatedPtrField< ::user_arena_contestant > &c_matches){
		ASSERT(c_matches.size() == 0);
		if (c_matches.size() > 0){
			return e_gsc_errorcode_unknown_error;
		}
		std::set<CArenaContestant*> matches;
		bool r = CArena::GetInstance()->GetMatches(GetRank(), matches);
		if (!r || matches.size() > MAXNUM_ARENA_MATCH){
			return e_gsc_errorcode_unknown_error;
		}

		m_cache_matches.clear();
		for (std::set<CArenaContestant*>::iterator it = matches.begin(); it != matches.end() ; ++it){
			CArenaContestant* PContestant = *it;
			ASSERT(PContestant);
			if (PContestant){
				user_arena_contestant* c_contestant = c_matches.Add();
				bool r = PContestant->Fill(*c_contestant);
				if (!r){
					m_cache_matches.clear();
					return e_gsc_errorcode_unknown_error;
				}
				m_cache_matches.insert(PContestant->m_user_id);
			}
		}

		return e_gsc_errorcode_success;
	}

	bool hasLeftChallengeNum();
	UINT32 GetChallengeNum();
	UINT32 GetLeftChallengeNum();
	//bool IsLeftChallengeNumFull();//剩余挑战次数是满的
	bool IncreaseChallengeNum(UINT32 now/*, gs_dbs_user_info_op_req &db_req*/);

	//判断重置次数是否还有
	bool hasLeftChallengeBuyNum(UINT32 now);
	bool IncreaseChallengeBuyNum(UINT32 now);
	UINT32 GetChallengeBuyNum(UINT32 now);

	bool MakeLog(bool isAttackerWin, CArenaContestant* pAttacker, UINT32 iChallengerRank, CArenaContestant* pDefender, UINT32 iDefenderRank, gs_dbs_user_info_op_req::struct_op_arena_challenge& db_log, gs_c_arena_challenge_ntf &attacker_ntf, gs_c_arena_challenge_ntf &defender_ntf);
	
	CUser* m_pUser;
	
	bool m_has_arena_data;//db上是否有此玩家的竞技场记录
	UINT32 m_rank;//排名
	UINT32 m_score ;//竞技场积分
	UINT32 m_last_challenge_time ;//上一次主动挑战的时间
	UINT32 m_last_day_challenge_num ;//last_challenge_time当天已经主动挑战的次数
	UINT32 m_history_highest_rank ;//历史最高排名
	UINT32 m_last_time_buy_arena_challenge;//上次购买挑战次数的时间
	UINT32 m_last_day_buy_arena_challenge_num;//上次当日购买挑战次数的次数，上限在sys_vip表
	//UINT32 heros = 6;//出场英雄id 已经存到竞技场中
	
	//UINT32 m_cur_figinting_match;//目前正在进行战斗的对手 这个需要放在竞技场中 而不是这里！ 因为玩家即使不上线 也需要

	//std::vector<CArenaContestant*> m_matches;//对手们 既不需要存在竞技场上 也不需要存到玩家身上 每次要用就根据规则随机给
	std::vector<CArenaLog*> m_log;//战斗日志 不需要存在竞技场上 存到玩家身上 策划案中说最多十条
	
	std::set<UINT32> m_cache_matches;//对手用户id 挑战的时候需要比对是否在这个里面 而不能随便打！
};

#endif // user_arena_strategy_h__
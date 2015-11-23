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

	//���Ӿ���������
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

	//db���Ƿ��о���������
	bool HasArenaData(){
		return m_has_arena_data;
	}

	//��ս���������� �糬ʱ
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
	//bool IsLeftChallengeNumFull();//ʣ����ս����������
	bool IncreaseChallengeNum(UINT32 now/*, gs_dbs_user_info_op_req &db_req*/);

	//�ж����ô����Ƿ���
	bool hasLeftChallengeBuyNum(UINT32 now);
	bool IncreaseChallengeBuyNum(UINT32 now);
	UINT32 GetChallengeBuyNum(UINT32 now);

	bool MakeLog(bool isAttackerWin, CArenaContestant* pAttacker, UINT32 iChallengerRank, CArenaContestant* pDefender, UINT32 iDefenderRank, gs_dbs_user_info_op_req::struct_op_arena_challenge& db_log, gs_c_arena_challenge_ntf &attacker_ntf, gs_c_arena_challenge_ntf &defender_ntf);
	
	CUser* m_pUser;
	
	bool m_has_arena_data;//db���Ƿ��д���ҵľ�������¼
	UINT32 m_rank;//����
	UINT32 m_score ;//����������
	UINT32 m_last_challenge_time ;//��һ��������ս��ʱ��
	UINT32 m_last_day_challenge_num ;//last_challenge_time�����Ѿ�������ս�Ĵ���
	UINT32 m_history_highest_rank ;//��ʷ�������
	UINT32 m_last_time_buy_arena_challenge;//�ϴι�����ս������ʱ��
	UINT32 m_last_day_buy_arena_challenge_num;//�ϴε��չ�����ս�����Ĵ�����������sys_vip��
	//UINT32 heros = 6;//����Ӣ��id �Ѿ��浽��������
	
	//UINT32 m_cur_figinting_match;//Ŀǰ���ڽ���ս���Ķ��� �����Ҫ���ھ������� ��������� ��Ϊ��Ҽ�ʹ������ Ҳ��Ҫ

	//std::vector<CArenaContestant*> m_matches;//������ �Ȳ���Ҫ���ھ������� Ҳ����Ҫ�浽������� ÿ��Ҫ�þ͸��ݹ��������
	std::vector<CArenaLog*> m_log;//ս����־ ����Ҫ���ھ������� �浽������� �߻�����˵���ʮ��
	
	std::set<UINT32> m_cache_matches;//�����û�id ��ս��ʱ����Ҫ�ȶ��Ƿ���������� ����������
};

#endif // user_arena_strategy_h__
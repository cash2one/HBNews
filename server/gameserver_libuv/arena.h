#ifndef arena_h__
#define arena_h__

#include <string>
#include <vector>
#include <set>
#include "config.h"

#include "dbs_gs_proto.pb.h"

//�������е�Ӣ������ ���ﲻ����user_hero ���½����ݽṹ����Ϊ�ò���һЩ�ֶ�
struct CArenaHero{
	struct CArenaHeroEquip{
		UINT32 equip_id;
		UINT32 level;

		bool Fill(user_hero_equip &c_equip){
			ASSERT(equip_id && level);
			c_equip.set_equip_id(equip_id);
			c_equip.set_level(level);
			return true;
		}

		bool CopyFrom(const db_user_hero_equip &db_equip){
			equip_id = db_equip.equip_id();
			level = db_equip.level();
			return true;
		}
	};

	struct CArenaHeroSkill{
		UINT32 bisha_level;
		UINT32 aoyi_level;
		UINT32 passive1_level;
		UINT32 passive2_level;
		UINT32 passive3_level;

		CArenaHeroSkill(){
			memset(this, 0, sizeof CArenaHeroSkill);
		}

		bool Fill(::google::protobuf::RepeatedPtrField< ::user_hero_skill > &c_skills);

		bool CopyFrom(const ::google::protobuf::RepeatedPtrField< ::db_user_hero_skill > &db_skills);
	};

	UINT32 hero_id;
	UINT32 star;
	UINT32 quality;
	UINT32 level;

	CArenaHeroEquip* equips[EEQUIPTYPE_MAX - 1];//
	//CArenaHeroSkill* skills[ESKILLTYPE_MAX - 1];//
	CArenaHeroSkill skills;//

	CArenaHero(){
		memset(equips, 0 , sizeof equips);
		Clean();
	}

	~CArenaHero(){
		Clean();
	}

	void Clean();

	UINT32 GetNextEmptyEquipPos();

	bool Fill(user_hero& c_hero);

	bool CopyFrom(const db_user_hero& db_hero);
};

//����������ѡ�� ���ϲ��ϰ�û�й�ϵ
struct CArenaContestant{

	//��Ҫֱ�Ӵ洢��������Ϣ
	UINT32 m_rank;	//����
	UINT32 m_user_id;	//�û�id
	//UINT32 power;//ս���� ����Ҫ
	
	//��Ҫ������������� �������ڴ����Ϣ
	std::string m_name;//����
	UINT32 m_level;	//�ȼ� ��Ҫ��ʾ��
	UINT32 m_iHeadID;	//ͷ��id
	UINT32 m_iHeadFrameID;	//ͷ����id
	//std::vector<user_hero*>  m_defend_heros;//����Ӣ��
	CArenaHero* m_defend_heros[MAXNUM_HEROINTEAM];//����Ӣ��
	
	//>>> ��Щ��Ϣ������������ϣ� ���ж�����Ҳ�ǲ���Ҫ�洢���������Ϣ
	//UINT32 score;	//��Ҿ���������
	//UINT32 last_challenge_time
	//UINT32 last_day_challenge_num
	//UINT32 history_highest_rank

	//��̬��Ϣ
	//>>> ս����Ϣ ���������ߺ� ���������߶���Ҫ
	bool m_is_fighting;//�Ƿ�����ս���� �����������Ǳ���
	UINT32 m_fight_start_time;//ս����ʼ��ʱ�� ����ͻ���һֱ������ ��ô��������߻ᶨʱ�����������Ҽ�¼��ս��־
	//����Ͳ������� ֻ������ս�߶�����ֶ� ������ʱ����ʱ��ʱ�� ֻ�����һ����¼
	//ͬʱ ��Ϊ����������Ҳ���������� ������Ҫ�õ����ֵ���Ϣ
		//>>>ֻ��Ҫ������������Ҫ����
	CArenaContestant* m_fighting_defender;//�������ŵ���ս�� ��ս�ߵ�����ֶ���Ҫ����! ����ս����������ֶβ���Ҫ����! ��������õĻ� ���ڵ�ʱ�� �ͻ��������log û��Ҫ
	UINT32 m_fighting_power_me;			//ս������ʱ �ң���ս�ߣ���ս����
	UINT32 m_fighting_power_defender;	//ս������ʱ ���ˣ������ߣ���ս����
	//<<< ս����Ϣ

	CArenaContestant(UINT32 iUserID, std::string& strName, UINT32 iLevel, UINT32 iHeadID, UINT32 iHeadFrameID) : m_fighting_defender(NULL){
		memset(m_defend_heros, 0, sizeof m_defend_heros);
		Clean();
		m_user_id = iUserID;
		m_name = strName;
		m_level = iLevel;
		m_iHeadID = iHeadID;
		m_iHeadFrameID = iHeadFrameID;
	}

	~CArenaContestant(){
		Clean();
	}

	void Clean();

	void CleanHero();

	bool IsInRankList(){
		return m_rank != 0;
	}

	bool IsFighting(){
		return m_is_fighting;
	}

	//����ս����������ս��
	bool IsFightingAndAttacker(){
		return m_is_fighting && (m_fighting_defender != NULL);
	}

	//ֻ��������ս����ս���ܵ��ã�
	bool IsAttackerFightingTimeOut();
	//void Fighting(UINT32 now){
	//	ASSERT(m_is_fighting == false);
	//	m_is_fighting = true;
	//	m_fight_start_time = now;
	//}

	//bool BeFighted(UINT32 when, CArenaContestant* pChallenger){
	//	ASSERT(pChallenger && !IsFighting());
	//	if (!pChallenger || IsFighting()){
	//		return false;
	//	}

	//	m_is_fighting = true;
	//	m_fight_start_time = when;
	//	pChallenger->m_fighting_defender = NULL;
	//	
	//	pChallenger->m_is_fighting = true;
	//	pChallenger->m_fight_start_time = when;
	//	pChallenger->m_fighting_defender = this;
	//}

	bool Fight(UINT32 when, UINT32 iMyPower, UINT32 iDefenderPower, CArenaContestant* pDefender){
		ASSERT(!IsFighting() && pDefender && !pDefender->IsFighting());
		if (IsFighting() || !pDefender || pDefender->IsFighting()){
			return false;
		}

		m_is_fighting = true;
		m_fight_start_time = when;
		m_fighting_defender = pDefender;
		m_fighting_power_me = iMyPower;
		m_fighting_power_defender = iDefenderPower;

		pDefender->m_is_fighting = true;
		pDefender->m_fight_start_time = when;
		pDefender->m_fighting_defender = NULL;
		pDefender->m_fighting_power_me = 0;
		pDefender->m_fighting_power_defender = 0;

		return true;
	}

	//����ս�� ֻ����ս�ߵ��ã� ˭����˭����
	bool EndFight(){
		//ASSERT(m_is_fighting && m_fight_start_time/* && m_fighting_defender*/);
		ASSERT(IsFighting());
		//if (!m_is_fighting || !m_fight_start_time/* || !m_fighting_defender*/){
		if (!IsFighting()){
			return false;
		}

		//Ҫô�Լ������������ ����Լ�����ս�� ��������˲�����ս��
		ASSERT( !m_fighting_defender || (m_fighting_defender && !m_fighting_defender->m_fighting_defender));
		if(m_fighting_defender && m_fighting_defender->m_fighting_defender){
			return false;
		}

		m_is_fighting = false;
		m_fight_start_time = 0;
		m_fighting_power_me = 0;
		m_fighting_power_defender = 0;
		bool r = false;
		if (m_fighting_defender){
			ASSERT(m_fighting_defender->m_fighting_defender == NULL);
			r = m_fighting_defender->EndFight();
			m_fighting_defender = NULL;
		}
		else{
			r = true;
		}

		return r;
	}

	UINT32 GetNextEmptyHeroPos();

	CArenaHero* GetHero(UINT32 iHeroID);

	bool Fill(user_arena_contestant& c_contestant);

	bool UpdateTeam(std::set<db_user_hero*> &heros);
	
	bool GetHeros(::google::protobuf::RepeatedField< ::google::protobuf::uint32 > & r_heros){
		for (UINT32 i = 0; i < MAXNUM_HEROINTEAM ; ++i){
			CArenaHero* hero = m_defend_heros[i];
			if (hero){
				ASSERT(hero->hero_id);
				UINT32* pHeroID = r_heros.Add();
				*pHeroID = hero->hero_id;
			}
		}

		return r_heros.size() > 0 ;
	}

	bool GetLogHeros(::google::protobuf::RepeatedPtrField< db_user_arena_log::db_user_arena_log_hero > & db_heros, ::google::protobuf::RepeatedPtrField< user_arena_log::user_arena_log_hero > & c_heros){
		for (UINT32 i = 0; i < MAXNUM_HEROINTEAM ; ++i){
			CArenaHero* hero = m_defend_heros[i];
			if (hero){
				ASSERT(hero->hero_id);
				db_user_arena_log::db_user_arena_log_hero* db_hero = db_heros.Add();
				db_hero->set_hero_id(hero->hero_id);
				db_hero->set_quality(hero->quality);
				db_hero->set_level(hero->level);
				db_hero->set_star(hero->star);

				user_arena_log::user_arena_log_hero* c_hero = c_heros.Add();
				c_hero->set_hero_id(hero->hero_id);
				c_hero->set_quality(hero->quality);
				c_hero->set_level(hero->level);
				c_hero->set_star(hero->star);
			}
		}

		return true;
	}
};

struct CArenaLog{
	//UINT32 attacker_user_id;//��ս��user_id
	//UINT32 attacker_rank;//��ս������
	//UINT32 attacker_power;//��ս�ߴ˴���ս��ս����
	//UINT32 defender_user_id;//������user_id
	//UINT32 defender_rank;//����������
	//UINT32 defender_power;//������ս����
	//bool is_win;//��ս���Ƿ��ʤ
	//UINT32 time;//��ս����ʱ��
	bool is_attacker_win;//��ս���Ƿ��ʤ
	UINT32 time;//��ս����ʱ��

	//�ҵ���Ϣ
	bool is_me_attacker;// ���Ƿ�����ս������
	UINT32 me_rank;//�ҵ�����

	//��ʾ������Ϣ
	UINT32 match_user_id;
	UINT32 match_rank;
	UINT32 match_power;
	std::string match_name;
	UINT32 match_level;

	struct CArenaLogHero{
		UINT32 hero_id;//id
		UINT32 quality;//Ʒ��
		UINT32 level;
		UINT32 star;
	};

	std::vector<CArenaLogHero*> match_heros;//���ֵĳ���Ӣ����Ϣ

	CArenaLog(){

	}

	~CArenaLog(){
		_CleanMatchHeros();
	}

	void _CleanMatchHeros(){
		for (std::vector<CArenaLogHero*>::iterator it = match_heros.begin() ; it != match_heros.end(); ++it){
			delete *it;// delete NULL�ǰ�ȫ��
		}
		match_heros.clear();
	}

	bool Fill(user_arena_log& c_log){
		c_log.set_is_attacker_win(is_attacker_win);
		c_log.set_time(time);
		c_log.set_is_me_attacker(is_me_attacker);
		c_log.set_me_rank(me_rank);

		c_log.set_match_user_id(match_user_id);
		c_log.set_match_rank(match_rank);
		c_log.set_match_power(match_power);
		c_log.set_match_name(match_name);
		c_log.set_match_level(match_level);

		for (std::vector<CArenaLogHero*>::iterator it = match_heros.begin() ; it != match_heros.end(); ++it){
			CArenaLogHero* p = *it;
			if (p){
				user_arena_log::user_arena_log_hero* c_hero = c_log.add_match_heros();
				c_hero->set_hero_id(p->hero_id);
				c_hero->set_quality(p->quality);
				c_hero->set_level(p->level);
				c_hero->set_star(p->star);
			}
		}

		return true;
	}

	bool CopyFrom(const db_user_arena_log& db_log){
		//attacker_user_id = db_log.attacker_user_id();
		//attacker_rank = db_log.attacker_rank();//��ս������
		//attacker_power = db_log.attacker_power();//��ս�ߴ˴���ս��ս����
		//defender_user_id = db_log.defender_user_id();//������user_id
		//defender_rank = db_log.defender_rank();//����������
		//defender_power = db_log.defender_power();//������ս����
		//is_win = db_log.is_win();//��ս���Ƿ��ʤ
		//time = db_log.time();

		is_attacker_win = db_log.is_attacker_win();//��ս���Ƿ��ʤ
		time = db_log.time();//��ս����ʱ��

		//�ҵ���Ϣ
		is_me_attacker = db_log.is_me_attacker();// ���Ƿ�����ս������
		me_rank = db_log.me_rank();//�ҵ�����

		//��ʾ������Ϣ
		match_user_id = db_log.match_user_id();
		match_rank = db_log.match_rank();
		match_power = db_log.match_power();
		match_name = db_log.match_name();
		match_level = db_log.match_level();

		_CleanMatchHeros();

		for ( ::google::protobuf::RepeatedPtrField< ::db_user_arena_log::db_user_arena_log_hero >::const_iterator it = db_log.match_heros().begin() ; it != db_log.match_heros().end() ; ++it){
			CArenaLogHero* p = new CArenaLogHero;
			p->hero_id = it->hero_id();
			p->quality = it->quality();
			p->level = it->level();
			p->star = it->star();
			match_heros.push_back(p);
		}

		return true;
	}
};

class CArena{
public:
	//todo ��������
	inline static CArena* GetInstance(){
		static CArena singleton;
		return &singleton;
	}

	bool Init();

	void UnInit(){
		for (UINT32 i = 0; i < MAXNUM_ARENA_RANK ; ++i){
			if (m_rank[i]){
				delete m_rank[i];
				m_rank[i] = NULL;
			}
		}
		memset(m_rank, 0, sizeof m_rank);


		for (std::map<UINT32, CArenaContestant*>::iterator it = m_mapOtherContestant.begin() ; it != m_mapOtherContestant.end(); ++it){
			if (it->second){
				delete it->second;
				it->second = NULL;
			}
		}

		m_mapOtherContestant.clear();
	}

	//CArenaContestant* AddNewContestant(UINT32 user_id){
	//	CArenaContestant* p = new CArenaContestant;
	//	p->m_rank = m_cur_rank_len;
	//	m_rank[m_cur_rank_len++] = p;
	//}

	bool AddNewContestant(CArenaContestant* p){
		//CArenaContestant* p = new CArenaContestant;
		ASSERT(p);
		if (!p){
			return false;
		}

		p->m_rank = 0;//����rank_list�����Ƿ���� ��������������rank���� ������end��ʱ��

		//��������δ�� �����ȵ�other �ɹ�����swap��ranklist��
		////�������������� ���뵽othermap��
		//if (m_cur_rank_len >= MAXNUM_ARENA_RANK){
		//	std::pair<std::map<UINT32, CArenaContestant*>::iterator, bool> r = m_mapOtherContestant.insert(std::make_pair(p->m_user_id, p));

		//	//return false;
		//}
		//else{//��δ��
		//	//p->m_rank = m_cur_rank_len;
		//	m_rank[m_cur_rank_len++] = p;
		//	//return true;
		//}

		//ֱ��ȫ�������뵽�����б���
		std::pair<std::map<UINT32, CArenaContestant*>::iterator, bool> r = m_mapOtherContestant.insert(std::make_pair(p->m_user_id, p));
		return r.second;
	}

	//bool IsValidContestant(UINT32 iCurRank/*, UINT32 iHistoryHighestRank*/){
	//	return /*iHistoryHighestRank != 0 &&*/ iCurRank != 0;
	//}

	//������1��ʼ ������1��ʼ
	CArenaContestant* GetArenaContestantInRankList(UINT32 rank){
		ASSERT(rank >=1 && rank <= MAXNUM_ARENA_RANK);
		if (rank >=1 && rank <= MAXNUM_ARENA_RANK){
			return m_rank[rank-1];
		}

		return NULL;
	}

	//���뵽����ָ��λ��
	CArenaContestant* ReplaceIntoRankList(UINT32 rank, CArenaContestant* p){
		return NULL;
	}

	CArenaContestant* GetArenaContestantByUserID(UINT32 iUserID);

	//�½���Ҽ�¼�ĵ� �ڸ��¶��飡 ����ˢ�� Ҳ����ս����
	bool UpdateTeam(UINT32 iUserID, std::set<db_user_hero*> &heros);

	CArenaHero* GetHero(UINT32 rank, UINT32 hero_id);

	//todo ���������
	bool GetMatches(UINT32 irank, std::set<CArenaContestant*> &ret_matches);
	bool GetMatches( UINT32 irank, ::google::protobuf::RepeatedPtrField< ::user_arena_contestant > &ret_matches );

	//bool Fill(UINT32 iUserID, user_arena &c_arena){
	//	ASSERT(iUserID);
	//	if (!iUserID){
	//		return false;
	//	}

	//	CArenaContestant* p = GetArenaContestantByUserID(iUserID);
	//	if (!p){
	//		return false;
	//	}

	//	p->f

	//}

	

	//bool Swap(UINT32 iRank1, UINT32 iRank2){
	//	if (iRank1 == 0 || iRank1 >= m_cur_rank_len || iRank2 == 0 || iRank2 >= m_cur_rank_len){
	//		return false;
	//	}

	//	CArenaContestant* p1 = m_rank[iRank1];
	//	CArenaContestant* p2 = m_rank[iRank2];

	//	ASSERT( p1 && p2 && p1 != p2);
	//	ASSERT(p1->m_rank == iRank1);
	//	ASSERT(p2->m_rank == iRank2);
	//	if (!p1 || !p2 || p1 == p2 || p1->m_rank != iRank1 || p2->m_rank != iRank2){//�ӳٵ������ж������Բ��ԣ�
	//		return false;
	//	}

	//	std::swap(p1->m_rank, p2->m_rank);
	//	std::swap(m_rank[iRank1], m_rank[iRank2]);

	//	return true;
	//}

	//2����� 1. ��ranklist ��other map�н��� 2. ��ranklist�ڲ�����
	//������other map�ڲ����� ��Ϊ���������������ս��
	bool Swap(UINT32 iChallengeUserID, UINT32 iChallengeRank, UINT32 iDefenderRank){
		
		/*
		ASSERT( (iChallengeRank == 0 || iChallengeRank > iDefenderRank) && iDefenderRank > 0 && iDefenderRank < m_cur_rank_len && iChallengeRank >= 0 && iChallengeRank < m_cur_rank_len);
		//���ж� �ų��� iChallengeRank == iDefenderRank == 0�Ĳ��������
		//�ų��� ��ս�����α����������λ��ߵĲ��������
		//�ų�iChallengeRank �� iDefenderRank������Ч������
		if (  (iChallengeRank != 0 && iChallengeRank <= iDefenderRank) 
			|| iDefenderRank <= 0 || iDefenderRank >= m_cur_rank_len
			|| iChallengeRank < 0 || iChallengeRank >= m_cur_rank_len
			){
			return false;
		}
		*/

		bool valid = (iChallengeRank == 0 || iChallengeRank > iDefenderRank) && iDefenderRank > 0 && iDefenderRank < m_cur_rank_len && iChallengeRank >= 0 && iChallengeRank < m_cur_rank_len;
		ASSERT(valid);
		CHECK_FALSE_RETURN_FALSE(valid);

		if(iChallengeRank == 0){// iDefenderRankһ�������� �ڰ��� ǰ���Ѿ��ж�
			std::map<UINT32, CArenaContestant*>::iterator it = m_mapOtherContestant.find(iChallengeUserID);
			ASSERT(it != m_mapOtherContestant.end() && it->second);
			if (it == m_mapOtherContestant.end() || it->second == NULL){
				return false;
			}

			CArenaContestant* pChallenger = it->second;
			CArenaContestant* pDefender = GetArenaContestantInRankList(iDefenderRank);
			ASSERT(pDefender);
			if (pDefender == NULL){
				return false;
			}

			ASSERT(pDefender->m_rank == iDefenderRank);
			if (pDefender->m_rank != iDefenderRank){
				return false;
			}

			//�жϰ����Ƿ��пյ����� ����˵�β����
			if (m_cur_rank_len < MAXNUM_ARENA_RANK){
				//����������
				pChallenger->m_rank = iDefenderRank;
				m_rank[iDefenderRank-1] = pChallenger;//������
				m_mapOtherContestant.erase(it);//�ӱ�ѡ���Ƴ�

				//���ط�����
				//pDefender->m_rank = m_cur_rank_len;//bug!! ���ﵼ�������ظ���
				pDefender->m_rank = m_cur_rank_len + 1;
				m_rank[m_cur_rank_len] = pDefender;
				++m_cur_rank_len;
			}
			else{
				//����������λ��
				std::swap(it->second->m_rank, m_rank[iDefenderRank-1]->m_rank);
				std::swap(it->second, m_rank[iDefenderRank-1]);
			}
		}
		else{//��ս�ߺͱ���ս�߶��ڰ���
			CArenaContestant* p1 = GetArenaContestantInRankList(iChallengeRank);
			CArenaContestant* p2 = GetArenaContestantInRankList(iDefenderRank);

			ASSERT( p1 && p2 && p1 != p2);
			ASSERT(p1->m_rank == iChallengeRank);
			ASSERT(p2->m_rank == iDefenderRank);
			if (!p1 || !p2 || p1 == p2 || p1->m_rank != iChallengeRank || p2->m_rank != iDefenderRank){//�ӳٵ������ж������Բ��ԣ�
				return false;
			}

			std::swap(p1->m_rank, p2->m_rank);
			std::swap(m_rank[iChallengeRank-1], m_rank[iDefenderRank-1]);
		}

		return true;
	}

	//�������ֻ�����ս��ʤ��ʱ���� ��սʱ��ս�߲�������������͵������
	bool GetHistoryRankRisePrize(UINT32 from, UINT32 to, std::vector<stGameElement> &output);

	std::vector<stGameElement>& GetEndFightPrize(bool isWin){
		if (isWin){
			return m_fightWinPrize;
		}
		else{
			return m_fightLosePrize;
		}
	}

	e_gsc_errorcode GetRanks(UINT32 iPageNum, ::google::protobuf::RepeatedPtrField<user_arena_contestant> & c_ranks);
	e_gsc_errorcode Search(const UINT32 iUserID, const std::string &strUserName, gs_c_arena_search_res &res);
	bool OnTime(UINT32 enter_time);
	
	std::vector<stGameElement>& GetRankPrize(UINT32 iRank);
private:
	//std::vector<CArenaContestant*> m_rank;//һ��ʼ��ʱ���趨���� �����10w
	//rank=1�����ڵ�0��Ԫ�أ�
	CArenaContestant* m_rank[MAXNUM_ARENA_RANK];//��ǰ����Ϣ �ᶨ�ڼ��ս����ʱ
	UINT32 m_cur_rank_len;//��ǰ���ϵ�������

	//�����б� �ǰ��ϵ��������ս��ʱ �½��Ĳ�����Ϣ ���߰��ϵ���ҵ������ģ�����ֱ��ɾ����
	//��Ҫ������Ϊ�˷�ֹ ��ս�ߵ��� ���������
	//�ᶨ��1.��������2.���ս����ʱ��
	std::map<UINT32, CArenaContestant*> m_mapOtherContestant;//

	std::vector<stGameElement> m_fightWinPrize;//��ս��ʤ����
	std::vector<stGameElement> m_fightLosePrize;//��սʧ�ܽ���
	std::map<UINT32, std::vector<stGameElement>> m_mapArenaRankPrize;//��������������
};

#endif // arena_h__
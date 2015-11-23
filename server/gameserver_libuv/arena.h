#ifndef arena_h__
#define arena_h__

#include <string>
#include <vector>
#include <set>
#include "config.h"

#include "dbs_gs_proto.pb.h"

//竞技场中的英雄数据 这里不重用user_hero 而新建数据结构是因为用不到一些字段
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

//竞技场参赛选手 与上不上榜没有关系
struct CArenaContestant{

	//需要直接存储的配置信息
	UINT32 m_rank;	//排名
	UINT32 m_user_id;	//用户id
	//UINT32 power;//战斗力 不需要
	
	//需要从其他表里读出 保存在内存的信息
	std::string m_name;//名字
	UINT32 m_level;	//等级 需要显示的
	UINT32 m_iHeadID;	//头像id
	UINT32 m_iHeadFrameID;	//头像框架id
	//std::vector<user_hero*>  m_defend_heros;//防守英雄
	CArenaHero* m_defend_heros[MAXNUM_HEROINTEAM];//防守英雄
	
	//>>> 这些信息保存在玩家身上！ 还有对手们也是不需要存储在这里的信息
	//UINT32 score;	//玩家竞技场积分
	//UINT32 last_challenge_time
	//UINT32 last_day_challenge_num
	//UINT32 history_highest_rank

	//动态信息
	//>>> 战斗信息 主动发起者和 被动接受者都需要
	bool m_is_fighting;//是否正在战斗中 不论主动还是被动
	UINT32 m_fight_start_time;//战斗开始的时间 如果客户端一直不结束 那么服务器这边会定时结束掉！并且记录挑战日志
	//这里就不设置了 只设置挑战者对象的字段 这样定时器超时的时候 只会插入一条记录
	//同时 因为主动发起者也会主动结束 所以需要用到对手的信息
		//>>>只需要主动发起者需要设置
	CArenaContestant* m_fighting_defender;//主动上门的挑战者 挑战者的这个字段需要设置! 被挑战者那里这个字段不需要设置! 如果都设置的话 过期的时候 就会添加两条log 没必要
	UINT32 m_fighting_power_me;			//战斗发生时 我（挑战者）的战斗力
	UINT32 m_fighting_power_defender;	//战斗发生时 别人（卫冕者）的战斗力
	//<<< 战斗信息

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

	//正在战斗并且是挑战者
	bool IsFightingAndAttacker(){
		return m_is_fighting && (m_fighting_defender != NULL);
	}

	//只有正在挑战中挑战者能调用！
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

	//结束战斗 只由挑战者调用！ 谁发起谁结束
	bool EndFight(){
		//ASSERT(m_is_fighting && m_fight_start_time/* && m_fighting_defender*/);
		ASSERT(IsFighting());
		//if (!m_is_fighting || !m_fight_start_time/* || !m_fighting_defender*/){
		if (!IsFighting()){
			return false;
		}

		//要么自己就是卫冕的人 如果自己是挑战方 则卫冕的人不是挑战方
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
	//UINT32 attacker_user_id;//挑战者user_id
	//UINT32 attacker_rank;//挑战者排名
	//UINT32 attacker_power;//挑战者此次挑战的战斗力
	//UINT32 defender_user_id;//防守者user_id
	//UINT32 defender_rank;//防守者排名
	//UINT32 defender_power;//防守者战斗力
	//bool is_win;//挑战者是否获胜
	//UINT32 time;//挑战发起时间
	bool is_attacker_win;//挑战方是否获胜
	UINT32 time;//挑战发起时间

	//我的信息
	bool is_me_attacker;// 我是否是挑战发起者
	UINT32 me_rank;//我的排名

	//显示对手信息
	UINT32 match_user_id;
	UINT32 match_rank;
	UINT32 match_power;
	std::string match_name;
	UINT32 match_level;

	struct CArenaLogHero{
		UINT32 hero_id;//id
		UINT32 quality;//品质
		UINT32 level;
		UINT32 star;
	};

	std::vector<CArenaLogHero*> match_heros;//对手的出阵英雄信息

	CArenaLog(){

	}

	~CArenaLog(){
		_CleanMatchHeros();
	}

	void _CleanMatchHeros(){
		for (std::vector<CArenaLogHero*>::iterator it = match_heros.begin() ; it != match_heros.end(); ++it){
			delete *it;// delete NULL是安全的
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
		//attacker_rank = db_log.attacker_rank();//挑战者排名
		//attacker_power = db_log.attacker_power();//挑战者此次挑战的战斗力
		//defender_user_id = db_log.defender_user_id();//防守者user_id
		//defender_rank = db_log.defender_rank();//防守者排名
		//defender_power = db_log.defender_power();//防守者战斗力
		//is_win = db_log.is_win();//挑战者是否获胜
		//time = db_log.time();

		is_attacker_win = db_log.is_attacker_win();//挑战方是否获胜
		time = db_log.time();//挑战发起时间

		//我的信息
		is_me_attacker = db_log.is_me_attacker();// 我是否是挑战发起者
		me_rank = db_log.me_rank();//我的排名

		//显示对手信息
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
	//todo 单例工具
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

		p->m_rank = 0;//不管rank_list现在是否空闲 都不是在这里作rank给予 而是在end的时候

		//不管已满未满 都是先到other 成功了再swap到ranklist中
		////竞技场榜单已满！ 插入到othermap中
		//if (m_cur_rank_len >= MAXNUM_ARENA_RANK){
		//	std::pair<std::map<UINT32, CArenaContestant*>::iterator, bool> r = m_mapOtherContestant.insert(std::make_pair(p->m_user_id, p));

		//	//return false;
		//}
		//else{//榜单未满
		//	//p->m_rank = m_cur_rank_len;
		//	m_rank[m_cur_rank_len++] = p;
		//	//return true;
		//}

		//直接全部都插入到备用列表里
		std::pair<std::map<UINT32, CArenaContestant*>::iterator, bool> r = m_mapOtherContestant.insert(std::make_pair(p->m_user_id, p));
		return r.second;
	}

	//bool IsValidContestant(UINT32 iCurRank/*, UINT32 iHistoryHighestRank*/){
	//	return /*iHistoryHighestRank != 0 &&*/ iCurRank != 0;
	//}

	//排名从1开始 主键从1开始
	CArenaContestant* GetArenaContestantInRankList(UINT32 rank){
		ASSERT(rank >=1 && rank <= MAXNUM_ARENA_RANK);
		if (rank >=1 && rank <= MAXNUM_ARENA_RANK){
			return m_rank[rank-1];
		}

		return NULL;
	}

	//插入到榜单上指定位置
	CArenaContestant* ReplaceIntoRankList(UINT32 rank, CArenaContestant* p){
		return NULL;
	}

	CArenaContestant* GetArenaContestantByUserID(UINT32 iUserID);

	//新建玩家记录的点 在更新队伍！ 不在刷新 也不在战斗！
	bool UpdateTeam(UINT32 iUserID, std::set<db_user_hero*> &heros);

	CArenaHero* GetHero(UINT32 rank, UINT32 hero_id);

	//todo 现在是随机
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
	//	if (!p1 || !p2 || p1 == p2 || p1->m_rank != iRank1 || p2->m_rank != iRank2){//延迟到这里判断排名对不对！
	//		return false;
	//	}

	//	std::swap(p1->m_rank, p2->m_rank);
	//	std::swap(m_rank[iRank1], m_rank[iRank2]);

	//	return true;
	//}

	//2种情况 1. 在ranklist 和other map中交换 2. 在ranklist内部交换
	//不存在other map内部交换 因为这种情况不允许挑战的
	bool Swap(UINT32 iChallengeUserID, UINT32 iChallengeRank, UINT32 iDefenderRank){
		
		/*
		ASSERT( (iChallengeRank == 0 || iChallengeRank > iDefenderRank) && iDefenderRank > 0 && iDefenderRank < m_cur_rank_len && iChallengeRank >= 0 && iChallengeRank < m_cur_rank_len);
		//此判断 排除了 iChallengeRank == iDefenderRank == 0的不允许情况
		//排除了 挑战者名次比卫冕者名次还高的不允许情况
		//排除iChallengeRank 和 iDefenderRank名次无效的问题
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

		if(iChallengeRank == 0){// iDefenderRank一定有名次 在榜单上 前面已经判断
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

			//判断榜单上是否还有空的排名 输的人到尾巴上
			if (m_cur_rank_len < MAXNUM_ARENA_RANK){
				//进攻方处理
				pChallenger->m_rank = iDefenderRank;
				m_rank[iDefenderRank-1] = pChallenger;//进榜了
				m_mapOtherContestant.erase(it);//从备选榜单移出

				//防守方处理
				//pDefender->m_rank = m_cur_rank_len;//bug!! 这里导致排名重复！
				pDefender->m_rank = m_cur_rank_len + 1;
				m_rank[m_cur_rank_len] = pDefender;
				++m_cur_rank_len;
			}
			else{
				//交换排名和位置
				std::swap(it->second->m_rank, m_rank[iDefenderRank-1]->m_rank);
				std::swap(it->second, m_rank[iDefenderRank-1]);
			}
		}
		else{//挑战者和被挑战者都在榜单上
			CArenaContestant* p1 = GetArenaContestantInRankList(iChallengeRank);
			CArenaContestant* p2 = GetArenaContestantInRankList(iDefenderRank);

			ASSERT( p1 && p2 && p1 != p2);
			ASSERT(p1->m_rank == iChallengeRank);
			ASSERT(p2->m_rank == iDefenderRank);
			if (!p1 || !p2 || p1 == p2 || p1->m_rank != iChallengeRank || p2->m_rank != iDefenderRank){//延迟到这里判断排名对不对！
				return false;
			}

			std::swap(p1->m_rank, p2->m_rank);
			std::swap(m_rank[iChallengeRank-1], m_rank[iDefenderRank-1]);
		}

		return true;
	}

	//这个函数只针对挑战者胜利时调用 挑战时挑战者不会出现排名降低的情况！
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
	//std::vector<CArenaContestant*> m_rank;//一开始的时候设定个数 大概是10w
	//rank=1的人在第0个元素！
	CArenaContestant* m_rank[MAXNUM_ARENA_RANK];//当前榜单信息 会定期检查战斗超时
	UINT32 m_cur_rank_len;//当前榜单上的人数！

	//备用列表 非榜单上的玩家想挑战榜单时 新建的参赛信息 或者榜单上的玩家掉下来的（可以直接删除）
	//需要保存是为了防止 挑战者掉线 或者落榜者
	//会定期1.过期清理！2.检查战斗超时！
	std::map<UINT32, CArenaContestant*> m_mapOtherContestant;//

	std::vector<stGameElement> m_fightWinPrize;//挑战获胜奖励
	std::vector<stGameElement> m_fightLosePrize;//挑战失败奖励
	std::map<UINT32, std::vector<stGameElement>> m_mapArenaRankPrize;//竞技场排名奖励
};

#endif // arena_h__
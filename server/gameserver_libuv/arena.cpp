#include <math.h>
#include "arena.h"
#include "str.h"
#include "db_mgr.h"
#include "rand.h"
#include "sys_setting_mgr.h"
#include "time_helper.h"
#include "user.h"
#include "user_mgr.h"
#include "server_for_user.h"
#include "marquee_mgr.h"
#include "mail_mgr.h"
#include "db_sql.h"
#include "client_for_dbs.h"
bool CArena::Init()
{
	//m_rank.resize(MAXNUM_ARENA_RANK);
	memset(m_rank, 0, sizeof m_rank);
	m_cur_rank_len = 0;

	const char* str = NULL;
	bool r = CSysSettingMgr::GetInstance()->GetStringValue("Arena_Fight_Win_Prize", str);
	ASSERT(r && str);
	CHECK_FALSE_RETURN_FALSE(r && str);

	r = stGameElement::Parse(str, m_fightWinPrize, stGameElement::INCREASE);
	ASSERT(r);
	CHECK_FALSE_RETURN_FALSE(r);

	str = NULL;
	r = CSysSettingMgr::GetInstance()->GetStringValue("Arena_Fight_Lose_Prize", str);
	ASSERT(r && str);
	CHECK_FALSE_RETURN_FALSE(r && str);

	r = stGameElement::Parse(str, m_fightLosePrize, stGameElement::INCREASE);
	ASSERT(r);
	CHECK_FALSE_RETURN_FALSE(r);

	MYSQL* con = CDBMgr::GetInstance()->GetConnection();
	CHECK_NULL_RETURN_FALSE(con);

	//������
	char* sql = 
	"SELECT T1.`rank`, T1.`user_id`, T2.`name`, T2.`level`, T2.`head_id`, T2.`head_frame_id` FROM `user_arena` AS T1 LEFT JOIN `user` AS T2 ON T1.`user_id` = T2.`user_id` WHERE T1.`rank` != 0 ORDER BY T1.`rank` ASC;"
	"SELECT T1.`rank`, T2.`hero_id`, T2.`star`, T2.`quality`, T2.`level` "
	"FROM `user_arena` AS T1 LEFT JOIN `user_hero` AS T2 "
		" ON T1.`user_id` = T2.`user_id` "
		" AND (T1.`hero_id1` = T2.`hero_id` OR T1.`hero_id2` = T2.`hero_id` OR T1.`hero_id3` = T2.`hero_id` OR T1.`hero_id4` = T2.`hero_id` OR T1.`hero_id5` = T2.`hero_id`) WHERE T1.`rank` != 0 ORDER BY T1.`rank` ASC;"
	"SELECT T1.`rank`, T2.`hero_id`, T2.`bisha_level`, T2.`aoyi_level`, T2.`passive1_level`, T2.`passive2_level`, T2.`passive3_level` "
	"FROM `user_arena` As T1"
		" LEFT JOIN `user_hero_skill` AS T2 "
		" ON T1.`user_id` = T2.`user_id` "
		" AND (T1.`hero_id1` = T2.`hero_id` OR T1.`hero_id2` = T2.`hero_id` OR T1.`hero_id3` = T2.`hero_id` OR T1.`hero_id4` = T2.`hero_id` OR T1.`hero_id5` = T2.`hero_id`) WHERE T1.`rank` != 0 ORDER BY T1.`rank` ASC;"
	"SELECT T1.`rank`, T2.`hero_id`, T2.`equip_id`, T2.`level` FROM `user_arena` As T1"
		" LEFT JOIN `user_hero_equip` AS T2 "
		" ON T1.`user_id` = T2.`user_id` "
		" AND (T1.`hero_id1` = T2.`hero_id` OR T1.`hero_id2` = T2.`hero_id` OR T1.`hero_id3` = T2.`hero_id` OR T1.`hero_id4` = T2.`hero_id` OR T1.`hero_id5` = T2.`hero_id`) WHERE T1.`rank` != 0 ORDER BY T1.`rank` ASC;"
		;
	int code = CDBMgr::GetInstance()->Query(con, sql, (unsigned int)strlen(sql));
	if (0 != code)   //��0��ѯʧ��  
	{
		printf("query failed! [%s] [%d] [%s] \n", sql, mysql_errno(con), mysql_error(con));       
		return false;  
	}

	int i = 0;
	do
	{
		MYSQL_RES* res = mysql_store_result( con );
		switch (i++)
		{
		case 0:{//user_arena * user
				   {
					   //����ṹ�Ƿ��иı�
					   std::string need_fields[] = {"rank", "user_id", "name", "level", "head_id", "head_frame_id"};
					   std::vector<std::string> v_need_fields(need_fields, need_fields + sizeof(need_fields)/sizeof(need_fields[0]));
					   ASSERT(CDBMgr::check_fields(res, v_need_fields));
				   }

				   MYSQL_ROW row;
				   while (row = mysql_fetch_row(res)){
					   if (row[2] != NULL && row[3] != NULL){//����Ч���ݣ� ��������������user����û�е��û���Ϣ��
						   //printf("key[%s] value_type[%d] value[%s] desc[%s] \n", row[0], atoi(row[1]), row[2], row[3]);
						   //continue;
						   UINT32 rank = atoi(row[0]);
						   UINT32 col = 1;
						   UINT32 user_id = atoi(row[col++]);
						   std::string name = row[col++];//��Ȼutf8
						   UINT32 level = atoi(row[col++]);
						   UINT32 iHeadID = atoi(row[col++]);
						   UINT32 iHeadFrameID = atoi(row[col++]);
						   ASSERT(rank != 0);
						   //if ( rank != 0 ){
						   CArenaContestant* p = GetArenaContestantInRankList(rank);
						   ASSERT(p == 0);//
						   if(p != 0){
							   printf("error�������ظ���\n");
							   return false;
						   }

						   ASSERT(m_cur_rank_len == (rank-1));//�жϴ�1��ʼ�������������ڿն���
						   if (m_cur_rank_len != (rank-1)){
							   printf("error������û��1��ʼ�����������ڿն���\n");
							   return false;
						   }
						   // }
						   // else{
						   //   CArenaContestant* p = GetArenaContestantByUserID(user_id);
						   //ASSERT(p == 0);//
						   //if(p != 0){
						   //	printf("error��user_id�ظ���\n");
						   //	return false;
						   //}
						   // }

						   CArenaContestant* pItem = new CArenaContestant(user_id, name, level, iHeadID, iHeadFrameID);
						   pItem->m_rank = rank;
						   //pItem->m_user_id = user_id;
						   //pItem->m_name = name;//��Ȼutf8
						   //pItem->m_level = level;

						   //if (rank != 0){
						   m_rank[rank-1] = pItem;
						   ++m_cur_rank_len;
						   //}
						   //else{
						   // m_mapOtherContestant[user_id] = pItem;
						   //}
					   }
					   else{
						   ASSERT(false && "����Ч���ݣ� ��������������user����û�е��û���Ϣ��");
						   return false;
					   }
				   }

				   ASSERT(m_cur_rank_len >= 10 && "��������ȱ�ٳ�ʼ�����������");
				   CHECK_FALSE_RETURN_FALSE(m_cur_rank_len >= 10);
			   }
			   break;
		case 1:
			{//user_arena * user_hero
				{
					//����ṹ�Ƿ��иı�
					std::string need_fields[] = {"rank", "hero_id", "star", "quality", "level"};
					std::vector<std::string> v_need_fields(need_fields, need_fields + sizeof(need_fields)/sizeof(need_fields[0]));
					ASSERT(CDBMgr::check_fields(res, v_need_fields));
				}

				MYSQL_ROW row;
				while (row = mysql_fetch_row(res)){
					if (row[1] != NULL){//hero_id���ֶο���Ϊnull��
						UINT32 rank = atoi(row[0]);
						CArenaContestant* pContestant = GetArenaContestantInRankList(rank);
						ASSERT(pContestant);
						if (!pContestant){
							return false;
						}

						UINT32 idx = pContestant->GetNextEmptyHeroPos();
						if (idx == -1){
							printf("error! Ӣ������ > MAXNUM_HEROINTEAM  rank[%u] user[%u]\n", pContestant->m_rank, pContestant->m_user_id);
							return false;
						}

						UINT32 col = 1;
				
						CArenaHero* pHero = new CArenaHero;
						pHero->hero_id = atoi(row[col++]);
						pHero->star = atoi(row[col++]);
						pHero->quality = atoi(row[col++]);
						pHero->level = atoi(row[col++]);
						pContestant->m_defend_heros[idx] = pHero;
					}
					else{
						ASSERT(false && "����Ч���ݣ� ��������������user_hero����û�е�Ӣ����Ϣ��");
						return false;
					}
				}
			}
			break;
		case 2:
			{//user_arena * user_hero_skill
				{
					//����ṹ�Ƿ��иı�
					std::string need_fields[] = {"rank", "hero_id", "bisha_level", "aoyi_level", "passive1_level", "passive2_level", "passive3_level"};
					std::vector<std::string> v_need_fields(need_fields, need_fields + sizeof(need_fields)/sizeof(need_fields[0]));
					ASSERT(CDBMgr::check_fields(res, v_need_fields));
				}

				MYSQL_ROW row;
				while (row = mysql_fetch_row(res)){
					if (row[1] != NULL){
						UINT32 rank = atoi(row[0]);
						CArenaContestant* pContestant = GetArenaContestantInRankList(rank);
						ASSERT(pContestant);
						if (!pContestant){
							return false;
						}

						UINT32 hero_id = atoi(row[1]);
						CArenaHero* pHero = pContestant->GetHero(hero_id);
						ASSERT(pHero);
						if (!pHero){
							return false;
						}

						UINT32 col = 2;
						pHero->skills.bisha_level = atoi(row[col++]);
						pHero->skills.aoyi_level = atoi(row[col++]);
						pHero->skills.passive1_level = atoi(row[col++]);
						pHero->skills.passive2_level = atoi(row[col++]);
						pHero->skills.passive3_level = atoi(row[col++]);
					}
					else{
						ASSERT(false && "����Ч���ݣ� ��������������user_hero����û�е�Ӣ����Ϣ��");
						return false;
					}
				}
			}
			break;
		case 3:
			{//user_arena * user_hero_equip
				{
					//����ṹ�Ƿ��иı�
					std::string need_fields[] = {"rank", "hero_id", "equip_id", "level"};
					std::vector<std::string> v_need_fields(need_fields, need_fields + sizeof(need_fields)/sizeof(need_fields[0]));
					ASSERT(CDBMgr::check_fields(res, v_need_fields));
				}

				MYSQL_ROW row;
				while (row = mysql_fetch_row(res)){
					if (row[1] != NULL){
						UINT32 rank = atoi(row[0]);
						UINT32 hero_id = atoi(row[1]);

						CArenaHero* pHero = GetHero(rank, hero_id);
						ASSERT(pHero);
						if (!pHero){
							return false;
						}

						UINT32 idx = pHero->GetNextEmptyEquipPos();
						if (idx == -1){
							printf("error! Ӣ��װ������ > EEQUIPTYPE_MAX \n");
							return false;
						}

						UINT32 col = 2;
						CArenaHero::CArenaHeroEquip* equip = new CArenaHero::CArenaHeroEquip;
						equip->equip_id = atoi(row[col++]);
						equip->level = atoi(row[col++]);

						pHero->equips[idx] = equip;
					}
					else{
						ASSERT(false && "����Ч���ݣ� ��������������user_hero����û�е�Ӣ����Ϣ��");
						return false;
					}
				}
			}
			break;
		}
		mysql_free_result(res);
	}while( !mysql_next_result( con ) );

	//todo ��龺������Ϣ�Ƿ���ȷ��
	

	//������������������(sys_arena_prize)
	sql = SELECT_ARENA_PRIZE;
	code = CDBMgr::Query(con, sql, (unsigned int)strlen(sql));
	if (0 != code)   //��0��ѯʧ��  
	{
		printf("query failed! [%s] [%d] [%s] \n", sql, mysql_errno(con), mysql_error(con));       
		return false;  
	}
	MYSQL_RES* res = mysql_store_result(con);
	
	{
		std::string need_fields[] = {"rank", "prize"};
		std::vector<std::string> v_task_need_field(need_fields, need_fields + sizeof(need_fields)/sizeof(need_fields[0]));
		ASSERT(CDBMgr::check_fields(res, v_task_need_field));
	}
	MYSQL_ROW row;
	while(row = mysql_fetch_row(res))
	{
		int col = 0;
		UINT32 iRank = atoi(row[col++]);
		
		char* pBuf = UTF8ToANSI(row[col++]);
		std::vector<std::string> vecPrizes;
		split_string(std::string(pBuf), vecPrizes, "|");
		free(pBuf);
		
		std::vector<stGameElement> vecGameEle;
		for (std::vector<std::string>::iterator it = vecPrizes.begin(); it !=  vecPrizes.end(); ++it)
		{
			std::vector<int> aprize;
			split_string_2i(*it, aprize, "-");
			ASSERT(aprize.size() == 3);
			const stGameElement& ele = stGameElement((EELEMENTTYPE)aprize[0], aprize[1], 0, stGameElement::INCREASE, aprize[2]);
			ASSERT(IsElementValid(ele));
			if (!IsElementValid(ele))
			{
				return false;
			}
			vecGameEle.push_back(ele);
		}
		ASSERT(m_mapArenaRankPrize.find(iRank) == m_mapArenaRankPrize.end());
		m_mapArenaRankPrize[iRank] = vecGameEle;
	}
	ASSERT(m_mapArenaRankPrize.size());

	return true;
}

bool CArena::GetMatches( UINT32 irank, std::set<CArenaContestant*> &ret_matches )
{
	//for (UINT32 i = 0 ; i < MAXNUM_ARENA_MATCH ;++i){
	//	UINT32 rank = CRandom::GetInstance()->GetRandom(0, m_cur_rank_len-1);
	//	ASSERT(m_rank[rank]);
	//	if (!m_rank[rank]){
	//		return false;
	//	}
	//	ret_matches.insert(m_rank[rank]);
	//}

	ASSERT(m_cur_rank_len > 10);

	std::set<UINT32> chosenRanks;

	//û���ϰ� ����ĩ��10% ���
	if (irank == 0){
		UINT32 start = floor(m_cur_rank_len * 0.9);
		ASSERT(m_cur_rank_len > 1);
		UINT32 end = m_cur_rank_len-1;

		ASSERT( start >= 1 && start < m_cur_rank_len && end >= 1);
		CRandom::GetInstance()->GetRandom(start, end, irank, MAXNUM_ARENA_MATCH, chosenRanks);
	}
	else if( irank <= 4){//1)��S��4������ʾ������������S�Ķ��֣������ֲ���3��������ʾ�������������Ķ��֣�ʣ���λ�ÿճ�����ʾ��
		for (UINT32 i = irank-1 ; i >= 1 ; --i){
			if ( i != irank){
				chosenRanks.insert(i);
			}
		}
	}
	else if (irank <= 10){//2)��4��S��10����������[��S-6����1����S-6����1��S-1]�������3������
		UINT32 start = irank  >= 7 ? irank - 6 : 1;
		UINT32 end = irank - 1;

		CRandom::GetInstance()->GetRandom(start, end, irank, MAXNUM_ARENA_MATCH, chosenRanks);
	}
	else{//3����S>10�����ȼ���K1=S*60%��K2=S*80%��K3=S*90%������ȡ����Ȼ��S1��[K1��K2-1]��S2��[K2,K3-1]��S3��[K3,S-1]��
		UINT32 k1 = floor(irank * 0.6);
		UINT32 k2 = floor(irank * 0.8);
		UINT32 k3 = floor(irank * 0.9);

		ASSERT(k1 && k2 > 1 && k3 >1 );
		CRandom::GetInstance()->GetRandom(k1, k2-1, irank, 1, chosenRanks);
		CRandom::GetInstance()->GetRandom(k2, k3-1, irank, 1, chosenRanks);
		CRandom::GetInstance()->GetRandom(k3, irank-1, irank, 1, chosenRanks);
	}

	for(std::set<UINT32>::iterator it = chosenRanks.begin(); it != chosenRanks.end(); ++it){
		UINT32 i = *it;
		CArenaContestant* p = GetArenaContestantInRankList(i);
		ASSERT(p);
		if (!p){
			return false;
		}

		ret_matches.insert(p);
	}

	return true;
}

bool CArena::GetMatches( UINT32 irank, ::google::protobuf::RepeatedPtrField< ::user_arena_contestant > &ret_matches )
{
	std::set<CArenaContestant*> matchs;
	bool r =  GetMatches(irank, matchs);
	if (!r){
		return false;
	}

	for (std::set<CArenaContestant*>::iterator it = matchs.begin(); it != matchs.end(); ++it){
		if (*it){
			user_arena_contestant* c_match = ret_matches.Add();
			bool r= (*it)->Fill(*c_match);
			if (!r){
				ASSERT(false);
				return false;
			}
		}
	}

	return true;
}
bool CArena::UpdateTeam( UINT32 iUserID, std::set<db_user_hero*> &heros )
{
	ASSERT(heros.size() <= MAXNUM_HEROINTEAM);
	if (heros.size() > MAXNUM_HEROINTEAM){
		return false;
	}

	CArenaContestant* p = GetArenaContestantByUserID(iUserID);
	if (!p){
		return false;
	}

	bool r = p->UpdateTeam(heros);
	if (!r){
		return false;
	}

	return true;
}

CArenaHero* CArena::GetHero( UINT32 rank, UINT32 hero_id )
{
	CArenaContestant* pContestant = GetArenaContestantInRankList(rank);
	//ASSERT(pContestant);
	if (!pContestant){
		return NULL;
	}

	CArenaHero* pHero = pContestant->GetHero(hero_id);
	return pHero;
}

CArenaContestant* CArena::GetArenaContestantByUserID( UINT32 iUserID )
{
	//todo ����ʱ�����ģ�
	for (UINT32 i = 0 ; i < m_cur_rank_len ; ++i){
		CArenaContestant* p = m_rank[i];
		ASSERT(p);
		if ( p && p->m_user_id == iUserID){
			return p;
		}
	}

	std::map<UINT32, CArenaContestant*>::iterator it = m_mapOtherContestant.find(iUserID);
	if ( it != m_mapOtherContestant.end()){
		return it->second;
	}

	return NULL;
}

bool CArena::GetHistoryRankRisePrize( UINT32 from, UINT32 to, std::vector<stGameElement> &output )
{
	//from����Ϊ0 û��������ʱ��
	ASSERT(/* from && */to && (from == 0 ||from > to)  && from < m_cur_rank_len && to < m_cur_rank_len);
	if (/*!from ||*/ !to || (from !=0 && from <= to) || from >= m_cur_rank_len || to >= m_cur_rank_len){
		return false;
	}

	UINT32 a = CSysSettingMgr::GetInstance()->Arena_History_Rank_Prize_factor1;
	UINT32 b = CSysSettingMgr::GetInstance()->Arena_History_Rank_Prize_factor2;
	ASSERT(a && b);
	UINT32 num = 0;
	if(from == 0){
		num = ceil(a/(double)(to + b));
	}
	else{
		num = ceil(a/(double)(to+b) - a/(double)(from+b));
	}

	output.push_back(stGameElement(EELEMENTTYPE_DIAMOND, 0, 0, stGameElement::INCREASE, num));
	//stGameElement ele(EELEMENTTYPE_DIAMOND, 0, 0, stGameElement::INCREASE, (to - from));
	return true;
}

bool CArena::OnTime( UINT32 enter_time )
{
	static UINT32 last_enter_time = 0;

	////��ʱ�и� ��������û��������� ����֪ͨ���������

	//printf("CArena::OnTime  last_enter_time[%u] - enter_time[%u] = [%u] \n", last_enter_time, enter_time, enter_time - last_enter_time);
	//if (diffTime(enter_time, last_enter_time) < 60){//һ����һ��
	//	return false;
	//}

	//last_enter_time = enter_time;
	////�������а��ϵ�������ս��
	//for (){

	//}

	////������ѡ�б������������ս��
	//for ( std::map<UINT32, CArenaContestant*>::iterator it = m_mapOtherContestant.begin(); it != m_mapOtherContestant.end(); ++it){
	//	CArenaContestant* p = it->second;
	//	ASSERT(p);
	//	
	//	//���ڹ�������ս��
	//	if (!p || p->IsFightingAndAttacker()){
	//		continue;
	//	}

	//	//�Ƿ�ʱ
	//	if (!p->IsAttackerFightingTimeOut){
	//		continue;
	//	}

	//	CUser* pAttacker = CUserMgr::GetInstance()->GetUserByUserID(p->m_user_id);
	//	ASSERT(pAttacker);
	//	if (!pAttacker){
	//		
	//	}

	//	//δ�����ľ������и���
	//	c_gs_arena_end_challenge_req req;
	//	req.set_is_win(false);

	//	gs_dbs_user_info_op_req* db_req = new gs_dbs_user_info_op_req;
	//	db_req->set_user_id(UserID());

	//	gs_c_arena_end_challenge_res res;
	//	gs_c_mail_ntf_array mail_ntf;
	//	gs_c_arena_challenge_ntf attacker_ntf;
	//	gs_c_arena_challenge_ntf defender_ntf;
	//	bool r = ArenaEndChallenge(req, *db_req, res, mail_ntf, attacker_ntf, defender_ntf);
	//	//send(stream, e_msg_gs_c_arena_end_challenge_res, &res);
	//	CServerForUser::GetInstance()->_send_db_req(db_req);

	//	//if (mail_ntf.mails_size() > 0){
	//	//	send(stream, e_msg_gs_c_mail_ntf, &mail_ntf);
	//	//}
	//	//����������߾͸����ַ���־
	//	UINT32 iDefenderUserID = 0;
	//	if(attacker_ntf.IsInitialized()){
	//		ASSERT(defender_ntf.IsInitialized());
	//		iDefenderUserID = attacker_ntf.ele().match_user_id();
	//	}

	//	if (iDefenderUserID && defender_ntf.IsInitialized()){
	//		CUser* pDefender = CUserMgr::GetInstance()->GetUserByUserID(iDefenderUserID);

	//		if (pDefender){
	//			uv_stream_t* con = pDefender->GetConnect();
	//			ASSERT(con);
	//			CServerForUser::GetInstance()->send(con, e_msg_gs_c_arena_challenge_ntf, &defender_ntf);
	//		}
	//		//ASSERT(pDefender);
	//	}

	//}


	//���а��б��ϵ���ս��
	for(UINT32 i = 0 ; i < m_cur_rank_len ; ++i){
		CArenaContestant* p = m_rank[i];
		ASSERT(p);

		//���ڹ�������ս��
		if (!p || !p->IsFightingAndAttacker()){
			continue;
		}

		//�Ƿ�ʱ
		if (!p->IsAttackerFightingTimeOut()){
			continue;
		}

		CUser* pUser = CUserMgr::GetInstance()->GetUserByUserID(p->m_user_id);
		ASSERT(pUser); 
		//������� �������ʱ ���Ƚ��������� �� ��usermap�� ȥ�� �������������û�н��� ��ô���Ӧ��Ҳ�������map
		//Ҳ����˵��һ�����
		pUser->ArenaLeave(true);
	}

	//������ѡ�б������������ս��
	for ( std::map<UINT32, CArenaContestant*>::iterator it = m_mapOtherContestant.begin(); it != m_mapOtherContestant.end(); ++it){
		CArenaContestant* p = it->second;
		ASSERT(p);
		
		//���ڹ�������ս��
		if (!p || !p->IsFightingAndAttacker()){
			continue;
		}

		//�Ƿ�ʱ
		if (!p->IsAttackerFightingTimeOut()){
			continue;
		}

		CUser* pUser = CUserMgr::GetInstance()->GetUserByUserID(p->m_user_id);
		ASSERT(pUser); 
		//������� �������ʱ ���Ƚ��������� �� ��usermap�� ȥ�� �������������û�н��� ��ô���Ӧ��Ҳ�������map
		//Ҳ����˵��һ�����
		pUser->ArenaLeave(true);
	}


	//�����������ʼ�
	gs_dbs_user_new_arena_mail_req* req = new gs_dbs_user_new_arena_mail_req;
	for (int i = 1; i <= m_cur_rank_len; ++i)
	{
		CArenaContestant* pArenaCon = GetArenaContestantInRankList(i);
		ASSERT(pArenaCon);
		
		std::vector<stGameElement>& vecEle = GetRankPrize(i);
		ASSERT(vecEle.size());

		std::map<std::string, CValue> values;
		CValue& vTime = values["num1"];
		vTime.SetIntValue(Now());
		
		CValue& vRank = values["num2"];
		vRank.SetIntValue(i);
		
		arena_mail* mail = req->add_mails();
		mail->set_user_id(pArenaCon->m_user_id);

		CMailMgr::GetInstance()->Fill_Arena_Mail_Req(MAIL_TYPE_ARENA_RANK_REWARD, *mail->mutable_mail(), NULL, &values, &vecEle);
	}
	CClientForDBS::GetInstance()->SendAsync(CClientForDBSAsync::EASyncOP_User_New_Arena_mail, 0, req);
	return true;
}

e_gsc_errorcode CArena::GetRanks( UINT32 iPageNum, ::google::protobuf::RepeatedPtrField<user_arena_contestant> & c_ranks )
{
	ASSERT(m_cur_rank_len > 0);
	if (m_cur_rank_len == 0){
		return e_gsc_errorcode_success;
	}
	UINT32 iTotalPageNum = m_cur_rank_len/MAXNUM_ARENA_PER_PAGE;
	if(iPageNum > iTotalPageNum){
		return e_gsc_errorcode_arena_rank_page_out;
	}

	UINT32 start = iPageNum*MAXNUM_ARENA_PER_PAGE;
	UINT32 end = start + MAXNUM_ARENA_PER_PAGE;
	for (UINT32 i = start ; i < end ; ++i){
		if (i >= m_cur_rank_len){
			break;
		}

		CArenaContestant* p = m_rank[i];
		ASSERT(p);
		if (!p){
			return e_gsc_errorcode_unknown_error;
		}

		bool r = p->Fill(*c_ranks.Add());
		ASSERT(r);
		if (!r){
			return e_gsc_errorcode_unknown_error;
		}
	}

	return e_gsc_errorcode_success;
}

e_gsc_errorcode CArena::Search(const UINT32 iUserID, const std::string &strUserName, gs_c_arena_search_res &res){
	if (iUserID){
		for (UINT32 i = 0 ; i < m_cur_rank_len ; ++i){
			CArenaContestant* p = m_rank[i];
			ASSERT(p);
			if ( p && p->m_user_id == iUserID){
				user_arena_contestant* contestant = res.mutable_contestant();
				bool r = p->Fill(*contestant);
				ASSERT(r);
				return e_gsc_errorcode_success;
			}
		}

		return e_gsc_errorcode_arena_search_no_result;
	}
	else if(!strUserName.empty()){
		for (UINT32 i = 0 ; i < m_cur_rank_len ; ++i){
			CArenaContestant* p = m_rank[i];
			ASSERT(p);
			if ( p && p->m_name == strUserName){
				user_arena_contestant* contestant = res.mutable_contestant();
				bool r = p->Fill(*contestant);
				ASSERT(r);
				return e_gsc_errorcode_success;
			}
		}

		return e_gsc_errorcode_arena_search_no_result;
	}
	else{
		return e_gsc_errorcode_unknown_error;
	}
}

std::vector<stGameElement>& CArena::GetRankPrize( UINT32 iRank)
{
	ASSERT(iRank > 0);
	std::map<UINT32, std::vector<stGameElement>>::iterator it = m_mapArenaRankPrize.find(iRank);
	while(it == m_mapArenaRankPrize.end())
	{
		iRank--;
		it = m_mapArenaRankPrize.find(iRank);
	}
	ASSERT(it != m_mapArenaRankPrize.end());
	return it->second;
}

bool CArenaHero::CArenaHeroSkill::Fill( ::google::protobuf::RepeatedPtrField< ::user_hero_skill > &c_skills )
{
	//���ܵȼ� Ϊ0ʱ��ʾδ����

	if (bisha_level){
		user_hero_skill* c_skill = c_skills.Add();
		c_skill->set_type(user_hero_skill::BISHA);
		c_skill->set_level(bisha_level);
	}
	else{//һ���а���
		return false;
	}

	if (aoyi_level){
		user_hero_skill* c_skill = c_skills.Add();
		c_skill->set_type(user_hero_skill::AOYI);
		c_skill->set_level(aoyi_level);
	}
	else{//һ���б�ɱ
		return false;
	}

	if (passive1_level){
		user_hero_skill* c_skill = c_skills.Add();
		c_skill->set_type(user_hero_skill::PASSIVE1);
		c_skill->set_level(passive1_level);
	}

	if (passive2_level){
		user_hero_skill* c_skill = c_skills.Add();
		c_skill->set_type(user_hero_skill::PASSIVE2);
		c_skill->set_level(passive2_level);
	}

	if (passive3_level){
		user_hero_skill* c_skill = c_skills.Add();
		c_skill->set_type(user_hero_skill::PASSIVE3);
		c_skill->set_level(passive3_level);
	}

	return true;
}

bool CArenaHero::CArenaHeroSkill::CopyFrom( const ::google::protobuf::RepeatedPtrField< ::db_user_hero_skill > &db_skills )
{
	//���ܵȼ� Ϊ0ʱ��ʾδ����

	for ( ::google::protobuf::RepeatedPtrField< ::db_user_hero_skill >::const_iterator it = db_skills.begin() ; it != db_skills.end() ; ++it){
		switch (it->type())
		{
		case user_hero_skill::BISHA:
			{
				bisha_level = it->level();
			}
			break;
		case user_hero_skill::AOYI:
			{
				aoyi_level = it->level();
			}
			break;
		case user_hero_skill::PASSIVE1:
			{
				passive1_level = it->level();
			}
			break;
		case user_hero_skill::PASSIVE2:
			{
				passive2_level = it->level();
			}
			break;
		case user_hero_skill::PASSIVE3:
			{
				passive3_level = it->level();
			}
			break;
		default:
			ASSERT(false && "��֧�ֵļ�������");
			return false;
		}
	}

	ASSERT(bisha_level && aoyi_level);//���������ǻ����ģ�

	return bisha_level && aoyi_level;
}

void CArenaHero::Clean()
{
	hero_id = 0;
	star = 0;
	quality = 0;
	level = 0;

	for (UINT32 i = 0; i < (EEQUIPTYPE_MAX - 1); ++i){
		if (equips[i]){
			delete equips[i];
			equips[i] = NULL;
		}
	}
	memset(equips, 0 , sizeof equips);

	//for (UINT32 i = 0; i < (ESKILLTYPE_MAX - 1); ++i){
	//	if (skills[i]){
	//		delete skills[i];
	//		skills[i] = NULL;
	//	}
	//}
	memset(&skills, 0 , sizeof skills);
}

UINT32 CArenaHero::GetNextEmptyEquipPos()
{
	UINT32 idx = -1;
	for (UINT32 i = 0 ; i < EEQUIPTYPE_MAX -1 ; ++i){
		if(equips[i] == 0){
			idx = i;
			break;
		}
	}

	return idx;
}

bool CArenaHero::Fill( user_hero& c_hero )
{
	/** Ӣ��id */
	c_hero.set_hero_id(hero_id);
	/** �Ǽ� */
	c_hero.set_star(star);
	/** Ʒ�� */
	c_hero.set_quality(quality);
	/** �ȼ� */
	c_hero.set_level(level);
	/** ��ǰ���� */
	c_hero.set_cur_exp(0);//����Ҫ,��Ҫ������
	/*��ȡʱ��*/
	c_hero.set_got_timestamp(0);//����Ҫ,��Ҫ������

	/** װ�� */
	//repeated user_hero_equip equips = 7 ;
	for (UINT32 i = 0 ; i < EEQUIPTYPE_MAX -1 ; ++i){
		CArenaHeroEquip* PEquip = equips[i];
		if(PEquip){
			user_hero_equip* c_equip = c_hero.add_equips();
			bool r = PEquip->Fill(*c_equip);
			ASSERT(r);
			if (!r){
				return false;
			}
		}
	}

	/** ���� */
	//repeated user_hero_skill skills = 8 ;
	bool r = skills.Fill(*c_hero.mutable_skills());
	ASSERT(r);
	return r;
}

bool CArenaHero::CopyFrom( const db_user_hero& db_hero )
{
	Clean();//��������

	/** Ӣ��id */
	hero_id = db_hero.hero_id();
	/** �Ǽ� */
	star = db_hero.star();
	/** Ʒ�� */
	quality = db_hero.quality();
	/** �ȼ� */
	level = db_hero.level();
	/** ��ǰ���� */
	//hero.cur_exp();//����Ҫ
	/*��ȡʱ��*/
	//hero.got_timestamp();//����Ҫ

	/** װ�� */
	//repeated user_hero_equip equips = 7 ;

	ASSERT(db_hero.equips_size() <= EEQUIPTYPE_MAX - 1);
	UINT32 i = 0;
	for ( ::google::protobuf::RepeatedPtrField< ::db_user_hero_equip >::const_iterator it = db_hero.equips().begin(); it != db_hero.equips().end() ; ++it){
		CArenaHeroEquip* pEquip = new CArenaHeroEquip;
		bool r = pEquip->CopyFrom(*it);
		ASSERT(r);
		if (!r){
			return false;
		}

		equips[i++] = pEquip;
	}

	/** ���� */
	//repeated user_hero_skill skills = 8 ;
	bool r = skills.CopyFrom(db_hero.skills());
	ASSERT(r);
	return r;
}

void CArenaContestant::Clean()
{
	m_rank = 0;
	m_user_id = 0;
	CleanHero();

	m_name.clear();
	m_level = 0;
	//score = 0;
	//power = 0;

	m_is_fighting = false;
	m_fight_start_time = 0;
	
	m_iHeadID = 0;
	m_iHeadFrameID = 0;
	//����������ɾ����
	//if(!m_fighting_defender){
	//	delete m_fighting_defender;
	//}
	m_fighting_defender = NULL;//����Ҫɾ�� �ɾ�����ͳһɾ����
}

void CArenaContestant::CleanHero()
{
	for (UINT32 i = 0; i < MAXNUM_HEROINTEAM ; ++i){
		if (m_defend_heros[i]){
			delete m_defend_heros[i];
			m_defend_heros[i] = NULL;
		}
	}

	memset(m_defend_heros, 0, sizeof m_defend_heros);
}

UINT32 CArenaContestant::GetNextEmptyHeroPos()
{
	UINT32 idx = -1;
	for (UINT32 i = 0 ; i < MAXNUM_HEROINTEAM; ++i){
		if(m_defend_heros[i] == NULL){
			idx = i;
			break;
		}
	}

	return idx;
}

CArenaHero* CArenaContestant::GetHero( UINT32 iHeroID )
{
	ASSERT(iHeroID);
	if (!iHeroID){
		return NULL;
	}

	CArenaHero* pHero = NULL;
	for (UINT32 i = 0 ; i < MAXNUM_HEROINTEAM; ++i){
		pHero = m_defend_heros[i];
		if( pHero && pHero->hero_id == iHeroID){
			return pHero;
		}
	}

	return pHero;
}

bool CArenaContestant::Fill( user_arena_contestant& c_contestant )
{
	c_contestant.set_rank(m_rank);	//����
	c_contestant.set_user_id(m_user_id);	//�û�id
	c_contestant.set_name(m_name);//����
	c_contestant.set_level(m_level);//�ȼ�
	c_contestant.set_head_id(m_iHeadID);
	c_contestant.set_head_frame_id(m_iHeadFrameID);
	for (UINT32 i = 0 ; i < MAXNUM_HEROINTEAM; ++i){
		CArenaHero* pHero = m_defend_heros[i];
		if(pHero){
			user_hero* c_hero = c_contestant.add_heros();
			bool r = pHero->Fill(*c_hero);
			ASSERT(r);
			if (!r){
				return false;
			}
		}
	}

	return true;
}

bool CArenaContestant::UpdateTeam( std::set<db_user_hero*> &heros )
{
	//������ǰӢ��
	CleanHero();
	UINT32 i = 0;
	for ( std::set<db_user_hero*>::iterator it = heros.begin(); it != heros.end(); ++it){
		db_user_hero* hero = *it;
		CArenaHero* pHero = new CArenaHero;
		ASSERT(pHero);
		bool r = pHero->CopyFrom(*hero);
		ASSERT(r);
		if (!r){
			return false;
		}

		UINT32 idx = GetNextEmptyHeroPos();
		if (idx == -1){
			printf("error! Ӣ������ > MAXNUM_HEROINTEAM  rank[%u] user[%u]\n", m_rank, m_user_id);
			return false;
		}

		m_defend_heros[idx] = pHero;
	}

	return true;
}

bool CArenaContestant::IsAttackerFightingTimeOut()
{
	ASSERT(IsFightingAndAttacker());
	if (IsFightingAndAttacker()){
		if (diffTime(Now(), m_fight_start_time) < CSysSettingMgr::GetInstance()->Arena_Fight_Time_Out){
			return false;
		}
	}

	return true;
}
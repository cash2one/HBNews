#pragma once
#ifndef rank_mgr_h__
#define rank_mgr_h__
#include "config.h"
#include "gs_c_proto.pb.h"
#include "dbs_gs_proto.pb.h"
struct stRank{
	stRank(){
		memset(this, 0, sizeof stRank);
	}

	UINT32 head_id;
	UINT32 head_frame_id;
	UINT32 level;
	char  name[MAXLEN_USERNAME];
	UINT32 arena_rank;
	UINT32 fight_rank;
	UINT32 fight_power;
	UINT32 level_rank;
	UINT32 vip;
	::google::protobuf::RepeatedPtrField<gs_c_rank_res::user_hero_brief_info> heros;

};

class CRankMgr
{
public:

	static CRankMgr* GetInstance(){
		static CRankMgr singleton;
		return &singleton;
	}

	bool Init();

	void ClearRank();

	void OnTime(UINT32 iTimer);

	void UpdateRank(dbs_gs_rank_res& res);

	bool GetRank(UINT32 iUserID, c_gs_rank_req::RANK_TYPE eType, gs_c_rank_res& res);
private:
	CRankMgr(void):m_iLastUpdateTime(0){};
	std::vector<stRank>	vecArenaRank;		//����������
	std::vector<stRank> vecFightPowerRank;	//ս�����а�
	std::vector<stRank> vecLevelRank;		//�ȼ�����
	std::vector<stRank> vecGuildRank;		//�������
	
	std::map<UINT32, UINT32> m_mapUsersLevelRanks;		//������ҵȼ�����
	std::map<UINT32, UINT32> m_mapUsersFightPowerRanks;	//�������ս��������
	UINT32 m_iLastUpdateTime;
};

#endif
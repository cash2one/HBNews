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
	std::vector<stRank>	vecArenaRank;		//竞技场排行
	std::vector<stRank> vecFightPowerRank;	//战力排行榜
	std::vector<stRank> vecLevelRank;		//等级排行
	std::vector<stRank> vecGuildRank;		//帮会排行
	
	std::map<UINT32, UINT32> m_mapUsersLevelRanks;		//所有玩家等级排名
	std::map<UINT32, UINT32> m_mapUsersFightPowerRanks;	//所有玩家战斗力排名
	UINT32 m_iLastUpdateTime;
};

#endif
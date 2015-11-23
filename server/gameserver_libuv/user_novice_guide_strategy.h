#ifndef user_novice_guide_strategy_h__
#define user_novice_guide_strategy_h__
#include <set>
#include "def.h"
#include "dbs_gs_proto.pb.h"
#include "config.h"
class CUser;
class IUserNoviceGuideStrategy{
public:
	~IUserNoviceGuideStrategy(){};
	virtual bool Init(CUser* pUser, db_user_novice_guide_array* guide_arr) = 0;
	virtual bool IsGuideDone(UINT32 iGuideID) = 0;
	virtual bool SetGuideDone(UINT32 iGuideID) = 0;
	virtual void FinishNoviceGuide(UINT32 iGuideID, gs_dbs_user_info_op_req& db_req, gs_c_finish_novice_guide_res& res, GS_C_NTF& ntf) = 0;
	virtual bool TriggerGuidMail(UINT32 iLastLevel, UINT32 iNowLevle, gs_dbs_user_info_op_req& db_req) = 0;
};

class CUserNoviceGuideStrategy: public IUserNoviceGuideStrategy{
public:
	CUserNoviceGuideStrategy():m_db_user_novice_guide(NULL), m_pUser(NULL){}
	virtual bool Init(CUser* pUser, db_user_novice_guide_array* guide_arr);
	virtual bool IsGuideDone(UINT32 iGuideID);
	virtual bool SetGuideDone(UINT32 iGuideID);
	virtual void FinishNoviceGuide(UINT32 iGuideID, gs_dbs_user_info_op_req& db_req, gs_c_finish_novice_guide_res& res, GS_C_NTF& ntf);
	virtual bool TriggerGuidMail(UINT32 iLastLevel, UINT32 iNowLevle, gs_dbs_user_info_op_req& db_req);
private:
	CUser* m_pUser;
	db_user_novice_guide_array* m_db_user_novice_guide;
	std::set<UINT32> m_setDoneGuideIDS;			//完成的新手引导id	
};

#endif
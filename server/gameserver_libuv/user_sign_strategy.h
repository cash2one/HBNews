#ifndef user_sign_strategy_h__
#define user_sign_strategy_h__

#include "dbs_gs_proto.pb.h"
#include "def.h"
#include "config.h"
class CUser;
class IUserSignStrategy{
public:
	virtual ~IUserSignStrategy(){};
	virtual bool Init(CUser* pUser, db_user_info& user_info, gs_dbs_user_info_op_req& db_req) = 0;
	virtual bool GetSignInfo(user_sign& sign) = 0;
	virtual e_gsc_errorcode Sign_1(gs_dbs_user_info_op_req& db_req, gs_c_sign_res& res, GS_C_NTF& ntf) = 0;
	virtual e_gsc_errorcode GetSignNReward(sign_n_reward::SIGN_N_REWARD_TYPE eType, gs_dbs_user_info_op_req& db_req, gs_c_sign_res& res, GS_C_NTF& ntf) = 0;
	virtual e_gsc_errorcode AddSign(gs_dbs_user_info_op_req& db_req, gs_c_sign_add_res& res, GS_C_NTF& ntf) = 0;
};



class CUserSignStrategy : public IUserSignStrategy
{
public:
	CUserSignStrategy(void):m_user_sign(NULL), m_pUser(NULL){};

	virtual bool Init(CUser* pUser, db_user_info& user_info, gs_dbs_user_info_op_req& db_req);
	virtual bool GetSignInfo(user_sign& sign);
	virtual e_gsc_errorcode Sign_1(gs_dbs_user_info_op_req& db_req, gs_c_sign_res& res, GS_C_NTF& ntf);
	virtual e_gsc_errorcode GetSignNReward(sign_n_reward::SIGN_N_REWARD_TYPE eType, gs_dbs_user_info_op_req& db_req, gs_c_sign_res& res, GS_C_NTF& ntf);
	virtual e_gsc_errorcode AddSign(gs_dbs_user_info_op_req& db_req, gs_c_sign_add_res& res, GS_C_NTF& ntf);
private:
	//检测补签(如果是这个月之前的记录，如果满足补签条件(vip,补签次数)则进行补签，获得的补签物品和达到的累计补签物品通过邮件发送)
	bool _check_add_sign(gs_dbs_user_info_op_req& db_req,  GS_C_NTF& ntf);

	//检测是否是本月之前的签到记录，如果是无效的签到重置签到数据
	bool _check_invalid_sign(gs_dbs_user_info_op_req& db_req);

	void _fill_sign_n_status(user_sign& sign);
	db_user_sign* m_user_sign;
	CUser* m_pUser;
};

#endif
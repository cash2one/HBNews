#include <vector>
#include "user_sign_strategy.h"
#include "time_helper.h"
#include "sys_setting_mgr.h"
#include "sign_mgr.h"
#include "user.h"
#include "gs_c_proto.pb.h"
#include "server_for_user.h"
bool CUserSignStrategy::Init(CUser* pUser,  db_user_info& user_info, gs_dbs_user_info_op_req& db_req)
{
	m_pUser = pUser;

	if(user_info.has_sign())
	{
		m_user_sign = user_info.mutable_sign();

		GS_C_NTF ntf;
		_check_add_sign(db_req, ntf);
		_check_invalid_sign(db_req);
	}
	else{
		m_user_sign = user_info.mutable_sign();
		m_user_sign->set_sign_1_times(0);
		m_user_sign->set_sign_n_reward_status("0000");
		m_user_sign->set_last_sign_time(0);
		m_user_sign->set_add_signed_times(0);

		db_req.mutable_new_sign()->set_sign_1_times(0);
		db_req.mutable_new_sign()->set_sign_n_reward_status("0000");
		db_req.mutable_new_sign()->set_last_sign_time(0);
		db_req.mutable_new_sign()->set_add_signed_times(0);
	}
	return true;
}	

e_gsc_errorcode CUserSignStrategy::Sign_1( gs_dbs_user_info_op_req& db_req, gs_c_sign_res& res, GS_C_NTF& ntf)
{
	//检测补签
	_check_add_sign(db_req, ntf);
	//检测是否是否需要重置签到记录
	_check_invalid_sign(db_req);

	//判断是否已经签到
	UINT32 iResetTime = 0;
	CSysSettingMgr::GetInstance()->GetIntValue("Global_Reset_Time", iResetTime);
	if(isSameDay(m_user_sign->last_sign_time(), Now(),iResetTime)){
		return e_gsc_errorcode_sign_already_sign_today;
	}

	//签到
	tm t_now = GetNowTM(Now());
	stGameElement* pGameEle = CSignMgr::GetInstance()->GetSign1Reward(t_now.tm_year, t_now.tm_mon, m_user_sign->sign_1_times());
	ASSERT(pGameEle);
	if (!pGameEle)
	{
		return e_gsc_errorcode_unknown_error;
	}

	m_pUser->UpdateGameElement(*pGameEle, db_req, *res.mutable_eles(), ntf);
	
	db_req.mutable_sign()->set_op_type(gs_dbs_user_info_op_req::Update);
	db_req.mutable_sign()->set_sign_1_times(m_user_sign->sign_1_times() + 1);
	db_req.mutable_sign()->set_last_sign_time(Now());

	m_user_sign->set_sign_1_times(m_user_sign->sign_1_times() + 1);
	m_user_sign->set_last_sign_time(Now());

	return e_gsc_errorcode_success;
}

e_gsc_errorcode CUserSignStrategy::GetSignNReward(sign_n_reward::SIGN_N_REWARD_TYPE eType, gs_dbs_user_info_op_req& db_req, gs_c_sign_res& res, GS_C_NTF& ntf)
{
	//检测补签
	_check_add_sign(db_req, ntf);
	//检测是否是否需要重置签到记录
	_check_invalid_sign(db_req);

	//查看是否已经领取
	std::string& status = *m_user_sign->mutable_sign_n_reward_status();

	int type = 0;
	switch(eType)
	{
	case sign_n_reward::SIGN_N_REWARD_TYPE_FIRST:
		type = 0;
		break;
	case sign_n_reward::SIGN_N_REWARD_TYPE_SECOND:
		type = 1;
		break;
	case sign_n_reward::SIGN_N_REWARD_TYPE_THREE:
		type = 2;
		break;
	case sign_n_reward::SIGN_N_REWARD_TYPE_FOUR:
		type = 3;
		break;
	default:
		ASSERT(false);
		return e_gsc_errorcode_unknown_error;
	}
	
	char cstatus = status[type];
	if (cstatus == '1')
	{
		return e_gsc_errorcode_sign_already_reward;
	}
	
	bool r = false;
	UINT32 iSignTimes = m_user_sign->sign_1_times();
	//判断条件是否满足
	switch(eType)
	{
	case sign_n_reward::SIGN_N_REWARD_TYPE_FIRST:
		r = iSignTimes >= SIGN_N_FIRST_TIMES;
		break;
	case sign_n_reward::SIGN_N_REWARD_TYPE_SECOND:
		r = iSignTimes >= SIGN_N_SECOND_TIMES;
		break;
	case sign_n_reward::SIGN_N_REWARD_TYPE_THREE:
		r = iSignTimes >= SIGN_N_THREE_TIMES;
		break;
	case sign_n_reward::SIGN_N_REWARD_TYPE_FOUR:
		r = iSignTimes >= SIGN_N_FOUR_TIMES;
		break;
	default:
		ASSERT(false);
		return e_gsc_errorcode_unknown_error;
	}
	if (!r)
	{
		return e_gsc_errorcode_sign_confition_not_satisfy;
	}
	
	status[type] = '1';
	db_req.mutable_sign()->set_sign_n_reward_status(status);
	db_req.mutable_sign()->set_op_type(gs_dbs_user_info_op_req::Update);
	m_user_sign->set_sign_n_reward_status(status);
	
	tm t_now = GetNowTM(Now());

	stGameElement* pGameEle = CSignMgr::GetInstance()->GetSignNReward(t_now.tm_year, t_now.tm_mon, type);
	ASSERT(pGameEle);
	if(!pGameEle)
	{
		return e_gsc_errorcode_unknown_error;
	}
	//pGameEle->Fill(*res.add_eles());
	m_pUser->UpdateGameElement(*pGameEle, db_req, *res.mutable_eles(), ntf);
	return e_gsc_errorcode_success;
}



e_gsc_errorcode CUserSignStrategy::AddSign( gs_dbs_user_info_op_req& db_req, gs_c_sign_add_res& res, GS_C_NTF& ntf)
{
	//查看签到记录是否需要重置(判断最后领取时间是否是上个月或者之前)
	_check_add_sign(db_req, ntf);
	_check_invalid_sign(db_req);
	//查看vip等级
	UINT32 iNeedAddSignVip = 0;
	CSysSettingMgr::GetInstance()->GetIntValue("Vip_Level_Function_Check_In_Cover", iNeedAddSignVip);
	if (m_pUser->GetVipLevel() < iNeedAddSignVip)
	{
		return e_gsc_errorcode_vip_level_not_reach;
	}

	//查看是是否有漏签的
	tm t = GetTM(Now());
	UINT32 iNeedAddTimes = t.tm_mday - m_user_sign->sign_1_times();
	ASSERT(iNeedAddTimes >= 0);
	if (0 == iNeedAddTimes)
	{
		return e_gsc_errorcode_sign_no_need_add;
	}

	//查看剩余次数
	UINT32 iTotalTimes = 0;
	ASSERT(CSysSettingMgr::GetInstance()->GetIntValue("CheckIn_Retrieve_Num_Per_Month", iTotalTimes));
	UINT32 iLeftAddTimes = iTotalTimes - m_user_sign->add_signed_times();
	if (0 == iLeftAddTimes)
	{
		return e_gsc_errorcode_sign_no_left_add_times;
	}

	//每次补签，签到一次
	tm t_now = GetNowTM(Now());
	stGameElement* pGameEle = CSignMgr::GetInstance()->GetSign1Reward(t_now.tm_year, t_now.tm_mon, m_user_sign->sign_1_times());
	ASSERT(pGameEle);
	if (!pGameEle)
	{
		return e_gsc_errorcode_unknown_error;
	}
	m_pUser->UpdateGameElement(*pGameEle, db_req, *res.mutable_eles(), ntf);
	//	vecGameEle.push_back(pGameEle);
	//UINT32 iSignedTimes = m_user_sign->sign_1_times() + 1;
	//UINT32 iReachTimes = iSignedTimes + iLeftAddTimes > t.tm_mday ? t.tm_mday : iSignedTimes + iLeftAddTimes;
	//std::vector<stGameElement*> vecGameEle;
	//for (int i = iSignedTimes - 1; i < iReachTimes - 1; ++i)
	//{
	//	tm t_now = GetNowTM(Now());
	//	stGameElement* pGameEle = CSignMgr::GetInstance()->GetSign1Reward(t_now.tm_year, t_now.tm_mon, i);
	//	ASSERT(pGameEle);
	//	if (!pGameEle)
	//	{
	//		return e_gsc_errorcode_unknown_error;
	//	}
	//	vecGameEle.push_back(pGameEle);
	//}

	m_user_sign->set_add_signed_times(m_user_sign->add_signed_times() + 1);
	m_user_sign->set_sign_1_times(m_user_sign->sign_1_times() + 1);
	
	db_req.mutable_sign()->set_sign_1_times(m_user_sign->sign_1_times());
	db_req.mutable_sign()->set_add_signed_times(m_user_sign->add_signed_times());
	db_req.mutable_sign()->set_op_type(gs_dbs_user_info_op_req::Update);
	res.set_left_add_sign_times(iTotalTimes - m_user_sign->add_signed_times());
	return e_gsc_errorcode_success;
}


bool CUserSignStrategy::GetSignInfo(user_sign& sign)
{
	sign.set_signed_1_times(m_user_sign->sign_1_times());
	
	UINT32 iVip = 0;
	ASSERT(CSysSettingMgr::GetInstance()->GetIntValue("Vip_Level_Function_Check_In_Cover", iVip));
	if (m_pUser->GetVipLevel() < iVip)
	{
		sign.set_left_add_sign_times(0);
	}
	else
	{
		UINT32 iTimes = 0;
		ASSERT(CSysSettingMgr::GetInstance()->GetIntValue("CheckIn_Retrieve_Num_Per_Month", iTimes));
		sign.set_left_add_sign_times(iTimes  - m_user_sign->add_signed_times());
	}
	
	UINT32 iResetTime = 0;
	ASSERT(CSysSettingMgr::GetInstance()->GetIntValue("Global_Reset_Time", iResetTime));

	if(m_user_sign->last_sign_time()  == 0)
		sign.set_is_already_sign(false);
	else if(isSameDay(m_user_sign->last_sign_time(), Now(), iResetTime))
	{
		sign.set_is_already_sign(true);
	}
	else
	{
		sign.set_is_already_sign(false);
	}

	_fill_sign_n_status(sign);

	return true;
}



bool CUserSignStrategy::_check_add_sign(gs_dbs_user_info_op_req& db_req, GS_C_NTF& ntf)
{
	//是不是没有签到记录
	if(m_user_sign->last_sign_time() == 0)
		return false;
	
	//判断时间
	time_t now = Now();
	tm  t_last_sign_time = GetNowTM(m_user_sign->last_sign_time());
	tm  t_now = GetNowTM(now);
	if (t_last_sign_time.tm_year * 12 + t_last_sign_time.tm_mon == t_now.tm_year * 12 + t_now.tm_mon)//当月
	{
		return false;
	}

	//判断vip等级
	UINT32 iVip = 0;
	bool r = CSysSettingMgr::GetInstance()->GetIntValue("Vip_Level_Function_Check_In_Cover", iVip);
	ASSERT(r);
	if (m_pUser->GetVipLevel() < iVip)
	{
		return false;
	}

	//判断是否有可用补签次数
	UINT32 iTotalSignedTimes = 0;
	r = CSysSettingMgr::GetInstance()->GetIntValue("CheckIn_Retrieve_Num_Per_Month", iTotalSignedTimes);
	ASSERT(r);
	ASSERT(m_user_sign->add_signed_times() <= iTotalSignedTimes);
	if (m_user_sign->add_signed_times() == iTotalSignedTimes)
	{
		return false;
	}

	
	//判断是否有漏签的
	UINT32 iDays = GetDaysInMonth(t_last_sign_time.tm_year, t_last_sign_time.tm_mon);
	if(m_user_sign->sign_1_times() == iDays)
	{
		return false;
	}
	

	//判断可领取的次数区间，和可累计领取的类型
	UINT32 iCanUse = iTotalSignedTimes - m_user_sign->add_signed_times();
	UINT32 iBeginTimes = m_user_sign->sign_1_times() + 1;
	UINT32 iEndTimes = m_user_sign->sign_1_times() + iCanUse > iDays ? iDays :  m_user_sign->sign_1_times() + iCanUse;

	std::vector<UINT32> vecSignType;
	std::string status = m_user_sign->sign_n_reward_status();
	if (iEndTimes > SIGN_N_FIRST_TIMES && status[0] == '0')
	{
		vecSignType.push_back(0);
	}

	if (iEndTimes > SIGN_N_SECOND_TIMES && status[1] == '0')
	{
		vecSignType.push_back(1);
	}

	if(iEndTimes > SIGN_N_THREE_TIMES && status[2] == '0')
	{
		vecSignType.push_back(2);
	}

	if(iEndTimes > SIGN_N_FOUR_TIMES && status[3] == '0')
	{
		vecSignType.push_back(3);
	}

	
	UINT32 iYear = t_last_sign_time.tm_year;
	UINT32 iMonth = t_last_sign_time.tm_mon;
	
	//领取签到奖励
	std::vector<stGameElement*> vecEle;
	for (int i = iBeginTimes - 1; i <= iEndTimes - 1; ++i)
	{
		stGameElement* pEle = CSignMgr::GetInstance()->GetSign1Reward(iYear, iMonth, i);
		ASSERT(pEle);
		if (!pEle)
		{
			return false;
		}
		vecEle.push_back(pEle);
	}
	
	//领取累计签到奖励
	for (size_t i = 0; i < vecSignType.size(); ++i)
	{
		stGameElement* pEle = CSignMgr::GetInstance()->GetSignNReward(iYear, iMonth, i);
		ASSERT(pEle);
		if (!pEle)
		{
			return false;
		}
		vecEle.push_back(pEle);
	}

	gs_dbs_user_info_op_req* db_req = new gs_dbs_user_info_op_req;
	db_req->set_user_id(m_pUser->UserID());
	m_pUser->CreateMail(MAIL_TYPE_PRE_MONTH_ADD_SIGN, *db_req, NULL, NULL, &vecEle);
	CServerForUser::GetInstance()->SendDBReq(db_req);
	return true;
}

bool CUserSignStrategy::_check_invalid_sign(gs_dbs_user_info_op_req& db_req)
{
	//是不是没有签到记录
	if(m_user_sign->last_sign_time() == 0)
		return false;

	//判断时间
	time_t now = Now();
	tm  t_last_sign_time = GetNowTM(m_user_sign->last_sign_time());
	tm  t_now = GetNowTM(now);
	if (t_last_sign_time.tm_year * 12 + t_last_sign_time.tm_mon == t_now.tm_year * 12 + t_now.tm_mon)//当月
	{
		return false;
	}

	db_req.mutable_sign()->set_sign_1_times(0);
	db_req.mutable_sign()->set_sign_n_reward_status("0000");
	db_req.mutable_sign()->set_last_sign_time(0);
	db_req.mutable_sign()->set_add_signed_times(0);
	db_req.mutable_sign()->set_op_type(gs_dbs_user_info_op_req::Update);

	m_user_sign->set_sign_1_times(0);
	m_user_sign->set_sign_n_reward_status("0000");
	m_user_sign->set_last_sign_time(0);
	m_user_sign->set_add_signed_times(0);
	return true;
}


void CUserSignStrategy::_fill_sign_n_status( user_sign& sign )
{
	std::string status = m_user_sign->sign_n_reward_status();
	ASSERT(status.length() == 4);
	for (int i = 0; i < status.length(); ++i)
	{
		sign_n_reward* reward = sign.add_signed_n_reward();
		if(0 == i)
		{
			reward->set_reward_type(sign_n_reward::SIGN_N_REWARD_TYPE_FIRST);
			if(status[i] == '0')
			{
				if(m_user_sign->sign_1_times() >= SIGN_N_FIRST_TIMES){
					reward->set_status(sign_n_reward::STATUS_CAN_ATTACH);
				}
				else
				{
					reward->set_status(sign_n_reward::STATUS_CONDITION_NOT_SATISFY);
				}
			}
			else if(status[i] == '1'){
				reward->set_status(sign_n_reward::STATUS_ALREADY_GET_ATTACH);
			}
			
		}
		else if(1 == i)
		{
			reward->set_reward_type(sign_n_reward::SIGN_N_REWARD_TYPE_SECOND);
			if(status[i] == '0')
			{
				if(m_user_sign->sign_1_times() >= SIGN_N_SECOND_TIMES){
					reward->set_status(sign_n_reward::STATUS_CAN_ATTACH);
				}
				else{
					reward->set_status(sign_n_reward::STATUS_CONDITION_NOT_SATISFY);
				}
			}
			else if(status[i] == '1'){
				reward->set_status(sign_n_reward::STATUS_ALREADY_GET_ATTACH);
			}
			
		}
		else if(2 == i)
		{
			reward->set_reward_type(sign_n_reward::SIGN_N_REWARD_TYPE_THREE);
			if(status[i] == '0')
			{
				if(m_user_sign->sign_1_times() >= SIGN_N_THREE_TIMES){
					reward->set_status(sign_n_reward::STATUS_CAN_ATTACH);
				}
				else{
					reward->set_status(sign_n_reward::STATUS_CONDITION_NOT_SATISFY);
				}
			}
			else if(status[i] == '1'){
				reward->set_status(sign_n_reward::STATUS_ALREADY_GET_ATTACH);
			}
		}
		else if(3 == i)
		{
			reward->set_reward_type(sign_n_reward::SIGN_N_REWARD_TYPE_FOUR);
			if(status[i] == '0')
			{
				if(m_user_sign->sign_1_times() >= SIGN_N_FOUR_TIMES){
					reward->set_status(sign_n_reward::STATUS_CAN_ATTACH);
				}
				else{
					reward->set_status(sign_n_reward::STATUS_CONDITION_NOT_SATISFY);
				}
			}
			else if(status[i] == '1'){
				reward->set_status(sign_n_reward::STATUS_ALREADY_GET_ATTACH);
			}
		}		
		else{
			ASSERT(false);
			reward->set_status(sign_n_reward::STATUS_CONDITION_NOT_SATISFY);
		}
	}
}


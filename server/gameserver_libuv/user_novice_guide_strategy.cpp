#include "user_novice_guide_strategy.h"
#include "novice_guide_mgr.h"
#include "user.h"
#include "sys_setting_mgr.h"
bool CUserNoviceGuideStrategy::Init(CUser* pUser, db_user_novice_guide_array* guide_arr)
{
	ASSERT(pUser && guide_arr);
	m_pUser = pUser;
	m_db_user_novice_guide = guide_arr;
	::google::protobuf::RepeatedField< ::google::protobuf::uint32 >::iterator iter = m_db_user_novice_guide->mutable_guide_ids()->begin();
	for (; iter != m_db_user_novice_guide->mutable_guide_ids()->end(); ++iter)
	{
		std::set<UINT32>::iterator it = m_setDoneGuideIDS.find(*iter);
		ASSERT(it == m_setDoneGuideIDS.end());
		CHECK_FALSE_RETURN_FALSE(it == m_setDoneGuideIDS.end());
		m_setDoneGuideIDS.insert(*iter);
	}
	return true;
}

bool CUserNoviceGuideStrategy::IsGuideDone( UINT32 iGuideID )
{
	return m_setDoneGuideIDS.find(iGuideID) != m_setDoneGuideIDS.end();
}

bool CUserNoviceGuideStrategy::SetGuideDone(UINT32 iGuideID)
{
	std::set<UINT32>::iterator it = m_setDoneGuideIDS.find(iGuideID);
	ASSERT(it == m_setDoneGuideIDS.end());
	CHECK_FALSE_RETURN_FALSE(it == m_setDoneGuideIDS.end());
	m_setDoneGuideIDS.insert(iGuideID);

	m_db_user_novice_guide->add_guide_ids(iGuideID);
	return true;
}

void CUserNoviceGuideStrategy::FinishNoviceGuide( UINT32 iGuideID, gs_dbs_user_info_op_req& db_req, gs_c_finish_novice_guide_res& res, GS_C_NTF& ntf)
{
	stGuide* pGuid = CNoviceGuideMgr::GetInstance()->GetGuideByGuideID(iGuideID);
	if (!pGuid)
	{
		res.set_error_code(e_gsc_errorcode_unknown_error);
		return;
	}

	if (IsGuideDone(iGuideID))
	{
		res.set_error_code(e_gsc_errorcode_guide_already_finish);
		return;
	}
	
	//判断触发条件是否满足
	if(!IsConditionsSatisfy(pGuid->triggerCondition, m_pUser->UserID()))
	{
		res.set_error_code(e_gsc_errorcode_guide_trigger_condition_not_satisfy);
		return;
	}

	//查看前置引导是否完成
	/*if(pGuid->iPreGuildID)
	{
		if(!IsGuideDone(pGuid->iPreGuildID))
		{
			res.set_error_code(e_gsc_errorcode_guide_pre_guide_not_finish);
			return;
		}
	}*/
	

	SetGuideDone(iGuideID);
	
	res.set_error_code(e_gsc_errorcode_success);
	res.set_guide_id(iGuideID);

	db_req.mutable_novice_guide()->set_op_type(gs_dbs_user_info_op_req::Add);
	db_req.mutable_novice_guide()->set_guide_id(iGuideID);
	
	

}

bool CUserNoviceGuideStrategy::TriggerGuidMail( UINT32 iLastLevel, UINT32 iNowLevle, gs_dbs_user_info_op_req& db_req)
{
	UINT32 guidMailLevel = 0;
	ASSERT(CSysSettingMgr::GetInstance()->GetIntValue("Guid_Send_Mail_Level", guidMailLevel));

	if (guidMailLevel > iLastLevel && guidMailLevel <= iNowLevle)
	{
		const char* str = NULL;
		bool r = CSysSettingMgr::GetInstance()->GetStringValue("Mail_Guide_Attach", str);
		ASSERT(r && str);
		if(!str || !r){
			return false;
		}
		std::vector<stGameElement> mailAttach;
		r = stGameElement::Parse(str, mailAttach, stGameElement::INCREASE);
		ASSERT(r);
		if (!r)
		{
			return false;
		}
		m_pUser->CreateMail(MAIL_TYPE_MAIL_GUIDE, db_req, NULL, NULL, &mailAttach);
	}
	return true;
}

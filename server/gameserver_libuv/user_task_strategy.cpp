#include <queue>
#include "user_task_strategy.h"

#include "task_mgr.h"
#include "user.h"

#include "dbs_gs_proto.pb.h"
#include "time_helper.h"
#include "sys_setting_mgr.h"
#include "vip_mgr.h"
#include "user.h"
#include "burst_mgr.h"
CUserTaskStrategy::~CUserTaskStrategy()
{
	mMapTaskID2SubTaskList.clear();
	mMapSubTaskID2SubTask.clear();
	m_pUser = NULL;
}


bool CUserTaskStrategy::Init(CUser* pUser,  db_user_sub_task_array* sub_task_info )
{	
	m_pUser = pUser;
	// todo �����������
	ASSERT(sub_task_info);
	m_sub_tasks = sub_task_info;
	::google::protobuf::RepeatedPtrField< ::db_user_sub_task >::iterator it = sub_task_info->mutable_items()->begin();
	for (; it != sub_task_info->mutable_items()->end(); ++it)
	{
		db_user_sub_task*  sub_task= &(*it);

		ASSERT(mMapSubTaskID2SubTask.find(sub_task->sub_task_id()) == mMapSubTaskID2SubTask.end());
		mMapSubTaskID2SubTask[sub_task->sub_task_id()] = sub_task;

		stSubTask* task = CTaskMgr::GetInstance()->GetSubTaskBySubTaskID(sub_task->sub_task_id());
		ASSERT(task);
		if (task)
		{
			mMapTaskID2SubTaskList[task->iTaskID].push_back(sub_task);
		}
		else{
			printf("error! CUserTaskStrategy::Init subtask[%u] no parent task", sub_task->sub_task_id());
		}
	}

	return true;
}

e_gsc_errorcode CUserTaskStrategy::GetChapterInfo( stUserChapterInfo& userChapterInfo )
{
	userChapterInfo.iCurrentChapter = 1;
	userChapterInfo.iCurrentMainTaskID = 0;

	std::map<UINT16, vector<CTask*>>& chapterMainTask = CTaskMgr::GetInstance()->GetMapChapterID2MainTaskList();

	//todo ��ȡ��ǰ������½ڣ�
	while (true)
	{
		map<UINT16, vector<CTask*>>::iterator it = chapterMainTask.find(userChapterInfo.iCurrentChapter);
		if (it != chapterMainTask.end())
		{
			vector<CTask*> & mainTaskList  = it->second;

			for (vector<CTask*>::iterator itIn = mainTaskList.begin() ; itIn != mainTaskList.end() ; ++itIn)
			{	
				//todo �ж� *itIn �Ƿ�Ϊ��
				CTask * pTask  = (*itIn);
				if (!pTask)
				{
					//log error��
					//return 0;
					return e_gsc_errorcode_unknown_error;
				}

				if(!IsTaskDone(pTask->iTaskID)){
					//return currentChapter;
					//userChapterInfo.pCurrentMainTask = pTask;
					userChapterInfo.iCurrentMainTaskID = pTask->iTaskID;
					goto END;
				}
			}

		}
		else{//˵����һ�½ڵ��������񶼹��� 
			break;
		}

		++userChapterInfo.iCurrentChapter;//ע�� ������Ϊ��ǰ�߻����õ�����½� + 1��
	}

END:
	return e_gsc_errorcode_success;
}

bool CUserTaskStrategy::IsSubTaskDone( UINT32 iSubTaskID )
{
	std::map<UINT32, db_user_sub_task*>::iterator itIn = mMapSubTaskID2SubTask.find(iSubTaskID);
	if(itIn != mMapSubTaskID2SubTask.end() && itIn->second){
		//return pUserSubTask->iStatus == EUSERSUBTASKSTATUS_DONE;
		return true;
	}

	return false;
}

bool CUserTaskStrategy::IsTaskDone( UINT32 iTaskID )
{
	map<UINT32, vector<db_user_sub_task*>>::iterator itIn = mMapTaskID2SubTaskList.find(iTaskID);
	if (itIn == mMapTaskID2SubTaskList.end())
	{
		return false;
	}

	std::vector<db_user_sub_task*>& user_sub_task_list = itIn->second;
	std::vector<stSubTask*> sub_task_list;
	CTaskMgr::GetInstance()->GetSubTaskListByTaskID(iTaskID, sub_task_list);//���������ñ�
	ASSERT(sub_task_list.size() >= 1);
	CHECK_NULL_RETURN_FALSE(sub_task_list.size() >= 1);

	//�������еĶ������ �Ӻ�����ǰ���ұȽϿ����д�
	for (vector<stSubTask*>::iterator it_sub = sub_task_list.begin() ; it_sub != sub_task_list.end() ; ++ it_sub)
	{
		//stSubTask* pSubTask = *it_sub;
		ASSERT(*it_sub);
		CHECK_NULL_RETURN_FALSE(*it_sub);
		db_user_sub_task* pUserSubTask = NULL;
		bool is_done = false;
		for (vector<db_user_sub_task*>::iterator it_user_sub = user_sub_task_list.begin() ; it_user_sub != user_sub_task_list.end() ; ++it_user_sub)
		{
			ASSERT(*it_user_sub);
			CHECK_NULL_RETURN_FALSE(*it_user_sub);
			if ( (*it_user_sub)->sub_task_id() == (*it_sub)->iSubTaskID )
			{
				is_done = true;
				break;
			}
		}

		//û���ҵ����������������Ϣ��
		if (!is_done)
		{
			return false;
		}
	}

	return true;
}

UINT32 CUserTaskStrategy::GetSubTaskLeftEnterNum(const UINT32 iSubTaskID, UINT32 iVipLevel){

	stSubTask* pSubTask = CTaskMgr::GetInstance()->GetSubTaskBySubTaskID(iSubTaskID);
	ASSERT(pSubTask);
	CHECK_NULL_RETURN_FALSE(pSubTask);

	CTask* pTask = CTaskMgr::GetInstance()->GetTaskByTaskID(pSubTask->iTaskID);
	ASSERT(pTask);
	CHECK_NULL_RETURN_FALSE(pTask);

	UINT32 iTotalEnterNumPerDay = 0;
	UINT32 iEnterNumToday = 0;

	switch (pTask->eType)
	{
	case CTask::ETASKTYPE_BRANCH:
	case CTask::ETASKTYPE_MAIN:
		{//�� �������ÿ�ս����������

			if( pSubTask->iTotalEnterNumPerDay == 0){//���������޴�������
				return -1;
			}

			db_user_sub_task* sub_task = GetSubTask(iSubTaskID);
			if (NULL == sub_task){//û��������������
				return pSubTask->iTotalEnterNumPerDay;
			}

			iTotalEnterNumPerDay = pSubTask->iTotalEnterNumPerDay;

			if(isSameDay(Now(), sub_task->last_enter_timestamp(), CSysSettingMgr::GetInstance()->Global_Reset_Time)){
				iEnterNumToday = sub_task->last_day_enter_num();
			}
			else{
				iEnterNumToday = 0;
			}
		}
		break;
	case CTask::ETASKTYPE_ACTIVITY:
		{//�� �������ÿ�ս����������

			if( pTask->iTotalEnterNumPerDay == 0){
				return -1;
			}

			iTotalEnterNumPerDay = pTask->iTotalEnterNumPerDay;

			std::map<UINT32, std::vector<db_user_sub_task*>>::iterator it = mMapTaskID2SubTaskList.find(pTask->iTaskID);
			if (it != mMapTaskID2SubTaskList.end()){
				for(std::vector<db_user_sub_task*>::iterator itIn = it->second.begin(); itIn != it->second.end() ; ++itIn){
					db_user_sub_task* db_task = *itIn;
					if (db_task){
						if(isSameDay(Now(), db_task->last_enter_timestamp(), CSysSettingMgr::GetInstance()->Global_Reset_Time)){
							iEnterNumToday += db_task->last_day_enter_num();
						}
					}
				}
			}
		}
		break;
	case CTask::ETASKTYPE_EXP:
		{
			CVipCfg* cfg = CVipMgr::GetInstance()->GetVipCfgByVipLevel(iVipLevel);
			ASSERT(cfg);

			if (cfg->m_max_num_exp_battle == 0){
				return -1;//����
			}

			iTotalEnterNumPerDay = cfg->m_max_num_exp_battle;
			db_user_sub_task* sub_task = GetSubTask(iSubTaskID);
			if (NULL == sub_task){//û��������������
				return iTotalEnterNumPerDay;
			}

			if(isSameDay(Now(), sub_task->last_enter_timestamp(), CSysSettingMgr::GetInstance()->Global_Reset_Time)){
				iEnterNumToday = sub_task->last_day_enter_num();
			}
			else{
				iEnterNumToday = 0;
			}
		}
		break;
	case CTask::ETASKTYPE_GOLD:
		{
			CVipCfg* cfg = CVipMgr::GetInstance()->GetVipCfgByVipLevel(iVipLevel);
			ASSERT(cfg);

			if (cfg->m_max_num_gold_battle == 0){
				return -1;//����
			}

			iTotalEnterNumPerDay = cfg->m_max_num_gold_battle;

			db_user_sub_task* sub_task = GetSubTask(iSubTaskID);
			if (NULL == sub_task){//û��������������
				return iTotalEnterNumPerDay;
			}

			if(isSameDay(Now(), sub_task->last_enter_timestamp(), CSysSettingMgr::GetInstance()->Global_Reset_Time)){
				iEnterNumToday = sub_task->last_day_enter_num();
			}
			else{
				iEnterNumToday = 0;
			}
		}
		break;

	default:
		ASSERT(false && "��֧�ֵ���������");
		return 0;
	}


	ASSERT( iEnterNumToday <= iTotalEnterNumPerDay);
	if (iTotalEnterNumPerDay >= iEnterNumToday){
		return iTotalEnterNumPerDay - iEnterNumToday;
	}
	else{
		return 0;
	}

	return 0;
}

e_gsc_errorcode CUserTaskStrategy::SpeedKillSubTask(UINT32 iSubTaskID, UINT32 iTimes, ::google::protobuf::RepeatedPtrField< ::struct_game_element >& res_consumes, ::google::protobuf::RepeatedPtrField< ::struct_game_element_array >& res_output_items, user_shop& res_shop, struct_game_element& req_consume_ele, gs_dbs_user_info_op_req& db_req, GS_C_NTF& ntf,  bool& isShopOpen){
	
	stSubTask* sub_task = CTaskMgr::GetInstance()->GetSubTaskBySubTaskID(iSubTaskID);
	if(!sub_task){//������������
		return e_gsc_errorcode_unknown_error;
	}

	CTask* task = CTaskMgr::GetInstance()->GetTaskByTaskID(sub_task->iTaskID);
	if(!task || task->bHidden){//��⸸������� ����û�����أ�
		return e_gsc_errorcode_unknown_error;
	}

	//�ж�vip ����ɨ��ʱ��ͬ��������Ƶ�������� if else��˳��
	switch (task->eType)
	{
	case CTask::ETASKTYPE_MAIN:
		{
			UINT32 Vip_Level_Function_Speed_Kill_Main_Task = -1;
			bool r = CSysSettingMgr::GetInstance()->GetIntValue("Vip_Level_Function_Speed_Kill_Main_Task", Vip_Level_Function_Speed_Kill_Main_Task);
			ASSERT(r);
			if (m_pUser->GetVipLevel() < Vip_Level_Function_Speed_Kill_Main_Task){
				return e_gsc_errorcode_vip_level_not_reach;
			}
		}
		break;
	case CTask::ETASKTYPE_BRANCH:
		{
			UINT32 Vip_Level_Function_Speed_Kill_Free_Task = -1;
			bool r = CSysSettingMgr::GetInstance()->GetIntValue("Vip_Level_Function_Speed_Kill_Free_Task", Vip_Level_Function_Speed_Kill_Free_Task);
			ASSERT(r);
			if (m_pUser->GetVipLevel() < Vip_Level_Function_Speed_Kill_Free_Task){
				return e_gsc_errorcode_vip_level_not_reach;
			}
		}
		break;
	case CTask::ETASKTYPE_ACTIVITY:
	case CTask::ETASKTYPE_EXP:
	case CTask::ETASKTYPE_GOLD:
		{
			UINT32 Vip_Level_Function_Speed_Kill_Activity_Task = -1;
			bool r = CSysSettingMgr::GetInstance()->GetIntValue("Vip_Level_Function_Speed_Kill_Activity_Task", Vip_Level_Function_Speed_Kill_Activity_Task);
			ASSERT(r);
			if (m_pUser->GetVipLevel() < Vip_Level_Function_Speed_Kill_Activity_Task){
				return e_gsc_errorcode_vip_level_not_reach;
			}
		}
		break;
	default:
		ASSERT(false && "��֧�ֵ���������");
		return e_gsc_errorcode_unknown_error;
	}

	//�ж��Ǽ�
	db_user_sub_task* pSubTask = GetSubTask(iSubTaskID);
	if (!pSubTask || pSubTask->star() < 3){//�Ǽ�����
		return e_gsc_errorcode_speed_kill_not_enough_star;
	}

	//�ж�����
	UINT32 need_health = sub_task->iSpendHealth * iTimes;
	if(m_pUser->GetHealth() < need_health){
		return e_gsc_errorcode_no_enough_health;
	}

	//�ж�ʣ��������
	if(GetSubTaskLeftEnterNum(sub_task->iSubTaskID, m_pUser->GetVipLevel()) < iTimes){
		return e_gsc_errorcode_speed_kill_not_enough_left_enter_num;
	}
	
	IncreaseSubTaskEnterNum(sub_task->iSubTaskID, iTimes, db_req);

	//������
	bool isFull2NotFull = false;
	bool r = m_pUser->DecreaseHealth(need_health, &isFull2NotFull);
	ASSERT(r);

	struct_game_element* ele = res_consumes.Add();
	ele->set_type(struct_game_element::EELEMENTTYPE_HEALTH);
	ele->set_id(0);
	if (isFull2NotFull)
	{
		ele->set_sub_id(m_pUser->GetHealthLastUpdateTime());//����ͻ��˱������������Զ��ظ����ֵ���� �ڿ�ȥ�������������� ����֮�� ��ʹ�����﷢��������ʱ��Ϊ����ʱ���
	}
	else{
		ele->set_sub_id(0);
	}
	ele->set_num_type(struct_game_element::DECREASE);
	ele->set_num(need_health);


	//m_start_sub_task_res.set_health_last_update_timestamp(_get_health_last_update_time());//�Ƶ� ���� element��sub_id�ϡ� ����ͻ��˱������������Զ��ظ����ֵ���� �ڿ�ȥ�������������� ����֮�� ��ʹ�����﷢��������ʱ��Ϊ����ʱ���

	//�޸�db������Ϣ
	gs_dbs_user_info_op_req_struct_op_base_info* base_info = db_req.mutable_base_info();
	base_info->set_op_type(gs_dbs_user_info_op_req::Update);
	base_info->set_health_cur(m_pUser->GetHealth());
	base_info->set_health_last_update_timestamp(m_pUser->GetHealthLastUpdateTime());

	//�۵���

	r = m_pUser->UpdateGameElement(req_consume_ele, db_req, res_consumes, ntf);
	ASSERT(r);

	//�ӵ���
	bool hasRandomOpenShop = false;
	for (UINT32 i = 0 ; i < iTimes ; ++i){
		struct_game_element_array* eles_arr = res_output_items.Add();//����ɨ���Ľ��, ����˴�û�в��� ��ô�����Ǹ��յļ� �ͻ��˿�����ʾ��

		std::vector<stGameElement> bursts;
		bool r = CBurstMgr::GetInstance()->GetBurst(sub_task->iAutoBattleBurstPacketID, bursts);
		ASSERT(r);
		if (!r){
			return e_gsc_errorcode_unknown_error;
		}

		if (sub_task->iExtraPacketID && IsConditionsSatisfy(sub_task->ExtraPacketCondition, m_pUser->UserID())){
			r = CBurstMgr::GetInstance()->GetBurst(sub_task->iExtraPacketID, bursts);
			ASSERT(r);
			if (!r){
				return e_gsc_errorcode_unknown_error;
			}
		}

		r = m_pUser->UpdateGameElement(bursts, db_req, *(eles_arr->mutable_items()), ntf);
		ASSERT(r);

		if (!hasRandomOpenShop && (task->eType == CTask::ETASKTYPE_MAIN || task->eType == CTask::ETASKTYPE_BRANCH)){
			bool r = m_pUser->OnShopRandomOpen(task->eType, db_req, res_shop);
			if (!r){//���û�п��� �̵� ���ͷ��̵�
				//res.set_allocated_shop(NULL);
			}
			else{
				hasRandomOpenShop = true;//�����˾Ͳ���������� û�����ż��������
			}
		}
	}
	isShopOpen = hasRandomOpenShop;
	
	return e_gsc_errorcode_success;
}


bool CUserTaskStrategy::CanStartFight( CUser* pUser, UINT32 iSubTaskID )
{
	//CUser* pUser = CUserMgr::GetInstance()->GetUserByUserID(iUserID);
	CHECK_NULL_RETURN_FALSE(pUser);

	stSubTask* pSubTask = CTaskMgr::GetInstance()->GetSubTaskBySubTaskID(iSubTaskID);
	CHECK_NULL_RETURN_FALSE(pSubTask);

	//1. �����ж�
	CHECK_NULL_RETURN_FALSE(pUser->GetHealth() >= pSubTask->iSpendHealth );

	CTask* pTask = CTaskMgr::GetInstance()->GetTaskByTaskID(pSubTask->iTaskID);
	CHECK_NULL_RETURN_FALSE(pTask);

	//2. �Ƿ������ж�
	if (pTask->bHidden){
		return false;
	}

	//3.��ʾ�����ж� �ɿͻ���������
	//if (!pTask->IsConditionsSatisfy(CTask::EConditionFor_Show, pUser->UserID())){
	//	return false;
	//}

	//4.���������жϣ�
	if (!IsConditionsSatisfy(pTask->enterConditions, pUser->UserID())){
		return false;
	}

	//5.�����Ĵ����޶�
	//if( pTask->iTotalEnterNumPerDay != 0){
	//	UINT32 enter_num_today = 0;
	//	std::map<UINT32, std::vector<db_user_sub_task*>>::iterator it = mMapTaskID2SubTaskList.find(pTask->iTaskID);
	//	if (it != mMapTaskID2SubTaskList.end()){
	//		for(std::vector<db_user_sub_task*>::iterator itIn = it->second.begin(); itIn != it->second.end() ; ++itIn){
	//			db_user_sub_task* db_task = *itIn;
	//			if (db_task){
	//				if(isSameDay(Now(), db_task->last_enter_timestamp(), CSysSettingMgr::GetInstance()->global_reset_time())){
	//					enter_num_today += db_task->last_day_enter_num();
	//				}
	//			}
	//		}
	//	}

	//	if (enter_num_today >= pTask->iTotalEnterNumPerDay)
	//	{
	//		ASSERT( enter_num_today == pTask->iTotalEnterNumPerDay);
	//		return false;
	//	}
	//}

	if (GetSubTaskLeftEnterNum(iSubTaskID, pUser->GetVipLevel()) == 0){
		return false;
	}
	
	if (pTask->iMapPointID){
		if (!CanVisitMapPoint(pTask->iMapPointID)){//���������˽���ȼ��ж�
			return false;
		}
	}
	

	return true;
}

bool CUserTaskStrategy::GetMapPointCanVisit( UINT16 iChapterID, std::set<UINT32> &retPoints, UINT32& curMainTaskPointID )
{
	retPoints.clear();
	curMainTaskPointID = 0;

	//1. ��ʼ��
	//GetUserMaxChapter�������ܴ� ���ⲿʹ�õģ� �ڲ������Լ������������ɣ�
	//��ȡ�������½���Ϣ
	stUserChapterInfo userChapterInfo = {0};
	e_gsc_errorcode ret = GetChapterInfo(userChapterInfo);
	if (ret != e_gsc_errorcode_success)
	{
		return false;
	}

	if (iChapterID > userChapterInfo.iCurrentChapter)
	{
		return false;
	}

	curMainTaskPointID = CTaskMgr::GetInstance()->GetMapPointIDByTaskID(userChapterInfo.iCurrentMainTaskID);

	UINT32 bornMapPointID = CTaskMgr::GetInstance()->GetChapterBornMapPointID(iChapterID);
	retPoints.insert(bornMapPointID);//��ʼID


	//2. ��ͨ����������ĵ�ͼ�ݵ�
	CTaskMgr* gpTaskMgr = CTaskMgr::GetInstance(); 
	map<UINT32, vector<db_user_sub_task*>>& userTaskList = mMapTaskID2SubTaskList;
	for (std::map<UINT32, vector<db_user_sub_task*>>::iterator itIn = userTaskList.begin() ; itIn != userTaskList.end() ; ++itIn)
	{
		UINT32 iTaskID =  itIn->first;
		UINT16 iUserTaskChapterID = CTaskMgr::GetInstance()->GetChapterIDByTaskID(iTaskID);
		if(iUserTaskChapterID != iChapterID){//����ͬһ���½ڵ�����
			continue;
		}

		CTask* pTask = gpTaskMgr->GetTaskByTaskID(iTaskID);
		if(!pTask){//�������ò�����
			//todo log error! ����GetTaskByTaskID����
			//continue;
			return false;
		}

		//�������������ͼ�ݵ�ɷ��ʣ�
		if ( (pTask->eType == CTask::ETASKTYPE_MAIN) && IsTaskDone(iTaskID))
		{
			retPoints.insert(pTask->iMapPointID);//set��ֹ  һ�����϶������
		}
	}

	//3. ��ȡ1,2��һ���ٽ���
	std::queue<UINT32> qMapPoint;
	std::set<UINT32>::iterator itMapPoint = retPoints.begin();
	while ( itMapPoint != retPoints.end())
	{
		qMapPoint.push(*itMapPoint);
		++itMapPoint;
	}
	CMapMgr* pMapMgr = CMapMgr::GetInstance();
	while (!qMapPoint.empty())
	{
		UINT32 iMapPointID = qMapPoint.front();
		qMapPoint.pop();
		//��ȡ�ٽ��ĵ�
		CMapPoint* pMapPoint = pMapMgr->GetMapPointByID(iMapPointID);
		ASSERT(pMapPoint);
		if (!pMapPoint){
			continue;
		}

		for (int i = 0 ; i < MAXNUM_JOINEDMAPPOINT ; ++i)
		{
			UINT32 iJoinedMapPointID = pMapPoint->arrJoinedMapPointID[i];
			if(iJoinedMapPointID && (retPoints.find(iJoinedMapPointID) == retPoints.end()) ){

				CMapPoint* p = CMapMgr::GetInstance()->GetMapPointByID(iJoinedMapPointID);
				if(p){//��֤���������ͼ�ݵ㣡 ʵ�����������Ӧ���ڶ�����ʱ���� �����������һ�±��գ�
					//std::pair<set<UINT32>::iterator, bool>& ins_ret =retPoints.insert(iJoinedMapPointID);
					//if (ins_ret->second)//֮ǰû�е� �µĵ� ��������
					//{
					//	qMapPoint.push(iJoinedMapPointID);
					//}

					//add by baomin 2015.9.22  for enter level limit
					ASSERT(m_pUser);
					if (m_pUser->GetLevel() < p->iEnterLevel){//�ݵ����ȼ�����
						continue;
					}

					retPoints.insert(iJoinedMapPointID);//һ����֮ǰû�еģ�ǰ�������ж� �еľͲ������
					qMapPoint.push(iJoinedMapPointID);
				}
				else{
					//todo log error!  �ڶ����õĵط�����⣡
					return false;
				}
			}
		}
	}

	return true;
}

bool CUserTaskStrategy::CanVisitMapPoint( UINT32 iMapPointID )
{
	//��ȡ�������½���Ϣ
	stUserChapterInfo userChapterInfo = {0};
	e_gsc_errorcode ret = GetChapterInfo(userChapterInfo);
	if (ret != e_gsc_errorcode_success)
	{
		return false;
	}

	UINT16 currentChapter = userChapterInfo.iCurrentChapter;

	//��ȡ�ݵ��½���Ϣ
	CMapPoint* pMapPoint = CMapMgr::GetInstance()->GetMapPointByID(iMapPointID);
	if (!pMapPoint)
	{
		//todo log error
		return false;
	}

	if(pMapPoint->iChapterID < currentChapter){
		return true;
	}
	else if (pMapPoint->iChapterID > currentChapter)
	{
		return false;
	}
	else{// �½���ȵ�ʱ�� �ж��������񸱱��Ƿ�Զ

		UINT16 iChapterID = pMapPoint->iChapterID;
		set<UINT32> userCanVisitMapPoints;
		UINT32 curMainTaskPointID;
		GetMapPointCanVisit(iChapterID, userCanVisitMapPoints, curMainTaskPointID);
		return userCanVisitMapPoints.find(iMapPointID) != userCanVisitMapPoints.end();
	}

	return false;
}

bool CUserTaskStrategy::AddOrUpdateSubTask( const db_user_sub_task& sub_task, gs_dbs_user_info_op_req* db_req )
{
	//ע�� ����ֻ������ �����κ�ҵ���߼��жϣ����ⲿ����  ���������Ƿ񳬹����޶�����

	ASSERT(db_req);

	gs_dbs_user_info_op_req_struct_op_sub_task* sub_task_op = db_req->mutable_sub_task();
	sub_task_op->set_op_type(gs_dbs_user_info_op_req_EOPType_Add_Or_Update);
	sub_task_op->set_sub_task_id(sub_task.sub_task_id());
	
	std::map<UINT32, db_user_sub_task*>::iterator it = mMapSubTaskID2SubTask.find(sub_task.sub_task_id());
	if ( it == mMapSubTaskID2SubTask.end()){//û�д�������Ϣ �����
		stSubTask* task = CTaskMgr::GetInstance()->GetSubTaskBySubTaskID(sub_task.sub_task_id());
		ASSERT(task);
		ASSERT(m_sub_tasks);
		db_user_sub_task* pSubTask = m_sub_tasks->add_items();

		//pSubTask->CopyFrom(sub_task);
		//�������������ֶΣ�
		pSubTask->set_sub_task_id(sub_task.sub_task_id());
		pSubTask->set_star(sub_task.star());//todo�� �Ǽ����㣡
		pSubTask->set_last_enter_timestamp(sub_task.last_enter_timestamp());
		pSubTask->set_last_day_enter_num(1);
		pSubTask->set_total_enter_num(1);
		pSubTask->set_last_reset_time(0);
		pSubTask->set_last_day_reset_num(0);

		//db��Ϣ
		sub_task_op->set_star(pSubTask->star());
		sub_task_op->set_last_enter_timestamp(pSubTask->last_enter_timestamp());
		sub_task_op->set_last_day_enter_num(1);
		sub_task_op->set_total_enter_num(1);
		sub_task_op->set_last_reset_time(0);
		sub_task_op->set_last_day_reset_num(0);

		std::pair<std::map<UINT32, db_user_sub_task*>::iterator, bool> r = mMapSubTaskID2SubTask.insert(std::make_pair<UINT32, db_user_sub_task*>(sub_task.sub_task_id(), pSubTask));
		ASSERT(r.second == true);

		//ASSERT(mMapSubTaskID2SubTask.find(sub_task->sub_task_id()) == mMapSubTaskID2SubTask.end());
		//mMapSubTaskID2SubTask[sub_task->sub_task_id()] = sub_task;
		//mMapTaskID2SubTaskList[sub_task.sub_task_id()].push_back(st);

		if (task)
		{
			mMapTaskID2SubTaskList[task->iTaskID].push_back(pSubTask);
		}
		else{
			printf("error! CUserTaskStrategy::AddOrUpdateSubTask subtask[%u] no parent task", sub_task.sub_task_id());
		}
	}
	else{//������Ϣ
		db_user_sub_task* pSubTask = it->second;
		ASSERT(pSubTask);
		if (sub_task.star() > pSubTask->star()){
			pSubTask->set_star(sub_task.star());
		}
		
		if (isSameDay(sub_task.last_enter_timestamp(), pSubTask->last_enter_timestamp(), CSysSettingMgr::GetInstance()->Global_Reset_Time)){
			pSubTask->set_last_day_enter_num(pSubTask->last_day_enter_num()+1);
		}
		else{
			pSubTask->set_last_day_enter_num(1);
		}
		pSubTask->set_last_enter_timestamp(sub_task.last_enter_timestamp());
		pSubTask->set_total_enter_num(pSubTask->total_enter_num() +1);

		sub_task_op->set_star(pSubTask->star());//db�Ǳ���ȫ��
		sub_task_op->set_last_day_enter_num(pSubTask->last_day_enter_num());
		sub_task_op->set_last_enter_timestamp(pSubTask->last_enter_timestamp());
		sub_task_op->set_total_enter_num(pSubTask->total_enter_num());
		//Ŀǰ db�Ǳ߶�sub_task��ȫ������ ������Ȼû�иı���ֶ�ҲҪ��ֵ
		sub_task_op->set_last_reset_time(pSubTask->last_reset_time());
		sub_task_op->set_last_day_reset_num(pSubTask->last_day_reset_num());
	}

	return true;
}

db_user_sub_task* CUserTaskStrategy::GetSubTask( UINT32 iSubTaskID )
{
	std::map<UINT32, db_user_sub_task*>::iterator it = mMapSubTaskID2SubTask.find(iSubTaskID);
	if (it != mMapSubTaskID2SubTask.end())
	{
		return it->second;
	}

	return NULL;
}

void CUserTaskStrategy::IncreaseSubTaskEnterNum( UINT32 iSubTaskID, UINT32 iTimes, gs_dbs_user_info_op_req& db_req)
{
	gs_dbs_user_info_op_req::struct_op_sub_task* sub_task = db_req.mutable_sub_task();
	sub_task->set_op_type(gs_dbs_user_info_op_req::Add_Or_Update);
	sub_task->set_sub_task_id(iSubTaskID);

	std::map<UINT32, db_user_sub_task*>::iterator it = mMapSubTaskID2SubTask.find(iSubTaskID);
	ASSERT(it != mMapSubTaskID2SubTask.end());
	if (it != mMapSubTaskID2SubTask.end())
	{
		if (isSameDay(Now(), it->second->last_reset_time(), CSysSettingMgr::GetInstance()->Global_Reset_Time))
		{
			it->second->set_last_day_enter_num(it->second->last_day_enter_num() + iTimes);
			it->second->set_total_enter_num(it->second->total_enter_num() + iTimes);

			sub_task->set_last_day_enter_num(it->second->last_day_enter_num());
			sub_task->set_total_enter_num(it->second->total_enter_num());
		}
		else{
			it->second->set_last_enter_timestamp(Now());
			it->second->set_last_day_enter_num(iTimes);
			it->second->set_total_enter_num(it->second->total_enter_num() + iTimes);

			sub_task->set_last_enter_timestamp(it->second->last_enter_timestamp());
			sub_task->set_last_day_enter_num(it->second->last_day_enter_num());
			sub_task->set_total_enter_num(it->second->total_enter_num());
		}
	}
}

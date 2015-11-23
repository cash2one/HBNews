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
	// todo 解析这个数据
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

	//todo 获取当前的最大章节！
	while (true)
	{
		map<UINT16, vector<CTask*>>::iterator it = chapterMainTask.find(userChapterInfo.iCurrentChapter);
		if (it != chapterMainTask.end())
		{
			vector<CTask*> & mainTaskList  = it->second;

			for (vector<CTask*>::iterator itIn = mainTaskList.begin() ; itIn != mainTaskList.end() ; ++itIn)
			{	
				//todo 判断 *itIn 是否为空
				CTask * pTask  = (*itIn);
				if (!pTask)
				{
					//log error！
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
		else{//说明这一章节的主线任务都过了 
			break;
		}

		++userChapterInfo.iCurrentChapter;//注意 它可能为当前策划配置的最大章节 + 1！
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
	CTaskMgr::GetInstance()->GetSubTaskListByTaskID(iTaskID, sub_task_list);//子任务配置表
	ASSERT(sub_task_list.size() >= 1);
	CHECK_NULL_RETURN_FALSE(sub_task_list.size() >= 1);

	//必须所有的都玩过！ 从后面往前面找比较快速判错！
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

		//没有找到玩家玩这个任务的信息！
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
		{//由 子任务的每日进入次数控制

			if( pSubTask->iTotalEnterNumPerDay == 0){//子任务本身无次数限制
				return -1;
			}

			db_user_sub_task* sub_task = GetSubTask(iSubTaskID);
			if (NULL == sub_task){//没有玩过这个子任务
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
		{//由 父任务的每日进入次数控制

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
				return -1;//无限
			}

			iTotalEnterNumPerDay = cfg->m_max_num_exp_battle;
			db_user_sub_task* sub_task = GetSubTask(iSubTaskID);
			if (NULL == sub_task){//没有玩过这个子任务
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
				return -1;//无限
			}

			iTotalEnterNumPerDay = cfg->m_max_num_gold_battle;

			db_user_sub_task* sub_task = GetSubTask(iSubTaskID);
			if (NULL == sub_task){//没有玩过这个子任务
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
		ASSERT(false && "不支持的任务类型");
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
	if(!sub_task){//检测子任务存在
		return e_gsc_errorcode_unknown_error;
	}

	CTask* task = CTaskMgr::GetInstance()->GetTaskByTaskID(sub_task->iTaskID);
	if(!task || task->bHidden){//检测父任务存在 并且没有隐藏！
		return e_gsc_errorcode_unknown_error;
	}

	//判断vip 根据扫荡时不同任务类型频率来设置 if else的顺序
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
		ASSERT(false && "不支持的任务类型");
		return e_gsc_errorcode_unknown_error;
	}

	//判断星级
	db_user_sub_task* pSubTask = GetSubTask(iSubTaskID);
	if (!pSubTask || pSubTask->star() < 3){//星级不够
		return e_gsc_errorcode_speed_kill_not_enough_star;
	}

	//判断体力
	UINT32 need_health = sub_task->iSpendHealth * iTimes;
	if(m_pUser->GetHealth() < need_health){
		return e_gsc_errorcode_no_enough_health;
	}

	//判断剩余进入次数
	if(GetSubTaskLeftEnterNum(sub_task->iSubTaskID, m_pUser->GetVipLevel()) < iTimes){
		return e_gsc_errorcode_speed_kill_not_enough_left_enter_num;
	}
	
	IncreaseSubTaskEnterNum(sub_task->iSubTaskID, iTimes, db_req);

	//扣体力
	bool isFull2NotFull = false;
	bool r = m_pUser->DecreaseHealth(need_health, &isFull2NotFull);
	ASSERT(r);

	struct_game_element* ele = res_consumes.Add();
	ele->set_type(struct_game_element::EELEMENTTYPE_HEALTH);
	ele->set_id(0);
	if (isFull2NotFull)
	{
		ele->set_sub_id(m_pUser->GetHealthLastUpdateTime());//假如客户端本地体力满（自动回复最大值）的 在扣去服务器发的体力 不满之后 就使用这里发的最后更新时间为倒计时起点
	}
	else{
		ele->set_sub_id(0);
	}
	ele->set_num_type(struct_game_element::DECREASE);
	ele->set_num(need_health);


	//m_start_sub_task_res.set_health_last_update_timestamp(_get_health_last_update_time());//移到 体力 element的sub_id上。 假如客户端本地体力满（自动回复最大值）的 在扣去服务器发的体力 不满之后 就使用这里发的最后更新时间为倒计时起点

	//修改db基本信息
	gs_dbs_user_info_op_req_struct_op_base_info* base_info = db_req.mutable_base_info();
	base_info->set_op_type(gs_dbs_user_info_op_req::Update);
	base_info->set_health_cur(m_pUser->GetHealth());
	base_info->set_health_last_update_timestamp(m_pUser->GetHealthLastUpdateTime());

	//扣道具

	r = m_pUser->UpdateGameElement(req_consume_ele, db_req, res_consumes, ntf);
	ASSERT(r);

	//加道具
	bool hasRandomOpenShop = false;
	for (UINT32 i = 0 ; i < iTimes ; ++i){
		struct_game_element_array* eles_arr = res_output_items.Add();//单次扫荡的结果, 如果此次没有产出 那么它就是个空的集 客户端可以显示空

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
			if (!r){//如果没有开启 商店 则释放商店
				//res.set_allocated_shop(NULL);
			}
			else{
				hasRandomOpenShop = true;//开到了就不再随机开了 没开到才继续随机！
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

	//1. 体力判断
	CHECK_NULL_RETURN_FALSE(pUser->GetHealth() >= pSubTask->iSpendHealth );

	CTask* pTask = CTaskMgr::GetInstance()->GetTaskByTaskID(pSubTask->iTaskID);
	CHECK_NULL_RETURN_FALSE(pTask);

	//2. 是否隐藏判断
	if (pTask->bHidden){
		return false;
	}

	//3.显示条件判断 由客户端来配置
	//if (!pTask->IsConditionsSatisfy(CTask::EConditionFor_Show, pUser->UserID())){
	//	return false;
	//}

	//4.进入条件判断！
	if (!IsConditionsSatisfy(pTask->enterConditions, pUser->UserID())){
		return false;
	}

	//5.副本的次数限定
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
		if (!CanVisitMapPoint(pTask->iMapPointID)){//里面增加了进入等级判断
			return false;
		}
	}
	

	return true;
}

bool CUserTaskStrategy::GetMapPointCanVisit( UINT16 iChapterID, std::set<UINT32> &retPoints, UINT32& curMainTaskPointID )
{
	retPoints.clear();
	curMainTaskPointID = 0;

	//1. 初始点
	//GetUserMaxChapter计算量很大 给外部使用的， 内部还是自己根据任务计算吧！
	//获取玩家最大章节信息
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
	retPoints.insert(bornMapPointID);//初始ID


	//2. 已通过主线任务的地图据点
	CTaskMgr* gpTaskMgr = CTaskMgr::GetInstance(); 
	map<UINT32, vector<db_user_sub_task*>>& userTaskList = mMapTaskID2SubTaskList;
	for (std::map<UINT32, vector<db_user_sub_task*>>::iterator itIn = userTaskList.begin() ; itIn != userTaskList.end() ; ++itIn)
	{
		UINT32 iTaskID =  itIn->first;
		UINT16 iUserTaskChapterID = CTaskMgr::GetInstance()->GetChapterIDByTaskID(iTaskID);
		if(iUserTaskChapterID != iChapterID){//不是同一个章节的任务！
			continue;
		}

		CTask* pTask = gpTaskMgr->GetTaskByTaskID(iTaskID);
		if(!pTask){//任务配置不存在
			//todo log error! 放在GetTaskByTaskID里面
			//continue;
			return false;
		}

		//主线任务决定地图据点可访问！
		if ( (pTask->eType == CTask::ETASKTYPE_MAIN) && IsTaskDone(iTaskID))
		{
			retPoints.insert(pTask->iMapPointID);//set防止  一个点上多个主线
		}
	}

	//3. 获取1,2的一级临近点
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
		//获取临近的点
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
				if(p){//验证存在这个地图据点！ 实际上这个工作应该在读出的时候做 但是这里多做一下保险！
					//std::pair<set<UINT32>::iterator, bool>& ins_ret =retPoints.insert(iJoinedMapPointID);
					//if (ins_ret->second)//之前没有的 新的点 则放入队列
					//{
					//	qMapPoint.push(iJoinedMapPointID);
					//}

					//add by baomin 2015.9.22  for enter level limit
					ASSERT(m_pUser);
					if (m_pUser->GetLevel() < p->iEnterLevel){//据点进入等级限制
						continue;
					}

					retPoints.insert(iJoinedMapPointID);//一定是之前没有的，前面作了判断 有的就不会进来
					qMapPoint.push(iJoinedMapPointID);
				}
				else{
					//todo log error!  在读配置的地方做检测！
					return false;
				}
			}
		}
	}

	return true;
}

bool CUserTaskStrategy::CanVisitMapPoint( UINT32 iMapPointID )
{
	//获取玩家最大章节信息
	stUserChapterInfo userChapterInfo = {0};
	e_gsc_errorcode ret = GetChapterInfo(userChapterInfo);
	if (ret != e_gsc_errorcode_success)
	{
		return false;
	}

	UINT16 currentChapter = userChapterInfo.iCurrentChapter;

	//获取据点章节信息
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
	else{// 章节相等的时候 判断主线任务副本是否够远

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
	//注意 这里只做更新 不作任何业务逻辑判断（由外部做）  比如任务是否超过了限定数量

	ASSERT(db_req);

	gs_dbs_user_info_op_req_struct_op_sub_task* sub_task_op = db_req->mutable_sub_task();
	sub_task_op->set_op_type(gs_dbs_user_info_op_req_EOPType_Add_Or_Update);
	sub_task_op->set_sub_task_id(sub_task.sub_task_id());
	
	std::map<UINT32, db_user_sub_task*>::iterator it = mMapSubTaskID2SubTask.find(sub_task.sub_task_id());
	if ( it == mMapSubTaskID2SubTask.end()){//没有此任务信息 则添加
		stSubTask* task = CTaskMgr::GetInstance()->GetSubTaskBySubTaskID(sub_task.sub_task_id());
		ASSERT(task);
		ASSERT(m_sub_tasks);
		db_user_sub_task* pSubTask = m_sub_tasks->add_items();

		//pSubTask->CopyFrom(sub_task);
		//拷贝了这三个字段！
		pSubTask->set_sub_task_id(sub_task.sub_task_id());
		pSubTask->set_star(sub_task.star());//todo！ 星级计算！
		pSubTask->set_last_enter_timestamp(sub_task.last_enter_timestamp());
		pSubTask->set_last_day_enter_num(1);
		pSubTask->set_total_enter_num(1);
		pSubTask->set_last_reset_time(0);
		pSubTask->set_last_day_reset_num(0);

		//db信息
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
	else{//更新信息
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

		sub_task_op->set_star(pSubTask->star());//db那边是全量
		sub_task_op->set_last_day_enter_num(pSubTask->last_day_enter_num());
		sub_task_op->set_last_enter_timestamp(pSubTask->last_enter_timestamp());
		sub_task_op->set_total_enter_num(pSubTask->total_enter_num());
		//目前 db那边对sub_task是全量更新 所以虽然没有改变的字段也要赋值
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

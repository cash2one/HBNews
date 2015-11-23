#include "task_mgr.h"

#include "db_mgr.h"
#include "db_sql.h"
#include "str.h"

#include "burst_mgr.h"
#include "condition.h"
#include "sys_setting_mgr.h"
bool CTaskMgr::Init()
{
	const char* str = NULL;
	bool r = CSysSettingMgr::GetInstance()->GetStringValue("Chapter_Map_Born_Point", str);
	ASSERT(r && str);
	CHECK_FALSE_RETURN_FALSE(r && str);

	split_string_2i(std::string(str), m_CharpterMapBornPoint, "|");

	//1. 读配置 组织基本map

	//2. 组织各个查询map，并且一旦有问题 就报错！！

	MYSQL* con = CDBMgr::GetInstance()->GetConnection();

	//解析task表！
	char* sql = SELECT_TASK;
	int code = CDBMgr::Query(con, sql, (unsigned int)strlen(sql));
	if (0 != code)   //非0查询失败  
	{
		printf("query failed! [%s]\n", sql);  
		return false;  
	}

	MYSQL_RES* res = mysql_store_result(con);   //保存查询结果  

	{
		//检查表结构是否有改变
		//string task_need_fields[] = {"id", "task_id", "type", "name", "desc", "mappoint_id", "priority", "has_enter_condition", "prize_element_type", "prize_element_id", "prize_element_num", "img", "unlock_mappoint_id", "hidden"};
		string need_fields[] = {"id", "task_id", "type", "name", "desc", "mappoint_id", "priority", "total_enter_num_per_day", "enter_condition_arr", "prize_element_type_id_num_arr", "unlock_mappoint_id_arr", "background_img", "hidden"};
		std::vector<std::string> v_task_need_field(need_fields, need_fields + sizeof(need_fields)/sizeof(need_fields[0]));
		ASSERT(CDBMgr::check_fields(res, v_task_need_field));
	}

	MYSQL_ROW row;
	CMapMgr* pMapMgr = CMapMgr::GetInstance();
	while (row = mysql_fetch_row(res))
	{
		int col = 1;
		CTask *pTask = new CTask();
		
		pTask->iTaskID = atoi(row[col++]);

		pTask->eType = (CTask::ETASKTYPE)atoi(row[col++]);
		ASSERT( pTask->eType > CTask::ETASKTYPE_MIN && pTask->eType < CTask::ETASKTYPE_MAX);

		char* pBuf = UTF8ToANSI(row[col++]);
		int len = strlen(pBuf);
		ASSERT( len <= MAXLEN_TASKNAME);
		memcpy(pTask->strName, pBuf, len);
		free(pBuf);

		pBuf = UTF8ToANSI(row[col++]);
		len = strlen(pBuf);
		ASSERT( len <= MAXLEN_TASKDESC);
		memcpy(pTask->strDesc, pBuf, len);
		free(pBuf);

		pTask->iMapPointID = atoi(row[col++]);
		ASSERT(CMapMgr::GetInstance()->GetMapPointByID(pTask->iMapPointID));//验证所属据点存在

		pTask->iPriority = atoi(row[col++]);

		pTask->iTotalEnterNumPerDay = atoi(row[col++]);

		//解析显示条件配置 <--服务器已经去掉这个设定

		//解析进入条件配置！
		pBuf = UTF8ToANSI(row[col++]);
		bool r = ParseCondition(pBuf, pTask->enterConditions);
		free(pBuf);
		

		pBuf = UTF8ToANSI(row[col++]);
		//len = strlen(pBuf);
		//ASSERT( len <= MAXNUM_TASKPRIZE_STRLEN);
		//分割字符串
		vector<string> prizes;
		split_string(string(pBuf), prizes, "|");
		free(pBuf);
		int i = 0;
		//ASSERT(prizes.size()<= MAXNUM_TASKPRIZE_NUM);
		for (vector<string>::iterator it = prizes.begin() ; it != prizes.end() ; ++it){
			vector<int> aprize;
			split_string_2i(*it, aprize, "-");
			ASSERT(aprize.size() == 3);
			pTask->prizes.push_back(stGameElement((EELEMENTTYPE)aprize[0], aprize[1], 0, stGameElement::INCREASE, aprize[2]));
			IsElementValid(pTask->prizes[i]);
			++i;
		}

		//pTask->iPrizeElementType = (EELEMENTTYPE)atoi(row[8]);
		//ASSERT(pTask->iPrizeElementType < EELEMENTTYPE::EELEMENTTYPE_MAX);

		//pTask->iPrizeElementID = atoi(row[9]);
		//pTask->iPrizeElementNum = atoi(row[10]);
		

		pBuf = UTF8ToANSI(row[col++]);
		len = strlen(pBuf);
		ASSERT( len <= MAXLEN_TASKIMG);
		memcpy(pTask->strImg, pBuf, len);
		free(pBuf);

		//pTask->iUnlockMappointID = atoi(row[12]);
		//ASSERT(CMapMgr::GetInstance()->GetMapPointByID(pTask->iUnlockMappointID));//验证所属据点存在
		pBuf = UTF8ToANSI(row[col++]);
		//len = strlen(pBuf);
		vector<int> mappoints;
		split_string_2i(string(pBuf), mappoints, "|");
		ASSERT(mappoints.size() <= MAXNUM_JOINEDMAPPOINT);
		i = 0;
		for (vector<int>::iterator it = mappoints.begin() ; it != mappoints.end() ;++it)
		{
			UINT32 iMappointID = *it;
			ASSERT(pMapMgr->GetMapPointByID(iMappointID));
			pTask->iUnlockMappointID[i++] = iMappointID;
		}
		free(pBuf);

		pTask->bHidden = (bool)atoi(row[col]);

		ASSERT(AddTask(pTask));
	}
	mysql_free_result(res);
	
	sql = SELECT_SUBTASK;
	code = CDBMgr::Query(con, sql, (unsigned int)strlen(sql));
	if (0 != code)   //非0查询失败  
	{
		printf("query failed! [%s]\n", sql);  
		return false;  
	}

	res = mysql_store_result(con);   //保存查询结果  

	//检查表结构是否有改变
	//, "battle_id_difficulty_time_arr"去掉这个配置！ 《-还是保留了
	{
		string need_fields[] = {"id", "sub_task_id", "task_id", "priority", "spend_health", "total_enter_num_per_day", "normal_packet_id", "auto_battle_packet_id", "extra_packet_id", "extra_packet_time"};
		std::vector<std::string> v_subtask_need_field(need_fields, need_fields + sizeof(need_fields)/sizeof(need_fields[0]));
		ASSERT(CDBMgr::check_fields(res, v_subtask_need_field));
	}
	while (row = mysql_fetch_row(res))
	{
		stSubTask *pSubTask = new stSubTask();
		memset(pSubTask, 0, sizeof stSubTask);
		int col = 1;//忽略主键id 

		pSubTask->iSubTaskID = atoi(row[col++]);
		pSubTask->iTaskID = atoi(row[col++]);
		ASSERT(GetTaskByTaskID(pSubTask->iTaskID) && "子任务初始化有问题 其父任务不存在！");

		pSubTask->iPriority = atoi(row[col++]);
		pSubTask->iSpendHealth = atoi(row[col++]);

		pSubTask->iTotalEnterNumPerDay = atoi(row[col++]);

		pSubTask->iBurstPacketID = atoi(row[col++]);
		ASSERT(CBurstMgr::GetInstance()->GetPacketByPacketID(pSubTask->iBurstPacketID));
		pSubTask->iAutoBattleBurstPacketID = atoi(row[col++]);
		ASSERT(CBurstMgr::GetInstance()->GetPacketByPacketID(pSubTask->iAutoBattleBurstPacketID));
		pSubTask->iExtraPacketID = atoi(row[col++]);
		if (pSubTask->iExtraPacketID){
			ASSERT(CBurstMgr::GetInstance()->GetPacketByPacketID(pSubTask->iExtraPacketID));
			//解析额外掉落包条件配置！
			char* pBuf = UTF8ToANSI(row[col++]);
			bool r = ParseTimeCondition(pBuf, pSubTask->ExtraPacketCondition);
			free(pBuf);
		}

		//char* pBuf = UTF8ToANSI(row[col++]);
		////len = strlen(pBuf);
		////ASSERT( len <= MAXLEN_TASKIMG);
		//vector<string> battlecontrols;
		//split_string(string(pBuf), battlecontrols, "|");
		//free(pBuf);
		//int i = 0;
		//ASSERT(battlecontrols.size()<= MAXNUM_TASKBATTLE);
		//for (vector<string>::iterator it = battlecontrols.begin() ; it != battlecontrols.end() ; ++it)
		//{
		//	vector<int> aBattle;
		//	split_string_2i(*it, aBattle, "-");
		//	ASSERT(aBattle.size() == 2);
		//	ASSERT(CFightMgr::GetInstance()->GetBattleByID(aBattle[0]));
		//	pSubTask->battles[i].iBattleID = aBattle[0];
		//	//pSubTask->battles[i].iDifficulty = aBattle[1];
		//	//pSubTask->battles[i].iTimeLimit = aBattle[2];
		//	pSubTask->battles[i].iTimeLimit = aBattle[1];
		//	++i;
		//}
		
		bool r = AddSubTask(pSubTask);
		ASSERT(r);
	}

	sql = SELECT_SUB_TASK_MAIN_BRANCH_ID;
	code = CDBMgr::Query(con, sql, (unsigned int)strlen(sql));
	if (0 != code) 
	{
		printf("query failed! [%s]\n", sql);  
		return false;  
	}

	res = mysql_store_result(con);   

	{
		string need_fields[] = {"chapter_id", "sub_task_id"};
		std::vector<std::string> v_subtask_need_field(need_fields, need_fields + sizeof(need_fields)/sizeof(need_fields[0]));
		ASSERT(CDBMgr::check_fields(res, v_subtask_need_field));
	}
	while (row = mysql_fetch_row(res))
	{
		m_mapChapterID2MainBranchSubTaskIDs;
		int col = 0;
		UINT32 iChapterID = atoi(row[col++]);
		UINT32 iSubTaskID = atoi(row[col++]);
		m_mapChapterID2MainBranchSubTaskIDs[iChapterID].push_back(iSubTaskID);
	}
	mysql_free_result(res);
	return true;
}

CTask* CTaskMgr::GetTaskByTaskID( UINT32 iTaskID )
{
	map<UINT32, CTask*>::iterator it = mMapTaskID2Task.find(iTaskID);
	if (it != mMapTaskID2Task.end())
	{
		return it->second;
	}
	else{
		//todo log error  打印堆栈 调用者！！！
		return NULL;
	}
}

stSubTask* CTaskMgr::GetSubTaskBySubTaskID( UINT32 iSubTaskID )
{
	map<UINT32, stSubTask*>::iterator it = mMapSubTaskID2SubTask.find(iSubTaskID);
	if (it != mMapSubTaskID2SubTask.end())
	{
		return it->second;
	}
	else{
		//todo log error  打印堆栈 调用者！！！
		return NULL;
	}
}

void CTaskMgr::GetSubTaskListByTaskID( INT32 iTaskID, vector<stSubTask*> &ret )
{
	map<UINT32, vector<stSubTask*>>::iterator it = mMapTaskID2SubTaskList.find(iTaskID);
	if (it != mMapTaskID2SubTaskList.end())
	{
		ret = it->second;
	}
}

void CTaskMgr::GetTaskListByMapPointID( UINT32 iMapPointID, vector<CTask*> &ret )
{
	map<UINT32, vector<CTask*>>::iterator it = mMapMapPointID2TaskList.find(iMapPointID);
	if (it != mMapMapPointID2TaskList.end())
	{
		ret = it->second;
	}
}

UINT16 CTaskMgr::GetChapterIDByTaskID( UINT32 iTaskID )
{
	CTask* pTask = GetTaskByTaskID(iTaskID);
	if (!pTask)
	{
		return 0;
	}

	CMapPoint* pMapPoint = CMapMgr::GetInstance()->GetMapPointByID(pTask->iMapPointID);
	if (!pMapPoint)
	{
		return 0;
	}

	return pMapPoint->iChapterID;
}

UINT32 CTaskMgr::GetMapPointIDByTaskID( UINT32 iTaskID )
{
	CTask* pTask = GetTaskByTaskID(iTaskID);
	if (!pTask)
	{
		return 0;
	}

	CMapPoint* pMapPoint = CMapMgr::GetInstance()->GetMapPointByID(pTask->iMapPointID);
	if (!pMapPoint)
	{
		return 0;
	}

	return pMapPoint->iMappointID;
}

bool CTaskMgr::AddTask( CTask* pTask )
{
	ASSERT( pTask && 
		pTask->iTaskID && 
		(mMapTaskID2Task.find(pTask->iTaskID) == mMapTaskID2Task.end())
		);

	mMapTaskID2Task[pTask->iTaskID] = pTask;

	if (pTask->iMapPointID)
	{
		//todo 需要验证是否已经有重复任务？
		mMapMapPointID2TaskList[pTask->iMapPointID].push_back(pTask);

		if (pTask->eType == CTask::ETASKTYPE_MAIN)
		{
			CMapPoint* pPoint = CMapMgr::GetInstance()->GetMapPointByID(pTask->iMapPointID);
			ASSERT(pPoint && pPoint->iChapterID);
			mMapChapterID2MainTaskList[pPoint->iChapterID].push_back(pTask);
		}
	}

	if (pTask->eType == CTask::ETASKTYPE_ACTIVITY){
		mMapTaskID2ActivityTask[pTask->iTaskID] = pTask;
	}

	return true;
}

bool CTaskMgr::AddSubTask( stSubTask* pSubTask )
{
	//if( !pSubTask || !pSubTask->iSubTaskID || !pSubTask->iTaskID){
	//	return false;
	//}
	//if(mMapSubTaskID2SubTask.find(pSubTask->iSubTaskID) != mMapSubTaskID2SubTask.end()){
	//	return false;
	//}

	ASSERT( pSubTask && 
		pSubTask->iSubTaskID && 
		pSubTask->iTaskID && 
		(mMapSubTaskID2SubTask.find(pSubTask->iSubTaskID) == mMapSubTaskID2SubTask.end())
		);

	mMapSubTaskID2SubTask[pSubTask->iSubTaskID] = pSubTask;
	//todo 判断是否重复
	mMapTaskID2SubTaskList[pSubTask->iTaskID].push_back(pSubTask);
	return true;
}

bool CTaskMgr::UnInit()
{
	mMapTaskID2SubTaskList.clear();
	mMapMapPointID2TaskList.clear();
	mMapChapterID2MainTaskList.clear();

	for (map<UINT32, CTask*>::iterator it = mMapTaskID2Task.begin(); it != mMapTaskID2Task.end(); ++it)
	{
		delete it->second;
	}
	mMapTaskID2Task.clear();

	for (map<UINT32, stSubTask*>::iterator it = mMapSubTaskID2SubTask.begin(); it != mMapSubTaskID2SubTask.end(); ++it)
	{
		delete it->second;
	}
	mMapSubTaskID2SubTask.clear();

	for(vector<ICondition*>::iterator it = m_baseEnterCondition.begin(); it != m_baseEnterCondition.end(); ++it){
		delete *it;
	}
	m_baseEnterCondition.clear();

	return true;
}
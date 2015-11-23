
/*****************************************************************************
*   
for : 用户任务信息管理, 用户推图信息相关
data : 20150312
author : baomin
*
******************************************************************************/
#ifndef __USER_TASK_MGR_H__
#define __USER_TASK_MGR_H__

#include <map>
#include <vector>
#include <set>
#include <queue>
#include "def.h"

#include "task_mgr.h"
#include "gs_c_msg_body.pb.h"

#include "user_mgr.h"

using namespace std;


struct CUserSubTask{
	//INT32 iUserID;
	INT32 iSubTaskID;//子任务ID
	UINT8 iStatus;//0 未通过 1已通过
	UINT8 iStar;//评价星级
	UINT32 iLastEnterTime ;//时间戳
	UINT32 iEnterNumLastDay;
	UINT32 iTotalEnterNum;//此玩家对这个任务总共玩的

	bool bHasGotItem1;//是否已经得到爆的第一种物品
	bool bHasGotItem2;
	bool bHasGotItem3;
	bool bHasGotItem4;
};

//不需要存用户父任务信息， 完成状态是根据子任务状态计算的！ <-- 还是需要的 因为计算很多 这里就先存着！并且需要更新内存状态，数据库则不需要存储这个数据！！
struct CUserTask{
	UINT32 iUserID;
	UINT32 iTaskID;
	EUSERTASKSTATUS iStatus;//0 未通关  1.已通关（奖励已给）
	
	vector<CUserSubTask*> mUserSubTaskList;
};


struct stUserChapterInfo{
	UINT16 iCurrentChapter;//当前最大章节 如果都完成过 就为配置的最大章节 + 1
	CTask* pCurrentMainTask;//下一个主线任务
};

class CUserTaskMgr{
	
public:

	//todo 单例工具
	inline static CUserTaskMgr* GetInstance(){
		static CUserTaskMgr singleton;
		return &singleton;
	}

	bool Init(){

	}

	bool UnInit();

	bool IsSubTaskDoneByUser(UINT32 iUserID, UINT32 iSubTaskID);

	bool IsTaskDoneByUser(UINT32 iUserID, UINT32 iTaskID);

	bool CanUserVisitMapPoint(UINT32 iUserID, UINT32 iMapPointID);

	//获取玩家的章节信息， 判断是否主线任务都过了
	EErrorCode GetUserChapterInfo( UINT32 iUserID, stUserChapterInfo& userChapterInfo);

	//todo!
	UINT32 GetChapterBornMapPointID(UINT16 iChapterID){
		return 1;
	}

	//获取玩家可以看得到的点， 实际上 服务器只需要给玩家目前任务信息即可， 客户站自己计算可到达点！减少服务器计算量！
	//但是服务器需要判定的时候 还是需要这个计算过程的！ 在上面一个函数做这个事情！
	bool GetMapPointUserCanVisit( UINT32 iUserID, UINT16 iChapterID, set<UINT32> &retPoints, UINT32& curMainTaskPointID);



	void OnGS_C_Req_Open_Chapter_Map(UINT32 iUserID, UINT16 iChapterID, GS_C_Res_Open_Chapter_Map& st);
private:

	bool CanStartFight(UINT32 iUserID, UINT32 iSubTaskID){
		
		CUser* pUser = CUserMgr::GetInstance()->GetUserByUserID(iUserID);
		CHECK_NULL_RETURN_FALSE(pUser);

		CSubTask* pSubTask = CTaskMgr::GetInstance()->GetSubTaskBySubTaskID(iSubTaskID);
		CHECK_NULL_RETURN_FALSE(pSubTask);

		//1. 体力判断
		CHECK_NULL_RETURN_FALSE(pUser->GetHealth() < pSubTask->iSpendHealth );

		//2. 进入限制判断
		CTask* pTask = CTaskMgr::GetInstance()->GetTaskByTaskID(pSubTask->iTaskID);
		CHECK_NULL_RETURN_FALSE(pTask);
	}
	/*
	vector<CUserTask*>& GetUserTaskListByUserID(INT32 iUserID)
	{
		static vector<CUserTask*> emptyUserTaskList;

		map<INT32, CUserTask*>::iterator it = mMapUserID2UserTaskList.find(iUserID);
		if(it != mMapUserID2UserTaskList.end()){
			return it->second;
		}
		else{
			//从数据库中读取信息

			//return 
		}

		return emptyUserTaskList;
	}
	*/
	
	//vector<CUserTask*>是否要用map，加快查询效率？ 用户id -> mappointid/taskid  => info！
	// 不用pair作为key 是因为需要根据userID 得到一个list！
	//map<UINT32, vector<CUserTask*>> mMapUserID2UserTaskList;

	map<UINT32, map<UINT32, CUserTask*>> mMapUserID2TaskID2UserTask;
	map<UINT32, map<UINT32, CUserSubTask*>> mMapUserID2SubTaskID2SubTask;
};

#endif
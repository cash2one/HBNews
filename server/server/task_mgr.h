
/*****************************************************************************
*   for : 任务基本配置
data : 20150312
author : baomin
*
******************************************************************************/
#ifndef __TASK_MGR_H__
#define __TASK_MGR_H__

#pragma  once

#include <map>
#include <vector>
#include "def.h"

#include "map_mgr.h"

using namespace std;

class ITaskEnterCondition;
class stBattle;

struct CTask{
	UINT32	iTaskID;	//主任务ID
	ETASKTYPE	iType;		//TASKTYPE 任务类型
	CHAR	strName[63];//任务名称
	CHAR	strDesc[255];//任务描述
	UINT32	iMapPointID;//任务所属的据点，有的有，有的没有
	UINT8	iShowPriority;//任务显示优先级0~127，值越大越在前. 应用场景 如:某城市中有多个任务， 活动任务列表 
	bool	bHasEnterCondition;//是否有进入限制 0没有 1有； 假如有的话，这个任务的进入条件写在./task/enter_condition/id.xml中  比如一天或者一周的次数限制  等级 职业等等限制
	UINT32	iPrizeItemID;//所有子任务完成后的奖励物品ID
	UINT32	iPrizeItemNum;//所有子任务完成后的奖励物品数量
	CHAR	strImg[63];//任务背景图
	bool	bHidden;//是否隐藏该任务

	ITaskEnterCondition* enterConditions[MAXNUM_TASKENTERCONDITION];//进入条件限制！

};

struct CSubTask{
	UINT32	iSubTaskID;//子任务ID，全局唯一
	UINT32	iTaskID;//所属父任务，可以没有。 比如 小点上没有父任务 所以这里为空
	UINT8	iOrderNum;//先后顺序， 数字越大 任务越后
	UINT32	iSpendHealth;//消耗体力值

	UINT32	iExpForUser;//用户所获经验
	UINT32	iExpForEachHero;//每个英雄所获经验
	
	UINT32	iBurstItemID1;//显示爆的物品1
	UINT32	iBurstItemID2;
	UINT32	iBurstItemID3;
	UINT32	iBurstItemID4;

	
	vector<stBattle*> battles;//
};

class  CTaskMgr
{
public:
	//todo 单例工具
	inline static CTaskMgr* GetInstance(){
		static CTaskMgr singleton;
		return &singleton;
	}

	bool Init();

	bool UnInit(){

		return true;
	}

	CTask* GetTaskByTaskID(UINT32 iTaskID){
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

	CSubTask* GetSubTaskBySubTaskID(UINT32 iSubTaskID){
		map<UINT32, CSubTask*>::iterator it = mMapSubTaskID2SubTask.find(iSubTaskID);
		if (it != mMapSubTaskID2SubTask.end())
		{
			return it->second;
		}
		else{
			//todo log error  打印堆栈 调用者！！！
			return NULL;
		}
	}

	//不直接返回引用的原因 是怕外面不小心作了修改 所以这里返回拷贝
	void GetSubTaskListByTaskID(INT32 iTaskID, vector<CSubTask*> &ret){
		map<UINT32, vector<CSubTask*>>::iterator it = mMapTaskID2SubTaskList.find(iTaskID);
		if (it != mMapTaskID2SubTaskList.end())
		{
			ret = it->second;
		}
	}

	void GetTaskListByMapPointID(UINT32 iMapPointID, vector<CTask*> &ret){
		
	}

	UINT16 GetChapterIDByTaskID(UINT32 iTaskID){
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

	UINT32 GetMapPointIDByTaskID(UINT32 iTaskID){
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

protected:

private:

	vector<ITaskEnterCondition*>	m_baseEnterCondition;

	//基本配置
	map<UINT32, CTask*> mMapTaskID2Task;
	map<UINT32, CSubTask*> mMapSubTaskID2SubTask;

	//为了高效索引而创建的
	map<UINT32, vector<CSubTask*>> mMapTaskID2SubTaskList;//父任务ID->子任务列表
	map<UINT32, vector<CTask*>> mMapMapPointID2TaskList;//地图据点ID->父任务列表
	map<UINT16, vector<CTask*>> mMapChapterID2MainTaskList;//章节ID->主线父任务列表  vector按照先后顺序放在vector中

	friend class CUserTaskMgr;
};

#endif
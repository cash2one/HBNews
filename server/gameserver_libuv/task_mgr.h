
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
#include <set>
#include "def.h"

#include "map_mgr.h"
#include "fight_mgr.h"
#include "condition.h"

using namespace std;


//struct stTaskPrize
//{
//	EELEMENTTYPE	iPrizeElementType;	//所有子任务完成后的奖励类型
//	UINT32	iPrizeElementID;	//所有子任务完成后的奖励事物ID
//	UINT32	iPrizeElementNum;	//所有子任务完成后的奖励事物数量
//};

struct CTask{

	enum ETASKTYPE{
		ETASKTYPE_MIN		= 0,
		ETASKTYPE_MAIN		= 1,	//主线剧情
		ETASKTYPE_BRANCH	= 2,	//支线，自由
		ETASKTYPE_ACTIVITY	= 3,	//活动
		ETASKTYPE_EXP		= 4,	//每日经验活动副本
		ETASKTYPE_GOLD		= 5,	//每日金币活动副本
		ETASKTYPE_MAX		= 6
	};

	UINT32	iTaskID;	//主任务ID
	ETASKTYPE	eType;		//TASKTYPE 任务类型
	CHAR	strName[MAXLEN_TASKNAME];//任务名称
	CHAR	strDesc[MAXLEN_TASKDESC];//任务描述
	UINT32	iMapPointID;//任务所属的据点，有的有，有的没有
	UINT8	iPriority;//任务优先级， 值越小越在前。控制 主线任务的顺序，支线任务的显示顺序 如:某城市中有多个任务；活动任务列表 
	
	UINT32  iTotalEnterNumPerDay;	//type = ETASKTYPE_ACTIVITY(3) 活动任务时有效： 活动任务所属所有子任务每日允许进入次数总和
	//bool	bHasEnterCondition;//是否有进入限制 0没有 1有； 假如有的话，这个任务的进入条件写在./task/enter_condition/id.xml中  比如一天或者一周的次数限制  等级 职业等等限制
	ICondition* showConditions[MAXNUM_CONDITION];//显示条件限制！
	ICondition* enterConditions[MAXNUM_CONDITION];//进入条件限制！
	//stGameElement prizes[MAXNUM_TASKPRIZE_NUM];
	std::vector<stGameElement> prizes;

	CHAR	strImg[MAXLEN_TASKIMG];//任务背景图
	UINT32	iUnlockMappointID[MAXNUM_JOINEDMAPPOINT];		//任务完成时解锁据点ID
	bool	bHidden;//是否隐藏该任务

	enum EConditionFor{
		EConditionFor_Show = 1,
		EConditionFor_Enter = 2
	};
	CTask(){

		iTaskID = 0;
		eType = ETASKTYPE_MIN;
		memset(strName, 0, sizeof strName);
		memset(strDesc, 0, sizeof strDesc);
		iMapPointID = 0;
		iPriority = 0;
		iTotalEnterNumPerDay = 0;
		memset(enterConditions, 0, sizeof enterConditions);
		memset(showConditions, 0, sizeof showConditions);
		memset(strImg, 0, sizeof strImg);
		memset(iUnlockMappointID, 0, sizeof iUnlockMappointID);
		bHidden = 0;
	}

	//bool IsConditionsSatisfy(EConditionFor efor, UINT32 iUserID){
	//	//ICondition** condition_ptr_arr = NULL;
	//	//if (efor == EConditionFor_Show)
	//	//{
	//	//	condition_ptr_arr = showConditions;
	//	//}
	//	//else if (efor == EConditionFor_Enter)
	//	//{
	//	//	condition_ptr_arr = enterConditions;
	//	//}
	//	//else{
	//	//	ASSERT(false);
	//	//	return NULL;
	//	//}

	//	//for (UINT32 i = 0; i < MAXNUM_TASKENTERCONDITION ; ++i)
	//	//{
	//	//	ICondition* pCondition = condition_ptr_arr[i];
	//	//	if (pCondition && !pCondition->IsSatisfy(iUserID))
	//	//	{
	//	//		return false;
	//	//	}
	//	//}

	//	//return true;

	//	if (efor == EConditionFor_Show){
	//		return _IsConditionsSatisfy(showConditions, iUserID);
	//	}
	//	else if (efor == EConditionFor_Enter){
	//		return _IsConditionsSatisfy(enterConditions, iUserID);
	//	}
	//	else{
	//		ASSERT(false);
	//		return false;
	//	}
	//}
};

struct stSubTask{
	UINT32	iSubTaskID;//子任务ID，全局唯一
	UINT32	iTaskID;//所属父任务，可以没有。 比如 小点上没有父任务 所以这里为空
	UINT8	iPriority;//优先级， 数字越大 任务越后
	UINT32	iSpendHealth;//消耗体力值

	//UINT32	iExpForUser;//用户所获经验
	//UINT32	iExpForEachHero;//每个英雄所获经验
	
	//UINT32	iBurstItemID1;//显示爆的物品1
	//UINT32	iBurstItemID2;
	//UINT32	iBurstItemID3;
	//UINT32	iBurstItemID4;

	UINT32	iTotalEnterNumPerDay;	//每日可进入次数 type = 1, 2的时候 表示崩本子任务的次数 如果为0 表示无限  type=3 活动任务无效 不配置在这里  活动任务配置在父任务上作控制： 表示所有子任务的加起来不超过次数

	UINT32	iBurstPacketID;		//爆落方案ID
	UINT32	iAutoBattleBurstPacketID;//扫荡爆落方案ID
	UINT32	iExtraPacketID;		//额外爆落方案

	//, "battle_id_difficulty_time_arr"去掉这个配置！
	//vector<CBattle*> battles;//
	//stBattleControl	battles[MAXNUM_TASKBATTLE];

	ICondition* ExtraPacketCondition[MAXNUM_CONDITION];//只允许时间类型！

	//bool IsConditionsSatisfy(UINT32 iUserID){
	//	return _IsConditionsSatisfy(pExtraPacketCondition, iUserID);
	//}
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

	bool UnInit();

	CTask* GetTaskByTaskID(UINT32 iTaskID);

	stSubTask* GetSubTaskBySubTaskID(UINT32 iSubTaskID);

	//根据子任务ID 获取所属的父任务
	CTask* GetTaskBySubTaskID(UINT32 iSubTaskID){
		stSubTask* pSubTask = GetSubTaskBySubTaskID(iSubTaskID);
		if (pSubTask)
		{
			return GetTaskByTaskID(pSubTask->iTaskID);
		}
		return NULL;
	}

	//UINT32 GetSubTaskTimeLimit(UINT32 iSubTaskID){
	//	stSubTask* pST = GetSubTaskBySubTaskID(iSubTaskID);
	//	ASSERT(pST);
	//	UINT32 timeLimit = 0;
	//	for (int i = 0; i < MAXNUM_TASKBATTLE; ++i)
	//	{
	//		if (pST->battles[i].iBattleID)
	//		{
	//			timeLimit += pST->battles[i].iTimeLimit;
	//		}
	//		else{
	//			break;//读取配置时从左到右填充战斗限制，所以这里可以直接break
	//		}
	//	}
	//	return timeLimit;
	//};

	//不直接返回引用的原因 是怕外面不小心作了修改 所以这里返回拷贝
	void GetSubTaskListByTaskID(INT32 iTaskID, vector<stSubTask*> &ret);

	void GetTaskListByMapPointID(UINT32 iMapPointID, vector<CTask*> &ret);

	UINT16 GetChapterIDByTaskID(UINT32 iTaskID);

	UINT32 GetMapPointIDByTaskID(UINT32 iTaskID);

	std::map<UINT16, std::vector<CTask*>>& GetMapChapterID2MainTaskList() {
		return mMapChapterID2MainTaskList; 
	}

	UINT32 GetChapterBornMapPointID(UINT16 iChapterID){
		//if (iChapterID <= 0 || iChapterID >= m_CharpterMapBornPoint.size()){
		if (iChapterID <= 0 || iChapterID > m_CharpterMapBornPoint.size()){//没有等于号
			return 0;
		}

		return m_CharpterMapBornPoint[iChapterID-1];
	}

	//获取主线任务的子任务id
	std::vector<UINT32>* GetMainBranchSubTaskIDsByChapterID(UINT32 iChapterID){
		std::map<UINT32, std::vector<UINT32>>::iterator iter = m_mapChapterID2MainBranchSubTaskIDs.find(iChapterID);
		if (iter == m_mapChapterID2MainBranchSubTaskIDs.end())
		{
			return NULL;
		}
		return &iter->second;
	}

	//为了显示哪些活动使用 已经废弃 改为使用显示条件来判断！
	//void GetShowActivityTask( std::set<UINT32> &ret ){
	//	map<UINT32, CTask*>::iterator it = mMapTaskID2ActivityTask.begin();
	//	for ( ; it != mMapTaskID2ActivityTask.end() ; ++it){
	//		if (it->second && it->second->IsConditionsSatisfy(CTask::EConditionFor_Show, 0)){
	//			ret.insert(it->second->iTaskID);
	//		}
	//	}
	//}
private:

	CTaskMgr(){}

	bool AddTask(CTask* pTask);

	bool AddSubTask(stSubTask* pSubTask);

	//bool  InitTaskEnterCondition(UINT32 iTaskID){
	//	return false;
	//}

	//基本配置
	
	//hash_map<UINT32, CTask*> mMapTaskID2Task;// todo 使用hashmap
	map<UINT32, CTask*> mMapTaskID2Task;//完整任务表 包含所有的任务
	map<UINT32, stSubTask*> mMapSubTaskID2SubTask;//完整子任务表 包含所有的子任务

	vector<ICondition*>	m_baseEnterCondition;//对所有的任务都应用的基本进入限制 暂未使用！

	//为了高效索引而创建的
	map<UINT32, vector<stSubTask*>> mMapTaskID2SubTaskList;//父任务ID->子任务列表	stSubTask按照优先级先后顺序放在vector中  由sql的order by来保证
	map<UINT32, vector<CTask*>> mMapMapPointID2TaskList;//地图据点ID->父任务列表
	map<UINT16, vector<CTask*>> mMapChapterID2MainTaskList;//章节ID->主线父任务列表  stTask按照先后顺序放在vector中	由sql的order by来保证
	map<UINT32, CTask*> mMapTaskID2ActivityTask;//活动任务表 

	vector<INT32> m_CharpterMapBornPoint;//章节地图的出生点
	
	std::map<UINT32, std::vector<UINT32>> m_mapChapterID2MainBranchSubTaskIDs;//章节id->子任务id集合
	//friend class CUserTaskMgr;
};

#endif

/*****************************************************************************
*   for : �����������
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
//	EELEMENTTYPE	iPrizeElementType;	//������������ɺ�Ľ�������
//	UINT32	iPrizeElementID;	//������������ɺ�Ľ�������ID
//	UINT32	iPrizeElementNum;	//������������ɺ�Ľ�����������
//};

struct CTask{

	enum ETASKTYPE{
		ETASKTYPE_MIN		= 0,
		ETASKTYPE_MAIN		= 1,	//���߾���
		ETASKTYPE_BRANCH	= 2,	//֧�ߣ�����
		ETASKTYPE_ACTIVITY	= 3,	//�
		ETASKTYPE_EXP		= 4,	//ÿ�վ�������
		ETASKTYPE_GOLD		= 5,	//ÿ�ս�һ����
		ETASKTYPE_MAX		= 6
	};

	UINT32	iTaskID;	//������ID
	ETASKTYPE	eType;		//TASKTYPE ��������
	CHAR	strName[MAXLEN_TASKNAME];//��������
	CHAR	strDesc[MAXLEN_TASKDESC];//��������
	UINT32	iMapPointID;//���������ľݵ㣬�е��У��е�û��
	UINT8	iPriority;//�������ȼ��� ֵԽСԽ��ǰ������ ���������˳��֧���������ʾ˳�� ��:ĳ�������ж�����񣻻�����б� 
	
	UINT32  iTotalEnterNumPerDay;	//type = ETASKTYPE_ACTIVITY(3) �����ʱ��Ч�� �������������������ÿ�������������ܺ�
	//bool	bHasEnterCondition;//�Ƿ��н������� 0û�� 1�У� �����еĻ����������Ľ�������д��./task/enter_condition/id.xml��  ����һ�����һ�ܵĴ�������  �ȼ� ְҵ�ȵ�����
	ICondition* showConditions[MAXNUM_CONDITION];//��ʾ�������ƣ�
	ICondition* enterConditions[MAXNUM_CONDITION];//�����������ƣ�
	//stGameElement prizes[MAXNUM_TASKPRIZE_NUM];
	std::vector<stGameElement> prizes;

	CHAR	strImg[MAXLEN_TASKIMG];//���񱳾�ͼ
	UINT32	iUnlockMappointID[MAXNUM_JOINEDMAPPOINT];		//�������ʱ�����ݵ�ID
	bool	bHidden;//�Ƿ����ظ�����

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
	UINT32	iSubTaskID;//������ID��ȫ��Ψһ
	UINT32	iTaskID;//���������񣬿���û�С� ���� С����û�и����� ��������Ϊ��
	UINT8	iPriority;//���ȼ��� ����Խ�� ����Խ��
	UINT32	iSpendHealth;//��������ֵ

	//UINT32	iExpForUser;//�û�������
	//UINT32	iExpForEachHero;//ÿ��Ӣ��������
	
	//UINT32	iBurstItemID1;//��ʾ������Ʒ1
	//UINT32	iBurstItemID2;
	//UINT32	iBurstItemID3;
	//UINT32	iBurstItemID4;

	UINT32	iTotalEnterNumPerDay;	//ÿ�տɽ������ type = 1, 2��ʱ�� ��ʾ����������Ĵ��� ���Ϊ0 ��ʾ����  type=3 �������Ч ������������  ����������ڸ������������ƣ� ��ʾ����������ļ���������������

	UINT32	iBurstPacketID;		//���䷽��ID
	UINT32	iAutoBattleBurstPacketID;//ɨ�����䷽��ID
	UINT32	iExtraPacketID;		//���ⱬ�䷽��

	//, "battle_id_difficulty_time_arr"ȥ��������ã�
	//vector<CBattle*> battles;//
	//stBattleControl	battles[MAXNUM_TASKBATTLE];

	ICondition* ExtraPacketCondition[MAXNUM_CONDITION];//ֻ����ʱ�����ͣ�

	//bool IsConditionsSatisfy(UINT32 iUserID){
	//	return _IsConditionsSatisfy(pExtraPacketCondition, iUserID);
	//}
};

class  CTaskMgr
{
public:
	//todo ��������
	inline static CTaskMgr* GetInstance(){
		static CTaskMgr singleton;
		return &singleton;
	}

	bool Init();

	bool UnInit();

	CTask* GetTaskByTaskID(UINT32 iTaskID);

	stSubTask* GetSubTaskBySubTaskID(UINT32 iSubTaskID);

	//����������ID ��ȡ�����ĸ�����
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
	//			break;//��ȡ����ʱ���������ս�����ƣ������������ֱ��break
	//		}
	//	}
	//	return timeLimit;
	//};

	//��ֱ�ӷ������õ�ԭ�� �������治С�������޸� �������ﷵ�ؿ���
	void GetSubTaskListByTaskID(INT32 iTaskID, vector<stSubTask*> &ret);

	void GetTaskListByMapPointID(UINT32 iMapPointID, vector<CTask*> &ret);

	UINT16 GetChapterIDByTaskID(UINT32 iTaskID);

	UINT32 GetMapPointIDByTaskID(UINT32 iTaskID);

	std::map<UINT16, std::vector<CTask*>>& GetMapChapterID2MainTaskList() {
		return mMapChapterID2MainTaskList; 
	}

	UINT32 GetChapterBornMapPointID(UINT16 iChapterID){
		//if (iChapterID <= 0 || iChapterID >= m_CharpterMapBornPoint.size()){
		if (iChapterID <= 0 || iChapterID > m_CharpterMapBornPoint.size()){//û�е��ں�
			return 0;
		}

		return m_CharpterMapBornPoint[iChapterID-1];
	}

	//��ȡ���������������id
	std::vector<UINT32>* GetMainBranchSubTaskIDsByChapterID(UINT32 iChapterID){
		std::map<UINT32, std::vector<UINT32>>::iterator iter = m_mapChapterID2MainBranchSubTaskIDs.find(iChapterID);
		if (iter == m_mapChapterID2MainBranchSubTaskIDs.end())
		{
			return NULL;
		}
		return &iter->second;
	}

	//Ϊ����ʾ��Щ�ʹ�� �Ѿ����� ��Ϊʹ����ʾ�������жϣ�
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

	//��������
	
	//hash_map<UINT32, CTask*> mMapTaskID2Task;// todo ʹ��hashmap
	map<UINT32, CTask*> mMapTaskID2Task;//��������� �������е�����
	map<UINT32, stSubTask*> mMapSubTaskID2SubTask;//����������� �������е�������

	vector<ICondition*>	m_baseEnterCondition;//�����е�����Ӧ�õĻ����������� ��δʹ�ã�

	//Ϊ�˸�Ч������������
	map<UINT32, vector<stSubTask*>> mMapTaskID2SubTaskList;//������ID->�������б�	stSubTask�������ȼ��Ⱥ�˳�����vector��  ��sql��order by����֤
	map<UINT32, vector<CTask*>> mMapMapPointID2TaskList;//��ͼ�ݵ�ID->�������б�
	map<UINT16, vector<CTask*>> mMapChapterID2MainTaskList;//�½�ID->���߸������б�  stTask�����Ⱥ�˳�����vector��	��sql��order by����֤
	map<UINT32, CTask*> mMapTaskID2ActivityTask;//������ 

	vector<INT32> m_CharpterMapBornPoint;//�½ڵ�ͼ�ĳ�����
	
	std::map<UINT32, std::vector<UINT32>> m_mapChapterID2MainBranchSubTaskIDs;//�½�id->������id����
	//friend class CUserTaskMgr;
};

#endif
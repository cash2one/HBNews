
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
#include "def.h"

#include "map_mgr.h"

using namespace std;

class ITaskEnterCondition;
class stBattle;

struct CTask{
	UINT32	iTaskID;	//������ID
	ETASKTYPE	iType;		//TASKTYPE ��������
	CHAR	strName[63];//��������
	CHAR	strDesc[255];//��������
	UINT32	iMapPointID;//���������ľݵ㣬�е��У��е�û��
	UINT8	iShowPriority;//������ʾ���ȼ�0~127��ֵԽ��Խ��ǰ. Ӧ�ó��� ��:ĳ�������ж������ ������б� 
	bool	bHasEnterCondition;//�Ƿ��н������� 0û�� 1�У� �����еĻ����������Ľ�������д��./task/enter_condition/id.xml��  ����һ�����һ�ܵĴ�������  �ȼ� ְҵ�ȵ�����
	UINT32	iPrizeItemID;//������������ɺ�Ľ�����ƷID
	UINT32	iPrizeItemNum;//������������ɺ�Ľ�����Ʒ����
	CHAR	strImg[63];//���񱳾�ͼ
	bool	bHidden;//�Ƿ����ظ�����

	ITaskEnterCondition* enterConditions[MAXNUM_TASKENTERCONDITION];//�����������ƣ�

};

struct CSubTask{
	UINT32	iSubTaskID;//������ID��ȫ��Ψһ
	UINT32	iTaskID;//���������񣬿���û�С� ���� С����û�и����� ��������Ϊ��
	UINT8	iOrderNum;//�Ⱥ�˳�� ����Խ�� ����Խ��
	UINT32	iSpendHealth;//��������ֵ

	UINT32	iExpForUser;//�û�������
	UINT32	iExpForEachHero;//ÿ��Ӣ��������
	
	UINT32	iBurstItemID1;//��ʾ������Ʒ1
	UINT32	iBurstItemID2;
	UINT32	iBurstItemID3;
	UINT32	iBurstItemID4;

	
	vector<stBattle*> battles;//
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
			//todo log error  ��ӡ��ջ �����ߣ�����
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
			//todo log error  ��ӡ��ջ �����ߣ�����
			return NULL;
		}
	}

	//��ֱ�ӷ������õ�ԭ�� �������治С�������޸� �������ﷵ�ؿ���
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

	//��������
	map<UINT32, CTask*> mMapTaskID2Task;
	map<UINT32, CSubTask*> mMapSubTaskID2SubTask;

	//Ϊ�˸�Ч������������
	map<UINT32, vector<CSubTask*>> mMapTaskID2SubTaskList;//������ID->�������б�
	map<UINT32, vector<CTask*>> mMapMapPointID2TaskList;//��ͼ�ݵ�ID->�������б�
	map<UINT16, vector<CTask*>> mMapChapterID2MainTaskList;//�½�ID->���߸������б�  vector�����Ⱥ�˳�����vector��

	friend class CUserTaskMgr;
};

#endif
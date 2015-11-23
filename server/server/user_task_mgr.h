
/*****************************************************************************
*   
for : �û�������Ϣ����, �û���ͼ��Ϣ���
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
	INT32 iSubTaskID;//������ID
	UINT8 iStatus;//0 δͨ�� 1��ͨ��
	UINT8 iStar;//�����Ǽ�
	UINT32 iLastEnterTime ;//ʱ���
	UINT32 iEnterNumLastDay;
	UINT32 iTotalEnterNum;//����Ҷ���������ܹ����

	bool bHasGotItem1;//�Ƿ��Ѿ��õ����ĵ�һ����Ʒ
	bool bHasGotItem2;
	bool bHasGotItem3;
	bool bHasGotItem4;
};

//����Ҫ���û���������Ϣ�� ���״̬�Ǹ���������״̬����ģ� <-- ������Ҫ�� ��Ϊ����ܶ� ������ȴ��ţ�������Ҫ�����ڴ�״̬�����ݿ�����Ҫ�洢������ݣ���
struct CUserTask{
	UINT32 iUserID;
	UINT32 iTaskID;
	EUSERTASKSTATUS iStatus;//0 δͨ��  1.��ͨ�أ������Ѹ���
	
	vector<CUserSubTask*> mUserSubTaskList;
};


struct stUserChapterInfo{
	UINT16 iCurrentChapter;//��ǰ����½� �������ɹ� ��Ϊ���õ�����½� + 1
	CTask* pCurrentMainTask;//��һ����������
};

class CUserTaskMgr{
	
public:

	//todo ��������
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

	//��ȡ��ҵ��½���Ϣ�� �ж��Ƿ��������񶼹���
	EErrorCode GetUserChapterInfo( UINT32 iUserID, stUserChapterInfo& userChapterInfo);

	//todo!
	UINT32 GetChapterBornMapPointID(UINT16 iChapterID){
		return 1;
	}

	//��ȡ��ҿ��Կ��õ��ĵ㣬 ʵ���� ������ֻ��Ҫ�����Ŀǰ������Ϣ���ɣ� �ͻ�վ�Լ�����ɵ���㣡���ٷ�������������
	//���Ƿ�������Ҫ�ж���ʱ�� ������Ҫ���������̵ģ� ������һ��������������飡
	bool GetMapPointUserCanVisit( UINT32 iUserID, UINT16 iChapterID, set<UINT32> &retPoints, UINT32& curMainTaskPointID);



	void OnGS_C_Req_Open_Chapter_Map(UINT32 iUserID, UINT16 iChapterID, GS_C_Res_Open_Chapter_Map& st);
private:

	bool CanStartFight(UINT32 iUserID, UINT32 iSubTaskID){
		
		CUser* pUser = CUserMgr::GetInstance()->GetUserByUserID(iUserID);
		CHECK_NULL_RETURN_FALSE(pUser);

		CSubTask* pSubTask = CTaskMgr::GetInstance()->GetSubTaskBySubTaskID(iSubTaskID);
		CHECK_NULL_RETURN_FALSE(pSubTask);

		//1. �����ж�
		CHECK_NULL_RETURN_FALSE(pUser->GetHealth() < pSubTask->iSpendHealth );

		//2. ���������ж�
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
			//�����ݿ��ж�ȡ��Ϣ

			//return 
		}

		return emptyUserTaskList;
	}
	*/
	
	//vector<CUserTask*>�Ƿ�Ҫ��map���ӿ��ѯЧ�ʣ� �û�id -> mappointid/taskid  => info��
	// ����pair��Ϊkey ����Ϊ��Ҫ����userID �õ�һ��list��
	//map<UINT32, vector<CUserTask*>> mMapUserID2UserTaskList;

	map<UINT32, map<UINT32, CUserTask*>> mMapUserID2TaskID2UserTask;
	map<UINT32, map<UINT32, CUserSubTask*>> mMapUserID2SubTaskID2SubTask;
};

#endif
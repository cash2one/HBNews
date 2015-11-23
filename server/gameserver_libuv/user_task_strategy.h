#ifndef user_task_strategy_h__
#define user_task_strategy_h__

#include <map>
#include <set>

#include "gs_c_proto.pb.h"
#include "dbs_gs_proto.pb.h"
#include "def.h"
#include "config.h"
class CUser;

struct stUserChapterInfo{
	UINT16 iCurrentChapter;
	UINT32 iCurrentMainTaskID;
};

//struct stUserSubTask{
//	/** ������ID */
//	UINT32 sub_task_id;
//	/** ����Ǽ� */
//	UINT8 star ;
//	/** ��һ�ν���ʱ�� */
//	UINT32 last_enter_timestamp ;
//	/** �ϴν��뵱�յĽ������ */
//	UINT32 last_day_enter_num ;
//	/** �ܹ��Ľ������ */
//	UINT32 total_enter_num ;
//};

class db_user_sub_task_array;
class db_user_sub_task;
class CUser;
class IUserTaskStrategy{
public:

	virtual ~IUserTaskStrategy(){}

	virtual bool Init(CUser* pUser, db_user_sub_task_array* sub_task_info) = 0;
	//��ȡ��ҵ��½���Ϣ�� �ж��Ƿ��������񶼹���
	virtual e_gsc_errorcode GetChapterInfo(stUserChapterInfo& userChapterInfo) = 0;

	virtual bool AddOrUpdateSubTask(const db_user_sub_task& sub_task, gs_dbs_user_info_op_req* db_req ) = 0;
	virtual bool IsSubTaskDone(UINT32 iSubTaskID) = 0;
	virtual bool IsTaskDone(UINT32 iTaskID) = 0;
	virtual bool CanStartFight(CUser* pUser, UINT32 iSubTaskID) = 0;
	virtual e_gsc_errorcode SpeedKillSubTask(UINT32 iSubTaskID, UINT32 iTimes, ::google::protobuf::RepeatedPtrField< ::struct_game_element >& res_consumes, ::google::protobuf::RepeatedPtrField< ::struct_game_element_array >& res_output_items, user_shop& res_shop, struct_game_element& req_consume_ele,  gs_dbs_user_info_op_req& db_req, GS_C_NTF& ntf, bool& isShopOpen) = 0;
	//��ȡ��ҿ��Կ��õ��ĵ㣬 ʵ���� ������ֻ��Ҫ�����Ŀǰ������Ϣ���ɣ� �ͻ�վ�Լ�����ɵ���㣡���ٷ�������������
	//���Ƿ�������Ҫ�ж���ʱ�� ������Ҫ���������̵ģ� ������һ��������������飡
	virtual bool GetMapPointCanVisit(UINT16 iChapterID, std::set<UINT32> &retPoints, UINT32& curMainTaskPointID) = 0;
	virtual bool CanVisitMapPoint(UINT32 iMapPointID) = 0;

	virtual db_user_sub_task* GetSubTask(UINT32 iSubTaskID) = 0;
	virtual UINT32 GetSubTaskLeftEnterNum(const UINT32 iSubTaskID, UINT32 iVipLevel) = 0;
};


class CUserTaskStrategy : public IUserTaskStrategy
{
public:
	CUserTaskStrategy():m_sub_tasks(NULL), m_pUser(NULL){

	}

	virtual ~CUserTaskStrategy();

	virtual bool Init(CUser* pUser, db_user_sub_task_array* sub_task_info);
	//��ȡ��ҵ��½���Ϣ�� �ж��Ƿ��������񶼹���
	virtual e_gsc_errorcode GetChapterInfo(stUserChapterInfo& userChapterInfo);

	virtual bool AddOrUpdateSubTask(const db_user_sub_task& sub_task, gs_dbs_user_info_op_req* db_req );
	virtual bool IsSubTaskDone(UINT32 iSubTaskID);
	virtual bool IsTaskDone(UINT32 iTaskID);
	virtual bool CanStartFight(CUser* pUser, UINT32 iSubTaskID);
	virtual e_gsc_errorcode SpeedKillSubTask(UINT32 iSubTaskID, UINT32 iTimes, ::google::protobuf::RepeatedPtrField< ::struct_game_element >& res_consumes, ::google::protobuf::RepeatedPtrField< ::struct_game_element_array >& res_output_items, user_shop& res_shop, struct_game_element& req_consume_ele,  gs_dbs_user_info_op_req& db_req, GS_C_NTF& ntf, bool& isShopOpen);
	//��ȡ��ҿ��Կ��õ��ĵ㣬 ʵ���� ������ֻ��Ҫ�����Ŀǰ������Ϣ���ɣ� �ͻ�վ�Լ�����ɵ���㣡���ٷ�������������
	//���Ƿ�������Ҫ�ж���ʱ�� ������Ҫ���������̵ģ� ������һ��������������飡
	virtual bool GetMapPointCanVisit(UINT16 iChapterID, std::set<UINT32> &retPoints, UINT32& curMainTaskPointID);
	virtual bool CanVisitMapPoint(UINT32 iMapPointID);

	virtual db_user_sub_task* GetSubTask(UINT32 iSubTaskID);
	virtual UINT32 GetSubTaskLeftEnterNum(const UINT32 iSubTaskID, UINT32 iVipLevel);

	void IncreaseSubTaskEnterNum(UINT32 iSubTaskID, UINT32 iTimes, gs_dbs_user_info_op_req& db_req);
private:
	//������Ϣ
	CUser* m_pUser;
	db_user_sub_task_array*	m_sub_tasks;//Ϊ�˿�����ӣ�
	std::map<UINT32, std::vector<db_user_sub_task*>> mMapTaskID2SubTaskList;//
	std::map<UINT32, db_user_sub_task*> mMapSubTaskID2SubTask;//����user����  ������ʱ��Ҫ�ͷ����
};

#endif // user_task_strategy_h__
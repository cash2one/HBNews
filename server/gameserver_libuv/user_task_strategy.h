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
//	/** 子任务ID */
//	UINT32 sub_task_id;
//	/** 最佳星级 */
//	UINT8 star ;
//	/** 上一次进入时间 */
//	UINT32 last_enter_timestamp ;
//	/** 上次进入当日的进入次数 */
//	UINT32 last_day_enter_num ;
//	/** 总共的进入次数 */
//	UINT32 total_enter_num ;
//};

class db_user_sub_task_array;
class db_user_sub_task;
class CUser;
class IUserTaskStrategy{
public:

	virtual ~IUserTaskStrategy(){}

	virtual bool Init(CUser* pUser, db_user_sub_task_array* sub_task_info) = 0;
	//获取玩家的章节信息， 判断是否主线任务都过了
	virtual e_gsc_errorcode GetChapterInfo(stUserChapterInfo& userChapterInfo) = 0;

	virtual bool AddOrUpdateSubTask(const db_user_sub_task& sub_task, gs_dbs_user_info_op_req* db_req ) = 0;
	virtual bool IsSubTaskDone(UINT32 iSubTaskID) = 0;
	virtual bool IsTaskDone(UINT32 iTaskID) = 0;
	virtual bool CanStartFight(CUser* pUser, UINT32 iSubTaskID) = 0;
	virtual e_gsc_errorcode SpeedKillSubTask(UINT32 iSubTaskID, UINT32 iTimes, ::google::protobuf::RepeatedPtrField< ::struct_game_element >& res_consumes, ::google::protobuf::RepeatedPtrField< ::struct_game_element_array >& res_output_items, user_shop& res_shop, struct_game_element& req_consume_ele,  gs_dbs_user_info_op_req& db_req, GS_C_NTF& ntf, bool& isShopOpen) = 0;
	//获取玩家可以看得到的点， 实际上 服务器只需要给玩家目前任务信息即可， 客户站自己计算可到达点！减少服务器计算量！
	//但是服务器需要判定的时候 还是需要这个计算过程的！ 在上面一个函数做这个事情！
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
	//获取玩家的章节信息， 判断是否主线任务都过了
	virtual e_gsc_errorcode GetChapterInfo(stUserChapterInfo& userChapterInfo);

	virtual bool AddOrUpdateSubTask(const db_user_sub_task& sub_task, gs_dbs_user_info_op_req* db_req );
	virtual bool IsSubTaskDone(UINT32 iSubTaskID);
	virtual bool IsTaskDone(UINT32 iTaskID);
	virtual bool CanStartFight(CUser* pUser, UINT32 iSubTaskID);
	virtual e_gsc_errorcode SpeedKillSubTask(UINT32 iSubTaskID, UINT32 iTimes, ::google::protobuf::RepeatedPtrField< ::struct_game_element >& res_consumes, ::google::protobuf::RepeatedPtrField< ::struct_game_element_array >& res_output_items, user_shop& res_shop, struct_game_element& req_consume_ele,  gs_dbs_user_info_op_req& db_req, GS_C_NTF& ntf, bool& isShopOpen);
	//获取玩家可以看得到的点， 实际上 服务器只需要给玩家目前任务信息即可， 客户站自己计算可到达点！减少服务器计算量！
	//但是服务器需要判定的时候 还是需要这个计算过程的！ 在上面一个函数做这个事情！
	virtual bool GetMapPointCanVisit(UINT16 iChapterID, std::set<UINT32> &retPoints, UINT32& curMainTaskPointID);
	virtual bool CanVisitMapPoint(UINT32 iMapPointID);

	virtual db_user_sub_task* GetSubTask(UINT32 iSubTaskID);
	virtual UINT32 GetSubTaskLeftEnterNum(const UINT32 iSubTaskID, UINT32 iVipLevel);

	void IncreaseSubTaskEnterNum(UINT32 iSubTaskID, UINT32 iTimes, gs_dbs_user_info_op_req& db_req);
private:
	//任务信息
	CUser* m_pUser;
	db_user_sub_task_array*	m_sub_tasks;//为了可以添加！
	std::map<UINT32, std::vector<db_user_sub_task*>> mMapTaskID2SubTaskList;//
	std::map<UINT32, db_user_sub_task*> mMapSubTaskID2SubTask;//存在user上面  析构的时候不要释放这里！
};

#endif // user_task_strategy_h__
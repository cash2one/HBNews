#ifndef db_sql_h__
#define db_sql_h__

#pragma once

#define SELECT_CHAPTER	" SELECT * FROM `sys_chapter`;"
#define SELECT_MAPPOINT	" SELECT * FROM `sys_chapter_mappoint`;"

//因为需要主线任务 mMapChapterID2MainTaskList 根据优先级顺序来！
#define SELECT_TASK	" SELECT * FROM `sys_task` ORDER BY `priority` ASC;"

// 因为需要 mMapTaskID2SubTaskList 根据优先级顺序来！
#define SELECT_SUBTASK	" SELECT * FROM `sys_sub_task` ORDER BY `priority` ASC;"

//所有主线任务task_id对应的task_sub_id
#define SELECT_SUB_TASK_MAIN_BRANCH_ID "SELECT t2.chapter_id,t3.sub_task_id  FROM  sys_task as t1 left join sys_chapter_mappoint as t2 ON t1.mappoint_id = t2.mappoint_id left join  sys_sub_task as t3 on t3.task_id = t1.task_id where (t1.type = 1 OR t1.type = 2) and t2.type = 1;"

#define SELECT_ITEM	" SELECT * FROM `sys_item`;"

#define SELECT_BATTLE " SELECT * FROM `sys_battle`;"

#define SELECT_PACKET " SELECT * FROM `sys_burst_packet`;"

#define SELECT_EQUIP " SELECT * FROM `sys_equip`;"
#define SELECT_EQUIP_LEVEL_GOLD " SELECT * FROM `sys_equip_level_gold`;"
#define SELECT_EQUIP_QUALITY_GOLD " SELECT * FROM `sys_equip_quality_gold`;"

#define SELECT_VIP "SELECT * FROM `sys_vip`;"
#define SELECT_VIP_BUY "SELECT * FROM `sys_vip_buy` ORDER BY `buy_type` ASC, `no_less_than_buy_num` DESC;"

#define SELECT_SYS_SETTING "SELECT * FROM `sys_setting`;"

#define SELECT_USER_LEVEL "SELECT * FROM `sys_user_level`;"

#define SELECT_MISSION "SELECT * FROM `sys_mission` ORDER BY `mission_chain_id` ASC, `pre_mission_id` ASC;"

#define SELECT_SIGN "SELECT * FROM `sys_sign`;"

#define SELECT_GUIDE "SELECT * FROM `sys_guide`;"

#define SELECT_MARQUEE "SELECT * FROM `sys_marquee`;"

#define SELECT_FORBID_WORD "SELECT * FROM `sys_forbid_word`";

#define SELECT_ARENA_PRIZE "SELECT * FROM `sys_arena_prize`;";

#define SELECT_CRUSADE_PRIZE "SELECT * FROM `sys_crusade_prize`;"
#endif // db_sql_h__
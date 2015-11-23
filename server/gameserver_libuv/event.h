#ifndef event_h__
#define event_h__
#include "dbs_gs_proto.pb.h"
#include "event.h"
#include "def.h"
//此事件用途1.任务事件的触发 2.玩家成长记录
enum EEventType
{
	EEventType_Min = 0,
	EEventType_Skill_Level_Up = 1,			
	EEventType_Recharge = 2,				//充值
	EEventType_Common_FB_Complete = 3,		//完成副本
	EEventType_Common_FB_All_Through = 4,	//通关副本(sub_type表示副本id)
	EEventType_Common_FB_All_Through_Three_Star = 5,	//三星通关副本(sub_type表示副本id) 
	EEventType_Equip_Level_Up = 6,			//装备升级
	EEventType_Recharge_Mission_Month_Card1 = 7,		//月卡1充值
	EEventType_Give_Friend_Health = 8,		//赠送好友体力
	EEventType_Hero_Collect = 9,			//英雄收集
	EEventType_Health_Meal = 10,			//体力套餐
	EEventType_Get_Reward = 11,				//任务领取
	EEventType_Recharge_Mission_Month_Card2 = 12,		//月卡2充值
	EEventType_Mission_Main = 13,			//主线任务
	EEventType_Mission_Free = 14,			//自由任务
	EEventType_Activity = 15,				//活动任务
	EEventType_Invoke = 16,					//祭坛召唤
	EEventType_Arena_Challenge = 17,		//竞技场挑战
	EEventType_Trial_Tower = 18,			//试练塔
	EEventType_Expedition = 19,				//远征
	EEventType_Level_Up = 20,				//等级提升
	EEventType_Buy_Gold = 21,				//点金
	EEventType_FB_Fail = 22,				//副本挑战失败
	EEventType_Complete_Chapter_FB_Three_Star = 23,	//三星通关特定章节副本（大点的主线和支线,sub_type表示哪一章节）
	EEventType_Arena_Rank_Top = 24,				//竞技场头名
	EEventType_Complete_Chapter_FB = 25,	//完成特定章节副本多少次(sub_type表示哪一章节)

	//以下是服务器用到的枚举，客户端协议不需要
	EEventType_Buy_Health = 1000,				//购买体力
	EEventType_Buy_Skill_Point = 1001,		//购买技能点
	EEventType_Login = 1002,					//登陆
	EEventType_Del_Friend = 1003,				//删除好友
	EEventType_Crusade_Hero_Life = 1004,			//英雄复活（远征）
	EEventType_Crusade_Reset = 1005,			//远征重置
	EEventType_Crusade_Promote_Reward = 1006,	//提升奖励
	EEventType_Crusade_Refresh_Buff = 1007,
	EEventType_Max  = 1008									
};

enum EEventSubType
{
	EEventSubType_Min = 0,
	EEventSubType_None = 1,						//无子类型
	EEventSubType_Hero_Quality_Green = 2,		//英雄绿色品质
	EEventSubType_Hero_Quality_Blue = 3,		//英雄蓝色品质
	EEventSubType_Hero_Star_Four = 4,			//四星英雄
	EEventSubType_Hero_Star_Three = 5,			//三星英雄
	EEventSubType_Player_Level = 6,				//普通等级
	EEventSubType_Vip_Level = 7,				//vip等级
	EEventSubType_HuaXia = 8,					//华夏副本

	//召唤子类型 两个N都已经存储在了user表中 实际上free也可以从user表中判断 但是后面准备统一移到事件表
	EEventSubType_Invoke_GOLD_FREE = 9,			// 免费金币单次抽取 free可以根据时间戳来判断是否是第一次 但是 1类型是么有办法知道的 所以都用event——type
	EEventSubType_Invoke_GOLD_1 = 10,			//金币单次抽取（玩家消耗金币） free可以根据时间戳来判断是否是第一次 但是 1类型是么有办法知道的 所以都用event——type
	//EEventSubType_Invoke_GOLD_N = 11,			//金币十连抽取  这个user表中已经存了
	EEventSubType_Invoke_DIAMOND_FREE = 11,		//普通免费钻石抽取
	EEventSubType_Invoke_DIAMOND_1 = 12,		//普通钻石抽取（玩家消耗了钻石）
	//EEventSubType_Invoke_DIAMOND_N = 14,		//钻石十连抽取  这个user表中已经存了

	EEventSubType_Max = 13
};

//枚举与DB枚举类型转换(event_type)
db_user_mission_event::MISSION_EVENT_TYPE GetEventType(EEventType eEventType);

EEventType GetEventType(db_user_mission_event::MISSION_EVENT_TYPE event_type);

//枚举与DB枚举类型转换(event_sub_type)
db_user_mission_event::MISSION_EVENT_SUB_TYPE GetEventSubType(EEventSubType eEventSubType);    

//db枚举与客户端枚举转换
db_user_mission_event::MISSION_EVENT_TYPE TransformType(user_mission::MISSION_EVENT_TYPE);

user_mission::MISSION_EVENT_TYPE TransformType(db_user_mission_event::MISSION_EVENT_TYPE);

#endif

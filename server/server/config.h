/*****************************************************************************
*   for : 基本配置
data : 20150312
author : baomin
*
******************************************************************************/
#ifndef __CONFIG_H__
#define __CONFIG_H__

#pragma once;

#define MAXLEN_MAPPOINTNAME 255		//据点名字最大字节数
#define MAXNUM_JOINEDMAPPOINT 10	//最多连接据点数

#define MAXNUM_HEROSKILL 10 //英雄最多十个技能

#define MAXNUM_HEROINTEAM	5	//一个队伍最多的英雄数
#define MAXNUM_TEAM		1		//一个玩家最多可以设定的队伍数 可设置更大来扩展队伍数

#define MAXNUM_TASKENTERCONDITION	5 //一个任务最多的进入条件限制

#define MAXPROBITY_BURST	10000	//最大的爆率

enum EErrorCode{
	EErrorCode_Success = 0,
	EErrorCode_UnKown_Error = 1,
	EErrorCode_TASKCONFIG_ERROR = 2//任务配置问题
	
};

enum EArea{
	EArea_Huaxia	= 1,	//华夏
	EArea_Dahe		= 2,	//大和
	EArea_Europe	= 3,	//欧联和
	EArea_MAX		= 4
};

enum ENatureProperty{
	ENatureProperty_Huo		=	1,	//火
	ENatureProperty_Feng	=	2,	//风
	ENatureProperty_Shui	=	3,	//水
	ENatureProperty_Guang	=	4,	//光
	ENatureProperty_An		=	5,	//暗
	ENatureProperty_MAX		=	6
};

//很多英雄属性服务器不需要
enum EProperty{
	EProperty_Health = 1, //体力
	EProperty_Damage = 2, //伤害
	//EProperty_AttackSpeed = 3, //攻速
	EProperty_MAX
};

enum EOccupation{
	EOccupation_Warrior		=	1,	//近战 战士
	EOccupation_Archer		=	2,	//弓手 铳手 等远程物理
	EOccupation_Mage		=	3,	//法师
	EOccupation_Auxiliary	=	4,	//辅助
	EOccupation_MAX			=	5
};


enum ETASKTYPE{
	ETASKTYPE_MAIN = 1, //主线剧情
	ETASKTYPE_BRANCH = 2, //支线
	ETASKTYPE_ACTIVITY = 3//活动
};

enum EUSERSUBTASKSTATUS{
	EUSERSUBTASKSTATUS_NOT_DONE = 0 ,//未通过
	EUSERSUBTASKSTATUS_DONE = 1//1已通过
};

enum EUSERTASKSTATUS{
	EUSERTASKSTATUS_NOT_DONE = 0, 
	EUSERTASKSTATUS_DONE = 1
};

#endif
/*****************************************************************************
*   for : 基本配置
data : 20150312
author : baomin
*
******************************************************************************/
#ifndef config_h__
#define config_h__

#pragma once;

#include "def.h"

#define GSC_VERSION 1	//gs  c之间的版本编号

#define HEALTH_RECOVER_SECONDS	(60*5)	//5分钟恢复一点

#define MAXLEN_MAPPOINTNAME 255		//据点名字最大字节数
#define MAXNUM_JOINEDMAPPOINT 10	//最多连接据点数


#define MAXLEN_TASKNAME	63		//任务名称长度 注意中文占两个字节
#define MAXLEN_TASKDESC	255		//任务描述
#define MAXLEN_TASKIMG	63		//任务背景图
#define MAXLEN_USERNAME	63		//玩家昵称长度 注意中文占两个字节

#define MAXNUM_TASKBATTLE 10	//最多十场战斗
#define MAXNUM_TASKBATTLEPOS 12	//一场战斗最多12个位置 

//#define MAXNUM_TASKPRIZE_STRLEN 1023	//奖励道具的配置字符串长度
//#define MAXNUM_TASKPRIZE_NUM 1	//最多一个道具

#define SUBTASK_START_SPEND_HEALTH 1 //子任务开启的时候消耗的体力值  如果有剩下的体力值在胜利的时候才扣


#define MAXNUM_HEROPASSIVESKILL 3 //英雄最多三个被动技能
//#define MAXNUM_HEROEQUIP 5 //英雄最多6个装备

#define MAXNUM_HEROINTEAM	5	//一个队伍最多的英雄数
#define MAXNUM_TEAM		1		//一个玩家最多可以设定的队伍数 可设置更大来扩展队伍数

#define MAXNUM_CONDITION	10 //条件限制数量上限

#define MAXNUM_PROBABILITY	100000	//最大的爆率

#define MAXMAILNUM           99		//邮件最大封数

#define EACH_DAY_SECONDS 24 * 3600

#define INVALID_SERVER_ID -1		//无效的服务器id

#define MAXNUM_STAR	5		//最大星级

#define MAXNUM_SKILLLEVEL	80	//最大的技能等级
#define MAXNUM_SKILLPOINT	10	//最大的技能点
#define SKILLPOINT_RECOVER_SECONDS	(60*5)	//5分钟恢复一点

#define INVOKE_GOLD_FREE_RECOVER_SECONDS (60*5)	//5分钟增加一次免费机会
#define INVOKE_GOLD_FREE_MAXNUM_PER_DAY 5		//一天最多5次免费机会

#define MAXNUM_SHOP_ITEM_IN_SHOP 12	//商店最多展示12个道具
#define MAXNUM_SHOP_AUTO_REFRESH	10 //商店每天最多自动刷新次数

#define MONTH_CARD_BUY_MISSION1_ID     4//月卡任务1 id值
#define MONTH_CARD_REWARD_MISSION1_ID	5	
#define MONTH_CARD_PAY_MISSION1_ID     6


#define MONTH_CARD_BUY_MISSION2_ID     7//月卡任务2 id值
#define MONTH_CARD_REWARD_MISSION2_ID	8	
#define MONTH_CARD_PAY_MISSION2_ID     9

#define SIGN_N_FIRST_TIMES  7
#define SIGN_N_SECOND_TIMES  14
#define SIGN_N_THREE_TIMES  21
#define SIGN_N_FOUR_TIMES  28

#define MAXNUM_ARENA_RANK 100000	//竞技场最大排名10w
#define MAXNUM_ARENA_MATCH 3		//竞技场对手 每次最多3个
#define MAXNUM_ARENA_LOG 10			//每个玩家竞技场日志最多十条
#define MAXNUM_ARENA_CHALLENGE 5	//每个玩家每天最多免费挑战5次
#define MAXNUM_ARENA_PER_PAGE 10	//竞技场排行榜每一页10个人

#define MAX_NUM_RANK	50			//排行榜最大数量

#define MAX_NUM_CRUSADE_WAVE 15     //远征最大波数

enum EArea{
	EArea_MIN		= 0,
	EArea_Huaxia	= 1,	//华夏
	EArea_Dahe		= 2,	//大和
	EArea_Europe	= 3,	//欧联和
	EArea_MAX		= 4
};

//去掉这个设定！
//enum ENatureProperty{
//	ENatureProperty_Huo		=	1,	//火
//	ENatureProperty_Feng	=	2,	//风
//	ENatureProperty_Shui	=	3,	//水
//	ENatureProperty_Guang	=	4,	//光
//	ENatureProperty_An		=	5,	//暗
//	ENatureProperty_MAX		=	6
//};

//很多英雄属性服务器不需要
//最好是用配置文件生成
enum EProperty{
	EProperty_Quality = 0,
	EProperty_Health ,
	EProperty_HealthGrow,

	EProperty_Damage , //伤害

	EProperty_MAX

};

enum EOccupation{
	EOccupation_MIN			=	0,
	EOccupation_Warrior		=	1,	//近战 战士
	EOccupation_Archer		=	2,	//弓手 铳手 等远程物理
	EOccupation_Mage		=	3,	//法师
	EOccupation_Auxiliary	=	4,	//辅助
	EOccupation_MAX			=	5
};


enum EMISSIONTYPE{
	EMISSIONTYPE_MIN = 0,
	EMISSIONTYPE_HEALTH_LUNCH = 1,		//午餐
	EMISSIONTYPE_HEALTH_DINNER = 2,		//晚餐
	EMISSIONTYPE_HEALTH_NIGHT = 3,		//夜宵
	EMISSIONTYPE_MONTH_CARD_BUY = 4,	//月卡购买
	EMISSIONTYPE_MONTH_CARD_REWARD = 5,	//月卡奖励
	EMISSIONTYPE_MONTH_CARD_PAY = 6,	//月卡续费
	EMISSIONTYPE_DAILY = 7,			//每日
	EMISSIONTYPE_ACHIEVEMENT = 8,	//成就
	EMISSIONTYPE_MAX = 9
};

//任务重置类型
enum MissionChainType{
	MissionChainType_Min = 0,
	MissionChainType_Reset_Daily = 1,	//每日重置
	MissionChainType_Forever = 2,		//永久
	MissionChainType_Max = 3
};

enum MONTH_CARD_STATUS{
	MONTH_CARD_NORMAL,	//正常使用
	MONTH_CARD_PAY,	//续费
	MONTH_CARD_BUY,//购买
};
//只要是用户有子任务信息 那就是完成的！
enum EUSERSUBTASKSTATUS{
	EUSERSUBTASKSTATUS_NOT_DONE = 0 ,//未通过
	EUSERSUBTASKSTATUS_DONE = 1//1已通过
};

//enum EUSERTASKSTATUS{
//	EUSERTASKSTATUS_NOT_DONE = 0, 
//	EUSERTASKSTATUS_DONE = 1
//};

enum EELEMENTTYPE{

	EELEMENTTYPE_MIN = 0,
	EELEMENTTYPE_SKILL = 1,		//技能
	EELEMENTTYPE_TASK = 2,		//父任务
	EELEMENTTYPE_SUB_TASK = 3,	//子任务
	EELEMENTTYPE_BATTLE = 4,	//战斗
	EELEMENTTYPE_HERO = 5,		//英雄
	EELEMENTTYPE_BOSS = 6,		//boss
	EELEMENTTYPE_MONSTER = 7,	//怪物
	EELEMENTTYPE_ITEM = 8,		//普通道具
	//EELEMENTTYPE_FRAGMENT = 9,	//碎片 合并到ITEM中
	EELEMENTTYPE_EQUIP = 10,	//装备
	EELEMENTTYPE_EXP = 11,		//经验   id=1 玩家 id=2英雄
	EELEMENTTYPE_GOLD = 12,		//金币
	EELEMENTTYPE_DIAMOND = 13,	//钻石
	EELEMENTTYPE_HEALTH = 14,	//体力
	EELEMENTTYPE_SKILL_POINT = 15,	//技能点点数
	EELEMENTTYPE_ARENA_POINT = 16,	//竞技场点数 也是竞技场积分
	EELEMENTTYPE_PACKET = 17,	//产出包
	EELEMENTTYPE_INVOKE_GOLD_FREE = 18,		//免费金币单次召唤
	EELEMENTTYPE_INVOKE_DIAMOND_FREE = 19,	//免费钻石单次召唤
	EELEMENTTYPE_PLAYER_LEVEL = 20,			//玩家等级   
	EELEMENTTYPE_PLAYER_EXP = 21,			//玩家经验值 和EELEMENTTYPE_EXP的区别是 EELEMENTTYPE_EXP表示会产出的东西 而EELEMENTTYPE_PLAYER_EXP是表示玩家身上的经验槽
	EELEMENTTYPE_HERO_LEVEL = 22,			//英雄等级 此时id表示hero_id
	EELEMENTTYPE_HERO_EXP = 23,				//英雄经验值 此时id表示hero_id 和EELEMENTTYPE_EXP的区别是 EELEMENTTYPE_EXP表示会产出的东西 而 EELEMENTTYPE_HERO_EXP 针对对象是英雄身上的经验槽
	EELEMENTTYPE_SUB_TASK_ENTER_NUM = 24,	//副本已挑战次数 此时id表示sub_task_id
	EELEMENTTYPE_SUB_TASK_RESET_NUM = 25,	//副本已重置次数 此时id表示sub_task_id
	EELEMENTTYPE_FRIEND_POINT = 26,			//友情点
	EELEMENTTYPE_CRUSADE_POINT = 27,		//远征点
	EELEMENTTYPE_MAX = 28
};

enum EQUALITY{
	/*
	1  = 木
	2  = 铜
	3  = 铜+1
	4  = 银
	5  = 银+1
	6  = 银+2
	7  = 金
	8  = 金+1
	9  = 金+2
	10 = 金+3
	11 = 钻
	*/
	EQUALITY_MIN = 0,
	EQUALITY_MU = 1,
	EQUALITY_TONG = 2,
	EQUALITY_TONG1 = 3,
	EQUALITY_YING = 4,
	EQUALITY_YING1 = 5,
	EQUALITY_YING2 = 6,
	EQUALITY_JING = 7,
	EQUALITY_JING1 = 8,
	EQUALITY_JING2 = 9,
	EQUALITY_JING3 = 10,
	EQUALITY_ZUAN = 11,
	EQUALITY_MAX
};

enum EEQUIPTYPE{
	EEQUIPTYPE_MIN = 0, 
	EEQUIPTYPE_WEAPON = 1,
	EEQUIPTYPE_NECKLACE = 2,
	EEQUIPTYPE_CLOTHES = 3,
	EEQUIPTYPE_BELT = 4,
	EEQUIPTYPE_RING = 5,
	EEQUIPTYPE_BOOTS = 6,
	EEQUIPTYPE_MAX
};

enum ESKILLTYPE{
	ESKILLTYPE_MIN = 0,
	ESKILLTYPE_BISHA = 1,	//必杀
	ESKILLTYPE_AOYI = 2,	//奥义
	ESKILLTYPE_PASSIVE1 = 3,	//被动技能
	ESKILLTYPE_PASSIVE2 = 4,	//被动技能
	ESKILLTYPE_PASSIVE3 = 5,	//被动技能
	ESKILLTYPE_MAX
};

enum ERANKTYPE{
	ERANKTYPE_MIN = 0,
	ERANKTYPE_ARENA = 1,
	ERANKTYPE_FIGHT_POWER = 2,
	ERANKTYPE_LEVEL = 3,
	ERANKTYPE_MAX
};

#include "gs_c_proto.pb.h" // for struct_game_element

struct stGameElement{

	enum ENUMTYPE{
		MIN = 0,
		INCREASE = 1,	//增量
		DECREASE = 2,	//减量
		TOTAL	= 3,	//全量
		MAX = 4	
	};

	stGameElement(){
		eType = EELEMENTTYPE_MIN;
		iID = 0;
		iSubID = 0;
		eNumType = stGameElement::MIN;
		iNum = 0;
	}
	stGameElement(EELEMENTTYPE eType, UINT32 eleID, UINT32 eleSubID, ENUMTYPE num_type, UINT32 num):eType(eType), iID(eleID), iSubID(eleSubID), eNumType(num_type), iNum(num){};
	
	bool Fill(struct_game_element& ele) const{
		ele.Clear();
		struct_game_element::EELEMENTTYPE ele_type;
		switch (eType)
		{
		case EELEMENTTYPE_ITEM:
			ele_type = struct_game_element::EELEMENTTYPE_ITEM;
			break;
		case EELEMENTTYPE_EXP:
			ele_type = struct_game_element::EELEMENTTYPE_EXP;
			break;
		case EELEMENTTYPE_GOLD:
			ele_type = struct_game_element::EELEMENTTYPE_GOLD;
			break;
		case EELEMENTTYPE_HERO:
			ele_type = struct_game_element::EELEMENTTYPE_HERO;
			break;
		case EELEMENTTYPE_DIAMOND:
			ele_type = struct_game_element::EELEMENTTYPE_DIAMOND;
			break;
		case EELEMENTTYPE_HEALTH:
			ele_type = struct_game_element::EELEMENTTYPE_HEALTH;
			break;
		case EELEMENTTYPE_ARENA_POINT:
			ele_type = struct_game_element::EELEMENTTYPE_ARENA_POINT;
			break;
		case EELEMENTTYPE_FRIEND_POINT:
			ele_type = struct_game_element::EELEMENTTYPE_FRIEND_POINT;
			break;
		case EELEMENTTYPE_CRUSADE_POINT:
			ele_type = struct_game_element::EELEMENTTYPE_CRUSADE_POINT;
			break;
		default:
			ASSERT(false && "不支持转化的元素类型");
			return false;
		}

		struct_game_element::EELEMENT_NUM_TYPE ele_num_type;
		switch (eNumType)
		{
		case stGameElement::INCREASE:
			ele_num_type = struct_game_element::INCREASE;
			break;
		case stGameElement::DECREASE:
			ele_num_type = struct_game_element::DECREASE;
			break;
		case stGameElement::TOTAL:
			ele_num_type = struct_game_element::TOTAL;
			break;
		default:
			ASSERT(false && "不支持转化的元素数量类型");
			return false;
		}

		ele.set_type(ele_type);
		ele.set_id(iID);
		ele.set_sub_id(iSubID);
		ele.set_num_type(ele_num_type);
		ele.set_num(iNum);

		return true;
	}
	
	bool CopyFrom(const struct_game_element& ele){
		switch (ele.type())
		{
		case struct_game_element::EELEMENTTYPE_ITEM:
			eType = EELEMENTTYPE_ITEM;
			break;
		case struct_game_element::EELEMENTTYPE_EXP:
			eType = EELEMENTTYPE_EXP;
			break;
		case struct_game_element::EELEMENTTYPE_GOLD:
			eType = EELEMENTTYPE_GOLD;
			break;
		case struct_game_element::EELEMENTTYPE_HERO:
			eType = EELEMENTTYPE_HERO;
			break;
		case struct_game_element::EELEMENTTYPE_DIAMOND:
			eType = EELEMENTTYPE_DIAMOND;
			break;
		case struct_game_element::EELEMENTTYPE_ARENA_POINT:
			eType = EELEMENTTYPE_ARENA_POINT;
			break;
		case struct_game_element::EELEMENTTYPE_FRIEND_POINT:
			eType = EELEMENTTYPE_FRIEND_POINT;
			break;
		case struct_game_element::EELEMENTTYPE_CRUSADE_POINT:
			eType = EELEMENTTYPE_CRUSADE_POINT;
			break;
		default:
			ASSERT(false && "不支持转化的元素类型");
			return false;
		}

		switch (ele.num_type())
		{
		case struct_game_element::INCREASE:
			eNumType = stGameElement::INCREASE;
			break;
		case struct_game_element::DECREASE:
			eNumType = stGameElement::DECREASE;
			break;
		case struct_game_element::TOTAL:
			eNumType = stGameElement::TOTAL;
			break;
		default:
			ASSERT(false && "不支持转化的元素数量类型");
			return false;
		}
		

		iID = ele.id();
		iSubID = ele.sub_id();
		iNum = ele.num();
		return true;
	}

	operator struct_game_element(){
		struct_game_element ele;

		//ele.set_type(struct_game_element::EELEMENTTYPE(eType));
		//ele.set_id(iID);
		//ele.set_sub_id(iSubID);
		//ele.set_num_type(struct_game_element::EELEMENT_NUM_TYPE(eNumType));
		//ele.set_num(iNum);

		bool r = Fill(ele);
		ASSERT(r);

		return ele;
	}

	static bool Parse( const char* utf8_str, std::vector<stGameElement> & ret, ENUMTYPE eNumType);

	EELEMENTTYPE eType;
	UINT32 iID;
	UINT32 iSubID;
	ENUMTYPE eNumType;
	UINT32 iNum;
};

bool IsElementValid(const stGameElement& ele, bool check_num_limit = false);
bool IsElementValid(EELEMENTTYPE eType, UINT32 iID);

struct GS_C_NTF{
	GS_C_NTF():mis_ntf(NULL), mail_ntf(NULL), marquee_ntf(NULL),element_ntf(NULL), friend_ntf(NULL), chat_ntf(NULL){}

	~GS_C_NTF(){
		SAFE_DEL(mis_ntf);
		SAFE_DEL(mail_ntf);
		SAFE_DEL(marquee_ntf);
		SAFE_DEL(element_ntf);
		SAFE_DEL(friend_ntf);
		SAFE_DEL(chat_ntf);
	}

	gs_c_mission_ntf_array* mutable_mis_ntf_arr(){
		if (!mis_ntf){
			mis_ntf = new gs_c_mission_ntf_array;
		}
		return mis_ntf;
	}
	
	gs_c_mail_ntf_array* mutable_mail_ntf_arr(){
		if (!mail_ntf){
			mail_ntf = new gs_c_mail_ntf_array;
		}
		return mail_ntf;
	}
	
	gs_c_marquee_ntf* mutable_marquee_ntf(){
		if (!marquee_ntf){
			marquee_ntf = new gs_c_marquee_ntf;
		}
		return marquee_ntf;
	}

	gs_c_element_ntf* mutable_element_ntf(){
		if (!element_ntf){
			element_ntf = new gs_c_element_ntf;
		}
		return element_ntf;
	}
	
	gs_c_friend_ntf* mutable_friend_ntf(){
		if (!friend_ntf){
			friend_ntf = new gs_c_friend_ntf;
		}
		return friend_ntf;
	}
	
	gs_c_chat_ntf* mutable_chat_ntf(){
		if (!chat_ntf){
			chat_ntf = new gs_c_chat_ntf;
		}
		return chat_ntf;
	}

	bool is_mission_ntf_arr_init() const{
		if (mis_ntf){
			return mis_ntf->mis_opts_size() > 0;
		}
		return false;
	}
	
	bool is_mail_ntf_arr_init() const{
		if (mail_ntf){
			return mail_ntf->mails_size() || mail_ntf->eles_size();
		}
		return false;
	}
	
	bool is_marquee_ntf_init() const{
		if (marquee_ntf){
			return marquee_ntf->IsInitialized();
		}
		return false;
	}
	
	bool is_element_ntf_init() const{
		if (element_ntf){
			return element_ntf->eles_size() != 0;
		}
		return false;
	}
	
	bool is_friend_ntf_init() const{
		if(friend_ntf){
			return friend_ntf->IsInitialized() && (friend_ntf->has_friend_id() || friend_ntf->has_base_info());
		}
		return false;
	}
	
	bool is_chat_ntf_init() const{
		if(chat_ntf){
			return chat_ntf->IsInitialized();
		}
		return false;
	}
	bool has_mail_ntf_arr(){
		return mail_ntf != NULL;
	}
	
	bool has_mission_ntf_arr(){
		return mis_ntf != NULL;
	}

	bool release_mail_ntf_arr(){
		SAFE_DEL(mail_ntf);
		return true;
	}
private:
	gs_c_mission_ntf_array* mis_ntf ;
	gs_c_mail_ntf_array* mail_ntf;
	gs_c_marquee_ntf* marquee_ntf;
	gs_c_element_ntf* element_ntf;
	gs_c_friend_ntf* friend_ntf;
	gs_c_chat_ntf* chat_ntf;
};


#endif // config_h__
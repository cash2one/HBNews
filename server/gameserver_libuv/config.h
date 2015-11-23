/*****************************************************************************
*   for : ��������
data : 20150312
author : baomin
*
******************************************************************************/
#ifndef config_h__
#define config_h__

#pragma once;

#include "def.h"

#define GSC_VERSION 1	//gs  c֮��İ汾���

#define HEALTH_RECOVER_SECONDS	(60*5)	//5���ӻָ�һ��

#define MAXLEN_MAPPOINTNAME 255		//�ݵ���������ֽ���
#define MAXNUM_JOINEDMAPPOINT 10	//������Ӿݵ���


#define MAXLEN_TASKNAME	63		//�������Ƴ��� ע������ռ�����ֽ�
#define MAXLEN_TASKDESC	255		//��������
#define MAXLEN_TASKIMG	63		//���񱳾�ͼ
#define MAXLEN_USERNAME	63		//����ǳƳ��� ע������ռ�����ֽ�

#define MAXNUM_TASKBATTLE 10	//���ʮ��ս��
#define MAXNUM_TASKBATTLEPOS 12	//һ��ս�����12��λ�� 

//#define MAXNUM_TASKPRIZE_STRLEN 1023	//�������ߵ������ַ�������
//#define MAXNUM_TASKPRIZE_NUM 1	//���һ������

#define SUBTASK_START_SPEND_HEALTH 1 //����������ʱ�����ĵ�����ֵ  �����ʣ�µ�����ֵ��ʤ����ʱ��ſ�


#define MAXNUM_HEROPASSIVESKILL 3 //Ӣ�����������������
//#define MAXNUM_HEROEQUIP 5 //Ӣ�����6��װ��

#define MAXNUM_HEROINTEAM	5	//һ����������Ӣ����
#define MAXNUM_TEAM		1		//һ������������趨�Ķ����� �����ø�������չ������

#define MAXNUM_CONDITION	10 //����������������

#define MAXNUM_PROBABILITY	100000	//���ı���

#define MAXMAILNUM           99		//�ʼ�������

#define EACH_DAY_SECONDS 24 * 3600

#define INVALID_SERVER_ID -1		//��Ч�ķ�����id

#define MAXNUM_STAR	5		//����Ǽ�

#define MAXNUM_SKILLLEVEL	80	//���ļ��ܵȼ�
#define MAXNUM_SKILLPOINT	10	//���ļ��ܵ�
#define SKILLPOINT_RECOVER_SECONDS	(60*5)	//5���ӻָ�һ��

#define INVOKE_GOLD_FREE_RECOVER_SECONDS (60*5)	//5��������һ����ѻ���
#define INVOKE_GOLD_FREE_MAXNUM_PER_DAY 5		//һ�����5����ѻ���

#define MAXNUM_SHOP_ITEM_IN_SHOP 12	//�̵����չʾ12������
#define MAXNUM_SHOP_AUTO_REFRESH	10 //�̵�ÿ������Զ�ˢ�´���

#define MONTH_CARD_BUY_MISSION1_ID     4//�¿�����1 idֵ
#define MONTH_CARD_REWARD_MISSION1_ID	5	
#define MONTH_CARD_PAY_MISSION1_ID     6


#define MONTH_CARD_BUY_MISSION2_ID     7//�¿�����2 idֵ
#define MONTH_CARD_REWARD_MISSION2_ID	8	
#define MONTH_CARD_PAY_MISSION2_ID     9

#define SIGN_N_FIRST_TIMES  7
#define SIGN_N_SECOND_TIMES  14
#define SIGN_N_THREE_TIMES  21
#define SIGN_N_FOUR_TIMES  28

#define MAXNUM_ARENA_RANK 100000	//�������������10w
#define MAXNUM_ARENA_MATCH 3		//���������� ÿ�����3��
#define MAXNUM_ARENA_LOG 10			//ÿ����Ҿ�������־���ʮ��
#define MAXNUM_ARENA_CHALLENGE 5	//ÿ�����ÿ����������ս5��
#define MAXNUM_ARENA_PER_PAGE 10	//���������а�ÿһҳ10����

#define MAX_NUM_RANK	50			//���а��������

#define MAX_NUM_CRUSADE_WAVE 15     //Զ�������

enum EArea{
	EArea_MIN		= 0,
	EArea_Huaxia	= 1,	//����
	EArea_Dahe		= 2,	//���
	EArea_Europe	= 3,	//ŷ����
	EArea_MAX		= 4
};

//ȥ������趨��
//enum ENatureProperty{
//	ENatureProperty_Huo		=	1,	//��
//	ENatureProperty_Feng	=	2,	//��
//	ENatureProperty_Shui	=	3,	//ˮ
//	ENatureProperty_Guang	=	4,	//��
//	ENatureProperty_An		=	5,	//��
//	ENatureProperty_MAX		=	6
//};

//�ܶ�Ӣ�����Է���������Ҫ
//������������ļ�����
enum EProperty{
	EProperty_Quality = 0,
	EProperty_Health ,
	EProperty_HealthGrow,

	EProperty_Damage , //�˺�

	EProperty_MAX

};

enum EOccupation{
	EOccupation_MIN			=	0,
	EOccupation_Warrior		=	1,	//��ս սʿ
	EOccupation_Archer		=	2,	//���� ��� ��Զ������
	EOccupation_Mage		=	3,	//��ʦ
	EOccupation_Auxiliary	=	4,	//����
	EOccupation_MAX			=	5
};


enum EMISSIONTYPE{
	EMISSIONTYPE_MIN = 0,
	EMISSIONTYPE_HEALTH_LUNCH = 1,		//���
	EMISSIONTYPE_HEALTH_DINNER = 2,		//���
	EMISSIONTYPE_HEALTH_NIGHT = 3,		//ҹ��
	EMISSIONTYPE_MONTH_CARD_BUY = 4,	//�¿�����
	EMISSIONTYPE_MONTH_CARD_REWARD = 5,	//�¿�����
	EMISSIONTYPE_MONTH_CARD_PAY = 6,	//�¿�����
	EMISSIONTYPE_DAILY = 7,			//ÿ��
	EMISSIONTYPE_ACHIEVEMENT = 8,	//�ɾ�
	EMISSIONTYPE_MAX = 9
};

//������������
enum MissionChainType{
	MissionChainType_Min = 0,
	MissionChainType_Reset_Daily = 1,	//ÿ������
	MissionChainType_Forever = 2,		//����
	MissionChainType_Max = 3
};

enum MONTH_CARD_STATUS{
	MONTH_CARD_NORMAL,	//����ʹ��
	MONTH_CARD_PAY,	//����
	MONTH_CARD_BUY,//����
};
//ֻҪ���û�����������Ϣ �Ǿ�����ɵģ�
enum EUSERSUBTASKSTATUS{
	EUSERSUBTASKSTATUS_NOT_DONE = 0 ,//δͨ��
	EUSERSUBTASKSTATUS_DONE = 1//1��ͨ��
};

//enum EUSERTASKSTATUS{
//	EUSERTASKSTATUS_NOT_DONE = 0, 
//	EUSERTASKSTATUS_DONE = 1
//};

enum EELEMENTTYPE{

	EELEMENTTYPE_MIN = 0,
	EELEMENTTYPE_SKILL = 1,		//����
	EELEMENTTYPE_TASK = 2,		//������
	EELEMENTTYPE_SUB_TASK = 3,	//������
	EELEMENTTYPE_BATTLE = 4,	//ս��
	EELEMENTTYPE_HERO = 5,		//Ӣ��
	EELEMENTTYPE_BOSS = 6,		//boss
	EELEMENTTYPE_MONSTER = 7,	//����
	EELEMENTTYPE_ITEM = 8,		//��ͨ����
	//EELEMENTTYPE_FRAGMENT = 9,	//��Ƭ �ϲ���ITEM��
	EELEMENTTYPE_EQUIP = 10,	//װ��
	EELEMENTTYPE_EXP = 11,		//����   id=1 ��� id=2Ӣ��
	EELEMENTTYPE_GOLD = 12,		//���
	EELEMENTTYPE_DIAMOND = 13,	//��ʯ
	EELEMENTTYPE_HEALTH = 14,	//����
	EELEMENTTYPE_SKILL_POINT = 15,	//���ܵ����
	EELEMENTTYPE_ARENA_POINT = 16,	//���������� Ҳ�Ǿ���������
	EELEMENTTYPE_PACKET = 17,	//������
	EELEMENTTYPE_INVOKE_GOLD_FREE = 18,		//��ѽ�ҵ����ٻ�
	EELEMENTTYPE_INVOKE_DIAMOND_FREE = 19,	//�����ʯ�����ٻ�
	EELEMENTTYPE_PLAYER_LEVEL = 20,			//��ҵȼ�   
	EELEMENTTYPE_PLAYER_EXP = 21,			//��Ҿ���ֵ ��EELEMENTTYPE_EXP�������� EELEMENTTYPE_EXP��ʾ������Ķ��� ��EELEMENTTYPE_PLAYER_EXP�Ǳ�ʾ������ϵľ����
	EELEMENTTYPE_HERO_LEVEL = 22,			//Ӣ�۵ȼ� ��ʱid��ʾhero_id
	EELEMENTTYPE_HERO_EXP = 23,				//Ӣ�۾���ֵ ��ʱid��ʾhero_id ��EELEMENTTYPE_EXP�������� EELEMENTTYPE_EXP��ʾ������Ķ��� �� EELEMENTTYPE_HERO_EXP ��Զ�����Ӣ�����ϵľ����
	EELEMENTTYPE_SUB_TASK_ENTER_NUM = 24,	//��������ս���� ��ʱid��ʾsub_task_id
	EELEMENTTYPE_SUB_TASK_RESET_NUM = 25,	//���������ô��� ��ʱid��ʾsub_task_id
	EELEMENTTYPE_FRIEND_POINT = 26,			//�����
	EELEMENTTYPE_CRUSADE_POINT = 27,		//Զ����
	EELEMENTTYPE_MAX = 28
};

enum EQUALITY{
	/*
	1  = ľ
	2  = ͭ
	3  = ͭ+1
	4  = ��
	5  = ��+1
	6  = ��+2
	7  = ��
	8  = ��+1
	9  = ��+2
	10 = ��+3
	11 = ��
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
	ESKILLTYPE_BISHA = 1,	//��ɱ
	ESKILLTYPE_AOYI = 2,	//����
	ESKILLTYPE_PASSIVE1 = 3,	//��������
	ESKILLTYPE_PASSIVE2 = 4,	//��������
	ESKILLTYPE_PASSIVE3 = 5,	//��������
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
		INCREASE = 1,	//����
		DECREASE = 2,	//����
		TOTAL	= 3,	//ȫ��
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
			ASSERT(false && "��֧��ת����Ԫ������");
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
			ASSERT(false && "��֧��ת����Ԫ����������");
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
			ASSERT(false && "��֧��ת����Ԫ������");
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
			ASSERT(false && "��֧��ת����Ԫ����������");
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
#ifndef event_h__
#define event_h__
#include "dbs_gs_proto.pb.h"
#include "event.h"
#include "def.h"
//���¼���;1.�����¼��Ĵ��� 2.��ҳɳ���¼
enum EEventType
{
	EEventType_Min = 0,
	EEventType_Skill_Level_Up = 1,			
	EEventType_Recharge = 2,				//��ֵ
	EEventType_Common_FB_Complete = 3,		//��ɸ���
	EEventType_Common_FB_All_Through = 4,	//ͨ�ظ���(sub_type��ʾ����id)
	EEventType_Common_FB_All_Through_Three_Star = 5,	//����ͨ�ظ���(sub_type��ʾ����id) 
	EEventType_Equip_Level_Up = 6,			//װ������
	EEventType_Recharge_Mission_Month_Card1 = 7,		//�¿�1��ֵ
	EEventType_Give_Friend_Health = 8,		//���ͺ�������
	EEventType_Hero_Collect = 9,			//Ӣ���ռ�
	EEventType_Health_Meal = 10,			//�����ײ�
	EEventType_Get_Reward = 11,				//������ȡ
	EEventType_Recharge_Mission_Month_Card2 = 12,		//�¿�2��ֵ
	EEventType_Mission_Main = 13,			//��������
	EEventType_Mission_Free = 14,			//��������
	EEventType_Activity = 15,				//�����
	EEventType_Invoke = 16,					//��̳�ٻ�
	EEventType_Arena_Challenge = 17,		//��������ս
	EEventType_Trial_Tower = 18,			//������
	EEventType_Expedition = 19,				//Զ��
	EEventType_Level_Up = 20,				//�ȼ�����
	EEventType_Buy_Gold = 21,				//���
	EEventType_FB_Fail = 22,				//������սʧ��
	EEventType_Complete_Chapter_FB_Three_Star = 23,	//����ͨ���ض��½ڸ������������ߺ�֧��,sub_type��ʾ��һ�½ڣ�
	EEventType_Arena_Rank_Top = 24,				//������ͷ��
	EEventType_Complete_Chapter_FB = 25,	//����ض��½ڸ������ٴ�(sub_type��ʾ��һ�½�)

	//�����Ƿ������õ���ö�٣��ͻ���Э�鲻��Ҫ
	EEventType_Buy_Health = 1000,				//��������
	EEventType_Buy_Skill_Point = 1001,		//�����ܵ�
	EEventType_Login = 1002,					//��½
	EEventType_Del_Friend = 1003,				//ɾ������
	EEventType_Crusade_Hero_Life = 1004,			//Ӣ�۸��Զ����
	EEventType_Crusade_Reset = 1005,			//Զ������
	EEventType_Crusade_Promote_Reward = 1006,	//��������
	EEventType_Crusade_Refresh_Buff = 1007,
	EEventType_Max  = 1008									
};

enum EEventSubType
{
	EEventSubType_Min = 0,
	EEventSubType_None = 1,						//��������
	EEventSubType_Hero_Quality_Green = 2,		//Ӣ����ɫƷ��
	EEventSubType_Hero_Quality_Blue = 3,		//Ӣ����ɫƷ��
	EEventSubType_Hero_Star_Four = 4,			//����Ӣ��
	EEventSubType_Hero_Star_Three = 5,			//����Ӣ��
	EEventSubType_Player_Level = 6,				//��ͨ�ȼ�
	EEventSubType_Vip_Level = 7,				//vip�ȼ�
	EEventSubType_HuaXia = 8,					//���ĸ���

	//�ٻ������� ����N���Ѿ��洢����user���� ʵ����freeҲ���Դ�user�����ж� ���Ǻ���׼��ͳһ�Ƶ��¼���
	EEventSubType_Invoke_GOLD_FREE = 9,			// ��ѽ�ҵ��γ�ȡ free���Ը���ʱ������ж��Ƿ��ǵ�һ�� ���� 1������ô�а취֪���� ���Զ���event����type
	EEventSubType_Invoke_GOLD_1 = 10,			//��ҵ��γ�ȡ��������Ľ�ң� free���Ը���ʱ������ж��Ƿ��ǵ�һ�� ���� 1������ô�а취֪���� ���Զ���event����type
	//EEventSubType_Invoke_GOLD_N = 11,			//���ʮ����ȡ  ���user�����Ѿ�����
	EEventSubType_Invoke_DIAMOND_FREE = 11,		//��ͨ�����ʯ��ȡ
	EEventSubType_Invoke_DIAMOND_1 = 12,		//��ͨ��ʯ��ȡ�������������ʯ��
	//EEventSubType_Invoke_DIAMOND_N = 14,		//��ʯʮ����ȡ  ���user�����Ѿ�����

	EEventSubType_Max = 13
};

//ö����DBö������ת��(event_type)
db_user_mission_event::MISSION_EVENT_TYPE GetEventType(EEventType eEventType);

EEventType GetEventType(db_user_mission_event::MISSION_EVENT_TYPE event_type);

//ö����DBö������ת��(event_sub_type)
db_user_mission_event::MISSION_EVENT_SUB_TYPE GetEventSubType(EEventSubType eEventSubType);    

//dbö����ͻ���ö��ת��
db_user_mission_event::MISSION_EVENT_TYPE TransformType(user_mission::MISSION_EVENT_TYPE);

user_mission::MISSION_EVENT_TYPE TransformType(db_user_mission_event::MISSION_EVENT_TYPE);

#endif

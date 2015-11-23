#ifndef sys_setting_mgr_h__
#define sys_setting_mgr_h__

#include <string>
#include <map>

#include "def.h"
#include "config.h"

using std::string;
using std::map;

class ICondition;

class CValue{
public:
	enum EValueType{
		EValueType_MIN = 0,
		EValueType_INT = 1,
		EValueType_STRING = 2,	//ֻ֧�� UTF8��
		EValueType_FLOAT = 3,
		EValueType_MAX = 4
	};

	CValue():value_type(EValueType_MIN), int_value(0), float_value(0), string_value(NULL){

	}

	~CValue(){
		if (value_type == EValueType_STRING && string_value != NULL)
		{
			delete string_value;
			string_value = NULL;
		}
	}

	UINT32 GetIntValue(){
		ASSERT(value_type == EValueType_INT);
		return int_value;
	}

	void SetIntValue(UINT32 value){
		ASSERT(value_type == EValueType_MIN);
		value_type = EValueType_INT;
		int_value = value;
	}

	float GetFloatValue(){
		ASSERT(value_type == EValueType_FLOAT);
		return float_value;
	}

	void SetFloatValue(float value){
		ASSERT(value_type == EValueType_MIN);
		value_type = EValueType_FLOAT;
		float_value = value;
	}

	const char* GetStringValue(){
		ASSERT(value_type == EValueType_STRING);
		return string_value;
	}

	void SetStringValue(const char* value){
		ASSERT(value_type == EValueType_MIN);
		value_type = EValueType_STRING;
		
		UINT32 len = strlen(value);
		string_value = new char[len+1];
		memcpy(string_value, value, len);
		string_value[len] = 0;//��ֹ��
	}

	EValueType value_type;
	UINT32	int_value;
	float float_value;
	char* string_value;
};

class CSysSettingMgr{

public:
	//todo ��������
	inline static CSysSettingMgr* GetInstance(){
		static CSysSettingMgr singleton;
		return &singleton;
	}

	bool Init();

	bool UnInit();

	bool CheckSettingValid(){
		if (Max_Num_Vip_Level
			&& Max_Num_User_Level
			&& Max_Num_Hero_Star
			&& Max_Num_Hero_Quality){
				return true;
		}

		return false;
	}

	//���봫ָ�����ý��� ������������β� �����ǽ��ղ������ݵģ�
	bool GetStringValue(std::string key, const char* &value){
		CValue* v = _get_value(key);
		if (!v)
		{
			return false;
		}

		value = v->GetStringValue();
		return true;
	}

	//bool GetIntValue(const char* key, UINT32& value){
	//	CValue* v = _get_value(key);
	//	if (!v)
	//	{
	//		return false;
	//	}

	//	value = v->GetIntValue();
	//	return true;
	//}

	bool GetIntValue(std::string key, UINT32& value){
		CValue* v = _get_value(key);
		if (!v)
		{
			return false;
		}

		value = v->GetIntValue();
		return true;
	}
	bool GetFloatValue(std::string key, float& value){
		CValue* v = _get_value(key);
		if (!v)
		{
			return false;
		}

		value = v->GetFloatValue();
		return true;
	}

	//UINT32 global_reset_time() const { return Global_Reset_Time; }
	//void global_reset_time(UINT32 val) { Global_Reset_Time = val; }


	//���õĲ���ֱ�Ӵ������� ������ÿ�ζ�ȥ����
	UINT32 Global_Reset_Time;//

	UINT32 User_Level_Function_Arena;
	UINT32 User_Level_Function_Shop_Common;
	UINT32 User_Level_Function_Shop_Qiwuge;
	UINT32 User_Level_Function_Shop_Yijiebaoku;
	UINT32 User_Level_Function_Shop_Arena;
	UINT32 User_Level_Function_Shop_Expedition;
	UINT32 User_Level_Function_Invoke;
	UINT32 User_Level_Function_Upgrade_Hero_Skill;
	UINT32 User_Level_Function_Expedition;
	UINT32 User_Level_Function_Trial_Tower;
	UINT32 User_Level_Function_Friend;
	UINT32 User_Level_Function_Hello_World;
	UINT32 User_Level_Function_Everyday_Mission;
	UINT32 User_Level_Function_Guild;
	UINT32 User_Level_Function_Achivement_Mission;
	UINT32 User_Level_Function_Mail;
	UINT32 User_Level_Function_CheckIn;
	
	ICondition* Shop_Qiwuge_Open_Activity_time[MAXNUM_CONDITION];
	ICondition* Shop_Yijiebaoku_Open_Activity_time[MAXNUM_CONDITION];
	UINT32 Shop_Common_Reset_Time[MAXNUM_SHOP_AUTO_REFRESH];//���Ϊ0�ͱ�ʾ���賿�� ����Ҫ������û�г�ʼ�����Ǳ�����Ҫ����Ϊ0�� ����Ҫ֪�����õĸ���
	UINT32 Shop_Common_Reset_Time_Num;//ÿ��ˢ�����ô���
	UINT32 Shop_Qiwuge_Reset_Time[MAXNUM_SHOP_AUTO_REFRESH];
	UINT32 Shop_Qiwuge_Reset_Time_Num;//ÿ��ˢ�����ô���
	UINT32 Shop_Yijiebaoku_Reset_Time[MAXNUM_SHOP_AUTO_REFRESH];
	UINT32 Shop_Yijiebaoku_Reset_Time_Num;//ÿ��ˢ�����ô���
	UINT32 Shop_Arena_Reset_Time[MAXNUM_SHOP_AUTO_REFRESH];
	UINT32 Shop_Arena_Reset_Time_Num;//ÿ��ˢ�����ô���
	UINT32 Shop_Expedition_Reset_Time[MAXNUM_SHOP_AUTO_REFRESH];
	UINT32 Shop_Expedition_Reset_Time_Num;//ÿ��ˢ�����ô���

	UINT32 Max_Num_Vip_Level;
	UINT32 Max_Num_User_Level;
	UINT32 Max_Num_Hero_Star;
	UINT32 Max_Num_Hero_Quality;

	UINT32 Arena_History_Rank_Prize_factor1;
	UINT32 Arena_History_Rank_Prize_factor2;
	UINT32 Arena_Fight_Time_Out;
	UINT32 Arena_Challenge_CD;
	UINT32 Arena_Challenge_CD_Reset_Consume_Diamond;
	//UINT32 Arena_Challenge_Num_Reset_Consume_Diamond;
	UINT32 Arena_Challenge_Num;
	//std::vector<stGameElement> Arena_Fight_Win_Prize;//������arena��

	UINT32 Health_Present_Factor_When_User_Level_Up;
	UINT32 Health_Limit;
	UINT32 Health_Initial;
	UINT32 Health_Grow;
private:

	CValue* _get_value(std::string& key){
		std::map<std::string, CValue*>::iterator it = m_values.find(key);
		if (it != m_values.end())
		{
			return it->second;
		}

		return NULL;
	}

	std::map<std::string, CValue*> m_values;
};

#endif // sys_setting_mgr_h__
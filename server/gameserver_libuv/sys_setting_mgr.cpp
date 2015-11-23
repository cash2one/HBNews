#include "sys_setting_mgr.h"
#include "db_mgr.h"
#include "str.h"
#include "condition.h"

bool CSysSettingMgr::Init()
{
	Global_Reset_Time = 0;

	User_Level_Function_Arena = -1;
	User_Level_Function_Shop_Common = -1;
	User_Level_Function_Shop_Qiwuge = -1;
	User_Level_Function_Shop_Yijiebaoku = -1;
	User_Level_Function_Shop_Arena = -1;
	User_Level_Function_Shop_Expedition = -1;
	User_Level_Function_Invoke = -1;
	User_Level_Function_Upgrade_Hero_Skill = -1;
	User_Level_Function_Expedition = -1;
	User_Level_Function_Trial_Tower = -1;
	User_Level_Function_Friend = -1;
	User_Level_Function_Hello_World = -1;
	User_Level_Function_Everyday_Mission = -1;
	User_Level_Function_Guild = -1;
	User_Level_Function_Achivement_Mission = -1;
	User_Level_Function_Mail = -1;
	User_Level_Function_CheckIn = -1;

	memset(Shop_Qiwuge_Open_Activity_time, 0 , sizeof Shop_Qiwuge_Open_Activity_time);
	memset(Shop_Yijiebaoku_Open_Activity_time, 0, sizeof Shop_Yijiebaoku_Open_Activity_time);
	Shop_Common_Reset_Time_Num = 0;
	Shop_Qiwuge_Reset_Time_Num = 0;
	Shop_Yijiebaoku_Reset_Time_Num = 0;
	Shop_Arena_Reset_Time_Num = 0;
	Shop_Expedition_Reset_Time_Num = 0;
	memset(Shop_Common_Reset_Time, 0, sizeof Shop_Common_Reset_Time);
	memset(Shop_Qiwuge_Reset_Time, 0, sizeof Shop_Qiwuge_Reset_Time);
	memset(Shop_Yijiebaoku_Reset_Time, 0, sizeof Shop_Yijiebaoku_Reset_Time);
	memset(Shop_Arena_Reset_Time, 0, sizeof Shop_Arena_Reset_Time);
	memset(Shop_Expedition_Reset_Time, 0, sizeof Shop_Expedition_Reset_Time);

	Max_Num_Vip_Level = 0;
	Max_Num_User_Level = 0;
	Max_Num_Hero_Star = 0;
	Max_Num_Hero_Quality = 0;

	Arena_History_Rank_Prize_factor1 = 0;
	Arena_History_Rank_Prize_factor2 = 0;
	Arena_Fight_Time_Out = 0;
	Arena_Challenge_CD = -1;//安全
	Arena_Challenge_CD_Reset_Consume_Diamond = -1;
	//Arena_Challenge_Num_Reset_Consume_Diamond = -1;
	Arena_Challenge_Num = 0;

	Health_Present_Factor_When_User_Level_Up = 0;
	Health_Limit = 0;
	Health_Initial = 0;
	Health_Grow = 0;

	MYSQL* con = CDBMgr::GetInstance()->GetConnection();

	//解析表！
	char* sql = "SELECT * FROM `sys_setting`";
	int code = CDBMgr::GetInstance()->Query(con, sql, (unsigned int)strlen(sql));
	if (0 != code)   //非0查询失败  
	{
		printf("query failed! [%s] [%d] [%s] \n", sql, mysql_errno(con), mysql_error(con));       
		return false;  
	}

	int i = 0;
	do
	{
		MYSQL_RES* res = mysql_store_result( con );
		switch (i++)
		{
		case 0:{//equip
				   {
					   //检查表结构是否有改变
					   string need_fields[] = {"key","value_type","value","desc"};
					   std::vector<std::string> v_need_fields(need_fields, need_fields + sizeof(need_fields)/sizeof(need_fields[0]));
					   ASSERT(CDBMgr::check_fields(res, v_need_fields));
				   }

				   MYSQL_ROW row;
				   while (row = mysql_fetch_row(res))
				   {
					   //printf("key[%s] value_type[%d] value[%s] desc[%s] \n", row[0], atoi(row[1]), row[2], row[3]);
					   //continue;

					   int col = 0;
					   std::string key = row[col++];
					   ASSERT(m_values.find(key) == m_values.end());

					   CValue::EValueType type =  (CValue::EValueType)atoi(row[col++]);
					   ASSERT(type > CValue::EValueType_MIN && type < CValue::EValueType_MAX);

					   CValue* pItem = new CValue;
					   if (type == CValue::EValueType_INT)
					   {
						   pItem->SetIntValue(atoi(row[col++]));
					   }
					   else if (type == CValue::EValueType_STRING)
					   {
						   pItem->SetStringValue(row[col++]);
					   }
					   else if (type == CValue::EValueType_FLOAT)
					   {
						   pItem->SetFloatValue(atof(row[col++]));
					   }
					   else{
						   ASSERT(false && "不支持的数据类型");
					   }

					   /*
					   {//evolution_input
					   char* pBuf = UTF8ToANSI(row[col++]);
					   std::vector<string> elements;
					   split_string(string(pBuf), elements, "|");
					   free(pBuf);
					   for (vector<string>::iterator it = elements.begin() ; it != elements.end() ; ++it)
					   {
					   vector<int> ele;
					   split_string_2i(*it, ele, "-");
					   ASSERT(ele.size() == 3);

					   stGameElement* pEle = new stGameElement((EELEMENTTYPE)ele[0], ele[1], NULL, ele[2]);
					   pItem->vEvolutionInput.push_back(pEle);
					   }
					   }
					   */

#define START_CHECK_AND_SET_SETTING_INT(name)  if (key == #name){\
					   ASSERT(pItem->value_type == CValue::EValueType_INT);\
					   name = pItem->int_value;\
					   }\

#define CHECK_AND_SET_SETTING_INT(name)  else if (key == #name){\
						ASSERT(pItem->value_type == CValue::EValueType_INT);\
						name = pItem->int_value;\
										   }\

					   m_values[key] = pItem;

					   /*
					   if(key == "Global_Reset_Time"){
						   ASSERT(pItem->value_type == CValue::EValueType_INT);
						   Global_Reset_Time = pItem->int_value;
					   }*/
					   START_CHECK_AND_SET_SETTING_INT(Global_Reset_Time)
						   CHECK_AND_SET_SETTING_INT(User_Level_Function_Arena)
						   CHECK_AND_SET_SETTING_INT(User_Level_Function_Shop_Common)
						   CHECK_AND_SET_SETTING_INT(User_Level_Function_Shop_Qiwuge)
						   CHECK_AND_SET_SETTING_INT(User_Level_Function_Shop_Yijiebaoku)
						   CHECK_AND_SET_SETTING_INT(User_Level_Function_Shop_Arena)
						   CHECK_AND_SET_SETTING_INT(User_Level_Function_Shop_Expedition)
						   CHECK_AND_SET_SETTING_INT(User_Level_Function_Invoke)
						   CHECK_AND_SET_SETTING_INT(User_Level_Function_Upgrade_Hero_Skill)
						   CHECK_AND_SET_SETTING_INT(User_Level_Function_Expedition)
						   CHECK_AND_SET_SETTING_INT(User_Level_Function_Trial_Tower)
						   CHECK_AND_SET_SETTING_INT(User_Level_Function_Friend)
						   CHECK_AND_SET_SETTING_INT(User_Level_Function_Hello_World)
						   CHECK_AND_SET_SETTING_INT(User_Level_Function_Everyday_Mission)
						   CHECK_AND_SET_SETTING_INT(User_Level_Function_Guild)
						   CHECK_AND_SET_SETTING_INT(User_Level_Function_Achivement_Mission)
						   CHECK_AND_SET_SETTING_INT(User_Level_Function_Mail)
						   CHECK_AND_SET_SETTING_INT(User_Level_Function_CheckIn)
					   else if (key == "Shop_Qiwuge_Open_Activity_time"){
						   ASSERT(pItem->value_type == CValue::EValueType_STRING);
						   char* pBuf = UTF8ToANSI(pItem->string_value);
						   bool r = ParseTimeCondition(pBuf, Shop_Qiwuge_Open_Activity_time);
						   ASSERT(r);
						   free(pBuf);
					   }
					   else if(key == "Shop_Yijiebaoku_Open_Activity_time"){
						   ASSERT(pItem->value_type == CValue::EValueType_STRING);
						   char* pBuf = UTF8ToANSI(pItem->string_value);
						   bool r = ParseTimeCondition(pBuf, Shop_Yijiebaoku_Open_Activity_time);
						   ASSERT(r);
						   free(pBuf);
					   }
					   else if (key == "Shop_Common_Reset_Time"){
						   ASSERT(pItem->value_type == CValue::EValueType_STRING);
						   char* pBuf = UTF8ToANSI(pItem->string_value);
						   bool r = split_string_to_int_array(std::string(pBuf), "|", Shop_Common_Reset_Time, Shop_Common_Reset_Time_Num, MAXNUM_SHOP_AUTO_REFRESH, true);
						   free(pBuf);
						   ASSERT(r);
					   }
					   else if (key == "Shop_Qiwuge_Reset_Time"){
						   ASSERT(pItem->value_type == CValue::EValueType_STRING);
						   char* pBuf = UTF8ToANSI(pItem->string_value);
						   bool r = split_string_to_int_array(std::string(pBuf), "|", Shop_Qiwuge_Reset_Time, Shop_Qiwuge_Reset_Time_Num, MAXNUM_SHOP_AUTO_REFRESH, true);
						   free(pBuf);
						   ASSERT(r);
					   }
					   else if (key == "Shop_Yijiebaoku_Reset_Time"){
						   ASSERT(pItem->value_type == CValue::EValueType_STRING);
						   char* pBuf = UTF8ToANSI(pItem->string_value);
						   bool r = split_string_to_int_array(std::string(pBuf), "|", Shop_Yijiebaoku_Reset_Time, Shop_Yijiebaoku_Reset_Time_Num, MAXNUM_SHOP_AUTO_REFRESH, true);
						   free(pBuf);
						   ASSERT(r);
					   }
					   else if (key == "Shop_Arena_Reset_Time"){
						   ASSERT(pItem->value_type == CValue::EValueType_STRING);
						   char* pBuf = UTF8ToANSI(pItem->string_value);
						   bool r = split_string_to_int_array(std::string(pBuf), "|", Shop_Arena_Reset_Time, Shop_Arena_Reset_Time_Num, MAXNUM_SHOP_AUTO_REFRESH, true);
						   free(pBuf);
						   ASSERT(r);
					   }
					   else if (key == "Shop_Expedition_Reset_Time"){
						   ASSERT(pItem->value_type == CValue::EValueType_STRING);
						   char* pBuf = UTF8ToANSI(pItem->string_value);
						   bool r = split_string_to_int_array(std::string(pBuf), "|", Shop_Expedition_Reset_Time, Shop_Expedition_Reset_Time_Num, MAXNUM_SHOP_AUTO_REFRESH, true);
						   free(pBuf);
						   ASSERT(r);
					   }
					   else if (key == "Max_Num_Vip_Level"){
						   ASSERT(pItem->value_type == CValue::EValueType_INT);
						   Max_Num_Vip_Level = pItem->int_value;
					   }
					   else if (key == "Max_Num_User_Level"){
						   ASSERT(pItem->value_type == CValue::EValueType_INT);
						   Max_Num_User_Level = pItem->int_value;
					   }
					   else if (key == "Max_Num_Hero_Star"){
						   ASSERT(pItem->value_type == CValue::EValueType_INT);
						   Max_Num_Hero_Star = pItem->int_value;
					   }
					   else if (key == "Max_Num_Hero_Quality"){
						   ASSERT(pItem->value_type == CValue::EValueType_INT);
						   Max_Num_Hero_Quality = pItem->int_value;
					   }
					   else if (key == "Arena_History_Rank_Prize_factor1"){
						   ASSERT(pItem->value_type == CValue::EValueType_INT);
						   Arena_History_Rank_Prize_factor1 = pItem->int_value;
					   }
					   else if (key == "Arena_History_Rank_Prize_factor2"){
						   ASSERT(pItem->value_type == CValue::EValueType_INT);
						   Arena_History_Rank_Prize_factor2 = pItem->int_value;
					   }
					   else if (key == "Arena_Fight_Time_Out"){
						   ASSERT(pItem->value_type == CValue::EValueType_INT);
						   Arena_Fight_Time_Out = pItem->int_value;
					   }
					   else if (key == "Arena_Challenge_CD"){
						   ASSERT(pItem->value_type == CValue::EValueType_INT);
						   Arena_Challenge_CD = pItem->int_value;
					   }
					   else if (key == "Arena_Challenge_CD_Reset_Consume_Diamond"){
						   ASSERT(pItem->value_type == CValue::EValueType_INT);
						   Arena_Challenge_CD_Reset_Consume_Diamond = pItem->int_value;
					   }
					   else if (key == "Arena_Challenge_Num"){
						   ASSERT(pItem->value_type == CValue::EValueType_INT);
						   Arena_Challenge_Num = pItem->int_value;
					   }
					   CHECK_AND_SET_SETTING_INT(Health_Present_Factor_When_User_Level_Up)
					   CHECK_AND_SET_SETTING_INT(Health_Limit)
					   CHECK_AND_SET_SETTING_INT(Health_Initial)
					   CHECK_AND_SET_SETTING_INT(Health_Grow)
						   
				   }
			   }
			   break;
		}
		mysql_free_result(res);
	}while( !mysql_next_result( con ) );

	return CheckSettingValid();
}
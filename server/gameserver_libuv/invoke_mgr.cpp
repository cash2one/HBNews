#include <string>
#include "invoke_mgr.h"
#include "db_mgr.h"
#include "db_sql.h"
#include "str.h"

#include "sys_setting_mgr.h"
#include "burst_mgr.h"

using std::string;

bool CInvokeMgr::Init()
{
	memset(m_invokeType2Cfg, 0, sizeof m_invokeType2Cfg);

	MYSQL* con = CDBMgr::GetInstance()->GetConnection();

	//解析表！
	char* sql = "SELECT * FROM `sys_invoke`";
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
					   string need_fields[] = {"invoke_type","packet_id","first_time_packet_id","activity_packet_id","activity_packet_time"};
					   std::vector<std::string> v_need_fields(need_fields, need_fields + sizeof(need_fields)/sizeof(need_fields[0]));
					   ASSERT(CDBMgr::check_fields(res, v_need_fields));
				   }

				   MYSQL_ROW row;
				   while (row = mysql_fetch_row(res))
				   {
					   EInvokeType type = (EInvokeType)atoi(row[0]);
					   ASSERT(type > EInvokeType_MIN && type < EInvokeType_MAX);
					   ASSERT(m_invokeType2Cfg[type] == NULL);

					   int col = 1;//忽略类型
					   stInvokeCfg* pItem = new stInvokeCfg;
					   memset(pItem, 0, sizeof stInvokeCfg);
					   pItem->iPacketID = atoi(row[col++]);
					   pItem->iFirstPacketID = atoi(row[col++]);
					   pItem->iActivityPacketID = atoi(row[col++]);

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

					   //解析进入条件配置！
					   if (pItem->iActivityPacketID){
						   char* pBuf = UTF8ToANSI(row[col++]);
						   bool r = ParseTimeCondition(pBuf, pItem->ActivityConditions);
						   ASSERT(r);
						   free(pBuf);
					   }

					   m_invokeType2Cfg[type] = pItem;
				   }

			   }
			   break;
		}
		mysql_free_result(res);
	}while( !mysql_next_result( con ) );

	return true;
}

bool CInvokeMgr::Invoke( EInvokeType invoke_type, UINT32 invoke_num, std::vector<stGameElement>& output, std::vector<stGameElement>& consume, bool isFirstTime /*= false*/ )
{
	stInvokeCfg* cfg = m_invokeType2Cfg[invoke_type];
	if ( cfg == NULL){
		ASSERT(false);
		return false;
	}

	//只有友情点可以多次
	if ( !invoke_num || (invoke_type != EInvokeType_FRIEND_POINT && invoke_num > 1)){
		ASSERT(false);
		return false;
	}

	bool r = false;

	/*
	EInvokeType_GOLD_FREE = 1,		// 免费金币单次抽取
	EInvokeType_GOLD_1 = 2,			//金币单次抽取（玩家消耗金币）
	EInvokeType_GOLD_FIRST_N = 3,	//金币十连抽首次抽取
	EInvokeType_GOLD_N = 4,			//金币十连抽普通抽取
	EInvokeType_DIAMOND_FREE = 5,	//普通免费钻石抽取
	EInvokeType_DIAMOND_1 = 6,		//普通钻石抽取（玩家消耗了钻石）
	EInvokeType_DIAMOND_FIRST_N = 7,	//钻石十连抽首次抽取
	EInvokeType_DIAMOND_N = 8,		//钻石十连抽普通抽取
	*/

	//先算消耗
	switch (invoke_type)
	{
	case EInvokeType_GOLD_FREE:
	case EInvokeType_DIAMOND_FREE:
		break;

	case EInvokeType_GOLD_1:{
		UINT32 value;
		r = CSysSettingMgr::GetInstance()->GetIntValue("Invoke_Gold_1", value);
		ASSERT(r);
		CHECK_FALSE_RETURN_FALSE(r);
		stGameElement ele(EELEMENTTYPE_GOLD, 0, 0, stGameElement::DECREASE, value);
		consume.push_back(ele);

	}break;

	case EInvokeType_GOLD_N:{
		UINT32 value;
		r = CSysSettingMgr::GetInstance()->GetIntValue("Invoke_Gold_N", value);
		ASSERT(r);
		CHECK_FALSE_RETURN_FALSE(r);
		stGameElement ele(EELEMENTTYPE_GOLD, 0, 0, stGameElement::DECREASE, value);
		consume.push_back(ele);
	}break;

	case EInvokeType_DIAMOND_1:{
		UINT32 value;
		r = CSysSettingMgr::GetInstance()->GetIntValue("Invoke_Diamond_1", value);
		ASSERT(r);
		CHECK_FALSE_RETURN_FALSE(r);
		stGameElement ele(EELEMENTTYPE_DIAMOND, 0, 0, stGameElement::DECREASE, value);
		consume.push_back(ele);
	}break;

	case EInvokeType_DIAMOND_N:{
		UINT32 value;
		r = CSysSettingMgr::GetInstance()->GetIntValue("Invoke_Diamond_N", value);
		ASSERT(r);
		CHECK_FALSE_RETURN_FALSE(r);
		stGameElement ele(EELEMENTTYPE_DIAMOND, 0, 0, stGameElement::DECREASE, value);
		consume.push_back(ele);
	}break;

	case EInvokeType_VIP_DIAMOND_1:{
		UINT32 value;
		r = CSysSettingMgr::GetInstance()->GetIntValue("Invoke_Vip_Diamond_1", value);
		ASSERT(r);
		CHECK_FALSE_RETURN_FALSE(r);
		stGameElement ele(EELEMENTTYPE_DIAMOND, 0, 0, stGameElement::DECREASE, value);
		consume.push_back(ele);
	}break;

	case EInvokeType_VIP_DIAMOND_N:{
		UINT32 value;
		r = CSysSettingMgr::GetInstance()->GetIntValue("Invoke_Vip_Diamond_N", value);
		ASSERT(r);
		CHECK_FALSE_RETURN_FALSE(r);
		stGameElement ele(EELEMENTTYPE_DIAMOND, 0, 0, stGameElement::DECREASE, value);
		consume.push_back(ele);
	 }break;

	case EInvokeType_FRIEND_POINT:{
		UINT32 value;
		r = CSysSettingMgr::GetInstance()->GetIntValue("Invoke_Friend_Point", value);
		ASSERT(r);
		CHECK_FALSE_RETURN_FALSE(r);
		stGameElement ele(EELEMENTTYPE_FRIEND_POINT, 0, 0, stGameElement::DECREASE, value * invoke_num);
		consume.push_back(ele);
	}break;

	default:
		ASSERT(false);
		return false;
	}

	UINT32 retPacketID = 0;
	if ( isFirstTime ){
		retPacketID = cfg->iFirstPacketID;
	}
	else{
		retPacketID = cfg->iPacketID;
	}

	//只有友情点可以循环多次  这里友情点不支持首抽不一样！
	for (UINT32 i = 0 ; i < invoke_num ; ++i){
		r = CBurstMgr::GetInstance()->GetBurst(retPacketID, output);
		ASSERT(r);
		CHECK_FALSE_RETURN_FALSE(r);

		if (cfg->iActivityPacketID && IsConditionsSatisfy(cfg->ActivityConditions, 0)){
			r = CBurstMgr::GetInstance()->GetBurst(cfg->iActivityPacketID, output);
			ASSERT(r);
			CHECK_FALSE_RETURN_FALSE(r);
		}
	}

	return r;
}
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

	//������
	char* sql = "SELECT * FROM `sys_invoke`";
	int code = CDBMgr::GetInstance()->Query(con, sql, (unsigned int)strlen(sql));
	if (0 != code)   //��0��ѯʧ��  
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
					   //����ṹ�Ƿ��иı�
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

					   int col = 1;//��������
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

					   //���������������ã�
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

	//ֻ���������Զ��
	if ( !invoke_num || (invoke_type != EInvokeType_FRIEND_POINT && invoke_num > 1)){
		ASSERT(false);
		return false;
	}

	bool r = false;

	/*
	EInvokeType_GOLD_FREE = 1,		// ��ѽ�ҵ��γ�ȡ
	EInvokeType_GOLD_1 = 2,			//��ҵ��γ�ȡ��������Ľ�ң�
	EInvokeType_GOLD_FIRST_N = 3,	//���ʮ�����״γ�ȡ
	EInvokeType_GOLD_N = 4,			//���ʮ������ͨ��ȡ
	EInvokeType_DIAMOND_FREE = 5,	//��ͨ�����ʯ��ȡ
	EInvokeType_DIAMOND_1 = 6,		//��ͨ��ʯ��ȡ�������������ʯ��
	EInvokeType_DIAMOND_FIRST_N = 7,	//��ʯʮ�����״γ�ȡ
	EInvokeType_DIAMOND_N = 8,		//��ʯʮ������ͨ��ȡ
	*/

	//��������
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

	//ֻ����������ѭ�����  ��������㲻֧���׳鲻һ����
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
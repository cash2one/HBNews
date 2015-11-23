#include "user_mgr.h"
#include "dbs_gs_proto.pb.h"
#include "sys_setting_mgr.h"
#include "str.h"

#include "db_mgr.h"
#include "db_sql.h"

#include "shop_mgr.h"
#include "vip_mgr.h"
#include "condition.h"
#include "hero_mgr.h"
#include "item_mgr.h"
bool CUserMgr::Init()
{
	const char* str = NULL;
	bool r = CSysSettingMgr::GetInstance()->GetStringValue("Initial_Player_Belongings", str);
	ASSERT(r && str);
	CHECK_FALSE_RETURN_FALSE(r && str);

	r = stGameElement::Parse(str, m_initPlayerBelongings, stGameElement::TOTAL);
	ASSERT(r);
	CHECK_FALSE_RETURN_FALSE(r);

	ASSERT(m_initPlayerBelongings.size() > 0);
	CHECK_FALSE_RETURN_FALSE(m_initPlayerBelongings.size() > 0 );

	//todo �����ж�Э���ö�ٺ��ֶ������ö���Ƿ�һ��

	MYSQL* con = CDBMgr::GetInstance()->GetConnection();

	//������
	char* sql = SELECT_USER_LEVEL;
	int code = CDBMgr::Query(con, sql, (unsigned int)strlen(sql));
	if (0 != code)   //��0��ѯʧ��  
	{
		printf("query failed! [%s]\n", sql);  
		return false;  
	}

	int i = 0;
	do
	{
		MYSQL_RES* res = mysql_store_result( con );
		switch (i++)
		{
		case 0:{//sys_user_level
				   {
					   //����ṹ�Ƿ��иı�
					   string need_fields[] = {"level","exp_for_next_level"};
					   std::vector<std::string> v_need_fields(need_fields, need_fields + sizeof(need_fields)/sizeof(need_fields[0]));
					   ASSERT(CDBMgr::check_fields(res, v_need_fields));
				   }

				   MYSQL_ROW row;
				   while (row = mysql_fetch_row(res))
				   {
					   int col = 0;//
					   stUserLevelConfig* pItem = new stUserLevelConfig;
					   pItem->iLevel = atoi(row[col++]);
					   pItem->iExpForNextLevel = atoi(row[col++]);
					  
					   ASSERT(m_mapLevel2UserCfg.find(pItem->iLevel) == m_mapLevel2UserCfg.end());
					   m_mapLevel2UserCfg[pItem->iLevel] = pItem;
				   }

				   ASSERT(!m_mapLevel2UserCfg.empty());
			   }
			   break;
		}
		mysql_free_result(res);
	}while( !mysql_next_result( con ) );

	return true;
}

std::string CUserMgr::GetPlantformAccount( UINT32 platform_id, UINT8 platform_sub_id, const char* raw_account )
{
	char buffer[255] = {0};
	sprintf_s(buffer, sizeof buffer, "%d_%d_%s", platform_id, platform_sub_id, raw_account);
	return std::string(buffer);
}

CUser* CUserMgr::GetUserByUserID( UINT32 iUserID )
{
	std::map<UINT32, CUser*>::iterator it = m_mapUserID2User.find(iUserID);
	if (it != m_mapUserID2User.end())
	{
		return it->second;
	}

	return NULL;
}

bool CUserMgr::AddUser( CUser* pUser )
{
	CHECK_NULL_RETURN_FALSE(pUser);
	CHECK_NULL_RETURN_FALSE(pUser->UserID());

	//�Ѿ�����
	if (m_mapUserID2User.end() != m_mapUserID2User.find(pUser->UserID()))
	{
		return false;
	}

	m_mapUserID2User[pUser->UserID()] = pUser;
	return true;
}

bool CUserMgr::RemoveUser( UINT32 user_id )
{
	std::map<UINT32, CUser*>::iterator it = m_mapUserID2User.find(user_id);
	if (it != m_mapUserID2User.end())
	{
		m_mapUserID2User.erase(it);
		return true;
	}
	else{
		return false;
	}
}

bool CUserMgr::InitNewUser( db_user_info* user_info, UINT32 user_id )
{
	ASSERT(user_info);

	UINT32 Initial_Player_Vip_Level = 0;
	bool r = CSysSettingMgr::GetInstance()->GetIntValue("Initial_Player_Vip_Level", Initial_Player_Vip_Level);
	ASSERT(r);

	UINT32 Initial_Player_Level = 0;
	r = CSysSettingMgr::GetInstance()->GetIntValue("Initial_Player_Level", Initial_Player_Level);
	ASSERT(r);

	UINT32 Invoke_Gold_Free_Per_Day = 0;
	r = CSysSettingMgr::GetInstance()->GetIntValue("Invoke_Gold_Free_Per_Day", Invoke_Gold_Free_Per_Day);
	ASSERT(r);

	UINT32 Invoke_Diamond_Free_Max_num = 0;
	r = CSysSettingMgr::GetInstance()->GetIntValue("Invoke_Diamond_Free_Max_num", Invoke_Diamond_Free_Max_num);
	ASSERT(r);

	//��������Ʋ�Initial_Player_Belongings ������user����֮��ͨ��update����

	db_user_base_info* base_info = user_info->mutable_user_base_info();
	UINT32 now = Now();
	//1.����������Ϣ
	base_info->set_user_id(user_id);
	base_info->set_name("");
	base_info->set_level(Initial_Player_Level);
	base_info->set_exp(0);
	base_info->set_vip(Initial_Player_Vip_Level);
	base_info->set_diamond(0);//Initial_Player_Belongings������
	base_info->set_gold(0);//Initial_Player_Belongings������
	base_info->set_health_cur(GetMaxHealth(base_info->level()));
	base_info->set_health_last_update_timestamp(now);
	base_info->set_skill_point(CVipMgr::GetInstance()->GetMaxSkillPoint(Initial_Player_Vip_Level));
	base_info->set_skill_point_last_update_timestamp(now);
	base_info->set_invoke_gold_free_left_num(Invoke_Gold_Free_Per_Day);
	base_info->set_invoke_gold_free_update_timestamp(0);//�����now ��������ҽ�����ֱ����ѽ���ٻ� ��cd��
	base_info->set_invoke_gold_n_total_num(0);
	base_info->set_invoke_diamond_free_left_num(Invoke_Diamond_Free_Max_num);
	base_info->set_invoke_diamond_free_update_timestamp(now);
	base_info->set_invoke_diamond_n_total_num(0);
	base_info->set_head_id(4);
	base_info->set_head_frame_id(901);
	base_info->set_friend_point(0);
	base_info->set_crusade_point(0);																																							
	//Ĭ��ǩ��
	const char* signature = NULL;
	r = CSysSettingMgr::GetInstance()->GetStringValue("Friend_Default_Signature", signature);
	ASSERT(r && signature);
	base_info->set_signature(signature);
	
	base_info->set_fight_power(0);

	//new_user_info->release_sub_tasks();//���������ݣ�=NULL Ȼ����db�Ǳ�mutable��ʱ�����


	//�����ʼ�Ƹ� �½���֮�� ��user�����ϵ�updateGameElement��������ȡ
	//r = pUser->GiveInitPlayerBelongings();
	//ASSERT(r);
	std::vector<stGameElement>& eles = CUserMgr::GetInstance()->GetInitPlayerBelongings();
	for(std::vector<stGameElement>::iterator it = eles.begin(); it != eles.end(); ++it){
		bool r = IsElementValid(*it, true);
		if (!r)
		{
			ASSERT(false);
			return false;
		}

		stGameElement stEle = *it;

PROCESS_ONE:
		ASSERT(stEle.eNumType == stGameElement::TOTAL);

		switch (stEle.eType)
		{
			case EELEMENTTYPE_GOLD:{
				base_info->set_gold(stEle.iNum);//Initial_Player_Belongings������
			}break;

			case EELEMENTTYPE_DIAMOND:{
				base_info->set_diamond(stEle.iNum);
			}break;
			case EELEMENTTYPE_ITEM:{
				//���汣֤������Ч �����ǳ����ѵ�����
				struct_game_element* ele = user_info->mutable_items()->add_items();
				ele->CopyFrom(stEle);
			}break;
			case EELEMENTTYPE_HERO:{
				
				bool has = false;
				::google::protobuf::RepeatedPtrField< ::db_user_hero >::iterator iter = user_info->mutable_heros()->mutable_items()->begin();
				for (; iter != user_info->mutable_heros()->mutable_items()->end(); ++iter)
				{
					if (stEle.iID == iter->hero_id())
					{
						has = true;
					}
				}

				if( has){//Ӣ���Ѿ�����

					CHeroCfg* hero_cfg = CHeroMgr::GetInstance()->GetHeroByHeroID(stEle.iID);
					ASSERT(hero_cfg);

					//��ȡת������Ƭ����
					stHeroStarCfg* need = CHeroMgr::GetInstance()->GetStarNeed(hero_cfg->iStar);
					ASSERT(need);

					CItemCfg* item_cfg = CItemMgr::GetInstance()->GetFragmentItemByHeroID(stEle.iID);
					ASSERT(item_cfg);

					struct_game_element fragment;
					//todo ��ƷID
					fragment.set_type(struct_game_element::EELEMENTTYPE_ITEM);
					fragment.set_id(item_cfg->m_iID);
					fragment.set_sub_id(1);
					fragment.set_num_type(struct_game_element::INCREASE);
					fragment.set_num(need->iToFragmentNum);
					stEle.CopyFrom(fragment);

					goto PROCESS_ONE;
				}
				else{
					bool r = CHeroMgr::GetInstance()->NewHero(stEle.iID, user_info->mutable_heros()->add_items(), NULL);
					ASSERT(r);
				}
				
			}break;
			default:
				ASSERT(false && "��֧�ֵĳ�ʼ�Ƹ�����");
				return false;
		}
	}

	//�����̵���߸��û�
	std::map<std::pair<EELEMENTTYPE, UINT32>, CShopItemWeightFactor*> emptyMapWeightFactor;//
	for (UINT32 iShopType = CShop::ESHOPTYPE_COMMON ; iShopType < CShop::ESHOPTYPE_MAX ; ++iShopType){
		CShop::ESHOPTYPE eShopType = (CShop::ESHOPTYPE)iShopType;
		switch (eShopType)
		{
		case CShop::ESHOPTYPE_COMMON:
			{
				//UINT32 Shop_Common_Switch;
				//bool r = CSysSettingMgr::GetInstance()->GetIntValue("Shop_Common_Switch", Shop_Common_Switch);
				//ASSERT(r);
				//if (Shop_Common_Switch == 0){
				//	return false;
				//}

				//�ӻ����п��ܹر� ���ǻ��ǻῪ�� ����������ҵ����ÿ����̵�
				CShop shop(eShopType);
				//shop.eShopType = eShopType;
				shop.iLastAutoRefreshTime = now;
				r = CShopMgr::GetInstance()->FillItemsToEmptyShop(CShop::ESHOPTYPE_COMMON, emptyMapWeightFactor, shop);
				ASSERT(r);

				shop.Fill(user_info->mutable_shops()->add_items(), NULL);
			}
			break;
		case CShop::ESHOPTYPE_QIWUGE:
			{
				UINT32 Shop_Qiwuge_Switch;
				bool r = CSysSettingMgr::GetInstance()->GetIntValue("Shop_Qiwuge_Switch", Shop_Qiwuge_Switch);
				ASSERT(r);

				if (Shop_Qiwuge_Switch == 0){
					break;//�˳�switch
				}

				//vip �ȼ�
				UINT32 Vip_Level_Function_Qiwuge;
				r = CSysSettingMgr::GetInstance()->GetIntValue("Vip_Level_Function_Qiwuge", Vip_Level_Function_Qiwuge);
				ASSERT(r);

				//vip�ȼ����� ��������Ӫ�ʱ����
				if (Initial_Player_Vip_Level >= Vip_Level_Function_Qiwuge || IsConditionsSatisfy(CSysSettingMgr::GetInstance()->Shop_Qiwuge_Open_Activity_time, user_id)){
					CShop shop(eShopType);
					//shop.eShopType = eShopType;
					shop.iLastAutoRefreshTime = now;
					r = CShopMgr::GetInstance()->FillItemsToEmptyShop(CShop::ESHOPTYPE_QIWUGE, emptyMapWeightFactor, shop);
					ASSERT(r);

					shop.Fill(user_info->mutable_shops()->add_items(), NULL);
				}

			}
			break;
		case CShop::ESHOPTYPE_YIJIEBAOKU:
			{
				UINT32 Shop_Yijiebaoku_Switch;
				bool r = CSysSettingMgr::GetInstance()->GetIntValue("Shop_Yijiebaoku_Switch", Shop_Yijiebaoku_Switch);
				ASSERT(r);

				if (Shop_Yijiebaoku_Switch == 0){
					break;//�˳�switch
				}

				//vip �ȼ�
				UINT32 Vip_Level_Function_Yijiebaoku;
				r = CSysSettingMgr::GetInstance()->GetIntValue("Vip_Level_Function_Yijiebaoku", Vip_Level_Function_Yijiebaoku);
				ASSERT(r);

				//vip�ȼ��㹻 ���� ����Ӫ�ʱ��
				if (Initial_Player_Vip_Level >= Vip_Level_Function_Yijiebaoku || IsConditionsSatisfy(CSysSettingMgr::GetInstance()->Shop_Yijiebaoku_Open_Activity_time, user_id)){
					CShop shop(eShopType);
					//shop.eShopType = eShopType;
					shop.iLastAutoRefreshTime = now;
					r = CShopMgr::GetInstance()->FillItemsToEmptyShop(CShop::ESHOPTYPE_YIJIEBAOKU, emptyMapWeightFactor, shop);
					ASSERT(r);

					shop.Fill(user_info->mutable_shops()->add_items(), NULL);
				}
			}
			break;
		case CShop::ESHOPTYPE_ARENA:
			{
				UINT32 Shop_Arena_Switch;
				bool r = CSysSettingMgr::GetInstance()->GetIntValue("Shop_Arena_Switch", Shop_Arena_Switch);
				ASSERT(r);

				if (Shop_Arena_Switch == 0){
					break;
				}

				//todo��
				printf("todo!!! �½������Ϣ ESHOPTYPE_ARENA �ݲ�֧�֣�\n");

			}
			break;
		case CShop::ESHOPTYPE_EXPEDITION:
			{
				UINT32 Shop_Expedition_Switch;
				bool r = CSysSettingMgr::GetInstance()->GetIntValue("Shop_Expedition_Switch", Shop_Expedition_Switch);
				ASSERT(r);

				if (Shop_Expedition_Switch == 0){
					break;
				}

				//todo��
				printf("todo!!!  �½������Ϣ  ESHOPTYPE_EXPEDITION �ݲ�֧�֣�\n");
			}
			break;
		}
	}

	return true;
}

UINT32 CUserMgr::IncreaseExp( const UINT32 cur_level, const UINT32 cur_exp, UINT32 add_exp, UINT32& up_level_num, UINT32& new_cur_exp )
{
	up_level_num = 0;
	new_cur_exp = cur_exp + add_exp;
	UINT32 temp_cur_exp = cur_exp;
	UINT32 temp_cur_level = cur_level;
	std::map<UINT32, stUserLevelConfig*>::iterator it = m_mapLevel2UserCfg.find(temp_cur_level);
	ASSERT(it != m_mapLevel2UserCfg.end() && it->second && cur_exp <= it->second->iExpForNextLevel );// cur_exp <= it->second->iExpForNextLevel ����ʱ���ܾ�Ϊ����
	stUserLevelConfig* cfg = NULL;
	UINT32 actual_add_exp = 0;
	while (it != m_mapLevel2UserCfg.end())
	{
		cfg = it->second;
		if (!cfg || cfg->iExpForNextLevel == 0){//û������ȼ������� ���� �Ѿ���������ã�==0��
			break;
		}

		if (new_cur_exp < cfg->iExpForNextLevel){
			break;
		}

		++up_level_num;
		new_cur_exp -= cfg->iExpForNextLevel;
		if ( temp_cur_exp != 0)
		{
			actual_add_exp += cfg->iExpForNextLevel - temp_cur_exp;
			temp_cur_exp = 0;
		}
		else{
			actual_add_exp += cfg->iExpForNextLevel;
		}
		
		it = m_mapLevel2UserCfg.find(++temp_cur_level);
	}

	ASSERT(cfg);
	UINT32 actual_cur_exp = min(new_cur_exp, cfg->iExpForNextLevel);//������

	//actual_add_exp  =  ������ǰ��һ�����ģ�����-��ǰ���飩 + �������� + ʣ���
	//���û����  ��ô���� left - cur_exp �õ����ӵľ���ֵ
	//���������  ��ôtemp_cur_exp���Ѿ�Ϊ0��  ���ӵľ���ֵ = �������� + ʣ���actual_cur_exp����ֵ
	//��û���� �� ������ ���и�������� ���ǵ�ǰ�Ѿ������ˣ� û����ʱ actual_cur_exp = cfg->iExpForNextLevel = 0 ;  ����ʱ ��
	ASSERT(actual_cur_exp >= temp_cur_exp);
	actual_add_exp += actual_cur_exp - temp_cur_exp;

	new_cur_exp = actual_cur_exp;

	return actual_add_exp;
}

UINT32 CUserMgr::GetMaxHealth( UINT32 level )
{
	//60+����ҵȼ�-1��*1
	return CSysSettingMgr::GetInstance()->Health_Initial + (level - 1) * CSysSettingMgr::GetInstance()->Health_Grow;
	////todo ��ȡ���ã�
	//return 100;

	//std::map<UINT32, stUserLevelConfig*>::iterator it = m_mapLevel2UserCfg.find(level);
	//ASSERT(it != m_mapLevel2UserCfg.end());
}

UINT32 CUserMgr::GetMaxHealthPro()
{
	//return 500;
	return CSysSettingMgr::GetInstance()->Health_Limit;
}

UINT32 CUserMgr::GetHealthPrizeOfLevelUp(UINT32 from, UINT32 to){
	ASSERT( to > from && from > 0);
	return (from + to-1)*(to-from)* CSysSettingMgr::GetInstance()->Health_Present_Factor_When_User_Level_Up/2;
}

CUser* CUserMgr::GetUserByName(const std::string& name )
{
	std::map<UINT32, CUser*>::iterator it = m_mapUserID2User.begin();
	for (; it != m_mapUserID2User.end(); ++it)
	{
		if (name == it->second->Name())
		{
			return it->second;
		}
	}
	return NULL;
}
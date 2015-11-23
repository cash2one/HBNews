#ifndef vip_h__
#define vip_h__
#include <map>
#include "def.h"
#include "config.h"
#include "sys_setting_mgr.h"
class CVipCfg{
public:
	UINT32 m_vip_level;
	UINT32 m_rmb;	//�ﵽ�˵ȼ���Ҫ�ﵽ���ۼƳ�ֵ���
	UINT32 m_buy_num_health;		//�ɹ�����������
	UINT32 m_buy_num_instance;		//���ƹ������ɣ�֧�ߣ����������
	UINT32 m_buy_num_arena;			//���ƹ��򾺼�����ս����
	UINT32 m_buy_num_gold;			//���ƿɵ�����
	UINT32 m_buy_skill_point;		//���ƹ����ܵ��������
	UINT32 m_max_num_skill_point;	//���ÿ�λظ����ܵ�
	UINT32 m_max_num_exp_battle;	//ÿ�վ�����������
	UINT32 m_max_num_gold_battle;	//ÿ�ս�һ��������
	UINT32 m_max_num_tower;			//ÿ����������
	UINT32 m_max_num_crusade;		//ÿ��Զ������
};

//class CVipBuyCfg{
//public:
//	UINT32 m_buy_type;				//����Ķ�����1.���� 2.��� 3.���ܵ�
//	UINT32 m_no_less_than_buy_num;	//>=�˹��������ʱ��
//	UINT32 m_diamond;				//>=no_less_than_buy_num���������ʱ����Ҫ���ѵ���ʯ��
//};

enum EBuy_Type{
	EBUY_MIN,
	EBUY_HEALTH = 1,			//����
	EBUY_GOLD = 2,				//���
	EBUY_SKILL = 3,				//���ܵ�
	EBUY_BRANCHTASK_RESET = 4,	//EBUY_SUB_TASK_ENTER_NUM ������������ //���ɣ�֧�ߣ���������ս����
	EBUY_SHOP_REFRESH = 5,		//�̵�ˢ��
	EBUY_ARENA_CHALLENGE_NUM = 6,//��������ս����
	EBUY_CRUSUDE_REFRESH_BUFF = 7,
	EBUY_CRUSUDE_HERO_LIFE = 8,			 //Ӣ�۸���(Զ��)
	EBUY_MAX,
};

enum EVip_Level_Function{
	EVip_Level_Function_Min = 0,
	EVip_Level_Function_Speed_Kill_N = 1,//>1ɨ��������vip�����ȼ�
	EVip_Level_Function_Max = 2
};

class CVipMgr{
public:
	
	bool VipLevelSatisfy(EVip_Level_Function eVipFun, UINT32 iVipLevel){
		ASSERT(iVipLevel <= CSysSettingMgr::GetInstance()->Max_Num_Vip_Level);
		ASSERT(eVipFun > EVip_Level_Function_Min && eVipFun < EVip_Level_Function_Max);
		
		switch(eVipFun)
		{
		case EVip_Level_Function_Speed_Kill_N:
			{
				UINT32 iCfgVip = 0;
				ASSERT(CSysSettingMgr::GetInstance()->GetIntValue("Vip_Level_Function_Speed_Kill_N", iCfgVip));
				return iVipLevel >= iCfgVip;
			}
			break;
		default:
			ASSERT(false && "��֧�ֵ�����");
			return false;
			break;
		}
	}

	//todo ��������
	inline static CVipMgr* GetInstance(){
		static CVipMgr singleton;
		return &singleton;
	}

	bool Init();

	bool UnInit();
	
	//����vip�ȼ���ȡvip��Ϣ
	CVipCfg* GetVipCfgByVipLevel(UINT32 iVipLevel);
	
	UINT32 GetMaxSkillPoint(UINT32 iVipLevel);

	//���ݹ������ͺʹ�����ȡ���ѵ���ʯ��
	UINT32 GetCostDiamond(EBuy_Type eBuyType, UINT32 iTimes);
private:
	std::map<UINT32, CVipCfg*> m_mapVipLevel2VipCfg;
	//std::map<EBuy_Type, std::map<UINT32, UINT32>>	m_mapBuyType2Cfg;//first:buy_type second:(first:num second:diamond)
	std::map<EBuy_Type, std::vector<std::pair<UINT32, UINT32>>> m_mapBuyType2Cfg;// first:buy_type second: list(first:num second:diamond) ��С��������
};

#endif
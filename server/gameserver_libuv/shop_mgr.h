#ifndef shop_mgr_h__
#define shop_mgr_h__

#include <map>
#include <vector>
#include <string>

#include "def.h"
#include "config.h"
#include "time_helper.h"
#include "dbs_gs_proto.pb.h"

struct stShopItem{

	enum ESHOPITEMFLAG{
		ESHOPITEMFLAG_NONE = 0,
		ESHOPITEMFLAG_HOT = 1,
		ESHOPITEMFLAG_DISCOUNT = 2, 
		ESHOPITEMFLAG_FLASHSALE = 3,
		ESHOPITEMFLAG_MAX = 4
	};

	UINT32	iPos;			//λ�� ��user_id shop
	EELEMENTTYPE eType;		//
	UINT32 iID;
	UINT32 iNum;//����
	EELEMENTTYPE eConsumeType;//��������Ҫ�����ĵ����� ������ ��ʯ
	UINT32 iConsumeID;///�������id
	UINT32 iConsumeNum;//�����������
	ESHOPITEMFLAG eFlag;
	bool bIsSold;//�Ƿ�����

	stShopItem(){
		memset(this, 0, sizeof stShopItem);
	}

	stShopItem(const db_user_shop_item& shop_item){
		iPos = shop_item.pos();
		eType = (EELEMENTTYPE)shop_item.element_type();
		iID = shop_item.element_id();
		iNum = shop_item.element_num();
		eConsumeType = (EELEMENTTYPE)shop_item.consume_type();
		iConsumeID = shop_item.consume_id();
		iConsumeNum = shop_item.consume_num();
		eFlag = (ESHOPITEMFLAG)shop_item.flag();
		bIsSold = shop_item.is_sold();
	}

	operator db_user_shop_item(){
		db_user_shop_item shop_item;
		shop_item.set_pos(iPos);
		shop_item.set_element_type((struct_game_element::EELEMENTTYPE)eType);
		shop_item.set_element_id(iID);
		shop_item.set_element_num(iNum);
		shop_item.set_consume_type((struct_game_element::EELEMENTTYPE)eConsumeType);
		shop_item.set_consume_id(iConsumeID);
		shop_item.set_consume_num(iConsumeNum);
		shop_item.set_flag((user_shop_item::ESHOPITEMFLAG)eFlag);
		shop_item.set_is_sold(bIsSold);

		return shop_item;
	}


	void Fill(db_user_shop_item *db_item, user_shop_item *c_item){
		ASSERT(db_item || c_item);
		if (db_item){
			db_item->set_pos(iPos);
			db_item->set_element_type((struct_game_element::EELEMENTTYPE)eType);
			db_item->set_element_id(iID);
			db_item->set_element_num(iNum);
			db_item->set_consume_type((struct_game_element::EELEMENTTYPE)eConsumeType);
			db_item->set_consume_id(iConsumeID);
			db_item->set_consume_num(iConsumeNum);
			db_item->set_flag((user_shop_item::ESHOPITEMFLAG)eFlag);
			db_item->set_is_sold(bIsSold);
		}
		
		if(c_item){
			c_item->set_pos(iPos);
			c_item->set_element_type((struct_game_element::EELEMENTTYPE)eType);
			c_item->set_element_id(iID);
			c_item->set_element_num(iNum);
			c_item->set_consume_type((struct_game_element::EELEMENTTYPE)eConsumeType);
			c_item->set_consume_id(iConsumeID);
			c_item->set_consume_num(iConsumeNum);
			c_item->set_flag((user_shop_item::ESHOPITEMFLAG)eFlag);
			c_item->set_is_sold(bIsSold);
		}
	
	}
};


class CShop{
public:
	enum ESHOPTYPE{
		ESHOPTYPE_MIN = 0,
		ESHOPTYPE_COMMON = 1,			//��ͨ�̵� �ӻ���
		ESHOPTYPE_QIWUGE = 2,			//�����
		ESHOPTYPE_YIJIEBAOKU = 3,		//��籦��
		ESHOPTYPE_ARENA = 4,			//�������̵�
		ESHOPTYPE_EXPEDITION = 5,		//Զ���̵�
		ESHOPTYPE_MAX = 6
	};

	CShop(ESHOPTYPE shop_type){
		//eShopType = ESHOPTYPE_MIN;
		eShopType = shop_type;
		iLastAutoRefreshTime = 0;
		iLastManualRefreshTime = 0;
		iLastDayManualRefreshNum = 0;
		iRandomOpenStartTimestamp = 0;
		iRandomOpenEndTimestamp = 0;

		memset(m_page, 0, sizeof m_page);
		iNextEmptyPos = 0;
	}

	~CShop(){
		eShopType = ESHOPTYPE_MIN;//�����������
		Clear();
	}

	bool Append(stShopItem* newItem){
		ASSERT(iNextEmptyPos < MAXNUM_SHOP_ITEM_IN_SHOP);
		if (iNextEmptyPos >= MAXNUM_SHOP_ITEM_IN_SHOP){
			return false;
		}

		m_page[iNextEmptyPos++] = newItem;
		return true;
	}

	void ClearItems(){
		for (UINT32 i = 0 ; i < MAXNUM_SHOP_ITEM_IN_SHOP ; ++i){
			stShopItem* pItem = m_page[i];
			if (pItem){
				delete pItem;
			}
		}

		memset(m_page, 0, sizeof m_page);
		iNextEmptyPos = 0;
	}

	UINT32 Num(){
		return iNextEmptyPos;
	}

	//bool InsertAt(UINT32 iPos, stShopItem* newItem){
	//	ASSERT(iPos < MAXNUM_SHOP_ITEM_IN_SHOP && !m_page[iPos]);
	//	m_page[iPos] = newItem;
	//};

	stShopItem* Get(UINT32 iPos){
		ASSERT(iPos < MAXNUM_SHOP_ITEM_IN_SHOP);
		return m_page[iPos];
	}

	//�����Ҫ�Զ�ˢ�� ��new_stamp ����ֵΪ�µ�ˢ�µ�
	bool IsNeedAutoRefresh(UINT32 &new_stamp);

	void Fill(db_user_shop *db_shop, user_shop *c_shop){
		ASSERT(db_shop || c_shop);
		if (db_shop){
			db_shop->set_shop_type((user_shop::ESHOPTYPE)eShopType);
			db_shop->set_last_auto_refresh_time(iLastAutoRefreshTime);
			db_shop->set_last_manual_refresh_time(iLastManualRefreshTime);
			db_shop->set_last_day_manual_refresh_num(GetLastDayManualRefreshNum());
			db_shop->set_random_open_start_timestamp(iRandomOpenStartTimestamp);
			db_shop->set_random_open_end_timestamp(iRandomOpenEndTimestamp);
		}

		if (c_shop){
			c_shop->set_shop_type((user_shop::ESHOPTYPE)eShopType);
			c_shop->set_last_auto_refresh_time(iLastAutoRefreshTime);
			c_shop->set_last_manual_refresh_time(iLastManualRefreshTime);
			c_shop->set_last_day_manual_refresh_num(GetLastDayManualRefreshNum());
			c_shop->set_random_open_start_timestamp(iRandomOpenStartTimestamp);
			c_shop->set_random_open_end_timestamp(iRandomOpenEndTimestamp);
		}
		
		for (UINT32 i = 0 ; i < Num() ; ++i){
			stShopItem* pItem = Get(i);
			ASSERT(pItem);
			if (pItem){

				db_user_shop_item* db_item = NULL;
				user_shop_item* c_item = NULL;
				if (db_shop){
					db_item = db_shop->add_items();
				}

				if (c_shop){
					c_item = c_shop->add_items();
				}

				pItem->Fill(db_item, c_item);
			}
		}
	}

	//UINT32 iUserID	;//����Ҫ���ֶ�

	UINT32 IncreaseLastDayManualRefreshNum();

	UINT32 GetLastDayManualRefreshNum();

	//����̵�������Ϣ
	ESHOPTYPE eShopType;				//�̵�����
	UINT32 iLastAutoRefreshTime;		//��һ�ε�ˢ��ʱ�� �費��Ҫ�����ֶ�ˢ�µ�ʱ�䣿 ����Ҫ������ ����ֶ�ˢ�²���ʱ �������Ҫ�Զ�ˢ��һ�� �����Զ�ˢ��һ�� ����ֻ����ʱ�� �����µ��� Ȼ�����ֶ����µ��ߣ�
	UINT32 iLastManualRefreshTime;		//��һ�ε��ֶ�ˢ��ʱ��
	UINT32 iLastDayManualRefreshNum;	//���ʱ�䵱���ֶ�ˢ�µĴ��� 
	//UINT32 iTotalManualRefreshNum;	//�ܹ����ֶ�ˢ�´��� ���ݿ����� ��Ϸ�߼�����Ҫ�����
	
	//��vip�ȼ��ﵽʱ һ�����ʿ���һ��ʱ����̵�
	//bool IsRandomedOpen; //�Ƿ���Ϊ���������
	UINT32 iRandomOpenStartTimestamp;	//��������̵�ʱ �̵꿪�ŵ���ʼʱ��
	UINT32 iRandomOpenEndTimestamp;	//��������̵�ʱ �̵꿪�ŵĽ���ʱ��

private:
	void Clear(){
		//eShopType = ESHOPTYPE_MIN;//�����������
		iLastAutoRefreshTime = 0;
		iLastManualRefreshTime = 0;
		iLastDayManualRefreshNum = 0;
		iRandomOpenStartTimestamp = 0;
		iRandomOpenEndTimestamp = 0;

		for (UINT32 i = 0 ; i < MAXNUM_SHOP_ITEM_IN_SHOP ; ++i)
		{
			stShopItem* pItem = m_page[i];
			if (pItem)
			{
				delete pItem;
			}
		}

		memset(m_page, 0, sizeof m_page);
		iNextEmptyPos = 0;
	}

	stShopItem* m_page[MAXNUM_SHOP_ITEM_IN_SHOP];	//�̵��б�

	//��������Ϣ����������Ҫ���ֶ�
	UINT32 iNextEmptyPos; //�̵���һ�����ÿ�λ��
};
//Ȩ����Ϣ ��Ʒ����Ȩ��=����Ȩ�ز���1*���Ƿ�Ϊ���ȱʧ��Ʒ��Ӣ����Ƭ�޴˲�����+����Ȩ�ز���2*�����ѱ��������
class  CShopItemWeightFactor
{
public:
	CShopItemWeightFactor(UINT32 iBuyNumToday, UINT32 iBuyLastTimestamp, bool bIsUserHeroEquipEvolutionNeed):
	m_iBuyNumToday(iBuyNumToday), m_iBuyLastTimestamp(iBuyLastTimestamp), m_bIsUserHeroEquipEvolutionNeed(bIsUserHeroEquipEvolutionNeed)
	{

	}

	UINT32 GetBuyNumToday();

	UINT32 GetLastBuyTime(){
		return m_iBuyLastTimestamp;
	}

	UINT32 AddBuyNumToday(UINT32 iBuyNum);

	bool GetIsUserHeroEquipEvolutionNeed(){
		return m_bIsUserHeroEquipEvolutionNeed;
	}

	void SetIsUserHeroEquipEvolutionNeed( bool isNeed){
		m_bIsUserHeroEquipEvolutionNeed = isNeed;
	}

private:
	//������Ϣʡ��
	UINT32 m_iBuyNumToday;//�����Ѿ��������
	UINT32 m_iBuyLastTimestamp;//������ʱ��
	bool m_bIsUserHeroEquipEvolutionNeed;//�Ƿ���Ӣ��װ����Ҫ��
};

class CShopMgr{	
public:
	//todo ��������
	inline static CShopMgr* GetInstance(){
		static CShopMgr singleton;
		return &singleton;
	}

	bool Init();

	bool UnInit();

	struct CShopPacket;
	CShopPacket* GetPacketByPacketID(UINT32 iPacketID);

	bool FillItemsToEmptyShop(CShop::ESHOPTYPE eShopType, const std::map<std::pair<EELEMENTTYPE, UINT32>, CShopItemWeightFactor*>& mapWeightFactor, CShop &shop);

	//UINT32 GetManualRefreshDiamond(UINT32 iManualRefreshNum);
private:
	struct stShopPacketElementFactor{
		//��ǩ��0=����ʾ��1=������2=���ۣ�3=��ʱ������
		
		UINT32 iNum;//����
		UINT32 iProbality;//���ָ��� ����֮�͵���MAXPROBITY_BURST �ڳ�ʼ�����õ�ʱ��������֤
		EELEMENTTYPE eConsumeType;//��������Ҫ�����ĵ����� ������ ��ʯ
		UINT32 iConsumeID;///�������id
		UINT32 iConsumeNum;//�����������
		stShopItem::ESHOPITEMFLAG eFlag;
	};

	struct CShopPacketElement{
		bool Get(CShop &ret);

		EELEMENTTYPE eType;
		UINT32 iID;
		std::vector<stShopPacketElementFactor*> vFactors;
	};

	struct CShopPacket{
		UINT32 iShopPacketID;
		std::vector<CShopPacketElement*> vPacketElements;

		//Nѡ1  part�Ѿ���burst_num������ ���part����������
		CShopPacketElement* Get(const std::map<std::pair<EELEMENTTYPE, UINT32>, CShopItemWeightFactor*>& mapWeightFactor);
		//cacheȨ�صĻ� ��Ҫ�������µģ�û��Ҫ
	};

	struct CShopPart{
		//UINT32 iShopPartID;//
		//UINT32 iShopID;//�����̵�
		UINT32 iBurstNum;//���䷽ʽΪ vShopPacketIDs.size * N + X = iBurstNum  NΪȫ��һ��  XΪʣ�µ������
		std::vector<UINT32> vShopPacketIDs;//ӵ�е��̵��
	};

	struct CShopCfg{
		/*
			��ȡ�̵����

			type-id ӳ�� Ȩ�ؼ�����Ҫ����Ϣ
		*/
		bool Get( const std::map<std::pair<EELEMENTTYPE, UINT32>, CShopItemWeightFactor*>& mapWeightFactor, CShop &ret);

		CShop::ESHOPTYPE eShopType;//�̵�ID
		std::string strName;//
		UINT32 iActivityShopPartID;//�̵�Ļ����
		UINT32 iActivityTimeType;//�ʱ����������
		std::string activity_time;//�ʱ������
		std::vector<CShopPart*>	vShopParts;//�ǻ���̵겿��
	};

	struct stShopRefreshCfg {
		UINT32 iRefreshNum;	//<= �������ʱ
		UINT32 iDiamond;	//��Ҫ���ѵ���ʯ��
	};

	std::map<UINT32, CShopPacket*> m_MapShopPacketID2ShopPacket;//
	//std::map<UINT32, std::vector<CShopPart*>> m_MapShopID2ShopParts;//
	std::map<UINT32, CShopPart*> m_MapShopPartID2ShopPart;
	std::map<CShop::ESHOPTYPE, CShopCfg*> m_MapShopType2ShopCfg;//
	//std::vector<stShopRefreshCfg*> m_VShopRefreshCfg;	//�̵�ˢ����������  ��Ҫ�ӵ͵������򣡣� �Ѿ�������sys_vip_buy����
};

#endif // shop_mgr_h__
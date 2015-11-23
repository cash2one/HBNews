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

	UINT32	iPos;			//位置 用user_id shop
	EELEMENTTYPE eType;		//
	UINT32 iID;
	UINT32 iNum;//数量
	EELEMENTTYPE eConsumeType;//购买所需要的消耗的类型 比如金币 钻石
	UINT32 iConsumeID;///消耗物的id
	UINT32 iConsumeNum;//消耗物的数量
	ESHOPITEMFLAG eFlag;
	bool bIsSold;//是否已售

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
		ESHOPTYPE_COMMON = 1,			//普通商店 杂货店
		ESHOPTYPE_QIWUGE = 2,			//奇物阁
		ESHOPTYPE_YIJIEBAOKU = 3,		//异界宝库
		ESHOPTYPE_ARENA = 4,			//竞技场商店
		ESHOPTYPE_EXPEDITION = 5,		//远征商店
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
		eShopType = ESHOPTYPE_MIN;//这个不能清理！
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

	//如果需要自动刷新 则new_stamp 被赋值为新的刷新点
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

	//UINT32 iUserID	;//不需要此字段

	UINT32 IncreaseLastDayManualRefreshNum();

	UINT32 GetLastDayManualRefreshNum();

	//玩家商店配置信息
	ESHOPTYPE eShopType;				//商店类型
	UINT32 iLastAutoRefreshTime;		//上一次的刷新时间 需不需要包括手动刷新的时间？ 不需要！！！ 如果手动刷新操作时 如果发现要自动刷新一下 就先自动刷新一下 但是只更新时间 不更新道具 然后再手动更新道具！
	UINT32 iLastManualRefreshTime;		//上一次的手动刷新时间
	UINT32 iLastDayManualRefreshNum;	//最后时间当日手动刷新的次数 
	//UINT32 iTotalManualRefreshNum;	//总共的手动刷新次数 数据库中有 游戏逻辑不需要这个！
	
	//非vip等级达到时 一定几率开启一段时间的商店
	//bool IsRandomedOpen; //是否因为随机而打开了
	UINT32 iRandomOpenStartTimestamp;	//随机开启商店时 商店开放的起始时间
	UINT32 iRandomOpenEndTimestamp;	//随机开启商店时 商店开放的结束时间

private:
	void Clear(){
		//eShopType = ESHOPTYPE_MIN;//这个不能清理！
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

	stShopItem* m_page[MAXNUM_SHOP_ITEM_IN_SHOP];	//商店列表

	//非配置信息，功能所需要的字段
	UINT32 iNextEmptyPos; //商店下一个可用空位置
};
//权重信息 物品额外权重=额外权重参数1*（是否为玩家缺失物品，英雄碎片无此参数）+额外权重参数2*今日已被购买次数
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
	//道具信息省略
	UINT32 m_iBuyNumToday;//今天已经买的数量
	UINT32 m_iBuyLastTimestamp;//最后购买的时间
	bool m_bIsUserHeroEquipEvolutionNeed;//是否是英雄装备需要的
};

class CShopMgr{	
public:
	//todo 单例工具
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
		//标签（0=不显示，1=热卖，2=打折，3=限时抢购）
		
		UINT32 iNum;//数量
		UINT32 iProbality;//出现概率 概率之和等于MAXPROBITY_BURST 在初始化配置的时候作过验证
		EELEMENTTYPE eConsumeType;//购买所需要的消耗的类型 比如金币 钻石
		UINT32 iConsumeID;///消耗物的id
		UINT32 iConsumeNum;//消耗物的数量
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

		//N选1  part已经用burst_num来控制 这个part出几个东西
		CShopPacketElement* Get(const std::map<std::pair<EELEMENTTYPE, UINT32>, CShopItemWeightFactor*>& mapWeightFactor);
		//cache权重的话 则要经常更新的！没必要
	};

	struct CShopPart{
		//UINT32 iShopPartID;//
		//UINT32 iShopID;//所属商店
		UINT32 iBurstNum;//爆落方式为 vShopPacketIDs.size * N + X = iBurstNum  N为全爆一次  X为剩下的随机爆
		std::vector<UINT32> vShopPacketIDs;//拥有的商店包
	};

	struct CShopCfg{
		/*
			获取商店道具

			type-id 映射 权重计算需要的信息
		*/
		bool Get( const std::map<std::pair<EELEMENTTYPE, UINT32>, CShopItemWeightFactor*>& mapWeightFactor, CShop &ret);

		CShop::ESHOPTYPE eShopType;//商店ID
		std::string strName;//
		UINT32 iActivityShopPartID;//商店的活动部分
		UINT32 iActivityTimeType;//活动时间限制类型
		std::string activity_time;//活动时间配置
		std::vector<CShopPart*>	vShopParts;//非活动的商店部分
	};

	struct stShopRefreshCfg {
		UINT32 iRefreshNum;	//<= 这个次数时
		UINT32 iDiamond;	//需要花费的钻石数
	};

	std::map<UINT32, CShopPacket*> m_MapShopPacketID2ShopPacket;//
	//std::map<UINT32, std::vector<CShopPart*>> m_MapShopID2ShopParts;//
	std::map<UINT32, CShopPart*> m_MapShopPartID2ShopPart;
	std::map<CShop::ESHOPTYPE, CShopCfg*> m_MapShopType2ShopCfg;//
	//std::vector<stShopRefreshCfg*> m_VShopRefreshCfg;	//商店刷新消耗配置  需要从低到高排序！！ 已经配置在sys_vip_buy里面
};

#endif // shop_mgr_h__
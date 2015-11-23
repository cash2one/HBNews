#ifndef __BURST_H__
#define __BURST_H__

#pragma once;

#include <vector>
#include <map>

#include "def.h"
using namespace std;

struct CBurstItem{

	CBurstItem(UINT32 iItemID, UINT32 iNum) : m_iItemID(iItemID), m_iNum(iNum){

	}
	UINT32 m_iItemID;
	UINT32 m_iNum;
};

struct CBurstItemConfig{

	/*
	CBurstItem(UINT32 iProbity, UINT32 itemID, UINT32 num) : iItemID(itemID), iNum(num){

	}
	*/

	UINT32 m_iProbity;//
	UINT32 m_iItemID;
	UINT32 m_iNum;
};

class IBurstPlan{
public:
	virtual bool GetBurst(vector<CBurstItem> & ret) = 0;
};

//������汬��һ��
class CBurstPlan_OnlyOne : public IBurstPlan{
public:
	CBurstPlan_OnlyOne(UINT32 iBurstPlanID) : m_bHasCachedTotalProbity(false), m_iCacheTotalProbity(0){
		
	}

	virtual bool GetBurst(vector<CBurstItem> & ret);

private:
	UINT32 m_iBurstPlanID;
	vector<CBurstItemConfig> m_vecBurstItems;

	bool m_bHasCachedTotalProbity;//�Ƿ񻺴������еĸ�����
	UINT32 m_iCacheTotalProbity;//�������еĸ�����
};

//ÿһ�������ܻᱬ��
class CBurstPlan_EachOne : public IBurstPlan{
public:
	CBurstPlan_EachOne(UINT32 iBurstPlanID) : m_iBurstPlanID(iBurstPlanID){

	}

	virtual bool GetBurst(vector<CBurstItem> & ret);

private:
	UINT32 m_iBurstPlanID;
	vector<CBurstItemConfig> m_vecBurstItems;
};

class CBurstMgr{
	//todo ��������
	inline static CBurstMgr* GetInstance(){
		static CBurstMgr singleton;
		return &singleton;
	}

	bool Init(){

	}

	bool UnInit();

	bool GetBurst(UINT32 iBurstPlanID, vector<CBurstItem> & ret){
		
	}

private:

	map<UINT32, IBurstPlan*> m_mapBurstPlanID2BurstPlan;
};

#endif
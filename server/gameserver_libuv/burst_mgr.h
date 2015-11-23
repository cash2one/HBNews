#ifndef burst_mgr_h__
#define burst_mgr_h__

#pragma once;

#include <vector>
#include <map>
#include "config.h"
#include "def.h"
using namespace std;

struct stPacketElement{
	EELEMENTTYPE iElementType;
	UINT32 iElementID;
	UINT32 iProbability;
	UINT32 iMAXNum;
	UINT32 iMinNum;
};

enum EPACKETBURSTTYPE{
	EPACKETBURSTTYPE_MIN = 0,
	EPACKETBURSTTYPE_1 = 1,		//N选1
	EPACKETBURSTTYPE_N = 2,		//N选N
	EPACKETBURSTTYPE_MAX
};

class CPacket{
public:
	//虚析构函数 以便用子类指针delete也会调用到父类的析构函数
	virtual ~CPacket(){
		for (std::vector<stPacketElement*>::iterator it = elements.begin() ; it != elements.end() ; ++it)
		{
			delete *it;
		}
	};

	virtual bool GetBurst(vector<stGameElement> & ret) = 0;

protected:
	UINT32 iPacketID;
	EPACKETBURSTTYPE eBurstType;
	std::vector<stPacketElement*> elements;//MAXPROBITY_BURST

	friend class CBurstMgr;
};

//多个里面爆出一个
class CPacket_Burst1 : public CPacket{
public:
	CPacket_Burst1()/* : m_bHasCachedTotalProbity(false), m_iCacheTotalProbity(0)*/{
		
	}

	virtual bool GetBurst(vector<stGameElement> & ret);

private:

	//以免每次都要计算概率总和 但是这种根据总的概率来 一定会爆一个的 而没有根据10w来
	//bool m_bHasCachedTotalProbity;//是否缓存了所有的概率数
	//UINT32 m_iCacheTotalProbity;//缓存所有的概率数
};

//每一个都可能会爆出
class CPacket_BurstN : public CPacket{
public:
	virtual bool GetBurst(vector<stGameElement> & ret);

};

class CBurstMgr{
public:
	//todo 单例工具
	inline static CBurstMgr* GetInstance(){
		static CBurstMgr singleton;
		return &singleton;
	}

	bool Init();

	bool UnInit();

	bool GetBurst(UINT32 iPacketID, vector<stGameElement> & ret);

	CPacket* GetPacketByPacketID(UINT32 iPacketID){
		map<UINT32, CPacket*>::iterator it = m_mapPacketID2Packet.find(iPacketID);
		if(it != m_mapPacketID2Packet.end()){
			return it->second;
		}
		return NULL;
	}

	bool AddPacket(CPacket* pPacket){
		ASSERT(pPacket);
		if (m_mapPacketID2Packet.find(pPacket->iPacketID) != m_mapPacketID2Packet.end())
		{
			printf("已经存在packet: id[%d]", pPacket->iPacketID);
			return false;
		}
		m_mapPacketID2Packet[pPacket->iPacketID] = pPacket;
		return true;
	}

private:

	CBurstMgr(){}

	map<UINT32, CPacket*> m_mapPacketID2Packet;
};

#endif // burst_mgr_h__
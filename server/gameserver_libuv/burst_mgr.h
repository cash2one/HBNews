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
	EPACKETBURSTTYPE_1 = 1,		//Nѡ1
	EPACKETBURSTTYPE_N = 2,		//NѡN
	EPACKETBURSTTYPE_MAX
};

class CPacket{
public:
	//���������� �Ա�������ָ��deleteҲ����õ��������������
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

//������汬��һ��
class CPacket_Burst1 : public CPacket{
public:
	CPacket_Burst1()/* : m_bHasCachedTotalProbity(false), m_iCacheTotalProbity(0)*/{
		
	}

	virtual bool GetBurst(vector<stGameElement> & ret);

private:

	//����ÿ�ζ�Ҫ��������ܺ� �������ָ����ܵĸ����� һ���ᱬһ���� ��û�и���10w��
	//bool m_bHasCachedTotalProbity;//�Ƿ񻺴������еĸ�����
	//UINT32 m_iCacheTotalProbity;//�������еĸ�����
};

//ÿһ�������ܻᱬ��
class CPacket_BurstN : public CPacket{
public:
	virtual bool GetBurst(vector<stGameElement> & ret);

};

class CBurstMgr{
public:
	//todo ��������
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
			printf("�Ѿ�����packet: id[%d]", pPacket->iPacketID);
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
#ifndef packet_mgr_h__
#define packet_mgr_h__

#include <vector>
#include <map>
#include "def.h"


struct stBurstElement
{
	UINT32 iElementType;
	UINT32 iElementSubType;
	UINT32 iElementID;
	UINT32 iElementNum;
};

struct stPacketElement
{
	UINT32 iElementType;
	UINT32 iElementSubType;
	UINT32 iElementID;
	UINT32 iElementProbability;
	UINT32 iElementNumMin;
	UINT32 iElementNumMax;
};

enum EPacketBurstType{
	EPacketBurstType_One = 1,//Nѡ1
	EPacketBurstType_N = 2//NѡN
};

struct stPacket{
	EPacketBurstType eBurstType;
	std::vector<stPacketElement> elements;
};

class CPacketMgr
{
public:
	bool BurstPacket(UINT32 packet_id, std::vector<stBurstElement>& ret){
		
	}
protected:
private:

	std::map<UINT32, stPacket*> m_mapPacketID2Packet;
};

#endif // packet_mgr_h__
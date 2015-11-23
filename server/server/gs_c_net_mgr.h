#ifndef _NET_MGR_HEADER_
#define _NET_MGR_HEADER_

#ifdef RAKNET
#include "RakPeerInterface.h"//RakPeerInterface
#include "RakNetTypes.h"//Packet
#else //ifdef LIBUV
#endif

#include <map>

#include "def.h"


class CNetMgr{
public:

	//todo ��������
	inline static CNetMgr* GetInstance(){
		static CNetMgr singleton;
		return &singleton;
	}

	void Init();

	void UnInit();

	/************************************************************************/
	/* 
	��Ҫ��Ƶ�ʿ��� ����ж����Ϣѭ��
	����ע�ⲻҪ���ͱ�raknet������ÿ� ����raknet�������
	*/
	/************************************************************************/
	void MessageLoop();

#ifdef RAKNET
	RakNet::RakPeerInterface * getRakPeerInterface(){
		return m_pRakPeer;
	}

	inline RakNet::Packet* GetCurPacket(){
		return m_pCurrentPacket;
	}

	inline void SetCurPacket(RakNet::Packet * packet){
		m_pCurrentPacket = packet;
	}

#else //ifdef LIBUV
#endif

	inline UINT32 GetCurPacketUserID(){
#ifdef RAKNET
		RakNet::Packet * pPkt = GetCurPacket();
		if (pPkt)
		{
			std::map<RakNet::SystemAddress, UINT32>::iterator it = m_mapAddress2UserID.find(pPkt->systemAddress);
			if (it != m_mapAddress2UserID.end())
			{
				return it->second;
			}
			else{
				return 0;
			}
		}
		else{
			return 0;
		}
#else //ifdef LIBUV

		return 0;
#endif
	}
	/* //��ʱ��ʹ��������Ϣͷ ֻ��Ҫ��һ�֣�
	// Copied from Multiplayer.cpp
	// If the first byte is ID_TIMESTAMP, then we want the 5th byte
	// Otherwise we want the 1st byte
	unsigned char GetPacketIdentifier(RakNet::Packet *p)
	{
	if (p==0)
	return 255;

	if ((unsigned char)p->data[0] == ID_TIMESTAMP)
	{
	RakAssert(p->length > sizeof(RakNet::MessageID) + sizeof(RakNet::Time));
	return (unsigned char) p->data[sizeof(RakNet::MessageID) + sizeof(RakNet::Time)];
	}
	else
	return (unsigned char) p->data[0];
	}
	*/

private:


#ifdef RAKNET
	RakNet::RakPeerInterface *m_pRakPeer;
	// Holds packets
	RakNet::Packet *m_pCurrentPacket;
	std::map<RakNet::SystemAddress, UINT32> m_mapAddress2UserID;
	std::map<UINT32, RakNet::SystemAddress> m_mapUserID2Address;
#else //ifdef LIBUV
#endif
};

#endif
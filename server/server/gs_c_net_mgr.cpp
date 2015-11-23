#include "MessageIdentifiers.h"

#include <stdio.h> //for puts gets
#include "Gets.h" // for gets

/*
#include <assert.h>
#include <cstdio>
#include <cstring>
#include <stdlib.h>

#include <stdio.h>
#include <string.h>
#include "Gets.h"
*/

#include "Kbhit.h"

#include "gs_c_message_handle_mgr.h"
#include "gs_c_net_mgr.h"
#include "gs_c_msg_body.pb.h"

void CNetMgr::Init()
{
#ifdef RAKNET
	RakNet::RakNetStatistics *rss;
	m_pRakPeer = RakNet::RakPeerInterface::GetInstance();
	m_pRakPeer->SetIncomingPassword("Rumpelstiltskin", (int)strlen("Rumpelstiltskin"));
	m_pRakPeer->SetTimeoutTime(30000,RakNet::UNASSIGNED_SYSTEM_ADDRESS);
	//	RakNet::PacketLogger packetLogger;
	//	server->AttachPlugin(&packetLogger);

#if LIBCAT_SECURITY==1
	cat::EasyHandshake handshake;
	char public_key[cat::EasyHandshake::PUBLIC_KEY_BYTES];
	char private_key[cat::EasyHandshake::PRIVATE_KEY_BYTES];
	handshake.GenerateServerKey(public_key, private_key);
	m_pRakPeer->InitializeSecurity(public_key, private_key, false);
	FILE *fp = fopen("publicKey.dat","wb");
	fwrite(public_key,sizeof(public_key),1,fp);
	fclose(fp);
#endif

	// Holds user data
	char portstring[30];

	// A server
	puts("Enter the server port to listen on");
	Gets(portstring,sizeof(portstring));
	if (portstring[0]==0)
		strcpy(portstring, "1234");

	puts("Starting server.");
	// Starting the server is very simple.  2 players allowed.
	// 0 means we don't care about a connectionValidationInteger, and false
	// for low priority threads
	// I am creating two socketDesciptors, to create two sockets. One using IPV6 and the other IPV4
	RakNet::SocketDescriptor socketDescriptors[2];
	socketDescriptors[0].port=atoi(portstring);
	socketDescriptors[0].socketFamily=AF_INET; // Test out IPV4
	socketDescriptors[1].port=atoi(portstring);
	socketDescriptors[1].socketFamily=AF_INET6; // Test out IPV6
	bool b = m_pRakPeer->Startup(4, socketDescriptors, 2 )==RakNet::RAKNET_STARTED;
	m_pRakPeer->SetMaximumIncomingConnections(4);
	if (!b)
	{
		printf("Failed to start dual IPV4 and IPV6 ports. Trying IPV4 only.\n");

		// Try again, but leave out IPV6
		b = m_pRakPeer->Startup(4, socketDescriptors, 1 )==RakNet::RAKNET_STARTED;
		if (!b)
		{
			puts("Server failed to start.  Terminating.");
			exit(1);
		}
	}
	m_pRakPeer->SetOccasionalPing(true);
	m_pRakPeer->SetUnreliableTimeout(1000);

	DataStructures::List< RakNet::RakNetSocket2* > sockets;
	m_pRakPeer->GetSockets(sockets);
	printf("Socket addresses used by RakNet:\n");
	for (unsigned int i=0; i < sockets.Size(); i++)
	{
		//md编译的dll里面没有这个符号！
		//printf("%i. %s\n", i+1, sockets[i]->GetBoundAddress().ToString(true));
	}

	printf("\nMy IP addresses:\n");
	for (unsigned int i=0; i < m_pRakPeer->GetNumberOfAddresses(); i++)
	{
		RakNet::SystemAddress sa = m_pRakPeer->GetInternalID(RakNet::UNASSIGNED_SYSTEM_ADDRESS, i);
		printf("%i. %s (LAN=%i)\n", i+1, sa.ToString(false), sa.IsLANAddress());
	}
#else //ifdef LIBUV

#endif
}

void CNetMgr::UnInit()
{
#ifdef RAKNET
	m_pRakPeer->Shutdown(300);
	// We're done with the network
	RakNet::RakPeerInterface::DestroyInstance(m_pRakPeer);
#else //ifdef LIBUV
#endif
}

void CNetMgr::MessageLoop()
{
#ifdef RAKNET
	/*
	// This sleep keeps RakNet responsive
	RakSleep(30);

	// Holds user data
	char portstring[30];
	char message[2048];
	if (kbhit())
	{
		// Notice what is not here: something to keep our network running.  It's
		// fine to block on gets or anything we want
		// Because the network engine was painstakingly written using threads.
		Gets(message,sizeof(message));

		if (strcmp(message, "quit")==0)
		{
			puts("Quitting.");
			break;
		}

		if (strcmp(message, "stat")==0)
		{
			rss=m_pRakPeer->GetStatistics(m_pRakPeer->GetSystemAddressFromIndex(0));
			StatisticsToString(rss, message, 2);
			printf("%s", message);
			printf("Ping %i\n", m_pRakPeer->GetAveragePing(m_pRakPeer->GetSystemAddressFromIndex(0)));

			continue;
		}

		if (strcmp(message, "ping")==0)
		{
			m_pRakPeer->Ping(clientID);

			continue;
		}

		if (strcmp(message, "pingip")==0)
		{
			printf("Enter IP: ");
			Gets(message,sizeof(message));
			printf("Enter port: ");
			Gets(portstring,sizeof(portstring));
			if (portstring[0]==0)
				strcpy(portstring, "1234");
			m_pRakPeer->Ping(message, atoi(portstring), false);

			continue;
		}

		if (strcmp(message, "kick")==0)
		{
			m_pRakPeer->CloseConnection(clientID, true, 0);

			continue;
		}

		if (strcmp(message, "getconnectionlist")==0)
		{
			RakNet::SystemAddress systems[10];
			unsigned short numConnections=10;
			m_pRakPeer->GetConnectionList((RakNet::SystemAddress*) &systems, &numConnections);
			for (int i=0; i < numConnections; i++)
			{
				printf("%i. %s\n", i+1, systems[i].ToString(true));
			}
			continue;
		}

		if (strcmp(message, "ban")==0)
		{
			printf("Enter IP to ban.  You can use * as a wildcard\n");
			Gets(message,sizeof(message));
			m_pRakPeer->AddToBanList(message);
			printf("IP %s added to ban list.\n", message);

			continue;
		}


		// Message now holds what we want to broadcast
		char message2[2048];
		// Append Server: to the message so clients know that it ORIGINATED from the server
		// All messages to all clients come from the server either directly or by being
		// relayed from other clients
		message2[0]=0;
		const static char prefix[] = "Server: ";
		strncpy(message2, prefix, sizeof(message2));
		strncat(message2, message, sizeof(message2) - strlen(prefix) - 1);

		// message2 is the data to send
		// strlen(message2)+1 is to send the null terminator
		// HIGH_PRIORITY doesn't actually matter here because we don't use any other priority
		// RELIABLE_ORDERED means make sure the message arrives in the right order
		// We arbitrarily pick 0 for the ordering stream
		// RakNet::UNASSIGNED_SYSTEM_ADDRESS means don't exclude anyone from the broadcast
		// true means broadcast the message to everyone connected
		m_pRakPeer->Send(message2, (const int) strlen(message2)+1, HIGH_PRIORITY, RELIABLE_ORDERED, 0, RakNet::UNASSIGNED_SYSTEM_ADDRESS, true);
	}
	*/

	//Sleep(30);//
	char message[2048];
	if (kbhit())
	{
		Gets(message,sizeof(message));//假如没有Gets kbhit()将会恒真！！
		if (strcmp(message, "test")==0)
		{
			GS_C_Register_Or_Login st;
			st.set_account("test");
			st.set_password("123456");
			st.set_plantform_id(1);
			st.set_plantform_sub_id(1);
			int len = st.ByteSize();
			char *buffer = new char[sizeof(RakNet::MessageID) + sizeof(unsigned int) + len];
			buffer[0] = (RakNet::MessageID)(ID_USER_PACKET_ENUM);
			*((unsigned int *)(buffer + 1)) = 1;
			st.SerializeToArray(buffer + sizeof(RakNet::MessageID) + sizeof(unsigned int), len);
			m_pRakPeer->Send(buffer, sizeof(RakNet::MessageID) + sizeof(unsigned int) + len, HIGH_PRIORITY,RELIABLE_ORDERED,0, RakNet::UNASSIGNED_SYSTEM_ADDRESS, true);
			delete[] buffer;
		}
	}

	for (m_pCurrentPacket = m_pRakPeer->Receive(); m_pCurrentPacket; m_pRakPeer->DeallocatePacket(m_pCurrentPacket), m_pCurrentPacket=m_pRakPeer->Receive())
	{
		// We got a packet, get the identifier with our handy function
		//packetIdentifier = GetPacketIdentifier(m_pGlobalPacket);

		unsigned char packetIdentifier = m_pCurrentPacket->data[0];

		// Check if this is a network message packet
		switch (packetIdentifier)
		{
		case ID_DISCONNECTION_NOTIFICATION:
			// Connection lost normally
			printf("ID_DISCONNECTION_NOTIFICATION from %s\n", m_pCurrentPacket->systemAddress.ToString(true));
			break;

		case ID_NEW_INCOMING_CONNECTION:
			{
				// Somebody connected.  We have their IP now
				printf("ID_NEW_INCOMING_CONNECTION from %s with GUID %s\n", m_pCurrentPacket->systemAddress.ToString(true), m_pCurrentPacket->guid.ToString());
				RakNet::SystemAddress clientID = m_pCurrentPacket->systemAddress; // Record the player ID of the client

				printf("Remote internal IDs:\n");
				for (int index=0; index < MAXIMUM_NUMBER_OF_INTERNAL_IDS; index++)
				{
					RakNet::SystemAddress internalId = m_pRakPeer->GetInternalID(m_pCurrentPacket->systemAddress, index);
					if (internalId!=RakNet::UNASSIGNED_SYSTEM_ADDRESS)
					{
						printf("%i. %s\n", index+1, internalId.ToString(true));
					}
				}

				break;
			}
		case ID_INCOMPATIBLE_PROTOCOL_VERSION:
			printf("ID_INCOMPATIBLE_PROTOCOL_VERSION\n");
			break;

		case ID_CONNECTED_PING:
		case ID_UNCONNECTED_PING:
			printf("Ping from %s\n", m_pCurrentPacket->systemAddress.ToString(true));
			break;

		case ID_CONNECTION_LOST:
			// Couldn't deliver a reliable packet - i.e. the other system was abnormally
			// terminated
			printf("ID_CONNECTION_LOST from %s\n", m_pCurrentPacket->systemAddress.ToString(true));
			break;

		case ID_USER_PACKET_ENUM:

			CMessageHandlerMgr::GetInstance()->ProcessMessage(m_pCurrentPacket);

			break;
		default:

			printf("no handle for sysmsdid %d from %s\n", packetIdentifier, m_pCurrentPacket->systemAddress.ToString(true));

			break;
		}
	}
#else //ifdef LIBUV
#endif
}
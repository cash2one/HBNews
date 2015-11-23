#include <stdio.h>
#include <Winsock2.h>
#pragma comment(lib,"WS2_32.lib")

void main()
{
	WORD wVersionRequested;
	WSADATA wsaData;
	int err;

	wVersionRequested = MAKEWORD(1,1);

	err = WSAStartup(wVersionRequested,&wsaData);

	if (err !=0)
	{
		return;
	}

	if (LOBYTE(wsaData.wVersion) != 1||
		HIBYTE( wsaData.wVersion ) != 1 )
	{
		WSACleanup();
		return;
	}

	SOCKET sockSrv = socket(AF_INET,SOCK_STREAM,0);

	SOCKADDR_IN addrSrv;
	addrSrv.sin_addr.S_un.S_addr=htonl(INADDR_ANY);
	addrSrv.sin_family=AF_INET;
	addrSrv.sin_port=htons(6000);
	bind(sockSrv,(SOCKADDR*)&addrSrv,sizeof(SOCKADDR));
	listen(sockSrv,5);

	SOCKADDR_IN addrClient;
	int len=sizeof(SOCKADDR);

	while(1)
	{
		SOCKET sockConn=accept(sockSrv,(SOCKADDR*)&addrClient,&len);
		printf("new client come: %s : %d", inet_ntoa(addrClient.sin_addr), addrClient.sin_port);
		char sendBuf[50];
		sprintf(sendBuf,"Welcome %s to here!",inet_ntoa(addrClient.sin_addr));
		send(sockConn,sendBuf,strlen(sendBuf)+1,0);
		char recvBuf[50] = {0};
		recv(sockConn,recvBuf,50,0);
		printf("%s\n",recvBuf);
		closesocket(sockConn);
	}

}
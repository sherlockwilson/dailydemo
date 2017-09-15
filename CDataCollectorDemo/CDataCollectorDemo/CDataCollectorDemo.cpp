// CDataCollectorDemo.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"

#include "ProcessCollector.h"
#include "ThreadCollector.h"

DWORD WINAPI ThreadProcessCollectorServer(LPVOID lpServer)
{
	CProcessCollector processCollector;
	processCollector.CreateSocket(COLLECTOR_PULL);
	int nBufSize = 1024*1024;
	processCollector.SetSockopt(SOCKET_RCVTBUF, nBufSize, sizeof(nBufSize));
	processCollector.Bind("127.0.0.1", 13);
	char* sendData = "Hello Client";
	char recvData[256] = { 0 };
	for (size_t i = 0; ; i++)
	{
		processCollector.RecvFrom(recvData, sizeof(recvData) / sizeof(char));
		printf("服务端接收的的第%d次数据为:  %s \n", i ,recvData);/*
		dataCollector.SendTo(sendData,strlen(sendData));
		printf("服务端发送数据为:  %s \n", sendData);*/
	}
	processCollector.CloseSocket();
	return 0;
}

DWORD WINAPI ThreadProcessCollectorClient(LPVOID lpClient)
{
	CProcessCollector processCollector;
	processCollector.CreateSocket(COLLECTOR_PUSH);
	processCollector.SetConnectInfo("127.0.0.1", 13);
	char* sendData = "Hello Server";
	char recvData[1024] = { 0 };
	for (size_t i = 0; i < 1000; i++)
	{
		processCollector.SendTo(sendData, strlen(sendData));
		printf("客户端发送的第%d次数据为:  %s \n", i,sendData);/*
		dataCollector.RecvFrom(recvData, sizeof(recvData) / sizeof(char));
		printf("客户端接收数据为:  %s \n", recvData);*/
	}
	processCollector.CloseSocket();
	return 0;
}

DWORD WINAPI ThreadCollectorServer(LPVOID lpServer)
{
	CThreadCollector threadCollector;
	char recvData[1024] = { 0 };
	for (int i = 0;;++i)
	{
		memset(recvData, 0, sizeof(recvData) / sizeof(char));
		threadCollector.RecvFrom(recvData, sizeof(recvData) / sizeof(char));
		printf("服务端第%d次收到了:%s\n",i,recvData);
	}
	return 0;
}

DWORD WINAPI ThreadCollectorClient(LPVOID lpClient)
{
	CThreadCollector threadCollector;
	char* strSendData = "Hello Server!";
	char szSendData[1024] = { 0 };
	int i = 0;
	for (;i<1000;++i)
	{
		memset(szSendData,0,sizeof(szSendData)/sizeof(char));
		memcpy_s(szSendData, sizeof(szSendData) / sizeof(char), strSendData, strlen(strSendData));
		threadCollector.SendTo(szSendData, sizeof(szSendData)/sizeof(char));
	}
	return 0;
}

int _tmain(int argc, _TCHAR* argv[])
{/*
	HANDLE lpServer = CreateThread(NULL, 0, ThreadCollectorServer, NULL, 0, NULL);
	CloseHandle(lpServer);

	Sleep(1000);
	
	for (unsigned i = 0;i<10;++i)
	{
		HANDLE lpClient = CreateThread(NULL, 0, ThreadCollectorClient, NULL, 0, NULL);
		CloseHandle(lpClient);
	}*/

	getchar();
	return 0;
}


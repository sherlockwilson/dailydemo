#include "stdafx.h"
#include "ProcessCollector.h"


CProcessCollector::CProcessCollector()
{
	WSAStartup(WINSOCK_VERSION, &m_struWsa);
	m_nConnectLen = sizeof(m_struConnectAddr);
	m_nPeerLen = sizeof(m_struPeerAddr);
}


CProcessCollector::~CProcessCollector()
{
	WSACleanup();
}

void CProcessCollector::CreateSocket(int nCollectorMode)
{
	m_nCollectorMode = nCollectorMode;
	switch (m_nCollectorMode)
	{
	case COLLECTOR_PULL:
		m_struBindSoc = socket(AF_INET, SOCK_DGRAM, 0);
		break;
	case COLLECTOR_PUSH:
		break;
	default:
		m_nCollectorMode = COLLECTOR_DEFAULT;
		break;
	}
	m_struConnectSoc = socket(AF_INET, SOCK_DGRAM, 0);
}

void CProcessCollector::SetSockopt(const int nOptName, const int nOptValue, const int nOptLen)
{
	int nLevel = SOL_SOCKET;
	int nResult = setsockopt(m_struBindSoc, nLevel, nOptName, (char*)&nOptValue, sizeof(nOptLen));
}

bool CProcessCollector::Bind(const char* strIP, int nPort)
{
	SetBindInfo(strIP, nPort);
	//绑定socket
	int nServerLen = sizeof(m_struBindAddr);
	int nResult = bind(m_struBindSoc, (SOCKADDR *)&m_struBindAddr, nServerLen);
	if (nResult == SOCKET_ERROR)
	{
		printf("套接字绑定失败!\n");
		return FALSE;
	}
	int nRecvBuf = 1024 * 1024;//设置为32K
	setsockopt(m_struBindSoc, SOL_SOCKET, SO_RCVBUF, (const char*)&nRecvBuf, sizeof(int));
	return TRUE;
}

void CProcessCollector::CloseSocket()
{
	switch (m_nCollectorMode)
	{
	case COLLECTOR_PULL:
		closesocket(m_struBindSoc);
		break;
	case COLLECTOR_PUSH:
		break;
	default:
		break;
	}
	closesocket(m_struConnectSoc);
	return;
}


void CProcessCollector::SetBindInfo(const char* strIP, int nPort)
{
	//命名协议，IP，端口
	m_struBindAddr.sin_family = AF_INET;
	m_struBindAddr.sin_port = htons(nPort);
	m_struBindAddr.sin_addr.s_addr = inet_addr(strIP);
}

void CProcessCollector::SetConnectInfo(const char* strIP, int nPort)
{
	//命名协议，IP，端口
	m_struConnectAddr.sin_family = AF_INET;
	m_struConnectAddr.sin_port = htons(nPort);
	m_struConnectAddr.sin_addr.s_addr = inet_addr(strIP);
}

void CProcessCollector::SendTo(const char* lpSend, const int nLen)
{
	int nResult = sendto(m_struConnectSoc, lpSend, nLen, 0, (SOCKADDR *)&m_struConnectAddr, m_nConnectLen);
}

void CProcessCollector::RecvFrom(char* lpRecv, const int nLen)
{
	int nResult = 0;
	switch (m_nCollectorMode)
	{
	case COLLECTOR_PULL:
		nResult = recvfrom(m_struBindSoc, lpRecv, nLen, 0, (SOCKADDR *)&m_struConnectAddr, &m_nConnectLen);
		break;
	case COLLECTOR_PUSH:
		nResult = recvfrom(m_struConnectSoc, lpRecv, nLen, 0, (SOCKADDR *)&m_struPeerAddr, &m_nPeerLen);
		break;
	default:
		break;
	}
	if (nResult >= 0)
	{
		lpRecv[nResult] = 0;
	}
}
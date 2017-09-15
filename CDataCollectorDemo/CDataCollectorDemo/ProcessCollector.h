#pragma once
#include "BaseCollector.h"

class CProcessCollector : CBaseCollector
{
#define COLLECTOR_DEFAULT 0x0000
#define COLLECTOR_PUSH    0x0001
#define COLLECTOR_PULL    0x0002

public:
	CProcessCollector();
	~CProcessCollector();

	void                   CreateSocket(int nCollectorMode);

#define SOCKET_RCVTIMEOUT SO_RCVTIMEO;
#define SOCKET_RCVTBUF SO_RCVBUF
	void                   SetSockopt(const int nOptName, const int nOptValue, const int nOptLen);

	bool                   Bind(const char* strIP, const int nPort);

	void                   CloseSocket();

	void                   SetBindInfo(const char* strIP, int nPort);

	void                   SetConnectInfo(const char* strIP, int nPort);

	void                   SendTo(const char* lpSend, const int nLen);

	void                   RecvFrom(char* lpRecv, const int nLen);

private:
	WSADATA                m_struWsa;

	SOCKET                 m_struBindSoc;
	SOCKET                 m_struConnectSoc;
	int                    m_nConnectLen;
	SOCKADDR_IN            m_struBindAddr;
	SOCKADDR_IN            m_struConnectAddr;
	SOCKADDR_IN            m_struPeerAddr;
	int                    m_nPeerLen;
	int                    m_nCollectorMode;
};


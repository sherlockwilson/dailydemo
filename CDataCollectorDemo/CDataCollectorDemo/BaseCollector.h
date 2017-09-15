#pragma once
class CBaseCollector
{

public:
	CBaseCollector();
	virtual ~CBaseCollector();

	virtual void                          SendTo(const char* lpSend, const int nLen)=0;

	virtual void                          RecvFrom(char* lpRecv, const int nLen)=0;

};


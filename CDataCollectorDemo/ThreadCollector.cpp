#include "stdafx.h"
#include <queue>
#include <mutex>
#include <condition_variable>
#include <initializer_list>
#include "ThreadCollector.h"


CThreadCollector::CThreadCollector()
{
	if (!m_cQueue)
	{
		m_cQueue = std::make_shared<BlockingQueue<DATA>>();
	}
}

CThreadCollector::~CThreadCollector()
{
}

std::shared_ptr<CThreadCollector::BlockingQueue<DATA>> CThreadCollector::m_cQueue;

void                                    CThreadCollector::SendTo(const char* lpSend, const int nLen)
{
	DATA struData;
	struData.nLen = nLen;
	memcpy_s(struData.szContent, sizeof(struData.szContent)/sizeof(char),lpSend,nLen);
	m_cQueue->Put(struData);
}

void                                    CThreadCollector::RecvFrom(char* lpRecv, const int nLen)
{
	DATA struData;
	struData=m_cQueue->Take();
	int nDataLen = struData.nLen;
	if (nLen >= nDataLen)
	{
		memcpy_s(lpRecv, nLen, struData.szContent, nDataLen);
	}
}


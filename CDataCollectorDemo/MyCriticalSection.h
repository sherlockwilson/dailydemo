#pragma once
class CMyCriticalSection
{
public:
public:
	CMyCriticalSection();

	~CMyCriticalSection();

	void Lock();

	void Unlock();

protected:
	CRITICAL_SECTION m_cs;
};


#pragma once
template<typename POOLTYPE, typename LOCKTYPE>
class CMTMemoryPool
{
public:
	void* Alloc(unsigned int size)
	{
		void* p = NULL;
		m_lock.Lock();
		p = m_pool.Alloc(size);
		m_lock.Unlock();

		return p;
	}

	void Free(void* p)
	{
		m_lock.Lock();
		m_pool.Free(p);
		m_lock.Unlock();
	}

private:
	POOLTYPE m_pool;
	LOCKTYPE m_lock;
};
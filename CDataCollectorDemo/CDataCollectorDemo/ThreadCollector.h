#pragma once
#include "BaseCollector.h"

#ifndef __THREAD_COLLECTOR_H__
#define __THREAD_COLLECTOR_H__
#endif

#define MAX_DATA_LEN 1024

typedef struct
{
	CHAR szContent[MAX_DATA_LEN];
	INT  nLen;
}DATA,*LPDATA;

class CThreadCollector : CBaseCollector
{
public:
	CThreadCollector();
	~CThreadCollector();

	virtual void                          SendTo(const char* lpSend, const int nLen);

	virtual void                          RecvFrom(char* lpRecv, const int nLen);            
protected:
	template<typename T>
	class BlockingQueue
	{
	public:
		BlockingQueue() : _mutex(), _condvar(), _queue()
		{

		}

		void Put(const T& task)
		{
			{
				std::lock_guard<std::mutex> lock(_mutex);
				_queue.push_back(task);
			}
			_condvar.notify_all();
		}

		T Take()
		{
			std::unique_lock<std::mutex> lock(_mutex);
			_condvar.wait(lock, [this]{return !_queue.empty(); });
			assert(!_queue.empty());
			T front(_queue.front());
			_queue.pop_front();

			return front;
		}

		size_t Size() const
		{
			std::lock_guard<std::mutex> lock(_mutex);
			return _queue.size();
		}

	private:
		BlockingQueue(const BlockingQueue& rhs);
		BlockingQueue& operator = (const BlockingQueue& rhs);

	private:
		mutable std::mutex _mutex;
		std::condition_variable _condvar;
		std::list<T> _queue;
	};
	static std::shared_ptr<BlockingQueue<DATA>>             m_cQueue;
};


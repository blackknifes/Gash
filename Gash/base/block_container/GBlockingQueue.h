#ifndef __GBLOCKINGQUEUE_H__
#define __GBLOCKINGQUEUE_H__
#include "../GObject.h"
#include "../sync/GLocker.h"
#include <queue>

template<typename _Ty>
class GBlockingQueue
{
public:
	typedef std::queue<GScopePointer<_Ty>> queue_type;


	GBlockingQueue()
		:m_threadLocker(200)
	{

	}

	bool empty() const
	{
		return m_queue.empty();
	}

	void push(GScopePointer<_Ty> pData)
	{
		GThreadAutoLock _autolock(m_threadLocker);
		m_queue.push(pData);
	}

	void swap(std::queue<GScopePointer<_Ty>>& _queue)
	{
		GThreadAutoLock _autolock(m_threadLocker);
		m_queue.swap(_queue);
	}

	GScopePointer<_Ty> pop()
	{
		if (m_queue.empty())
			return nullptr;
		GThreadAutoLock _autolock(m_threadLocker);
		GScopePointer<_Ty> pData = m_queue.back();
		m_queue.pop();
		return pData;
	}
private:
	GDisallowAssignAndCopy(GBlockingQueue);
	GThreadLocker m_threadLocker;
	std::queue<GScopePointer<_Ty>> m_queue;
};
#endif
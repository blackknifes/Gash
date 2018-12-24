#ifndef __GLOCKER_H__
#define __GLOCKER_H__
#include <Windows.h>

class GThreadLocker
{
public:
	GThreadLocker();
	GThreadLocker(DWORD spainCount);
	~GThreadLocker();
	void lock() const;
	void unlock() const;
private:
	GThreadLocker(GThreadLocker&&);
	GThreadLocker(const GThreadLocker&);
	CRITICAL_SECTION m_section;
};

template<typename _Ty>
class GAutoLock
{
public:
	GAutoLock(_Ty& locker)
		:m_locker(locker)
	{
		m_locker.lock();
	}
	~GAutoLock()
	{
		m_locker.unlock();
	}
private:
	const _Ty& m_locker;
};

typedef GAutoLock<GThreadLocker> GThreadAutoLock;
#endif
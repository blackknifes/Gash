#ifndef __GTEMPMEMORY_H__
#define __GTEMPMEMORY_H__
#include <functional>

class GTempMemory
{
public:
	typedef std::function<void(void* pData)> Callback;
	GTempMemory(size_t size, const Callback& callback);
private:
	Callback m_callback;
};
#endif
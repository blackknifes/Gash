#include <objbase.h>

#pragma comment(lib, "Ole32.lib")
class ComInit
{
public:
	ComInit()
	{
		::CoInitializeEx(nullptr, COINIT_APARTMENTTHREADED);
	}
	~ComInit()
	{
		::CoUninitialize();
	}
};

static __declspec(thread) ComInit __cominit;
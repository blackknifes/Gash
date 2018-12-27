#include "GTempMemory.h"



GTempMemory::GTempMemory(size_t size, const Callback& callback)
{
	if (size < 0x4000)
	{
		char buf[0x4000];
		callback(buf);
	}
	else
	{
		char* buf = (char*)malloc(size);
		callback(buf);
		free(buf);
	}
}

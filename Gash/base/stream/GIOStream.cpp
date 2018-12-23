#include "GIOStream.h"



GIOStream::GIOStream()
{

}

size_t GIOStream::read(void* pData, size_t _size)
{
	return std::cin.readsome((char*)pData, _size);
}

void GIOStream::write(const void* pData, size_t _size)
{
	std::cout.write((const char*)pData, _size);
}

void GIOStream::flush()
{
	
}

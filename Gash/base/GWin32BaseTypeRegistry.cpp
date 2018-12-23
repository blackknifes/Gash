#include "GWin32BaseTypeRegistry.h"



bool _CompareUCharArray(const void* pLeft, const void* pRight, size_t size)
{
	const unsigned char* _left = (const unsigned char*)pLeft;
	const unsigned char* _right = (const unsigned char*)pRight;
	for (size_t i = 0; i < size; ++i)
	{
		if (_left[i] != _right[i])
			return false;
	}
	return true;
}

void Win32BaseTypeRegister()
{
	GAddUserType(RECT);
	GAddUserType(POINT);
	GAddUserType(SIZE);
	GAddUserType(POINTS);
}

#include "GGlobalFunction.h"

GString GGetErrorMessage(DWORD errCode)
{
	wchar_t buf[1024] = {};
	FormatMessageW(FORMAT_MESSAGE_FROM_SYSTEM, nullptr, errCode, LANG_USER_DEFAULT, buf, 1024, nullptr);
	return buf;
}

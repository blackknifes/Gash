#include "GErrno.h"

__declspec(thread) int gerrno = 0;

void GSetErrno(int errCode)
{
	gerrno = errCode;
}

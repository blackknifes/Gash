#ifndef __GERRNO_H__
#define __GERRNO_H__

extern __declspec(thread) int gerrno;

void GSetErrno(int errCode);
#endif
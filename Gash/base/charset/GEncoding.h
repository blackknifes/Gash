#ifndef __GENCODING_H__
#define __GENCODING_H__
#include <corecrt.h>

struct Encoder
{
	void* unused;
};

enum GEncoding
{
	EncodingAnsi = 0,
	EncodingUtf8,
	EncodingGBK,
	EncodingGB2312,
	EncodingUtf16LE,
	EncodingUtf16BE,
	EncodingUtf16 = EncodingUtf16LE,
	EncodingBinary
};

errno_t EncodeString(GEncoding from, GEncoding to, const void** src, size_t* srcsize, void** dest, size_t* destsize);
errno_t EncodeString(Encoder* pEncoder, const void** src, size_t* srcsize, void** dest, size_t* destsize);
bool EncodeIsCritical(errno_t err);

Encoder* FindEncoder(GEncoding from, GEncoding to);
#endif
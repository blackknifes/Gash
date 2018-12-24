#ifndef __GSTREAM_H__
#define __GSTREAM_H__

#include "../GObject.h"
#include "../GDataArray.h"
#include "../io/GIODevice.h"
#include <vector>
#include <set>
#include "GStreamDevice.h"

class GOutputStream;
class GInputStream;
GREFPTR_DEF(GOutputStream);
GREFPTR_DEF(GInputStream);

class GInputStream: public virtual GStreamDevice
{
public:
	virtual size_t read(void* pData, size_t _size) = 0;

	virtual void read(GOutputStreamPtr pOutputStream);

	virtual void pipe(GOutputStreamPtr pOutputStream);
	virtual GDataArray read(size_t _size);
	virtual GDataArray readAll();
	virtual bool isAsync() const { return false; }
	virtual bool isPending() const { return false; };
	virtual void sync() {}

	virtual const GInputStream& operator>>(unsigned char& val);
	virtual const GInputStream& operator>>(char& val);
	virtual const GInputStream& operator>>(unsigned short& val);
	virtual const GInputStream& operator>>(short& val);
	virtual const GInputStream& operator>>(unsigned int& val);
	virtual const GInputStream& operator>>(int& val);
	virtual const GInputStream& operator>>(unsigned __int64& val);
	virtual const GInputStream& operator>>(__int64& val);
	virtual const GInputStream& operator>>(float& val);
	virtual const GInputStream& operator>>(double& val);
	virtual const GInputStream& operator>>(GString& val);
	virtual const GInputStream& operator>>(std::string& val);
	virtual const GInputStream& operator>>(std::wstring& val);
	virtual const GInputStream& operator>>(GDataArray& val);
};

class GOutputStream : public virtual GStreamDevice
{
public:
	virtual void write(const void* pData, size_t _size) = 0;
	virtual void end() {};
	virtual void flush() = 0;

	virtual void write(const GDataArray& dataArray);
	virtual const GOutputStream& operator<<(unsigned char val);
	virtual const GOutputStream& operator<<(char val);
	virtual const GOutputStream& operator<<(unsigned short val);
	virtual const GOutputStream& operator<<(short val);
	virtual const GOutputStream& operator<<(unsigned int val);
	virtual const GOutputStream& operator<<(int val);
	virtual const GOutputStream& operator<<(unsigned __int64 val);
	virtual const GOutputStream& operator<<(__int64 val);
	virtual const GOutputStream& operator<<(float val);
	virtual const GOutputStream& operator<<(double val);
	virtual const GOutputStream& operator<<(const GString& val);
	virtual const GOutputStream& operator<<(const std::string& val);
	virtual const GOutputStream& operator<<(const std::wstring& val);
	virtual const GOutputStream& operator<<(const GDataArray& val);
	virtual const GOutputStream& operator<<(const char* val);
	virtual const GOutputStream& operator<<(const wchar_t* val);
};
#endif
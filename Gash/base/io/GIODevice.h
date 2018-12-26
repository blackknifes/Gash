#ifndef __GDEVICE_H__
#define __GDEVICE_H__
#include "../GObject.h"
#include "GIOEnums.h"
#include <functional>

GREFPTR_DEF(GIODevice);

class GIODevice: public virtual GObject
{
public:
	typedef std::function<void(size_t)> WriteCallback;
	typedef std::function<void(const void*, size_t)> ReadCallback;

	GIODevice();

	//打开设备
	virtual bool open() = 0;
	//关闭设备
	virtual void close() = 0;
	//设备是否关闭
	virtual bool isClosed() const = 0;
	//写出数据到设备
	virtual bool write(const void* pData, size_t size, const WriteCallback& callback) = 0;
	//从设备读入数据
	virtual bool read(size_t size, const ReadCallback& callback) = 0;
	//写出数据到设备
	virtual size_t writeSync(const void* pData, size_t size) = 0;
	//从设备读入数据
	virtual size_t readSync(void* pData, size_t size) = 0;

	//刷写数据到设备
	virtual void flush() = 0;
	virtual void* getNativeHandle() const = 0;

	//得到错误代码
	int getErrorCode() const;
	std::string getErrorMessage() const;
protected:
	void setErrorCode(int errCode);
private:
	int m_iErrorCode;
};
#endif

#ifndef __GDEVICE_H__
#define __GDEVICE_H__
#include "../GObject.h"

class GDevice: public GObject
{
public:
	enum IOResult
	{
		IO_Success = 0,
		IO_Internal_Error,
		IO_NotOpen,
		IO_Read_Pending,
		IO_Write_Pending
	};
	GDevice();

	//打开设备
	virtual bool open(bool async = false) = 0;
	//关闭设备
	virtual void close() = 0;
	//设备是否关闭
	virtual bool isClosed() const = 0;
	//写出数据到设备
	virtual IOResult write(const void* pData, size_t size) = 0;
	//从设备读入数据
	virtual IOResult read(void* pData, size_t size, size_t* readsize = nullptr) = 0;
	//刷写数据到设备
	virtual void flush() = 0;
	//是否是异步设备
	virtual bool isAsync() const = 0;
	//是否为同步设备
	bool isSync() const;
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

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

	//���豸
	virtual bool open() = 0;
	//�ر��豸
	virtual void close() = 0;
	//�豸�Ƿ�ر�
	virtual bool isClosed() const = 0;
	//д�����ݵ��豸
	virtual bool write(const void* pData, size_t size, const WriteCallback& callback) = 0;
	//���豸��������
	virtual bool read(size_t size, const ReadCallback& callback) = 0;
	//д�����ݵ��豸
	virtual size_t writeSync(const void* pData, size_t size) = 0;
	//���豸��������
	virtual size_t readSync(void* pData, size_t size) = 0;

	//ˢд���ݵ��豸
	virtual void flush() = 0;
	virtual void* getNativeHandle() const = 0;

	//�õ��������
	int getErrorCode() const;
	std::string getErrorMessage() const;
protected:
	void setErrorCode(int errCode);
private:
	int m_iErrorCode;
};
#endif

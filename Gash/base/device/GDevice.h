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

	//���豸
	virtual bool open(bool async = false) = 0;
	//�ر��豸
	virtual void close() = 0;
	//�豸�Ƿ�ر�
	virtual bool isClosed() const = 0;
	//д�����ݵ��豸
	virtual IOResult write(const void* pData, size_t size) = 0;
	//���豸��������
	virtual IOResult read(void* pData, size_t size, size_t* readsize = nullptr) = 0;
	//ˢд���ݵ��豸
	virtual void flush() = 0;
	//�Ƿ����첽�豸
	virtual bool isAsync() const = 0;
	//�Ƿ�Ϊͬ���豸
	bool isSync() const;
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

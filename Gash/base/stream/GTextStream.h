#ifndef __GTEXTSTREAM_H__
#define __GTEXTSTREAM_H__
#include "GStream.h"

class GTextInputStream: public GInputStream
{
public:
	using Encoding = GDataArray::Encoding;

	GTextInputStream(GInputStreamPtr is, Encoding encoding = Encoding::EncodingUtf8);
	virtual size_t read(void* pData, size_t _size) override;

	virtual const GInputStream& operator >>(unsigned char& val) override;
	virtual const GInputStream& operator >>(char& val) override;
	virtual const GInputStream& operator >>(unsigned short& val) override;
	virtual const GInputStream& operator >>(short& val) override;
	virtual const GInputStream& operator >>(unsigned int& val) override;
	virtual const GInputStream& operator >>(int& val) override;
	virtual const GInputStream& operator >>(unsigned __int64& val) override;
	virtual const GInputStream& operator >>(__int64& val) override;
	virtual const GInputStream& operator >>(float& val) override;
	virtual const GInputStream& operator >>(double& val) override;
	virtual const GInputStream& operator >>(GString& val) override;
	virtual const GInputStream& operator >>(std::string& val) override;
	virtual const GInputStream& operator >>(std::wstring& val) override;
	virtual const GInputStream& operator >>(GDataArray& val) override;

private:
	GObjectClassPooled(GTextInputStream);
	GInputStreamPtr m_pInputStream;
	Encoding m_encoding;
};

class GTextOutputStream: public GOutputStream
{
public:
	using Encoding = GDataArray::Encoding;

	GTextOutputStream(GOutputStreamPtr out, Encoding encoding = Encoding::EncodingUtf8);
	virtual void write(const void* pData, size_t _size) override;
	virtual void flush() override;

	virtual const GOutputStream& operator <<(unsigned char val) override;
	virtual const GOutputStream& operator <<(char val) override;
	virtual const GOutputStream& operator <<(unsigned short val) override;
	virtual const GOutputStream& operator <<(short val) override;
	virtual const GOutputStream& operator <<(unsigned int val) override;
	virtual const GOutputStream& operator <<(int val) override;
	virtual const GOutputStream& operator <<(unsigned __int64 val) override;
	virtual const GOutputStream& operator <<(__int64 val) override;
	virtual const GOutputStream& operator <<(float val) override;
	virtual const GOutputStream& operator <<(double val) override;
	virtual const GOutputStream& operator <<(const GString& val) override;
	virtual const GOutputStream& operator <<(const std::string& val) override;
	virtual const GOutputStream& operator <<(const std::wstring& val) override;
	virtual const GOutputStream& operator <<(const GDataArray& val) override;

private:
	GObjectClassPooled(GTextOutputStream);
	GOutputStreamPtr m_pOutputStream;
	Encoding m_encoding;
};
#endif
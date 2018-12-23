#ifndef __GDATAARRAY_H__
#define __GDATAARRAY_H__
#include "GString.h"

class GDataArray
{
public:
	enum Encoding
	{
		EncodingAnsi = 0,
		EncodingUnicode,
		EncodingUtf8
	};

	typedef unsigned char byte_t;
	GDataArray();

	GDataArray(const wchar_t* str, Encoding encoding = EncodingUnicode);
	explicit GDataArray(size_t _size);
	GDataArray(const void* pData, size_t _size);
	GDataArray(const GDataArray& dataArray);
	GDataArray(GDataArray&& dataArray);
	const GDataArray& operator=(const GDataArray& dataArray);
	const GDataArray& operator=(GDataArray&& dataArray);
	~GDataArray();

	byte_t* data();
	const byte_t* data() const;

	byte_t& operator[](size_t index);
	byte_t operator[](size_t index) const;

	byte_t& at(size_t index);
	byte_t at(size_t index) const;

	byte_t* begin();
	byte_t* end();
	const byte_t* begin() const;
	const byte_t* end() const;

	size_t size() const;
	size_t capacity() const;
	void reserve(size_t _size);
	void resize(size_t _size);
	void append(const void* pData, size_t bytes);
	void assign(const void* pData, size_t bytes);

	size_t read(void* pData, size_t bytes, size_t offset) const;
	void write(const void* pData, size_t bytes, size_t offset);

	GString toString(Encoding from = EncodingUtf8) const;
private:
	byte_t* m_pData;
	size_t m_size;
	size_t m_capacity;
};
#endif
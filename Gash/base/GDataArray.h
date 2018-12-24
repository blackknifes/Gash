#ifndef __GDATAARRAY_H__
#define __GDATAARRAY_H__
#include <memory>
#include <Windows.h>
#include "charset/GEncoding.h"
#include "GString.h"

class GDataArray
{
public:
	using Encoding = GEncoding;
	typedef unsigned char byte_t;
	GDataArray();

	GDataArray(const char* str, GEncoding encoding = GEncoding::EncodingAnsi);
	GDataArray(const char* str, size_t len, GEncoding encoding = GEncoding::EncodingAnsi);

	GDataArray(const wchar_t* str);
	GDataArray(const wchar_t* str, size_t len);
	explicit GDataArray(size_t _size);
	GDataArray(const void* pData, size_t _size);
	GDataArray(const GDataArray& dataArray);
	GDataArray(GDataArray&& dataArray);
	const GDataArray& operator=(const GDataArray& dataArray);
	const GDataArray& operator=(GDataArray&& dataArray);
	~GDataArray();

	bool operator==(const GDataArray& dataArray) const;
	bool operator!=(const GDataArray& dataArray) const;
	bool operator>(const GDataArray& dataArray) const;
	bool operator>=(const GDataArray& dataArray) const;
	bool operator<(const GDataArray& dataArray) const;
	bool operator<=(const GDataArray& dataArray) const;

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
	void insert(const void* pData, size_t bytes, size_t offset);
	void erase(size_t offset, size_t count = 1);
	void replace(const void* replacement, size_t bytes, size_t offset, size_t count = 1);

	GDataArray sub(size_t offset, size_t count) const;
	GDataArray slice(size_t _start, size_t _end) const;

	bool empty() const;

	size_t read(void* pData, size_t bytes, size_t offset) const;
	void write(const void* pData, size_t bytes, size_t offset);
	size_t hashCode() const;

	GDataArray encode(GEncoding to) const;
	GDataArray encode(GEncoding from, GEncoding to) const;

	GString toString(GEncoding encoding = EncodingUtf8) const;
	std::string toAnsi(GEncoding encoding = EncodingUtf8) const;
	std::string toUtf8(GEncoding encoding = EncodingUtf8) const;
private:
	byte_t* m_pData;
	size_t m_size;
	size_t m_capacity;
};

template<>
struct std::hash<GDataArray>
{
	size_t operator()(const GDataArray& dataArray) const
	{
		return dataArray.hashCode();
	}
};
#endif
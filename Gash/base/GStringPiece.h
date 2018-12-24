#ifndef __GSTRINGPIECE_H__
#define __GSTRINGPIECE_H__
#include "GString.h"

//字符串片段，必须保证原字符串内存不被释放或修改
class GStringPiece
{
public:
	typedef GString::const_iterator const_iterator;
	static const size_t npos = GString::npos;

	GStringPiece();
	GStringPiece(const GString& str);
	GStringPiece(const GString& str, size_t size);
	GStringPiece(const GString& str, size_t _start, size_t _end);

	GStringPiece(const std::wstring& str);
	GStringPiece(const std::wstring& str, size_t size);
	GStringPiece(const std::wstring& str, size_t _start, size_t _end);
	GStringPiece(const_iterator _begin, const_iterator _end);

	const_iterator begin() const;
	const_iterator end() const;

	size_t size() const;
	bool empty() const;
	bool isEmptyOrBlank() const;

	bool operator==(const GStringPiece& str) const;
	bool operator!=(const GStringPiece& str) const;
	bool operator>(const GStringPiece& str) const;
	bool operator>=(const GStringPiece& str) const;
	bool operator<(const GStringPiece& str) const;
	bool operator<=(const GStringPiece& str) const;

	GString operator+(const GStringPiece& str) const;

	wchar_t operator[](size_t index) const;
	wchar_t at(size_t index) const;

	int compare(const GStringPiece& str, bool caseSentive = true) const;
	bool compareWith(const GStringPiece& str, bool caseSentive = true) const;
	bool startsWith(const GStringPiece& str, bool caseSentive = true) const;
	bool endsWith(const GStringPiece& str, bool caseSentive = true) const;
	bool endsWithSeperator() const;
	bool startsWithSeperator() const;
	bool isUpper() const;
	bool isLower() const;
	bool isNumber() const;

	bool toBool() const;
	int toInt() const;
	unsigned int toUInt() const;
	unsigned int toHex() const;
	__int64 toInt64() const;
	unsigned __int64 toUInt64() const;
	double toDouble() const;

	GStringPiece substr(size_t offset, size_t count = npos) const;
	GStringPiece slice(size_t _start, size_t _end = npos) const;
	size_t find(const GStringPiece& str, size_t offset = 0, bool caseSentive = true) const;
	size_t findFirst(const GStringPiece& str, size_t offset = 0, bool caseSentive = true) const;
	size_t findFirstNot(const GStringPiece& str, size_t offset = 0, bool caseSentive = true) const;
	size_t findLast(const GStringPiece& str, size_t offset = npos, bool caseSentive = true) const;
	size_t findLastNot(const GStringPiece& str, size_t offset = npos, bool caseSentive = true) const;


	size_t find(const wchar_t* str, size_t offset = 0, bool caseSentive = true) const;
	size_t findFirst(const wchar_t* str, size_t offset = 0, bool caseSentive = true) const;
	size_t findFirstNot(const wchar_t* str, size_t offset = 0, bool caseSentive = true) const;
	size_t findLast(const wchar_t* str, size_t offset = npos, bool caseSentive = true) const;
	size_t findLastNot(const wchar_t* str, size_t offset = npos, bool caseSentive = true) const;


	GString toString() const;

	size_t read(wchar_t* pOut, size_t readsize) const;
private:
	const_iterator m_begin;
	const_iterator m_end;
};
#endif

#ifndef __GSTRINGPIECE_H__
#define __GSTRINGPIECE_H__
#include "GString.h"
#include "GStringPieceIterator.h"

//字符串片段，必须保证原字符串内存不被释放或修改
class GStringPiece
{
public:
	typedef std::wstring::const_iterator const_iterator;

	static const size_t npos = std::wstring::npos;

	static GStringPiece From(const GString& str);
	static GStringPiece From(const GString& str, size_t size);
	static GStringPiece From(const GString& str, size_t _start, size_t _end);

	static GStringPiece From(const std::wstring& str);
	static GStringPiece From(const std::wstring& str, size_t size);
	static GStringPiece From(const std::wstring& str, size_t _start, size_t _end);

	static GStringPiece From(const wchar_t* str);
	static GStringPiece From(const wchar_t* _start, const wchar_t* _end);
	static GStringPiece From(const wchar_t* str, size_t size);
	static GStringPiece From(const wchar_t* str, size_t _start, size_t _end);

	GStringPiece();
	GStringPiece(const GString& str);
	GStringPiece(const GString& str, size_t size);
	GStringPiece(const GString& str, size_t _start, size_t _end);

	GStringPiece(const std::wstring& str);
	GStringPiece(const std::wstring& str, size_t size);
	GStringPiece(const std::wstring& str, size_t _start, size_t _end);


	GStringPiece(const GStringPieceIterator& _begin, const GStringPieceIterator& _end);
	GStringPiece(const_iterator _begin, const_iterator _end);
	GStringPiece(const wchar_t* str);
	GStringPiece(const wchar_t* _begin, const wchar_t* _end);
	GStringPiece(const wchar_t* _begin, size_t size);

	GStringPieceIterator begin() const;
	GStringPieceIterator end() const;

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

	GString addStartsWith(const GStringPiece& str, bool caseSentive = true) const;
	GString addEndsWith(const GStringPiece& str, bool caseSentive = true) const;
	GString addEndsWithSeperator() const;
	GString addStartsWithSeperator() const;

	bool isUpper() const;
	bool isLower() const;
	bool isNumber() const;
	bool isBool() const;

	GString toUpper() const;
	GString toLower() const;

	bool toBool() const;
	int toInt() const;
	unsigned int toUInt() const;
	unsigned int toHex() const;
	__int64 toInt64() const;
	unsigned __int64 toUInt64() const;
	double toDouble() const;

	GStringPiece substr(size_t offset, size_t count = npos) const;
	GStringPiece slice(size_t _start, size_t _end = npos) const;

	size_t copy(wchar_t* buffer, size_t bufsize) const;
	size_t insert(GString& str, size_t offset, size_t count = npos) const;
	size_t append(GString& str, size_t count = npos) const;

	size_t find(const GStringPiece& str, size_t offset = 0, bool caseSentive = true) const;
	size_t findFirst(const GStringPiece& str, size_t offset = 0, bool caseSentive = true) const;
	size_t findFirstNot(const GStringPiece& str, size_t offset = 0, bool caseSentive = true) const;
	size_t findLast(const GStringPiece& str, size_t offset = npos, bool caseSentive = true) const;
	size_t findLastNot(const GStringPiece& str, size_t offset = npos, bool caseSentive = true) const;

	GString toString() const;
private:
	GStringPieceIterator m_begin;
	GStringPieceIterator m_end;
};
#endif

#ifndef __GSTRING_H__
#define __GSTRING_H__
#include <string>
#include <vector>
#include "charset/GEncoding.h"

class GRegex;

class GString
{
public:
	typedef typename std::wstring StdString;
	typedef typename wchar_t char_type;
	typedef typename std::wstring::size_type size_type;
	typedef typename std::wstring::iterator iterator;
	typedef typename std::wstring::const_iterator const_iterator;
	typedef typename std::wstring::reverse_iterator reverse_iterator;
	typedef typename std::wstring::const_reverse_iterator const_reverse_iterator;

	static const size_t npos = std::wstring::npos;

	static GString FromEncoding(const char* str, GEncoding encoding = EncodingAnsi);
	static GString FromEncoding(const char* str, size_t size, GEncoding encoding = EncodingAnsi);

	static GString FromStringPiece(const_iterator _begin, const_iterator _end);

	GString();
	GString(const_iterator _begin, const_iterator _end);
	GString(const char* str, GEncoding encoding = EncodingAnsi);
	GString(const char* str, size_t size, GEncoding encoding = EncodingAnsi);
	GString(const wchar_t* str);
	GString(const wchar_t* str, size_t size);

	GString(const GString& str);
	GString(GString&& str);

	const GString& operator=(const GString& str);
	const GString& operator=(GString&& str);

	const GString& operator=(const wchar_t* str);
	const GString& operator=(const char* str);

	bool operator==(const GString& str) const;
	bool operator==(const wchar_t* str) const;

	const GString& operator+=(const GString& str);
	const GString& operator+=(const wchar_t* str);
	GString operator+(const GString& str) const;
	GString operator+(const wchar_t* str) const;

	bool operator>(const GString& str) const;
	bool operator>(const wchar_t* str) const;
	bool operator>=(const GString& str) const;
	bool operator>=(const wchar_t* str) const;
	bool operator<(const GString& str) const;
	bool operator<(const wchar_t* str) const;
	bool operator<=(const GString& str) const;
	bool operator<=(const wchar_t* str) const;

	const wchar_t* data() const;
	const wchar_t* c_str() const;
	GString& resize(size_t size, wchar_t ch = 0);
	GString& reserve(size_t size);
	GString& clear();

	size_t size() const;
	size_t capacity() const;
	bool empty() const;
	bool isEmptyOrBlank() const;

	iterator begin();
	iterator end();
	const_iterator begin() const;
	const_iterator end() const;

	reverse_iterator rbegin();
	reverse_iterator rend();
	const_reverse_iterator rbegin() const;
	const_reverse_iterator rend() const;

	std::string toAnsi() const;
	std::string toUtf8() const;
	const std::wstring& toUnicode() const;
	std::wstring& toUnicode();

	int compare(const GString& str, bool caseSentive = true) const;
	bool compareWith(const GString& str, bool caseSentive = true) const;

	wchar_t& front();
	const wchar_t& front() const;
	wchar_t& back();
	const wchar_t& back() const;
	const wchar_t& at(size_t index) const;
	wchar_t& at(size_t index);
	const wchar_t& operator[](size_t index) const;
	wchar_t& operator[](size_t index);

	wchar_t pop_back();
	GString& push_back(wchar_t ch);

	GString& append(size_t count, wchar_t ch = 0);
	GString& append(const GString& str);
	GString& append(const GString& str, size_t count);
	GString& append(const wchar_t* str);
	GString& append(const wchar_t* str, size_t count);
	GString& append(const_iterator _begin, const_iterator _end);

	GString& assign(const GString& str);
	GString& assign(const GString& str, size_t count);
	GString& assign(const wchar_t* str);
	GString& assign(const wchar_t* str, size_t count);
	GString& assign(const_iterator _begin, const_iterator _end);

	GString& insert(size_t offset, const GString& str);
	GString& insert(size_t offset, const wchar_t* str);
	GString& insert(size_t offset, const wchar_t* str, size_t _size);
	GString& insert(size_t offset, size_t count, wchar_t ch);

	GString& erase(size_t offset, size_t count = 1);
	GString& eraseRange(size_t _start, size_t _end);
	GString& remove(size_t offset, size_t count = 1);
	GString& removeRange(size_t _start, size_t _end);

	GString& replace(size_t offset, size_t count, const GString& replacement);
	GString& replace(size_t offset, size_t count, const wchar_t* replacement);
	GString& replace(const_iterator _begin, const_iterator _end, const GString& replacement);
	GString& replace(const_iterator _begin, const_iterator _end, const wchar_t* replacement);
	GString& replaceAll(const GString& str, const GString& replacement);
	GString& replaceAll(const wchar_t* str, const wchar_t* replacement);

	GString& replaceRegex(const GRegex& re, const GString& replacement);
	GString& replaceRegex(const GRegex& re, const wchar_t* replacement);
	GString substr(size_t offset, size_t count = npos) const;
	GString substrRange(size_t _start, size_t _end) const;
	GString slice(size_t _start, size_t _end) const;
	GString slice(size_t _start) const;

	std::vector<GString> split(const GString& str, size_t maxSize = npos) const;
	std::vector<GString> split(const wchar_t* str, size_t maxSize = npos) const;
	std::vector<GString> splitRegex(const GRegex& re, size_t maxSize = npos) const;

	size_t find(const GString& str, size_t offset = 0, bool caseSentive = true) const;
	size_t findFirst(const GString& str, size_t offset = 0, bool caseSentive = true) const;
	size_t findFirstNot(const GString& str, size_t offset = 0, bool caseSentive = true) const;
	size_t findLast(const GString& str, size_t offset = npos, bool caseSentive = true) const;
	size_t findLastNot(const GString& str, size_t offset = npos, bool caseSentive = true) const;

	size_t find(const wchar_t* str, size_t offset = 0, bool caseSentive = true) const;
	size_t findFirst(const wchar_t* str, size_t offset = 0, bool caseSentive = true) const;
	size_t findFirstNot(const wchar_t* str, size_t offset = 0, bool caseSentive = true) const;
	size_t findLast(const wchar_t* str, size_t offset = npos, bool caseSentive = true) const;
	size_t findLastNot(const wchar_t* str, size_t offset = npos, bool caseSentive = true) const;

	bool startsWith(const GString& str, bool caseSendtive = true) const;
	bool endsWith(const GString& str, bool caseSendtive = true) const;
	GString& addStartsWith(const GString& str, bool caseSendtive = true);
	GString& addEndsWith(const GString& str, bool caseSendtive = true);
	GString& removeStartsWith(const GString& str, bool caseSendtive = true);
	GString& removeEndsWith(const GString& str, bool caseSendtive = true);

	bool startsWith(const wchar_t* str, bool caseSendtive = true) const;
	bool endsWith(const wchar_t* str, bool caseSendtive = true) const;
	GString& addStartsWith(const wchar_t* str, bool caseSendtive = true);
	GString& addEndsWith(const wchar_t* str, bool caseSendtive = true);
	GString& removeStartsWith(const wchar_t* str, bool caseSendtive = true);
	GString& removeEndsWith(const wchar_t* str, bool caseSendtive = true);

	bool endsWithSperator() const;
	GString& addEndsWithSperator();
	GString& removeEndsWithSperator();

	bool startsWithSperator() const;
	GString& addStartsWithSperator();
	GString& removeStartsWithSperator();

	GString& toUpper();
	GString& toLower();

	GString& trimFront();
	GString& trimBack();
	GString& trim();

	int toInt() const;
	__int64 toInt64() const;
	int toHex() const;
	__int64 toHex64() const;
	double toDouble() const;

	size_t hashCode() const;
	GString clone() const;

	friend class GRegex;
private:
	std::wstring m_string;
};

template<>
struct std::hash<GString>
{
	size_t operator()(const GString& str) const
	{
		return str.hashCode();
	}
};
#endif
#ifndef __GSTRING_H__
#define __GSTRING_H__
#include <string>
#include <vector>

class GRegex;
class GStringPiece;

class GString
{
public:
	typedef typename std::wstring StdString;
	typedef typename std::wstring::value_type char_type;
	typedef typename std::wstring::value_type value_type;
	typedef typename std::wstring::pointer pointer;
	typedef typename std::wstring::const_pointer const_pointer;
	typedef typename std::wstring::reference reference;
	typedef typename std::wstring::const_reference const_reference;
	typedef typename std::wstring::iterator iterator;
	typedef typename std::wstring::const_iterator const_iterator;
	typedef typename std::wstring::reverse_iterator reverse_iterator;
	typedef typename std::wstring::const_reverse_iterator const_reverse_iterator;
	typedef typename std::wstring::size_type size_type;

	static const size_t npos = std::wstring::npos;

	static GString FromAnsi(const char* str);
	static GString FromUtf8(const char* str);

	static GString FromAnsi(const char* str, int bytes);
	static GString FromUtf8(const char* str, int bytes);

	GString();
	GString(const GString& str);
	GString(GString&& str);
	GString(const wchar_t* str);
	GString(const std::wstring& str);
	GString(std::wstring&& str);
	GString(const GStringPiece& piece);

	const GString& operator=(std::wstring&& str);
	const GString& operator=(const GString& str);
	const GString& operator=(GString&& str);
	const GString& operator=(const GStringPiece& piece);

	bool operator==(const GString& str) const;
	bool operator==(const GStringPiece& str) const;
	bool operator==(const wchar_t* str) const;
	bool operator!=(const GStringPiece& str) const;
	bool operator>(const GStringPiece& str) const;
	bool operator<(const GStringPiece& str) const;
	bool operator>=(const GStringPiece& str) const;
	bool operator<=(const GStringPiece& str) const;


	const GString& operator+=(const GStringPiece& piece);
	GString operator+(const GStringPiece& piece) const;

	const wchar_t* data() const;
	const wchar_t* c_str() const;
	GString& resize(size_t size, wchar_t ch = 0);
	GString& reserve(size_t size);
	GString& clear();

	size_t size() const;
	size_t capacity() const;
	bool empty() const;

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
	std::wstring& getStdString();
	const std::wstring& getStdString() const;

	int compare(const GStringPiece& str, bool ignoreCase = false) const;
	bool compareWith(const GStringPiece& str, bool ignoreCase = false) const;

	wchar_t& front();
	wchar_t front() const;
	wchar_t& back();
	wchar_t back() const;
	wchar_t at(size_t index) const;
	wchar_t& at(size_t index);
	wchar_t operator[](size_t index) const;
	wchar_t& operator[](size_t index);

	wchar_t pop_back();
	GString& push_back(wchar_t ch);


	GString& append(size_t count, wchar_t ch = 0);
	GString& append(const GStringPiece& str);
	GString& append(const GStringPiece& str, size_t count);
	GString& assign(const GStringPiece& str);
	GString& assign(const GStringPiece& str, size_t count);

	GString& insert(size_t offset, const GStringPiece& str);
	GString& insert(size_t offset, size_t count, wchar_t ch);

	GString& erase(size_t offset, size_t count = 1);
	GString& eraseRange(size_t _start, size_t _end);
	GString& remove(size_t offset, size_t count = 1);
	GString& removeRange(size_t _start, size_t _end);

	GString& replace(size_t offset, size_t count, const GStringPiece& replacement);
	GString& replaceAll(const GStringPiece& str, const GStringPiece& replacement);
	GString& replaceRegex(const GRegex& re, const GString& replacement);
	GString substr(size_t offset, size_t count = npos) const;
	GString substrRange(size_t _start, size_t _end) const;
	GString slice(size_t _start, size_t _end) const;

	std::vector<GString> split(const GString& str, size_t maxSize = npos) const;
	std::vector<GString> split(const wchar_t* str, size_t maxSize = npos) const;
	std::vector<GString> splitRegex(const GRegex& re, size_t maxSize = npos) const;

	size_t find(const GStringPiece& str, size_t offset = 0, bool caseSentive = true) const;
	size_t findFirst(const GStringPiece& str, size_t offset = 0, bool caseSentive = true) const;
	size_t findFirstNot(const GStringPiece& str, size_t offset = 0, bool caseSentive = true) const;
	size_t findLast(const GStringPiece& str, size_t offset = npos, bool caseSentive = true) const;
	size_t findLastNot(const GStringPiece& str, size_t offset = npos, bool caseSentive = true) const;

	bool startsWith(const GStringPiece& str, bool ignoreCase = false) const;
	bool endsWith(const GStringPiece& str, bool ignoreCase = false) const;

	GString& addStartsWith(const GStringPiece& str, bool ignoreCase = false);
	GString& addEndsWith(const GStringPiece& str, bool ignoreCase = false);

	GString& removeStartsWith(const GStringPiece& str, bool ignoreCase = false);
	GString& removeEndsWith(const GStringPiece& str, bool ignoreCase = false);

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
	double toDouble() const;

	friend class GRegex;
private:
	std::wstring m_string;
};

template<>
struct std::hash<GString>
{
	size_t operator()(const GString& str) const
	{
		return std::hash<std::wstring>()(str.getStdString());
	}
};
#endif
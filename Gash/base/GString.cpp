#include "GString.h"
#include "GRegex.h"

static void AssignStringWithEncoding(std::wstring& wstr, const char* str, size_t size, GEncoding encoding)
{
	wchar_t buf[2048];

	const char* ptr = str;
	if(size == -1)
		size = strlen(str);
	wchar_t* ptr2 = buf;
	size_t bufsize = sizeof(buf);
	Encoder* pEncoder = FindEncoder(encoding, EncodingUtf16LE);
	if (pEncoder)
	{
		while (size != 0 && !EncodeIsCritical(EncodeString(pEncoder, (const void**)&ptr, &size, (void**)&ptr2, &bufsize)))
		{
			wstr.append(buf, (sizeof(buf) - bufsize) >> 1);
			ptr2 = buf;
			bufsize = sizeof(buf);
		}
	}
}

static std::string ConvertEncoding(const std::wstring& wstr, GEncoding to = EncodingGBK)
{
	std::string str;
	str.reserve(wstr.size());
	char buf[4096] = {};
	Encoder* pEncoder = FindEncoder(EncodingUtf16LE, to);
	const void* pData = wstr.data();
	size_t srcsize = wstr.size() << 1;
	size_t bufsize = sizeof(buf);
	char* ptr = buf;
	if (pEncoder)
	{
		while (srcsize != 0 && !EncodeIsCritical(EncodeString(pEncoder, &pData, &srcsize, (void**)&ptr, &bufsize)))
		{
			str.append(buf, sizeof(buf) - bufsize);
			ptr = buf;
			bufsize = sizeof(buf);
		}
	}
	return std::move(str);
}

static int CompareString(const wchar_t* lbegin, const wchar_t* lend, const wchar_t* rbegin, const wchar_t* rend, bool caseSentive = false)
{
	if(caseSentive)
	{
		while (lbegin != lend && rbegin != rend && *lbegin == *rbegin)
		{
			++lbegin;
			++rbegin;
		}
		if (*lbegin > *rbegin)
			return 1;
		else if (*lbegin < *rbegin)
			return -1;
		return 0;
	}
	while (lbegin != lend && rbegin != rend && ::tolower(*lbegin) == ::tolower(*rbegin))
	{
		++lbegin;
		++rbegin;
	}
	if (::tolower(*lbegin) > ::tolower(*rbegin))
		return 1;
	else if (::tolower(*lbegin) < ::tolower(*rbegin))
		return -1;
	return 0;
}

GString GString::FromEncoding(const char* str, GEncoding encoding /*= EncodingAnsi*/)
{
	return GString(str, encoding);
}

GString GString::FromEncoding(const char* str, size_t size, GEncoding encoding /*= EncodingAnsi*/)
{
	return GString(str, size, encoding);
}

GString GString::FromStringPiece(const_iterator _begin, const_iterator _end)
{
	return GString(_begin, _end);
}

GString::GString()
{

}

GString::GString(const char* str, GEncoding encoding /*= EncodingAnsi*/)
{
	AssignStringWithEncoding(m_string, str, -1, encoding);
}

GString::GString(const char* str, size_t size, GEncoding encoding /*= EncodingAnsi*/)
{
	AssignStringWithEncoding(m_string, str, size, encoding);
}

GString::GString(const wchar_t* str)
	:m_string(str)
{

}

GString::GString(const wchar_t* str, size_t size)
	: m_string(str, size)
{

}

GString::GString(const GString& str)
	:m_string(str.m_string)
{

}

GString::GString(GString&& str)
	: m_string(std::move(str.m_string))
{

}

GString::GString(const_iterator _begin, const_iterator _end)
	:m_string(_begin, _end)
{

}

const GString& GString::operator+=(const wchar_t* str)
{
	m_string += str;
	return *this;
}

const GString& GString::operator+=(const GString& str)
{
	m_string += str.m_string;
	return *this;
}

GString GString::operator+(const GString& str) const
{
	GString tmp = *this;
	return tmp += str;
}

GString GString::operator+(const wchar_t* str) const
{
	GString tmp = *this;
	return tmp += str;
}

bool GString::operator>(const GString& str) const
{
	return m_string.compare(str.m_string) > 0;
}

bool GString::operator>(const wchar_t* str) const
{
	return m_string.compare(str) > 0;
}

bool GString::operator>=(const GString& str) const
{
	return m_string.compare(str.m_string) >= 0;
}

bool GString::operator>=(const wchar_t* str) const
{
	return m_string.compare(str) >= 0;
}

bool GString::operator<(const GString& str) const
{
	return m_string.compare(str.m_string) < 0;
}

bool GString::operator<(const wchar_t* str) const
{
	return m_string.compare(str) < 0;
}

bool GString::operator<=(const GString& str) const
{
	return m_string.compare(str.m_string) <= 0;
}

bool GString::operator<=(const wchar_t* str) const
{
	return m_string.compare(str) <= 0;
}

const wchar_t* GString::data() const
{
	return m_string.data();
}

const wchar_t* GString::c_str() const
{
	return m_string.c_str();
}

GString& GString::resize(size_t size, wchar_t ch /*= 0*/)
{
	m_string.resize(size, ch);
	return *this;
}

GString& GString::reserve(size_t size)
{
	m_string.reserve(size);
	return *this;
}

GString& GString::clear()
{
	m_string.clear();
	return *this;
}

size_t GString::size() const
{
	return m_string.size();
}

size_t GString::capacity() const
{
	return m_string.capacity();
}

bool GString::empty() const
{
	return m_string.empty();
}

bool GString::isEmptyOrBlank() const
{
	if (empty())
		return true;
	for (auto elem: m_string)
	{
		if (!::isblank(elem))
			return false;
	}
	return true;
}

GString::iterator GString::begin()
{
	return m_string.begin();
}

GString::const_iterator GString::begin() const
{
	return m_string.begin();
}

GString::iterator GString::end()
{
	return m_string.end();
}

GString::const_iterator GString::end() const
{
	return m_string.end();
}

GString::reverse_iterator GString::rbegin()
{
	return m_string.rbegin();
}

GString::const_reverse_iterator GString::rbegin() const
{
	return m_string.rbegin();
}

GString::reverse_iterator GString::rend()
{
	return m_string.rend();
}

GString::const_reverse_iterator GString::rend() const
{
	return m_string.rend();
}

std::string GString::toAnsi() const
{
	return ConvertEncoding(m_string, GEncoding::EncodingAnsi);
}

std::string GString::toUtf8() const
{
	return ConvertEncoding(m_string, GEncoding::EncodingUtf8);
}

const std::wstring& GString::toUnicode() const
{
	return m_string;
}

std::wstring& GString::toUnicode()
{
	return m_string;
}

int GString::compare(const GString& str, bool caseSentive /*= true*/) const
{
	return CompareString(m_string.data(), m_string.data() + m_string.size(), str.data(), str.data() + str.size(), caseSentive);
}

bool GString::compareWith(const GString& str, bool caseSentive /*= true*/) const
{
	return CompareString(m_string.data(), m_string.data() + m_string.size(), str.data(), str.data() + str.size(), caseSentive) == 0;
}

wchar_t& GString::front()
{
	return m_string.front();
}

const wchar_t& GString::front() const
{
	return m_string.front();
}

wchar_t& GString::back()
{
	return m_string.back();
}

const wchar_t& GString::back() const
{
	return m_string.back();
}

const wchar_t& GString::at(size_t index) const
{
	return m_string.at(index);
}

wchar_t& GString::at(size_t index)
{
	return m_string.at(index);
}

const wchar_t& GString::operator[](size_t index) const
{
	return m_string[index];
}

wchar_t& GString::operator[](size_t index)
{
	return m_string[index];
}

wchar_t GString::pop_back()
{
	wchar_t ch = m_string.back();
	m_string.pop_back();
	return ch;
}

GString& GString::push_back(wchar_t ch)
{
	m_string.push_back(ch);
	return *this;
}

GString& GString::append(size_t count, wchar_t ch /*= 0*/)
{
	m_string.append(count, ch);
	return *this;
}

GString& GString::append(const GString& str)
{
	m_string.append(str.m_string);
	return *this;
}

GString& GString::append(const GString& str, size_t count)
{
	m_string.append(str.m_string, count);
	return *this;
}

GString& GString::append(const wchar_t* str)
{
	m_string.append(str);
	return *this;
}

GString& GString::append(const wchar_t* str, size_t count)
{
	m_string.append(str, count);
	return *this;
}

GString& GString::append(const_iterator _begin, const_iterator _end)
{
	m_string.append(_begin, _end);
	return *this;
}

GString& GString::assign(const GString& str)
{
	m_string.assign(str.m_string);
	return *this;
}

GString& GString::assign(const GString& str, size_t count)
{
	m_string.assign(str.m_string, count);
	return *this;
}

GString& GString::assign(const wchar_t* str)
{
	m_string.assign(str);
	return *this;
}

GString& GString::assign(const wchar_t* str, size_t count)
{
	m_string.assign(str, count);
	return *this;
}

GString& GString::assign(const_iterator _begin, const_iterator _end)
{
	m_string.assign(_begin, _end);
	return *this;
}

GString& GString::insert(size_t offset, const GString& str)
{
	m_string.insert(offset, str.m_string);
	return *this;
}

GString& GString::insert(size_t offset, const wchar_t* str)
{
	m_string.insert(offset, str);
	return *this;
}

GString& GString::insert(size_t offset, const wchar_t* str, size_t _size)
{
	m_string.insert(offset, str, _size);
	return *this;
}

GString& GString::insert(size_t offset, size_t count, wchar_t ch)
{
	m_string.insert(offset, count, ch);
	return *this;
}

GString& GString::erase(size_t offset, size_t count /*= 1*/)
{
	m_string.erase(offset, count);
	return *this;
}

GString& GString::eraseRange(size_t _start, size_t _end)
{
	m_string.erase(_start, _end - _start);
	return *this;
}

GString& GString::remove(size_t offset, size_t count /*= 1*/)
{
	return erase(offset, count);
}

GString& GString::removeRange(size_t _start, size_t _end)
{
	return eraseRange(_start, _end);
}

GString& GString::replace(size_t offset, size_t count, const GString& replacement)
{
	m_string.replace(offset, count, replacement.m_string);
	return *this;
}

GString& GString::replace(size_t offset, size_t count, const wchar_t* replacement)
{
	m_string.replace(offset, count, replacement);
	return *this;
}

GString& GString::replace(const_iterator _begin, const_iterator _end, const wchar_t* replacement)
{
	m_string.replace(_begin, _end, replacement);
	return *this;
}

GString& GString::replace(const_iterator _begin, const_iterator _end, const GString& replacement)
{
	m_string.replace(_begin, _end, replacement.m_string);
	return *this;
}

GString& GString::replaceAll(const GString& str, const GString& replacement)
{
	size_t findindex = 0;
	while ((findindex = m_string.find(str.m_string, findindex)) != npos)
	{
		m_string.replace(findindex, str.size(), replacement.m_string);
		++findindex;
	}
	return *this;
}

GString& GString::replaceAll(const wchar_t* str, const wchar_t* replacement)
{
	size_t findindex = 0;
	size_t len = wcslen(str);
	while ((findindex = m_string.find(str, findindex)) != npos)
	{
		m_string.replace(findindex, len, replacement);
		++findindex;
	}
	return *this;
}

GString& GString::replaceRegex(const GRegex& re, const GString& replacement)
{
	return replaceRegex(re, replacement.data());
}

GString& GString::replaceRegex(const GRegex& re, const wchar_t* replacement)
{
	typedef std::pair<size_t, size_t> EraseRange;
	typedef std::vector<EraseRange> EraseRangeList;
	EraseRangeList list;
	for (auto itor = re.begin(*this); itor != re.end(); ++itor)
		list.push_back(EraseRange(itor->first - begin(), itor->length()));
	for (size_t i = list.size(); i > 0; --i)
	{
		const EraseRange& range = list.at(i - 1);
		m_string.replace(range.first, range.second, replacement);
	}
	return *this;
}

GString GString::substr(size_t offset, size_t count /*= npos*/) const
{
	GString tmp;
	tmp.m_string = m_string.substr(offset, count);
	return std::move(tmp);
}

GString GString::substrRange(size_t _start, size_t _end) const
{
	return substr(_start, _end - _start);
}

GString GString::slice(size_t _start, size_t _end) const
{
	return substr(_start, _end - _start);
}

GString GString::slice(size_t _start) const
{
	return substr(_start);
}

std::vector<GString> GString::split(const GString& str, size_t maxSize /*= npos*/) const
{
	return split(str.data(), maxSize);
}

std::vector<GString> GString::split(const wchar_t* str, size_t maxSize /*= npos*/) const
{
	std::vector<GString> strs;
	if (empty())
		return std::move(strs);

	size_t len = wcslen(str);
	size_t preindex = 0;
	size_t findindex = 0;
	while ((findindex = find(str, preindex)) != npos && strs.size() < maxSize)
	{
		if (preindex == findindex)
		{
			preindex = findindex + len;
			continue;
		}
		strs.push_back(slice(preindex, findindex));
		preindex = findindex + len;
	}
	if (preindex < m_string.size())
	{
		strs.push_back(slice(preindex));
	}
	return std::move(strs);
}

std::vector<GString> GString::splitRegex(const GRegex& re, size_t maxSize /*= npos*/) const
{
	return std::vector<GString>();
}

size_t GString::find(const GString& str, size_t offset /*= 0*/, bool caseSentive /*= true*/) const
{
	return find(str.data(), offset, caseSentive);
}

size_t GString::find(const wchar_t* str, size_t offset /*= 0*/, bool caseSentive /*= true*/) const
{
	size_t len = wcslen(str);
	size_t srclen = this->size();
	if (srclen < len)
		return npos;
	for (size_t i = offset; i <= srclen - len; ++i)
	{
		if (CompareString(
			this->data() + i, this->data() + i + len - 1,
			str, str + len - 1,
			caseSentive) == 0)
			return i;
	}
	return -1;
}

size_t GString::findFirst(const GString& str, size_t offset /*= 0*/, bool caseSentive /*= true*/) const
{
	return find(str, offset, caseSentive);
}

size_t GString::findFirst(const wchar_t* str, size_t offset /*= 0*/, bool caseSentive /*= true*/) const
{
	return find(str, offset, caseSentive);
}

size_t GString::findFirstNot(const GString& str, size_t offset /*= 0*/, bool caseSentive /*= true*/) const
{
	return findFirstNot(str.data(), offset, caseSentive);
}

size_t GString::findFirstNot(const wchar_t* str, size_t offset /*= 0*/, bool caseSentive /*= true*/) const
{
	size_t len = wcslen(str);
	size_t srclen = this->size();
	if (srclen < len)
		return npos;
	for (size_t i = offset; i <= srclen - len; ++i)
	{
		if (CompareString(
			this->data() + i, this->data() + i + len - 1,
			str, str + len - 1,
			caseSentive) != 0)
			return i;
	}
	return -1;
}

size_t GString::findLast(const GString& str, size_t offset /*= npos*/, bool caseSentive /*= true*/) const
{
	return findLast(str.data(), offset, caseSentive);
}

size_t GString::findLast(const wchar_t* str, size_t offset /*= npos*/, bool caseSentive /*= true*/) const
{
	size_t len = wcslen(str);
	size_t srclen = this->size();
	if (srclen < len)
		return npos;

	if (offset >= srclen)
		offset = srclen - len;
	for (size_t i = offset; i != npos; --i)
	{
		if (CompareString(
			this->data() + i, this->data() + i + len - 1,
			str, str + len - 1,
			caseSentive) == 0)
			return i;
	}
	return -1;
}

size_t GString::findLastNot(const GString& str, size_t offset /*= npos*/, bool caseSentive /*= true*/) const
{
	return findLastNot(str.data(), offset, caseSentive);
}

size_t GString::findLastNot(const wchar_t* str, size_t offset /*= npos*/, bool caseSentive /*= true*/) const
{
	size_t len = wcslen(str);
	size_t srclen = this->size();
	if (srclen < len)
		return npos;

	if (offset >= srclen)
		offset = srclen - len;
	for (size_t i = offset; i != npos; --i)
	{
		if (CompareString(
			this->data() + i, this->data() + i + len - 1,
			str, str + len - 1,
			caseSentive) != 0)
			return i;
	}
	return -1;
}

bool GString::startsWith(const GString& str, bool caseSentive /*= true*/) const
{
	return startsWith(str.data(), caseSentive);
}

bool GString::startsWith(const wchar_t* str, bool caseSentive /*= true*/) const
{
	size_t len = wcslen(str);
	size_t srclen = m_string.size();
	if (srclen < len)
		return false;
	return CompareString(m_string.data(), m_string.data() + srclen - 1, str, str + len - 1, caseSentive) == 0;
}

bool GString::endsWith(const GString& str, bool caseSentive /*= true*/) const
{
	return endsWith(str.data(), caseSentive);
}

bool GString::endsWith(const wchar_t* str, bool caseSentive /*= true*/) const
{
	size_t len = wcslen(str);
	size_t srclen = m_string.size();
	if (srclen < len)
		return false;
	return CompareString(m_string.data() + srclen - len, m_string.data() + srclen - 1, str, str + len - 1, caseSentive) == 0;
}

GString& GString::addStartsWith(const GString& str, bool caseSentive /*= true*/)
{
	if (!startsWith(str, caseSentive))
		m_string.insert(0, str.m_string);
	return *this;
}

GString& GString::addStartsWith(const wchar_t* str, bool caseSentive /*= true*/)
{
	if (!startsWith(str, caseSentive))
		m_string.insert(0, str);
	return *this;
}

GString& GString::addEndsWith(const GString& str, bool caseSentive /*= true*/)
{
	if (!endsWith(str, caseSentive))
		m_string.append(str.m_string);
	return *this;
}

GString& GString::addEndsWith(const wchar_t* str, bool caseSentive /*= true*/)
{
	if (!endsWith(str, caseSentive))
		m_string.append(str);
	return *this;
}

GString& GString::removeStartsWith(const GString& str, bool caseSentive /*= true*/)
{
	if (startsWith(str, caseSentive))
		m_string.erase(0, str.size());
	return *this;
}

GString& GString::removeStartsWith(const wchar_t* str, bool caseSentive /*= true*/)
{
	if (startsWith(str, caseSentive))
		m_string.erase(0, wcslen(str));
	return *this;
}

GString& GString::removeEndsWith(const GString& str, bool caseSentive /*= true*/)
{
	if (endsWith(str, caseSentive))
		m_string.erase(this->size() - str.size(), str.size());
	return *this;
}

GString& GString::removeEndsWith(const wchar_t* str, bool caseSentive /*= true*/)
{
	size_t len = wcslen(str);
	if (endsWith(str, caseSentive))
		m_string.erase(this->size() - len, len);
	return *this;
}

bool GString::endsWithSperator() const
{
	if (empty())
		return false;
	return m_string.back() == '/' || m_string.back() == '\\';
}

GString& GString::addEndsWithSperator()
{
	if (!endsWithSperator())
		m_string.push_back('\\');
	return *this;
}

GString& GString::removeEndsWithSperator()
{
	if (endsWithSperator())
		m_string.pop_back();
	return *this;
}

bool GString::startsWithSperator() const
{
	if (empty())
		return false;
	return m_string.front() == '/' || m_string.front() == '\\';
}

GString& GString::addStartsWithSperator()
{
	if (!startsWithSperator())
		m_string.insert(0, 1, '\\');
	return *this;
}

GString& GString::removeStartsWithSperator()
{
	if (startsWithSperator())
		m_string.erase(0);
	return *this;
}

GString& GString::toUpper()
{
	for (auto itor = m_string.begin(); itor != m_string.end(); ++itor)
		*itor = ::toupper(*itor);
	return *this;
}

GString& GString::toLower()
{
	for (auto itor = m_string.begin(); itor != m_string.end(); ++itor)
		*itor = ::tolower(*itor);
	return *this;
}

GString& GString::trimFront()
{
	size_t count = 0;
	auto itor = m_string.begin();
	for (; itor != m_string.end() && ::isblank(*itor); ++itor) {}
	count = itor - m_string.begin();
	if(count != 0)
		m_string.erase(0, count);
	return *this;
}

GString& GString::trimBack()
{
	while (!empty() && ::isblank(m_string.back()))
		m_string.pop_back();
	return *this;
}

GString& GString::trim()
{
	trimBack();
	return trimFront();
}

int GString::toInt() const
{
	return ::_wtoi(m_string.data());
}

__int64 GString::toInt64() const
{
	return ::_wtoll(m_string.data());
}

int GString::toHex() const
{
	wchar_t* ptr;
	return (int)wcstol(m_string.data(), &ptr, 16);
}

__int64 GString::toHex64() const
{
	wchar_t* ptr;
	return (__int64)wcstoll(m_string.data(), &ptr, 16);
}

double GString::toDouble() const
{
	return ::_wtof(m_string.data());
}

size_t GString::hashCode() const
{
	return std::hash<std::wstring>()(m_string);
}

GString GString::clone() const
{
	return *this;
}

bool GString::isIp() const
{
	return isIpV4() || isIpV6();
}

bool GString::isIpV4() const
{
	static std::wregex re = std::wregex(LR"(^(\d{1,3}\.){3}\d{1,3}$)");
	if (m_string.size() < 7)
		return false;
	return std::regex_match(m_string, re);
}

bool GString::isIpV6() const
{
	static std::wregex re = std::wregex(LR"(^([\da-zA-Z]{1,4}:){7}[\da-zA-Z]{1,4}$)");
	if (m_string.size() < 16)
		return false;
	return std::regex_match(m_string, re);
}

bool GString::operator==(const wchar_t* str) const
{
	return m_string == str;
}

bool GString::operator==(const GString& str) const
{
	return m_string == str.m_string;
}

const GString& GString::operator=(const char* str)
{
	return *this = FromEncoding(str);
}

const GString& GString::operator=(const wchar_t* str)
{
	m_string = str;
	return *this;
}

const GString& GString::operator=(GString&& str)
{
	m_string = std::move(str.m_string);
	return *this;
}

const GString& GString::operator=(const GString& str)
{
	m_string = str.m_string;
	return *this;
}

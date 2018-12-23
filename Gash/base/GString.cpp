#include "GString.h"
#include <Windows.h>
#include "GStringPiece.h"
#include "GRegex.h"

namespace
{
	GString ConverToGString(DWORD codePage, const char* str, int len = -1)
	{
		GString tmp;
		if (len == -1)
			len = strlen(str);
		int wlen = MultiByteToWideChar(codePage, 0, str, len, 0, 0);
		if (wlen <= 0)
			return std::move(tmp);
		tmp.resize(wlen);
		MultiByteToWideChar(codePage, 0, str, len, (wchar_t*)tmp.data(), wlen);
		return std::move(tmp);
	}
}

GString GString::FromAnsi(const char* str)
{
	return ConverToGString(CP_ACP, str);
}

GString GString::FromAnsi(const char* str, int bytes)
{
	return ConverToGString(CP_ACP, str, bytes);
}

GString GString::FromUtf8(const char* str)
{
	return ConverToGString(CP_UTF8, str);
}

GString GString::FromUtf8(const char* str, int bytes)
{
	return ConverToGString(CP_UTF8, str, bytes);
}

GString::GString()
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

GString::GString(const wchar_t* str)
	: m_string(str)
{

}

GString::GString(const std::wstring& str)
	: m_string(str)
{

}

GString::GString(std::wstring&& str)
	: m_string(std::move(str))
{

}

GString::GString(const GStringPiece& piece)
{
	*this = piece.toString();
}

const GString& GString::operator=(std::wstring&& str)
{
	m_string = std::move(str);
	return *this;
}

const GString& GString::operator=(const GStringPiece& piece)
{
	return assign(piece);
}

bool GString::operator==(const GStringPiece& str) const
{
	return GStringPiece(m_string) == str;
}
bool GString::operator!=(const GStringPiece& str) const
{
	return GStringPiece(m_string) != str;
}

const GString& GString::operator=(const GString& str)
{
	m_string = str.m_string;
	return *this;
}

bool GString::operator==(const GString& str) const
{
	return m_string == str.m_string;
}

bool GString::operator==(const wchar_t* str) const
{
	return operator==(GStringPiece(str));
}

bool GString::operator>(const GStringPiece& str) const
{
	return GStringPiece(m_string) > str;
}
bool GString::operator<(const GStringPiece& str) const
{
	return GStringPiece(m_string) < str;
}
bool GString::operator>=(const GStringPiece& str) const
{
	return GStringPiece(m_string) >= str;
}
bool GString::operator<=(const GStringPiece& str) const
{
	return GStringPiece(m_string) <= str;
}

const GString& GString::operator+=(const GStringPiece& piece)
{
	return append(piece);
}

GString GString::operator+(const GStringPiece& piece) const
{
	GString str = *this;
	str += piece;
	return std::move(str);
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
	std::string str;
	int len = WideCharToMultiByte(CP_ACP, 0, m_string.data(), (int)m_string.size(), 0, 0, 0, 0);
	if (len < 0)
		return str;
	str.resize(len);
	WideCharToMultiByte(CP_ACP, 0, m_string.data(), (int)m_string.size(), (char*)str.data(), (int)str.length(), 0, 0);
	return std::move(str);
}

std::string GString::toUtf8() const
{
	std::string str;
	int len = WideCharToMultiByte(CP_UTF8, 0, m_string.data(), (int)m_string.size(), 0, 0, 0, 0);
	if (len < 0)
		return str;
	str.resize(len);
	WideCharToMultiByte(CP_UTF8, 0, m_string.data(), (int)m_string.size(), (char*)str.data(), (int)str.length(), 0, 0);
	return std::move(str);
}

std::wstring& GString::getStdString()
{
	return m_string;
}

const std::wstring& GString::getStdString() const
{
	return m_string;
}

int GString::compare(const GStringPiece& str, bool ignoreCase /*= false*/) const
{
	return GStringPiece(m_string).compare(str, ignoreCase);
}

bool GString::compareWith(const GStringPiece& str, bool ignoreCase /*= false*/) const
{
	return GStringPiece(m_string).compareWith(str, ignoreCase);
}

wchar_t& GString::front()
{
	return m_string.front();
}

wchar_t GString::front() const
{
	return m_string.front();
}

wchar_t& GString::back()
{
	return m_string.back();
}

wchar_t GString::back() const
{
	return m_string.back();
}

wchar_t GString::at(size_t index) const
{
	return m_string.at(index);
}

wchar_t& GString::at(size_t index)
{
	return m_string.at(index);
}

wchar_t GString::operator[](size_t index) const
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

GString& GString::append(const GStringPiece& str)
{
	size_t appendSize = str.size();
	reserve(appendSize + this->size());
	
	for (auto itor = str.begin(); itor != str.end(); ++itor)
		m_string.push_back(*itor);

	return *this;
}

GString& GString::append(const GStringPiece& str, size_t count)
{
	return append(str.substr(0, count));
}

GString& GString::append(size_t count, wchar_t ch /*= 0*/)
{
	m_string.append(count, ch);
	return *this;
}

GString& GString::assign(const GStringPiece& str)
{
	*this = str.toString();
	return *this;
}

GString& GString::assign(const GStringPiece& str, size_t count)
{
	return assign(str.substr(0, count));
}

GString& GString::insert(size_t offset, const GStringPiece& str)
{
	if (offset >= this->size())
		offset = this->size() - 1;
	m_string.insert(offset, 0, str.size());
	for (size_t i = 0; i < str.size(); ++i)
		m_string[i + offset] = str[i];
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

GString& GString::replace(size_t offset, size_t count, const GStringPiece& replacement)
{
	if (offset >= size())
		return *this;
	size_t _size = replacement.size();
	m_string.replace(offset, count, _size, 0);
	for (size_t i = 0; i < _size; ++i)
		m_string[i + offset] = replacement[i];
	return *this;
}

GString& GString::replaceAll(const GStringPiece& str, const GStringPiece& replacement)
{
	size_t offset = find(str);
	while (offset != npos)
	{
		replace(offset, str.size(), replacement);
		offset = find(str, offset + replacement.size());
	}
	return *this;
}

GString& GString::replaceRegex(const GRegex& re, const GString& replacement)
{
	re.replace(*this, replacement);
	return *this;
}

GString GString::substr(size_t offset, size_t count /*= npos*/) const
{
	return m_string.substr(offset, count);
}

GString GString::substrRange(size_t _start, size_t _end) const
{
	if (_end == npos)
		return substr(_start);
	return substr(_start, _end - _start);
}

GString GString::slice(size_t _start, size_t _end) const
{
	return substrRange(_start, _end);
}

std::vector<GString> GString::split(const GString& str, size_t maxSize /*= npos*/) const
{
	std::vector<GString> strs;
	size_t offset = m_string.find(str.m_string);
	size_t preOffset = 0;
	while (offset != npos && strs.size() < maxSize)
	{
		GString tmp = substrRange(preOffset, offset);
		if (!tmp.empty())
			strs.push_back(std::move(tmp));
		preOffset = offset + str.size();
		offset = m_string.find(str.m_string, preOffset);
	}
	if (strs.size() < maxSize)
	{
		GString tmp = substrRange(preOffset, offset);
		if (!tmp.empty())
			strs.push_back(std::move(tmp));
	}
	return std::move(strs);
}

std::vector<GString> GString::split(const wchar_t* str, size_t maxSize /*= npos*/) const
{
	size_t strLen = wcslen(str);
	std::vector<GString> strs;
	size_t offset = m_string.find(str);
	size_t preOffset = 0;
	while (offset != npos && strs.size() < maxSize)
	{
		GString tmp = substrRange(preOffset, offset);
		if (!tmp.empty())
			strs.push_back(std::move(tmp));
		preOffset = offset + strLen;
		offset = m_string.find(str, preOffset);
	}
	if (strs.size() < maxSize)
	{
		GString tmp = substrRange(preOffset, offset);
		if (!tmp.empty())
			strs.push_back(std::move(tmp));
	}
	return std::move(strs);
}

std::vector<GString> GString::splitRegex(const GRegex& re, size_t maxSize /*= npos*/) const
{
	std::vector<GString> strs;
	auto strItor = begin();
	for (auto itor = re.begin(*this); itor != re.end() && strs.size() < maxSize; ++itor)
	{
		GString str;
		str.m_string = std::wstring(strItor, itor->first);
		strItor = itor->second;
		if(str.empty())
			continue;
		strs.push_back(std::move(str));
	}

	return std::move(strs);
}

size_t GString::find(const GStringPiece& str, size_t offset /*= 0*/, bool caseSentive /*= true*/) const
{
	return findFirst(str, offset, caseSentive);
}

size_t GString::findFirst(const GStringPiece& str, size_t offset /*= 0*/, bool caseSentive /*= true*/) const
{
	return GStringPiece(m_string).find(str, offset, caseSentive);
}

size_t GString::findFirstNot(const GStringPiece& str, size_t offset /*= 0*/, bool caseSentive /*= true*/) const
{
	return GStringPiece(m_string).findFirstNot(str, caseSentive);
}

size_t GString::findLast(const GStringPiece& str, size_t offset /*= npos*/, bool caseSentive /*= true*/) const
{
	return GStringPiece(m_string).findLast(str, offset, caseSentive);
}

size_t GString::findLastNot(const GStringPiece& str, size_t offset /*= npos*/, bool caseSentive /*= true*/) const
{
	return GStringPiece(m_string).findLastNot(str, offset, caseSentive);
}

bool GString::startsWith(const GStringPiece& str, bool ignoreCase /*= false*/) const
{
	return GStringPiece(m_string).startsWith(str, ignoreCase);
}

bool GString::endsWith(const GStringPiece& str, bool ignoreCase /*= false*/) const
{
	return GStringPiece(m_string).endsWith(str, ignoreCase);
}

GString& GString::addStartsWith(const GStringPiece& str, bool ignoreCase /*= false*/)
{
	if (!startsWith(str, ignoreCase))
		str.insert(*this, 0);
	return *this;
}

GString& GString::addEndsWith(const GStringPiece& str, bool ignoreCase /*= false*/)
{
	if (!endsWith(str, ignoreCase))
		this->append(str);
	return *this;
}

GString& GString::removeStartsWith(const GStringPiece& str, bool ignoreCase /*= false*/)
{
	if (startsWith(str, ignoreCase))
		m_string.erase(0, str.size());
	return *this;
}

GString& GString::removeEndsWith(const GStringPiece& str, bool ignoreCase /*= false*/)
{
	if (endsWith(str, ignoreCase))
		m_string.erase(this->size() - str.size(), str.size());
	return *this;
}

bool GString::endsWithSperator() const
{
	if (m_string.empty())
		return false;
	return m_string.back() == L'\\' || m_string.back() == L'/';
}

GString& GString::addEndsWithSperator()
{
	if (!endsWithSperator())
		m_string.push_back(L'/');
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
	return front() == '/' || front() == '\\';
}

GString& GString::addStartsWithSperator()
{
	if (!startsWithSperator())
		m_string.insert(0, L"/");
	return *this;
}

GString& GString::removeStartsWithSperator()
{
	if (startsWithSperator())
		m_string.erase(m_string.begin());
	return *this;
}

GString& GString::toUpper()
{
	for (auto itor = begin(); itor != end(); ++itor)
		*itor = (wchar_t)::toupper(*itor);
	return *this;
}

GString& GString::toLower()
{
	for (auto itor = begin(); itor != end(); ++itor)
		*itor = (wchar_t)::tolower(*itor);
	return *this;
}

GString& GString::trimFront()
{
	for (size_t i = 0; i < m_string.size(); ++i)
	{
		if (!::isblank(m_string[i]))
		{
			if(i == 0)
				break;
			m_string = m_string.substr(i);
			break;
		}
	}
	return *this;
}

GString& GString::trimBack()
{
	if (empty())
		return *this;
	while (::isblank(m_string.back()))
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
	return ::_wtoi64(m_string.data());
}

double GString::toDouble() const
{
	return ::_wtof(m_string.data());
}

const GString& GString::operator=(GString&& str)
{
	m_string = std::move(str.m_string);
	return *this;
}
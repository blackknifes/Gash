#include "GPath.h"
#include "GRegex.h"
#include <Windows.h>

#define GPROTOCOL_FILE L"file://"
#define GPROTOCOL_FILE_SIZE 7

static GRegex s_normalize = L"[/\\\\]+";

static GString& NormalizePath(GString& path)
{
	path.replaceRegex(s_normalize, L"\\");

	auto strs = path.split(L"\\");
	auto itor = strs.begin();
	while (itor != strs.end())
	{
		if (*itor == L"..")
		{
			if (itor == strs.begin() + 1)
				itor = strs.erase(itor);
			else
			{
				itor = strs.erase(--itor);
				itor = strs.erase(itor);
			}
			continue;
		}
		else if (*itor == L".")
		{
			itor = strs.erase(itor);
			continue;
		}
		++itor;
	}
	path.clear();
	for (itor = strs.begin(); itor != strs.end(); ++itor)
	{
		path += *itor;
		path.push_back('\\');
	}
	path.removeEndsWithSperator();

	return path;
}

static bool IsPathSeperator(int ch)
{
	return ch == '/' || ch == '\\';
}

GPath::GPath()
{

}

GPath::GPath(const GString& str)
{
	parse(str);
}

bool GPath::isValid() const
{
	return !m_strPath.empty();
}

void GPath::parse(const GString& str)
{
	GString tmp = str;
	parse(std::move(tmp));
}

void GPath::parse(GString&& str)
{
	static GRegex regex = L"([\\w]:)?([\\\\/]([\\s\\S]+[\\\\/])?)?([^\\\\/\\.]+)?(\\..*)?";
	{
		if (str.startsWith(GPROTOCOL_FILE, true))
		{
			str.remove(0, GPROTOCOL_FILE_SIZE);
			if (str.startsWith(L"/"))
				str.remove(0);
		}
		m_strPath = std::move(str);
	}
	auto results = regex.match(m_strPath);
	if (results.size() != 6)
	{
		m_strPath.clear();
		return;
	}

	m_strDriver = GRegex::StringPiece(results[1]);
	m_strDir = GRegex::Merge(results[1], results[2]);
	m_strFilename = GRegex::StringPiece(results[4]);
	m_strExtname = GRegex::StringPiece(results[5]);
	m_strBasename = GRegex::Merge(results[4], results[5]);
}

void GPath::inspect() const
{
	wprintf_s(L"path: %s\n", m_strPath.data());
	wprintf_s(L"driver: %s\n", m_strDriver.toString().data());
	wprintf_s(L"dir: %s\n", m_strDir.toString().data());
	wprintf_s(L"basename: %s\n", m_strBasename.toString().data());
	wprintf_s(L"filename: %s\n", m_strFilename.toString().data());
	wprintf_s(L"extname: %s\n", m_strExtname.toString().data());
}

bool GPath::isAbsolutePath() const
{
	return !m_strDriver.empty();
}

GPath GPath::getAbsolutePath() const
{
	if (isAbsolutePath())
		return *this;
	wchar_t buf[MAX_PATH] = {};
	if(GetCurrentDirectoryW(MAX_PATH, buf) == FALSE)
		return GPath();
	GString tmp = buf;
	tmp.addEndsWithSperator();
	tmp += m_strPath;
	return GPath(tmp);
}

const GString& GPath::getPath() const
{
	return m_strPath;
}

GString GPath::getDriver() const
{
	return m_strDriver.toString();
}

GString GPath::getDir() const
{
	return m_strDir.toString();
}

GString GPath::getBasename() const
{
	return m_strBasename.toString();
}

GString GPath::getFilename() const
{
	return m_strFilename.toString();
}

GString GPath::getExtname() const
{
	return m_strExtname.toString();
}

GPath& GPath::normalize()
{
	if (!isValid())
		return *this;
	NormalizePath(m_strPath);
	return *this;
}

GString GPath::Resolve(const wchar_t* str, ...)
{
	GString buf = str;
	buf.reserve(MAX_PATH);
	buf.addEndsWithSperator();
	va_list ap;
	va_start(ap, str);
	while ((str = va_arg(ap, const wchar_t*)) != nullptr)
	{
		if (IsPathSeperator(str[0]))
			str = str + 1;
		buf.append(str);
	}
	va_end(ap);

	if (buf.size() < 2 || buf[1] != ':')
	{
		wchar_t workDir[MAX_PATH];
		GetCurrentDirectoryW(MAX_PATH, workDir);
		buf = GString(workDir).addEndsWithSperator() + buf.removeStartsWithSperator();
	}
	NormalizePath(buf);
	return std::move(buf);
}

GString GPath::Resolve(const char* str, ...)
{
	GString buf = GString::FromEncoding(str);
	buf.reserve(MAX_PATH);
	buf.addEndsWithSperator();
	va_list ap;
	va_start(ap, str);
	while ((str = va_arg(ap, const char*)) != nullptr)
	{
		if (IsPathSeperator(str[0]))
			str = str + 1;
		buf.append(GString::FromEncoding(str));
	}
	va_end(ap);

	if (buf.size() < 2 || buf[1] != ':')
	{
		wchar_t workDir[MAX_PATH];
		GetCurrentDirectoryW(MAX_PATH, workDir);
		buf = GString(workDir).addEndsWithSperator() + buf.removeStartsWithSperator();
	}
	NormalizePath(buf);

	return std::move(buf);
}

GString GPath::GetWorkDir()
{
	wchar_t buf[1024];
	GetCurrentDirectoryW(1024, buf);
	return buf;
}

GString GPath::GetExePath()
{
	wchar_t buf[MAX_PATH];
	GetModuleFileNameW(GetModuleHandle(nullptr), buf, MAX_PATH);
	return buf;
}

GString GPath::GetExeDir()
{
	wchar_t buf[MAX_PATH];
	GetModuleFileNameW(GetModuleHandle(nullptr), buf, MAX_PATH);
	return Resolve(buf, L"..", nullptr);
}

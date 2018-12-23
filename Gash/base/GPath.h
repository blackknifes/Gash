#ifndef __GPATH_H__
#define __GPATH_H__
#include "GStringPiece.h"
#include "GString.h"

class GPath
{
public:
	static GString Resolve(const wchar_t* str, ...);
	static GString Resolve(const char* str, ...);
	static GString GetWorkDir();
	static GString GetExePath();
	static GString GetExeDir();
	GPath();
	GPath(const GString& str);

	bool isValid() const;

	void parse(const GString& str);
	void parse(GString&& str);
	void inspect() const;

	bool isAbsolutePath() const;
	GPath getAbsolutePath() const;
	const GString& getPath() const;
	GString getDriver() const;
	GString getDir() const;
	GString getBasename() const;
	GString getFilename() const;
	GString getExtname() const;
	GPath& normalize();
	

private:
	GString m_strPath;
	GStringPiece m_strDriver;
	GStringPiece m_strDir;
	GStringPiece m_strBasename;
	GStringPiece m_strFilename;
	GStringPiece m_strExtname;
};
#endif
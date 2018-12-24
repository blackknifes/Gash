#include "GShell.h"
#include <ShlObj.h>
#include "GStringPiece.h"
#include <shellapi.h>

#pragma comment(lib, "Shell32.lib")
#pragma warning(disable: 4311)
#pragma warning(disable: 4302)
#pragma warning(disable: 4090)

static int GetPathClassId(GDirectoryType type)
{
	switch (type)
	{
	case GShell::UserAdminTools:
		return CSIDL_ADMINTOOLS;
	case GShell::UserAltStartup:
		return CSIDL_ALTSTARTUP;
	case GShell::UserBitBucket:
		return CSIDL_BITBUCKET;
	case GShell::UserCDBurnArea:
		return CSIDL_CDBURN_AREA;
	case GShell::AllUserAdminTools:
		return CSIDL_COMMON_ADMINTOOLS;
	case GShell::AllUserAltStartup:
		return CSIDL_COMMON_ALTSTARTUP;
	case GShell::AllUserAppData:
		return CSIDL_COMMON_APPDATA;
	case GShell::AllUserDesktop:
		return CSIDL_COMMON_DESKTOPDIRECTORY;
	case GShell::AllUserDocuments:
		return CSIDL_COMMON_DOCUMENTS;
	case GShell::AllUserMusic:
		return CSIDL_COMMON_MUSIC;
	case GShell::AllUserOemLinks:
		return CSIDL_COMMON_OEM_LINKS;
	case GShell::AllUserPictures:
		return CSIDL_COMMON_PICTURES;
	case GShell::AllUserPrograms:
		return CSIDL_COMMON_PROGRAMS;
	case GShell::AllUserStartMenu:
		return CSIDL_COMMON_STARTMENU;
	case GShell::AllUserStartup:
		return CSIDL_COMMON_STARTUP;
	case GShell::AllUserTemplates:
		return CSIDL_COMMON_TEMPLATES;
	case GShell::AllUserVideo:
		return CSIDL_COMMON_VIDEO;
	case GShell::ComputersNearMe:
		return CSIDL_COMPUTERSNEARME;
	case GShell::Connections:
		return CSIDL_CONNECTIONS;
	case GShell::Controls:
		return CSIDL_CONTROLS;
	case GShell::Cookies:
		return CSIDL_COOKIES;
	case GShell::MyComputer:
		return CSIDL_DRIVES;
	case GShell::Favorites:
		return CSIDL_FAVORITES;
	case GShell::Fonts:
		return CSIDL_FONTS;
	case GShell::History:
		return CSIDL_HISTORY;
	case GShell::Internet:
		return CSIDL_INTERNET;
	case GShell::InternetCache:
		return CSIDL_INTERNET_CACHE;
	case GShell::LocalAppData:
		return CSIDL_LOCAL_APPDATA;
	case GShell::MyDocuments:
		return CSIDL_MYDOCUMENTS;
	case GShell::MyMusic:
		return CSIDL_MYMUSIC;
	case GShell::MyPictures:
		return CSIDL_MYPICTURES;
	case GShell::MyVideo:
		return CSIDL_MYVIDEO;
	case GShell::NetHood:
		return CSIDL_NETHOOD;
	case GShell::Network:
		return CSIDL_NETWORK;
	case GShell::PrintHood:
		return CSIDL_PRINTHOOD;
	case GShell::Profile:
		return CSIDL_PROFILE;
	case GShell::ProgramFiles:
		return CSIDL_PROGRAM_FILES;
	case GShell::ProgramFilesX86:
		return CSIDL_PROGRAM_FILESX86;
	case GShell::ProgramFilesCommon:
		return CSIDL_PROGRAM_FILES_COMMON;
	case GShell::ProgramFilesCommonX86:
		return CSIDL_PROGRAM_FILES_COMMONX86;
	case GShell::Recent:
		return CSIDL_RECENT;
	case GShell::Resources:
		return CSIDL_RESOURCES;
	case GShell::ResourcesLocalized:
		return CSIDL_RESOURCES_LOCALIZED;
	case GShell::SendTo:
		return CSIDL_SENDTO;
	case GShell::System:
		return CSIDL_SYSTEM;
	case GShell::SystemX86:
		return CSIDL_SYSTEMX86;
	case GShell::Windows:
		return CSIDL_WINDOWS;
	default:
		break;
	}
	return -1;
}

GString GShell::GetPath(DirectoryType type)
{
	wchar_t buf[MAX_PATH] = {};
	HRESULT result;
	if ((result = SHGetFolderPathW(nullptr, GetPathClassId(type), nullptr, SHGFP_TYPE_CURRENT, buf)) != S_OK)
	{
		SetLastError(result);
		return L"";
	}
	return buf;
}

bool GShell::SetPath(DirectoryType type, const GString& str)
{
	wchar_t buf[MAX_PATH] = {};
	HRESULT result;
	if ((result = SHSetFolderPathW(GetPathClassId(type), nullptr, SHGFP_TYPE_CURRENT, buf)) != S_OK)
	{
		SetLastError(result);
		return false;
	}
	return true;
}

GString GShell::GetDefaultPath(DirectoryType type)
{
	wchar_t buf[MAX_PATH] = {};
	HRESULT result;
	if ((result = SHGetFolderPathW(nullptr, GetPathClassId(type), nullptr, SHGFP_TYPE_DEFAULT, buf)) != S_OK)
	{
		SetLastError(result);
		return L"";
	}
	return buf;
}

bool GShell::OpenFile(const GString& str)
{
	DWORD errCode = (DWORD)::ShellExecuteW(nullptr, L"open", str.data(), 0, 0, 1);
	if(errCode <= 32)
		SetLastError(errCode);
	return errCode > 32;
}

void GShell::OpenFileFolder(const GString& str)
{
	ITEMIDLIST *pidl;
	if (str.find(L"/") != -1)
	{
		GString tmp = str;
		tmp.replaceAll(L"/", L"\\");
		pidl = ILCreateFromPathW(tmp.data());
	}
	else
		pidl = ILCreateFromPathW(str.data());
	if (pidl)
	{
		SHOpenFolderAndSelectItems(pidl, 0, 0, 0);
		ILFree(pidl);
	}
}

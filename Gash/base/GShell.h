#ifndef __GSHELL_H__
#define __GSHELL_H__
#include "GString.h"
#include <Windows.h>

class GShell
{
public:
	enum DirectoryType
	{
		UserAdminTools,
		UserAltStartup,
		UserBitBucket,
		UserCDBurnArea,

		AllUserAdminTools,
		AllUserAltStartup,
		AllUserAppData,
		AllUserDesktop,
		AllUserDocuments,
		AllUserMusic,
		AllUserOemLinks,
		AllUserPictures,
		AllUserPrograms,
		AllUserStartMenu,
		AllUserStartup,
		AllUserTemplates,
		AllUserVideo,
		ComputersNearMe,
		Connections,
		Controls,
		Cookies,
		MyComputer,
		Favorites,
		Fonts,
		History,
		Internet,
		InternetCache,
		LocalAppData,
		MyDocuments,
		MyMusic,
		MyPictures,
		MyVideo,
		NetHood,
		Network,
		PrintHood,
		Profile,
		ProgramFiles,
		ProgramFilesX86,
		ProgramFilesCommon,
		ProgramFilesCommonX86,
		Recent,
		Resources,
		ResourcesLocalized,
		SendTo,
		System,
		SystemX86,
		Windows
	};
	static GString GetPath(DirectoryType type);
	static bool SetPath(DirectoryType type, const GString& str);
	static GString GetDefaultPath(DirectoryType type);

	static bool OpenFile(const GString& str);
	static void OpenFileFolder(const GString& str);
};

typedef GShell::DirectoryType GDirectoryType;
#endif
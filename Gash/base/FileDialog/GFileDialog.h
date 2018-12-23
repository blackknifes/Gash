#ifndef __GFILEDIALOG_H__
#define __GFILEDIALOG_H__
#include <Windows.h>
#pragma comment(lib, "Shlwapi.lib")

typedef struct
{
	HINSTANCE hInstance;
	const wchar_t* strDefaultPath;
	const wchar_t* strTitle;
	const wchar_t* strFilter;
	unsigned char bMultil;
	unsigned char bHideReadonly;
	unsigned char bOpenFolder;
	unsigned char bIsSave;
	unsigned char bOverwritePromp;
} OpenFileDialogOptions, *LPOpenFileDialogOptions;

BOOL OpenFileDialog(LPOpenFileDialogOptions options, wchar_t* buf, size_t buflen);
#endif
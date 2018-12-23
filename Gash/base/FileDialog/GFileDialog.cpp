#include "GFileDialog.h"
#include <CommCtrl.h>
#include <locale.h>
#include <shlwapi.h>

#pragma warning(disable: 4267)

#define  ID_COMBO_ADDR 0x47c
#define  ID_LEFT_TOOBAR 0x4A0

static UINT_PTR CALLBACK ParentFolderProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	WNDPROC wndProc = (WNDPROC)GetPropW(hwnd, L"ParentProc");
	switch (uMsg)
	{
	case WM_COMMAND:
	{
		if (wParam == IDOK)
		{
			wchar_t wcDirPath[MAX_PATH] = { 0 };
			HWND hComboAddr = GetDlgItem(hwnd, ID_COMBO_ADDR);
			if (hComboAddr != NULL)
				GetWindowTextW(hComboAddr, wcDirPath, MAX_PATH);
			if (!wcslen(wcDirPath))
				break;
			DWORD dwAttr = GetFileAttributesW(wcDirPath);
			if (dwAttr != -1 && (FILE_ATTRIBUTE_DIRECTORY & dwAttr))
			{
				LPOPENFILENAMEW oFn = (LPOPENFILENAMEW)GetPropW(hwnd, L"OPENFILENAME");
				if (oFn)
				{
					int size = oFn->nMaxFile > MAX_PATH ? MAX_PATH : oFn->nMaxFile;
					memcpy(oFn->lpstrFile, wcDirPath, size * sizeof(wchar_t));
					RemovePropW(hwnd, L"OPENFILENAME");
					EndDialog(hwnd, 1);
				}
				else
					EndDialog(hwnd, 0);
			}
			break;
		}
		//////////////////////////////////////////////////////////////////////////
		//如果是左边toolbar发出的WM_COMMOND消息（即点击左边的toolbar）, 则清空OK按钮旁的组合框。
		HWND hCtrl = (HWND)lParam;
		if (hCtrl == NULL)
			break;
		int ctrlId = GetDlgCtrlID(hCtrl);
		if (ctrlId == ID_LEFT_TOOBAR)
		{
			HWND hComboAddr = GetDlgItem(hwnd, ID_COMBO_ADDR);
			if (hComboAddr != NULL)
				SetWindowTextW(hComboAddr, L"");
		}
	}
	break;
	}
	return CallWindowProc((WNDPROC)wndProc, hwnd, uMsg, wParam, lParam);
}

static void CenterTopWindow(HWND hWnd)
{
	HWND hParent = GetParent(hWnd);
	while ((hParent = GetParent(hWnd)) != nullptr)
		hWnd = hParent;
	RECT wndRc;
	RECT workArea;
	SystemParametersInfo(SPI_GETWORKAREA, 0, &workArea, 0);
	GetWindowRect(hWnd, &wndRc);
	int x = ((workArea.right - workArea.left) >> 1) - ((wndRc.right - wndRc.left) >> 1);
	int y = ((workArea.bottom - workArea.top) >> 1) - ((wndRc.bottom - wndRc.top) >> 1);
	SetWindowPos(hWnd, nullptr, x, y, 0, 0, SWP_NOZORDER | SWP_NOACTIVATE | SWP_NOSIZE);
}

static UINT_PTR CALLBACK FolderProc(HWND hdlg, UINT uiMsg, WPARAM wParam, LPARAM lParam)
{
	if (uiMsg == WM_NOTIFY)
	{
		LPOFNOTIFY lpOfNotify = (LPOFNOTIFY)lParam;
		switch (lpOfNotify->hdr.code)
		{
		case CDN_INITDONE:
		{
			CenterTopWindow(hdlg);
			HWND hParent = GetParent(hdlg);
			SetPropW(hParent, L"OPENFILENAME", (HANDLE)(lpOfNotify->lpOFN));
			SetPropW(hParent, L"ParentProc", (HANDLE)GetWindowLongPtr(hParent, GWLP_WNDPROC));
			SetWindowLongPtrW(GetParent(hdlg), GWLP_WNDPROC, (LONG_PTR)ParentFolderProc);
		}
		break;
		case CDN_FOLDERCHANGE:
		{
			wchar_t wcDirPath[MAX_PATH] = { 0 };
			CommDlg_OpenSave_GetFilePathW(GetParent(hdlg), wcDirPath, sizeof(wcDirPath));
			HWND hComboAddr = GetDlgItem(GetParent(hdlg), ID_COMBO_ADDR);
			if (hComboAddr != NULL)
			{
				if (wcslen(wcDirPath) > 0)
				{
					//去掉文件夹快捷方式的后缀名。
					int pathSize = wcslen(wcDirPath);
					if (pathSize >= 4)
					{
						wchar_t* wcExtension = PathFindExtensionW(wcDirPath);
						if (wcslen(wcExtension))
						{
							wcExtension = CharLowerW(wcExtension);
							if (!wcscmp(wcExtension, L".lnk"))
							{
								wcDirPath[pathSize - 4] = L'\0';
							}
						}
					}
					SetWindowTextW(hComboAddr, wcDirPath);
				}
				else
					SetWindowTextW(hComboAddr, L"");
			}
			break;
		}
		default:
			break;
		}
	}
	return 0;
}
static BOOL OpenFolderDialog(wchar_t* pData, size_t buflen, HINSTANCE hInstance)
{
	OPENFILENAMEW openFile;
	memset(&openFile, 0, sizeof(openFile));
	openFile.lStructSize = sizeof(openFile);

	OPENFILENAME openFileName = { 0 };
	openFile.lStructSize = sizeof(OPENFILENAME);
	openFile.nMaxFile = MAX_PATH;
	openFile.lpstrTitle = L"选择文件夹";
	openFile.lpstrFilter = L"文件夹\0..\0\0";
	openFile.lpstrFile = pData;
	openFile.nMaxFile = buflen;
	openFile.nFilterIndex = 1;
	openFile.Flags = OFN_EXPLORER | OFN_ENABLEHOOK;
	openFile.hInstance = hInstance;
	openFile.lpfnHook = FolderProc;
	return GetOpenFileNameW(&openFile);
}

BOOL OpenFileDialog(LPOpenFileDialogOptions options, wchar_t* buf, size_t buflen)
{
	if (options->hInstance == nullptr)
		options->hInstance = GetModuleHandle(nullptr);
	if (options->bOpenFolder)
		return OpenFolderDialog(buf, buflen, options->hInstance);
	OPENFILENAMEW ofn = {};
	ofn.lStructSize = sizeof(ofn);
	ofn.Flags = OFN_EXPLORER;
	if (options->bHideReadonly)
		ofn.Flags |= OFN_HIDEREADONLY;
	if (options->bMultil)
		ofn.Flags |= OFN_ALLOWMULTISELECT;
	if (options->bOverwritePromp)
		ofn.Flags |= OFN_OVERWRITEPROMPT;

	if (options->strFilter)
		ofn.lpstrFilter = options->strFilter;

	if (options->strDefaultPath)
		ofn.lpstrInitialDir = options->strDefaultPath;
	if (options->strTitle)
		ofn.lpstrTitle = options->strTitle;
	ofn.lpstrFile = buf;
	ofn.nMaxFile = buflen;

	if (options->bIsSave)
		return GetSaveFileNameW(&ofn);
	return GetOpenFileNameW(&ofn);
}

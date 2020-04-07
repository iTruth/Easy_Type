#define UNICODE

#include <stdio.h>
#include <string.h>
#include <wchar.h>
#include <conio.h>
#include <windows.h>

#define MAX_TEXT_LENTH 256

HHOOK _hook = NULL;
FILE *_fp = NULL;
int _text_index = 0;
wchar_t _szText[32767] = {0};
BOOL isFirstTime = TRUE;
BOOL isOpenNow = FALSE;

void readAll(wchar_t *strDest, FILE *fp)
{
	wchar_t *dest_cpy = strDest;
	while((*strDest++ = fgetwc(fp))|1 && !feof(fp) && !ferror(fp));
	*--strDest = '\0';
	wchar_t _display_text[550] = L"分割线下是读取到的文本的前500个字符,如果出现了乱码\n请将该文本文件保存为UTF-8格式并按F4重新载入文件\n--------------------------------------------------\n";
	int _info_len = wcslen(_display_text);
	int _display_text_len = _info_len>500?500:_info_len;
	for(int i=0; i<_display_text_len; ++i){
		_display_text[i+_info_len] = *dest_cpy++;
	}
	MessageBox(NULL, _display_text, L"注意", MB_OK|MB_ICONINFORMATION);
}

void reSetFile()
{
	isFirstTime = TRUE;
	isOpenNow = FALSE;
}

BOOL GetCourseFileName(HWND hwndOwner, wchar_t *szFile, DWORD nMaxFile)
{
	wchar_t szFileTitle[MAX_TEXT_LENTH] = {0};
	OPENFILENAME ofn;
	memset(&ofn,0,sizeof(ofn));
	ofn.lStructSize=sizeof(ofn);
	ofn.hwndOwner=hwndOwner;
	ofn.lpstrFilter=L"自定义课程文本文件(*.*)\0*.*;\0\0";
	ofn.nFilterIndex=1;
	ofn.lpstrTitle=L"打开[金山打字通 2016]自定义课程文本文件";
	ofn.lpstrFile=szFile;
	ofn.nMaxFile=nMaxFile;
	ofn.lpstrFileTitle=szFileTitle;
	ofn.nMaxFileTitle=sizeof(szFileTitle);
	ofn.Flags=OFN_PATHMUSTEXIST|OFN_FILEMUSTEXIST|OFN_EXPLORER;
	return GetOpenFileName(&ofn);
}

__declspec(dllexport) LRESULT funcCallback(int nCode, WPARAM wParam, LPARAM lParam)
{
	if (nCode < 0) // do not process message 
		return CallNextHookEx(_hook, nCode, wParam, lParam);
	PMSG pMsg=(PMSG)lParam;
	if(isFirstTime && !isOpenNow){
		isOpenNow = TRUE;
		wchar_t szFile[MAX_TEXT_LENTH] = {0};
		// 打开文件
		if(GetCourseFileName(pMsg->hwnd, szFile, sizeof(szFile))){
			_fp = _wfopen(szFile, L"rt+,ccs=UTF-8");
			if(_fp == NULL){
				//MessageBox(NULL, "Failed to open file", "Error", MB_OK|MB_ICONERROR);
				isOpenNow = FALSE;
				return CallNextHookEx(_hook, nCode, wParam, lParam);
			}
		} else {
			//MessageBox(NULL, "Please choice a file", "Error", MB_OK|MB_ICONERROR);
			isOpenNow = FALSE;
			return CallNextHookEx(_hook, nCode, wParam, lParam);
		}
		readAll(_szText, _fp);
		fclose(_fp);
		_text_index = 0;
		isFirstTime = FALSE;
		isOpenNow = FALSE;
	}
	// 功能按键
	if(pMsg->message == WM_KEYDOWN){
		if(pMsg->wParam == VK_F1){
			_text_index = 0;
			return CallNextHookEx(_hook, nCode, wParam, lParam);
		}
		if(pMsg->wParam == VK_F2){
			if(_text_index >= 2){
				_text_index -= 2;
			}
			return CallNextHookEx(_hook, nCode, wParam, lParam);
		}
		if(pMsg->wParam == VK_F3){
			_text_index += 2;
			return CallNextHookEx(_hook, nCode, wParam, lParam);
		}
		if(pMsg->wParam == VK_F4 && !isOpenNow){
			reSetFile();
			return CallNextHookEx(_hook, nCode, wParam, lParam);
		}
	}
	if(pMsg->message == WM_CHAR)
	{
		if(pMsg->wParam == VK_BACK){
			if(_text_index != 0)
				--_text_index;
			return CallNextHookEx(_hook, nCode, wParam, lParam);
		}
		pMsg->wParam = _szText[_text_index/2];
		++_text_index;
		PostMessage(pMsg->hwnd, WM_KEYDOWN, wParam, lParam);
		pMsg->message = WM_NULL;
		return 0;
	}
	return CallNextHookEx(_hook, nCode, wParam, lParam);
}

__declspec(dllexport) void setHook(HHOOK hook)
{
	_hook = hook;
}

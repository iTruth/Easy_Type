#include <stdio.h>
#include <conio.h>
#include <windows.h>

typedef void(*SetHookProc)(HHOOK);
const char *title = "金山打字通 2016";
char errorText[256] = {0};

#define FailLog(T) printf("[失败]\n");\
	MessageBox(NULL, T, "错误", MB_OK|MB_ICONERROR);
#define ErrorTextLog FailLog(errorText);

HWND GetConsoleHwnd(void)
{
#define MY_BUFSIZE 1024 // Buffer size for console window titles.
	HWND hwndFound;         // This is what is returned to the caller.
	char pszNewWindowTitle[MY_BUFSIZE]; // Contains fabricated
	// WindowTitle.
	char pszOldWindowTitle[MY_BUFSIZE]; // Contains original
	// WindowTitle.
	// Fetch current window title.
	GetConsoleTitle(pszOldWindowTitle, MY_BUFSIZE);
	// Format a "unique" NewWindowTitle.
	wsprintf(pszNewWindowTitle,"%d/%d",
			GetTickCount(),
			GetCurrentProcessId());
	// Change current window title.
	SetConsoleTitle(pszNewWindowTitle);
	// Ensure window title has been updated.
	Sleep(40);
	// Look for NewWindowTitle.
	hwndFound=FindWindow(NULL, pszNewWindowTitle);
	// Restore original window title.
	SetConsoleTitle(pszOldWindowTitle);
	return(hwndFound);
}

HHOOK hook = NULL;

void whenExit()
{
	if(hook != NULL){
		UnhookWindowsHookEx(hook);
	}
}

int main()
{
	SetConsoleTitle("Easy Type v1.0 by iTruth");
	atexit(whenExit);
	printf("正在加载hookdll_utf8_x32.dll...");
	HMODULE dll = LoadLibrary("hookdll_utf8_x32.dll");
	if(dll == NULL){
		sprintf(errorText, "动态链接库hookdll_utf8_x32.dll丢失\n错误代码: %ld", GetLastError());
		ErrorTextLog;
		return -1;
	}
	printf("[完成]\n");
	HOOKPROC hookCallBack = (HOOKPROC)GetProcAddress(dll, "funcCallback");
	SetHookProc setHook = (SetHookProc)GetProcAddress(dll, "setHook");

	printf("正在获取[%s]的窗口句柄...", title);
	HWND hWndDest = FindWindow(NULL, title);
	if(hWndDest == NULL){
		sprintf(errorText, "无法获取窗口句柄,可能是[%s]并未启动或者版本不对", title);
		ErrorTextLog;
		return 0;
	}
	printf("[完成]\n");

	DWORD dwThreadDest = GetWindowThreadProcessId(hWndDest, NULL);
	printf("正在向目标线程注入钩子...");
	hook = SetWindowsHookEx(WH_GETMESSAGE, hookCallBack, dll, dwThreadDest);
	if(hook == NULL){
		sprintf(errorText, "注入钩子失败,请稍后重试");
		ErrorTextLog;
		return -1;
	}
	printf("[完成]\n");
	setHook(hook);
	printf("钩子注入成功!\n\n在本窗口按任意键撤销钩子并退出\n");
	ShowWindow(GetConsoleHwnd(), SW_SHOWMINIMIZED);
	(void)getch();
	return 0;
}

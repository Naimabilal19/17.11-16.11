#include <windows.h>
#include <windowsX.h>
#include <ctime>
#include <tchar.h>
#include <commctrl.h>
#include "resource.h"

HWND hB, hBar, hEdit;
HANDLE Timer, Th;

BOOL CALLBACK DlgProc(HWND, UINT, WPARAM, LPARAM);

void Cls_OnClose(HWND hwnd)
{
	EndDialog(hwnd, 0);
}

int WINAPI _tWinMain(HINSTANCE hInstance, HINSTANCE hPrevInst, LPTSTR lpszCmdLine, int nCmdShow)
{
	return DialogBox(hInstance, MAKEINTRESOURCE(IDD_ABOUTBOX), NULL, (DLGPROC)DlgProc);
}

DWORD WINAPI Thread(LPVOID lp)
{
	DWORD pr = GetPriorityClass(GetCurrentProcess());
	SetPriorityClass(GetCurrentProcess(), REALTIME_PRIORITY_CLASS);
	SetThreadPriority(GetCurrentThread(), THREAD_PRIORITY_ABOVE_NORMAL);

    hEdit = (HWND)lp;
	while (true)
	{
		TCHAR buff[64];
		short seconds = 120;
		while (seconds != NULL)
		{
			--seconds;
			wsprintf(buff, TEXT("%d left"), seconds);
			SetWindowText(hEdit, buff);
		}
		if (seconds == NULL)
		{
			TerminateThread(Th, NULL);
			CloseHandle(Th);
			EndDialog(hWnd, NULL);
		}
		Sleep(1000);
	}
	SetThreadPriority(GetCurrentThread(), THREAD_PRIORITY_NORMAL);
		return(NULL);
}

DWORD WINAPI Tim(LPVOID lp)
{
	static time_t t;
	static TCHAR str[50];
	while (true)
	{
		t = time(NULL);
		lstrcpy(str, _tctime(&t));
		str[lstrlen(str) - 1] = '\0';
		SetWindowText(time1, str);
	}
}

BOOL Cls_OnInitDialog(HWND hwnd, HWND hwndFocus, LPARAM lParam)
{
	hB = GetDlgItem(hwnd, IDC_BUTTON1);
	time1 = GetDlgItem(hWnd, IDC_EDIT3);
	hBar = GetDlgItem(hwnd, IDC_PROGRESS1);
	SendMessage(hBar, PBM_SETRANGE, NULL, MAKELPARAM(NULL, 200));
	SendMessage(hBar, PBM_SETPOS, NULL, NULL);
	SendMessage(hBar, PBM_SETBKCOLOR, NULL, LPARAM(RGB(255, 255, 255)));
	SendMessage(hBar, PBM_SETBARCOLOR, NULL, LPARAM(RGB(NULL, 214, 120)));
	Th = CreateThread(0, 0, Thread, hTimer, 0, 0);
	Timer = CreateThread(NULL, 0, Tim, hWnd, 0, NULL);

}

void Cls_OnCommand(HWND hwnd, int id, HWND hwndCtl, UINT codeNotify)
{
	int step = 0, score = 0;

	if (IDC_RADIO1)
	{
		step++;
		score++;
	}
	else if (IDC_RADIO2)
	{
		step++;
	}
	else if (IDC_RADIO3)
	{
		step++;
	}

	if (IDC_RADIO4)
	{
		step++;
	}
	else if (IDC_RADIO5)
	{
		step++;
		score++;
	}
	else if (IDC_RADIO6)
	{
		step++;
		score++;
	}

	if (step == 2)
	{
		EnableWindow(hB, TRUE);
	}
	else
	{
		EnableWindow(hB, FALSE);
	}

	if (score == 1)
	{
		SendMessage(hBar, PBM_SETSTEP, 100, NULL);
	}
	else if (score == 2)
	{
		SendMessage(hBar, PBM_SETSTEP, 100, NULL);
	}

	if (IDC_BUTTON1)
	{
		SuspendThread(Th);
		if (score == 1)
		{
			EndDialog(hwnd, NULL);
			MessageBox(hwnd, TEXT("Good +1"), TEXT("Box"), MB_OK);
		}
		else if (score == 2)
		{
			EndDialog(hwnd, NULL);
			MessageBox(hwnd, TEXT("Nice +2"), TEXT("Box"), MB_OK);
		}
		else
		{
			EndDialog(hwnd, NULL);
			MessageBox(hwnd, TEXT("Bad 0"), TEXT("Box"), MB_OK);
		}
		TerminateThread(Th, NULL);
		CloseHandle(Th);
	}
}

BOOL CALLBACK DlgProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
		HANDLE_MSG(hwnd, WM_CLOSE, Cls_OnClose);
		HANDLE_MSG(hwnd, WM_INITDIALOG, Cls_OnInitDialog);
		HANDLE_MSG(hwnd, WM_COMMAND, Cls_OnCommand);
		return TRUE;
	}
}


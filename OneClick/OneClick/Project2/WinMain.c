#include "WinMain.h"
#include "FileIO.h"
#include "resource1.h"

HWND g_hwnd;
HWND B_hwnd[3];
HINSTANCE g_hInst;
WCHAR tempPresetPath[255];
wchar_t *tempPath = NULL;
NOTIFYICONDATA notifyIconData;
SYSTEMTIME time;
int msgboxopen[6] = { 0 };
int num;

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPWSTR lpCmdLine, int nCmdShow)
{
	InitWindow(hInstance, nCmdShow, winWidth, winHeight);
	InitNotifyIconData(g_hwnd);

	MSG Message = { 0 };

	while (GetMessage(&Message, 0, 0, 0)) {
		TranslateMessage(&Message);
		DispatchMessage(&Message);
	}
	return (int)Message.wParam;
}

int APIENTRY InitWindow(HINSTANCE hInstance, int nCmdShow, int width, int height)
{
	WNDCLASSEX wndEx;

	int xpos = GetSystemMetrics(SM_CXSCREEN);
	int ypos = GetSystemMetrics(SM_CYSCREEN);

	wndEx.cbSize = sizeof(WNDCLASSEX);
	wndEx.cbClsExtra = 0;
	wndEx.cbWndExtra = 0;
	wndEx.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	wndEx.hCursor = LoadCursor(NULL, IDC_ARROW);
	wndEx.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	wndEx.hIconSm = NULL;
	wndEx.hInstance = hInstance;
	wndEx.lpfnWndProc = WndProc;
	wndEx.lpszClassName = lpszClass;
	wndEx.lpszMenuName = NULL;
	wndEx.style = CS_HREDRAW | CS_VREDRAW;
	g_hInst = hInstance;

	RegisterClassEx(&wndEx);

	g_hwnd = CreateWindowEx(
		WS_EX_APPWINDOW,
		lpszClass,
		lpszClass,
		WS_BORDER | WS_OVERLAPPED | WS_SYSMENU | WS_MINIMIZEBOX | WS_VISIBLE,
		xpos / 2,  // x 시작점 
		ypos / 6,  // y 시작점
		width,     // 가로
		height,    // 세로
		NULL,
		(HMENU)NULL,
		hInstance,
		NULL);

	ShowWindow(g_hwnd, nCmdShow);

	return S_OK;
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT iMessage, WPARAM wParam, LPARAM lParam)
{
	if (iMessage == 0 && !IsWindowVisible(hwnd))
	{
		ShowWindow(hwnd, SW_HIDE);
		return 0;
	}
	switch (iMessage)
	{
	case WM_ACTIVATE:
		Shell_NotifyIcon(NIM_ADD, &notifyIconData);
		break;
	case WM_CREATE:
		CreateDirectory(dirPath, NULL);
		CreateButtons(hwnd);
		CreatePresetFile();
		ShowWindow(hwnd, SW_HIDE);
		Hmenu = CreatePopupMenu();
		AppendMenu(Hmenu, MF_STRING, ID_TRAY_EXIT, TEXT("종료"));
		FILE* alarm = NULL;
		fopen_s(&alarm, ".\\Preset\\Alarm.txt", "r");
		for (int i = 0; i < 6; i++) {
			fscanf_s(alarm, "%d", &alarmon[i]);
			if(alarmon[i]==1)
				fscanf_s(alarm, "%d %d", &uhour[i],&uminute[i]);
		}
		fclose(alarm);
		SetTimer(hwnd, 1, 1000,NULL);
		break;

	case WM_TIMER:
		switch (wParam) {
		case 1:
			GetLocalTime(&time);
			for (int i = 0; i < 6; i++) {
				if (time.wHour == uhour[i] && time.wMinute == uminute[i] && msgboxopen[i] == 0) {
					msgboxopen[i] = 1;
					switch (MessageBox(hwnd, L"지금 설정한 프리셋을 실행할까요?", "알람", MB_OKCANCEL)) {
					case IDOK:
						RunPreset(hwnd, dirPreset[i]);
						break;
					case IDCANCEL:
						break;
					}

				}
			}
			break;
		}
		break;
	case WM_COMMAND:
		switch (LOWORD(wParam))
		{
		case 0: // 프리셋 실행 
			if (hExcDlg == NULL)
				DialogBox(g_hInst, MAKEINTRESOURCE(IDD_DIALOG2), HWND_DESKTOP, ExecuteDlgProc);
			break;
		case 1: // 프리셋 설정 
		{
			if (hFileDlg == NULL)
				DialogBox(g_hInst, MAKEINTRESOURCE(IDD_DIALOG1), HWND_DESKTOP, EditDlgProc);
			break;
		}
		case 2: // 알람 설정 
			if (hAlarmDlg == NULL)
				DialogBox(g_hInst, MAKEINTRESOURCE(IDD_DIALOG2), HWND_DESKTOP, SelectPresetDlgProc);
			break;
		}
		switch (wParam & 0xFFF0)
		{
		case SC_MINIMIZE:
		case SC_CLOSE:
			ShowWindow(hwnd, SW_HIDE);
			return 0;
			break;
		}
		break;
		return TRUE;

	case WM_KEYDOWN:
		switch (wParam) {
		case VK_ESCAPE:
			PostQuitMessage(0);
			break;
		}
		return TRUE;

	case WM_SYSICON:
	{
		switch (wParam)
		{
		case ID_TRAY_APP_ICON:
			SetForegroundWindow(hwnd);
			break;
		}

		if (lParam == WM_LBUTTONUP)
			ShowWindow(hwnd, SW_SHOW);
		else if (lParam == WM_RBUTTONDOWN)
		{
			POINT curPoint;
			GetCursorPos(&curPoint);
			SetForegroundWindow(hwnd);

			UINT clicked = TrackPopupMenu(Hmenu, TPM_RETURNCMD | TPM_NONOTIFY, curPoint.x, curPoint.y, 0, hwnd, NULL);
			SendMessage(hwnd, WM_NULL, 0, 0);
			if (clicked == ID_TRAY_EXIT)
			{
				Shell_NotifyIcon(NIM_DELETE, &notifyIconData);
				PostQuitMessage(0);
			}
		}
	}
	break;

	case WM_NCHITTEST:
	{
		UINT uHitTest = DefWindowProc(hwnd, WM_NCHITTEST, wParam, lParam);
		if (uHitTest == HTCLIENT)
			return HTCAPTION;
		else
			return uHitTest;
	}
	case WM_CLOSE:
		ShowWindow(hwnd, SW_HIDE);
		return 0;
		break;
	case WM_DESTROY:
		KillTimer(hwnd, 1);
		PostQuitMessage(0);
		return TRUE;
	default:
		return DefWindowProc(hwnd, iMessage, wParam, lParam);
	}
	return TRUE;
}

LRESULT CALLBACK EditDlgProc(HWND hDlg, UINT iMessage, WPARAM wParam, LPARAM lParam)
{
	switch (iMessage)
	{
	case WM_INITDIALOG:
		hFileDlg = hDlg;
		hButtonFile = GetDlgItem(hDlg, IDC_BUTTON1);
		hPathEdit = GetDlgItem(hDlg, IDC_EDIT1);
		SelectPreset(hDlg, &hFileDlg, &tempPath); 
		return TRUE;

	case WM_COMMAND:
		switch (LOWORD(wParam))
		{
		case IDC_BUTTON1: // 프리셋에 원하는 프로그램 저장
		{
			wchar_t buf[1024] = { 0 };
			GetDlgItemText(hDlg, IDC_EDIT1, buf, 1024);
			if (buf[0] == '\0') // 에디트 박스에 아무 경로를 치지 않았으면
				SelectFile(&tempPath, NULL, hPathEdit); // 파일 선택창 켜짐.
			else {				// 경로가 들어있다면 
				MessageBox(NULL, L"저장 되었습니다.", L"알림", MB_OK);
				SelectFile(&tempPath, buf, hPathEdit); // 그 입력한 경로가 저장 됨.
			}

			return TRUE;
		}
		case IDC_BUTTON2: // 프리셋 파일 삭제 
			DeletePreset(&tempPath, &hFileDlg);
			return TRUE;
		}
		return TRUE;

	case WM_CLOSE:
		free(tempPath);
		tempPath = NULL;
		EndDialog(hDlg, IDOK);
		hFileDlg = NULL;
		return TRUE;
	}
	
	return 0;
}

LRESULT CALLBACK ExecuteDlgProc(HWND hDlg, UINT iMessage, WPARAM wParam, LPARAM lParam)
{
	switch (iMessage)
	{
	case WM_INITDIALOG:
		hExcDlg = hDlg;
		break;

	case WM_COMMAND: // 프리셋 버튼 클릭 이벤트 
		switch (LOWORD(wParam))
		{
		case IDC_PRESET1:
			RunPreset(hDlg, dirPreset[0]);
			break;
		case IDC_PRESET2:
			RunPreset(hDlg, dirPreset[1]);
			break;
		case IDC_PRESET3:
			RunPreset(hDlg, dirPreset[2]);
			break;
		case IDC_PRESET4:
			RunPreset(hDlg, dirPreset[3]);
			break;
		case IDC_PRESET5:
			RunPreset(hDlg, dirPreset[4]);
			break;
		case IDC_PRESET6:
			RunPreset(hDlg, dirPreset[5]);
			break;
		}
		break;

	case WM_CLOSE:
		EndDialog(hDlg, IDOK);
		hExcDlg = NULL;
		return TRUE;
	}

	return 0;
}

LRESULT CALLBACK AlarmSetDlgProc(HWND hDlg, UINT iMessage, WPARAM wParam, LPARAM lParam){
	switch (iMessage)
	{
	case WM_INITDIALOG:
		hAlarmDlg = hDlg;
		return TRUE;

	case WM_COMMAND:
		switch (LOWORD(wParam))
		{
		case IDOK: // 프리셋 알람시간 설정
		{
			HANDLE fHandle;
			static DWORD filePointer = 0;

			BOOL fError, fRelative=NULL;

			int h=0, m=0;
			h = GetDlgItemInt(hDlg,IDC_EDIT1, &fError,fRelative);
			m = GetDlgItemInt(hDlg,IDC_EDIT2, &fError, fRelative);

			FILE* alarm = NULL;
			fopen_s(&alarm, ".\\Preset\\Alarm.txt", "w");
			if (h< 24 && h >= 0 && m < 60 && m >= 0) // 시간입력을 제대로 했을때
			{
				alarmon[num] = 1;
				uhour[num] = h;
				uminute[num] = m;
				for (int i = 0; i < 6; i++) {
					if (alarmon[i] == 1)
						fprintf(alarm, "%d %d %d\n", 1, uhour[i], uminute[i]);
					else
						fprintf(alarm, "0\n");
				}
				fclose(alarm);
			}
			else{				// 시간입력을 잘못했을 때
				MessageBox(NULL, L"시간입력을 다시 해주세요.", L"알림", MB_OK);
			}

			EndDialog(hAlarmDlg, IDOK);
			hAlarmDlg = NULL;
			return TRUE;
		}
		case IDCANCEL: // 그냥 취소
			EndDialog(hAlarmDlg, IDOK);
			hAlarmDlg = NULL;
			return TRUE;
		}
		return TRUE;

	case WM_CLOSE:
		EndDialog(hAlarmDlg, IDOK);
		hAlarmDlg = NULL;
		return TRUE;
	}

	return 0;
}

LRESULT CALLBACK SelectPresetDlgProc(HWND hDlg, UINT iMessage, WPARAM wParam, LPARAM lParam) {
	switch (iMessage)
	{
	case WM_INITDIALOG:
		hSelectDlg = hDlg;
		break;

	case WM_COMMAND: // 프리셋 버튼 클릭 이벤트 
		switch (LOWORD(wParam))
		{
		case IDC_PRESET1:
			num = 0;
			DialogBox(g_hInst, MAKEINTRESOURCE(IDD_DIALOG3), HWND_DESKTOP, AlarmSetDlgProc);
			break;
		case IDC_PRESET2:
			num = 1;
			DialogBox(g_hInst, MAKEINTRESOURCE(IDD_DIALOG3), HWND_DESKTOP, AlarmSetDlgProc);
			break;
		case IDC_PRESET3:
			num = 2;
			DialogBox(g_hInst, MAKEINTRESOURCE(IDD_DIALOG3), HWND_DESKTOP, AlarmSetDlgProc);
			break;
		case IDC_PRESET4:
			num = 3;
			DialogBox(g_hInst, MAKEINTRESOURCE(IDD_DIALOG3), HWND_DESKTOP, AlarmSetDlgProc);
			break;
		case IDC_PRESET5:
			num = 4;
			DialogBox(g_hInst, MAKEINTRESOURCE(IDD_DIALOG3), HWND_DESKTOP, AlarmSetDlgProc);
			break;
		case IDC_PRESET6:
			num = 5;
			DialogBox(g_hInst, MAKEINTRESOURCE(IDD_DIALOG3), HWND_DESKTOP, AlarmSetDlgProc);
			break;
		}
		break;

	case WM_CLOSE:
		EndDialog(hDlg, IDOK);
		hExcDlg = NULL;
		return TRUE;
	}

	return 0;
}

//프리셋 알람 삭제 대화상자
LRESULT CALLBACK DelPresetDlgProc(HWND hDlg, UINT iMessage, WPARAM wParam, LPARAM lParam) {
	switch (iMessage)
	{
	case WM_INITDIALOG:
		hDelDlg = hDlg;
		break;

	case WM_COMMAND: // 프리셋 버튼 클릭 이벤트 
		switch (LOWORD(wParam))
		{
		case IDC_PRESET1:
			alarmon[0] = 0;
			uhour[0] = 24;
			uminute[0] = 60;
			break;
		case IDC_PRESET2:
			alarmon[1] = 0;
			uhour[1] = 24;
			uminute[1] = 60;
			break;
		case IDC_PRESET3:
			alarmon[2] = 0;
			uhour[2] = 24;
			uminute[2] = 60;
			break;
		case IDC_PRESET4:
			alarmon[3] = 0;
			uhour[3] = 24;
			uminute[3] = 60;
			break;
		case IDC_PRESET5:
			alarmon[4] = 0;
			uhour[4] = 24;
			uminute[4] = 60;
			break;
		case IDC_PRESET6:
			alarmon[5] = 0;
			uhour[5] = 24;
			uminute[5] = 60;
			break;
		}
		break;

	case WM_CLOSE:
		EndDialog(hDlg, IDOK);
		hExcDlg = NULL;
		FILE* alarm = NULL;
		fopen_s(&alarm, "Alarm.txt", "w");
		for (int i = 0; i < 6; i++) {
			if (alarmon[num] == 1)
				fprintf(alarm, "%d %d %d", 1, uhour[i], uminute[i]);
			else
				fprintf(alarm, "0");
		}
		return TRUE;
	}

	return 0;
}

void CreateButtons(HWND hwnd)
{
	int i;
	int buttonXstart = winWidth / 6, buttonYstart = 50;

	for (i = 0; i < 3; i++)
		B_hwnd[i] = CreateWindow(TEXT("button"), a[i], WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
			buttonXstart, buttonYstart + (100 * i), buttonWidth, buttonHeight, hwnd, (HMENU)i, g_hInst, NULL);

	return;
}

void InitNotifyIconData(HWND Hwnd)
{
	memset(&notifyIconData, 0, sizeof(NOTIFYICONDATA));

	notifyIconData.cbSize = sizeof(NOTIFYICONDATA);
	notifyIconData.hWnd = Hwnd;
	notifyIconData.uID = ID_TRAY_APP_ICON;
	notifyIconData.uFlags = NIF_ICON | NIF_MESSAGE | NIF_TIP;
	notifyIconData.uCallbackMessage = WM_SYSICON; //Set up our invented Windows Message
	notifyIconData.hIcon = (HICON)LoadIcon(GetModuleHandle(NULL), MAKEINTRESOURCE(IDI_ICON1));
	lstrcpy(notifyIconData.szTip, szTIP);
}
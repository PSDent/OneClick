#pragma once
#include <Windows.h>

////////////////////////////////////////
// 상수 
////////////////////////////////////////
#define WM_SYSICON          (WM_USER + 1)

extern HINSTANCE   g_hInst;    // 프로그램 핸들 
extern HWND         g_hwnd;      // 윈도우 핸들(Handle)
extern HWND        B_hwnd[3];   // 버튼 핸들
LPCSTR a[] = { L"프리셋 실행" , L"프리셋 설정", L"알람 설정" }; // 매인 윈도우 버튼에 출력할 문자열
LPCTSTR lpszClass = TEXT("One Click");                  // 윈도우 타이틀 바 
LPCWSTR szTIP = L"원클릭!!";

HWND hExcDlg;                        // 프리셋 실행 대화상자 핸들 
HWND hFileDlg;                       // 파일열기 대화상자의 핸들 
HWND hAlarmDlg;
HWND hSelectDlg;
HWND hDelDlg;
HWND hButtonFile;                    // 파일열기 대화상자를 실행하기 위한 버튼의 핸들
HWND hPathEdit;                      // 파일의 경로와 이름을 가져오는 에디트 컨트롤의 핸들
HMENU Hmenu;

const int winWidth = 300;              // 윈도우 가로 
const int winHeight = 400;                 // 윈도우 세로
const int buttonWidth = 200;              // 버튼 가로
const int buttonHeight = 50;                // 버튼 세로  
int uhour[6] = { 24,24,24,24,24,24 };								//알람맞춘 시
int uminute[6] = { 60,60,60,60,60,60 };							//알람맞춘 분
int alarmon[6] = { 0,0,0,0,0,0 };						//알람이 맞춰져있는지

// 윈도우 초기화
int APIENTRY InitWindow(HINSTANCE, int, int, int);

// 메인 윈도우 
int WINAPI wWinMain(HINSTANCE, HINSTANCE, LPWSTR, int);

// 윈도우 프로시저
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

// 파일 탐색 대화상자(Dialog) 
LRESULT CALLBACK EditDlgProc(HWND, UINT, WPARAM, LPARAM);

// 프리셋 실행 대화상자
LRESULT CALLBACK ExecuteDlgProc(HWND, UINT, WPARAM, LPARAM);

//프리셋 알람 설정 대화상자
LRESULT CALLBACK AlarmSetDlgProc(HWND, UINT, WPARAM, LPARAM);

//프리셋 알람 설정 대화상자
LRESULT CALLBACK SelectPresetDlgProc(HWND, UINT, WPARAM, LPARAM);

//프리셋 알람 삭제 대화상자
LRESULT CALLBACK DelPresetDlgProc(HWND, UINT, WPARAM, LPARAM);


// 버튼 생성 
void CreateButtons(HWND);

// 트레이 아이콘 생성
void InitNotifyIconData(HWND);
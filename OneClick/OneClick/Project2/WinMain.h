#pragma once
#include <Windows.h>

////////////////////////////////////////
// ��� 
////////////////////////////////////////
#define WM_SYSICON          (WM_USER + 1)

extern HINSTANCE   g_hInst;    // ���α׷� �ڵ� 
extern HWND         g_hwnd;      // ������ �ڵ�(Handle)
extern HWND        B_hwnd[3];   // ��ư �ڵ�
LPCSTR a[] = { L"������ ����" , L"������ ����", L"�˶� ����" }; // ���� ������ ��ư�� ����� ���ڿ�
LPCTSTR lpszClass = TEXT("One Click");                  // ������ Ÿ��Ʋ �� 
LPCWSTR szTIP = L"��Ŭ��!!";

HWND hExcDlg;                        // ������ ���� ��ȭ���� �ڵ� 
HWND hFileDlg;                       // ���Ͽ��� ��ȭ������ �ڵ� 
HWND hAlarmDlg;
HWND hSelectDlg;
HWND hDelDlg;
HWND hButtonFile;                    // ���Ͽ��� ��ȭ���ڸ� �����ϱ� ���� ��ư�� �ڵ�
HWND hPathEdit;                      // ������ ��ο� �̸��� �������� ����Ʈ ��Ʈ���� �ڵ�
HMENU Hmenu;

const int winWidth = 300;              // ������ ���� 
const int winHeight = 400;                 // ������ ����
const int buttonWidth = 200;              // ��ư ����
const int buttonHeight = 50;                // ��ư ����  
int uhour[6] = { 24,24,24,24,24,24 };								//�˶����� ��
int uminute[6] = { 60,60,60,60,60,60 };							//�˶����� ��
int alarmon[6] = { 0,0,0,0,0,0 };						//�˶��� �������ִ���

// ������ �ʱ�ȭ
int APIENTRY InitWindow(HINSTANCE, int, int, int);

// ���� ������ 
int WINAPI wWinMain(HINSTANCE, HINSTANCE, LPWSTR, int);

// ������ ���ν���
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

// ���� Ž�� ��ȭ����(Dialog) 
LRESULT CALLBACK EditDlgProc(HWND, UINT, WPARAM, LPARAM);

// ������ ���� ��ȭ����
LRESULT CALLBACK ExecuteDlgProc(HWND, UINT, WPARAM, LPARAM);

//������ �˶� ���� ��ȭ����
LRESULT CALLBACK AlarmSetDlgProc(HWND, UINT, WPARAM, LPARAM);

//������ �˶� ���� ��ȭ����
LRESULT CALLBACK SelectPresetDlgProc(HWND, UINT, WPARAM, LPARAM);

//������ �˶� ���� ��ȭ����
LRESULT CALLBACK DelPresetDlgProc(HWND, UINT, WPARAM, LPARAM);


// ��ư ���� 
void CreateButtons(HWND);

// Ʈ���� ������ ����
void InitNotifyIconData(HWND);
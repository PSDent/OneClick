#include "FileIO.h"

void WritePath(wchar_t *presetPath, wchar_t *filePath)
{
	HANDLE fHandle; 
	static DWORD filePointer = 0;
	unsigned short mark = 0xFEFF; // 유니코드 파일에 입력시 발생하는 공백 문제를 해결하기 위한 값. 
	BYTE byMultiLine[] = { 0x0d, 0x00, 0x0a, 0x00 };

	if (presetPath == NULL) { // 경로를 제대로 받지 못하여 NULL일 경우 오류 출력 후 종료
		MessageBox(NULL, L"Pointer is NullPtr.", L"ERROR", MB_OK);
		return;
	}

	// 프리셋 파일을 염.
	fHandle = CreateFile(presetPath, GENERIC_READ | GENERIC_WRITE, FILE_SHARE_READ, NULL, OPEN_EXISTING, 0, NULL);

    filePointer = SetFilePointer(fHandle, 0, NULL, FILE_END); // 개행한 줄의 위치 기록 
	WriteFile(fHandle, &mark, sizeof(mark), NULL, NULL);    
	WriteFile(fHandle, filePath, sizeof(LPWSTR) * wcslen(filePath), NULL, NULL);
	WriteFile(fHandle, byMultiLine, 4, NULL, NULL);
	CloseHandle(fHandle);
	
	return;
}

void RunPreset(HWND hDlg, LPCTSTR *presetPath)
{
	wchar_t buf = 0;
	wchar_t pathBuffer[4096] = { 0 };
	HANDLE fHandle;
	unsigned short mark = 0xFEFF;

	_wsetlocale(LC_ALL, "");
	FILE *fp = NULL;

	if (_wfopen_s(&fp, presetPath, L"r, ccs=UTF-16LE") != NULL) {
		MessageBox(NULL, L"파일 열기 실패", L"Error", MB_OK);
		int a = errno;
		return;
	}

	//buf = fgetwc(fp);
	fwscanf_s(fp, L"%[^\n]s", pathBuffer, 4096);
	while (buf != WEOF) {
		RunProgram(hDlg, pathBuffer);
		buf = fgetwc(fp);
		buf = fgetwc(fp);
		fwscanf_s(fp, L"%[^\n]s", pathBuffer, 4096);
	}
	
	fclose(fp);

	return;
}

void RunProgram(HWND hWnd, wchar_t *path) {
	int temp = ShellExecute(hWnd, L"open", path, NULL, NULL, SW_SHOW);
	if(temp == ERROR_FILE_NOT_FOUND || temp == ERROR_PATH_NOT_FOUND)
		MessageBox(NULL, L"이 프리셋이 비어있거나 프리셋 경로가 잘 못 전달되었습니다.", L"Error", MB_OK);

	return;
}

WCHAR* SelectPreset(HWND hwnd, HWND *hFileDlg, wchar_t **presetPath)
{
	OPENFILENAME OFN;                               // 파일열기 대화상자를 초기화하기 위한 변수
	const UINT nFileNameMaxLen = 512;           // 다음 줄에 정의하는 szFileName 문자열의 최대 길이
	WCHAR szFileName[512] = { 0 };    // 파일의 경로 및 이름을 복사하기 위한 문자열

	// OPENFILENAME형 변수의 멤버들의 값을 설정
	memset(&OFN, 0, sizeof(OPENFILENAME));
	OFN.lStructSize = sizeof(OPENFILENAME);
	OFN.hwndOwner = hwnd;
	OFN.lpstrFilter = L"All Files(*.*)\0*.*\0";
	OFN.lpstrInitialDir = L".\\Preset";

	OFN.lpstrFile = szFileName;
	OFN.nMaxFile = nFileNameMaxLen;

	// 파일열기 대화상자 연 후, 선택된 파일의 이름 에디트에 올림 
	if (GetOpenFileName(&OFN) != 0) {
		//SelectFile(OFN.lpstrFile, hwnd);
		//return OFN.lpstrFile;
		int len = wcslen(OFN.lpstrFile);
		*presetPath = (wchar_t*)malloc( sizeof(wchar_t) * ( len + 1) );
		wcscpy_s(*presetPath, len + 1, OFN.lpstrFile);
	}
	else {
		EndDialog(hwnd, IDOK);
		*hFileDlg = NULL;
	}

	return NULL;
}

void SelectFile(wchar_t **presetPath, wchar_t *editPath, HWND hDlg)
{
	if (editPath != NULL) {
		WritePath(*presetPath, editPath);
		return;
	}

	OPENFILENAME ofn;                 // 파일열기 대화상자를 초기화하기 위한 변수
	UINT nFileNameMaxLen = 1024;       // 다음 줄에 정의하는 szFileName 문자열의 최대 길이
	WCHAR szFileName[1024] = { 0 };    // 파일의 경로 및 이름을 복사하기 위한 문자열
	
	// OPENFILENAME형 변수의 멤버들의 값을 설정
	memset(&ofn, 0, sizeof(OPENFILENAME));
	ofn.lStructSize = sizeof(OPENFILENAME);
	ofn.hwndOwner = hDlg;
	ofn.lpstrFilter = L"All Files(*.*)\0*.*\0";

	ofn.lpstrFile = szFileName;
	ofn.nMaxFile = nFileNameMaxLen;

	// 파일열기 대화상자 연 후, 선택된 파일의 이름 에디트에 올림 
	if (GetOpenFileName(&ofn) != 0) 
		WritePath(*presetPath, ofn.lpstrFile);

	return;
}

void DeletePreset(wchar_t** path, HWND *hDlg)
{
	DeleteFile(*path);
	MessageBox(NULL, L"해당 프리셋이 초기화 되었습니다.", L"알림", MB_OK);
	EndDialog(*hDlg, IDOK);
	*hDlg = NULL;
	return;
}
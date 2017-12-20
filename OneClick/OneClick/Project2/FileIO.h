#pragma once
#include <Windows.h>
#include <tchar.h>
#include <stdio.h>
#include <locale.h>

static LPCWSTR dirPath = L".\\Preset";

static LPCWSTR dirPreset[6] = {
	L".\\Preset\\Preset1.txt",
	L".\\Preset\\Preset2.txt",
	L".\\Preset\\Preset3.txt",
	L".\\Preset\\Preset4.txt",
	L".\\Preset\\Preset5.txt",
	L".\\Preset\\Preset6.txt"
};

void WritePath(wchar_t*, wchar_t*);

static void CreatePresetFile()
{
	int i = 0;
	HANDLE fh = NULL;
	for (i = 0; i < 6; i++) {
		fh = CreateFile(dirPreset[i], GENERIC_READ | GENERIC_WRITE, 0, NULL, CREATE_NEW, 0, NULL);
		CloseHandle(fh);
	}

	fh = CreateFile(L".\\Preset\\Alarm.txt", GENERIC_READ | GENERIC_WRITE, 0, NULL, CREATE_NEW, 0, NULL);
	CloseHandle(fh);

	return;
}

WCHAR* SelectPreset(HWND, HWND*, wchar_t**);

void RunPreset(HWND, LPCTSTR *);

void RunProgram(HWND, LPCTSTR);

void SelectFile(wchar_t**, wchar_t *, HWND);

void DeletePreset(wchar_t**, HWND*);
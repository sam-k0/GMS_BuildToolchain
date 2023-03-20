#pragma once
#include <Windows.h>
#include <string>

struct processInfo {
	DWORD pid;
	HWND hwnd;
	std::string completeExePath;
};

typedef struct taskMgrProcInfo {
	DWORD pid;
	HMODULE procHandle;
	taskMgrProcInfo(DWORD p, HMODULE ph)
	{
		pid = p; procHandle = ph;
	}

	~taskMgrProcInfo()
	{
		CloseHandle(procHandle); // close the handle when no longer needed
	}

} taskMgrProcInfo;

typedef struct StartProcessData {
	DWORD pid = NULL;
	HANDLE hProc = NULL;

	StartProcessData(DWORD _p, HANDLE _hp) {
		pid = _p;
		hProc = _hp;
	}
} StartProcessData;
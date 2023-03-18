#include "WindowHook.h"
#include <iostream>

bool sendCompileSignal(processInfo* ide)
{
	DWORD dwPID; // The PID returned from thread func
	DWORD hThread = GetWindowThreadProcessId(ide->hwnd, &dwPID);
	if (dwPID == ide->pid && hThread != NULL) {
		//std::cout << PostThreadMessage(hThread, WM_KEYDOWN, 'A', 1) << std::endl;
		PostMessageW(ide->hwnd, WM_KEYDOWN, VK_F5 , 0);
		//SendMessage(ide->hwnd, WM_KEYDOWN, NULL, (LPARAM)TEXT("helol"));
		return true;
	}
	// Failure
	std::cout << "[!] Could not send signal: procID differs: "<< dwPID << " != " << ide->pid << std::endl;
	return false;
}

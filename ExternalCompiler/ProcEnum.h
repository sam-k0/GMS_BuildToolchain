#pragma once
#include <Windows.h>
#include <TlHelp32.h>
#include <iostream>
#include <comdef.h>
#include "structDefs.h"
#include <stdio.h>
#include <tchar.h>
#include <psapi.h>
#include <atlstr.h>


#define GMSname "GameMaker-Studio.exe"
#define IDEname "5piceIDE.exe"


struct handle_data {
	unsigned long process_id;
	HWND window_handle;
};

/**
 * @brief returns a struct containing ProcessID and Window Handle to the main IDE window.
 */
processInfo* getProgamProcInfo(const char* searchedProcName);

/**
 * @brief Checks if the processInfo is valid.
 */
bool is_valid(processInfo* pi);
/**  
* @brief returns the hwnd of the main window.
* @param process_id the pid to find the hwnd of
*/
HWND find_main_window(unsigned long process_id);

/**
 * @brief Given a hwnd, checks if this is the main window of a process.
 */
BOOL is_main_window(HWND handle);

/**
 * @brief I don't even know
 */
BOOL CALLBACK enum_windows_callback(HWND handle, LPARAM lParam);


taskMgrProcInfo getProcessHandleTaskMgrName(std::string name);


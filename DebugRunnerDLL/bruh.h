#pragma once
#include <Windows.h>
#include <TlHelp32.h>
#include <iostream>
#include <comdef.h>
#include <stdio.h>
#include <tchar.h>
#include <psapi.h>
#include <atlstr.h>


struct handle_data {
	unsigned long process_id;
	HWND window_handle;
};

BOOL is_main_window(HWND handle)
{
	return GetWindow(handle, GW_OWNER) == (HWND)0 && IsWindowVisible(handle);
}


BOOL CALLBACK enum_windows_callback(HWND handle, LPARAM lParam)
{
	handle_data& data = *(handle_data*)lParam;
	unsigned long process_id = 0;
	GetWindowThreadProcessId(handle, &process_id);
	if (data.process_id != process_id || !is_main_window(handle))
		return TRUE;
	data.window_handle = handle;
	return FALSE;
}


/**
 * Thank you stackoverflow!
 * No way I'd be able to overcome the pain of doing this myself
 * https://stackoverflow.com/questions/1888863/how-to-get-main-window-handle-from-process-id
 */
HWND find_main_window(unsigned long process_id)
{
	handle_data data;
	data.process_id = process_id;
	data.window_handle = 0;
	EnumWindows(enum_windows_callback, (LPARAM)&data);
	return data.window_handle;
}


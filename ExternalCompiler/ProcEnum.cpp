#include "ProcEnum.h"
/**
* 
*/

processInfo* getProgamProcInfo(const char* searchedProcName)
{
	// Alloc struct
	bool foundProc = false;
	processInfo* pi = new processInfo();
	pi->pid = NULL;
	pi->hwnd = NULL;

	HANDLE hSnapShot = CreateToolhelp32Snapshot(TH32CS_SNAPALL, NULL);
	PROCESSENTRY32 pEntry;
	/*initializing size - needed for using Process32First*/
	pEntry.dwSize = sizeof(pEntry);
	BOOL hRes = Process32First(hSnapShot, &pEntry);
	/*while first process in pEntry exists*/
	while (hRes && !foundProc)
	{
		/*create const char for string comparison*/
		_bstr_t b(pEntry.szExeFile);
		if (strcmp(b, searchedProcName) == 0)
		{
			// Begin to fill struct
			pi->pid = (DWORD)pEntry.th32ProcessID;
			foundProc = true;
		}
		hRes = Process32Next(hSnapShot, &pEntry);
	}

	// IDE not running?
	if (foundProc == false)
	{
		std::cout << "[!] The process does not appear to be running." << std::endl;
		delete pi;
		return nullptr;
	}

	// try to find the IDE window
	HWND hwndIDE = find_main_window((unsigned long)pi->pid);
	pi->hwnd = hwndIDE;

	return pi;
}

bool is_valid(processInfo* pi)
{
	// WOOHOO here we goo boolean operators
	bool res;
	return res = (pi->hwnd != NULL) && (pi->pid != NULL) ? true : false; //((pi->hwnd != NULL) && (pi->pid != NULL));
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

BOOL is_main_window(HWND handle)
{
	return GetWindow(handle, GW_OWNER) == (HWND)0 && IsWindowVisible(handle);
}

// https://learn.microsoft.com/en-us/windows/win32/psapi/enumerating-all-processes
taskMgrProcInfo getProcessHandleTaskMgrName(std::string name)
{
	// Get the list of process identifiers.
	DWORD aProcesses[1024], cbNeeded, cProcesses;
	unsigned int i;

	if (!EnumProcesses(aProcesses, sizeof(aProcesses), &cbNeeded))
	{
		return taskMgrProcInfo(NULL, NULL);
	}
	// Calculate how many process identifiers were returned.
	cProcesses = cbNeeded / sizeof(DWORD);
	// Print the name and process identifier for each process.
	for (i = 0; i < cProcesses; i++)
	{
		if (aProcesses[i] != 0)
		{
			DWORD processID = (aProcesses[i]);
			TCHAR szProcessName[MAX_PATH] = TEXT("<unknown>");
			// Get a handle to the process.
			HANDLE hProcess = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, FALSE, processID);
			// Get the process name.
			if (NULL != hProcess)
			{
				HMODULE hMod;
				DWORD cbNeeded;

				if (EnumProcessModules(hProcess, &hMod, sizeof(hMod),
					&cbNeeded))
				{
					GetModuleBaseName(hProcess, hMod, szProcessName,
						sizeof(szProcessName) / sizeof(TCHAR));


					TCHAR szNameSearch[99];
					_tcscpy_s(szNameSearch, CA2T(name.c_str()));
					if(szProcessName == szNameSearch)
					{ 
						std::cout << "Success! " << szProcessName << " found" << std::endl;
						return taskMgrProcInfo(processID, hMod);
					}
					
				}
			}
			// Print the process name and identifier.
			/**if (szProcessName != "<unknown>")
			{
				_tprintf(TEXT("%s  (PID: %u)\n"), szProcessName, processID);
			}*/
			
			
		}
	}
	return taskMgrProcInfo(NULL, NULL);
}
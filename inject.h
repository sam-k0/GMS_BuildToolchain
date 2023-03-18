#pragma once
#include "headerincludes.h"
using namespace std;
// Get ID from Window
void getProcessId(const char* window_title, DWORD& process_id)
{
    GetWindowThreadProcessId(FindWindow(NULL, window_title), &process_id);
}

bool InjectDLL(const int& pid, const string& DLL_Path)
{
    long dll_size = DLL_Path.length() + 1;
    HANDLE hProc = OpenProcess(PROCESS_ALL_ACCESS, FALSE, pid);

    if (hProc == NULL)
    {
        cerr << "[!]Fail to open target process!" << endl;
        return false;
    }
    cout << "[+]Opening Target Process..." << endl;

    LPVOID MyAlloc = VirtualAllocEx(hProc, NULL, dll_size, MEM_COMMIT, PAGE_EXECUTE_READWRITE);
    if (MyAlloc == NULL)
    {
        cerr << "[!]Fail to allocate memory in Target Process." << endl;
        return false;
    }

    cout << "[+]Allocating memory in Target Process." << endl;
    int IsWriteOK = WriteProcessMemory(hProc, MyAlloc, DLL_Path.c_str(), dll_size, 0);
    if (IsWriteOK == 0)
    {
        cerr << "[!]Fail to write in Target Process memory." << endl;
        return false;
    }
    cout << "[+]Creating Remote Thread in Target Process" << endl;

    DWORD dWord;
    LPTHREAD_START_ROUTINE addrLoadLibrary = (LPTHREAD_START_ROUTINE)GetProcAddress(LoadLibrary("kernel32"), "LoadLibraryA");
    HANDLE ThreadReturn = CreateRemoteThread(hProc, NULL, 0, addrLoadLibrary, MyAlloc, 0, &dWord);
    if (ThreadReturn == NULL)
    {
        cerr << "[!]Fail to create Remote Thread" << endl;
        return false;
    }

    if ((hProc != NULL) && (MyAlloc != NULL) && (IsWriteOK != ERROR_INVALID_HANDLE) && (ThreadReturn != NULL))
    {
        cout << "[+]DLL Successfully Injected :)" << endl;
        return true;
    }

    return false;
}

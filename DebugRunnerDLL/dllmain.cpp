#define DLL_EXPORT __declspec(dllexport)
#include <windows.h>
#include <stdio.h>
#include <iostream>
#define _WIN32_WINNT 0x0501
#include <conio.h>
#include <fcntl.h>
#include <io.h>
#include <wincon.h>
#include <thread>
#include "bruh.h"
#include "Hooking.h"
#include <atlstr.h>
#include <stdarg.h>
using namespace std;

setDlgItemTextW pSetDlgItemTextW = nullptr;
setDlgItemTextW pSetDlgItemTextWTarget; // Orig function before hook

peekMessageA pPeekMessageA = nullptr;
peekMessageA pPeekMessageATarget;


BOOL WINAPI hookedPeekMessageA(LPMSG lpMsg, HWND hWnd, UINT wmsgfiltermin, UINT wmsgfiltermax, UINT mremovemsg)
{
    if (lpMsg->message == WM_KEYDOWN)
    {
        std::cout << "Key pressed!" << std::endl;
        //MessageBoxA(NULL, "sus", "sus", 1);
    }
    return pPeekMessageA(lpMsg, hWnd, wmsgfiltermin, wmsgfiltermax, mremovemsg);
}

BOOL WINAPI hookedSetDlgItemTextW( HWND hDlg,int nIDDlgItem, LPCWSTR lpString)
{
    cout << "niddlgItem: " << nIDDlgItem << endl;
    if (nIDDlgItem == 1001)
    {
        std::cout << CW2A(lpString) << std::endl;

        std::cout << "The game crashed fatally. Press any key to exit." << std::endl;
        getchar();
        exit(-1);
    }
    return pSetDlgItemTextW(hDlg, nIDDlgItem, lpString);
}


std::thread t; // the thread to hide the main window.

//https://stackoverflow.com/questions/6486289/how-can-i-clear-console
// Clears the console window without using system() commands
void clear() {
    COORD topLeft = { 0, 0 };
    HANDLE console = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_SCREEN_BUFFER_INFO screen;
    DWORD written;

    GetConsoleScreenBufferInfo(console, &screen);
    FillConsoleOutputCharacterA(
        console, ' ', screen.dwSize.X * screen.dwSize.Y, topLeft, &written
    );
    FillConsoleOutputAttribute(
        console, FOREGROUND_GREEN | FOREGROUND_RED | FOREGROUND_BLUE,
        screen.dwSize.X * screen.dwSize.Y, topLeft, &written
    );
    SetConsoleCursorPosition(console, topLeft);
}

void redirectStdoutNew()
{
    // hide the main window

    if (!AllocConsole()) {
        // Add some error handling here.
        // You can call GetLastError() to get more info about the error.
        return;
    }

    // std::cout, std::clog, std::cerr, std::cin
    FILE* fDummy;
    freopen_s(&fDummy, "CONOUT$", "w", stdout);
    freopen_s(&fDummy, "CONOUT$", "w", stderr);
    freopen_s(&fDummy, "CONIN$", "r", stdin);
    std::cout.clear();
    std::clog.clear();
    std::cerr.clear();
    std::cin.clear();

    // std::wcout, std::wclog, std::wcerr, std::wcin
    HANDLE hConOut = CreateFileA(("CONOUT$"), GENERIC_READ | GENERIC_WRITE, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
    HANDLE hConIn = CreateFileA(("CONIN$"), GENERIC_READ | GENERIC_WRITE, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
    SetStdHandle(STD_OUTPUT_HANDLE, hConOut);
    SetStdHandle(STD_ERROR_HANDLE, hConOut);
    SetStdHandle(STD_INPUT_HANDLE, hConIn);
    std::wcout.clear();
    std::wclog.clear();
    std::wcerr.clear();
    std::wcin.clear();

}

// Hides the main game window as soon as it pops up
void hide()
{
    std::string params = string(GetCommandLineA());
    if (params.find("-show") != std::string::npos)// check the args
    {
        cout << "Not hiding the main window." << endl;
        return;
    }


    while (true) // Continuously Hide the thing
    {
        //cout << "Hiding.." << endl;
        HWND h = find_main_window(GetCurrentProcessId());
        HWND c = GetConsoleWindow();

        while ((h = find_main_window(GetCurrentProcessId())) == c)
        {
            //cout << h << "/" << c << endl;
            this_thread::sleep_for(300ms);
        }

        ShowWindowAsync(h, SW_HIDE);
        //ShowWindowAsync(c, SW_NORMAL);
        this_thread::sleep_for(300ms);
    }
}

void installHooks()
{
    cout << "[MH] Installing hooks" << endl;
    if (MH_Initialize() != ::MH_OK)
    {
        cout << "[MH] Could not initialize MinHook" << endl;
        return;
    }

    if (MH_CreateHookApiEx(L"user32", "PeekMessageA", &hookedPeekMessageA, reinterpret_cast<void**>(&pPeekMessageA), reinterpret_cast<void**>(&pPeekMessageATarget)) != MH_OK)
    {
        cout << "[MH] Could not hook PeekMessageA" << endl;
        return;
    }

    if (MH_CreateHookApi(L"user32", "SetDlgItemTextW", &hookedSetDlgItemTextW, reinterpret_cast<void**>(&pSetDlgItemTextW)) != MH_OK)
    {
        cout << "[MH] Could not hook SetDlgItemTextW" << endl;
        return;
    }



    if (MH_EnableHook(MH_ALL_HOOKS) != MH_OK)
    {
        cout << "[MH] Could not enable all hooks" << endl;
    }
}

extern "C" DLL_EXPORT BOOL APIENTRY DllMain(HINSTANCE hinstDLL, DWORD fdwReason, LPVOID lpvReserved)
{
    switch (fdwReason)
    {
    case DLL_PROCESS_ATTACH:
        redirectStdoutNew();
        t = thread(hide);
        std::cout << "[GMS Build Toolchain] Hello from the hooked runner!" << std::endl;
        installHooks();
        break;

    case DLL_PROCESS_DETACH:
        t.join();
        // detach from process
        break;

    case DLL_THREAD_ATTACH:
        // attach to thread
        break;

    case DLL_THREAD_DETACH:
        // detach from thread
        break;
    }
    return TRUE; // succesful
}

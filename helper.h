#pragma once
#include "headerincludes.h"
using namespace std;



// Show err
void showErr(const char* errTitle, const char* errMessage)
{
    MessageBox(0, errMessage, errTitle, NULL);
    exit(-1);
}
// Show message
void showMsg(const char* errTitle, const char* errMessage)
{
    MessageBox(0, errMessage, errTitle, NULL);
}

// Check if file exists
bool fileExists(string fileName)
{
    struct stat buffer;
    return (stat(fileName.c_str(), &buffer) == 0);
}


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
#pragma once
#pragma warning(disable : 4996)
#include <iostream>
#include <windows.h>
#include <string>
#include <thread>
#include <libloaderapi.h>
#include <stdio.h>
#include <direct.h>
#include <processthreadsapi.h>
#include <vector>
#include <iterator>
#include <sstream>
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

//https://www.geeksforgeeks.org/how-to-check-a-file-or-directory-exists-in-cpp/
bool isDirectory(const char* dir)
{
    struct stat sb;

    if (stat(dir, &sb) == 0)
        return true;
    else
        return false;
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



//https://www.techiedelight.com/check-if-a-string-ends-with-another-string-in-cpp/
bool endsWith(std::string const& str, std::string const& suffix) {
    if (str.length() < suffix.length()) {
        return false;
    }
    return str.compare(str.length() - suffix.length(), suffix.length(), suffix) == 0;
}

std::string getCurrentDir() // Returns EXE directory
{
    char cCurrentPath[FILENAME_MAX]; // get working directory into buffer
    if (!_getcwd(cCurrentPath, sizeof(cCurrentPath)))
        exit(-1);
    cCurrentPath[sizeof(cCurrentPath) - 1] = '\0'; // not really required

    char* s = cCurrentPath; // save path from buffer into currentpath chararr
    std::string str(s);
    //free(s);
    return str;
}

string Join(vector<string> strings)
{
    const char* const delim = " ";
    std::ostringstream imploded;
    string ret;
    std::copy(strings.begin(), strings.end(), std::ostream_iterator<std::string>(imploded, delim));
    ret = imploded.str();
    ret.pop_back();

    return ret;
}
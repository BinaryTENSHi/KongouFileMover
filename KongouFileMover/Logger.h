#pragma once

#include <windows.h>
#include <fstream>

class Logger
{
public:
    static Logger* getInstance();
    void Start();
    void Stop();

    void Error(LPCWSTR);
    void Info(LPCWSTR);

private:
    Logger();
    ~Logger();
    void operator=(Logger const&);

    void getCurrentTime(wchar_t*);

private:
    FILE* file;
    wchar_t* timestr;
};
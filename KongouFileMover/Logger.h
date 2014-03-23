#pragma once

#include <windows.h>
#include <fstream>

class Logger
{
public:
    static Logger* getInstance();
    void start();
    void stop();

    void error(LPCWSTR);
    void info(LPCWSTR);

private:
    Logger();
    ~Logger();
    void operator=(Logger const&);

    void getCurrentTime(wchar_t*);

private:
    FILE* file;
    wchar_t* timestr;
};
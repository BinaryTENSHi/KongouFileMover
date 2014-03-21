#pragma once

#include <windows.h>
#include <fstream>

class Logger
{
public:
    static Logger* getInstance();
    void Start();
    void Stop();

    void Error(LPWSTR);
    void Info(LPWSTR);

private:
    Logger();
    ~Logger();
    void operator=(Logger const&); 
    
    LPWSTR getCurrentTime();

private:
    FILE* file;
};

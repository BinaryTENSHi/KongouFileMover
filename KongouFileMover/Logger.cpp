#include "Logger.h"

#include <iostream>
#include <fstream>
#include <time.h>

Logger* Logger::getInstance()
{
    static Logger instance;
    return &instance;
}

Logger::Logger()
{
    timestr = new wchar_t[25]{'\0'};
}

Logger::~Logger()
{
    if (file != nullptr)
        Stop();

    if (timestr != nullptr)
    {
        delete[] timestr;
        timestr = nullptr;
    }
}

void Logger::Start()
{
    _wfopen_s(&file, L"kfm.log", L"w");
    getCurrentTime(timestr);
    fputws(timestr, file);
    fputws(L"Starting log...", file);
    fputws(L"\n", file);
}

void Logger::Stop()
{
    getCurrentTime(timestr);
    fputws(timestr, file);
    fputws(L"Stopping log...", file);
    fputws(L"\n", file);
    fclose(file);
    file = nullptr;
}

void Logger::Error(LPCWSTR str)
{
    getCurrentTime(timestr);
    fputws(timestr, file);
    fputws(L"ERROR: ", file);
    fputws(str, file);
    fputws(L"\n", file);
}

void Logger::Info(LPCWSTR str)
{
    getCurrentTime(timestr);
    fputws(timestr, file);
    fputws(L"INFO: ", file);
    fputws(str, file);
    fputws(L"\n", file);
}

void Logger::getCurrentTime(wchar_t* dest)
{
    time_t timer;
    tm tinfo;

    time(&timer);
    localtime_s(&tinfo, &timer);

    wcsftime(dest, 25, L"[%d.%m.%Y %H:%M:%S] ", &tinfo);
}
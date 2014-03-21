#include "Logger.h"

#include <iostream>
#include <fstream>
#include <time.h>

Logger* Logger::getInstance(){
    static Logger instance;
    return &instance;
}

Logger::Logger()
{
}

Logger::~Logger()
{
    if (!file)
        Stop();
}

void Logger::Start()
{
    fopen_s(&file, "kfm.log", "w");
    fputws(getCurrentTime(), file);
    fputws(L"Starting log...", file);
    fputws(L"\n", file);
}

void Logger::Stop()
{
    fputws(getCurrentTime(), file);
    fputws(L"Stopping log...", file);
    fputws(L"\n", file);
    fclose(file);
    file = 0;
}

void Logger::Error(LPWSTR str)
{
    fputws(getCurrentTime(), file);
    fputws(L"ERROR: ", file);
    fputws(str, file);
    fputws(L"\n", file);
}

void Logger::Info(LPWSTR str)
{
    fputws(getCurrentTime(), file);
    fputws(L"INFO: ", file);
    fputws(str, file);
    fputws(L"\n", file);
}

LPWSTR Logger::getCurrentTime()
{
    time_t timer;
    tm tinfo;

    time(&timer);
    localtime_s(&tinfo, &timer);

    wchar_t dest[50] = { '\0' };
    wcsftime(dest, 50, L"[%d.%m.%Y %H:%M:%S] ", &tinfo);
    return dest;
}
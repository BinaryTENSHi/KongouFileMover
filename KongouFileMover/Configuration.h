#pragma once

#include <windows.h>
#include <string>

class Configuration
{
public:
    static Configuration* getInstance();
    int Read(LPWSTR);
    LPCWSTR rootFolder;

private:
    Configuration();
    ~Configuration();
    void operator=(Configuration const&);
};
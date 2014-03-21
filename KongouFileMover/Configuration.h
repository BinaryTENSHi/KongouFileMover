#pragma once

#include <windows.h>

class Configuration
{
public:
    static Configuration* getInstance();
    int Read(LPWSTR);

private:
    Configuration();
    ~Configuration();
    void operator=(Configuration const&);

};


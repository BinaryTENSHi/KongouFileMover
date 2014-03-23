#pragma once

#include <windows.h>
#include <string>

#define CONFIG_OK 0
#define CONFIG_NOTAVAILABLE 1
#define CONFIG_NOTREAD 2
#define CONFIG_NOROOT 3
#define CONFIG_NOFOLDER 4
#define CONFIG_NOFILE 5

class Configuration
{
public:
    static Configuration* getInstance();
    int Read(LPWSTR);
    LPCWSTR rootFolder;
    LPCWSTR folderContent;
    LPCWSTR fileContent;

private:
    Configuration();
    ~Configuration();
    void operator=(Configuration const&);
};
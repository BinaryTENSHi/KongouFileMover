#include "Configuration.h"
#include "Logger.h"

#pragma comment( lib, "shlwapi.lib")

#include <iostream>
#include <fstream>
#include <regex>
#include <atlconv.h>
#include <Shlwapi.h>

Configuration* Configuration::getInstance()
{
    static Configuration instance;
    return &instance;
}

Configuration::Configuration()
{
}

Configuration::~Configuration()
{
}

int Configuration::Read(LPWSTR filename)
{
    Logger* log = Logger::getInstance();

    HANDLE file = CreateFile(filename, GENERIC_READ, FILE_SHARE_READ , NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL , NULL);

    if (file == INVALID_HANDLE_VALUE)
        return 1;

    DWORD size = GetFileSize(file, &size);
    char* dataArr = new char[size]{'\0'};
    DWORD read = 0;

    if (!ReadFile(file, dataArr, size, &read, NULL))
        return 2;

    CloseHandle(file);
    std::string data = std::string(dataArr);
    delete[] dataArr;

    std::regex rootPattern("^root=\\w:(\\w|\\W)*?\r\n");
    std::smatch match;
    bool root = false;

    if (std::regex_search(data, match, rootPattern))
    {
        USES_CONVERSION;
#pragma warning(suppress: 6255)
        rootFolder = A2W(data.substr(5, match.length() - 7).c_str());

        if (PathFileExists(rootFolder))
            root = true;
    }

    if (!root)
        return 3;

    std::wstring rootWstring(rootFolder);
    std::wstring concatString = L"Using " + rootWstring + L" as root folder";
    LPWSTR info = (LPWSTR)concatString.c_str();
    log->Info(info);

    return 0;
}
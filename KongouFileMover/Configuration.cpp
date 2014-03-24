#include "Configuration.h"
#include "Logger.h"

#pragma comment( lib, "shlwapi.lib")

#include <iostream>
#include <fstream>
#include <regex>
#include <atlconv.h>
#include <Shlwapi.h>
#include <locale>
#include <codecvt>
#include <string>

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

int Configuration::read(LPCWSTR filename)
{
    Logger* log = Logger::getInstance();

    HANDLE file = CreateFile(filename, GENERIC_READ, FILE_SHARE_READ , NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL , NULL);

    if (file == INVALID_HANDLE_VALUE)
        return CONFIG_NOTAVAILABLE;

    DWORD size = GetFileSize(file, &size);
    char* dataArr = new char[size]{'\0'};
    DWORD read = 0;

    if (!ReadFile(file, dataArr, size, &read, NULL))
        return CONFIG_NOTREAD;

    CloseHandle(file);
    std::string data = std::string(dataArr);
    std::string subData = "";
    delete[] dataArr;

    std::regex pattern("^root=\\w:(\\w|\\W)*?\r\n");
    std::smatch match;
    bool res = false;

    if (std::regex_search(data, match, pattern))
    {
        std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;
        rootFolder = converter.from_bytes(data.substr(match.prefix().length() + 5, match.length() - 7));

        if (PathFileExists(rootFolder.c_str()))
            res = true;
    }

    if (!res)
        return CONFIG_NOROOT;

    pattern = std::regex("^folder( |\r\n)*\\{(.|\r\n)*?\\}");
    if (std::regex_search(data, match, pattern))
    {
        pattern = std::regex("\\{(.|\r\n)*?\\}");
        subData = data.substr(match.prefix().length(), match.length());
        std::regex_search(subData, match, pattern);

        std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;
        folderContent = converter.from_bytes(subData.substr(match.prefix().length() + 1, match.length() - 2));

        res = true;
    }

    if (!res)
        return CONFIG_NOFOLDER;

    pattern = std::regex("^file( |\r\n)*\\{(.|\r\n)*?\\}");
    if (std::regex_search(data, match, pattern))
    {
        pattern = std::regex("\\{(.|\r\n)*?\\}");
        subData = data.substr(match.prefix().length(), match.length());
        std::regex_search(subData, match, pattern);

        std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;
        fileContent = converter.from_bytes(subData.substr(match.prefix().length() + 1, match.length() - 2));

        res = true;
    }

    if (!res)
        return CONFIG_NOFILE;

    log->info(L"Using " + std::wstring(rootFolder) + L" as root folder");

    return CONFIG_OK;
}
#include "Configuration.h"

#include <fstream>

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
    FILE* file;
    _wfopen_s(&file, filename, L"r");

    if (file == nullptr)
        return 1;

    /* read config */

    fclose(file);
    return 0;
}
#include "Configuration.h"

#include <fstream>

Configuration* Configuration::getInstance(){
    static Configuration instance;
    return &instance;
}

Configuration::Configuration()
{
}

Configuration::~Configuration()
{
}

void Configuration::Read(LPWSTR filename)
{
}
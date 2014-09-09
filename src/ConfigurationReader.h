#pragma once

#include <string>
#include <vector>

#include "Configuration.h"
#include "ConfigurationState.h"

class ConfigurationReader
{
public:
    ConfigurationReader(std::string configPath);

    ~ConfigurationReader();

    ConfigurationState state = ConfigurationState::OK;
    Configuration config;
};


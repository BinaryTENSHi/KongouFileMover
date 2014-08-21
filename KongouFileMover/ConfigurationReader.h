#pragma once

#include <string>
#include <vector>

struct Configuration
{
    std::string root;
    std::vector<std::string> folderRegex;
    std::vector<std::string> fileRegex;
};

enum ConfigurationState
{
    OK,
    FileDoesNotExist,
    FileNotActuallyAFile,
    FileNotReadable,
};

class ConfigurationReader
{
public:
    ConfigurationReader(std::string configPath);
    ~ConfigurationReader();
    ConfigurationState state = ConfigurationState::OK;
};


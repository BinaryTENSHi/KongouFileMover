#pragma once

#include <string>
#include <vector>

struct Configuration
{
    std::string root;
    std::vector<std::string> folderRegex;
    std::vector<std::string> fileRegex;
};

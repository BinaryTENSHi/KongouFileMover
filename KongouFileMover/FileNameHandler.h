#pragma once

#include <string>
#include <vector>

#include "Configuration.h"

struct FileRename
{
    std::string oldPath;
    std::string newPath;
};

class FileNameHandler
{
public:
    FileNameHandler(std::vector<std::string> *paths, Configuration config);
    ~FileNameHandler();
    std::vector<FileRename> fileRenames;
};


#pragma once

#include <string>
#include <vector>

#include "FileNameRegexer.h"
#include "Configuration.h"
#include "FileRename.h"
#include "FilesState.h"

#include <boost/filesystem.hpp>
namespace fs = boost::filesystem;

class FileNameHandler
{
public:
    FileNameHandler(std::vector<std::string> *paths, Configuration config);
    ~FileNameHandler();
    std::vector<FileRename> fileRenames;
    FilesState state = FilesState::OK;

private:
    std::vector<fs::directory_entry> files;
};


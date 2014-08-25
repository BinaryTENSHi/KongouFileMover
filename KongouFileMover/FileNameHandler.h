#pragma once

#include <string>
#include <vector>

#include "Configuration.h"
#include "FileRename.h"
#include "RegexState.h"
#include "KRegex.h"

#include <boost/filesystem.hpp>
namespace fs = boost::filesystem;

class FileNameHandler
{
public:
    FileNameHandler(std::vector<std::string> *paths, Configuration config);
    ~FileNameHandler();
    std::vector<FileRename> fileRenames;
    RegexState state = RegexState::OK;

public:
    void process();

private:
    void parseRegexes(std::vector<std::string> source, std::vector<KRegex> *dest);
    std::string applyRegexes(fs::directory_entry file, std::vector<KRegex> *regexes);

private:
    Configuration config;
    std::vector<fs::directory_entry> files;
    std::vector<KRegex> fileRegexes;
    std::vector<KRegex> folderRegexes;
};


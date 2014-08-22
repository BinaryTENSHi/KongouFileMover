#pragma once

#include "Configuration.h"
#include "RegexState.h"
#include "KRegex.h"

class FileNameRegexer
{
public:
    FileNameRegexer(Configuration config);
    ~FileNameRegexer();
    RegexState state;

private:
    void ParseRegexes(std::vector<std::string> source, std::vector<KRegex> *dest);

private:
    Configuration config;
    std::vector<KRegex> fileRegexes;
    std::vector<KRegex> folderRegexes;
};


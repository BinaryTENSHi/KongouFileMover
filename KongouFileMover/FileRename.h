#pragma once

#include <string>

#include <boost/filesystem.hpp>
namespace fs = boost::filesystem;

struct FileRename
{
    fs::path srcFile;
    fs::path destDirectory;
    std::string destFileName;
};

#include "ConfigurationReader.h"

#include <sys/stat.h>
#include <iostream>

#include <boost/filesystem.hpp>
#include <boost/filesystem/fstream.hpp>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>

namespace fs = boost::filesystem;
namespace pt = boost::property_tree;

ConfigurationReader::ConfigurationReader(std::string configPath)
{
    fs::path path(configPath);
    if (!is_regular_file(path))
    {
        state = ConfigurationState::FileNotActuallyAFile;
        return;
    }

    struct stat sb;
    if (stat(configPath.c_str(), &sb) == -1)
    {
        state = ConfigurationState::FileNotReadable;
        return;
    }

    fs::ifstream s(path);
    pt::ptree pt;

    read_json(s, pt);
    s.close();

    config.root = pt.get<std::string>("root");

    for (pt::ptree::value_type &v : pt.get_child("folderRegex"))
        config.folderRegex.push_back(v.second.data());

    for (pt::ptree::value_type &v : pt.get_child("fileRegex"))
        config.fileRegex.push_back(v.second.data());
}

ConfigurationReader::~ConfigurationReader()
{
}

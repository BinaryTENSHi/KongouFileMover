#include "ConfigurationReader.h"

#include <sys/stat.h>
#include <iostream>

#include <boost/filesystem.hpp>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>
#include <boost/foreach.hpp>

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

    std::ifstream s(path.c_str());
    pt::ptree pt;

    read_json(s, pt);
    s.close();

    std::string root = pt.get<std::string>("root");
    std::vector<std::string> folderRegex;
    std::vector<std::string> fileRegex;

    BOOST_FOREACH(pt::ptree::value_type &v, pt.get_child("folderRegex"))
        folderRegex.push_back(v.second.data());

    BOOST_FOREACH(pt::ptree::value_type &v, pt.get_child("fileRegex"))
        fileRegex.push_back(v.second.data());
}

ConfigurationReader::~ConfigurationReader()
{
}

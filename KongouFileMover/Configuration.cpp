#include "Configuration.h"

#include <boost\filesystem.hpp>

namespace fs = boost::filesystem;

Configuration::Configuration(std::string configPath)
{
    fs::path path(configPath);
}

Configuration::~Configuration()
{
}

#include "FileNameHandler.h"

#include <boost/filesystem.hpp>
#include <boost/fusion/container/vector.hpp>

namespace fs = boost::filesystem;

FileNameHandler::FileNameHandler(std::vector<std::string> *paths, Configuration config)
{
    std::vector<std::string>::iterator i = paths->begin();
    std::vector<fs::directory_entry> files;

    do
    {
        fs::path path(*i);
        if (is_directory(path))
            std::copy(fs::directory_iterator(path), fs::directory_iterator(), std::back_inserter(files));
        else if (is_regular_file(path))
            files.push_back(fs::directory_entry(path));
    } while (++i != paths->end());
}


FileNameHandler::~FileNameHandler()
{
}

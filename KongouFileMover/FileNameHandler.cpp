#include "FileNameHandler.h"

FileNameHandler::FileNameHandler(std::vector<std::string> *paths, Configuration config)
{
    std::vector<std::string>::iterator i = paths->begin();
    do
    {
        fs::path path(*i);
        if (is_directory(path))
            std::copy(fs::directory_iterator(path), fs::directory_iterator(), std::back_inserter(files));
        else if (is_regular_file(path))
            files.push_back(fs::directory_entry(path));
    } while (++i != paths->end());

    if (files.empty())
        state = FilesState::NoFiles;
}


FileNameHandler::~FileNameHandler()
{
}

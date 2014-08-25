#include "FileNameHandler.h"

#include <boost/algorithm/string.hpp>
#include <boost/regex.hpp>

namespace al = boost::algorithm;

FileNameHandler::FileNameHandler(std::vector<std::string> *paths, Configuration config)
{
    this->config = config;

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
        state = RegexState::NoFiles;

    parseRegexes(config.fileRegex, &fileRegexes);
    parseRegexes(config.folderRegex, &folderRegexes);

    if (folderRegexes.empty() && fileRegexes.empty())
        state = RegexState::NoRegex;
}

FileNameHandler::~FileNameHandler()
{
}

void FileNameHandler::process()
{
    FileRename rename;
    for (fs::directory_entry file : files)
    {
        rename.srcFile = fs::absolute(file);
        rename.destDirectory = fs::path(config.root) / fs::path(applyRegexes(file, &folderRegexes));
        rename.destFileName = applyRegexes(file, &fileRegexes) + fs::extension(file);
        fileRenames.push_back(rename);
    }
}

std::string FileNameHandler::applyRegexes(fs::directory_entry file, std::vector<KRegex> *regexes)
{
    std::string name(fs::basename(file));
    std::vector<KRegex>::iterator i = regexes->begin();
    do
    {
        switch (i->type)
        {
        case RegexType::Trim:
        {
            al::trim(name);
            break;
        }
        case RegexType::Remove:
        {
            boost::regex reg(i->arguments[0]);
            name = boost::regex_replace(name, reg, "");
            break;
        }
        case RegexType::Replace:
        {
            boost::regex reg(i->arguments[0]);
            name = boost::regex_replace(name, reg, i->arguments[1]);
            break;
        }
        }
    } while (++i != regexes->end());
    return name;
}

void FileNameHandler::parseRegexes(std::vector<std::string> source, std::vector<KRegex> *dest)
{
    std::vector<std::string>::iterator i = source.begin();
    do
    {
        std::string s = *i;
        KRegex regex;
        if (boost::istarts_with(s, "trim"))
        {
            regex.type = RegexType::Trim;
        }
        else if (boost::istarts_with(s, "remove"))
        {
            regex.type = RegexType::Remove;
            regex.arguments.push_back(s.substr(6, s.length() - 6));
        }
        else if (boost::istarts_with(s, "replace"))
        {
            regex.type = RegexType::Replace;
            s = s.substr(7, s.length() - 7);
            al::split(regex.arguments, s, boost::is_any_of(","));
        }
        else
        {
            state = RegexState::Errors;
        }

        if (!regex.arguments.empty())
        {
            std::vector<std::string>::iterator j = regex.arguments.begin();
            do
            {
                al::trim(*j);
            } while (++j != regex.arguments.end());
        }

        dest->push_back(regex);
    } while (++i != source.end());
}
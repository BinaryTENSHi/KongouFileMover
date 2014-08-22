#include "FileNameRegexer.h"

#include <string>
#include <vector>

#include <boost/algorithm/string.hpp>
#include <boost/regex.hpp>

namespace al = boost::algorithm;

FileNameRegexer::FileNameRegexer(Configuration config)
{
    this->config = config;

    ParseRegexes(config.fileRegex, &fileRegexes);
    ParseRegexes(config.folderRegex, &folderRegexes);

    if (fileRegexes.empty() && folderRegexes.empty())
        state = RegexState::NoRegex;
}


FileNameRegexer::~FileNameRegexer()
{
}

void FileNameRegexer::ParseRegexes(std::vector<std::string> source, std::vector<KRegex> *dest)
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

        std::vector<std::string>::iterator j = regex.arguments.begin();
        do
        {
            al::trim(*j);
        } while (++j != regex.arguments.end());

        dest->push_back(regex);
    } while (++i != source.end());
}
#pragma once

#include <string>
#include <vector>

enum RegexType
{
    Trim,
    Remove,
    Replace
};

struct KRegex
{
    RegexType type;
    std::vector<std::string> arguments;
};
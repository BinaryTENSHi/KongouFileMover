#include "Expression.h"
#include <string>
#include <algorithm>
#include <vector>
#include <regex>

Expression::Expression()
{
}

Expression::~Expression()
{
}

int Expression::compile(std::wstring input)
{
    input.erase(std::remove(input.begin(), input.end(), ' '), input.end());
    wchar_t* context = &input[0];
    wchar_t* res = L"";

    std::vector<std::wstring>::iterator it = expressions.begin();

    while ((res = wcstok_s(context, L"\r\n", &context)) != nullptr)
        it = expressions.insert(it, res);

    return EXPR_OK;
}

void Expression::run(std::wstring& input)
{
    std::vector<std::wstring>::iterator it = expressions.end();

    while (it != expressions.begin())
    {
        --it;
        std::wstring command = *it;

        if (command.find(L"remove") == 0)
        {
            std::wregex regex(command.substr(6, command.length()));
            input = std::regex_replace(input, regex, L"");
        }
        else if (command.find(L"replace") == 0)
        {
            /* to do */
        }
        if (command.find(L"trim") == 0)
        {
            int ld = input.find_last_of('.');
            std::wstring end;

            if (ld != -1)
            {
                end = (input.substr(ld + 1, input.length()));
                input = input.substr(0, ld);
            }

            std::wstring::size_type pos = input.find_last_not_of(' ');

            if (pos != std::wstring::npos)
            {
                input.erase(pos + 1);
                pos = input.find_first_not_of(' ');
                if (pos != std::wstring::npos) input.erase(0, pos);
            }
            else
            {
                input.erase(input.begin(), input.end());
            }

            if (ld != -1)
                input += L"." + end;
        }
    }
}
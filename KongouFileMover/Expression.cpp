#include "Expression.h"
#include "StringExtension.h"

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
    trim(input);
    wchar_t* context = &input[0];
    wchar_t* res = L"";

    std::vector<std::wstring>::iterator it = expressions.begin();

    while ((res = wcstok_s(context, L"\r\n", &context)) != nullptr)
    {
        std::wstring s = trim(std::wstring(res));

        if (s.find(L"remove") == 0)
        {
            if (s.length() < 7)
                return EXPR_ERROR_REM;

            if (trim(s.substr(7, s.length() - 7)).empty())
                return EXPR_ERROR_REM;
        }
        else if (s.find(L"replace") == 0)
        {
            if (s.length() < 8)
                return EXPR_ERROR_REP;

            std::wstring ws = trim(s.substr(8, s.length() - 8));
            if (ws.empty() || ws.find(L"'") == -1)
                return EXPR_ERROR_REP;
        }

        it = expressions.insert(it, s);
    }

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
            std::wregex regex(command.substr(7, command.length()));
            input = std::regex_replace(input, regex, L"");
        }
        else if (command.find(L"replace") == 0)
        {
            std::wstring r = command.substr(8, command.length() - 8);
            int fc = r.find_first_of(',');
            std::wregex regex(r.substr(0, fc));
            std::wstring re(r.substr(fc + 1, r.length() - fc - 1));
            int f = re.find_first_of(L"'");
            int l = re.find_last_of(L"'");
            if (f != -1 && l != -1)
                re = re.substr(f + 1, l - (f + 1));

            input = std::regex_replace(input, regex, re);
        }
        else if (command.find(L"trim") == 0)
        {
            int ld = input.find_last_of('.');
            std::wstring end;

            if (ld != -1)
            {
                end = (input.substr(ld + 1, input.length()));
                input = input.substr(0, ld);
            }

            trim(input);

            if (ld != -1)
                input += L"." + end;
        }
    }
}
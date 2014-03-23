#include "Expression.h"
#include <string>
#include <algorithm>
#include <vector>

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
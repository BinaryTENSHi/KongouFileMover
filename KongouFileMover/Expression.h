#pragma once

#include <windows.h>
#include <string>
#include <vector>

#define EXPR_OK 0
#define EXPR_ERROR 1

class Expression
{
public:
    Expression();
    ~Expression();
    int compile(std::wstring);

public:
    std::vector<std::wstring> expressions;

private:
    void operator=(Expression const&);
};
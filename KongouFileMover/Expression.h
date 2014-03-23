#pragma once

#include <windows.h>
#include <string>
#include <vector>

#define EXPR_OK 0
#define EXPR_ERROR_REM 1
#define EXPR_ERROR_REP 2

class Expression
{
public:
    Expression();
    ~Expression();
    int compile(std::wstring);
    void run(std::wstring&);

public:
    std::vector<std::wstring> expressions;

private:
    void operator=(Expression const&);
};
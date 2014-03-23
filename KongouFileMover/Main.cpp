#include <windows.h>

#include "Logger.h"
#include "Configuration.h"
#include "Resource.h"
#include "Expression.h"

void PrintUsage();

INT WINAPI WinMain(
    _In_ HINSTANCE instance,
         _In_opt_ HINSTANCE previousInstance,
         _In_ LPSTR commandline,
         _In_ INT showCommand)
{
    int argcount;
    LPWSTR* args = CommandLineToArgvW(GetCommandLine(), &argcount);

    static Logger* log = Logger::getInstance();
    static Configuration* config = Configuration::getInstance();

    log->Start();

    if (argcount == 1 || wcsncmp(args[argcount - 1], L"-c:", 3) == 0)
    {
        PrintUsage();
        return 1;
    }

    wchar_t* configFile = L"config.ini";

    for (int i = 1; i < argcount; ++i)
    {
        if (wcsncmp(args[i], L"-c:", 3) == 0)
        {
            std::wstring configWString(configFile);
            std::wstring infoWString = L"Using " + configWString + L" as root folder";
            log->Info(infoWString.c_str());
        }
    }

    std::wstring oriPath(args[argcount - 1]);
    std::wstring oriFile(oriPath.substr(oriPath.find_last_of(L"\\") + 1, oriPath.length()));

    LocalFree(args);

    int res = config->read(configFile);
    switch (res)
    {
    case CONFIG_NOTAVAILABLE:
        {
            HRSRC resourceInfo = FindResource(instance, MAKEINTRESOURCE(IDR_TEXT_DEFAULTCONFIG), L"TEXT");
            if (!resourceInfo)
            {
                MessageBox(NULL, L"Default configuration not found.", L"Configuration", MB_OK | MB_ICONINFORMATION);
                return 1;
            }

            HGLOBAL resource = LoadResource(instance, resourceInfo);
            if (!resource)
            {
                MessageBox(NULL, L"Resource could not loaded.", L"Configuration", MB_OK | MB_ICONINFORMATION);
                return 1;
            }

            LPVOID memory = LockResource(resource);
            DWORD size = SizeofResource(instance, resourceInfo);
            DWORD written = -1;

            HANDLE file = CreateFile(configFile, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL , NULL);
            WriteFile(file, resource, size, &written, NULL);
            CloseHandle(file);
            FreeResource(resource);

            MessageBox(NULL, L"Configuration created. Please edit it.", L"Configuration", MB_OK | MB_ICONINFORMATION);
            return 0;
        }

    case CONFIG_NOTREAD:
        {
            MessageBox(NULL, L"Configuration could not be read.", L"Configuration", MB_OK | MB_ICONINFORMATION);
            return 1;
        }

    case CONFIG_NOROOT:
        {
            MessageBox(NULL, L"Root folder not configured or doesn't exist.", L"Configuration", MB_OK | MB_ICONINFORMATION);
            return 1;
        }
    case CONFIG_NOFOLDER:
        {
            MessageBox(NULL, L"No folder expressions configured.", L"Configuration", MB_OK | MB_ICONINFORMATION);
            return 1;
        }
    case CONFIG_NOFILE:
        {
            MessageBox(NULL, L"No file expressions configured.", L"Configuration", MB_OK | MB_ICONINFORMATION);
            return 1;
        }
    }

    log->Info(L"Config read. Interpreting folder expressions.");
    Expression* folderExp = new Expression;
    res = folderExp->compile(config->folderContent);

    switch (res)
    {
    case EXPR_OK:
        {
            log->Info(L"Found the following folder expressions:");
            std::vector<std::wstring>::iterator it = folderExp->expressions.end();
            while (it != folderExp->expressions.begin())
            {
                --it;
                log->Info((*it).c_str());
            }
            break;
        }
    }

    log->Info(L"Interpreting file expressions.");
    Expression* fileExp = new Expression;
    res = fileExp->compile(config->fileContent);

    switch (res)
    {
    case EXPR_OK:
        {
            log->Info(L"Found the following file expressions:");
            std::vector<std::wstring>::iterator it = fileExp->expressions.end();
            while (it != fileExp->expressions.begin())
            {
                --it;
                log->Info((*it).c_str());
            }
            break;
        }
    }

    std::wstring in(oriFile);
    folderExp->run(in);
    log->Info(in.c_str());

    in = std::wstring(oriFile);
    fileExp->run(in);
    log->Info(in.c_str());

    log->Stop();
    delete folderExp;
    delete fileExp;
    return 0;
}

void PrintUsage()
{
    MessageBox(NULL,
                   L"KongouFileMover.exe [-c:configfile] %filename%",
                   L"KongouFileMover usage",
                   MB_OK | MB_ICONINFORMATION);
}

/* Not yet used */
LRESULT CALLBACK WndProc(HWND window, UINT message, WPARAM wparam, LPARAM lparam)
{
    switch (message)
    {
    case WM_DESTROY:
    case WM_CLOSE:
        PostQuitMessage(0);
        break;

    default:
        return DefWindowProc(window, message, wparam, lparam);
    }

    return 0;
}
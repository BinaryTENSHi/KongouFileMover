#include <windows.h>

#include "Logger.h"
#include "Configuration.h"

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

    if (argcount == 1)
    {
        PrintUsage();
    }

    wchar_t* configFile = L"config.ini";

    for (int i = 1; i < argcount; ++i)
    {
        if (wcsncmp(args[i], L"-c:", 3) == 0)
        {
            wchar_t* str = wcstok_s(args[i], L":", &configFile);

            wchar_t* setting = L"Setting custom config to '";
            int size = wcslen(setting) + wcslen(configFile) + 2;
            wchar_t* dest = new wchar_t[size]{'\0'};
            wcscat_s(dest, size, setting);
            wcscat_s(dest, size, configFile);
            wcscat_s(dest, size, L"'");
            log->Info(dest);
            delete[] dest;
        }
    }

    int res = config->Read(configFile);
    if (res == 1)
    {
        MessageBox(NULL, L"Configuration created. Please edit it.", L"Configuration", MB_OK | MB_ICONINFORMATION);
        return 0;
    }

    LocalFree(args);
    log->Stop();
}

void PrintUsage()
{
    MessageBox(NULL,
                   L"KongouFileMover.exe [-c:configfile] %filename%",
                   L"KongouFileMover usage",
                   MB_OK | MB_ICONINFORMATION);

    PostQuitMessage(1);
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
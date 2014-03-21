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

    for (int i = 1; i < argcount; ++i)
    {
        wchar_t* context = (wchar_t*)malloc(sizeof(wchar_t));
        wchar_t* str = wcstok_s(args[i], L":", &context);

        if (wcscmp(str, L"-c") == 0)
        {
            wchar_t* setting = L"Setting Config to '";
            int size = wcslen(setting) + wcslen(context) + 2;
            wchar_t* dest = new wchar_t[size]{'\0'};
            wcscat_s(dest, size, setting);
            wcscat_s(dest, size, context);
            wcscat_s(dest, size, L"'");
            log->Info(dest);
            delete[] dest;

            int res = config->Read(context);
            if (res == 1)
            {
                MessageBox(NULL, L"Configuration created. Please edit it.", L"Configuration", MB_OK | MB_ICONINFORMATION);
                return 0;
            }
            else if (res == 2)
            {
                MessageBox(NULL, L"Configuration could not be created.\nPlease make sure you have sufficient permissions.", L"Configuration", MB_OK | MB_ICONERROR);
                return 1;
            }
        }
        else if (wcscmp(str, L"-r") == 0)
        {
            wchar_t* setting = L"Setting Regex to '";
            int size = wcslen(setting) + wcslen(context) + 2;
            wchar_t* dest = new wchar_t[size]{'\0'};
            wcscat_s(dest, size, setting);
            wcscat_s(dest, size, context);
            wcscat_s(dest, size, L"'");
            log->Info(dest);
            delete[] dest;
        }
        else
        {
            PrintUsage();
        }
    }

    LocalFree(args);
    log->Stop();
}

void PrintUsage()
{
    MessageBox(NULL,
               L"KongouFileMover.exe [-c:configfile] [-r:regexfile] %filename%",
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
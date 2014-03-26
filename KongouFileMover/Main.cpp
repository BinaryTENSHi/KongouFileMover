/* Windowstyles */
#define ISOLATION_AWARE_ENABLED 1
#if defined _M_IX86
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='X86' publicKeyToken='6595b64144ccf1df' language='*'\"")
#elif defined _M_X64
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='amd64' publicKeyToken='6595b64144ccf1df' language='*'\"")
#else
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")
#endif

#include <windows.h>
#include <shlwapi.h>
#include <CommCtrl.h>

#include "Logger.h"
#include "Configuration.h"
#include "Resource.h"
#include "Expression.h"

void printUsage();
bool initializeWindow(HINSTANCE);

const LPCWSTR className = L"popupWindowClass";
const LPCWSTR progressClassName = L"popupWindowProgressClass";

HWND windowHandle;
HWND progressHandle;
WNDCLASSEX windowClass;

LRESULT CALLBACK WndProc(HWND window, UINT message, WPARAM wparam, LPARAM lparam);

INT WINAPI WinMain(
    _In_ HINSTANCE instance,
         _In_opt_ HINSTANCE previousInstance,
         _In_ LPSTR commandline,
         _In_ INT showCommand)
{
    if (!initializeWindow(instance))
        return 1;

    ShowWindow(windowHandle, SW_SHOW);
    UpdateWindow(windowHandle);

    MSG msg;
    while (GetMessage(&msg, NULL, 0, 0) > 0)
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return 0;

    bool move = false;
    bool hasFile = false;
    int argcount;
    LPWSTR* args = CommandLineToArgvW(GetCommandLine(), &argcount);

    static Logger* log = Logger::getInstance();
    static Configuration* config = Configuration::getInstance();

    log->start();

    wchar_t* configFile = L"config.ini";

    for (int i = 1; i < argcount; ++i)
    {
        if (wcsncmp(args[i], L"-c:", 3) == 0)
        {
            log->info(L"Using " + std::wstring(configFile) + L" as root folder");
        }
        else if (wcsncmp(args[i], L"-x", 2) == 0)
        {
            move = true;
        }
        else
        {
            hasFile = true;
        }
    }

    if (!hasFile)
    {
        printUsage();
        return 1;
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

    log->info(L"Config read. Interpreting folder expressions.");
    Expression* folderExp = new Expression;
    res = folderExp->compile(config->folderContent);

    switch (res)
    {
    case EXPR_OK:
        {
            log->info(L"Found the following folder expressions:");
            std::vector<std::wstring>::iterator it = folderExp->expressions.end();
            while (it != folderExp->expressions.begin())
            {
                --it;
                log->info((*it).c_str());
            }
            break;
        }
    case EXPR_ERROR_REM:
        {
            log->error(L"A remove expression is not correct. (remove $regex)");
            return 1;
        }
    case EXPR_ERROR_REP:
        {
            log->error(L"A replace expression is not correct. (replace $regex,'$value')");
            return 1;
        }
    }

    log->info(L"Interpreting file expressions.");
    Expression* fileExp = new Expression;
    res = fileExp->compile(config->fileContent);

    switch (res)
    {
    case EXPR_OK:
        {
            log->info(L"Found the following file expressions:");
            std::vector<std::wstring>::iterator it = fileExp->expressions.end();
            while (it != fileExp->expressions.begin())
            {
                --it;
                log->info((*it).c_str());
            }
            break;
        }
    case EXPR_ERROR_REM:
        {
            log->error(L"A remove expression is not correct. (remove $regex)");
            return 1;
        }
    case EXPR_ERROR_REP:
        {
            log->error(L"A replace expression is not correct. (replace $regex,'$value'");
            return 1;
        }
    }

    log->info(L"Processing file " + oriPath);

    std::wstring folder(oriFile);
    folderExp->run(folder);
    log->info(L"Resulting folder: " + folder);

    std::wstring file(oriFile);
    fileExp->run(file);
    log->info(L"Resulting file: " + file);

    wchar_t* path = new wchar_t[520];
    PathCombine(path, config->rootFolder.c_str(), folder.c_str());
    log->info(path);

    if (!PathFileExists(path))
    {
        if (move)
        {
            log->info(L"Creating folder...");
            if (!CreateDirectory(path, NULL))
            {
                log->error(L"Could not create directory");
                return 1;
            }
        }
    }

    PathCombine(path, path, file.c_str());
    log->info(path);

    if (move)
    {
        log->info(L"Moving file...");
    }
    else
    {
        log->info(L"Moving is disabled!");
    }

    log->stop();
    delete[] path;
    delete folderExp;
    delete fileExp;
    return 0;
}

void printUsage()
{
    MessageBox(NULL,
                   L"KongouFileMover.exe [-c:configfile] [-x] %filename%",
                   L"KongouFileMover usage",
                   MB_OK | MB_ICONINFORMATION);
}

bool initializeWindow(HINSTANCE instance)
{
    ZeroMemory(&windowClass, sizeof(WNDCLASSEX));
    windowClass.cbSize = sizeof(WNDCLASSEX);
    windowClass.style = CS_HREDRAW | CS_VREDRAW | CS_NOCLOSE;
    windowClass.lpfnWndProc = WndProc;
    windowClass.cbClsExtra = 0;
    windowClass.cbWndExtra = 0;
    windowClass.hInstance = instance;
    windowClass.hIcon = LoadIcon(instance, MAKEINTRESOURCE(IDI_ICON_APPLICATION));
    windowClass.hCursor = LoadCursor(NULL, IDC_ARROW);
    windowClass.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    windowClass.lpszMenuName = NULL;
    windowClass.lpszClassName = className;
    windowClass.hIconSm = LoadIcon(instance, MAKEINTRESOURCE(IDI_ICON_APPLICATION));

    if (!RegisterClassEx(&windowClass))
    {
        MessageBox(NULL, L"Window Registration Failed!", L"Error!", MB_ICONEXCLAMATION | MB_OK);
        return false;
    }

    RECT desktop;
    const HWND hDesktop = GetDesktopWindow();
    GetWindowRect(hDesktop, &desktop);

    int x = desktop.right - 240;
    int y = desktop.bottom - 120;

    HWND taskbar = FindWindow(L"Shell_TrayWnd", L"");
    if (taskbar != nullptr)
    {
        RECT rect;
        GetWindowRect(taskbar, &rect);
        x = rect.right - 240;
        y = rect.top - 120;
    }

    windowHandle = CreateWindowEx(
        WS_EX_CLIENTEDGE,
                        className,
                        L"KongouFileMover",
                        WS_POPUP,
                        x,
                        y,
                        240,
                        120,
                        NULL,
                        NULL,
                        instance,
                        NULL);

    if (windowHandle == nullptr)
    {
        MessageBox(NULL, L"Window Creation Failed!", L"Error!", MB_ICONEXCLAMATION | MB_OK);
        return false;
    }

    RECT client_rectangle;
    GetClientRect(windowHandle, &client_rectangle);
    int width = client_rectangle.right - client_rectangle.left;
    int height = client_rectangle.bottom - client_rectangle.top;

    progressHandle = CreateWindowEx(WS_EX_WINDOWEDGE | WS_EX_CLIENTEDGE,
                                                    PROGRESS_CLASS,
                                                    NULL,
                                                    WS_CHILD | WS_VISIBLE | PBS_SMOOTH,
                                                    (width - 220) / 2,
                                                    height - 25,
                                                    220,
                                                    20,
                                                    windowHandle,
                                                    NULL,
                                                    instance,
                                                    NULL);

    SendMessage(progressHandle, PBM_SETRANGE, 0, (LPARAM)MAKELPARAM(0, 100));
    SendMessage(progressHandle, PBM_SETPOS, 0, 50);
    return true;
}

LRESULT CALLBACK WndProc(HWND window, UINT message, WPARAM wparam, LPARAM lparam)
{
    switch (message)
    {
    case WM_INITDIALOG:
        return true;

    case WM_DESTROY:
    case WM_CLOSE:
        PostQuitMessage(0);
        break;

    default:
        return DefWindowProc(window, message, wparam, lparam);
    }

    return 0;
}
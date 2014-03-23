#include <windows.h>
#include <shlwapi.h>

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
            std::wstring configWString(configFile);
            std::wstring infoWString = L"Using " + configWString + L" as root folder";
            log->info(infoWString.c_str());
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
        PrintUsage();
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

    std::wstring folder(oriFile);
    folderExp->run(folder);
    log->info((L"Resulting folder: " + folder).c_str());

    std::wstring file(oriFile);
    fileExp->run(file);
    log->info((L"Resulting file: " + file).c_str());

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

        if(!MoveFile(oriPath.c_str(), path))
        {
            log->error(L"Failed to move file...");
            return 1;
        }
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

void PrintUsage()
{
    MessageBox(NULL,
                   L"KongouFileMover.exe [-c:configfile] [-x] %filename%",
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
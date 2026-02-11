#include <iostream>
#include <Windows.h>
#include "Application.h"
#include "ConfigLoader.h"
#include "Logger.h"

static Application* g_app = nullptr;

BOOL WINAPI ConsoleHandler(DWORD signal)
{
    if (signal == CTRL_C_EVENT)
    {
        if (g_app)
            g_app->RequestShutdown();
        return TRUE;
    }
    return FALSE;
}


int main()
{
    try
    {
        ConfigLoader loader;

        if (!loader.LoadFromFile("config/system.ini"))
        {
            Logger::GetInstance().Log("Config file 'config/system.ini' not found. Using default values.", LogLevel::WARNING);
        }
        else
        {
            Logger::GetInstance().Log("Config file loaded successfully.", LogLevel::INFO);
        }

        Application app;
        g_app = &app;

        SetConsoleCtrlHandler(ConsoleHandler, TRUE);

        app.Run();
    }
    catch (const std::exception& e)
    {
        Logger::GetInstance().Log(
            std::string("Unhandled exception: ") + e.what(),
            LogLevel::ERR
        );
        return 1;
    }
    catch (...)
    {
        Logger::GetInstance().Log("Unhandled unknown exception", LogLevel::ERR);
        return 1;
    }

    Logger::GetInstance().Shutdown();

    return 0;
}


#include <iostream>
#include "Application.h"
#include "ConfigLoader.h"
#include "Logger.h"

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
    return 0;
}


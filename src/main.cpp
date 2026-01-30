#include <iostream>
#include "Application.h"
#include "ConfigLoader.h"

int main()
{
    try
    {
        ConfigLoader loader;

        if (!loader.LoadFromFile("config/system.ini"))
        {
            std::cerr << "Config file not found. Using default values.\n";
        }

        Application app;
        app.Run();
    }
    catch (const std::exception& e)
    {
        std::cerr << "Unhandled exception: " << e.what() << std::endl;
        return 1;
    }
    catch (...)
    {
        std::cerr << "Unhandled unknown exception" << std::endl;
        return 1;
    }
    return 0;
}


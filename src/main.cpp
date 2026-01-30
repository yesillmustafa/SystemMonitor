#include <iostream>
#include <Application.h>

int main()
{
    try
    {
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


#include "ConsoleOutput.h"
#include <iostream>

void ConsoleOutput::Write(const std::string& formattedMessage)
{
	std::cout << formattedMessage << std::endl;
}

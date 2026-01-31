#pragma once

#include "ILogOutput.h"

class ConsoleOutput : public ILogOutput
{
public:
	void Write(const std::string& formattedMessage) override;
};
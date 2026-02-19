#pragma once
#include "Config.h"
#include <string>
#include <vector>

class ConfigValidator
{
public:
	static std::vector<std::string> Validate(const Config& config);
};
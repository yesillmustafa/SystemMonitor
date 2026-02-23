#pragma once
#include <string>

namespace ConfigError
{
    std::string InvalidValue(
        const std::string& section,
        const std::string& key,
        const std::string& value,
        const std::string& expected);

    std::string UnknownKey(const std::string& section, const std::string& key);

    std::string UnknownSection(const std::string& section);

    std::string MissingSection(const std::string& line);

    std::string ValidationError(const std::string& section, const std::string& message);

    std::string InvalidLine(const std::string& line);
}
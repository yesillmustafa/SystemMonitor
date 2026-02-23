#include "ConfigError.h"

namespace
{
    std::string Prefix()
    {
        return "[CONFIG] ";
    }
}

namespace ConfigError
{
    std::string InvalidValue(
        const std::string& section,
        const std::string& key,
        const std::string& value,
        const std::string& expected)
    {
        return Prefix() +
            "Section='" + section +
            "', Key='" + key +
            "': Invalid value '" + value +
            "'. Expected " + expected + ".";
    }

    std::string UnknownKey(const std::string& section, const std::string& key)
    {
        return Prefix() + "Section='" + section + "': Unknown key '" + key + "'.";
    }

    std::string UnknownSection(const std::string& section)
    {
        return Prefix() + "Unknown section '" + section + "'.";
    }

    std::string MissingSection(const std::string& line)
    {
        return Prefix() + "Key-value outside of section: '" + line + "'.";
    }

    std::string ValidationError(const std::string& section, const std::string& message)
    {
        return Prefix() + "Section='" + section + "': " + message;
    }
    std::string InvalidLine(const std::string& line)
    {
        return Prefix() + "Invalid line: '" + line + "'. Expected key=value.";
    }
}
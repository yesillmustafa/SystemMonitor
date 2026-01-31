#pragma once
#include <string>

class ILogOutput
{
public:
    virtual ~ILogOutput() = default;

    // Logger tarafýndan formatlanmýþ log satýrýný alýr
    virtual void Write(const std::string& formattedMessage) = 0;
};


#pragma once

#include <string>

class ConfigLoader
{
public:
    // system.ini gibi bir config dosyasýný yükler
    // baþarýlýysa true, hata varsa false döner
    bool LoadFromFile(const std::string& path);
};

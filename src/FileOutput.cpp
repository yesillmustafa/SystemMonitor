#include "FileOutput.h"
#include <filesystem>
#include <iostream>

FileOutput::FileOutput(const std::string& filePath)
    : m_filePath(filePath)
{
    OpenFileIfNeeded();
}

FileOutput::~FileOutput()
{
    if (m_file.is_open())
        m_file.close();
}

void FileOutput::OpenFileIfNeeded()
{
    if (!m_enabled)
        return;

    if (!m_file.is_open())
    {
        try
        {
            std::filesystem::path path(m_filePath);

            if (!path.parent_path().empty() &&
                !std::filesystem::exists(path.parent_path()))
            {
                std::filesystem::create_directories(path.parent_path());
            }

            m_file.open(m_filePath, std::ios::out | std::ios::app);

            if (!m_file)
            {
                std::cerr << "[LOGGER] FileOutput disabled. Could not open: "
                    << m_filePath << std::endl;
                m_enabled = false;   //  fallback: file output kapandý
            }
        }
        catch (const std::exception& e)
        {
            std::cerr << "[LOGGER] Exception while opening log file: "
                << e.what() << std::endl;
            m_enabled = false;
        }
    }
}

void FileOutput::Write(const std::string& formattedMessage)
{
    if (!m_enabled)
        return;

    OpenFileIfNeeded();

    if (m_file.is_open())
        m_file << formattedMessage << std::endl;
}
#include "FileOutput.h"
#include <filesystem>

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
    if (!m_file.is_open())
    {
        std::filesystem::path path(m_filePath);

        // logs/ gibi parent klasörü yoksa oluþtur
        if (!path.parent_path().empty() &&
            !std::filesystem::exists(path.parent_path()))
        {
            std::filesystem::create_directories(path.parent_path());
        }

        m_file.open(m_filePath, std::ios::out | std::ios::app);

        if (!m_file)
        {
            // Logger yüzünden programý çökertmek yerine fallback yap
            throw std::runtime_error("FileOutput: log file could not be opened: " + m_filePath);
        }
    }
}

void FileOutput::Write(const std::string& formattedMessage)
{
	OpenFileIfNeeded();
	m_file << formattedMessage << std::endl;
}
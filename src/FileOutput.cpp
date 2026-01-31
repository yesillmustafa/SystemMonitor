#include "FileOutput.h"

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
		m_file.open(m_filePath, std::ios::out | std::ios::app);
		if (!m_file)
		{
			throw std::runtime_error("FileOutput: log file could not be opened: " + m_filePath);
		}
	}
}

void FileOutput::Write(const std::string& formattedMessage)
{
	OpenFileIfNeeded();
	m_file << formattedMessage << std::endl;
}
#pragma once

#include "ILogOutput.h"
#include <fstream>
#include <string>

class FileOutput : public ILogOutput
{
public:
	explicit FileOutput(const std::string& filePath);
	~FileOutput();

	void Write(const std::string& formattedMessage) override;

private:
	
	std::ofstream m_file;
	std::string m_filePath;

	void OpenFileIfNeeded();
};
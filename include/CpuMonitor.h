#pragma once

#include <Windows.h>

class CpuMonitor
{
public:
	CpuMonitor();
	
	double GetUsage(); // CPU yüzdesi

private:

	ULONGLONG m_prevIdle;
	ULONGLONG m_prevKernel;
	ULONGLONG m_prevUser;

	ULONGLONG FileTimeToULL(const FILETIME& ft);

};
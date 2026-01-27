#include <CpuMonitor.h>

CpuMonitor::CpuMonitor()
{
	FILETIME idle, kernel, user;
	GetSystemTimes(&idle, &kernel, &user);

	m_prevIdle = FileTimeToULL(idle);
	m_prevKernel = FileTimeToULL(kernel);
	m_prevUser = FileTimeToULL(user);

}

ULONGLONG CpuMonitor::FileTimeToULL(const FILETIME& ft)
{
	return (static_cast<ULONGLONG>(ft.dwHighDateTime) << 32) | ft.dwLowDateTime;
}

double CpuMonitor::GetUsage()
{
	FILETIME idle, kernel, user;
	GetSystemTimes(&idle, &kernel, &user);

	ULONGLONG idleTime = FileTimeToULL(idle);
	ULONGLONG kernelTime = FileTimeToULL(kernel);
	ULONGLONG userTime = FileTimeToULL(user);

	ULONGLONG idleDiff = idleTime - m_prevIdle;
	ULONGLONG kernelDiff = kernelTime - m_prevKernel;
	ULONGLONG userDiff = userTime - m_prevUser;

	ULONGLONG total = kernelDiff + userDiff;
	double usage = 0.0;

	if (total > 0)
		usage = (1.0 - (double)idleDiff / total) * 100.0;

	m_prevIdle = idleTime;
	m_prevKernel = kernelTime;
	m_prevUser = userTime;

	return usage;
}
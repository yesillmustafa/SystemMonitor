#pragma once
#include <string>
#include <Windows.h>

struct ProcessInfo {
	DWORD pid;
	std::string name;
	double cpuUsage = 0;
	SIZE_T ramUsage = 0;
	bool accessDenied = false;
};
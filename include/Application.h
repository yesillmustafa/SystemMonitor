#pragma once

#include <Schedular.h>

class Application
{
public:
	Application();
	~Application();

	void Run();

private:
	Schedular m_schedular;
	bool m_running;
};
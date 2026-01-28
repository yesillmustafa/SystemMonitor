#pragma once

class IMonitor
{
public:
	virtual ~IMonitor() = default;

	virtual void Update() = 0;
};
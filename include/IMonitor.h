#pragma once

class IMonitor
{
public:
	virtual ~IMonitor() = default;

	virtual void Update() = 0;
	virtual std::string GetName() = 0;
	virtual double GetLastValue() = 0;
};
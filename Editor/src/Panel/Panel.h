#pragma once

#include <string>

class Panel
{
public:
	Panel(const std::string& name);
	virtual ~Panel();

	virtual void OnUpdate() = 0;

protected:
	std::string m_Name;
};
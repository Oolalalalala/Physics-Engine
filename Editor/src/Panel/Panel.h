#pragma once

#include <string>

class Panel
{
public:
	Panel(const std::string& name);
	virtual ~Panel();

	void SetOpen(bool isOpen) { m_IsOpen = isOpen; }

	virtual void OnUpdate() = 0;
	virtual void OnImGuiRender() = 0;

protected:
	std::string m_Name;
	bool m_IsOpen = true;
};
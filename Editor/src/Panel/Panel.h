#pragma once

#include <string>

class Panel
{
public:
	Panel(const std::string& name);
	virtual ~Panel();

	void SetOpen(bool isOpen) { m_IsOpen = isOpen; }
	bool GetIsFocused() { return m_IsFocused; }

	virtual void OnUpdate(float dt) = 0;
	virtual void OnImGuiRender() = 0;

protected:
	std::string m_Name;
	bool m_IsOpen = true;
	bool m_IsFocused = false;
};
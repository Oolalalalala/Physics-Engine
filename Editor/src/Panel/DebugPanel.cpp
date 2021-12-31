#include "pch.h"
#include "DebugPanel.h"

DebugPanel::DebugPanel(bool* drawColliderBorder)
	: Panel("Debug")
{
	m_DrawColliderBorder = drawColliderBorder;
}

DebugPanel::~DebugPanel()
{
}

void DebugPanel::OnUpdate()
{
}

void DebugPanel::OnImGuiRender()
{
	if (m_IsOpen)
	{
		ImGui::Begin(m_Name.c_str(), &m_IsOpen);
		ImGui::Checkbox("Show collider border", m_DrawColliderBorder);

		m_IsFocused = ImGui::IsWindowFocused();
		ImGui::End();
	}
}

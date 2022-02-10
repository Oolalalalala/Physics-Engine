#include "pch.h"
#include "DebugPanel.h"

DebugPanel::DebugPanel(Olala::Ref<SceneViewPanel> sceneViewPanel)
	: Panel("Debug")
{
	m_SceneViewPanel = sceneViewPanel;
}

DebugPanel::~DebugPanel()
{
}

void DebugPanel::OnUpdate(float dt)
{
	constexpr float updateInterval = .5f;

	m_AccumulatedFrametime += dt;

	if (m_AccumulatedFrametime > updateInterval)
	{
		m_FrameRate = 1.f / dt;
		m_AccumulatedFrametime -= updateInterval;
	}
}

void DebugPanel::OnImGuiRender()
{
	if (m_IsOpen)
	{
		ImGui::Begin(m_Name.c_str(), &m_IsOpen);

		ImGui::Text("FPS : %d", (int)m_FrameRate);
		if (ImGui::Checkbox("Show collider border", &m_DrawColliderBorder)) m_SceneViewPanel->SetDrawCollider(m_DrawColliderBorder);
		if (ImGui::Checkbox("VSync", &m_VSync)) Olala::Application::Get().GetWindow().SetVSync(m_VSync);
		
		m_IsFocused = ImGui::IsWindowFocused();
		ImGui::End();
	}
}

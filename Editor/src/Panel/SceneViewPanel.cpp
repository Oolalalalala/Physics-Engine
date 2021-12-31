#include "SceneViewPanel.h"

SceneViewPanel::SceneViewPanel(Olala::Entity camera)
	: Panel("Scene"), m_EditorCamera(camera)
{
}

SceneViewPanel::~SceneViewPanel()
{
}

void SceneViewPanel::OnUpdate()
{

}

void SceneViewPanel::OnImGuiRender()
{
	if (m_IsOpen)
	{
		ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.f);
		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, { 0.f, 0.f });
		ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.f);

		ImGui::Begin(m_Name.c_str(), &m_IsOpen);
		ImVec2 viewportPanelSize = ImGui::GetContentRegionAvail();

		auto& camera = m_EditorCamera.GetComponent<Olala::CameraComponent>();

		ImGui::Image((ImTextureID)camera.RenderTarget->GetColorBufferRendererId(), m_ViewportSize, { 0, 1 }, { 1, 0 });

		m_IsFocused = ImGui::IsWindowFocused();

		ImGui::End();
		ImGui::PopStyleVar(3);


		// Resize framebuffer and camera to fit the window
		if ((uint32_t)m_ViewportSize.x != (uint32_t)viewportPanelSize.x || (uint32_t)m_ViewportSize.y != (uint32_t)viewportPanelSize.y)
		{
			camera.RenderTarget->Resize((uint32_t)viewportPanelSize.x, (uint32_t)viewportPanelSize.y);
			if (camera.Camera->GetProjectionType() == Olala::CameraProjectionType::Perspective)
			{
				auto& specs = std::static_pointer_cast<Olala::PerspectiveCamera>(camera.Camera)->GetSpecs();
				specs.AspectRatio = viewportPanelSize.x / viewportPanelSize.y;
			}
			else
			{
				auto& specs = std::static_pointer_cast<Olala::OrthographicCamera>(camera.Camera)->GetSpecs();
				specs.Width = specs.Height * viewportPanelSize.x / viewportPanelSize.y;
			}
			camera.Camera->RecalculateProjectionMatrix();
		
			m_ViewportSize = viewportPanelSize;
		}
	}

}
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

		ImGuiWindowFlags flags = ImGuiWindowFlags_NoScrollbar;

		ImGui::Begin(m_Name.c_str(), &m_IsOpen, flags);
		ImGui::Image((ImTextureID)m_EditorCamera.GetComponent<Olala::CameraComponent>().RenderTarget->GetColorBufferRendererId(),
			ImGui::GetWindowSize(), { 0, 1 }, { 1, 0 });
		ImGui::End();

		ImGui::PopStyleVar(3);
	}

}
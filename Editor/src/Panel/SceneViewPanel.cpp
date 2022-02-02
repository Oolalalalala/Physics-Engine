#include "SceneViewPanel.h"

#include "glm/gtx/rotate_vector.hpp"

SceneViewPanel::SceneViewPanel(Olala::Ref<Olala::Scene> scene, Olala::Entity camera)
	: Panel("Scene"), m_Scene(scene), m_EditorCamera(camera)
{
}

SceneViewPanel::~SceneViewPanel()
{
}

void SceneViewPanel::OnUpdate(float dt)
{
	if (!m_Scene || !m_EditorCamera)
		return;

	auto& camera = m_EditorCamera.GetComponent<Olala::CameraComponent>(); 
	auto& framebufferSpecs = camera.RenderTarget->GetSpecs();
	if ((uint32_t)m_ViewportSize.x != framebufferSpecs.Width || (uint32_t)m_ViewportSize.y != framebufferSpecs.Height)
	{
		camera.RenderTarget->Resize((uint32_t)m_ViewportSize.x, (uint32_t)m_ViewportSize.y);
		if (camera.Camera->GetProjectionType() == Olala::CameraProjectionType::Perspective)
		{
			auto& specs = std::static_pointer_cast<Olala::PerspectiveCamera>(camera.Camera)->GetSpecs();
			specs.AspectRatio = m_ViewportSize.x / m_ViewportSize.y;
		}
		else
		{
			auto& specs = std::static_pointer_cast<Olala::OrthographicCamera>(camera.Camera)->GetSpecs();
			specs.Width = specs.Height * m_ViewportSize.x / m_ViewportSize.y;
		}
		camera.Camera->RecalculateProjectionMatrix();
	}
}

void SceneViewPanel::OnImGuiRender()
{
	if (m_IsOpen)
	{
		if (m_Scene && m_EditorCamera)
			OnOverlayRender();

		ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.f);
		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, { 0.f, 0.f });
		ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.f);

		ImGui::Begin(m_Name.c_str(), &m_IsOpen);
		if (m_Scene && m_EditorCamera)
		{
			ImVec2 viewportPanelSize = ImGui::GetContentRegionAvail();
			auto& camera = m_EditorCamera.GetComponent<Olala::CameraComponent>();
			ImGui::Image((ImTextureID)camera.RenderTarget->GetColorBufferRendererId(), m_ViewportSize, { 0, 1 }, { 1, 0 });

			m_ViewportSize = viewportPanelSize;
		}

		m_IsFocused = ImGui::IsWindowFocused();

		ImGui::PopStyleVar(3);
		ImGui::End();
	}

}

void SceneViewPanel::OnOverlayRender()
{
	if (m_DrawCollider)
	{
		Olala::RenderCommand::SetRenderTarget(m_EditorCamera.GetComponent<Olala::CameraComponent>().RenderTarget);
		Olala::Renderer2D::BeginScene(*m_EditorCamera.GetComponent<Olala::CameraComponent>().Camera);

		auto view = m_Scene->GetAllEntitiesWith<Olala::TransformComponent, Olala::CircleCollider2DComponent>();
		for (auto e : view)
		{
			auto [transform, cc2d] = view.get<Olala::TransformComponent, Olala::CircleCollider2DComponent>(e);

			Olala::Renderer2D::DrawCircle((glm::vec2)transform.Position + glm::rotate(cc2d.Center, glm::radians(transform.Rotation.z)),
				cc2d.Radius, 0.05f, glm::vec4(0.f, 1.f, 0.f, 1.f));
		}

		// TODO : implement box collider visualization

		Olala::Renderer2D::EndScene();
		Olala::RenderCommand::SetRenderTarget(nullptr);
	}
}

void SceneViewPanel::SetCamera(Olala::Entity camera)
{
	m_EditorCamera = camera;
}

void SceneViewPanel::SetScene(Olala::Ref<Olala::Scene> scene)
{
	m_Scene = scene;
}

void SceneViewPanel::SetDrawCollider(bool enabled)
{
	m_DrawCollider = enabled;
}

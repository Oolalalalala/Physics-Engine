#include "EditorLayer.h"


void EditorLayer::OnAttach()
{
	m_Scene = Olala::CreateRef<Olala::Scene>();

	// Editor camera
	Olala::Entity editorCamera = m_Scene->CreateEntity("Editor Camera");
	auto& cameraComponent = editorCamera.AddComponent<Olala::CameraComponent>(Olala::CreateRef<Olala::OrthographicCamera>());
	Olala::FramebufferSpecs specs;
	specs.Width = 800, specs.Height = 600;
	cameraComponent.RenderTarget = Olala::Framebuffer::Create(specs);

	m_SceneViewPanel = Olala::CreateRef<SceneViewPanel>(editorCamera);
	m_PropertyPanel = Olala::CreateRef<PropertyPanel>();
	m_SceneHierarchyPanel = Olala::CreateRef<SceneHierarchyPanel>(m_Scene, m_PropertyPanel);

	Olala::Ref<Olala::Texture2D> exampleTextures[2];
	exampleTextures[0] = Olala::Texture2D::Create("../Olala/Asset/Planet9.jpg");
	exampleTextures[1] = Olala::Texture2D::Create("../Olala/Asset/Abe.png");

	// Quads
	for (int i = 0; i < 8; i++)
	{
		Olala::Entity quad = m_Scene->CreateEntity("Quad" + std::to_string(i));
		quad.AddComponent<Olala::SpriteRendererComponent>(glm::vec2{ 80.f, 45.f }, glm::vec4{ 1.0f, 1.0f, 1.0f, 1.0f }, exampleTextures[i%2]);
		quad.GetComponent<Olala::TransformComponent>().Position.x = -350.f + 100.f * i;
	}

}

void EditorLayer::OnDetach()
{
}

void EditorLayer::OnUpdate(float dt)
{
	m_Scene->OnUpdate();
}

void EditorLayer::OnImGuiRender()
{
	// Background docking space
	{
		ImGui::SetNextWindowSize(ImGui::GetIO().DisplaySize);
		ImGui::SetNextWindowPos(ImGui::GetPlatformIO().Platform_GetWindowPos(ImGui::GetMainViewport()));

		ImGuiWindowFlags_ flags = (ImGuiWindowFlags_)(
			ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoBringToFrontOnFocus |
			ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize);// | ImGuiWindowFlags_NoBackground);

		ImGui::Begin("DockingGround", NULL, flags);
		ImGui::End();
	}

	// Panels
	m_SceneViewPanel->OnImGuiRender();
	//m_RuntimeViewPanel->OnImGuiRender();
	m_SceneHierarchyPanel->OnImGuiRender();
	m_PropertyPanel->OnImGuiRender();
}

void EditorLayer::OnEvent(Olala::Event& e)
{
}

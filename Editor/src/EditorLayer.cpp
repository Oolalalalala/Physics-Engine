#include "EditorLayer.h"

void EditorLayer::OnAttach()
{
	m_ExampleTextures[0] = Olala::Texture2D::Create("../Olala/Asset/Planet9.jpg");
	m_ExampleTextures[1] = Olala::Texture2D::Create("../Olala/Asset/Abe.png");

	m_Scene = Olala::CreateRef<Olala::Scene>();

	// Editor camera
	m_EditorCamera = m_Scene->CreateEntity("Editor Camera");
	auto& cameraComponent = m_EditorCamera.AddComponent<Olala::CameraComponent>(Olala::CreateRef<Olala::OrthographicCamera>());
	Olala::FramebufferSpecs specs;
	specs.Width = 800, specs.Height = 600;
	cameraComponent.RenderTarget = Olala::Framebuffer::Create(specs);

	// Quad 1
	for (int i = 0; i < 8; i++)
	{
		Olala::Entity quad = m_Scene->CreateEntity("Quad" + std::to_string(i));
		quad.AddComponent<Olala::SpriteRendererComponent>(glm::vec2{ 80.f, 45.f }, glm::vec4{ 1.0f, 1.0f, 1.0f, 1.0f }, m_ExampleTextures[i%2]);
		quad.GetComponent<Olala::TransformComponent>().Position.x = -350.f + 100.f * i;
	}

	m_Shader = Olala::Shader::Create("../Olala/Asset/Texture.glsl");

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
	{
		ImGui::SetNextWindowSize(ImGui::GetIO().DisplaySize);
		ImGui::SetNextWindowPos(ImGui::GetPlatformIO().Platform_GetWindowPos(ImGui::GetMainViewport()));

		ImGuiWindowFlags_ flags = (ImGuiWindowFlags_)(
			ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoBringToFrontOnFocus |
			ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize);// | ImGuiWindowFlags_NoBackground);

		ImGui::Begin("DockingGround", NULL, flags);
		ImGui::End();
	}

	{
		if (temp_b)
		{
			ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.f);
			ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, { 0.f, 0.f });
			ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.f);

			ImGuiWindowFlags_ flags = ImGuiWindowFlags_NoScrollbar;

			ImGui::Begin("Hello World", &temp_b, flags);
			ImGui::Image((ImTextureID)m_EditorCamera.GetComponent<Olala::CameraComponent>().RenderTarget->GetColorBufferRendererId(),
				ImGui::GetWindowSize(), { 0, 1 }, { 1, 0 });
			ImGui::End();

			ImGui::PopStyleVar(3);
		}

		ImGui::Begin("Switch", NULL);
		if (ImGui::Button("Button"))
			temp_b = true;
		ImGui::End();
	}

	//ImGui::ShowDemoWindow();
}

void EditorLayer::OnEvent(Olala::Event& e)
{
}

#include "EditorLayer.h"

void EditorLayer::OnAttach()
{
	m_ExampleTexture = Olala::Texture2D::Create("../Olala/Asset/Planet9.jpg");

	m_Scene = Olala::CreateRef<Olala::Scene>();

	// Editor camera
	m_EditorCamera = m_Scene->CreateEntity("Editor Camera");
	auto& cameraComponent = m_EditorCamera.AddComponent<Olala::CameraComponent>(Olala::CreateRef<Olala::OrthographicCamera>());
	Olala::FramebufferSpecs specs;
	specs.Width = 800, specs.Height = 600;
	cameraComponent.RenderTarget = nullptr;//Olala::Framebuffer::Create(specs);

	// Quad 1
	for (int i = 0; i < 30; i++)
	{
		Olala::Entity quad = m_Scene->CreateEntity("Quad" + std::to_string(i));
		quad.AddComponent<Olala::SpriteRendererComponent>(glm::vec2{ 80.f, 45.f }, glm::vec4{ 1.0f, 1.0f, 1.0f, 1.0f }, m_ExampleTexture);
		quad.GetComponent<Olala::TransformComponent>().Position.x = -300.f + 20.f * i; // doesnt return references
	}

	m_Shader = Olala::Shader::Create("../Olala/Asset/Texture.glsl");

}

void EditorLayer::OnDetach()
{
}

void EditorLayer::OnUpdate(float dt)
{
	m_Scene->OnUpdate();



	//Olala::RenderCommand::SetRenderTarget(nullptr);
	//
	//Olala::RenderCommand::SetClearColor({ 0.8f, 0.1f, 0.2f, 1.0f });
	//Olala::RenderCommand::Clear();
	//
	//Olala::OrthographicCamera camera;
	//Olala::Renderer2D::BeginScene(camera);
	//
	//m_Shader->Bind();
	//glBindTexture(GL_TEXTURE_2D, m_EditorCamera.GetComponent<Olala::CameraComponent>().RenderTarget->GetColorBufferRendererId());
	//Olala::Renderer2D::DrawQuad({ 0, 0 }, { 300, 300 }, { 1.0f, 1.0f, 1.0f, 1.0f });
	//
	//Olala::Renderer2D::EndScene();
}

void EditorLayer::OnEvent(Olala::Event& e)
{
}

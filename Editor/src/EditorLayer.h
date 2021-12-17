#pragma once
#include <Olala.h>

class EditorLayer : public Olala::Layer
{
public:
	EditorLayer() = default;
	~EditorLayer() = default;

	void OnAttach() override;
	void OnDetach() override;
	void OnUpdate(float dt) override;
	void OnEvent(Olala::Event& e) override;
	void OnImGuiRender() override;

private:
	Olala::Ref<Olala::Scene> m_Scene;
	Olala::Entity m_EditorCamera;

	Olala::Ref<Olala::Shader> m_Shader;
	Olala::Ref<Olala::Texture2D> m_ExampleTextures[2];

	float angle = 0.f;

	bool temp_b = true;
};


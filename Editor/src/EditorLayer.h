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

private:
	Olala::Ref<Olala::Scene> m_Scene;
	Olala::Entity m_EditorCamera;

	Olala::Ref<Olala::Shader> m_Shader;
	Olala::Ref<Olala::Texture2D> m_ExampleTexture;

	float angle = 0.f;
};


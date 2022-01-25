#pragma once

#include <Olala.h>

#include "Panel.h"

class SceneViewPanel : public Panel
{
public:
	SceneViewPanel(Olala::Ref<Olala::Scene> scene, Olala::Entity camera);
	~SceneViewPanel();

	void OnUpdate(float dt) override;
	void OnImGuiRender() override;

	void SetCamera(Olala::Entity camera);
	void SetScene(Olala::Ref<Olala::Scene> scene);
	void SetDrawCollider(bool enabled);
private:
	void OnOverlayRender();

private:
	Olala::Ref<Olala::Scene> m_Scene;
	Olala::Entity m_EditorCamera;
	bool m_DrawCollider = false;
	
	ImVec2 m_ViewportSize = ImVec2(100.f, 100.f);
};
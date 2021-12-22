#pragma once

#include <Olala.h>

#include "Panel.h"

class SceneViewPanel : public Panel
{
public:
	SceneViewPanel(Olala::Entity camera);
	~SceneViewPanel();

	void OnUpdate() override;
	void OnImGuiRender() override;
	
private:
	Olala::Entity m_EditorCamera;
	ImVec2 m_ViewportSize;
};
#pragma once

#include <Olala.h>
#include "Panel.h"

#include "SceneViewPanel.h"

class DebugPanel : public Panel
{
public:
	DebugPanel(Olala::Ref<SceneViewPanel> sceneViewPanel);
	~DebugPanel();


	void OnUpdate(float dt) override;
	void OnImGuiRender() override;

private:
	Olala::Ref<SceneViewPanel> m_SceneViewPanel;

	bool m_DrawColliderBorder = false;

	float m_FrameRate = 0.f;
	float m_AccumulatedFrametime = 0.f;
};
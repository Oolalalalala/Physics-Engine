#pragma once

#include <Olala.h>
#include "Panel/SceneViewPanel.h"
#include "Panel/PropertyPanel.h"
#include "Panel/SceneHierarchyPanel.h"
#include "Panel/RuntimeViewPanel.h"

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
	void DrawMenuBar();

private:
	Olala::Ref<Olala::Scene> m_Scene;

	// Panels
	Olala::Ref<SceneViewPanel> m_SceneViewPanel;
	Olala::Ref<RuntimeViewPanel> m_RuntimeViewPanel;
	Olala::Ref<SceneHierarchyPanel> m_SceneHierarchyPanel;
	Olala::Ref<PropertyPanel> m_PropertyPanel;
};


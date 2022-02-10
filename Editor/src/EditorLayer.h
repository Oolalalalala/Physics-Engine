#pragma once

#include "Panel/SceneViewPanel.h"
#include "Panel/PropertyPanel.h"
#include "Panel/SceneHierarchyPanel.h"
#include "Panel/AssetPanel.h"
#include "Panel/DebugPanel.h"

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
	void DrawMenuBar();
	void ProcessShortcuts();

	// Application Function
	void CreateNewScene();
	void OpenScene();
	void CloseScene();
	void SaveScene();
	void SaveSceneAsNew();
	void ImportAsset();

	void StartStopRuntime();

	void OnRuntimeBegin();
	void OnRuntimeEnd();

	bool OnKeyPressed(Olala::KeyPressedEvent& e);

private:
	Olala::Ref<Olala::Scene> m_Scene;
	Olala::Ref<Olala::Scene> m_RuntimeScene;
	Olala::Ref<Olala::SceneSerializer> m_SceneSerializer;

	// Panels
	Olala::Ref<SceneViewPanel> m_SceneViewPanel;
	Olala::Ref<SceneHierarchyPanel> m_SceneHierarchyPanel;
	Olala::Ref<PropertyPanel> m_PropertyPanel;
	Olala::Ref<AssetPanel> m_AssetPanel;
	Olala::Ref<DebugPanel> m_DebugPanel;

	Olala::Entity m_EditorCamera;

	bool m_IsRuntime = false;
	bool m_IsPausing = false;
	bool m_IsSceneLoaded = false;
	bool m_IsSceneSaved = false;
};


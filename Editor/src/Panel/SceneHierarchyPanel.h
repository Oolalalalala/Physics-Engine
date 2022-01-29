#pragma once
#include <Olala.h>

#include "Panel.h"
#include "PropertyPanel.h"

class SceneHierarchyPanel : public Panel
{
public:
	SceneHierarchyPanel(Olala::Ref<Olala::Scene>& scene, Olala::Ref<PropertyPanel>& propertyPanel);
	~SceneHierarchyPanel();

	void OnUpdate(float dt) override;
	void OnImGuiRender() override;

	void SetDisplayingScene(Olala::Ref<Olala::Scene> scene);

private:
	Olala::Ref<Olala::Scene> m_Scene;
	Olala::Ref<PropertyPanel> m_PropertyPanel;
	Olala::Entity m_SelectedEntity;
	bool m_SceneSelected = true;

	char m_SearchResult[25];
};

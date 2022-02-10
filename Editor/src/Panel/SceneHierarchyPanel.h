#pragma once
#include <Olala.h>
#include <unordered_set>

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
	void RemoveEntity(Olala::Entity entity);
	void SearchForEntities();

private:
	Olala::Ref<Olala::Scene> m_Scene;
	Olala::Ref<PropertyPanel> m_PropertyPanel;
	Olala::Entity m_SelectedEntity;
	bool m_SceneSelected = true;

	char m_SearchKeyword[25];
	std::unordered_set<entt::entity> m_SearchResults;
};

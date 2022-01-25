#include "SceneHierarchyPanel.h"
#include "../../Olala/src/Scene/Scene.h"

SceneHierarchyPanel::SceneHierarchyPanel(Olala::Ref<Olala::Scene>& scene, Olala::Ref<PropertyPanel>& propertyPanel)
	: Panel("Entities"), m_Scene(scene), m_PropertyPanel(propertyPanel)
{
}

SceneHierarchyPanel::~SceneHierarchyPanel()
{
}

void SceneHierarchyPanel::OnUpdate(float dt)
{
}

void SceneHierarchyPanel::OnImGuiRender()
{
	if (m_IsOpen)
	{
		ImGui::Begin(m_Name.c_str(), &m_IsOpen);

		m_Scene->m_Registry.each([&](auto entityID)
		{
			Olala::Entity entity(entityID, m_Scene.get());

			ImGuiTreeNodeFlags flags = (m_SelectedEntity == entity ? ImGuiTreeNodeFlags_Selected : 0) |
				ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_SpanFullWidth;
			bool opened = ImGui::TreeNodeEx(entity.GetComponent<Olala::TagComponent>().Tag.c_str(), flags);

			if (ImGui::IsItemClicked())
			{
				m_SelectedEntity = entity;
				m_PropertyPanel->SetDisplayedEntity(entity);
			}

			if (opened)
				ImGui::TreePop();
		});

		m_IsFocused = ImGui::IsWindowFocused();
		ImGui::End();
	}
}

void SceneHierarchyPanel::SetDisplayingScene(Olala::Ref<Olala::Scene>& scene)
{
	m_Scene = scene;
}

#include "SceneHierarchyPanel.h"


SceneHierarchyPanel::SceneHierarchyPanel(Olala::Ref<Olala::Scene>& scene, Olala::Ref<PropertyPanel>& propertyPanel)
	: Panel("Entities"), m_Scene(scene), m_PropertyPanel(propertyPanel), m_SearchResult("")
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
		if (m_Scene)
		{
			ImGui::Begin(m_Name.c_str(), &m_IsOpen, ImGuiWindowFlags_MenuBar);

			ImGui::BeginMenuBar();

			// Add Button
			float paddingX = 5.f, addButtonWidth = 50.f;
			ImGui::SetCursorPosX(paddingX);
			if (ImGui::Button("Add", ImVec2(addButtonWidth, 0.f)))
				m_PropertyPanel->DisplayEntity(m_Scene->CreateEntity("Entity"));
			
			ImGui::SetCursorPosX(paddingX * 2 + addButtonWidth);
			ImGui::PushItemWidth(ImGui::GetWindowSize().x - paddingX * 3 - addButtonWidth);
			ImGui::PushStyleColor(ImGuiCol_FrameBg, ImVec4(0.125f, 0.125f, 0.125f, 1.f));
			ImGui::InputTextWithHint("##SearchBar", "Search", m_SearchResult, 25);
			ImGui::PopStyleColor();
			ImGui::PopItemWidth();
			ImGui::EndMenuBar();
			
			if (ImGui::TreeNodeEx(m_Scene->GetName().c_str(), (m_SceneSelected ? ImGuiTreeNodeFlags_Selected : 0) | ImGuiTreeNodeFlags_OpenOnArrow
				                                              | ImGuiTreeNodeFlags_SpanFullWidth | ImGuiTreeNodeFlags_DefaultOpen))
			{
				if (ImGui::IsItemClicked())
				{
					m_SceneSelected = true;
					m_SelectedEntity = Olala::Entity();
					m_PropertyPanel->DisplayScene(m_Scene);
				}

				m_Scene->m_Registry.each([&](auto entityID)
				{
					Olala::Entity entity(entityID, m_Scene.get());

					ImGuiTreeNodeFlags flags = (m_SelectedEntity == entity ? ImGuiTreeNodeFlags_Selected : 0) |
						ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_SpanFullWidth;
					bool opened = ImGui::TreeNodeEx(entity.GetComponent<Olala::TagComponent>().Tag.c_str(), flags);

					if (ImGui::IsItemClicked())
					{
						m_SceneSelected = false;
						m_SelectedEntity = entity;
						m_PropertyPanel->DisplayEntity(entity);
					}

					if (opened)
						ImGui::TreePop();
				});

				ImGui::TreePop();
			}
		}
		else
			ImGui::Begin(m_Name.c_str(), &m_IsOpen);

		m_IsFocused = ImGui::IsWindowFocused();
		ImGui::End();
	}
}

void SceneHierarchyPanel::SetDisplayingScene(Olala::Ref<Olala::Scene> scene)
{
	m_Scene = scene;
}

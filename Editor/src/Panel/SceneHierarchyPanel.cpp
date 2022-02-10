#include "SceneHierarchyPanel.h"


SceneHierarchyPanel::SceneHierarchyPanel(Olala::Ref<Olala::Scene>& scene, Olala::Ref<PropertyPanel>& propertyPanel)
	: Panel("Entities"), m_Scene(scene), m_PropertyPanel(propertyPanel), m_SearchKeyword("")
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
			if (ImGui::InputTextWithHint("##SearchBar", "Search", m_SearchKeyword, 25))
				SearchForEntities();
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

				if (strlen(m_SearchKeyword))
				{
					for (auto entityID : m_SearchResults)
					{
						Olala::Entity entity(entityID, m_Scene.get());

						ImGuiTreeNodeFlags flags = (m_SelectedEntity == entity ? ImGuiTreeNodeFlags_Selected : 0) |
							ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_SpanFullWidth;
						bool opened = ImGui::TreeNodeEx(entity.GetComponent<Olala::TagComponent>().Tag.c_str(), flags);
						bool shouldRemove = false;

						if (ImGui::BeginPopupContextItem())
						{
							if (ImGui::MenuItem("Remove##entity"))
								shouldRemove = true;
							ImGui::EndPopup();
						}

						if (ImGui::IsItemClicked())
						{
							m_SceneSelected = false;
							m_SelectedEntity = entity;
							m_PropertyPanel->DisplayEntity(entity);
						}

						if (opened)
							ImGui::TreePop();

						if (shouldRemove)
							RemoveEntity(entity);
					}
				}
				else
				{
					m_Scene->m_Registry.each([&](auto entityID)
					{
						Olala::Entity entity(entityID, m_Scene.get());

						ImGuiTreeNodeFlags flags = (m_SelectedEntity == entity ? ImGuiTreeNodeFlags_Selected : 0) |
							ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_SpanFullWidth;
						bool opened = ImGui::TreeNodeEx(entity.GetComponent<Olala::TagComponent>().Tag.c_str(), flags);
						bool shouldRemove = false;

						if (ImGui::BeginPopupContextItem())
						{
							if (ImGui::MenuItem("Remove##entity"))
								shouldRemove = true;
							ImGui::EndPopup();
						}

						if (ImGui::IsItemClicked())
						{
							m_SceneSelected = false;
							m_SelectedEntity = entity;
							m_PropertyPanel->DisplayEntity(entity);
						}

						if (opened)
							ImGui::TreePop();

						if (shouldRemove)
							RemoveEntity(entity);
					});
				}

				ImGui::TreePop();
			}
		}
		else
			ImGui::Begin(m_Name.c_str(), &m_IsOpen);

		m_IsFocused = ImGui::IsWindowFocused();
		ImGui::End();
	}
}

static bool FindRegardlessOfCase(const std::string& str, const char* keyword)
{
	std::string lower(str), lowerKeyword(keyword);
	for (auto& ch : lower)
	{
		ch = tolower(ch);
	}
	for (auto& ch : lowerKeyword)
	{
		ch = tolower(ch);
	}

	if (lower.find(lowerKeyword) == std::string::npos)
		return false;

	return true;
}

void SceneHierarchyPanel::SearchForEntities()
{
	m_SearchResults.clear();
	
	if (strlen(m_SearchKeyword) == 0)
		return;

	m_Scene->m_Registry.each([&](auto entityID)
	{
		Olala::Entity entity(entityID, m_Scene.get());
		if (FindRegardlessOfCase(entity.GetComponent<Olala::TagComponent>().Tag, m_SearchKeyword))
			m_SearchResults.insert(entityID);
	});
}

void SceneHierarchyPanel::RemoveEntity(Olala::Entity entity)
{
	m_Scene->DestroyEntity(entity);

	if (entity == m_SelectedEntity)
	{
		m_SelectedEntity = Olala::Entity();
		m_PropertyPanel->DisplayEntity(Olala::Entity());
	}
}

void SceneHierarchyPanel::SetDisplayingScene(Olala::Ref<Olala::Scene> scene)
{
	m_Scene = scene;
}

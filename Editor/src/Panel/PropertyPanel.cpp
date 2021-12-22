#include "PropertyPanel.h"

PropertyPanel::PropertyPanel()
	: Panel("Property")
{
}

PropertyPanel::~PropertyPanel()
{
}

void PropertyPanel::OnUpdate()
{
}

void PropertyPanel::OnImGuiRender()
{
	if (m_IsOpen)
	{
		ImGui::Begin(m_Name.c_str(), &m_IsOpen);
		if (m_DisplayedEntity)
			DrawComponents();
		ImGui::End();
	}
}

void PropertyPanel::DrawComponents()
{
	auto& entity = m_DisplayedEntity;

	// Entity Name
	if (entity.HasComponent<Olala::TagComponent>())
	{
		auto& tag = m_DisplayedEntity.GetComponent<Olala::TagComponent>();
		ImGui::Text("Name");
		ImGui::SameLine();
		constexpr size_t maxLength = 30;
		char name[maxLength];
		strcpy(name, tag.Tag.c_str());
		ImGui::InputText("", name, maxLength);
		if (strlen(name))
			tag.Tag = name;
	}
	
	// Add component
	{
		ImGui::SameLine();
		if (ImGui::Button("Add"))
		{
			ImGui::OpenPopup("Add component");
		}

		constexpr char* componentList[] = { "Camera", "Sprite Renderer2D" };
		if (ImGui::BeginPopup("Add component"))
		{
			for (int i = 0; i < 2; i++)
			{
				if (ImGui::Selectable(componentList[i]))
				{
					switch (i)
					{
					case 0:
					{
						if (!m_DisplayedEntity.HasComponent<Olala::CameraComponent>())
						{
							auto& camera = m_DisplayedEntity.AddComponent<Olala::CameraComponent>();
							camera.Camera = Olala::CreateRef<Olala::PerspectiveCamera>();
						}
						break;
					}
					case 1:
					{
						if (!m_DisplayedEntity.HasComponent<Olala::SpriteRendererComponent>())
						{
							auto& sprite = m_DisplayedEntity.AddComponent<Olala::SpriteRendererComponent>();
						}
						break;
					}
					}
				}
			}
			ImGui::EndPopup();
		}
	}

	ImGui::Separator();
	ImGuiTreeNodeFlags componentFlags = ImGuiTreeNodeFlags_DefaultOpen;

	// Transform Component
	if (entity.HasComponent<Olala::TransformComponent>())
	{
		auto& transform = entity.GetComponent<Olala::TransformComponent>();

		if (ImGui::CollapsingHeader("Transform", componentFlags))
		{
			ImGui::InputFloat3("Position", (float*)&transform.Position, 2);
			ImGui::InputFloat3("Rotation", (float*)&transform.Rotation, 2);
			ImGui::InputFloat3("Scale", (float*)&transform.Scale, 2);
		}
	}

	// Camera Component
	if (entity.HasComponent<Olala::CameraComponent>())
	{
		auto& camera = entity.GetComponent<Olala::CameraComponent>();
		bool shouldRemove = false;

		constexpr const char* projectionTypes[] = { "Perspective", "Orthographic"};
		int selectedProjectionType = (int)camera.Camera->GetProjectionType();

		if (ImGui::CollapsingHeader("Camera", componentFlags))
		{
			ComponentFunctionality(&shouldRemove);
			ImGui::Checkbox("On", &camera.IsOn);
			ImGui::Checkbox("Main", &camera.IsMain);
			if (ImGui::BeginCombo("Projection type", projectionTypes[selectedProjectionType]))
			{
				for (int i = 0; i < 2; i++)
				{
					bool isSelected = selectedProjectionType == i;
					if (ImGui::Selectable(projectionTypes[i], &isSelected))
					{
						// Change the camera type
						if (i == (int)Olala::CameraProjectionType::Perspective)
						{
							camera.Camera = Olala::CreateRef<Olala::PerspectiveCamera>();
						}
						else
						{
							camera.Camera = Olala::CreateRef<Olala::OrthographicCamera>();
						}
					}
				}
				ImGui::EndCombo();
			}

			if (camera.Camera->GetProjectionType() == Olala::CameraProjectionType::Perspective)
			{
				auto& specs = std::static_pointer_cast<Olala::PerspectiveCamera>(camera.Camera)->GetSpecs();
				auto temp = specs;
				ImGui::InputFloat("FOV", &temp.Fov);
				ImGui::InputFloat("Aspect Ratio", &temp.AspectRatio);
				ImGui::InputFloat("Near Plane", &temp.NearPlane);
				ImGui::InputFloat("Far Plane", &temp.FarPlane);
				if (temp != specs)
				{
					specs = temp;
					camera.Camera->RecalculateProjectionMatrix();
				}
			}
			else
			{
				auto& specs = std::static_pointer_cast<Olala::OrthographicCamera>(camera.Camera)->GetSpecs();
				auto temp = specs;
				ImGui::InputFloat("Width", &temp.Width);
				ImGui::InputFloat("Height", &temp.Height);
				ImGui::InputFloat("Near Plane", &temp.NearPlane);
				ImGui::InputFloat("Far Plane", &temp.FarPlane);
				if (temp != specs)
				{
					specs = temp;
					camera.Camera->RecalculateProjectionMatrix();
				}
			}
		}
		else
			ComponentFunctionality(&shouldRemove);

		if (shouldRemove)
			m_DisplayedEntity.RemoveComponent<Olala::CameraComponent>();
	}

	// Sprite Renderer Component
	if (entity.HasComponent<Olala::SpriteRendererComponent>())
	{
		auto& sprite = entity.GetComponent<Olala::SpriteRendererComponent>();
		bool shouldRemove = false;

		if (ImGui::CollapsingHeader("Sprite", componentFlags))
		{
			ComponentFunctionality(&shouldRemove);
			ImGui::InputFloat2("Size", (float*)&sprite.Size, 2);
			ImGui::ColorEdit4("Color", (float*)&sprite.Color, 2);
		}
		else
			ComponentFunctionality(&shouldRemove);

		if (shouldRemove)
			m_DisplayedEntity.RemoveComponent<Olala::SpriteRendererComponent>();
	}
}

void PropertyPanel::ComponentFunctionality(bool* remove)
{
	if (ImGui::BeginPopupContextItem("Component Functionality"))
	{
		if (remove)	if (ImGui::Selectable("Remove")) *remove = true;
		ImGui::EndPopup();
	}
}

void PropertyPanel::SetDisplayedEntity(const Olala::Entity& entity)
{
	m_DisplayedEntity = entity;
}

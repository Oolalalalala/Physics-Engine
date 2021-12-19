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

	if (entity.HasComponent<Olala::TransformComponent>())
	{
		auto& transform = entity.GetComponent<Olala::TransformComponent>();

		if (ImGui::CollapsingHeader("Transform"))
		{
			ImGui::InputFloat3("Position", (float*)&transform.Position, 2);
			ImGui::InputFloat3("Rotation", (float*)&transform.Rotation, 2);
			ImGui::InputFloat3("Scale", (float*)&transform.Scale, 2);
		}
	}

	if (entity.HasComponent<Olala::CameraComponent>())
	{
		auto& camera = entity.GetComponent<Olala::CameraComponent>();

		constexpr const char* projectionTypes[] = { "Perspective", "Orthographic"};
		int selectedProjectionType = (int)camera.Camera->GetProjectionType();

		if (ImGui::CollapsingHeader("Camera"))
		{
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
				ImGui::InputFloat("Left", &temp.Left);
				ImGui::InputFloat("Right", &temp.Right);
				ImGui::InputFloat("Top", &temp.Top);
				ImGui::InputFloat("Bottom", &temp.Bottom);
				if (temp != specs)
				{
					specs = temp;
					camera.Camera->RecalculateProjectionMatrix();
				}
			}
		}
	}

	if (entity.HasComponent<Olala::SpriteRendererComponent>())
	{
		auto& sprite = entity.GetComponent<Olala::SpriteRendererComponent>();

		if (ImGui::CollapsingHeader("Sprite"))
		{
			ImGui::InputFloat2("Size", (float*)&sprite.Size, 2);
			ImGui::ColorEdit4("Color", (float*)&sprite.Color, 2);
		}
	}
}

void PropertyPanel::SetDisplayedEntity(const Olala::Entity& entity)
{
	m_DisplayedEntity = entity;
}

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
			DrawProperty();
		ImGui::End();
	}
}

void PropertyPanel::DrawProperty()
{
	auto& tag = m_DisplayedEntity.GetComponent<Olala::TagComponent>();
	constexpr size_t maxLength = 30;
	char name[maxLength];
	strcpy(name, tag.Tag.c_str());
	ImGui::InputText("Name", name, maxLength);
	if (strlen(name))
		tag.Tag = name;
}

void PropertyPanel::SetDisplayedEntity(const Olala::Entity& entity)
{
	m_DisplayedEntity = entity;
}

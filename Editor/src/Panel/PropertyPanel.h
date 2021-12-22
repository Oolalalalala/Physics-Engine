#pragma once
#include <Olala.h>

#include "Panel.h"

class PropertyPanel : public Panel
{
public:
	PropertyPanel();
	~PropertyPanel();

	void OnUpdate();
	void OnImGuiRender() override;

	void SetDisplayedEntity(const Olala::Entity& entity);

private:
	void DrawComponents();
	void ComponentFunctionality(bool* remove = nullptr);

private:
	Olala::Entity m_DisplayedEntity;
};
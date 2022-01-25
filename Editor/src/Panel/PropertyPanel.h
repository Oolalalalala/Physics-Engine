#pragma once
#include <Olala.h>

#include "Panel.h"

class PropertyPanel : public Panel
{
public:
	PropertyPanel();
	~PropertyPanel();

	void OnUpdate(float dt);
	void OnImGuiRender() override;

	void SetDisplayedEntity(const Olala::Entity& entity);

private:
	void DrawContext();

private:
	Olala::Entity m_DisplayedEntity;
};
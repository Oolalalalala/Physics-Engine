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

	void SetIsRuntime(bool isRuntime);
	void SetDisplayedEntity(const Olala::Entity& entity);

private:
	void DrawContext();
	void DrawContextRuntime(); // Properties in physics engine is updated instantly

private:
	Olala::Entity m_DisplayedEntity;
	bool m_IsRuntime = false;
};
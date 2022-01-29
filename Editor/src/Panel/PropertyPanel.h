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
	void DisplayEntity(const Olala::Entity& entity);
	void DisplayScene(Olala::Ref<Olala::Scene> scene);

private:
	void DrawEntityContext();
	void DrawEntityContextRuntime(); // Properties in physics engine is updated instantly
	void DrawSceneContext();

private:
	Olala::Entity m_DisplayedEntity;
	Olala::Ref<Olala::Scene> m_DisplayedScene;
	bool m_IsRuntime = false;
};
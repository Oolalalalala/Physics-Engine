#pragma once
#include <Olala.h>

#include "Panel.h"

class SceneHierarchyPanel : public Panel
{
public:
	SceneHierarchyPanel();
	~SceneHierarchyPanel();

	void OnUpdate() override;
};

#pragma once
#include <Olala.h>

#include "Panel.h"

class SceneViewPanel : public Panel
{
public:
	SceneViewPanel();
	~SceneViewPanel();

	void OnUpdate() override;
};
#pragma once
#include <Olala.h>

#include "Panel.h"

class PropertyPanel : public Panel
{
public:
	PropertyPanel();
	~PropertyPanel();

	void OnUpdate() override;
};
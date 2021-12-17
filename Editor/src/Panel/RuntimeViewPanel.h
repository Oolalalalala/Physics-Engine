#pragma once
#include <Olala.h>

#include "Panel.h"

class RuntimeViewPanel : public Panel
{
public:
	RuntimeViewPanel();
	~RuntimeViewPanel();

	void OnUpdate() override;
};
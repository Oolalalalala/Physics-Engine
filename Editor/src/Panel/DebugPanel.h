#pragma once

#include <Olala.h>
#include "Panel.h"

class DebugPanel : public Panel
{
public:
	DebugPanel(bool* drawColliderBorder);
	~DebugPanel();


	void OnUpdate() override;
	void OnImGuiRender() override;

private:
	bool* m_DrawColliderBorder;
};
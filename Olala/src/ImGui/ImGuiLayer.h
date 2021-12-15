#pragma once
#include "Core/Layer.h"

namespace Olala {

	class ImGuiLayer : public Layer
	{
	public:
		ImGuiLayer() = default;
		~ImGuiLayer() = default;

		void OnAttach() override;
		void OnUpdate(float dt) override;
		void OnDetach() override;
		void OnEvent(Event& e) override;

		void Begin();
		void End();
	};
}
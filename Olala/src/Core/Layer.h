#pragma once

#include "Event/Event.h"

namespace Olala {

	class Layer
	{
	public:
		Layer() = default;
		virtual ~Layer() = default;

		virtual void OnAttach() {}
		virtual void OnDetach() {}
		virtual void OnUpdate(float dt) {}
		virtual void OnEvent(Event& e) {}
		virtual void OnImGuiRender() {}
	};

}
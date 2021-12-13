#pragma once

#include "entt/entt.hpp"
#include "Renderer/Texture2D.h"

namespace Olala {

	class Entity;

	class Scene
	{
	public:
		Scene();
		~Scene();

		void OnUpdate();

		Entity CreateEntity(const std::string& name);

	private:
		entt::registry m_Registry;

		friend Entity;
	};

}


#pragma once

#include "entt/entt.hpp"

class SceneHierarchyPanel;

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

		friend class Entity;
		friend class ::SceneHierarchyPanel;
	};

}


#pragma once

#include "entt/entt.hpp"
#include "Physics.h"

class SceneHierarchyPanel;

namespace Olala {

	class Entity;

	class Scene
	{
	public:
		Scene();
		~Scene();

		void OnUpdate(float dt);

		Entity CreateEntity(const std::string& name);

	private:
		entt::registry m_Registry;
		Ref<PhysicsWorld> m_PhysicsWorld;

		friend class Entity;
		friend class ::SceneHierarchyPanel;
	};

}


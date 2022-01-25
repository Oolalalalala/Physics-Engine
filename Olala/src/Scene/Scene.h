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
		void OnRuntimeUpdate(float dt);

		Entity CreateEntity(const std::string& name);
		void DestroyEntity(Entity entity);

		void InitializePhysics();

		template<typename... Components>
		auto GetAllEntitiesWith()
		{
			return m_Registry.view<Components...>();
		}

		static Ref<Scene> Copy(Ref<Scene> source);

	private:
		entt::registry m_Registry;
		Ref<PhysicsWorld> m_PhysicsWorld;

		std::string m_Name = "Untitled";

		friend class Entity;
		friend class SceneSerializer;
		friend class ::SceneHierarchyPanel;
	};

}


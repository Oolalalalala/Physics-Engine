#pragma once

#include "entt/entt.hpp"
#include "AssetManager.h"
#include "Physics.h"

#include "box2d/box2d.h"

#define USE_BOX2D

class SceneHierarchyPanel;

namespace Olala {

	class Entity;

	class Scene
	{
	public:
		Scene();
		~Scene();

		void OnUpdate(float dt);
		void OnUpdateRuntime(float dt);

		Entity CreateEntity(const std::string& name);
		void DestroyEntity(Entity entity);

		void InitializePhysics();

		template<typename... Components>
		auto GetAllEntitiesWith()
		{
			return m_Registry.view<Components...>();
		}

		void Reset();

		Ref<AssetManager> GetAssetManager() { return m_AssetManager; }
		std::string& GetName() { return m_Name; }

		static Ref<Scene> Copy(Ref<Scene> source);

	private:
		entt::registry m_Registry;
		Ref<AssetManager> m_AssetManager;
		std::string m_Name = "Untitled";

	#ifdef USE_BOX2D
		Ref<b2World> m_PhysicsWorld;
	#else
		Ref<PhysicsWorld> m_PhysicsWorld;
	public:
		Ref<PhysicsWorld> GetPhysicsWorld() { return m_PhysicsWorld; }
	#endif

	private:
		friend class Entity;
		friend class SceneSerializer;
		friend class ::SceneHierarchyPanel;
	};

}


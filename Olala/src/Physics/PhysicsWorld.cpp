#include "pch.h"
#include "PhysicsWorld.h"

#include "Scene/Component.h"

namespace Olala {

	PhysicsWorld::PhysicsWorld(Scene& scene)
		: m_Scene(scene)
	{

	}

	PhysicsWorld::~PhysicsWorld()
	{

	}

	void PhysicsWorld::OnUpdate(float dt)
	{
		// Reset Forces
		{
			auto view = m_Scene.m_Registry.view<Rigidbody2DComponent>();
			for (auto entity : view)
			{
				auto& rigidbody2d = view.get<Rigidbody2DComponent>(entity);
				rigidbody2d.Force = rigidbody2d.ApplyGravity ? glm::vec2(0.f, rigidbody2d.Mass * -9.81f) : glm::vec2(0.f);
			}
		}
		///////////////////////////////////////////////////////////////////////////////////////////////
		////////////                    NEEDS TO TEST COLLISION HERE                      /////////////
		///////////////////////////////////////////////////////////////////////////////////////////////
		// Update position
		{
			auto view = m_Scene.m_Registry.view<TransformComponent, Rigidbody2DComponent>();
			for (auto entity : view)
			{
				auto [transform, rigidbody2d] = view.get<TransformComponent, Rigidbody2DComponent>(entity);
				rigidbody2d.Velocity += rigidbody2d.Force / rigidbody2d.Mass * dt;
				transform.Position.x += rigidbody2d.Velocity.x;
				transform.Position.y += rigidbody2d.Velocity.y;
			}
		}
	}

}
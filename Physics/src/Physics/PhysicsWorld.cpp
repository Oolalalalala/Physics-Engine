#include "PhysicsWorld.h"

#include "Collision.h"
#include "glm/gtx/projection.hpp"

#include <random>
#include <iostream>

namespace Olala {

	static std::random_device s_RandomDevice;
	static std::mt19937 s_Engine(s_RandomDevice());
	static std::uniform_int_distribution<uint32_t> s_UniformDistribution;

	PhysicsWorld::PhysicsWorld()
	{
	}

	PhysicsWorld::~PhysicsWorld()
	{
	}

	void PhysicsWorld::OnUpdate(float dt)
	{
		for (auto& [id, body] : m_PhysicsBodies)
		{
			if (!body.IsStatic && body.ApplyGravity)
				body.Velocity.y -= 9.81f * dt;
		}
		
		// Update Position
		for (auto& [id, body] : m_PhysicsBodies)
		{
			if (!body.IsStatic)
				body.Position += body.Velocity * dt;
		}

		CollisionData data;
		for (auto& i = m_PhysicsBodies.begin(); i != m_PhysicsBodies.end(); i++)
		{
			for (auto j = std::next(i); j != m_PhysicsBodies.end(); j++)
			{
				auto& bodyA = i->second, & bodyB = j->second;
				if (bodyA.IsStatic && bodyB.IsStatic) continue;

				if (Collision::TestCollision(bodyA, bodyB, &data))
				{
					//if (bodyA.Collider->Type == ColliderType::BoundingBox && bodyB.Collider->Type == ColliderType::BoundingBox)
					//{
						bodyA.Position -= data.Normal * data.Depth * 0.5f;
						bodyB.Position += data.Normal * data.Depth * 0.5f;
					//}
					//Collision::SolveCollision(bodyA, bodyB, dt);
				}
			}
		}
	}

	PhysicsID PhysicsWorld::CreatePhysicsBody(ColliderType type)
	{
		PhysicsID id;
		do {
			id = s_UniformDistribution(s_Engine);
		} while (m_PhysicsBodies.find(id) != m_PhysicsBodies.end() || id == 0);

		switch (type)
		{
		case ColliderType::None:            m_PhysicsBodies[id].Collider = nullptr;                      break;
		case ColliderType::BoundingBox:     m_PhysicsBodies[id].Collider = CreateRef<BoundingBox>();     break;
		case ColliderType::BoundingCircle:  m_PhysicsBodies[id].Collider = CreateRef<BoundingCircle>();  break;
		}

		return id;
	}

	void PhysicsWorld::RemovePhysicsBody(const PhysicsID& id)
	{
		m_PhysicsBodies.erase(id);
	}

	PhysicsBody& PhysicsWorld::GetPhysicsBody(const PhysicsID& id)
	{
		//if (id == 0) ; // error when id = 0
		// error when id does not exists
		return m_PhysicsBodies.at(id);
	}

}
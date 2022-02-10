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
			if (body.InvMass != 0.f && body.ApplyGravity)
				body.Velocity.y -= 9.81f * dt;
		}
		
		// Update Position
		for (auto& [id, body] : m_PhysicsBodies)
		{
			if (body.InvMass != 0.f)
			{
				body.Position += body.Velocity * dt;
				body.Rotation += body.AngularVelocity * dt;
			}
		}

		CollisionData data;
		for (auto& i = m_PhysicsBodies.begin(); i != m_PhysicsBodies.end(); i++)
		{
			for (auto j = std::next(i); j != m_PhysicsBodies.end(); j++)
			{
				auto& bodyA = i->second, & bodyB = j->second;
				if (bodyA.InvMass == 0.f && bodyB.InvMass == 0.f) continue;

				bool collided = false;
				
				if (bodyA.Collider->GetColliderType() == ColliderType::BoundingCircle && bodyB.Collider->GetColliderType() == ColliderType::BoundingCircle)
					collided = Collision::TestCollision<BoundingCircle, BoundingCircle>(bodyA, bodyB, &data);
				else if (bodyA.Collider->GetColliderType() == ColliderType::BoundingBox && bodyB.Collider->GetColliderType() == ColliderType::BoundingBox)
					collided = Collision::TestCollision<BoundingBox, BoundingBox>(bodyA, bodyB, &data);
				else if (bodyA.Collider->GetColliderType() == ColliderType::BoundingBox && bodyB.Collider->GetColliderType() == ColliderType::BoundingCircle)
					collided = Collision::TestCollision<BoundingBox, BoundingCircle>(bodyA, bodyB, &data);
				else if (bodyA.Collider->GetColliderType() == ColliderType::BoundingCircle && bodyB.Collider->GetColliderType() == ColliderType::BoundingBox)
					collided = Collision::TestCollision<BoundingCircle, BoundingBox>(bodyA, bodyB, &data);
				
				if (collided)
				{
					if (bodyA.InvMass == 0.f) 
						bodyB.Position += data.Normal * data.Depth;
					else if (bodyB.InvMass == 0.f)
						bodyA.Position -= data.Normal * data.Depth;
					else
					{
						// Need fixes
						float vA = glm::dot(data.Normal, bodyA.Velocity), vB = glm::dot(data.Normal, bodyB.Velocity);
						bodyA.Position -= data.Normal * std::abs(vA / (vB - vA));
						bodyB.Position += data.Normal * std::abs(vB / (vA - vB));
					}

					float e = std::min(bodyA.Restitution, bodyB.Restitution); // There are many ways of calculating combined restitution
					float j = -(1.f + e) * glm::dot(bodyB.Velocity - bodyA.Velocity, data.Normal) / (bodyA.InvMass + bodyB.InvMass);
					bodyA.Velocity -= j * bodyA.InvMass * data.Normal;
					bodyB.Velocity += j * bodyB.InvMass * data.Normal;
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
		if (id == 0) return;

		m_PhysicsBodies.erase(id);
	}

	PhysicsBody& PhysicsWorld::GetPhysicsBody(const PhysicsID& id)
	{
		//if (id == 0) ; // error when id = 0
		// error when id does not exists

		return m_PhysicsBodies.at(id);
	}

}
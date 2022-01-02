#include "pch.h"
#include "PhysicsWorld.h"

#include "Scene/Component.h"
#include "Scene/Entity.h"
#include "Collision.h"

#include <random>

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
		
		// Update Position
		for (auto& i : m_PhysicsObjects)
		{
			PhysicsObject& object = i.second;
			if (object.IsStatic)
				object.GetPosition() += object.GetVelocity();
		}
	}

	PhysicsID PhysicsWorld::CreatePhysicsObject()
	{
		PhysicsID id;
		do {
			id = s_UniformDistribution(s_Engine);
		} while (m_PhysicsObjects.find(id) == m_PhysicsObjects.end());

		return id;
	}

	void PhysicsWorld::RemovePhysicsObject(PhysicsID id)
	{
		m_PhysicsObjects.erase(id);
	}

	PhysicsObject& PhysicsWorld::GetPhysicsObject(PhysicsID id)
	{
		// error when id does not exists
		return m_PhysicsObjects.at(id);
	}

}
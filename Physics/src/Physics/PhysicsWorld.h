#pragma once

#include "PhysicsBody.h"
#include <unordered_map>

namespace Olala {

	typedef uint32_t PhysicsID;

	class PhysicsWorld
	{
	public:
		PhysicsWorld();
		~PhysicsWorld();

		void OnUpdate(float dt);

		PhysicsID CreatePhysicsBody(ColliderType type = ColliderType::None);
		void RemovePhysicsBody(const PhysicsID& id);

		PhysicsBody& GetPhysicsBody(const PhysicsID& id);

	private:
		std::unordered_map<PhysicsID, PhysicsBody> m_PhysicsBodies;
	};

}
#pragma once

#include "PhysicsObject.h"

namespace Olala {

	typedef uint32_t PhysicsID;

	class PhysicsWorld
	{
	public:
		PhysicsWorld();
		~PhysicsWorld();

		void OnUpdate(float dt);

		PhysicsID CreatePhysicsObject();
		void RemovePhysicsObject(PhysicsID id);

		PhysicsObject& GetPhysicsObject(PhysicsID id);

	private:
		std::unordered_map<PhysicsID, PhysicsObject> m_PhysicsObjects;
	};

}
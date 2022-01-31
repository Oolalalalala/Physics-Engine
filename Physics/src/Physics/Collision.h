#pragma once

#include "PhysicsBody.h"

#include <utility>
#include "glm/glm.hpp"

namespace Olala {

	struct CollisionData
	{
		float Depth;
		glm::vec2 Normal;
	};

	class Collision
	{
	public:
		static bool TestCollision(const PhysicsBody& a, const PhysicsBody& b, CollisionData* data); // Normal pointing from A to B
	};

}
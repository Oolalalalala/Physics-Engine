#pragma once

#include "PhysicsBody.h"

#include <utility>
#include "glm/glm.hpp"

namespace Olala {

	class Collision
	{
	public:
		static std::pair<bool, glm::vec2> TestCollision(const PhysicsBody& a, const PhysicsBody& b);
	};

}
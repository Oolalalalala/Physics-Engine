#pragma once

#include "PhysicsBody.h"

namespace Olala {

	class Collision
	{
	public:
		static bool TestCollision(const PhysicsBody& a, const PhysicsBody& b);
	};

}
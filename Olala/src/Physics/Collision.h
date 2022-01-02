#pragma once

#include "PhysicsObject.h"

namespace Olala {

	class Collision
	{
	public:
		static bool TestCollision(const PhysicsObject& a, const PhysicsObject& b);
	};

}
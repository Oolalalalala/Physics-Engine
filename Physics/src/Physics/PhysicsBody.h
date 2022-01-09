#pragma once

#include "Base.h"
#include "Collider.h"

#include "glm/glm.hpp"

namespace Olala {

	// Position is set to the center of mass
	struct PhysicsBody
	{
		glm::vec2 Position = glm::vec2(0.f);
		glm::vec2 Velocity = glm::vec2(0.f);
		float Rotation = 0.f;
		float Mass = 1.f;
		Ref<Collider> Collider;

		bool IsStatic = false;
		bool ApplyGravity = true;

		void SetColliderType(ColliderType type)
		{
			switch (type)
			{
			case ColliderType::None:            Collider = nullptr;                      break;
			case ColliderType::BoundingBox:     Collider = CreateRef<BoundingBox>();     break;
			case ColliderType::BoundingCircle:  Collider = CreateRef<BoundingCircle>();  break;
			default: break; //error
			}
		}
	};

}
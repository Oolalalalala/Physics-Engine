#include "Collision.h"

#include "Collider.h"
#include "glm/gtx/norm.hpp"

namespace Olala {

	std::pair<bool, glm::vec2> Collision::TestCollision(const PhysicsBody& a, const PhysicsBody& b)
	{
		if (a.Collider->Type == ColliderType::BoundingCircle && b.Collider->Type == ColliderType::BoundingCircle)
		{
			float collideDistance = std::static_pointer_cast<BoundingCircle>(a.Collider)->Radius + 
				                    std::static_pointer_cast<BoundingCircle>(b.Collider)->Radius;

			if (glm::distance2(a.Position + a.Collider->Offset, b.Position + b.Collider->Offset) <= collideDistance * collideDistance)
				return { true, (a.Position + a.Collider->Offset + b.Position + b.Collider->Offset) * 0.5f };

			return { false, glm::vec2(0.f) };
		}
		else if (a.Collider->Type == ColliderType::BoundingBox && b.Collider->Type == ColliderType::BoundingBox)
		{
			return { false, glm::vec2(0.f) };
		}
		else if (a.Collider->Type == ColliderType::BoundingBox && b.Collider->Type == ColliderType::BoundingCircle)
		{
			return { false, glm::vec2(0.f) };
		}
		else if (a.Collider->Type == ColliderType::BoundingCircle && b.Collider->Type == ColliderType::BoundingBox)
		{
			return { false, glm::vec2(0.f) };
		}
		return { false, glm::vec2(0.f) };
	}

}
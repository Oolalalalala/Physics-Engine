#include "Collision.h"

#include "Collider.h"
#include "glm/gtx/norm.hpp"

namespace Olala {

	bool Collision::TestCollision(const PhysicsBody& a, const PhysicsBody& b)
	{
		if (a.Collider->Type == ColliderType::BoundingCircle && b.Collider->Type == ColliderType::BoundingCircle)
		{
			float collideDistance = std::static_pointer_cast<BoundingCircle>(a.Collider)->Radius + 
				                    std::static_pointer_cast<BoundingCircle>(b.Collider)->Radius;
			return glm::distance2(a.Position + a.Collider->Offset, b.Position + b.Collider->Offset) <= collideDistance * collideDistance;
		}
		else if (a.Collider->Type == ColliderType::BoundingBox && b.Collider->Type == ColliderType::BoundingBox)
		{
			return false;
		}
		else if (a.Collider->Type == ColliderType::BoundingBox && b.Collider->Type == ColliderType::BoundingCircle)
		{
			return false;
		}
		else if (a.Collider->Type == ColliderType::BoundingCircle && b.Collider->Type == ColliderType::BoundingBox)
		{
			return false;
		}
		return false;
	}

}
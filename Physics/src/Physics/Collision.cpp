#include "Collision.h"

#include "Collider.h"
#include "glm/gtx/norm.hpp"
#include "glm/gtx/projection.hpp"
#include <limits>

namespace Olala {

	static std::pair<float, float> ProjectVertices(const glm::vec2& axis, const std::vector<glm::vec2>& points, const glm::vec2 offset, const float& rotation)
	{
		float min = FLT_MAX, max = -FLT_MAX, proj;
		for (auto& p : points)
		{
			proj = glm::dot(axis, offset + p);
			min = std::min(min, proj);
			max = std::max(max, proj);
		}
		return { min, max };
	}

	static int FindBoxClosestPointToCircle(const glm::vec2 points[4], const glm::vec2 offset, const float& rotation, const glm::vec2& circlePos)
	{
		float minDist2 = FLT_MAX, idx = 0, dist2;
		for (int i = 0; i < 4; i++)
		{
			dist2 = glm::distance2(points[i], circlePos);
			minDist2 = std::min(minDist2, dist2);
			idx = i;
		}
		return idx;
	}

	bool Collision::TestCollision(const PhysicsBody& bodyA, const PhysicsBody& bodyB, CollisionData* data)
	{
		if (bodyA.Collider->GetColliderType() == ColliderType::BoundingCircle && bodyB.Collider->GetColliderType() == ColliderType::BoundingCircle)
		{
			float collideDistance = std::static_pointer_cast<BoundingCircle>(bodyA.Collider)->Radius +
			                        std::static_pointer_cast<BoundingCircle>(bodyB.Collider)->Radius;

			if (glm::distance2(bodyA.Position + bodyA.Collider->Offset, bodyB.Position + bodyB.Collider->Offset) >= collideDistance * collideDistance)
				return false;

			data->Depth = collideDistance - glm::distance(bodyA.Position + bodyA.Collider->Offset, bodyB.Position + bodyB.Collider->Offset);
			data->Normal = glm::normalize(bodyB.Position - bodyA.Position);

			return true;
		}
		else if (bodyA.Collider->GetColliderType() == ColliderType::BoundingBox && bodyB.Collider->GetColliderType() == ColliderType::BoundingBox)
		{
			auto boxA = std::static_pointer_cast<BoundingBox>(bodyA.Collider);
			auto boxB = std::static_pointer_cast<BoundingBox>(bodyB.Collider);

			glm::vec2 axises[] = { glm::normalize(boxA->Points[1] - boxA->Points[0]), glm::normalize(boxA->Points[2] - boxA->Points[1]),
								   glm::normalize(boxB->Points[1] - boxB->Points[0]), glm::normalize(boxB->Points[2] - boxB->Points[1]) };

			float minDepth = FLT_MAX, depth, proj;
			glm::vec2 normal(0.f);

			float minA, maxA, minB, maxB;
			for (int i = 0; i < 4; i++)
			{
				minA = FLT_MAX, maxA = -FLT_MAX, minB = FLT_MAX, maxB = -FLT_MAX;
				// TODO : apply rotation
				for (int j = 0; j < 4; j++)
				{
					proj = glm::dot(axises[i], bodyA.Position +boxA->Offset + boxA->Points[j]);
					minA = std::min(minA, proj);
					maxA = std::max(maxA, proj);
				}

				for (int j = 0; j < 4; j++)
				{
					proj = glm::dot(axises[i], bodyB.Position + boxB->Offset + boxB->Points[j]);
					minB = std::min(minB, proj);
					maxB = std::max(maxB, proj);
				}
				
				if (minA > maxB || minB > maxA)
					return false;

				depth = std::min(std::abs(maxA - minB), std::abs(maxB - minA));
				if (depth < minDepth)
				{
					minDepth = depth;
					normal = axises[i];
				}
			}

			data->Depth = minDepth;
			data->Normal = glm::dot(bodyB.Position - bodyA.Position, normal) > 0.f ? normal : -normal;

			return true;
		}
		else if (bodyA.Collider->GetColliderType() == ColliderType::BoundingBox && bodyB.Collider->GetColliderType() == ColliderType::BoundingCircle)
		{
			auto box = std::static_pointer_cast<BoundingBox>(bodyA.Collider);
			auto circle = std::static_pointer_cast<BoundingCircle>(bodyB.Collider);

			glm::vec2 axises[] = { glm::normalize(box->Points[1] - box->Points[0]), glm::normalize(box->Points[2] - box->Points[1]) };

			float minDepth = FLT_MAX, depth;
			glm::vec2 normal(0.f);

			for (int i = 0; i < 2; i++)
			{
				// todo : apply rotation
				float minBox = glm::dot(axises[i], bodyA.Position + box->Offset + box->Points[0]);
				float maxBox = glm::dot(axises[i], bodyA.Position + box->Offset + box->Points[2]);
				if (minBox > maxBox) std::swap(minBox, maxBox);

				float circleCenter = glm::dot(axises[i], bodyB.Position + circle->Offset);
				float minCircle = circleCenter - circle->Radius;
				float maxCircle = circleCenter + circle->Radius;

				if (minBox > maxCircle || minCircle > maxBox)
					return false;

				depth = std::min(std::abs(maxBox - minCircle), std::abs(maxCircle - minBox));
				if (depth < minDepth)
				{
					minDepth = depth;
					normal = axises[i];
				}
			}

			{
				glm::vec2 axis = glm::normalize(bodyB.Position - 
					box->Points[FindBoxClosestPointToCircle(box->Points, bodyA.Position + box->Offset, bodyA.Rotation, bodyB.Position + circle->Offset)]);
				float minBox = FLT_MAX, maxBox = -FLT_MAX, proj;
				for (int i = 0; i < 4; i++)
				{
					proj = glm::dot(axis, bodyA.Position + box->Offset + box->Points[i]);
					minBox = std::min(minBox, proj);
					maxBox = std::max(maxBox, proj);
				}

				proj = glm::dot(axis, bodyB.Position + circle->Offset);
				float minCircle = proj - circle->Radius;
				float maxCircle = proj + circle->Radius;

				if (minBox > maxCircle || minCircle > maxBox)
					return false;

				depth = std::min(std::abs(maxBox - minCircle), std::abs(maxCircle - minBox));
				if (depth < minDepth)
				{
					minDepth = depth;
					normal = axis;
				}
			}

			data->Depth = minDepth;
			data->Normal = glm::dot(bodyB.Position - bodyA.Position, normal) > 0.f ? normal : -normal;

			return true;
		}
		else if (bodyA.Collider->GetColliderType() == ColliderType::BoundingCircle && bodyB.Collider->GetColliderType() == ColliderType::BoundingBox)
		{
			auto circle = std::static_pointer_cast<BoundingCircle>(bodyA.Collider);
			auto box = std::static_pointer_cast<BoundingBox>(bodyB.Collider);

			glm::vec2 axises[] = { glm::normalize(box->Points[1] - box->Points[0]), glm::normalize(box->Points[2] - box->Points[1]) };

			float minDepth = FLT_MAX, depth;
			glm::vec2 normal(0.f);

			for (int i = 0; i < 2; i++)
			{
				// todo : apply rotation
				float minBox = glm::dot(axises[i], bodyB.Position + box->Offset + box->Points[0]);
				float maxBox = glm::dot(axises[i], bodyB.Position + box->Offset + box->Points[2]);
				if (minBox > maxBox) std::swap(minBox, maxBox);

				float circleCenter = glm::dot(axises[i], bodyA.Position + circle->Offset);
				float minCircle = circleCenter - circle->Radius;
				float maxCircle = circleCenter + circle->Radius;

				if (minBox > maxCircle || minCircle > maxBox)
					return false;

				depth = std::min(std::abs(maxBox - minCircle), std::abs(maxCircle - minBox));
				if (depth < minDepth)
				{
					minDepth = depth;
					normal = axises[i];
				}
			}

			{
				glm::vec2 axis = glm::normalize(bodyA.Position -
					box->Points[FindBoxClosestPointToCircle(box->Points, bodyB.Position + box->Offset, bodyB.Rotation, bodyA.Position + circle->Offset)]);
				float minBox = FLT_MAX, maxBox = -FLT_MAX, proj;
				for (int i = 0; i < 4; i++)
				{
					proj = glm::dot(axis, bodyB.Position + box->Offset + box->Points[i]);
					minBox = std::min(minBox, proj);
					maxBox = std::max(maxBox, proj);
				}

				proj = glm::dot(axis, bodyA.Position + circle->Offset);
				float minCircle = proj - circle->Radius;
				float maxCircle = proj + circle->Radius;

				if (minBox > maxCircle || minCircle > maxBox)
					return false;

				depth = std::min(std::abs(maxBox - minCircle), std::abs(maxCircle - minBox));
				if (depth < minDepth)
				{
					minDepth = depth;
					normal = axis;
				}
			}

			data->Depth = minDepth;
			data->Normal = glm::dot(bodyB.Position - bodyA.Position, normal) > 0.f ? normal : -normal;

			return true;
		}
		// Separate Axis Theorem
		else if (bodyA.Collider->GetColliderType() == ColliderType::BoundingPolygon && bodyB.Collider->GetColliderType() == ColliderType::BoundingPolygon)
		{
			auto polygonA = std::static_pointer_cast<BoundingPolygon>(bodyA.Collider);
			auto polygonB = std::static_pointer_cast<BoundingPolygon>(bodyB.Collider);
			
			// assert
			if (polygonA->Points.size() < 3 || polygonB->Points.size() < 3) return false;

			float minDepth = FLT_MAX, depth;
			glm::vec2 normal(0.f);

			for (int i = 0; i < (int)polygonA->Points.size(); i++)
			{
				glm::vec2 edge = polygonA->Points[(i + 1) % polygonA->Points.size()] - polygonA->Points[i];
				glm::vec2 axis(-edge.y, edge.x);
				axis = glm::normalize(axis);
				
				// TODO : Apply rotation
				auto [minA, maxA] = ProjectVertices(axis, polygonA->Points, bodyA.Position + polygonA->Offset, 0.f);
				auto [minB, maxB] = ProjectVertices(axis, polygonB->Points, bodyB.Position + polygonB->Offset, 0.f);

				if (minA > maxB || minB > maxA)
					return false;

				depth = std::min(std::abs(maxA - minB), std::abs(maxB - minA));
				if (depth < minDepth)
				{
					minDepth = depth;
					normal = axis;
				}
			}
			for (int i = 0; i < (int)polygonB->Points.size(); i++)
			{
				glm::vec2 edge = polygonB->Points[(i + 1) % polygonB->Points.size()] - polygonB->Points[i];
				glm::vec2 axis(-edge.y, edge.x);
				axis = glm::normalize(axis);

				// TODO : Apply rotation
				auto [minA, maxA] = ProjectVertices(axis, polygonA->Points, bodyA.Position + polygonA->Offset, 0.f);
				auto [minB, maxB] = ProjectVertices(axis, polygonB->Points, bodyB.Position + polygonB->Offset, 0.f);

				if (minA > maxB || minB > maxA)
					return false;

				depth = std::min(std::abs(maxA - minB), std::abs(maxB - minA));
				if (depth < minDepth)
				{
					minDepth = depth;
					normal = axis;
				}
			}

			data->Depth = minDepth;
			data->Normal = glm::dot(bodyB.Position - bodyA.Position, normal) > 0.f ? normal : -normal;

			return true;
		}

		return false;
	}

}
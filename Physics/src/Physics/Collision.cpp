#include "Collision.h"

#include "Collider.h"
#include "glm/gtx/norm.hpp"
#include "glm/gtx/projection.hpp"
#include "glm/gtx/rotate_vector.hpp"
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



	static int FindBoxClosestPointToCircle(const glm::vec2 points[4], const glm::vec2& circlePos)
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

	template<typename A, typename B>
	bool Collision::TestCollision(const PhysicsBody& bodyA, const PhysicsBody& bodyB, CollisionData* data)
	{
		assert(false);
	}


	template<>
	bool Collision::TestCollision<BoundingCircle, BoundingCircle>(const PhysicsBody& bodyA, const PhysicsBody& bodyB, CollisionData* data)
	{
		float collideDistance = std::static_pointer_cast<BoundingCircle>(bodyA.Collider)->Radius +
			                    std::static_pointer_cast<BoundingCircle>(bodyB.Collider)->Radius;

		glm::vec2 posA = bodyA.Position + glm::rotate(bodyA.Collider->Offset, bodyA.Rotation);
		glm::vec2 posB = bodyB.Position + glm::rotate(bodyB.Collider->Offset, bodyB.Rotation);

		if (glm::distance2(posA, posB) >= collideDistance * collideDistance)
			return false;

		data->Depth = collideDistance - glm::distance(posA, posB);
		data->Normal = glm::normalize(posB - posA);
		

		return true;
	}


	template<>
	bool Collision::TestCollision<BoundingBox, BoundingBox>(const PhysicsBody& bodyA, const PhysicsBody& bodyB, CollisionData* data)
	{
		auto boxA = std::static_pointer_cast<BoundingBox>(bodyA.Collider);
		auto boxB = std::static_pointer_cast<BoundingBox>(bodyB.Collider);

		glm::vec2 pA[4], pB[4], posA = bodyA.Position + glm::rotate(boxA->Offset, glm::radians(bodyA.Rotation)), posB = bodyB.Position + glm::rotate(boxB->Offset, glm::radians(bodyB.Rotation));
		for (int i = 0; i < 4; i++)
		{
			pA[i] = bodyA.Position + glm::rotate(boxA->Offset + boxA->Points[i], glm::radians(bodyA.Rotation));
			pB[i] = bodyB.Position + glm::rotate(boxB->Offset + boxB->Points[i], glm::radians(bodyB.Rotation));
		}

		glm::vec2 axises[] = { glm::normalize(pA[1] - pA[0]), glm::normalize(pA[2] - pA[1]),
							   glm::normalize(pB[1] - pB[0]), glm::normalize(pB[2] - pB[1]) };

		float minDepth = FLT_MAX, depth, proj;
		glm::vec2 normal(0.f);

		float minA, maxA, minB, maxB;
		for (int i = 0; i < 4; i++)
		{
			minA = FLT_MAX, maxA = -FLT_MAX, minB = FLT_MAX, maxB = -FLT_MAX;
			// TODO : apply rotation
			for (int j = 0; j < 4; j++)
			{
				proj = glm::dot(axises[i], pA[j]);
				minA = std::min(minA, proj);
				maxA = std::max(maxA, proj);
			}

			for (int j = 0; j < 4; j++)
			{
				proj = glm::dot(axises[i], pB[j]);
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
		data->Normal = glm::dot(posB - posA, normal) > 0.f ? normal : -normal;

		return true;
	}


	template<>
	bool Collision::TestCollision<BoundingBox, BoundingCircle>(const PhysicsBody& bodyA, const PhysicsBody& bodyB, CollisionData* data)
	{
		auto box = std::static_pointer_cast<BoundingBox>(bodyA.Collider);
		auto circle = std::static_pointer_cast<BoundingCircle>(bodyB.Collider);


		glm::vec2 boxPos = bodyA.Position + glm::rotate(box->Offset, glm::radians(bodyA.Rotation));
		glm::vec2 circlePos = bodyB.Position + glm::rotate(circle->Offset, glm::radians(bodyB.Rotation));
		
		glm::vec2 points[4];
		for (int i = 0; i < 4; i++)
			points[i] = bodyA.Position + glm::rotate(box->Offset + box->Points[i], glm::radians(bodyA.Rotation));

		glm::vec2 axises[] = { glm::normalize(points[1] - points[0]), glm::normalize(points[2] - points[1]) };

		float minDepth = FLT_MAX, depth;
		glm::vec2 normal(0.f);

		for (int i = 0; i < 2; i++)
		{
			// todo : apply rotation
			float minBox = glm::dot(axises[i], points[0]);
			float maxBox = glm::dot(axises[i], points[2]);
			if (minBox > maxBox) std::swap(minBox, maxBox);

			float circleCenter = glm::dot(axises[i], circlePos);
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
			glm::vec2 axis = glm::normalize(circlePos - points[FindBoxClosestPointToCircle(points, circlePos)]);
			float minBox = FLT_MAX, maxBox = -FLT_MAX, proj;
			for (int i = 0; i < 4; i++)
			{
				proj = glm::dot(axis, points[i]);
				minBox = std::min(minBox, proj);
				maxBox = std::max(maxBox, proj);
			}

			proj = glm::dot(axis, circlePos);
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
		data->Normal = glm::dot(circlePos - boxPos, normal) > 0.f ? normal : -normal; // The relative position should change

		return true;
	}


	template<>
	bool Collision::TestCollision<BoundingCircle, BoundingBox>(const PhysicsBody& bodyA, const PhysicsBody& bodyB, CollisionData* data)
	{
		auto circle = std::static_pointer_cast<BoundingCircle>(bodyA.Collider);
		auto box = std::static_pointer_cast<BoundingBox>(bodyB.Collider);
		
		glm::vec2 boxPos = bodyB.Position + glm::rotate(box->Offset, glm::radians(bodyB.Rotation));
		glm::vec2 circlePos = bodyA.Position + glm::rotate(circle->Offset, glm::radians(bodyA.Rotation));

		glm::vec2 points[4];
		for (int i = 0; i < 4; i++)
			points[i] = bodyB.Position + glm::rotate(box->Offset + box->Points[i], glm::radians(bodyB.Rotation));


		glm::vec2 axises[] = { glm::normalize(points[1] - points[0]), glm::normalize(points[2] - points[1]) };

		float minDepth = FLT_MAX, depth;
		glm::vec2 normal(0.f);

		for (int i = 0; i < 2; i++)
		{
			float minBox = glm::dot(axises[i], points[0]);
			float maxBox = glm::dot(axises[i], points[2]);
			if (minBox > maxBox) std::swap(minBox, maxBox);

			float circleCenter = glm::dot(axises[i], circlePos);
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
			glm::vec2 axis = glm::normalize(circlePos - points[FindBoxClosestPointToCircle(points, circlePos)]);
			float minBox = FLT_MAX, maxBox = -FLT_MAX, proj;
			for (int i = 0; i < 4; i++)
			{
				proj = glm::dot(axis, points[i]);
				minBox = std::min(minBox, proj);
				maxBox = std::max(maxBox, proj);
			}

			proj = glm::dot(axis, circlePos);
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
		data->Normal = glm::dot(boxPos - circlePos, normal) > 0.f ? normal : -normal;

		return true;
	}

	// TODO : implement rotation and offset on polygon collisions
	template<>
	bool Collision::TestCollision<BoundingPolygon, BoundingPolygon>(const PhysicsBody& bodyA, const PhysicsBody& bodyB, CollisionData* data)
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


}
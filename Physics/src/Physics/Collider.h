#pragma once

#include "glm/glm.hpp"

#include <vector>

namespace Olala {

	enum class ColliderType {
		None = 0, BoundingCircle, BoundingBox, BoundingPolygon
	};

	struct Collider
	{
		glm::vec2 Offset = glm::vec2(0.f);

		virtual ColliderType GetColliderType() = 0;
	};

	struct BoundingCircle : public Collider
	{
		float Radius = 1.f;

		ColliderType GetColliderType() override { return ColliderType::BoundingCircle; }
	};

	struct BoundingBox : public Collider
	{
		glm::vec2 Points[4] = {}; // Three points can represent a rectangle

		ColliderType GetColliderType() override { return ColliderType::BoundingBox; }

		void SetSize(glm::vec2 size)
		{
			Points[0] = { -size.x * 0.5f,  size.y * 0.5f };
			Points[1] = {  size.x * 0.5f,  size.y * 0.5f };
			Points[2] = {  size.x * 0.5f, -size.y * 0.5f };
			Points[3] = { -size.x * 0.5f, -size.y * 0.5f };
		}
	};

	struct BoundingPolygon : public Collider
	{
		std::vector<glm::vec2> Points;

		ColliderType GetColliderType() override { return ColliderType::BoundingPolygon; }
	};

}
#pragma once

#include "glm/glm.hpp"

namespace Olala {

	enum class ColliderType {
		None = 0, BoundingBox, BoundingCircle
	};

	struct Collider
	{
		glm::vec2 Offset = glm::vec2(0.f);
		ColliderType Type;
	};

	struct BoundingBox : public Collider
	{
		glm::vec2 Size = glm::vec2(1.f, 1.f);
		float Rotation = 0.f;
		BoundingBox() { Type = ColliderType::BoundingBox; }
	};

	struct BoundingCircle : public Collider
	{
		float Radius = 1.f;
		BoundingCircle() { Type = ColliderType::BoundingCircle; }
	};
}
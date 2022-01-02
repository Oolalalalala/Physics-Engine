#pragma once

#include "glm/glm.hpp"

namespace Olala {

	struct BoundingBox
	{
		glm::vec2 Offset = glm::vec2(0.f);
		float Rotation = 0.f;
		glm::vec2 Size = glm::vec2(1.f, 1.f);
	};

	struct BoundingCircle
	{
		glm::vec2 Offset = glm::vec2(0.f);
		float Radius = 1.f;
	};

	struct Collider // TODO : make it into some inheritance stuff
	{
		glm::vec2 Offset = glm::vec2(0.f);
		float Rotation = 0.f;
		glm::vec2 Size = glm::vec2(1.f, 1.f);

		enum class ColliderType {
			BoundingBox = 0, BoundingCircle
		};
		ColliderType Type = ColliderType::BoundingBox;

		Collider() = default;

		operator BoundingBox() { return BoundingBox{ Offset, Rotation, Size }; }
		operator BoundingCircle() { return BoundingCircle{ Offset, Size.x }; }
	};

}
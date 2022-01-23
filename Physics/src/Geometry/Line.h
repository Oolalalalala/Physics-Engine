#pragma once

#include "glm/glm.hpp"

namespace Olala {

	struct Segment
	{
		glm::vec2 pA = glm::vec2(0.f), pB = glm::vec2(1.0f);

		Segment() = default;
		Segment(const glm::vec2& a, const glm::vec2& b)
			: pA(a), pB(b) {}
	};

	struct Line
	{
		// y = ax + b
		float a = 0.f, b = 0.f;

		Line() = default;
		Line(const glm::vec2& pA, const glm::vec2& pB)
		{
			a = pB.y - pA.y / pB.x - pA.x;
			b = pA.y - pA.x * a;
		}
		Line(const Segment& segment)
		{
			a = segment.pB.y - segment.pA.y / segment.pB.x - segment.pA.x;
			b = segment.pA.y - segment.pA.x * a;
		}
	};

	struct Ray
	{
		glm::vec2 origin, direction;
	};

}
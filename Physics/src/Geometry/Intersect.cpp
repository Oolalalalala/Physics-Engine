#include "Intersect.h"

#include "glm/gtx/norm.hpp"
#include "glm/gtx/fast_square_root.hpp"

namespace Olala {

	static float CrossProduct(const glm::vec2& a, const glm::vec2& b)
	{
		return a.x * b.y - a.y * b.x;
	}

	std::pair<bool, glm::vec2> Intersect::TestIntersect(const Segment& a, const Segment& b)
	{
		glm::vec2 lengthA = a.pB - a.pA, lengthB = b.pB - b.pA;
		float dirCross = CrossProduct(lengthA, lengthB);

		// Parellel or overlapping
		if (std::abs(dirCross) <= 0.0001f) return { false, glm::vec2(0.f) };

		//t = (q − p) × s / (r × s)
		//u = (q − p) × r / (r × s)
		float t = CrossProduct(b.pA - a.pA, lengthB) / dirCross;
		float u = CrossProduct(b.pA - a.pA, lengthA) / dirCross;

		if (0.f <= t && t <= 1.f && 0.f <= u && u <= 1.f)
			return { true, a.pA + t * lengthA };

		return { false, glm::vec2(0.f) };
	}

	std::pair<bool, glm::vec2> Intersect::TestIntersect(const Line& a, const Segment& b)
	{
		return { false, glm::vec2(0.f) };
	}

	std::pair<bool, glm::vec2> Intersect::TestIntersect(const Line& a, const Line& b)
	{
		return { false, glm::vec2(0.f) };
	}

	std::pair<bool, glm::vec2> Intersect::TestIntersect(const Circle& circle, const Line& line)
	{
		// Convert into ax + by + c = 0
		// ,which is line.a * x - 1 * y + line.b = 0
		float dist2 = (line.a * circle.pos.x - 1.f * circle.pos.y + line.b) * (line.a * circle.pos.x - 1.f * circle.pos.y + line.b)	/ (line.a + 1.f);
		if (dist2 <= circle.radius * circle.radius)
			return { true, glm::vec2(0.0f)/*Calculate the contact position*/};

		return { false, glm::vec2(1.0f) };
	}

	std::pair<bool, glm::vec2> Intersect::TestIntersect(const Circle& a, const Segment& b)
	{
		return { false, glm::vec2(0.f) };
	}

	std::pair<bool, glm::vec2> Intersect::TestIntersect(const Circle& a, const Circle& b)
	{
		if (glm::length2(b.pos - a.pos) <= (a.radius + b.radius) * (a.radius + b.radius))
			return { true, (a.pos + b.pos) * 0.5f };

		return { false, glm::vec2(0.f) };
	}

}
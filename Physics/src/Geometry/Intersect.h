#pragma once

#include "Line.h"
#include "Circle.h"

#include <utility>

namespace Olala {

	class Intersect
	{
	public:
		static std::pair<bool, glm::vec2> TestIntersect(const Segment& a, const Segment& b);
		static std::pair<bool, glm::vec2> TestIntersect(const Line& a, const Segment& b);
		static std::pair<bool, glm::vec2> TestIntersect(const Line& a, const Line& b);
		static std::pair<bool, glm::vec2> TestIntersect(const Circle& a, const Line& b);
		static std::pair<bool, glm::vec2> TestIntersect(const Circle& a, const Segment& b);
		static std::pair<bool, glm::vec2> TestIntersect(const Circle& a, const Circle& b);
	};

}
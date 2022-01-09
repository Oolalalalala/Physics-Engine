#pragma once

#include <chrono>

namespace Olala {

	class Timer
	{
	public:
		Timer();

		float GetDeltaTime(); //As seconds

	private:
		std::chrono::time_point<std::chrono::steady_clock> m_LastFrameTime;
	};

}
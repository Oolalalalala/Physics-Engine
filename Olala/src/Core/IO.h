#pragma once

namespace Olala {

	class IO
	{
	public:
		static bool IsKeyPressed(const int keycode);
		static bool IsMouseButtonPressed(const int button);
		static std::pair<float, float> GetMousePosition();
		static std::pair<int, int> GetWindowSize();
		static void SetMousePos(float xpos, float ypos);
	};

}
#pragma once

#include "DeeDeeEngine/Core/Core.h"

namespace DeeDeeEngine {
	class DEE_API Input {
	public:
		static bool IsKeyPressed(int keycode);

		static bool IsKeyRelease(int keycode);

		static bool IsKeyRepeat(int keycode);

		static bool IsMouseButtonPressed(int button);
		static float GetMouseX();
		static float GetMouseY();
	    static std::pair<float, float> GetMousePosition();
	};
}
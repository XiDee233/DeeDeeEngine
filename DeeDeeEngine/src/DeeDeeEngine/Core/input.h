#pragma once

#include "DeeDeeEngine/Core/Core.h"

namespace DeeDeeEngine {
	class DEE_API Input {
	public:
		static bool IsKeyPressed(int keycode);

		inline static bool IsMouseButtonPressed(int button);
		inline static float GetMouseX();
		inline static float GetMouseY();
	    inline static std::pair<float, float> GetMousePosition();
	};
}
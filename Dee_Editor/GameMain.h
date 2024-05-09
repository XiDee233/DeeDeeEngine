#pragma once
#include "src\EditorLayer.h"
#include <DeeDeeEngine\Events\Event.h>

namespace DeeDeeEngine {
	class GameMain {
	public:
		static void Init(EditorLayer* layer);

		static void OnEvent(Event& e);
	};
}

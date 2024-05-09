#pragma once
#include "src\EditorLayer.h"
#include <DeeDeeEngine\Events\Event.h>
#include "DeeDeeEngine\Scene\Components.h"

namespace DeeDeeEngine {

	class GameMain {
	public:
		static GameMain& GetInstance() {
			static GameMain instance;
			return instance;
		}

		void Init(EditorLayer* layer);

		// Other methods and members...
	public:
		Rigidbody2DComponent* m_rigid;
		Entity m_player;

	private:
		GameMain() {} // Private constructor for the singleton pattern.

		// Delete copy constructor and assignment operator.
		GameMain(const GameMain&) = delete;
		GameMain& operator=(const GameMain&) = delete;

		// Members
		
		EditorLayer* m_layer;
		Entity m_Groud;
	};

}

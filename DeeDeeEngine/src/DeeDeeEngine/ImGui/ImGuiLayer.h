#pragma once
#include "DeeDeeEngine/Layer.h"
#include "DeeDeeEngine/Events/KeyEvent.h"
#include "DeeDeeEngine/Events/MouseEvent.h"
#include "DeeDeeEngine/Events/ApplicationEvent.h"
namespace DeeDeeEngine {
	class DEE_API ImGuiLayer :public Layer {
	public:
		ImGuiLayer();
		~ImGuiLayer();

		virtual void OnAttach();
		virtual void OnDetach();
		void OnUpdate();
		void OnEvent(Event& event);
	private:
		bool OnMouseButtonPressedEvent(MouseButtonPressedEvent& e);
		bool OnMouseButtonReleasedEvent(MouseButtonReleasedEvent& e);
		bool OnMouseScrolledEvent(MouseScrolledEvent& e);
		bool OnMouseMoveEvent(MouseMoveEvent& e);
		bool OnWindowResizeEvent(WindowResizeEvent& e);
		bool OnKeyPressedEvent(KeyPressedEvent& e);
		bool OnKeyReleaseEvent(KeyReleasedEvent& e);
		bool OnKeyTypedEvent(KeyTypedEvent& e);
	private:
		float m_Time = 0.0f;
	};
}



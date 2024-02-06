#pragma once
#include "Core.h"
#include "Window.h"
#include "DeeDeeEngine/Events/ApplicationEvent.h"
#include "Layer.h"
#include "LayerStack.h"


namespace DeeDeeEngine {
	class DEE_API Application
	{
	public:
		Application();
		virtual ~Application();

		void Run();

		void OnEvent(Event& e);

		void PushLayer(Layer* layer);
		void PushOverlay(Layer* layer);
	private:
		bool OnWindowClose(WindowCloseEvent& e);
		std::unique_ptr<Window> m_Window;
		bool m_Running = true;
		LayerStack m_LayerStack;
	};
	Application* CreateApplication();
}

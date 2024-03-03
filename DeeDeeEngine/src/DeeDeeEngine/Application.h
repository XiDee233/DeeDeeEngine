#pragma once
#include "Core.h"
#include "Window.h"
#include "DeeDeeEngine/Events/ApplicationEvent.h"
#include "Layer.h"
#include "LayerStack.h"
#include "DeeDeeEngine/ImGui/ImGuiLayer.h"

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
		inline static Application& Get() {
			return *s_Instance;
		}
		inline Window& GetWindow() { return *m_Window; }
	private:
		bool OnWindowClose(WindowCloseEvent& e);
		std::unique_ptr<Window> m_Window;
		ImGuiLayer* m_ImGuiLayer;
		bool m_Running = true;
		LayerStack m_LayerStack;
	private:
		static Application* s_Instance;
	};
	Application* CreateApplication();
}

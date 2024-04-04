#pragma once
#include "Core.h"
#include "Window.h"
#include "DeeDeeEngine/Events/ApplicationEvent.h"
#include "Layer.h"
#include "LayerStack.h"
#include "DeeDeeEngine/ImGui/ImGuiLayer.h"

#include "DeeDeeEngine\Core\Timestep.h"


#include "DeeDeeEngine\Renderer\OrthographicCamera.h"


namespace DeeDeeEngine {
	class Application
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
		float m_LastFrameTime = 0.0f;
	
	private:
		static Application* s_Instance;
	};
	Application* CreateApplication();
}

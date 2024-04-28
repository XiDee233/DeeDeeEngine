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
		Application(const std::string& name="DeeApp");
		virtual ~Application();

		void Run();

		void OnEvent(Event& e);

		void PushLayer(Layer* layer);
		void PushOverlay(Layer* layer);

		ImGuiLayer* GetImGuiLayer() { return m_ImGuiLayer; }
		inline static Application& Get() {
			return *s_Instance;
		}
		void Close();
		inline Window& GetWindow() { return *m_Window; }
	private:
		bool OnWindowClose(WindowCloseEvent& e);
		bool OnWindowResize(WindowResizeEvent& e);

		std::unique_ptr<Window> m_Window;
		ImGuiLayer* m_ImGuiLayer;
		bool m_Running = true;
		bool m_Minimized = false;
		LayerStack m_LayerStack;
		float m_LastFrameTime = 0.0f;
	
	private:
		static Application* s_Instance;
	};
	Application* CreateApplication();
}

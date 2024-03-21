#pragma once
#include "Core.h"
#include "Window.h"
#include "DeeDeeEngine/Events/ApplicationEvent.h"
#include "Layer.h"
#include "LayerStack.h"
#include "DeeDeeEngine/ImGui/ImGuiLayer.h"
#include "DeeDeeEngine/Renderer/Shader.h"
#include <Platform/OpenGL/OpenGLBuffer.h>

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

		unsigned int m_VertexArray/*, *//*m_VertexBuffer,*/ /*m_IndexBuffer*/;
		std::unique_ptr<Shader> m_Shader;
		std::unique_ptr<VertexBuffer> m_VertexBuffer;
		std::unique_ptr<IndexBuffer> m_IndexBuffer;
	private:
		static Application* s_Instance;
	};
	Application* CreateApplication();
}

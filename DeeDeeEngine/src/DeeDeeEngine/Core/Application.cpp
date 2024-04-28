#include "deepch.h"
#include "Application.h"
#include "DeeDeeEngine/Core/Log.h"
#include "input.h"
#include "glm/glm.hpp"
#include "DeeDeeEngine\Renderer\Renderer.h"

#include <GLFW/glfw3.h>
namespace DeeDeeEngine {

#define BIND_EVENT_FN(x) std::bind(&Application::x, this, std::placeholders::_1)

	Application* Application::s_Instance = nullptr;
	
	Application::Application(const std::string& name)
	{
		DEE_PROFILE_FUNCTION();

		DEE_CORE_ASSERT(!s_Instance, "Application already exists!")
			s_Instance = this;
		m_Window = std::unique_ptr<Window>(Window::Create(WindowProps(name)));
		m_Window->SetEventCallback(BIND_EVENT_FN(OnEvent));

		Renderer::Init();
		m_ImGuiLayer = new ImGuiLayer();
		PushOverlay(m_ImGuiLayer);

		//vertex array
		//Vertex Buffer
		//Index Buffer
		/*首先，创建了1个顶点数组对象，
		并将其标识符存储在变量m_VertexArray中。可以将顶点数组对象看作是一个容器，用于存储顶点数据和配置顶点属性*/

	}






	Application::~Application()
	{
		DEE_PROFILE_FUNCTION();

	}

	void Application::PushLayer(Layer* layer) {
		DEE_PROFILE_FUNCTION();

		m_LayerStack.PushLayer(layer);
		layer->OnAttach();
	}
	void Application::PushOverlay(Layer* layer) {
		DEE_PROFILE_FUNCTION();

		m_LayerStack.PushOverlay(layer);
		layer->OnAttach();
	}

	void Application::OnEvent(Event& e) {
		DEE_PROFILE_FUNCTION();

		EventDispatcher dispatcher(e);
		dispatcher.Dispatch<WindowCloseEvent>(BIND_EVENT_FN(OnWindowClose));
		dispatcher.Dispatch<WindowResizeEvent>(BIND_EVENT_FN(OnWindowResize));
		for (auto it = m_LayerStack.end(); it != m_LayerStack.begin();) {
			(*--it)->OnEvent(e);
			if (e.Handled)
				break;
		}

	}

	void Application::Run() {
		DEE_PROFILE_FUNCTION();

		while (m_Running)
		{

			float time = glfwGetTime();
			Timestep timestep = time - m_LastFrameTime;
			m_LastFrameTime = time;
			
			if (!m_Minimized) {
				{
					DEE_PROFILE_SCOPE("layer->OnUpdate");
					for (Layer* layer : m_LayerStack)
						layer->OnUpdate(timestep);
				}
				m_ImGuiLayer->Begin();
				{
					DEE_PROFILE_SCOPE("layer->OnImGuiRender");

					for (Layer* layer : m_LayerStack)
						layer->OnImGuiRender();
				}
				m_ImGuiLayer->End();
			}
			
			m_Window->OnUpdate();
		}
	}

    void Application::Close()
    {
		m_Running = false;
    }

    bool Application::OnWindowClose(WindowCloseEvent& e) {
		DEE_PROFILE_FUNCTION();

		m_Running = false;
		return true;
	}
	bool Application::OnWindowResize(WindowResizeEvent& e)
	{
		DEE_PROFILE_FUNCTION();

		if (e.GetWidth() == 0 || e.GetHeight() == 0) {
			m_Minimized = true;
			return false;
		}
		m_Minimized = false;
		Renderer::OnWindowResize(e.GetWidth(), e.GetHeight());
		return false;
	}
}


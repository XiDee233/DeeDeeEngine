#include "deepch.h"
#include "Application.h"
#include "DeeDeeEngine/Events/ApplicationEvent.h"
#include "DeeDeeEngine/Log.h"

#include <GLFW/glfw3.h>
namespace DeeDeeEngine {
	Application::Application()
	{
		m_Window = std::unique_ptr<Window>(Window::Create());
	}

	Application::~Application()
	{
	}

	void Application::Run() {
	
		while (m_Running)
		{
			glClearColor(1, 0, 1, 1);//设置清空颜色为紫色
			glClear(GL_COLOR_BUFFER_BIT);//使用当前清空颜色清空颜色缓冲区，即将窗口内容清空为之前设置的清空颜色。
			m_Window->OnUpdate();
		}
	}
}


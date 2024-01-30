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
			glClearColor(1, 0, 1, 1);//���������ɫΪ��ɫ
			glClear(GL_COLOR_BUFFER_BIT);//ʹ�õ�ǰ�����ɫ�����ɫ�����������������������Ϊ֮ǰ���õ������ɫ��
			m_Window->OnUpdate();
		}
	}
}


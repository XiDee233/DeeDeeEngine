#include "deepch.h"
#include "WindowsWindow.h"
#include <DeeDeeEngine/Log.h>

namespace DeeDeeEngine {
	static bool s_GLFWInitialized = false;
	Window* Window::Create(const WindowProps& props) {
		return new WindowsWindow(props);
	}
	WindowsWindow::WindowsWindow(const WindowProps& props)
	{
		Init(props);
	}
	WindowsWindow::~WindowsWindow() {

	}

	void WindowsWindow::Init(const WindowProps& props)
	{
		m_Data.Title = props.Title;
		m_Data.Width = props.Width;
		m_Data.Height = props.Height;
		DEE_CORE_INFO("Createing window {0} ({1},{2})", props.Title, props.Width, props.Height);

		if (!s_GLFWInitialized) {
			int success = glfwInit();
			DEE_CORE_ASSERT(success, "Could not initialize GLFW!");
			
			s_GLFWInitialized = true;
		}

		m_Window = glfwCreateWindow((int)props.Width, (int)props.Height, m_Data.Title.c_str(), nullptr, nullptr);
		glfwMakeContextCurrent(m_Window);// �����ڵ�OpenGL����������Ϊ��ǰ������
		glfwSetWindowUserPointer(m_Window,&m_Data);// ���û���������ݣ�m_Data���봰�ڹ�����
		SetVSync(true);
	}

	void WindowsWindow::Shutdown() {
		glfwDestroyWindow(m_Window);
	}
	void WindowsWindow::OnUpdate() {
		glfwPollEvents();
		//�������ڽ���ǰ�󻺳�������˫��������£����Ƶ�ͼ�����ȱ���Ⱦ���󻺳�����
		// Ȼ��ͨ�����øú������󻺳��������ݽ�����ǰ��������ʹ����ʾ�ڴ����ϡ��������Ա�������Ⱦ�����г��ֿɼ�����˸��˺������
		glfwSwapBuffers(m_Window);
	}

	void WindowsWindow::SetVSync(bool enabled) {
		if (enabled)
			glfwSwapInterval(1);
		else
			glfwSwapInterval(0);
	}

	bool WindowsWindow::IsVSync() const
	{
		return m_Data.VSync;
	}
}

#include "deepch.h"
#include "WindowsWindow.h"
#include <DeeDeeEngine/Core/Log.h>
#include <DeeDeeEngine/Events/ApplicationEvent.h>
#include <DeeDeeEngine/Events/KeyEvent.h>
#include <DeeDeeEngine/Events/MouseEvent.h>
#include "Platform/OpenGL/OpenGLContext.h"
#include <glad/glad.h>

namespace DeeDeeEngine {
	static bool s_GLFWInitialized = false;
	static void GLFWErrorCallback(int error_code, const char* description) {
		DEE_CORE_ERROR("GLFW Error ({0}):{1}", error_code, description);
	}
	Window* Window::Create(const WindowProps& props) {
		return new WindowsWindow(props);
	}
	WindowsWindow::WindowsWindow(const WindowProps& props)
	{
		DEE_PROFILE_FUNCTION();
		Init(props);
	}
	WindowsWindow::~WindowsWindow() {

	}

	void WindowsWindow::Init(const WindowProps& props)
	{

		DEE_PROFILE_FUNCTION();
		m_Data.Title = props.Title;
		m_Data.Width = props.Width;
		m_Data.Height = props.Height;

		DEE_CORE_INFO("Createing window {0} ({1},{2})", props.Title, props.Width, props.Height);

		if (!s_GLFWInitialized) {
			int success = glfwInit();
			DEE_CORE_ASSERT(success, "Could not initialize GLFW!");
			glfwSetErrorCallback(GLFWErrorCallback);
			s_GLFWInitialized = true;
		}

		m_Window = glfwCreateWindow((int)props.Width, (int)props.Height, m_Data.Title.c_str(), nullptr, nullptr);
		m_Context = new OpenGLContext(m_Window);

		m_Context->Init();
	
		glfwSetWindowUserPointer(m_Window, &m_Data);// ���û���������ݣ�m_Data���봰�ڹ�����
		SetVSync(true);

		//Set glfw callbacks
		glfwSetWindowSizeCallback(m_Window, [](GLFWwindow* window, int width, int height) {
			WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
			data.Width = width;
			data.Height = height;
			WindowResizeEvent event(width, height);
			data.EventCallback(event);// �������jj������jy������sy����
			});
		glfwSetWindowCloseCallback(m_Window, [](GLFWwindow* window) {
			WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
			WindowCloseEvent event;
			data.EventCallback(event);
			});
		glfwSetKeyCallback(m_Window, [](GLFWwindow* window, int key, int scancode, int action, int mods) {
			WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

			switch (action)
			{
			case GLFW_PRESS:
			{
				KeyPressedEvent event(key, 0);
				data.EventCallback(event);
				break;
			}
			case GLFW_RELEASE:
			{
				KeyReleasedEvent event(key);
				data.EventCallback(event);
				break;
			}
			case GLFW_REPEAT:
			{
				KeyPressedEvent event(key, 1);
				data.EventCallback(event);
				break;
			}
			default:
				break;
			}
			});

		glfwSetCharCallback(m_Window, [](GLFWwindow* window, unsigned int codepoint) {
			WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
			KeyTypedEvent event(codepoint);
			data.EventCallback(event);
			});
		glfwSetMouseButtonCallback(m_Window, [](GLFWwindow* window, int button, int action, int mods) {
			WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

			switch (action) {
			case GLFW_PRESS:
			{
				MouseButtonPressedEvent event(button);
				data.EventCallback(event);
				break;
			}
			case GLFW_RELEASE:
			{
				MouseButtonReleasedEvent event(button);
				data.EventCallback(event);
				break;
			}
			}
			});

		glfwSetScrollCallback(m_Window, [](GLFWwindow* window, double xoffset, double yoffset) {
			WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
			MouseScrolledEvent event((float)xoffset, (float)yoffset);
			data.EventCallback(event);
			});

		glfwSetCursorPosCallback(m_Window, [](GLFWwindow* window, double xpos, double ypos) {
			WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

			MouseMoveEvent event((float)xpos, (float)ypos);
			data.EventCallback(event);
			});
	}

	void WindowsWindow::Shutdown() {
		DEE_PROFILE_FUNCTION();
		glfwDestroyWindow(m_Window);
	}
	void WindowsWindow::OnUpdate() {

		DEE_PROFILE_FUNCTION();
		glfwPollEvents();
		m_Context->SwapBuffers();
		//�������ڽ���ǰ�󻺳�������˫��������£����Ƶ�ͼ�����ȱ���Ⱦ���󻺳�����
		// Ȼ��ͨ�����øú������󻺳��������ݽ�����ǰ��������ʹ����ʾ�ڴ����ϡ��������Ա�������Ⱦ�����г��ֿɼ�����˸��˺������
	}

	void WindowsWindow::SetVSync(bool enabled) {

		DEE_PROFILE_FUNCTION();
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

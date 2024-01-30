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
		glfwMakeContextCurrent(m_Window);// 将窗口的OpenGL上下文设置为当前上下文
		glfwSetWindowUserPointer(m_Window,&m_Data);// 将用户定义的数据（m_Data）与窗口关联。
		SetVSync(true);
	}

	void WindowsWindow::Shutdown() {
		glfwDestroyWindow(m_Window);
	}
	void WindowsWindow::OnUpdate() {
		glfwPollEvents();
		//函数用于交换前后缓冲区。在双缓冲机制下，绘制的图像首先被渲染到后缓冲区，
		// 然后通过调用该函数将后缓冲区的内容交换到前缓冲区，使其显示在窗口上。这样可以避免在渲染过程中出现可见的闪烁或撕裂现象。
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

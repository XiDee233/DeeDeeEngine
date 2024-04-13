#include "deepch.h"
#include "OpenGLContext.h"
#include <GLFW/glfw3.h>
#include <glad/glad.h>
#include <DeeDeeEngine/Core/Log.h>


namespace DeeDeeEngine {
	OpenGLContext::OpenGLContext(GLFWwindow* windowHandle):m_WindowHandle(windowHandle) {
		DEE_CORE_ASSERT(windowHandle,"Window handle is null!")
	}
	void OpenGLContext::Init() {
		glfwMakeContextCurrent(m_WindowHandle);// 将窗口的OpenGL上下文设置为当前上下文,win = current
		int status = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);//加载opengl函数指针
		DEE_CORE_ASSERT(status, "Faid to initialize Glad!");
		DEE_CORE_INFO("OpenGL Renderer:{0} {1}", reinterpret_cast<const char*>(glGetString(GL_VENDOR)), reinterpret_cast<const char*>(glGetString(GL_RENDERER)));
	}
	void OpenGLContext::SwapBuffers() {

		glfwSwapBuffers(m_WindowHandle);

	}
}

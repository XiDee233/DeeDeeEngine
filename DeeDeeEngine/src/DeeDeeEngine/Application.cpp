#include "deepch.h"
#include "Application.h"
#include "DeeDeeEngine/Log.h"
#include "glad/glad.h"
#include "input.h"
#include "glm/glm.hpp"
namespace DeeDeeEngine {

#define BIND_EVENT_FN(x) std::bind(&Application::x, this, std::placeholders::_1)

	Application* Application::s_Instance = nullptr;
	Application::Application()
	{
		DEE_CORE_ASSERT(!s_Instance, "Application already exists!")
			s_Instance = this;
		m_Window = std::unique_ptr<Window>(Window::Create());
		m_Window->SetEventCallback(BIND_EVENT_FN(OnEvent));

		m_ImGuiLayer = new ImGuiLayer();
		PushOverlay(m_ImGuiLayer);

		//vertex array
		//Vertex Buffer
		//Index Buffer
		/*首先，创建了1个顶点数组对象，
		并将其标识符存储在变量m_VertexArray中。可以将顶点数组对象看作是一个容器，用于存储顶点数据和配置顶点属性*/


		glGenVertexArrays(1, &m_VertexArray);
		//接下来，将创建的顶点数组对象绑定到OpenGL的当前上下文中，
		//使其成为当前操作的顶点数组对象。这样，后续对顶点缓冲区和顶点属性的设置和使用都会与这个绑定的顶点数组对象相关联。*/
		glBindVertexArray(m_VertexArray);

		////生成一个缓冲区对象，并将其标识符存储在变量m_VertexBuffer中。缓冲区对象用于存储顶点数据或其他OpenGL中需要使用的数据。
		//glGenBuffers(1, &m_VertexBuffer);
		////将缓冲区对象绑定到OpenGL的当前上下文中，使其成为当前操作的缓冲区对象。在这里，GL_ARRAY_BUFFER表示我们将要操作的是顶点属性数组的缓冲区对象。
		//glBindBuffer(GL_ARRAY_BUFFER, m_VertexBuffer);

		float vertices[3 * 6] = {
			-0.5f, -0.5f, 0.0f,  // 位置
			0.5f, -0.5f, 0.0f,   // 位置
			0.0f, 0.5f, 0.0f,    // 位置
		};
		m_VertexBuffer.reset(VertexBuffer::Create(vertices, sizeof(vertices)));
		//将顶点数据存储到VBO中。glBufferData函数将vertices数组的数据复制到VBO中。
	// sizeof(vertices)用于获取数组的字节大小。GL_STATIC_DRAW表示这些数据将被静态地传递给OpenGL进行绘制。
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

		//glEnableVertexAttribArray启用顶点属性数组。(在OpenGL中，顶点属性是用来描述顶点数据的信息，
		// 比如位置、颜色、法线等。每个顶点属性都有一个索引，用来标识不同的属性。

		//glEnableVertexAttribArray(0)的作用是启用属性索引为0的顶点属性数组。
		// 这意味着我们告诉OpenGL我们将要使用索引为0的顶点属性来渲染顶点。
		// 在这种情况下，索引为0的属性通常是顶点的位置属性，用来描述顶点在三维空间中的位置。)
		glEnableVertexAttribArray(0);
		//第一个参数：指定属性索引，这里是0，表示位置属性。
		//	第二个参数：指定每个属性的组件数，这里是3，表示每个位置属性由3个浮点数组成（x、y、z坐标）。
		//	第三个参数：指定每个组件的数据类型，这里是GL_FLOAT，表示使用浮点数作为数据类型。
		//	第四个参数：指定是否对数据进行归一化处理，这里是GL_FALSE，表示不进行归一化处理。
		//	第五个参数：指定每个顶点属性的字节大小。在这里，3 * sizeof(float)表示每个顶点属性占用的字节数，因为每个属性有3个浮点数，所以乘以sizeof(float)。
		//	第六个参数：指定顶点属性数据的起始位置或者缓冲区偏移量。在这里，nullptr表示数据是从当前绑定的顶点缓冲区的开头开始的。
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), nullptr);

		//glGenBuffers(1, &m_IndexBuffer);
		//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_IndexBuffer);
		//OpenGL根据IBO(索引缓冲区对象)中的索引值来查找对应的顶点数据。例如，索引值为0表示顶点缓冲区中的第一个顶点，索引值为1表示第二个顶点，
		// 以此类推。OpenGL根据索引值找到对应的顶点数据，并按照顺序连接起来形成图元（如三角形）
		unsigned int indices[3] = { 0,1,2 };
		//glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
		m_IndexBuffer.reset(IndexBuffer::Create(indices, sizeof(indices)/sizeof(uint32_t)));

		std::string vertexSrc = R"(
         #version 330 core
         layout(location = 0)in vec3 a_Position;
out vec3 v_Position;
         
         void main(){
v_Position = a_Position;
             gl_Position = vec4(a_Position,1.0);
}
         )";
		std::string fragmentSrc = R"(
  #version 330 core
         layout(location = 0)out vec4 color;

in vec3 v_Position;
         
         void main(){
             color = vec4(v_Position+0.5,1.0);
}
         )";

		m_Shader.reset(new Shader(vertexSrc,fragmentSrc));
	}

	Application::~Application()
	{
	}

	void Application::PushLayer(Layer* layer) {
		m_LayerStack.PushLayer(layer);
		layer->OnAttach();
	}
	void Application::PushOverlay(Layer* layer) {
		m_LayerStack.PushOverlay(layer);
		layer->OnAttach();
	}

	void Application::OnEvent(Event& e) {
		EventDispatcher dispatcher(e);
		dispatcher.Dispatch<WindowCloseEvent>(BIND_EVENT_FN(OnWindowClose));
		for (auto it = m_LayerStack.end(); it != m_LayerStack.begin();) {
			(*--it)->OnEvent(e);
			if (e.Handled)
				break;
		}

	}

	void Application::Run() {

		while (m_Running)
		{
			glClearColor(0.2f, 0.2f, 0.2f, 1);
			glClear(GL_COLOR_BUFFER_BIT);//使用当前清空颜色清空颜色缓冲区，即将窗口内容清空为之前设置的清空颜色。
			m_Shader->Bind();
			glBindVertexArray(m_VertexArray);
			glDrawElements(GL_TRIANGLES, m_IndexBuffer->GetCount(), GL_UNSIGNED_INT, nullptr);

			for (Layer* layer : m_LayerStack)
				layer->OnUpdate();

			m_ImGuiLayer->Begin();
			for (Layer* layer : m_LayerStack)
				layer->OnImGuiRender();
			m_ImGuiLayer->End();
			m_Window->OnUpdate();
		}
	}

	bool Application::OnWindowClose(WindowCloseEvent& e) {
		m_Running = false;
		return true;
	}
}


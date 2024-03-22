#include "deepch.h"
#include "Application.h"
#include "DeeDeeEngine/Log.h"
#include "glad/glad.h"
#include "input.h"
#include "glm/glm.hpp"
namespace DeeDeeEngine {

#define BIND_EVENT_FN(x) std::bind(&Application::x, this, std::placeholders::_1)

	Application* Application::s_Instance = nullptr;
	static GLenum ShaderDataTypeToOpenGLBaseType(ShaderDataType type) {
		switch (type) {
		case DeeDeeEngine::ShaderDataType::Float: return GL_FLOAT;
		case DeeDeeEngine::ShaderDataType::Float2: return GL_FLOAT;
		case DeeDeeEngine::ShaderDataType::Float3: return GL_FLOAT;
		case DeeDeeEngine::ShaderDataType::Float4: return GL_FLOAT;
		case DeeDeeEngine::ShaderDataType::Mat3: return GL_FLOAT;
		case DeeDeeEngine::ShaderDataType::Mat4: return GL_FLOAT;
		case DeeDeeEngine::ShaderDataType::Int: return GL_INT;
		case DeeDeeEngine::ShaderDataType::Int2: return GL_INT;
		case DeeDeeEngine::ShaderDataType::Int3: return GL_INT;
		case DeeDeeEngine::ShaderDataType::Int4: return GL_INT;
		case DeeDeeEngine::ShaderDataType::Bool: return GL_BOOL;
		}
		DEE_CORE_ASSERT(false, "Unknown ShaderDataType!");
		return 0;
	}
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

		float vertices[3 * 7] = {
			-0.5f, -0.5f, 0.0f,0.0f,1.0f,0.0f,1.0f,  // 位置
			0.5f, -0.5f, 0.0f, 0.0f,0.0f,1.0f,1.0f,   // 位置
			0.0f, 0.5f, 0.0f,  1.0f,0.0f,0.0f,1.0f,   // 位置
		};
		m_VertexBuffer.reset(VertexBuffer::Create(vertices, sizeof(vertices)));

		BufferLayout layout = {
			{ShaderDataType::Float3,"a_Position"},
			{ShaderDataType::Float4,"a_Color"}

		};
		m_VertexBuffer->SetLayout(layout);
		uint32_t index = 0;
		for (const auto& element : m_VertexBuffer->GetLayout()) {
			glEnableVertexAttribArray(index);
			glVertexAttribPointer(index, element.GetComponentCount(), 
				ShaderDataTypeToOpenGLBaseType(element.Type),
				element.Normalized?GL_TRUE:GL_FALSE, 
				layout.GetStride(), 
				(const void*)element.Offset);
			index++;
		}

		uint32_t indices[3] = { 0,1,2 };
		//glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
		m_IndexBuffer.reset(IndexBuffer::Create(indices, sizeof(indices) / sizeof(uint32_t)));

		std::string vertexSrc = R"(
         #version 330 core
         layout(location = 0)in vec3 a_Position;
layout(location =1) in vec4 a_Color;
out vec3 v_Position;
out vec4 v_Color;
         
         void main(){
v_Position = a_Position;
v_Color =a_Color;
             gl_Position = vec4(a_Position,1.0);
}
         )";
		std::string fragmentSrc = R"(
  #version 330 core
         layout(location = 0)out vec4 color;

in vec3 v_Position;
in vec4 v_Color;
         
         void main(){
             color = vec4(v_Position+0.5,1.0);
color = v_Color;
}           
         )";

		m_Shader.reset(new Shader(vertexSrc, fragmentSrc));
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


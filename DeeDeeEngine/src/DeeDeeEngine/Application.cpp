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
		/*���ȣ�������1�������������
		�������ʶ���洢�ڱ���m_VertexArray�С����Խ����������������һ�����������ڴ洢�������ݺ����ö�������*/


		glGenVertexArrays(1, &m_VertexArray);
		//���������������Ķ����������󶨵�OpenGL�ĵ�ǰ�������У�
		//ʹ���Ϊ��ǰ�����Ķ���������������������Զ��㻺�����Ͷ������Ե����ú�ʹ�ö���������󶨵Ķ�����������������*/
		glBindVertexArray(m_VertexArray);

		////����һ�����������󣬲������ʶ���洢�ڱ���m_VertexBuffer�С��������������ڴ洢�������ݻ�����OpenGL����Ҫʹ�õ����ݡ�
		//glGenBuffers(1, &m_VertexBuffer);
		////������������󶨵�OpenGL�ĵ�ǰ�������У�ʹ���Ϊ��ǰ�����Ļ��������������GL_ARRAY_BUFFER��ʾ���ǽ�Ҫ�������Ƕ�����������Ļ���������
		//glBindBuffer(GL_ARRAY_BUFFER, m_VertexBuffer);

		float vertices[3 * 7] = {
			-0.5f, -0.5f, 0.0f,0.0f,1.0f,0.0f,1.0f,  // λ��
			0.5f, -0.5f, 0.0f, 0.0f,0.0f,1.0f,1.0f,   // λ��
			0.0f, 0.5f, 0.0f,  1.0f,0.0f,0.0f,1.0f,   // λ��
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
			glClear(GL_COLOR_BUFFER_BIT);//ʹ�õ�ǰ�����ɫ�����ɫ�����������������������Ϊ֮ǰ���õ������ɫ��
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


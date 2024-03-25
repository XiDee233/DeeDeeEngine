#include "deepch.h"
#include "Application.h"
#include "DeeDeeEngine/Log.h"
#include "input.h"
#include "glm/glm.hpp"
#include "DeeDeeEngine\Renderer\Renderer.h"
namespace DeeDeeEngine {

#define BIND_EVENT_FN(x) std::bind(&Application::x, this, std::placeholders::_1)

	Application* Application::s_Instance = nullptr;
	
	Application::Application()
		:m_Camera(-1.6f,1.6f,-0.9f,0.9f)
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

		m_VertexArray.reset(VertexArray::Create());

		////生成一个缓冲区对象，并将其标识符存储在变量m_VertexBuffer中。缓冲区对象用于存储顶点数据或其他OpenGL中需要使用的数据。
		//glGenBuffers(1, &m_VertexBuffer);
		////将缓冲区对象绑定到OpenGL的当前上下文中，使其成为当前操作的缓冲区对象。在这里，GL_ARRAY_BUFFER表示我们将要操作的是顶点属性数组的缓冲区对象。
		//glBindBuffer(GL_ARRAY_BUFFER, m_VertexBuffer);

		float vertices[3 * 7] = {
			-0.5f, -0.5f, 0.0f,0.0f,1.0f,0.0f,1.0f,  
			0.5f, -0.5f, 0.0f, 0.0f,0.0f,1.0f,1.0f,  
			0.0f, 0.5f, 0.0f,  1.0f,0.0f,0.0f,1.0f,  
		};
		std::shared_ptr<VertexBuffer> triVB;
		triVB.reset(VertexBuffer::Create(vertices, sizeof(vertices)));

		BufferLayout _layout = {
			{ShaderDataType::Float3,"a_Position"},
			{ShaderDataType::Float4,"a_Color"}

		};
		triVB->SetLayout(_layout);
		
		m_VertexArray->AddVertexBuffer(triVB);

		uint32_t indices[3] = { 0,1,2 };
		std::shared_ptr<IndexBuffer> triIB;
		triIB.reset(IndexBuffer::Create(indices, sizeof(indices) / sizeof(uint32_t)));
		m_VertexArray->SetIndexBuffer(triIB);

		m_SquareVA.reset(VertexArray::Create());


		float vertices2[3 * 4] = {
			-0.75f, -0.75f, 0.0f,
			 0.75f, -0.75f, 0.0f, 
			 0.75f,  0.75f, 0.0f,  
			-0.75f,  0.75f, 0.0f,

		};
		std::shared_ptr<VertexBuffer> squareVB;
		squareVB.reset(VertexBuffer::Create(vertices2, sizeof(vertices2)));
		BufferLayout _layout2 = {
			{ShaderDataType::Float3,"a_Position"},
		};
		squareVB->SetLayout(_layout2);

		m_SquareVA->AddVertexBuffer(squareVB);

		uint32_t indices2[6] = { 0,1,2,2,3,0 };
		std::shared_ptr<IndexBuffer> squareIB;
		squareIB.reset(	IndexBuffer::Create(indices, sizeof(indices)/sizeof(uint32_t)));
		squareIB.reset(IndexBuffer::Create(indices2, sizeof(indices2) / sizeof(uint32_t)));
		m_SquareVA->SetIndexBuffer(squareIB);
		std::string vertexSrc = R"(
         #version 330 core
         layout(location = 0)in vec3 a_Position;
layout(location =1) in vec4 a_Color;

 uniform mat4 u_ViewProjection;

out vec3 v_Position;
out vec4 v_Color;
         
         void main(){
v_Position = a_Position;
v_Color =a_Color;
             gl_Position =u_ViewProjection* vec4(a_Position,1.0);
}
         )";
		std::string fragmentSrc = R"(
  #version 330 core
         layout(location = 0)out vec4 color;

uniform vec4 u_Color;

in vec3 v_Position;
in vec4 v_Color;
         
         void main(){
             color = vec4(v_Position+0.5,1.0);
color = v_Color;
}           
         )";

		m_Shader.reset(new Shader(vertexSrc, fragmentSrc));

		std::string vertexSrc2 = R"(
         #version 330 core
         layout(location = 0)in vec3 a_Position;

 uniform mat4 u_ViewProjection;


out vec3 v_Position;
         
         void main(){
v_Position = a_Position;
             gl_Position = u_ViewProjection * vec4(a_Position,1.0);
}
         )";
		std::string fragmentSrc2 = R"(
  #version 330 core
         layout(location = 0)out vec4 color;
in vec3 v_Position;
         
         void main(){
             color = vec4(0.2,0.3,0.8,1.0);
}           
         )";

		m_Shader2.reset(new Shader(vertexSrc2, fragmentSrc2));
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

			RenderCommand::SetClearColor({ 0.2f, 0.2f, 0.2f, 1 });
			RenderCommand::Clear();
			m_Camera.SetRotation(45);

			Renderer::BeginScene(m_Camera);

		
			Renderer::Submit(m_Shader2,m_SquareVA);

			Renderer::Submit(m_Shader,m_VertexArray);

			Renderer::EndScene();


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


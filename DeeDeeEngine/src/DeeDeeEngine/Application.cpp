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

		float vertices[3 * 6] = {
			-0.5f, -0.5f, 0.0f,  // λ��
			0.5f, -0.5f, 0.0f,   // λ��
			0.0f, 0.5f, 0.0f,    // λ��
		};
		m_VertexBuffer.reset(VertexBuffer::Create(vertices, sizeof(vertices)));
		//���������ݴ洢��VBO�С�glBufferData������vertices��������ݸ��Ƶ�VBO�С�
	// sizeof(vertices)���ڻ�ȡ������ֽڴ�С��GL_STATIC_DRAW��ʾ��Щ���ݽ�����̬�ش��ݸ�OpenGL���л��ơ�
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

		//glEnableVertexAttribArray���ö����������顣(��OpenGL�У��������������������������ݵ���Ϣ��
		// ����λ�á���ɫ�����ߵȡ�ÿ���������Զ���һ��������������ʶ��ͬ�����ԡ�

		//glEnableVertexAttribArray(0)��������������������Ϊ0�Ķ����������顣
		// ����ζ�����Ǹ���OpenGL���ǽ�Ҫʹ������Ϊ0�Ķ�����������Ⱦ���㡣
		// ����������£�����Ϊ0������ͨ���Ƕ����λ�����ԣ�����������������ά�ռ��е�λ�á�)
		glEnableVertexAttribArray(0);
		//��һ��������ָ������������������0����ʾλ�����ԡ�
		//	�ڶ���������ָ��ÿ�����Ե��������������3����ʾÿ��λ��������3����������ɣ�x��y��z���꣩��
		//	������������ָ��ÿ��������������ͣ�������GL_FLOAT����ʾʹ�ø�������Ϊ�������͡�
		//	���ĸ�������ָ���Ƿ�����ݽ��й�һ������������GL_FALSE����ʾ�����й�һ������
		//	�����������ָ��ÿ���������Ե��ֽڴ�С�������3 * sizeof(float)��ʾÿ����������ռ�õ��ֽ�������Ϊÿ��������3�������������Գ���sizeof(float)��
		//	������������ָ�������������ݵ���ʼλ�û��߻�����ƫ�����������nullptr��ʾ�����Ǵӵ�ǰ�󶨵Ķ��㻺�����Ŀ�ͷ��ʼ�ġ�
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), nullptr);

		//glGenBuffers(1, &m_IndexBuffer);
		//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_IndexBuffer);
		//OpenGL����IBO(��������������)�е�����ֵ�����Ҷ�Ӧ�Ķ������ݡ����磬����ֵΪ0��ʾ���㻺�����еĵ�һ�����㣬����ֵΪ1��ʾ�ڶ������㣬
		// �Դ����ơ�OpenGL��������ֵ�ҵ���Ӧ�Ķ������ݣ�������˳�����������γ�ͼԪ���������Σ�
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


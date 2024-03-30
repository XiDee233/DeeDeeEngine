#include <Dee.h>

#include <glm/vec3.hpp> // glm::vec3
#include <glm/vec4.hpp> // glm::vec4
#include <glm/mat4x4.hpp> // glm::mat4
#include <glm/gtc/matrix_transform.hpp> // glm::translate, glm::rotate, glm::scale, glm::perspective
#include "imgui/imgui.h"
glm::mat4 camera(float Translate, glm::vec2 const& Rotate)
{
	glm::mat4 Projection = glm::perspective(glm::radians(45.0f), 4.0f / 3.0f, 0.1f, 100.f);
	glm::mat4 View = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, -Translate));
	View = glm::rotate(View, Rotate.y, glm::vec3(-1.0f, 0.0f, 0.0f));
	View = glm::rotate(View, Rotate.x, glm::vec3(0.0f, 1.0f, 0.0f));
	glm::mat4 Model = glm::scale(glm::mat4(1.0f), glm::vec3(0.5f));
	return Projection * View * Model;
}

class ExampleLayer :public DeeDeeEngine::Layer {
public:
	ExampleLayer()
		:Layer("Example"), m_Camera(-1.6f, 1.6f, -0.9f, 0.9f), m_CameraPosition(0.0f)
	{

		m_VertexArray.reset(DeeDeeEngine::VertexArray::Create());

		float vertices[3 * 7] = {
			-0.5f, -0.5f, 0.0f,0.0f,1.0f,0.0f,1.0f,
			0.5f, -0.5f, 0.0f, 0.0f,0.0f,1.0f,1.0f,
			0.0f, 0.5f, 0.0f,  1.0f,0.0f,0.0f,1.0f,
		};
		std::shared_ptr<DeeDeeEngine::VertexBuffer> triVB;
		triVB.reset(DeeDeeEngine::VertexBuffer::Create(vertices, sizeof(vertices)));

		DeeDeeEngine::BufferLayout _layout = {
			{DeeDeeEngine::ShaderDataType::Float3,"a_Position"},
			{DeeDeeEngine::ShaderDataType::Float4,"a_Color"}

		};
		triVB->SetLayout(_layout);

		m_VertexArray->AddVertexBuffer(triVB);

		uint32_t indices[3] = { 0,1,2 };
		std::shared_ptr<DeeDeeEngine::IndexBuffer> triIB;
		triIB.reset(DeeDeeEngine::IndexBuffer::Create(indices, sizeof(indices) / sizeof(uint32_t)));
		m_VertexArray->SetIndexBuffer(triIB);

		m_SquareVA.reset(DeeDeeEngine::VertexArray::Create());


		float vertices2[3 * 4] = {
			-0.75f, -0.75f, 0.0f,
			 0.75f, -0.75f, 0.0f,
			 0.75f,  0.75f, 0.0f,
			-0.75f,  0.75f, 0.0f,

		};
		std::shared_ptr<DeeDeeEngine::VertexBuffer> squareVB;
		squareVB.reset(DeeDeeEngine::VertexBuffer::Create(vertices2, sizeof(vertices2)));
		DeeDeeEngine::BufferLayout _layout2 = {
			{DeeDeeEngine::ShaderDataType::Float3,"a_Position"},
		};
		squareVB->SetLayout(_layout2);

		m_SquareVA->AddVertexBuffer(squareVB);

		uint32_t indices2[6] = { 0,1,2,2,3,0 };
		std::shared_ptr<DeeDeeEngine::IndexBuffer> squareIB;
		squareIB.reset(DeeDeeEngine::IndexBuffer::Create(indices, sizeof(indices) / sizeof(uint32_t)));
		squareIB.reset(DeeDeeEngine::IndexBuffer::Create(indices2, sizeof(indices2) / sizeof(uint32_t)));
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

		m_Shader.reset(new DeeDeeEngine::Shader(vertexSrc, fragmentSrc));

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

		m_Shader2.reset(new DeeDeeEngine::Shader(vertexSrc2, fragmentSrc2));
	}
	void ExampleLayer::OnUpdate() override {
		// 获取相机的旋转角度（假设你的相机旋转是绕Z轴进行的）
		float radians = glm::radians(m_CameraRotation);

		// 计算相机的前向向量和右向量
		glm::vec3 cameraRight = glm::vec3(cos(radians), sin(radians), 0.0f);
		glm::vec3 cameraForward = glm::vec3(-sin(radians), cos(radians), 0.0f);

		// 处理按键输入
		if (DeeDeeEngine::Input::IsKeyPressed(DEE_KEY_LEFT)) {
			m_CameraPosition -= cameraRight * m_CameraSpeed;
		}
		else if (DeeDeeEngine::Input::IsKeyPressed(DEE_KEY_RIGHT)) {
			m_CameraPosition += cameraRight * m_CameraSpeed;
		}

		if (DeeDeeEngine::Input::IsKeyPressed(DEE_KEY_DOWN)) {
			m_CameraPosition -= cameraForward * m_CameraSpeed;
		}
		else if (DeeDeeEngine::Input::IsKeyPressed(DEE_KEY_UP)) {
			m_CameraPosition += cameraForward * m_CameraSpeed;
		}

		if (DeeDeeEngine::Input::IsKeyPressed(DEE_KEY_Q)) {
			m_CameraRotation += m_CameraRotateSpeed;
		}
		else if (DeeDeeEngine::Input::IsKeyPressed(DEE_KEY_E)) {
			m_CameraRotation -= m_CameraRotateSpeed;
		}

		DeeDeeEngine::RenderCommand::SetClearColor({ 0.2f, 0.2f, 0.2f, 1 });
		DeeDeeEngine::RenderCommand::Clear();
		m_Camera.SetRotation(m_CameraRotation);
		m_Camera.SetPosition(m_CameraPosition);

		DeeDeeEngine::Renderer::BeginScene(m_Camera);


		DeeDeeEngine::Renderer::Submit(m_Shader2, m_SquareVA);

		DeeDeeEngine::Renderer::Submit(m_Shader, m_VertexArray);

		DeeDeeEngine::Renderer::EndScene();
	}
	void ExampleLayer::OnEvent(DeeDeeEngine::Event& event)override {
	/*	if (event.GetEventType() == DeeDeeEngine::EventType::KeyPressed)
		{
			DeeDeeEngine::KeyPressedEvent& e = (DeeDeeEngine::KeyPressedEvent&)event;
			DEE_TRACE("{0}", (char)e.GetKeyCode());
		}*/
		DeeDeeEngine::EventDispatcher dispatcher(event);
		dispatcher.Dispatch<DeeDeeEngine::KeyPressedEvent>(DEE_BIND_EVENT_FN(ExampleLayer::OnKeyPressedEvent));

	}

	bool OnKeyPressedEvent(DeeDeeEngine::KeyPressedEvent& event) {
		
		return false;
	}
	void ExampleLayer::OnImGuiRender() override {

	}
private:

	std::shared_ptr<DeeDeeEngine::Shader> m_Shader;
	std::shared_ptr<DeeDeeEngine::VertexArray> m_VertexArray;


	std::shared_ptr<DeeDeeEngine::Shader> m_Shader2;
	std::shared_ptr<DeeDeeEngine::VertexArray> m_SquareVA;

	DeeDeeEngine::OrthographicCamera m_Camera;
	glm::vec3 m_CameraPosition;
	float m_CameraRotation = 0.0f;
	float m_CameraSpeed = 0.1f;
	float m_CameraRotateSpeed = 1.0f;

};
class Sandbox :public DeeDeeEngine::Application {
public:
	Sandbox()
	{
		PushLayer(new ExampleLayer());
	}
	~Sandbox() {

	}
};
DeeDeeEngine::Application* DeeDeeEngine::CreateApplication() {
	return new Sandbox();
}
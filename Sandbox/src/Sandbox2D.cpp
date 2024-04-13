#include "Sandbox2D.h"
#include "imgui\imgui.h"
#include <glm/gtc/matrix_transform.hpp> // glm::translate, glm::rotate, glm::scale, glm::perspective
#include <glm/gtc/type_ptr.hpp>
#include <Platform\OpenGL\OpenGLShader.h>

Sandbox2D::Sandbox2D()
	:Layer("Sandbox2D"),m_CameraController(1280.0f/720.0f)
{
}

void Sandbox2D::OnAttach()
{
	

	m_SquareVA =(DeeDeeEngine::VertexArray::Create());


	float vertices2[5 * 4] = {
		-0.5f, -0.5f, 0.0f,
		 0.5f, -0.5f, 0.0f,
		 0.5f,  0.5f, 0.0f,
		-0.5f,  0.5f, 0.0f

	};
	DeeDeeEngine::Ref<DeeDeeEngine::VertexBuffer> squareVB;
	squareVB.reset(DeeDeeEngine::VertexBuffer::Create(vertices2, sizeof(vertices2)));
	DeeDeeEngine::BufferLayout _layout2 = {
		{DeeDeeEngine::ShaderDataType::Float3,"a_Position"},
	};
	squareVB->SetLayout(_layout2);

	m_SquareVA->AddVertexBuffer(squareVB);

	uint32_t indices2[6] = { 0,1,2,2,3,0 };
	DeeDeeEngine::Ref<DeeDeeEngine::IndexBuffer> squareIB;
	squareIB.reset(DeeDeeEngine::IndexBuffer::Create(indices2, sizeof(indices2) / sizeof(uint32_t)));
	m_SquareVA->SetIndexBuffer(squareIB);
	

	m_FlatColorShader = DeeDeeEngine::Shader::Create("assets/shaders/FlatColor.glsl");


	
}

void Sandbox2D::OnDetach()
{
}

void Sandbox2D::OnUpdate(DeeDeeEngine::Timestep ts)
{
	m_CameraController.OnUpdate(ts);
	DeeDeeEngine::RenderCommand::SetClearColor({ 0.2f, 0.2f, 0.2f, 1 });
	DeeDeeEngine::RenderCommand::Clear();


	DeeDeeEngine::Renderer::BeginScene(m_CameraController.GetCamera());



	std::dynamic_pointer_cast<DeeDeeEngine::OpenGLShader>(m_FlatColorShader)->Bind();
	std::dynamic_pointer_cast<DeeDeeEngine::OpenGLShader>(m_FlatColorShader)->UploadUniformFloat4("u_Color", m_SquareColor);


	
	DeeDeeEngine::Renderer::Submit(m_FlatColorShader, m_SquareVA, glm::scale(glm::mat4(1.0f), glm::vec3(1.5f)));
	DeeDeeEngine::Renderer::EndScene();
}

void Sandbox2D::OnImGuiRender()
{
	ImGui::Begin("Settings");
	ImGui::ColorEdit4("Square Color", glm::value_ptr(m_SquareColor));
	ImGui::End();
}

void Sandbox2D::OnEvent(DeeDeeEngine::Event& e)
{
}

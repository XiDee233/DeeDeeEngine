#include "Sandbox2D.h"
#include "imgui\imgui.h"
#include <glm/gtc/matrix_transform.hpp> // glm::translate, glm::rotate, glm::scale, glm::perspective
#include <glm/gtc/type_ptr.hpp>

Sandbox2D::Sandbox2D()
	:Layer("Sandbox2D"),m_CameraController(1280.0f/720.0f)
{
}

void Sandbox2D::OnAttach()
{
	



	
}

void Sandbox2D::OnDetach()
{
}

void Sandbox2D::OnUpdate(DeeDeeEngine::Timestep ts)
{
	m_CameraController.OnUpdate(ts);
	DeeDeeEngine::RenderCommand::SetClearColor({ 0.2f, 0.2f, 0.2f, 1 });
	DeeDeeEngine::RenderCommand::Clear();


	DeeDeeEngine::Renderer2D::BeginScene(m_CameraController.GetCamera());
	DeeDeeEngine::Renderer2D::DrawQuad({ 0.0f,0.0f }, { 1.0f,1.0f }, { 0.8f,0.2f,0.3f,1.0f });
	DeeDeeEngine::Renderer2D::DrawQuad({ 0.5f,-0.5f }, { 0.5f,0.75f }, { 0.2f,0.3f,0.8f,1.0f });



	//std::dynamic_pointer_cast<DeeDeeEngine::OpenGLShader>(m_FlatColorShader)->Bind();
	//std::dynamic_pointer_cast<DeeDeeEngine::OpenGLShader>(m_FlatColorShader)->UploadUniformFloat4("u_Color", m_SquareColor);


	//
	//DeeDeeEngine::Renderer::Submit(m_FlatColorShader, m_SquareVA, glm::scale(glm::mat4(1.0f), glm::vec3(1.5f)));
	DeeDeeEngine::Renderer2D::EndScene();
}

void Sandbox2D::OnImGuiRender()
{
	ImGui::Begin("Settings");
	ImGui::ColorEdit4("Square Color", glm::value_ptr(m_SquareColor));
	ImGui::End();
}

void Sandbox2D::OnEvent(DeeDeeEngine::Event& e)
{
	m_CameraController.OnEvent(e);
}

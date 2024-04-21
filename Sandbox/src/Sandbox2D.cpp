#include "Sandbox2D.h"
#include "imgui\imgui.h"
#include <glm/gtc/matrix_transform.hpp> // glm::translate, glm::rotate, glm::scale, glm::perspective
#include <glm/gtc/type_ptr.hpp>
#include <chrono>



Sandbox2D::Sandbox2D()
	:Layer("Sandbox2D"), m_CameraController(1280.0f / 720.0f)
{
}

void Sandbox2D::OnAttach()
{
	DEE_PROFILE_FUNCTION();

	m_CheckerboardTexture = DeeDeeEngine::Texture2D::Create("assets/textures/cjy.png");
	

}

void Sandbox2D::OnDetach()
{
}



void Sandbox2D::OnUpdate(DeeDeeEngine::Timestep ts)
{
	DEE_PROFILE_FUNCTION();
	{
		DEE_PROFILE_SCOPE("m_CameraController.OnUpdate")
			m_CameraController.OnUpdate(ts);
	}
	DeeDeeEngine::RenderCommand::SetClearColor({ 0.2f, 0.2f, 0.2f, 1 });
	DeeDeeEngine::RenderCommand::Clear();

	{

		static float rotation = 0.0f;
		rotation += ts * 20.0f;
		DEE_PROFILE_SCOPE("DrawQuad")
		DeeDeeEngine::Renderer2D::BeginScene(m_CameraController.GetCamera());
		DeeDeeEngine::Renderer2D::DrawQuad({ 0.0f,0.0f }, { 1.0f,1.0f }, { 0.8f,0.2f,0.3f,1.0f });
		DeeDeeEngine::Renderer2D::DrawRotateQuad({ -2.0f,-2.0f }, { 1.0f,1.0f }, rotation*10.0f, { 0.8f,0.2f,0.3f,1.0f });
		DeeDeeEngine::Renderer2D::DrawQuad({ 1.5f,-0.5f }, { 0.5f,0.75f }, { 0.2f,0.3f,0.8f,1.0f });
		DeeDeeEngine::Renderer2D::DrawQuad({ 0.0f,0.0f,-0.1f }, { 10.0f,10.0f }, m_CheckerboardTexture,10.0f);
		DeeDeeEngine::Renderer2D::DrawQuad({ -3.0f,-3.0f,-0.1f }, { 2.0f,2.0f }, m_CheckerboardTexture, 10.0f);

		DeeDeeEngine::Renderer2D::DrawRotateQuad({ 1.0f,1.0f,-0.05f }, { 10.0f,10.0f }, rotation, m_CheckerboardTexture, 10.f,glm::vec4(0.5f,0.1f,0.1f,1.0f));




		//std::dynamic_pointer_cast<DeeDeeEngine::OpenGLShader>(m_FlatColorShader)->Bind();
		//std::dynamic_pointer_cast<DeeDeeEngine::OpenGLShader>(m_FlatColorShader)->UploadUniformFloat4("u_Color", m_SquareColor);


		//
		//DeeDeeEngine::Renderer::Submit(m_FlatColorShader, m_SquareVA, glm::scale(glm::mat4(1.0f), glm::vec3(1.5f)));
		DeeDeeEngine::Renderer2D::EndScene();
	}
}

void Sandbox2D::OnImGuiRender()
{

	ImGui::Begin("Settings");

	ImGui::End();
	

}

void Sandbox2D::OnEvent(DeeDeeEngine::Event& e)
{

	m_CameraController.OnEvent(e);
}

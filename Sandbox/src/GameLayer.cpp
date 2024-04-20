#include "GameLayer.h"
#include "imgui\imgui.h"
#include <glm/gtc/matrix_transform.hpp> // glm::translate, glm::rotate, glm::scale, glm::perspective
#include <glm/gtc/type_ptr.hpp>
#include <chrono>

using namespace DeeDeeEngine;

GameLayer::GameLayer()
	:Layer("GameLayer")
{
	auto& window = Application::Get().GetWindow();
	CreateCamera(window.GetWidth(), window.GetHeight());
}



void GameLayer::OnAttach()
{
	m_Level.Init();
}

void GameLayer::OnDetach()
{
}



void GameLayer::OnUpdate(DeeDeeEngine::Timestep ts)
{
	m_Level.OnUpdate(ts);

	const auto& playerPos = m_Level.GetPlayer().GetPosition();
	m_Camera->SetPosition({ playerPos.x,0.0f,0.0f });
	DeeDeeEngine::RenderCommand::SetClearColor({ 0.2f, 0.2f, 0.2f, 1 });
	DeeDeeEngine::RenderCommand::Clear();

	DeeDeeEngine::Renderer2D::BeginScene(*m_Camera);
	m_Level.OnRender();
	DeeDeeEngine::Renderer2D::EndScene();
}

void GameLayer::OnImGuiRender()
{

	ImGui::Begin("Settings");
	m_Level.OnImGuiRender();
	ImGui::End();


}

void GameLayer::OnEvent(DeeDeeEngine::Event& e)
{
	EventDispatcher dispatcher(e);
	dispatcher.Dispatch<WindowResizeEvent>(DEE_BIND_EVENT_FN(GameLayer::OnWindowResize));
}

bool GameLayer::OnWindowResize(DeeDeeEngine::WindowResizeEvent& e)
{
	CreateCamera(e.GetWidth(), e.GetHeight());
	return false;
}

void GameLayer::CreateCamera(uint32_t width, uint32_t height)
{
	float aspectRatio = (float)width / (float)height;
	float camWidth = 10.0f;
	float bottom = -camWidth;
	float top = camWidth;
	float left = bottom* aspectRatio;
	float right = top * aspectRatio;
	m_Camera = CreateScope<DeeDeeEngine::OrthographicCamera>(left, right, bottom, top);
}

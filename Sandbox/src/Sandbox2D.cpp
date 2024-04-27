#include "Sandbox2D.h"
#include "imgui\imgui.h"
#include <glm/gtc/matrix_transform.hpp> // glm::translate, glm::rotate, glm::scale, glm::perspective
#include <glm/gtc/type_ptr.hpp>
#include <chrono>

static const uint32_t s_MapWidth = 23;
static const char* s_MapTiles = 
"wwwwwwwwwwwwwwwwwwwwwww"
"wwwwwwwwwddwwwwwwwwwwww"
"wwwwddddddddddddwwwwwww"
"wwwwdddwwwddddddddwwwww"
"wwwwdddwwwwdddddddwwwww"
"wwwwdddddwddddddddwwwww"
"wwwwddddddddddddddwwwww"
"wwwwddddddddddddddwwwww"
"wwwwwwwwdddwwwwwwwwwwww"
"wwwwwwwwwwwwwwwwwwwwwww"
"wwwwwwwwwwwwwwwwwwwwwww"
;

Sandbox2D::Sandbox2D()
	:Layer("Sandbox2D"), m_CameraController(1280.0f / 720.0f)
{
}

void Sandbox2D::OnAttach()
{
	DEE_PROFILE_FUNCTION();

	m_CheckerboardTexture = DeeDeeEngine::Texture2D::Create("assets/textures/cjy.png");
	m_SpriteSheet = DeeDeeEngine::Texture2D::Create("assets/textures/rpg.png");

	m_MapWidth = s_MapWidth;
	m_MapHeight = strlen(s_MapTiles) / s_MapWidth;

	m_TextureStairs = DeeDeeEngine::SubTexture2D::CreateFromCoords(m_SpriteSheet, { 7,6 }, { 128.0f,128.0f });
	m_TextureBarrel = DeeDeeEngine::SubTexture2D::CreateFromCoords(m_SpriteSheet, { 8,2 }, { 128.0f,128.0f }, {1,2});
	s_TextureMap['d'] = DeeDeeEngine::SubTexture2D::CreateFromCoords(m_SpriteSheet, {6,11}, {128.0f,128.0f}, {1,1});
	s_TextureMap['w'] = DeeDeeEngine::SubTexture2D::CreateFromCoords(m_SpriteSheet, { 11,11 }, { 128.0f,128.0f }, { 1,1 });


	m_CameraController.SetZoomLevel(5.5f);

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
	DeeDeeEngine::Renderer2D::ResetStats();
	DeeDeeEngine::RenderCommand::SetClearColor({ 0.2f, 0.2f, 0.2f, 1 });
	DeeDeeEngine::RenderCommand::Clear();
#if 1
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

		for (uint32_t y = 0; y < m_MapHeight; y++)
		{
			for (uint32_t x = 0; x < m_MapWidth; x++) {
				char tileType = s_MapTiles[x + y * m_MapWidth];
				DeeDeeEngine::Ref<DeeDeeEngine::SubTexture2D> texture;
				if (s_TextureMap.find(tileType)!=s_TextureMap.end())
				{
					texture = s_TextureMap[tileType];
				}
				else {
					texture = m_TextureBarrel;
				}
				DeeDeeEngine::Renderer2D::DrawQuad({ x,y,0.1f }, { 1.0f,1.0f }, texture);

			}
		}


		//std::dynamic_pointer_cast<DeeDeeEngine::OpenGLShader>(m_FlatColorShader)->Bind();
		//std::dynamic_pointer_cast<DeeDeeEngine::OpenGLShader>(m_FlatColorShader)->UploadUniformFloat4("u_Color", m_SquareColor);


		//
		//DeeDeeEngine::Renderer::Submit(m_FlatColorShader, m_SquareVA, glm::scale(glm::mat4(1.0f), glm::vec3(1.5f)));
		//DeeDeeEngine::Renderer2D::EndScene();

		//DeeDeeEngine::Renderer2D::BeginScene(m_CameraController.GetCamera());
		for (float y = -5.0f; y < 5.0f; y += 0.5f)
		{
			for (float x = -5.0f; x < 5.0f; x += 0.5f)
			{
				glm::vec4 color = { (x + 5.0f) / 10.0f, 0.4f, (y + 5.0f) / 10.0f, 0.7f };
				DeeDeeEngine::Renderer2D::DrawQuad({ x, y }, { 0.45f, 0.45f }, color);
			}
		}
		DeeDeeEngine::Renderer2D::EndScene();
}

#endif

		DeeDeeEngine::Renderer2D::BeginScene(m_CameraController.GetCamera());
		DeeDeeEngine::Renderer2D::DrawQuad({ 0.0f,0.0f,0.1f }, { 1.0f,1.0f }, m_TextureStairs);
		DeeDeeEngine::Renderer2D::DrawQuad({ 1.0f,0.0f,0.1f }, { 1.0f,2.0f }, m_TextureBarrel);

		DeeDeeEngine::Renderer2D::EndScene();
}

void Sandbox2D::OnImGuiRender()
{

	ImGui::Begin("Settings"); 
	auto stats = DeeDeeEngine::Renderer2D::GetStats();
	ImGui::Text("Renderer2D Stats:");
	ImGui::Text("Draw Calls: %d", stats.DrawCalls);
	ImGui::Text("Quads: %d", stats.QuadCount);
	ImGui::Text("Vertices: %d", stats.GetTotalVertexCount());
	ImGui::Text("Indices: %d", stats.GetTotalIndexCount());

	ImGui::End();

	static bool show = true;
	ImGui::ShowDemoWindow(&show);
	

}

void Sandbox2D::OnEvent(DeeDeeEngine::Event& e)
{

	m_CameraController.OnEvent(e);
}

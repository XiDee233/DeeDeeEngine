#include "EditorLayer.h"
#include "imgui\imgui.h"
#include <glm/gtc/matrix_transform.hpp> // glm::translate, glm::rotate, glm::scale, glm::perspective
#include <glm/gtc/type_ptr.hpp>
#include <chrono>

namespace DeeDeeEngine {

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

	EditorLayer::EditorLayer()
		:Layer("EditorLayer"), m_CameraController(1280.0f / 720.0f)
	{
	}

	void EditorLayer::OnAttach()
	{
		DEE_PROFILE_FUNCTION();

		m_CheckerboardTexture = DeeDeeEngine::Texture2D::Create("assets/textures/cjy.png");
		m_SpriteSheet = DeeDeeEngine::Texture2D::Create("assets/textures/rpg.png");

		DeeDeeEngine::FramebufferSpecification fbSpec;
		fbSpec.Width = 1280;
		fbSpec.Height = 720;
		m_Framebuffer = DeeDeeEngine::Framebuffer::Create(fbSpec);

		m_MapWidth = s_MapWidth;
		m_MapHeight = strlen(s_MapTiles) / s_MapWidth;

		m_TextureStairs = DeeDeeEngine::SubTexture2D::CreateFromCoords(m_SpriteSheet, { 7,6 }, { 128.0f,128.0f });
		m_TextureBarrel = DeeDeeEngine::SubTexture2D::CreateFromCoords(m_SpriteSheet, { 8,2 }, { 128.0f,128.0f }, { 1,2 });
		s_TextureMap['d'] = DeeDeeEngine::SubTexture2D::CreateFromCoords(m_SpriteSheet, { 6,11 }, { 128.0f,128.0f }, { 1,1 });
		s_TextureMap['w'] = DeeDeeEngine::SubTexture2D::CreateFromCoords(m_SpriteSheet, { 11,11 }, { 128.0f,128.0f }, { 1,1 });


		m_CameraController.SetZoomLevel(5.5f);

	}

	void EditorLayer::OnDetach()
	{
	}



	void EditorLayer::OnUpdate(DeeDeeEngine::Timestep ts)
	{

		DEE_PROFILE_FUNCTION();
		{
			DEE_PROFILE_SCOPE("m_CameraController.OnUpdate")
				m_CameraController.OnUpdate(ts);
		}
		DeeDeeEngine::Renderer2D::ResetStats();

		m_Framebuffer->Bind();
		DeeDeeEngine::RenderCommand::SetClearColor({ 0.2f, 0.2f, 0.2f, 1 });
		DeeDeeEngine::RenderCommand::Clear();

#if 1
		{

			static float rotation = 0.0f;
			rotation += ts * 20.0f;
			DEE_PROFILE_SCOPE("DrawQuad")
				DeeDeeEngine::Renderer2D::BeginScene(m_CameraController.GetCamera());
			DeeDeeEngine::Renderer2D::DrawQuad({ 0.0f,0.0f }, { 1.0f,1.0f }, { 0.8f,0.2f,0.3f,1.0f });
			DeeDeeEngine::Renderer2D::DrawRotateQuad({ -2.0f,-2.0f }, { 1.0f,1.0f }, rotation * 10.0f, { 0.8f,0.2f,0.3f,1.0f });
			DeeDeeEngine::Renderer2D::DrawQuad({ 1.5f,-0.5f }, { 0.5f,0.75f }, { 0.2f,0.3f,0.8f,1.0f });
			DeeDeeEngine::Renderer2D::DrawQuad({ 0.0f,0.0f,-0.1f }, { 10.0f,10.0f }, m_CheckerboardTexture, 10.0f);
			DeeDeeEngine::Renderer2D::DrawQuad({ -3.0f,-3.0f,-0.1f }, { 2.0f,2.0f }, m_CheckerboardTexture, 10.0f);

			DeeDeeEngine::Renderer2D::DrawRotateQuad({ 1.0f,1.0f,-0.05f }, { 10.0f,10.0f }, rotation, m_CheckerboardTexture, 10.f, glm::vec4(0.5f, 0.1f, 0.1f, 1.0f));

			for (uint32_t y = 0; y < m_MapHeight; y++)
			{
				for (uint32_t x = 0; x < m_MapWidth; x++) {
					char tileType = s_MapTiles[x + y * m_MapWidth];
					DeeDeeEngine::Ref<DeeDeeEngine::SubTexture2D> texture;
					if (s_TextureMap.find(tileType) != s_TextureMap.end())
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
			m_Framebuffer->Unbind();
		}

#endif

		DeeDeeEngine::Renderer2D::BeginScene(m_CameraController.GetCamera());
		DeeDeeEngine::Renderer2D::DrawQuad({ 0.0f,0.0f,0.1f }, { 1.0f,1.0f }, m_TextureStairs);
		DeeDeeEngine::Renderer2D::DrawQuad({ 1.0f,0.0f,0.1f }, { 1.0f,2.0f }, m_TextureBarrel);

		DeeDeeEngine::Renderer2D::EndScene();
	}

	void EditorLayer::OnImGuiRender()
	{

		//
		//static bool dockspaceOpen = true;
		//static bool opt_fullscreen = true;
		//static bool opt_padding = false;
		//static ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_None;

		//
		//ImGuiWindowFlags window_flags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;
		//if (opt_fullscreen)
		//{
		//	const ImGuiViewport* viewport = ImGui::GetMainViewport();
		//	ImGui::SetNextWindowPos(viewport->WorkPos);
		//	ImGui::SetNextWindowSize(viewport->WorkSize);
		//	ImGui::SetNextWindowViewport(viewport->ID);
		//	ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
		//	ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
		//	window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
		//	window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;
		//}
		//else
		//{
		//	dockspace_flags &= ~ImGuiDockNodeFlags_PassthruCentralNode;
		//}


		//if (dockspace_flags & ImGuiDockNodeFlags_PassthruCentralNode)
		//	window_flags |= ImGuiWindowFlags_NoBackground;


		//if (!opt_padding)
		//	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
		//ImGui::Begin("DockSpace Demo", &dockspaceOpen, window_flags);
		//if (!opt_padding)
		//	ImGui::PopStyleVar();

		//if (opt_fullscreen)
		//	ImGui::PopStyleVar(2);

		//ImGuiIO& io = ImGui::GetIO();
		//if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable)
		//{
		//	ImGuiID dockspace_id = ImGui::GetID("MyDockSpace");
		//	ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspace_flags);
		//}
		//else
		//{
		//}

		//if (ImGui::BeginMenuBar())
		//{
		//	if (ImGui::BeginMenu("File"))
		//	{
		//		

		//		if (ImGui::MenuItem("Exit"))DeeDeeEngine::Application::Get().Close();
		//		
		//		ImGui::EndMenu();
		//	}
		//	
		//	ImGui::EndMenuBar();
		//}
		ImGui::Begin("Settings");

		auto stats = DeeDeeEngine::Renderer2D::GetStats();
		ImGui::Text("Renderer2D Stats:");
		ImGui::Text("Draw Calls: %d", stats.DrawCalls);
		ImGui::Text("Quads: %d", stats.QuadCount);
		ImGui::Text("Vertices: %d", stats.GetTotalVertexCount());
		ImGui::Text("Indices: %d", stats.GetTotalIndexCount());

		uint32_t textureID = m_Framebuffer->GetColorAttachmentRendererID();

		ImGui::Image((void*)textureID, ImVec2(1280.0f, 720.0f), ImVec2{ 0,1 }, ImVec2{1,0});


		ImGui::End();

		//ImGui::End();


	}

	void EditorLayer::OnEvent(DeeDeeEngine::Event& e)
	{

		m_CameraController.OnEvent(e);
	}
}
#include "EditorLayer.h"
#include "imgui\imgui.h"
#include <glm/gtc/matrix_transform.hpp> // glm::translate, glm::rotate, glm::scale, glm::perspective
#include <glm/gtc/type_ptr.hpp>
#include <chrono>

#include "ImGuizmo.h"

#include "DeeDeeEngine/Math/Math.h"

#include "DeeDeeEngine\Scene\ScriptableEntity.h"
#include "DeeDeeEngine\Scene\SceneSerializer.h"
#include "DeeDeeEngine\Utils\PlatformUtils.h"

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

		m_ActiveScene = CreateRef<Scene>();
		Entity square = m_ActiveScene->CreateEntity("Hello Entity");
		square.AddComponent<SpriteRendererComponent>(glm::vec4(0.0f, 1.0f, 0.0f, 1.0f));

		auto redSquare = m_ActiveScene->CreateEntity("Red Square");
		redSquare.AddComponent<SpriteRendererComponent>(glm::vec4{ 1.0f, 0.0f, 0.0f, 1.0f });

		m_SquareEntity = square;

		m_CameraEntity = m_ActiveScene->CreateEntity("MainCamera");
		m_CameraEntity.AddComponent<CameraComponent>();
		m_SecondCamera = m_ActiveScene->CreateEntity("SecondCamera");
		auto& cc = m_SecondCamera.AddComponent<CameraComponent>();
		cc.Primary = false;
		//m_CameraController.SetZoomLevel(5.5f);

		class CameraController :public ScriptableEntity {
		public:
			virtual void OnCreate()override {
				auto& translation = GetComponent<TransformComponent>().Translation;
				translation.x = rand() % 10 - 5.0f;

			}

			virtual void OnDestroy()override {

			}

			virtual void OnUpdate(Timestep ts)override {
				auto& translation = GetComponent<TransformComponent>().Translation;
				float speed = 5.0f;

				/*if (Input::IsKeyPressed(DEE_KEY_A))
					translation.x -= speed * ts;
				if (Input::IsKeyPressed(DEE_KEY_D))
					translation.x += speed * ts;
				if (Input::IsKeyPressed(DEE_KEY_W))
					translation.y += speed * ts;
				if (Input::IsKeyPressed(DEE_KEY_S))
					translation.y -= speed * ts;*/
			}
		};
		m_CameraEntity.AddComponent<NativeScriptComponent>().Bind<CameraController>();
		m_SecondCamera.AddComponent<NativeScriptComponent>().Bind<CameraController>();

		m_SceneHierarchyPanel.SetContext(m_ActiveScene);
	}

	void EditorLayer::OnDetach()
	{
	}



	void EditorLayer::OnUpdate(DeeDeeEngine::Timestep ts)
	{
		if (FramebufferSpecification spec = m_Framebuffer->GetSpecification();
			m_ViewportSize.x > 0.0f && m_ViewportSize.y > 0.0f && // zero sized framebuffer is invalid
			(spec.Width != m_ViewportSize.x || spec.Height != m_ViewportSize.y))
		{
			m_Framebuffer->Resize((uint32_t)m_ViewportSize.x, (uint32_t)m_ViewportSize.y);
			m_CameraController.OnResize(m_ViewportSize.x, m_ViewportSize.y);
			m_ActiveScene->OnViewportResize((uint32_t)m_ViewportSize.x, (uint32_t)m_ViewportSize.y);
		}

		DEE_PROFILE_FUNCTION();
		{
			DEE_PROFILE_SCOPE("m_CameraController.OnUpdate")
				if (m_ViewportFocused)
				{
					m_CameraController.OnUpdate(ts);
				}
		}
		DeeDeeEngine::Renderer2D::ResetStats();

		m_Framebuffer->Bind();
		DeeDeeEngine::RenderCommand::SetClearColor({ 0.2f, 0.2f, 0.2f, 1 });
		DeeDeeEngine::RenderCommand::Clear();


		m_ActiveScene->OnUpdate(ts);

		m_Framebuffer->Unbind();
	}

	void EditorLayer::OnImGuiRender()
	{


		static bool dockspaceOpen = true;
		static bool opt_fullscreen = true;
		static bool opt_padding = false;
		static ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_None;


		ImGuiWindowFlags window_flags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;
		if (opt_fullscreen)
		{
			const ImGuiViewport* viewport = ImGui::GetMainViewport();
			ImGui::SetNextWindowPos(viewport->WorkPos);
			ImGui::SetNextWindowSize(viewport->WorkSize);
			ImGui::SetNextWindowViewport(viewport->ID);
			ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
			ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
			window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
			window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;
		}
		else
		{
			dockspace_flags &= ~ImGuiDockNodeFlags_PassthruCentralNode;
		}


		if (dockspace_flags & ImGuiDockNodeFlags_PassthruCentralNode)
			window_flags |= ImGuiWindowFlags_NoBackground;


		if (!opt_padding)
			ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
		ImGui::Begin("DockSpace Demo", &dockspaceOpen, window_flags);
		if (!opt_padding)
			ImGui::PopStyleVar();

		if (opt_fullscreen)
			ImGui::PopStyleVar(2);

		ImGuiIO& io = ImGui::GetIO();
		ImGuiStyle& style = ImGui::GetStyle();
		float minWinSizeX = style.WindowMinSize.x;
		style.WindowMinSize.x = 370.0f;
		if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable)
		{
			ImGuiID dockspace_id = ImGui::GetID("MyDockSpace");
			ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspace_flags);
		}
		else
		{
		}
		style.WindowMinSize.x = minWinSizeX;

		if (ImGui::BeginMenuBar())
		{
			if (ImGui::BeginMenu(u8"文件"))
			{
				if (ImGui::MenuItem(u8"新建", "Ctrl+N"))
					NewScene();

				if (ImGui::MenuItem(u8"打开...", "Ctrl+O"))
					OpenScene();

				if (ImGui::MenuItem(u8"保存...", "Ctrl+Shift+S"))
					SaveSceneAs();

				if (ImGui::MenuItem(u8"退出"))DeeDeeEngine::Application::Get().Close();

				ImGui::EndMenu();
			}

			ImGui::EndMenuBar();
		}
		m_SceneHierarchyPanel.OnImGuiRender();
		ImGui::Begin(u8"状态");

		auto stats = DeeDeeEngine::Renderer2D::GetStats();
		ImGui::Text("Renderer2D Stats:");
		ImGui::Text("Draw Calls: %d", stats.DrawCalls);
		ImGui::Text("Quads: %d", stats.QuadCount);
		ImGui::Text("Vertices: %d", stats.GetTotalVertexCount());
		ImGui::Text("Indices: %d", stats.GetTotalIndexCount());

		ImGui::End();

		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));


		ImGui::Begin(u8"视口");
		m_ViewportFocused = ImGui::IsWindowFocused();
		m_ViewportHovered = ImGui::IsWindowHovered();
		Application::Get().GetImGuiLayer()->SetBlockEvents(!m_ViewportFocused || !m_ViewportHovered);
		ImVec2 viewportPanelSize = ImGui::GetContentRegionAvail();
		if (m_ViewportSize != *(glm::vec2*)&viewportPanelSize) {
			m_ViewportSize = { viewportPanelSize.x, viewportPanelSize.y };
		}
		uint32_t textureID = m_Framebuffer->GetColorAttachmentRendererID();

		ImGui::Image((void*)textureID, ImVec2(m_ViewportSize.x, m_ViewportSize.y), ImVec2{ 0,1 }, ImVec2{ 1,0 });

		// Gizmos
		Entity selectedEntity = m_SceneHierarchyPanel.GetSelectedEntity();
		if (selectedEntity && m_GizmoType != -1)
		{
			ImGuizmo::SetOrthographic(false);
			ImGuizmo::SetDrawlist();

			float windowWidth = (float)ImGui::GetWindowWidth();
			float windowHeight = (float)ImGui::GetWindowHeight();
			ImGuizmo::SetRect(ImGui::GetWindowPos().x, ImGui::GetWindowPos().y, windowWidth, windowHeight);

			// Camera
			auto cameraEntity = m_ActiveScene->GetPrimaryCameraEntity();
			const auto& camera = cameraEntity.GetComponent<CameraComponent>().Camera;
			const glm::mat4& cameraProjection = camera.GetProjection();
			glm::mat4 cameraView = glm::inverse(cameraEntity.GetComponent<TransformComponent>().GetTransform());

			// Entity transform
			auto& tc = selectedEntity.GetComponent<TransformComponent>();
			glm::mat4 transform = tc.GetTransform();

			// Snapping
			bool snap = Input::IsKeyPressed(DEE_KEY_LEFT_CONTROL);
			float snapValue = 0.5f; // Snap to 0.5m for translation/scale
			// Snap to 45 degrees for rotation
			if (m_GizmoType == ImGuizmo::OPERATION::ROTATE)
				snapValue = 45.0f;

			float snapValues[3] = { snapValue, snapValue, snapValue };

			ImGuizmo::Manipulate(glm::value_ptr(cameraView), glm::value_ptr(cameraProjection),
				(ImGuizmo::OPERATION)m_GizmoType, ImGuizmo::LOCAL, glm::value_ptr(transform),
				nullptr, snap ? snapValues : nullptr);

			if (ImGuizmo::IsUsing())
			{
				glm::vec3 translation, rotation, scale;
				Math::DecomposeTransform(transform, translation, rotation, scale);

				glm::vec3 deltaRotation = rotation - tc.Rotation;
				tc.Translation = translation;
				tc.Rotation += deltaRotation;
				tc.Scale = scale;
			}
		}


		ImGui::End();
		ImGui::PopStyleVar();

		ImGui::End();


	}

	void EditorLayer::OnEvent(DeeDeeEngine::Event& e)
	{

		m_CameraController.OnEvent(e);

		EventDispatcher dispatcher(e);
		dispatcher.Dispatch<KeyPressedEvent>(DEE_BIND_EVENT_FN(EditorLayer::OnKeyPressed));
	}

	bool EditorLayer::OnKeyPressed(KeyPressedEvent& e)
	{
		if (e.IsRepeat())
			return false;

		bool control = Input::IsKeyPressed(DEE_KEY_LEFT_CONTROL) || Input::IsKeyPressed(DEE_KEY_RIGHT_CONTROL);
		bool shift = Input::IsKeyPressed(DEE_KEY_LEFT_SHIFT) || Input::IsKeyPressed(DEE_KEY_RIGHT_SHIFT);
		switch (e.GetKeyCode())
		{
		case DEE_KEY_N:
		{
			if (control)
				NewScene();

			break;
		}
		case DEE_KEY_O:
		{
			if (control)
				OpenScene();

			break;
		}
		case DEE_KEY_S:
		{
			if (control && shift)
				SaveSceneAs();

			break;
		}
		// Gizmos
		case DEE_KEY_Q:
			m_GizmoType = -1;
			break;
		case DEE_KEY_W:
			m_GizmoType = ImGuizmo::OPERATION::TRANSLATE;
			break;
		case DEE_KEY_E:
			m_GizmoType = ImGuizmo::OPERATION::ROTATE;
			break;
		case DEE_KEY_R:
			m_GizmoType = ImGuizmo::OPERATION::SCALE;
			break;
		}
	}

	void EditorLayer::NewScene()
	{
		m_ActiveScene = CreateRef<Scene>();
		m_ActiveScene->OnViewportResize((uint32_t)m_ViewportSize.x, (uint32_t)m_ViewportSize.y);
		m_SceneHierarchyPanel.SetContext(m_ActiveScene);
	}

	void EditorLayer::OpenScene()
	{
		std::string filepath = FileDialogs::OpenFile("Dee Scene (*.dee)\0*.dee\0");
		if (!filepath.empty())
		{
			m_ActiveScene = CreateRef<Scene>();
			m_ActiveScene->OnViewportResize((uint32_t)m_ViewportSize.x, (uint32_t)m_ViewportSize.y);
			m_SceneHierarchyPanel.SetContext(m_ActiveScene);

			SceneSerializer serializer(m_ActiveScene);
			serializer.Deserialize(filepath);
		}
	}

	void EditorLayer::SaveSceneAs()
	{
		std::string filepath = FileDialogs::SaveFile("Dee Scene (*.dee)\0*.dee\0");
		if (!filepath.empty())
		{
			SceneSerializer serializer(m_ActiveScene);
			serializer.Serialize(filepath);
		}
	}
}
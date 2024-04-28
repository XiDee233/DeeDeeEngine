#pragma once

#include "Dee.h"
namespace DeeDeeEngine {
	class EditorLayer :public Layer
	{
	public:
		EditorLayer();
		virtual ~EditorLayer() = default;
		virtual void OnAttach() override;
		virtual void OnDetach() override;
		void OnUpdate(Timestep ts)override;
		virtual void OnImGuiRender() override;
		void OnEvent(Event& e)override;
	private:
		OrthographicCameraController m_CameraController;

		// Temp
		Ref<VertexArray> m_SquareVA;
		Ref<Shader> m_FlatColorShader;

		Ref<Texture2D> m_CheckerboardTexture;
		Ref<Texture2D> m_SpriteSheet;
		Ref<SubTexture2D> m_TextureStairs;
		Ref<SubTexture2D> m_TextureBarrel;

		Ref<Framebuffer> m_Framebuffer;

		uint32_t m_MapWidth, m_MapHeight;


		glm::vec4 m_SquareColor = { 0.2f, 0.3f, 0.8f, 1.0f };
		std::unordered_map<char, Ref<SubTexture2D>> s_TextureMap;
	};

}
#pragma once

#include "Dee.h"
class Sandbox2D:public DeeDeeEngine::Layer
{
public:
	Sandbox2D();
	virtual ~Sandbox2D() = default;
	virtual void OnAttach() override;
	virtual void OnDetach() override;
	void OnUpdate(DeeDeeEngine::Timestep ts)override;
	virtual void OnImGuiRender() override;
	void OnEvent(DeeDeeEngine::Event& e)override;
private:
	DeeDeeEngine::OrthographicCameraController m_CameraController;

	// Temp
	DeeDeeEngine::Ref<DeeDeeEngine::VertexArray> m_SquareVA;
	DeeDeeEngine::Ref<DeeDeeEngine::Shader> m_FlatColorShader;

	DeeDeeEngine::Ref<DeeDeeEngine::Texture2D> m_CheckerboardTexture;
	DeeDeeEngine::Ref<DeeDeeEngine::Texture2D> m_SpriteSheet;
	DeeDeeEngine::Ref<DeeDeeEngine::SubTexture2D> m_TextureStairs;
	DeeDeeEngine::Ref<DeeDeeEngine::SubTexture2D> m_TextureBarrel;

	uint32_t m_MapWidth, m_MapHeight;


	glm::vec4 m_SquareColor = { 0.2f, 0.3f, 0.8f, 1.0f };
	std::unordered_map<char, DeeDeeEngine::Ref<DeeDeeEngine::SubTexture2D>> s_TextureMap;
};


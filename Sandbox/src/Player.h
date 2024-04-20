#pragma once
#include <Dee.h>
class Player
{
public:
	Player();

	void LoadAssets();

	void OnUpdate(DeeDeeEngine::Timestep ts);
	void OnRender();
	void OnImGuiRender();

	float GetRotation() { return m_Velocity.y * 4.0f - 90.0f; }
	const glm::vec2& GetPosition() const { return m_Position; }
private:
	glm::vec2 m_Position = { 0.0f,0.0f };
	glm::vec2 m_Velocity = { 0.0f,0.0f };

	float m_Gravity = 0.8f;
	float m_EnginePower = 1.0f;

	DeeDeeEngine::Ref<DeeDeeEngine::Texture2D> m_ShipTexturel;
};


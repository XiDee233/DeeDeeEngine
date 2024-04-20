#include "Player.h"

#include <imgui/imgui.h>
using namespace DeeDeeEngine;
Player::Player()
{
}

void Player::LoadAssets()
{
	m_ShipTexturel = Texture2D::Create("assets/textures/ball.png");
}

void Player::OnUpdate(DeeDeeEngine::Timestep ts)
{
	if (Input::IsKeyPressed(DEE_KEY_SPACE)) {
		m_Velocity.y += m_EnginePower;
		if (m_Velocity.y < 0.0f) {
			m_Velocity.y + m_EnginePower * 2.0f;
		}
	}
	else {
		m_Velocity.y -= m_Gravity;
	}
	m_Velocity.y = glm::clamp(m_Velocity.y, -20.0f, 20.0f);
	m_Velocity.x = 2.0f;
	m_Position += m_Velocity * (float)ts;
}

void Player::OnRender()
{
	Renderer2D::DrawRotateQuad({ m_Position.x,m_Position.y,0.5f }, { 1.0f,1.0f }, glm::radians(GetRotation()), m_ShipTexturel);
}

void Player::OnImGuiRender() {
	ImGui::DragFloat("Engine Power", &m_EnginePower, 0.1f);
	ImGui::DragFloat("Gravity", &m_Gravity, 0.1f);
}

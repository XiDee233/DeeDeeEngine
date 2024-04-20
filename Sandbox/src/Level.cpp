#include "Level.h"
#include "Color.h"
#include "Random.h"
#include <glm\ext\matrix_transform.hpp>
Level::Level()
{
}
static bool PointInQuad(const glm::vec2& p, const glm::vec2& p0, const glm::vec2& p1, const glm::vec2& p2, const glm::vec2& p3)
{
	// Triangle 1: p0, p1, p2
	float s1 = p0.y * p2.x - p0.x * p2.y + (p2.y - p0.y) * p.x + (p0.x - p2.x) * p.y;
	float t1 = p0.x * p1.y - p0.y * p1.x + (p0.y - p1.y) * p.x + (p1.x - p0.x) * p.y;

	if ((s1 < 0) != (t1 < 0))
		return false;

	float A1 = -p1.y * p2.x + p0.y * (p2.x - p1.x) + p0.x * (p1.y - p2.y) + p1.x * p2.y;

	if (A1 < 0 ? (s1 <= 0 && s1 + t1 >= A1) : (s1 >= 0 && s1 + t1 <= A1))
		return true;

	// Triangle 2: p0, p2, p3
	float s2 = p0.y * p3.x - p0.x * p3.y + (p3.y - p0.y) * p.x + (p0.x - p3.x) * p.y;
	float t2 = p0.x * p2.y - p0.y * p2.x + (p0.y - p2.y) * p.x + (p2.x - p0.x) * p.y;

	if ((s2 < 0) != (t2 < 0))
		return false;

	float A2 = -p2.y * p3.x + p0.y * (p3.x - p2.x) + p0.x * (p2.y - p3.y) + p2.x * p3.y;

	return A2 < 0 ? (s2 <= 0 && s2 + t2 >= A2) : (s2 >= 0 && s2 + t2 <= A2);
}


void Level::Init()
{
	m_Player.LoadAssets();
	m_Pillars.resize(5);
	for (int i = 0; i < 5; i++)
	{
		CreatePillar(i, i * 10.0f);
	}
}

void Level::OnUpdate(DeeDeeEngine::Timestep ts)
{
	m_Player.OnUpdate(ts);

	if (CollisionTest())
	{
		DEE_CORE_INFO("Collision");
		//GameOver();
		return;
	}
	if (m_Player.GetPosition().x>m_PillarTarget)
	{
		CreatePillar(m_PillarIndex, m_PillarTarget + 20.0f);
		m_PillarIndex = ++m_PillarIndex % m_Pillars.size();
		m_PillarTarget += 10.0f;
	}
}

void Level::OnRender()
{
	const auto& playerPos = m_Player.GetPosition();
	DeeDeeEngine::Renderer2D::DrawQuad({ playerPos.x,20.0f }, { 50.0f,10.0f }, Color::Black);
	DeeDeeEngine::Renderer2D::DrawQuad({ playerPos.x,-20.0f }, { 50.0f,10.0f }, Color::Black);
	for (auto& pillar : m_Pillars) {
		DeeDeeEngine::Renderer2D::DrawQuad(pillar.TopPosition, pillar.TopScale, Color::White);
		DeeDeeEngine::Renderer2D::DrawQuad(pillar.BottomPosition, pillar.BottomScale, Color::White);
	}
	m_Player.OnRender();
}

void Level::OnImGuiRender() {
	m_Player.OnImGuiRender();
}

void Level::CreatePillar(int index, float offset)
{
	Pillar& pillar = m_Pillars[index];
	pillar.TopPosition.x = offset;
	pillar.BottomPosition.x = offset;

	float center = Random::Float()*5.0f-2.5f;
	float gap = 2.0f;
	pillar.TopPosition.y = 12.0f + center + gap * 0.5f;
	pillar.BottomPosition.y = -12.0f + center + gap * 0.5f;

}

bool Level::CollisionTest()
{
	if (glm::abs(m_Player.GetPosition().y) > 8.5f)
		return true;

	glm::vec4 playerVertices[4] = {
		{ -0.5f, -0.5f, 0.0f, 1.0f },
		{  0.5f, -0.5f, 0.0f, 1.0f },
		{  0.5f,  0.5f, 0.0f, 1.0f },
		{ -0.5f,  0.5f, 0.0f, 1.0f }
	};

	const auto& pos = m_Player.GetPosition();
	glm::vec4 playerTransformedVerts[4];
	for (int i = 0; i < 4; i++)
	{
		playerTransformedVerts[i] = glm::translate(glm::mat4(1.0f), { pos.x, pos.y, 0.0f })
			* glm::rotate(glm::mat4(1.0f), glm::radians(m_Player.GetRotation()), { 0.0f, 0.0f, 1.0f })
			* glm::scale(glm::mat4(1.0f), { 1.0f, 1.0f, 1.0f })
			* playerVertices[i];
	}

	// Define the vertices for the quadrilateral
	glm::vec4 pillarVertices[4] = {
		{ -0.5f , -0.5f , 0.0f, 1.0f },
		{  0.5f , -0.5f , 0.0f, 1.0f },
		{  0.5f ,  0.5f , 0.0f, 1.0f },
		{ -0.5f,  0.5f , 0.0f, 1.0f }
	};

	for (auto& p : m_Pillars)
	{
		glm::vec2 quad[4];

		// Top pillars
		for (int i = 0; i < 4; i++)
		{
			quad[i] = glm::translate(glm::mat4(1.0f), { p.TopPosition.x, p.TopPosition.y, 0.0f })
				* glm::scale(glm::mat4(1.0f), { p.TopScale.x, p.TopScale.y, 1.0f })
				* pillarVertices[i];
		}

		for (auto& vert : playerTransformedVerts)
		{
			if (PointInQuad({ vert.x, vert.y }, quad[0], quad[1], quad[2], quad[3]))
				return true;
		}

		// Bottom pillars
		for (int i = 0; i < 4; i++)
		{
			quad[i] = glm::translate(glm::mat4(1.0f), { p.BottomPosition.x, p.BottomPosition.y, 0.0f })
				* glm::scale(glm::mat4(1.0f), { p.BottomScale.x, p.BottomScale.y, 1.0f })
				* pillarVertices[i];
		}

		for (auto& vert : playerTransformedVerts)
		{
			if (PointInQuad({ vert.x, vert.y }, quad[0], quad[1], quad[2], quad[3]))
				return true;
		}
	}
	return false;
}

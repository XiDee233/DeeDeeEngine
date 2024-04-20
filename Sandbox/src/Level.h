#pragma once
#include "Player.h"
class Level {
	struct Pillar {
		glm::vec3 TopPosition = { 0.0f,10.0f,0.0f };
		glm::vec2 TopScale = { 5.0f,15.0f};

		glm::vec3 BottomPosition = { 10.0f,10.0f,0.0f };
		glm::vec2 BottomScale = { 5.0f,15.0f };
	};
public:
	Level();
	void Init();
	void OnUpdate(DeeDeeEngine::Timestep ts);
	void OnRender();
	void OnImGuiRender();

	Player& GetPlayer() { return m_Player; }

private:
	void CreatePillar(int index, float offset);
	bool CollisionTest();
private:
	Player m_Player;

	float m_PillarTarget = 30.0f;
	int m_PillarIndex = 0;

	std::vector<Pillar> m_Pillars;
};
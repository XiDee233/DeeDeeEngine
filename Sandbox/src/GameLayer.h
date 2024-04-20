#pragma once

#include "Dee.h"
#include "Level.h"
class GameLayer :public DeeDeeEngine::Layer
{
public:
	GameLayer();
	virtual ~GameLayer() = default;
	virtual void OnAttach() override;
	virtual void OnDetach() override;
	void OnUpdate(DeeDeeEngine::Timestep ts)override;
	virtual void OnImGuiRender() override;
	void OnEvent(DeeDeeEngine::Event& e)override;
	bool OnWindowResize(DeeDeeEngine::WindowResizeEvent& e);
private:
	void CreateCamera(uint32_t width, uint32_t height);
private:
	DeeDeeEngine::Scope<DeeDeeEngine::OrthographicCamera> m_Camera;
	Level m_Level;

};
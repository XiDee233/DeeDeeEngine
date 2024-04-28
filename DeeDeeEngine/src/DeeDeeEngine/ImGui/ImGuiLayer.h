#pragma once
#include "DeeDeeEngine/Core/Layer.h"
#include "DeeDeeEngine/Events/KeyEvent.h"
#include "DeeDeeEngine/Events/MouseEvent.h"
#include "DeeDeeEngine/Events/ApplicationEvent.h"
namespace DeeDeeEngine {
	class DEE_API ImGuiLayer :public Layer {
	public:
		ImGuiLayer();
		~ImGuiLayer();

		virtual void OnAttach() override;
		virtual void OnDetach() override;
		virtual void OnEvent(Event& event)override;

		virtual void OnImGuiRender() override;
		void Begin();
		void End();
		void SetBlockEvents(bool block) { m_BlockEvents = block; }
	private:
		bool m_BlockEvents = true;
		float m_Time = 0.0f;
	};
}



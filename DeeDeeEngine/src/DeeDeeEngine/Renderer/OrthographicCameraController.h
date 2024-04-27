#pragma once
#include "DeeDeeEngine\Renderer\OrthographicCamera.h"
#include "DeeDeeEngine\Core\Timestep.h"
#include "DeeDeeEngine\Events\MouseEvent.h"
#include "DeeDeeEngine\Events\ApplicationEvent.h"
namespace DeeDeeEngine {
	class OrthographicCameraController {
	public:
		OrthographicCameraController(float aspectRatio, bool rotation = false);

		void OnUpdate(Timestep ts);
		void OnEvent(Event& e);

		OrthographicCamera& GetCamera() { return m_Camera; }
		const OrthographicCamera& GetCamera()const { return m_Camera; }

		void SetZoomLevel(float level) { m_ZoomLevel = level; CalculateView(); };
		float GetZoomLevel() const { return m_ZoomLevel; }
	private:
		void CalculateView();
		bool OnMouseScrolled(MouseScrolledEvent& e);
		bool OnWindowResized(WindowResizeEvent& e);
	private:
		float m_AspectRatio;
		float m_ZoomLevel = 1.0f;
		OrthographicCamera m_Camera;

		bool m_Rotation;

		glm::vec3 m_CameraPosition = { 0.0f,0.0f,0.0f };
		float m_CameraRotation = 0.0f;
		float m_CameraRotateSpeed = 1.0f, m_CameraSpeed = 1.0f;
	};

}
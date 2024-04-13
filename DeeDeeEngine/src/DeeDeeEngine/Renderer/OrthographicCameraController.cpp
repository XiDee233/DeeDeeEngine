#include "deepch.h"
#include "OrthographicCameraController.h"
#include "DeeDeeEngine\Core\keyCode.h"
#include "DeeDeeEngine/Core\input.h"
namespace DeeDeeEngine {
	DeeDeeEngine::OrthographicCameraController::OrthographicCameraController(float aspectRatio,bool rotation)
		:m_AspectRatio(aspectRatio),m_Camera(-m_AspectRatio*m_ZoomLevel,m_AspectRatio* m_ZoomLevel,-m_ZoomLevel,m_ZoomLevel),m_Rotation(rotation)
	{
	}

	void OrthographicCameraController::OnUpdate(Timestep ts)
	{
		// 获取相机的旋转角度（假设你的相机旋转是绕Z轴进行的）
		float radians = glm::radians(m_CameraRotation);

		// 计算相机的前向向量和右向量
		glm::vec3 cameraRight = glm::vec3(cos(radians), sin(radians), 0.0f);
		glm::vec3 cameraForward = glm::vec3(-sin(radians), cos(radians), 0.0f);

		// 处理按键输入
		if (Input::IsKeyPressed(DEE_KEY_LEFT)) {
			m_CameraPosition -= cameraRight * (m_CameraSpeed * ts);
		}
		else if (Input::IsKeyPressed(DEE_KEY_RIGHT)) {
			m_CameraPosition += cameraRight * (m_CameraSpeed * ts);
		}

		if (Input::IsKeyPressed(DEE_KEY_DOWN)) {
			m_CameraPosition -= cameraForward * (m_CameraSpeed * ts);
		}
		else if (Input::IsKeyPressed(DEE_KEY_UP)) {
			m_CameraPosition += cameraForward * (m_CameraSpeed * ts);
		}
		if (m_Rotation)
		{
			if (Input::IsKeyPressed(DEE_KEY_Q)) {
				m_CameraRotation += m_CameraRotateSpeed * ts*10;
			}
			else if (Input::IsKeyPressed(DEE_KEY_E)) {
				m_CameraRotation -= m_CameraRotateSpeed * ts*10;
			}
			m_Camera.SetRotation(m_CameraRotation);
		}

		m_Camera.SetPosition(m_CameraPosition);
		m_CameraSpeed = m_ZoomLevel;
	}
	void OrthographicCameraController::OnEvent(Event& e)
	{
		EventDispatcher dispatcher(e);
		dispatcher.Dispatch<MouseScrolledEvent>(DEE_BIND_EVENT_FN(OrthographicCameraController::OnMouseScrolled));
		dispatcher.Dispatch<WindowResizeEvent>(DEE_BIND_EVENT_FN(OrthographicCameraController::OnWindowResized));
	}
	bool OrthographicCameraController::OnMouseScrolled(MouseScrolledEvent& e)
	{
		m_ZoomLevel -= e.GetYOffset()*0.1f;
		m_ZoomLevel = std::max(m_ZoomLevel, 0.1f);
		m_Camera.SetProjection(-m_AspectRatio * m_ZoomLevel, m_AspectRatio * m_ZoomLevel, -m_ZoomLevel, m_ZoomLevel);
		return false;
	}
	bool OrthographicCameraController::OnWindowResized(WindowResizeEvent& e)
	{
		m_AspectRatio = (float)e.GetWidth() / (float)e.GetHeight();
		m_Camera.SetProjection(-m_AspectRatio * m_ZoomLevel, m_AspectRatio * m_ZoomLevel, -m_ZoomLevel, m_ZoomLevel);
		return false;
	}
}
#include "deepch.h"
#include "OrthographicCamera.h"
#include <glm/gtc/matrix_transform.hpp>
namespace DeeDeeEngine {

	DeeDeeEngine::OrthographicCamera::OrthographicCamera(float left, float right, float bottom, float top)
		:m_ProjectionMatrix(glm::ortho(left,right,bottom,top,-1.0f,1.0f)),m_ViewMatrix(1.0f)
	{
		//glm::ortho 函数用于创建一个正交投影矩阵，它接受左、右、底部、顶部、近平面和远平面作为参数。
		// 这些参数定义了一个平行于视口的视景体（viewing volume），视景体内的物体将被投影到裁剪空间（clip space）中。
		m_ViewProjectionMartix = m_ProjectionMatrix * m_ViewMatrix;
	}

	void OrthographicCamera::SetProjection(float left, float right, float bottom, float top)
	{
		m_ProjectionMatrix = glm::ortho(left, right, bottom, top, -1.0f, 1.0f);
		m_ViewProjectionMartix = m_ProjectionMatrix * m_ViewMatrix;
	}

	void OrthographicCamera::RecalculateViewMatrix() {

		glm::mat4 transform = glm::translate(glm::mat4(1.0f), m_Position) *
			glm::rotate(glm::mat4(1.0f),glm::radians( m_Rotation), glm::vec3(0,0, 1));
	
		m_ViewMatrix = glm::inverse(transform);

		m_ViewProjectionMartix = m_ProjectionMatrix * m_ViewMatrix;
	}

}
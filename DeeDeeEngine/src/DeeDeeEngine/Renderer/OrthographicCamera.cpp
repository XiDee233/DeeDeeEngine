#include "deepch.h"
#include "OrthographicCamera.h"
#include <glm/gtc/matrix_transform.hpp>
namespace DeeDeeEngine {

	DeeDeeEngine::OrthographicCamera::OrthographicCamera(float left, float right, float bottom, float top)
		:m_ProjectionMatrix(glm::ortho(left,right,bottom,top,-1.0f,1.0f)),m_ViewMatrix(1.0f)
	{
		//glm::ortho �������ڴ���һ������ͶӰ�������������ҡ��ײ�����������ƽ���Զƽ����Ϊ������
		// ��Щ����������һ��ƽ�����ӿڵ��Ӿ��壨viewing volume�����Ӿ����ڵ����彫��ͶӰ���ü��ռ䣨clip space���С�
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
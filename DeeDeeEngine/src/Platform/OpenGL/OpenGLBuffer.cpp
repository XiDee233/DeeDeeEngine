#include "deepch.h"
#include "OpenGLBuffer.h"
#include <glad/glad.h>

namespace DeeDeeEngine {

	 OpenGLIndexBuffer::OpenGLIndexBuffer(uint32_t* indices, uint32_t count) :m_Count(count)
	{
		 DEE_PROFILE_FUNCTION();

		glCreateBuffers(1, &m_RendererID);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_RendererID);

		glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(uint32_t), indices, GL_STATIC_DRAW);
	}

	OpenGLIndexBuffer::~OpenGLIndexBuffer()
	{
		DEE_PROFILE_FUNCTION();

		glDeleteBuffers(1, &m_RendererID);
	}

	void OpenGLIndexBuffer::Bind() const
	{
		DEE_PROFILE_FUNCTION();

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_RendererID);
	}

	void OpenGLIndexBuffer::Unbind() const
	{
		DEE_PROFILE_FUNCTION();

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	}

	OpenGLVertexBuffer::OpenGLVertexBuffer(float* vertices, uint32_t size) {
		DEE_PROFILE_FUNCTION(); 

		glCreateBuffers(1, &m_RendererID); // ����һ���µĻ���������
		glBindBuffer(GL_ARRAY_BUFFER, m_RendererID); // ���´����Ļ������󶨵�GL_ARRAY_BUFFERĿ�ꡣ
		glBufferData(GL_ARRAY_BUFFER, size, vertices, GL_STATIC_DRAW); // Ϊ�������ṩ���ݣ�ָ��Ϊ��̬������;��
	}

	OpenGLVertexBuffer::OpenGLVertexBuffer(uint32_t size)
	{
		glCreateBuffers(1, &m_RendererID); // ����һ���µĻ���������
		glBindBuffer(GL_ARRAY_BUFFER, m_RendererID); // ���´����Ļ������󶨵�GL_ARRAY_BUFFERĿ�ꡣ
		glBufferData(GL_ARRAY_BUFFER, size, nullptr, GL_DYNAMIC_DRAW); // �����ڴ棬����������ݣ�ָ��Ϊ��̬������;��
	}

	OpenGLVertexBuffer::~OpenGLVertexBuffer() {
		DEE_PROFILE_FUNCTION();

		glDeleteBuffers(1, &m_RendererID);
	}
	void OpenGLVertexBuffer::Bind() const
	{
		DEE_PROFILE_FUNCTION();

		glBindBuffer(GL_ARRAY_BUFFER, m_RendererID);
	}

	void OpenGLVertexBuffer::Unbind() const
	{
		DEE_PROFILE_FUNCTION();

		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}

	void OpenGLVertexBuffer::SetData(const void* data, uint32_t size)
	{
		glBindBuffer(GL_ARRAY_BUFFER, m_RendererID);
		glBufferSubData(GL_ARRAY_BUFFER, 0,size,data);
	}


}

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

		glCreateBuffers(1, &m_RendererID); // 创建一个新的缓冲区对象。
		glBindBuffer(GL_ARRAY_BUFFER, m_RendererID); // 将新创建的缓冲区绑定到GL_ARRAY_BUFFER目标。
		glBufferData(GL_ARRAY_BUFFER, size, vertices, GL_STATIC_DRAW); // 为缓冲区提供数据，指定为静态绘制用途。
	}

	OpenGLVertexBuffer::OpenGLVertexBuffer(uint32_t size)
	{
		glCreateBuffers(1, &m_RendererID); // 创建一个新的缓冲区对象。
		glBindBuffer(GL_ARRAY_BUFFER, m_RendererID); // 将新创建的缓冲区绑定到GL_ARRAY_BUFFER目标。
		glBufferData(GL_ARRAY_BUFFER, size, nullptr, GL_DYNAMIC_DRAW); // 分配内存，但不填充数据，指定为动态绘制用途。
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

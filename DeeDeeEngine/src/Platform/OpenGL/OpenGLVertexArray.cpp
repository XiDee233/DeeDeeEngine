#include "deepch.h"
#include "OpenGLVertexArray.h"
#include <glad/glad.h>
namespace DeeDeeEngine {
	static GLenum ShaderDataTypeToOpenGLBaseType(ShaderDataType type) {
		switch (type) {
		case DeeDeeEngine::ShaderDataType::Float: return GL_FLOAT;
		case DeeDeeEngine::ShaderDataType::Float2: return GL_FLOAT;
		case DeeDeeEngine::ShaderDataType::Float3: return GL_FLOAT;
		case DeeDeeEngine::ShaderDataType::Float4: return GL_FLOAT;
		case DeeDeeEngine::ShaderDataType::Mat3: return GL_FLOAT;
		case DeeDeeEngine::ShaderDataType::Mat4: return GL_FLOAT;
		case DeeDeeEngine::ShaderDataType::Int: return GL_INT;
		case DeeDeeEngine::ShaderDataType::Int2: return GL_INT;
		case DeeDeeEngine::ShaderDataType::Int3: return GL_INT;
		case DeeDeeEngine::ShaderDataType::Int4: return GL_INT;
		case DeeDeeEngine::ShaderDataType::Bool: return GL_BOOL;
		}
		DEE_CORE_ASSERT(false, "Unknown ShaderDataType!");
		return 0;
	}

	OpenGLVertexArray::OpenGLVertexArray() {
		glCreateVertexArrays(1, &m_RendererID);
	}
	OpenGLVertexArray::~OpenGLVertexArray()
	{
		glDeleteVertexArrays(1, &m_RendererID);
	}
	void DeeDeeEngine::OpenGLVertexArray::Bind() const
	{
		glBindVertexArray(m_RendererID);
	}
	void OpenGLVertexArray::Unbind() const {
		glBindVertexArray(0);
	}
	void OpenGLVertexArray::AddVertexBuffer(const Ref<VertexBuffer>& vertexBuffer) {
		glBindVertexArray(m_RendererID);
		DEE_CORE_ASSERT(vertexBuffer->GetLayout().GetElements().size(), "Vertex Buffer has no layout!");
			vertexBuffer->Bind();
		uint32_t index = 0;
		const auto& layout = vertexBuffer->GetLayout();
		for (const auto& element : layout) {
			glEnableVertexAttribArray(index);
			glVertexAttribPointer(index, element.GetComponentCount(),
				ShaderDataTypeToOpenGLBaseType(element.Type),
				element.Normalized ? GL_TRUE : GL_FALSE,
				layout.GetStride(),
				(const void*)element.Offset);
			index++;
		}
		m_VertexBuffers.push_back(vertexBuffer);
	}
	void OpenGLVertexArray::SetIndexBuffer(const Ref<IndexBuffer>& indexBuffer) {
		glBindVertexArray(m_RendererID);
		indexBuffer->Bind();

		m_IndexBuffer = indexBuffer;
	}
}
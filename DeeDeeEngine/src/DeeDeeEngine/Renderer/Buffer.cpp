#include "deepch.h"
#include "Buffer.h"
#include "Renderer.h"
#include <DeeDeeEngine/Core.h>
#include "Platform/OpenGL/OpenGLBuffer.h"

namespace DeeDeeEngine {
	
	IndexBuffer* IndexBuffer::Create(uint32_t* indices, uint32_t count) {
		switch (Renderer::GetAPI())
		{
		case RendererAPI::None:
			DEE_CORE_ASSERT(false, "RendererAPI::None is currently not supported!");
			return nullptr;
		case RendererAPI::OpenGL:
			return new OpenGLIndexBuffer(indices, count);

		}
		DEE_CORE_ASSERT(false, "Unknown RendererAPI!");
		return nullptr;
	}
	VertexBuffer* VertexBuffer::Create(float* vertices, uint32_t size) {
		switch (Renderer::GetAPI())
		{
		case RendererAPI::None:
			DEE_CORE_ASSERT(false, "RendererAPI::None is currently not supported!");
			return nullptr;
		case RendererAPI::OpenGL:
			return new OpenGLVertexBuffer(vertices, size);

		}
		DEE_CORE_ASSERT(false, "Unknown RendererAPI!");
		return nullptr;
	}
}
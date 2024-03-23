#include "deepch.h"
#include "VertexArray.h"
#include "Renderer.h"
#include "Platform\OpenGL\OpenGLVertexArray.h"
namespace DeeDeeEngine {

	VertexArray* DeeDeeEngine::VertexArray::Create()
	{
		switch (Renderer::GetAPI())
		{
		case RendererAPI::None:
			DEE_CORE_ASSERT(false, "RendererAPI::None is currently not supported!");
			return nullptr;
		case RendererAPI::OpenGL:
			return new OpenGLVertexArray();

		}
		DEE_CORE_ASSERT(false, "Unknown RendererAPI!");
		return nullptr;
	}


}
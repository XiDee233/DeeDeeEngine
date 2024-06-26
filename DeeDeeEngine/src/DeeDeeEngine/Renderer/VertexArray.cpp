#include "deepch.h"
#include "VertexArray.h"
#include "Renderer.h"
#include "Platform\OpenGL\OpenGLVertexArray.h"
namespace DeeDeeEngine {

	Ref<VertexArray> DeeDeeEngine::VertexArray::Create()
	{
		switch (Renderer::GetAPI())
		{
		case RendererAPI::API::None:
			DEE_CORE_ASSERT(false, "RendererAPI::None is currently not supported!");
			return nullptr;
		case RendererAPI::API::OpenGL:
			return  std::make_shared<OpenGLVertexArray>();

		}
		DEE_CORE_ASSERT(false, "Unknown RendererAPI!");
		return nullptr;
	}


}
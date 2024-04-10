#include "deepch.h"
#include "Texture.h"
#include "Renderer.h"
#include "Platform\OpenGL\OpenGLTexture.h"
namespace DeeDeeEngine {
	Ref<Texture2D> Texture2D::Create(const std::string& path)
	{
		switch (Renderer::GetAPI()) {
		case RendererAPI::API::None:
			DEE_CORE_ASSERT(false, "RendererAPI::None is currently not supported!");
		case RendererAPI::API::OpenGL:
			return std::make_shared<OpenGLTexture2D>(path);
		}
		DEE_CORE_ASSERT(false, "Unknown RendererAPI!");
		return nullptr;
	}
}
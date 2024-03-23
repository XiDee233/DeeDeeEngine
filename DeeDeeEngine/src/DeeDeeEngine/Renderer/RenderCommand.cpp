#include "deepch.h"
#include "RenderCommand.h"
#include "Platform\OpenGL\OpenGLRendererAPI.h"

namespace DeeDeeEngine {
	RendererAPI* RenderCommand::s_RendererAPI = new OpenGLRendererAPI;
}
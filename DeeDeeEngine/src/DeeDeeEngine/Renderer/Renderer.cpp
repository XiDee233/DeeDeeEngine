#include "deepch.h"
#include "Renderer.h"
#include <Platform\OpenGL\OpenGLShader.h>

#include "Renderer2D.h"

namespace DeeDeeEngine {

	Renderer::SceneData* Renderer::m_SceneData = new Renderer::SceneData;
	void Renderer::Init()
	{
		RenderCommand::Init();
		Renderer2D::Init();
	}
	void Renderer::OnWindowResize(uint32_t width,uint32_t height)
	{
	RenderCommand::SetViewport(0, 0, width, height);
	}
	void Renderer::BeginScene(OrthographicCamera& camera)
	{
		m_SceneData->ViewProjectionMatrix = camera.GetViewProjectionMartix();
	}
	void Renderer::EndScene()
	{
	}
	// Renderer类的Submit方法，用于提交渲染命令
	void Renderer::Submit(const Ref<Shader>& shader, const Ref<VertexArray>& vertexArray, const glm::mat4 transform)
	{
		shader->Bind(); // 绑定着色器，以便将其用于接下来的渲染操作

		// 将着色器强制转换为OpenGLShader类型，并上传视图投影矩阵
		std::dynamic_pointer_cast<OpenGLShader>(shader)->UploadUniformMat4("u_ViewProjection", m_SceneData->ViewProjectionMatrix);

		// 上传变换矩阵，这将决定物体在世界空间中的位置和方向
		std::dynamic_pointer_cast<OpenGLShader>(shader)->UploadUniformMat4("u_Transform", transform);

		vertexArray->Bind(); // 绑定顶点数组，以便OpenGL知道使用哪个数据进行渲染

		RenderCommand::DrawIndexed(vertexArray); // 执行绘制命令，根据索引来渲染顶点数组
	}
}

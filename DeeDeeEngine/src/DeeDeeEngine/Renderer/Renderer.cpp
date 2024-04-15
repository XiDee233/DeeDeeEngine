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
	// Renderer���Submit�����������ύ��Ⱦ����
	void Renderer::Submit(const Ref<Shader>& shader, const Ref<VertexArray>& vertexArray, const glm::mat4 transform)
	{
		shader->Bind(); // ����ɫ�����Ա㽫�����ڽ���������Ⱦ����

		// ����ɫ��ǿ��ת��ΪOpenGLShader���ͣ����ϴ���ͼͶӰ����
		std::dynamic_pointer_cast<OpenGLShader>(shader)->UploadUniformMat4("u_ViewProjection", m_SceneData->ViewProjectionMatrix);

		// �ϴ��任�����⽫��������������ռ��е�λ�úͷ���
		std::dynamic_pointer_cast<OpenGLShader>(shader)->UploadUniformMat4("u_Transform", transform);

		vertexArray->Bind(); // �󶨶������飬�Ա�OpenGL֪��ʹ���ĸ����ݽ�����Ⱦ

		RenderCommand::DrawIndexed(vertexArray); // ִ�л������������������Ⱦ��������
	}
}

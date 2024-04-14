#include "deepch.h"
#include "Renderer2D.h"
#include "VertexArray.h"
#include "Shader.h"
#include <Platform\OpenGL\OpenGLShader.h>
#include <glm/gtc/matrix_transform.hpp>

namespace DeeDeeEngine {

	struct Renderer2DStorage {
		Ref<VertexArray> QuadVertexArray;
		Ref<Shader> FlatColorShader;
	};
	static Renderer2DStorage* m_Data;
	void Renderer2D::Init()
	{
		m_Data = new Renderer2DStorage();
		m_Data->QuadVertexArray = (VertexArray::Create());


		float vertices2[5 * 4] = {
			-0.5f, -0.5f, 0.0f,
			 0.5f, -0.5f, 0.0f,
			 0.5f,  0.5f, 0.0f,
			-0.5f,  0.5f, 0.0f

		};
		Ref<VertexBuffer> squareVB;
		squareVB.reset(VertexBuffer::Create(vertices2, sizeof(vertices2)));
		BufferLayout _layout2 = {
			{ShaderDataType::Float3,"a_Position"},
		};
		squareVB->SetLayout(_layout2);

		m_Data->QuadVertexArray->AddVertexBuffer(squareVB);

		uint32_t indices2[6] = { 0,1,2,2,3,0 };
		Ref<IndexBuffer> squareIB;
		squareIB.reset(IndexBuffer::Create(indices2, sizeof(indices2) / sizeof(uint32_t)));
		m_Data->QuadVertexArray->SetIndexBuffer(squareIB);


		m_Data->FlatColorShader = Shader::Create("assets/shaders/FlatColor.glsl");
	}
	void Renderer2D::Shutdown()
	{
		delete m_Data;
	}
	void Renderer2D::BeginScene(const OrthographicCamera& camera)
	{
		m_Data->FlatColorShader->Bind();
		m_Data->FlatColorShader->SetMat4("u_ViewProjection",camera.GetViewProjectionMartix());

	}
	void Renderer2D::EndScene()
	{
	}
	void Renderer2D::DrawQuad(const glm::vec3& position, const glm::vec2& size, const glm::vec4& color)
	{

		m_Data->FlatColorShader->Bind();
		m_Data->FlatColorShader->SetFloat4("u_Color", color);

		glm::mat4 transform = glm::translate(glm::mat4(1.0f), position) * glm::scale(glm::mat4(1.0f), { size.x,size.y,1.0f });
		m_Data->FlatColorShader->SetMat4("u_Transform", transform);


		m_Data->QuadVertexArray->Bind();
		RenderCommand::DrawIndexed(m_Data->QuadVertexArray);
	}
	void Renderer2D::DrawQuad(const glm::vec2& position, const glm::vec2& size, const glm::vec4& color)
	{
		DrawQuad({ position.x,position.y,0.0f }, size, color);
	}
}

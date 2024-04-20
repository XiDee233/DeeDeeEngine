#include "deepch.h"
#include "Renderer2D.h"
#include "VertexArray.h"
#include "Shader.h"
#include <Platform\OpenGL\OpenGLShader.h>
#include <glm/gtc/matrix_transform.hpp>

namespace DeeDeeEngine {

	struct QuadVertex {
		glm::vec3 Position;
		glm::vec4 Color;
		glm::vec2 TexCoord;
	};
	struct Renderer2DData {

		const uint32_t MaxQuads = 10000;
		const uint32_t MaxVertices = MaxQuads * 4;
		const uint32_t MaxIndices = MaxQuads * 6;

		Ref<VertexArray> QuadVertexArray;
		Ref<VertexBuffer> QuadVertexBuffer;
		Ref<Shader> TextureShader;
		Ref<Texture> WhiteTexture;

		uint32_t QuadIndexCount = 0;
		QuadVertex* QuadVertexBufferBase = nullptr;
		QuadVertex* QuadVertexBufferPtr = nullptr;
	};
	static Renderer2DData m_Data;

	
	void Renderer2D::Init()
	{
		DEE_PROFILE_FUNCTION();

		m_Data.QuadVertexArray = (VertexArray::Create());

		m_Data.QuadVertexBuffer = VertexBuffer::Create(m_Data.MaxVertices*sizeof(QuadVertex));
		BufferLayout _layout2 = {
			{ShaderDataType::Float3,"a_Position"},
			{ShaderDataType::Float4,"a_Color"},
			{ShaderDataType::Float2,"a_TexCoord"},

		};
		m_Data.QuadVertexBuffer->SetLayout(_layout2);

		m_Data.QuadVertexArray->AddVertexBuffer(m_Data.QuadVertexBuffer);

		m_Data.QuadVertexBufferBase = new QuadVertex[m_Data.MaxVertices];

		uint32_t* quadIndices = new uint32_t[m_Data.MaxIndices];

		uint32_t offset = 0;
		for (uint32_t i = 0; i < m_Data.MaxIndices; i += 6)
		{
			quadIndices[i + 0] = offset + 0;
			quadIndices[i + 1] = offset + 1;
			quadIndices[i + 2] = offset + 2;

			quadIndices[i + 3] = offset + 2;
			quadIndices[i + 4] = offset + 3;
			quadIndices[i + 5] = offset + 0;

			offset += 4;
		}
		Ref<IndexBuffer> quadIB;
		quadIB.reset(IndexBuffer::Create(quadIndices, m_Data.MaxIndices));
		m_Data.QuadVertexArray->SetIndexBuffer(quadIB);
		delete[] quadIndices;


		m_Data.WhiteTexture = Texture2D::Create(1, 1);
		uint32_t whiteTextureData = 0xffffffff;
		m_Data.WhiteTexture->SetData(&whiteTextureData, sizeof(uint32_t));

		m_Data.TextureShader = Shader::Create("assets/shaders/Texture.glsl");
		m_Data.TextureShader->Bind();
		m_Data.TextureShader->SetInt("u_Texture", 0);
	}
	void Renderer2D::Shutdown()
	{
		DEE_PROFILE_FUNCTION();

	}
	void Renderer2D::BeginScene(const OrthographicCamera& camera)
	{
		DEE_PROFILE_FUNCTION();

		m_Data.TextureShader->Bind();
		m_Data.TextureShader->SetMat4("u_ViewProjection", camera.GetViewProjectionMartix());

		m_Data.QuadIndexCount = 0;
		m_Data.QuadVertexBufferPtr = m_Data.QuadVertexBufferBase;
	}
	void Renderer2D::EndScene()
	{
		DEE_PROFILE_FUNCTION();
		uint32_t dataSize = (uint8_t*)m_Data.QuadVertexBufferPtr - (uint8_t*)m_Data.QuadVertexBufferBase;
		m_Data.QuadVertexBuffer->SetData(m_Data.QuadVertexBufferBase,dataSize);
		Flush();
	}
	void Renderer2D::Flush()
	{
		RenderCommand::DrawIndexed(m_Data.QuadVertexArray,m_Data.QuadIndexCount);
	}
	void Renderer2D::DrawQuad(const glm::vec3& position, const glm::vec2& size, const glm::vec4& color)
	{
		DEE_PROFILE_FUNCTION();

		m_Data.QuadVertexBufferPtr->Position = position;
		m_Data.QuadVertexBufferPtr->Color = color;
		m_Data.QuadVertexBufferPtr->TexCoord = {0.0f,0.0f};
		m_Data.QuadVertexBufferPtr++;

		m_Data.QuadVertexBufferPtr->Position = { position.x + size.x,position.y,0.0f };
		m_Data.QuadVertexBufferPtr->Color = color;
		m_Data.QuadVertexBufferPtr->TexCoord = { 0.0f,0.0f };
		m_Data.QuadVertexBufferPtr++;
		
		m_Data.QuadVertexBufferPtr->Position = { position.x + size.x,position.y+size.y,0.0f };
		m_Data.QuadVertexBufferPtr->Color = color;
		m_Data.QuadVertexBufferPtr->TexCoord = { 1.0f,0.0f };
		m_Data.QuadVertexBufferPtr++;
		
		m_Data.QuadVertexBufferPtr->Position = { position.x ,position.y + size.y,0.0f };
		m_Data.QuadVertexBufferPtr->Color = color;
		m_Data.QuadVertexBufferPtr->TexCoord = { 0.0f,1.0f };
		m_Data.QuadVertexBufferPtr++;

		m_Data.QuadIndexCount += 6;
		/*m_Data.TextureShader->SetFloat4("u_Color", color);
		m_Data.WhiteTexture->Bind();
		glm::mat4 transform = glm::translate(glm::mat4(1.0f), position) * glm::scale(glm::mat4(1.0f), { size.x,size.y,1.0f });
		m_Data.TextureShader->SetMat4("u_Transform", transform);


		m_Data.QuadVertexArray->Bind();
		RenderCommand::DrawIndexed(m_Data.QuadVertexArray);*/
	}

	void Renderer2D::DrawQuad(const glm::vec2& position, const glm::vec2& size, const glm::vec4& color)
	{
		DEE_PROFILE_FUNCTION();

		DrawQuad({ position.x,position.y,0.0f }, size, color);
	}

	void Renderer2D::DrawQuad(const glm::vec2& position, const glm::vec2& size, const Ref<Texture2D> texture,const float tilingFactor,const glm::vec4& tintColor)
	{
		DEE_PROFILE_FUNCTION();

		DrawQuad({ position.x,position.y,0.0f }, size, texture,tilingFactor,tintColor);
	}
	void Renderer2D::DrawQuad(const glm::vec3& position, const glm::vec2& size, const Ref<Texture2D> texture, const float tilingFactor,const glm::vec4& tintColor)
	{
		DEE_PROFILE_FUNCTION();

		m_Data.TextureShader->SetFloat4("u_Color", tintColor);
		m_Data.TextureShader->SetFloat("u_TilingFactor", tilingFactor);
		texture->Bind();
		glm::mat4 transform = glm::translate(glm::mat4(1.0f), position)
			* glm::scale(glm::mat4(1.0f), { size.x,size.y,1.0f });
		m_Data.TextureShader->SetMat4("u_Transform", transform);

		
		m_Data.QuadVertexArray->Bind();
		RenderCommand::DrawIndexed(m_Data.QuadVertexArray);
	}
	void Renderer2D::DrawRotateQuad(const glm::vec2& position, const glm::vec2& size, float rotation, const glm::vec4& color)
	{
		DrawRotateQuad({ position.x,position.y,0.0f }, size,rotation,color);
	}
	void Renderer2D::DrawRotateQuad(const glm::vec3& position, const glm::vec2& size, float rotation, const glm::vec4& color)
	{
		DEE_PROFILE_FUNCTION();

		m_Data.TextureShader->SetFloat4("u_Color", color);
		m_Data.WhiteTexture->Bind();
		glm::mat4 transform = glm::translate(glm::mat4(1.0f), position) 
			* glm::rotate(glm::mat4(1.0f),rotation, { 0.0f,0.0f,1.0f })
			* glm::scale(glm::mat4(1.0f), { size.x,size.y,1.0f });
		m_Data.TextureShader->SetMat4("u_Transform", transform);


		m_Data.QuadVertexArray->Bind();
		RenderCommand::DrawIndexed(m_Data.QuadVertexArray);
	}
	void Renderer2D::DrawRotateQuad(const glm::vec2& position, const glm::vec2& size, float rotation, const Ref<Texture2D> texture, const float tilingFactor, glm::vec4& tintColor)
	{
		DrawRotateQuad({ position.x,position.y,0.0f }, size, rotation, texture,tilingFactor);

	}
	void Renderer2D::DrawRotateQuad(const glm::vec3& position, const glm::vec2& size, float rotation, const Ref<Texture2D> texture, const float tilingFactor, glm::vec4& tintColor)
	{

		DEE_PROFILE_FUNCTION();

		m_Data.TextureShader->SetFloat4("u_Color", tintColor);
		m_Data.TextureShader->SetFloat("u_TilingFactor", tilingFactor);
		texture->Bind();
		glm::mat4 transform = glm::translate(glm::mat4(1.0f), position)
			* glm::rotate(glm::mat4(1.0f), rotation, { 0.0f,0.0f,1.0f })
			* glm::scale(glm::mat4(1.0f), { size.x,size.y,1.0f });
		m_Data.TextureShader->SetMat4("u_Transform", transform);


		m_Data.QuadVertexArray->Bind();
		RenderCommand::DrawIndexed(m_Data.QuadVertexArray);
	}
}

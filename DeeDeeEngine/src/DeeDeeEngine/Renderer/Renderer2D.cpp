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
		float TexIndex;
		float TilingFactor;
	};
	struct Renderer2DData {

		static const uint32_t MaxQuads = 100;
		static const uint32_t MaxVertices = MaxQuads * 4;
		static const uint32_t MaxIndices = MaxQuads * 6;
		static const uint32_t MaxTextureSlots = 32;

		Ref<VertexArray> QuadVertexArray;
		Ref<VertexBuffer> QuadVertexBuffer;
		Ref<Shader> TextureShader;
		Ref<Texture2D> WhiteTexture;

		uint32_t QuadIndexCount = 0;
		QuadVertex* QuadVertexBufferBase = nullptr;
		QuadVertex* QuadVertexBufferPtr = nullptr;
		std::array<Ref<Texture2D>, MaxTextureSlots> TextureSlots;
		uint32_t TextureSlotIndex = 1;

		glm::vec4 QuadVertexPositions[4];

		Renderer2D::Statistics Stats;
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
			{ShaderDataType::Float,"a_TexIndex"},
			{ShaderDataType::Float,"a_TilingFactor"},


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

		int32_t samplers[m_Data.MaxTextureSlots];
		for (uint32_t i = 0; i < m_Data.MaxTextureSlots; i++)
			samplers[i] = i;

		m_Data.TextureShader = Shader::Create("assets/shaders/Texture.glsl");
		m_Data.TextureShader->Bind();
		m_Data.TextureShader->SetIntArray("u_Textures", samplers,m_Data.MaxTextureSlots);
		//�� m_Data.TextureSlots ���������Ԫ������Ϊ 0
		m_Data.TextureSlots[0] = m_Data.WhiteTexture;
		m_Data.QuadVertexPositions[0] = { -0.5f, -0.5f, 0.0f, 1.0f };
		m_Data.QuadVertexPositions[1] = { 0.5f, -0.5f, 0.0f, 1.0f };
		m_Data.QuadVertexPositions[2] = { 0.5f,  0.5f, 0.0f, 1.0f };
		m_Data.QuadVertexPositions[3] = { -0.5f,  0.5f, 0.0f, 1.0f };
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

		m_Data.TextureSlotIndex = 1;
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

		for (uint32_t i = 0; i < m_Data.TextureSlotIndex; i++) {
			m_Data.TextureSlots[i]->Bind(i);
		}
		RenderCommand::DrawIndexed(m_Data.QuadVertexArray,m_Data.QuadIndexCount);
		m_Data.Stats.DrawCalls++;
	}

	 void Renderer2D::FlushAndReset() {
		 EndScene();
		m_Data.QuadIndexCount = 0;
		m_Data.QuadVertexBufferPtr = m_Data.QuadVertexBufferBase;

		m_Data.TextureSlotIndex = 1;
	}
	void Renderer2D::DrawQuad(const glm::vec3& position, const glm::vec2& size, const glm::vec4& color)
	{
		DEE_PROFILE_FUNCTION();
		if (m_Data.QuadIndexCount >= Renderer2DData::MaxIndices) {
			FlushAndReset();
		}
		constexpr size_t quadVertexCount = 4;
		constexpr glm::vec2 textureCoords[] = {
			{ 0.0f,0.0f },
			{ 1.0f,0.0f },
			{ 1.0f,1.0f },
			{ 0.0f,1.0f }
		};
		const float textureIndex = 0.0f;
		const float tilingFactor = 1.0f;

		glm::mat4 transform = glm::translate(glm::mat4(1.0f), position)
			* glm::scale(glm::mat4(1.0f), { size.x, size.y, 1.0f });
		for (size_t i = 0; i < quadVertexCount; i++) {
			m_Data.QuadVertexBufferPtr->Position = transform * m_Data.QuadVertexPositions[i];
			m_Data.QuadVertexBufferPtr->Color = color;
			m_Data.QuadVertexBufferPtr->TexCoord = textureCoords[i];
			m_Data.QuadVertexBufferPtr->TexIndex = textureIndex;
			m_Data.QuadVertexBufferPtr->TilingFactor = tilingFactor;
			m_Data.QuadVertexBufferPtr++;
		}
	

		m_Data.QuadIndexCount += 6;
		m_Data.Stats.QuadCount++;

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
		constexpr glm::vec4 color = { 1.0f,1.0f,1.0f,1.0f };
		float textureIndex = 0.0f;
		constexpr size_t quadVertexCount = 4;
		constexpr glm::vec2 textureCoords[] = {
			{ 0.0f,0.0f },
			{ 1.0f,0.0f },
			{ 1.0f,1.0f },
			{ 0.0f,1.0f }
		};

		if (m_Data.QuadIndexCount >= Renderer2DData::MaxIndices) {
			FlushAndReset();
		}
	

		for (uint32_t i = 1; i < m_Data.TextureSlotIndex; i++)
		{
			if (*m_Data.TextureSlots[i].get() == *texture.get())
			{
				textureIndex = (float)i;
				break;
			}
		}
		if (textureIndex == 0.0f) {
			textureIndex = (float)m_Data.TextureSlotIndex;
			m_Data.TextureSlots[m_Data.TextureSlotIndex] = texture;
			m_Data.TextureSlotIndex++;
		}
		glm::mat4 transform = glm::translate(glm::mat4(1.0f), position)
			* glm::scale(glm::mat4(1.0f), { size.x, size.y, 1.0f });

		for (size_t i = 0; i < quadVertexCount; i++) {
			m_Data.QuadVertexBufferPtr->Position = transform * m_Data.QuadVertexPositions[i];
			m_Data.QuadVertexBufferPtr->Color = color;
			m_Data.QuadVertexBufferPtr->TexCoord = textureCoords[i];
			m_Data.QuadVertexBufferPtr->TexIndex = textureIndex;
			m_Data.QuadVertexBufferPtr->TilingFactor = tilingFactor;
			m_Data.QuadVertexBufferPtr++;
		}


		m_Data.QuadIndexCount += 6;
		m_Data.Stats.QuadCount++;

	}
	void Renderer2D::DrawQuad(const glm::vec2& position, const glm::vec2& size, const Ref<SubTexture2D> subTexture, const float tilingFactor, const glm::vec4& tintColor)
	{
		DEE_PROFILE_FUNCTION();

		DrawQuad({ position.x,position.y,0.0f }, size, subTexture, tilingFactor, tintColor);
	}
	void Renderer2D::DrawQuad(const glm::vec3& position, const glm::vec2& size, const Ref<SubTexture2D> subTexture, const float tilingFactor, const glm::vec4& tintColor)
	{
		DEE_PROFILE_FUNCTION();

		if (m_Data.QuadIndexCount >= Renderer2DData::MaxIndices) {
			FlushAndReset();
		}
		constexpr glm::vec4 color = { 1.0f,1.0f,1.0f,1.0f };
		float textureIndex = 0.0f;
		constexpr size_t quadVertexCount = 4;
		const glm::vec2* textureCoords = subTexture->GetTexCoords();
		const Ref<Texture2D> texture = subTexture->GetTexture();

		for (uint32_t i = 1; i < m_Data.TextureSlotIndex; i++)
		{
			if (*m_Data.TextureSlots[i].get() == *texture.get())
			{
				textureIndex = (float)i;
				break;
			}
		}
		if (textureIndex == 0.0f) {
			textureIndex = (float)m_Data.TextureSlotIndex;
			m_Data.TextureSlots[m_Data.TextureSlotIndex] = texture;
			m_Data.TextureSlotIndex++;
		}

		glm::mat4 transform = glm::translate(glm::mat4(1.0f), position)
			* glm::scale(glm::mat4(1.0f), { size.x, size.y, 1.0f });
		for (size_t i = 0; i < quadVertexCount; i++) {
			m_Data.QuadVertexBufferPtr->Position = transform * m_Data.QuadVertexPositions[i];
			m_Data.QuadVertexBufferPtr->Color = color;
			m_Data.QuadVertexBufferPtr->TexCoord = textureCoords[i];
			m_Data.QuadVertexBufferPtr->TexIndex = textureIndex;
			m_Data.QuadVertexBufferPtr->TilingFactor = tilingFactor;
			m_Data.QuadVertexBufferPtr++;
		}



		m_Data.QuadIndexCount += 6;
		m_Data.Stats.QuadCount++;
	}
	void Renderer2D::DrawRotateQuad(const glm::vec2& position, const glm::vec2& size, float rotation, const glm::vec4& color)
	{
		DrawRotateQuad({ position.x,position.y,0.0f }, size,rotation,color);
	}
	void Renderer2D::DrawRotateQuad(const glm::vec3& position, const glm::vec2& size, float rotation, const glm::vec4& color)
	{
		DEE_PROFILE_FUNCTION();

		if (m_Data.QuadIndexCount >= Renderer2DData::MaxIndices) {
			FlushAndReset();
		}
		constexpr size_t quadVertexCount = 4;
		constexpr glm::vec2 textureCoords[] = {
			{ 0.0f,0.0f },
			{ 1.0f,0.0f },
			{ 1.0f,1.0f },
			{ 0.0f,1.0f }
		};
		const float textureIndex = 0.0f;
		const float tilingFactor = 1.0f;

	
		glm::mat4 transform = glm::translate(glm::mat4(1.0f), position)
			* glm::rotate(glm::mat4(1.0f), glm::radians(rotation), { 0.0f, 0.0f, 1.0f })
			* glm::scale(glm::mat4(1.0f), { size.x, size.y, 1.0f });
		for (size_t i = 0; i < quadVertexCount; i++) {
			m_Data.QuadVertexBufferPtr->Position = transform * m_Data.QuadVertexPositions[i];
			m_Data.QuadVertexBufferPtr->Color = color;
			m_Data.QuadVertexBufferPtr->TexCoord = textureCoords[i];
			m_Data.QuadVertexBufferPtr->TexIndex = textureIndex;
			m_Data.QuadVertexBufferPtr->TilingFactor = tilingFactor;
			m_Data.QuadVertexBufferPtr++;
		}
		

		m_Data.QuadIndexCount += 6;
		m_Data.Stats.QuadCount++;

	}
	void Renderer2D::DrawRotateQuad(const glm::vec2& position, const glm::vec2& size, float rotation, const Ref<Texture2D> texture, const float tilingFactor, glm::vec4& tintColor)
	{
		DrawRotateQuad({ position.x,position.y,0.0f }, size, rotation, texture,tilingFactor);

	}
	void Renderer2D::DrawRotateQuad(const glm::vec3& position, const glm::vec2& size, float rotation, const Ref<Texture2D> texture, const float tilingFactor, glm::vec4& tintColor)
	{

		DEE_PROFILE_FUNCTION();

		if (m_Data.QuadIndexCount >= Renderer2DData::MaxIndices) {
			FlushAndReset();
		}
		constexpr glm::vec4 color = { 1.0f,1.0f,1.0f,1.0f };
		float textureIndex = 0.0f;
		constexpr size_t quadVertexCount = 4;
		constexpr glm::vec2 textureCoords[] = {
			{ 0.0f,0.0f },
			{ 1.0f,0.0f },
			{ 1.0f,1.0f },
			{ 0.0f,1.0f }
		};

		for (uint32_t i = 1; i < m_Data.TextureSlotIndex; i++)
		{
			if (*m_Data.TextureSlots[i].get() == *texture.get())
			{
				textureIndex = (float)i;
				break;
			}
		}
		if (textureIndex == 0.0f) {
			textureIndex = (float)m_Data.TextureSlotIndex;
			m_Data.TextureSlots[m_Data.TextureSlotIndex] = texture;
			m_Data.TextureSlotIndex++;
		}

		glm::mat4 transform = glm::translate(glm::mat4(1.0f), position)
			* glm::rotate(glm::mat4(1.0f), glm::radians(rotation), { 0.0f, 0.0f, 1.0f })
			* glm::scale(glm::mat4(1.0f), { size.x, size.y, 1.0f });
		for (size_t i = 0; i < quadVertexCount; i++) {
			m_Data.QuadVertexBufferPtr->Position = transform * m_Data.QuadVertexPositions[i];
			m_Data.QuadVertexBufferPtr->Color = color;
			m_Data.QuadVertexBufferPtr->TexCoord = textureCoords[i];
			m_Data.QuadVertexBufferPtr->TexIndex = textureIndex;
			m_Data.QuadVertexBufferPtr->TilingFactor = tilingFactor;
			m_Data.QuadVertexBufferPtr++;
		}

	

		m_Data.QuadIndexCount += 6;
		m_Data.Stats.QuadCount++;
	}

	void Renderer2D::DrawRotateQuad(const glm::vec2& position, const glm::vec2& size, float rotation, const Ref<SubTexture2D> subTexture, const float tilingFactor, glm::vec4& tintColor)
	{
		DrawRotateQuad({ position.x,position.y,0.0f }, size, rotation, subTexture, tilingFactor);

	}
	void Renderer2D::DrawRotateQuad(const glm::vec3& position, const glm::vec2& size, float rotation, const Ref<SubTexture2D> subTexture, const float tilingFactor, glm::vec4& tintColor)
	{

		DEE_PROFILE_FUNCTION();

		if (m_Data.QuadIndexCount >= Renderer2DData::MaxIndices) {
			FlushAndReset();
		}
		constexpr glm::vec4 color = { 1.0f,1.0f,1.0f,1.0f };
		float textureIndex = 0.0f;
		constexpr size_t quadVertexCount = 4;
		const glm::vec2* textureCoords = subTexture->GetTexCoords();
		const Ref<Texture2D> texture = subTexture->GetTexture();

		for (uint32_t i = 1; i < m_Data.TextureSlotIndex; i++)
		{
			if (*m_Data.TextureSlots[i].get() == *texture.get())
			{
				textureIndex = (float)i;
				break;
			}
		}
		if (textureIndex == 0.0f) {
			textureIndex = (float)m_Data.TextureSlotIndex;
			m_Data.TextureSlots[m_Data.TextureSlotIndex] = texture;
			m_Data.TextureSlotIndex++;
		}

		glm::mat4 transform = glm::translate(glm::mat4(1.0f), position)
			* glm::rotate(glm::mat4(1.0f), glm::radians(rotation), { 0.0f, 0.0f, 1.0f })
			* glm::scale(glm::mat4(1.0f), { size.x, size.y, 1.0f });
		for (size_t i = 0; i < quadVertexCount; i++) {
			m_Data.QuadVertexBufferPtr->Position = transform * m_Data.QuadVertexPositions[i];
			m_Data.QuadVertexBufferPtr->Color = color;
			m_Data.QuadVertexBufferPtr->TexCoord = textureCoords[i];
			m_Data.QuadVertexBufferPtr->TexIndex = textureIndex;
			m_Data.QuadVertexBufferPtr->TilingFactor = tilingFactor;
			m_Data.QuadVertexBufferPtr++;
		}



		m_Data.QuadIndexCount += 6;
		m_Data.Stats.QuadCount++;
	}
	Renderer2D::Statistics Renderer2D::GetStats()
	{
		return m_Data.Stats;
	}
	void Renderer2D::ResetStats()
	{
		memset(&m_Data.Stats, 0, sizeof(Statistics));
	}
}

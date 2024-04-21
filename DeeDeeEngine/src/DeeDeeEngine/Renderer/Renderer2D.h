#pragma once
#include "OrthographicCamera.h"
#include "Texture.h"
namespace DeeDeeEngine {
	class Renderer2D
	{
	public:
		static void Init();
		static void Shutdown();
		static void BeginScene(const OrthographicCamera& camera);
		static void EndScene();
		static void Flush();

		static void DrawQuad(const glm::vec2& position, const glm::vec2& size,const glm::vec4& color);
		static void DrawQuad(const glm::vec3& position, const glm::vec2& size,const glm::vec4& color);
		static void DrawQuad(const glm::vec2& position, const glm::vec2& size,const Ref<Texture2D> texture, const float tilingFactor =1.0f,const glm::vec4& tintColor = glm::vec4(1.0f));
		static void DrawQuad(const glm::vec3& position, const glm::vec2& size,const Ref<Texture2D> texture, const float tilingFactor =1.0f,const glm::vec4& tintColor = glm::vec4(1.0f));
	
		static void DrawRotateQuad(const glm::vec2& position, const glm::vec2& size, float rotation, const glm::vec4& color);
		static void DrawRotateQuad(const glm::vec3& position, const glm::vec2& size, float rotation, const glm::vec4& color);

		static void DrawRotateQuad(const glm::vec2& position, const glm::vec2& size, float rotation, const Ref<Texture2D> texture, const float tilingFactor=1.0f, glm::vec4& tintColor = glm::vec4(1.0f));
		static void DrawRotateQuad(const glm::vec3& position, const glm::vec2& size, float rotation, const Ref<Texture2D> texture, const float tilingFactor=1.0f, glm::vec4& tintColor = glm::vec4(1.0f));


		struct Statistics {
			uint32_t DrawCalls;
			uint32_t QuadCount = 0;
			uint32_t GetTotalVertexCount() {
				return QuadCount * 4;
			}
			uint32_t GetTotalIndexCount() {
				return QuadCount * 6;
			}
		};
		static Statistics GetStats();
		static void ResetStats();

	private:
		static void FlushAndReset();
	};
}

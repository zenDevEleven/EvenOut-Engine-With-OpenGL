#pragma once
#include "OrthographicCamera.h"
#include "Texture.h"

namespace Engine 
{
	class Renderer2D
	{
	public:
		static void Init();
		static void Shutdown();

		static void BeginScene(const OrthographicCamera& camera);
		static void BeginBatch();
		static void EndScene();
		static void EndBatch();

		static void FlushAndReset();

		static void Flush();

		static void DrawQuad(const glm::vec2& postion, const glm::vec2& size);
		static void DrawQuad(const glm::vec3& postion, const glm::vec2& size);
		static void DrawQuad(const glm::vec2& postion, const glm::vec2& size, SDL_FRect* sourceRect, const std::shared_ptr<Texture2D>& texture);
		static void DrawQuad(const glm::vec3& postion, const glm::vec2& size, SDL_FRect* sourceRect, const std::shared_ptr<Texture2D>& texture);
		static void DrawRotatedQuad(const glm::vec2& postion, const glm::vec2& size, float rotation, SDL_FRect* sourceRect, const std::shared_ptr<Texture2D>& texture);
		static void DrawRotatedQuad(const glm::vec3& postion, const glm::vec2& size, float rotation, SDL_FRect* sourceRect, const std::shared_ptr<Texture2D>& texture);
	
		//Stats

		struct Statistics
		{
			uint32_t DrawCalls = 0;
			uint32_t QuadCount = 0;

			uint32_t GetTotalVertexCount() { return QuadCount * 4; }
			uint32_t GetTotalIndexCount() { return QuadCount * 6; }
		};
		static Statistics GetStats();
		static void ResetStats();


		
	};
}



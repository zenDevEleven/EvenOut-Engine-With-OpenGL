#pragma once
#include "epch.h"
#include "RenderCommand.h"
#include "OrthographicCamera.h"
#include "Texture.h"

namespace Engine
{
	class Renderer
	{
	public:
		static void Init(SDL_Window* window);
		static SDL_Texture* LoadTexture(std::string filePath);
		static void RenderTexture(Texture2D& tex);

		static void BeginScene(const OrthographicCamera& camera);
		static void EndScene();

		static void Submit(const std::shared_ptr<VertexArray>& vertexArray, const glm::mat4 transform = glm::mat4(1.0f));

		static SDL_GLContext GetRenderer() { return m_RenderContext; }
	private:
		static SDL_GLContext m_RenderContext;

	};
}


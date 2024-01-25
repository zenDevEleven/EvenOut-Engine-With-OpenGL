#include "epch.h"
#include "Renderer.h"
#include "Engine/LogSystem/Log.h"
#include "ShaderRenderer.h"
#include "Renderer2D.h"

namespace Engine {
	SDL_GLContext Renderer::m_RenderContext;

	void Renderer::Init(SDL_Window* window)
	{	
		if (window != nullptr) {
			m_RenderContext = SDL_GL_CreateContext(window);

			if (!gladLoadGLLoader((GLADloadproc)SDL_GL_GetProcAddress))
			{
				LOG_CORE("Failed to initialize GLAD", LOG_ERROR);
				std::cout << "Failed to initialize GLAD" << std::endl;
				SDL_Quit();
			}
			LOG_CORE("GLAD Initialized!", LOG_INFO);
		}

		Renderer2D::Init();
	}

	SDL_Texture* Renderer::LoadTexture(std::string filePath)
	{
		SDL_Texture* texture = nullptr;
/*
		SDL_Surface* surface = SDL_LoadBMP(filePath.c_str());
		if (surface == nullptr) {
			std::cout << "Error loading the image: " << filePath << std::endl;
		}
		else {
			SDL_SetSurfaceColorKey(surface, SDL_TRUE, SDL_MapRGB(surface->format, 255, 0, 255));
			if (m_RenderTarget != nullptr) {
				texture = SDL_CreateTextureFromSurface(m_RenderTarget, surface);
				if (texture == nullptr)
					std::cout << "Error creating texture from surface: " << filePath << std::endl;
			}
			else {
				std::cout << "Error creating texture from surface (render is null pointer): " << filePath << std::endl;

			}
		}

		SDL_DestroySurface(surface);*/
		return texture;
	}

	void Renderer::RenderTexture(Texture2D& texture) {
		/*SDL_RenderTexture(m_RenderTarget, texture, texture.t_ScreenRect, texture.t_ScreenPosition);*/
	}


	void Renderer::BeginScene(const OrthographicCamera& camera)
	{
		ShaderRenderer::Bind();
		ShaderRenderer::SetMat4("u_ViewProjection", camera.GetViewProjectionMatrix());
	}


	void Renderer::EndScene()
	{

	}


	void Renderer::Submit(const std::shared_ptr<VertexArray>& vertexArray, const glm::mat4 transform)
	{
		ShaderRenderer::Bind();
		ShaderRenderer::SetMat4("u_Transform", transform);

		vertexArray->Bind();
		RenderCommand::DrawIndexed(vertexArray);
	}

}

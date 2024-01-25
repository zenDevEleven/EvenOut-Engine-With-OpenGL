#include "epch.h"
#include "GameEngine.h"
#include "Engine/Core/GameWorld.h"
#include "Engine/Core/PhysicsWorld.h"
#include "Engine/Core/ShaderRenderer.h"

#include "Engine/Core/RenderCommand.h"
#include "Engine/LogSystem/Log.h"

#include "Renderer.h"
#include "Texture.h"
#include "Renderer2D.h"
#include "AudioSystem.h"

namespace Engine {

	GameEngine* GameEngine::m_Instance;

	GameEngine::GameEngine()
	{
		m_Instance = this;
	}

	GameEngine::~GameEngine()
	{

	}

	void GameEngine::Initialize(const char* title, int width, int height)
	{

		LOG_CORE(std::format("Initializing Window({0}, {1})", width, height).c_str(), LOG_INFO);


		if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_GAMEPAD) != 0) {
			LOG_CORE("SDL Initialized", LOG_INFO);
		}

		float aspectRatio = (float)width / height;
		m_Camera = OrthographicCamera(0, width, height, 0);

		SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 5);
		SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 0);

		m_Window = SDL_CreateWindow(title, width, height, SDL_WINDOW_OPENGL);

		Renderer::Init(m_Window);

		m_World = new GameWorld();
		m_PhysicsWorld = new PhysicsWorld();
		m_PhysicsWorld->SetWorld(m_World);

	}

	void GameEngine::HandleEvents()
	{
		while (SDL_PollEvent(&m_GamepadEvent))
		{
			switch (m_GamepadEvent.type) 
			{
			case SDL_EVENT_GAMEPAD_ADDED:
				m_ID = m_GamepadEvent.gdevice.which;
				m_Controller = SDL_OpenGamepad(m_GamepadEvent.gdevice.which);
				LOG_CORE("Controller: " + std::string(SDL_GetGamepadInstanceName(m_GamepadEvent.gdevice.which)), LOG_INFO);
				Input::SendGamepad(m_Controller);
				break;
			case SDL_EVENT_QUIT:
				isRunning = false;
				break;
			}
		}
	}

	void GameEngine::Run()
	{
		isRunning = true;
		while (isRunning) {
			m_Time = (float)SDL_GetTicks();
			float deltaTime = (m_Time - m_PreviousTime) / 1000.0f;
			m_PreviousTime = m_Time;
			
			m_FrameTime += deltaTime;


			HandleEvents();
			m_World->Refresh(); 
			m_PhysicsWorld->Update(deltaTime);
			m_World->Update(deltaTime);

			Render();
		}

		Clean();
	}

	void GameEngine::Render()
	{
		Renderer2D::BeginScene(m_Camera);

		RenderCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 1 });
		RenderCommand::Clear();

		m_World->Draw();

		Renderer2D::EndScene();

		SDL_GL_SwapWindow(m_Window);
	}

	void GameEngine::Clean()
	{
		SDL_DestroyWindow(m_Window);
		//SDL_DestroyRenderer(Renderer::GetRenderer());
		SDL_Quit();
		LOG_CORE("Engine cleaned!", LOG_INFO);
	}

	bool GameEngine::IsRunning()
	{
		return isRunning;
	}

}
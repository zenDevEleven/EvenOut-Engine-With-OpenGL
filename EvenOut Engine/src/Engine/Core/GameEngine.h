#pragma once
#include "Engine/Core/Renderer.h"
#include "Engine/Core/Input.h"

#include "Engine/LogSystem/Log.h"
#include "Buffer.h"
#include "VertexArray.h"
#include "OrthographicCamera.h"

#include "Texture.h"

namespace Engine 
{
	class GameWorld;
	class PhysicsWorld;

	class GameEngine
	{
	public:
		GameEngine();
		~GameEngine();

		void Initialize(const char* title, int width, int height);

		void HandleEvents();
		void Run();
		void Render();
		void Clean();

		bool IsRunning();

		static GameEngine* GetEngine() { return m_Instance; }

		GameWorld& GetWorld() { return *m_World; }

	private:
		static GameEngine* m_Instance;

		float m_PreviousTime = 0;
		float m_Time = 0;

		float m_FrameRate = 60.0f;
		float m_FrameTime = 0.0f;

		bool isRunning = false;

		int m_WindowWidth = 0;
		int m_WindowHeight = 0;

		SDL_Window* m_Window;
		SDL_Renderer* m_Renderer;

		SDL_Event m_GamepadEvent;
		SDL_Gamepad* m_Controller;
		SDL_JoystickID m_ID;

		GameWorld* m_World;
		PhysicsWorld* m_PhysicsWorld;

		OrthographicCamera m_Camera;

		std::shared_ptr<Texture2D> m_Texture;


		glm::mat4 triangle_transformMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(-0.3, 0.3, 0));
	};
}


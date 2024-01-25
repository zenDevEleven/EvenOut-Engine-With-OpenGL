#pragma once
#include "Engine/Core/Actor.h"
#include "Engine/Core/GameWorld.h"
#include "Engine/Core/Components.h"
#include "Engine/LogSystem/Log.h"
#include "Player.h"
#include "UIManager.h"
#include <random>

class GameManager : public Engine::Actor
{
public:
	GameManager();
	virtual ~GameManager() {};

	void Start() override;
	void Update(float deltaTime) override;

	static GameManager& GetManager() { return *m_Instance; }

	UIManager& GetUIManager() const { return *UImanager; }

	Engine::GameWorld& GetWorld() const {return m_World;}

	template<typename T, std::enable_if_t<std::is_floating_point_v<T>, int> = 0>
	T RandomNumber(T min, T max) {

		std::random_device rd;
		std::uniform_real_distribution<T> dist(min, max);

		return dist(rd);
	}

	template<typename T, std::enable_if_t<std::is_integral_v<T>, int> = 0>
	T RandomNumber(T min, T max) {
		std::random_device rd;
		std::uniform_int_distribution<T> dist(min, max);

		return dist(rd);
	}

	template<typename T, typename ...TArgs>
	T* InstantiateObject(TArgs&&... mArgs) {
		return m_World.CreateActor<T>(std::forward<TArgs>(mArgs)...);
	}


	void OnContactEvent(Object* other) override;


	void OnEndContactEvent(Object* other) override;

private:
	static GameManager* m_Instance;

	Engine::GameWorld& m_World = Engine::GameEngine::GetEngine()->GetWorld();
	Player* player;
	UIManager* UImanager;
};


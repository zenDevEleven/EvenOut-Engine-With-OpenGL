#pragma once
#include "Engine/Core/Object.h"
#include "Engine/Core/GameWorld.h"

enum AsteroidsSize : int {
	SMALL = 32,
	MEDIUM = 64,
	LARGE = 96
};

class EnemyManager : public Engine::Object
{
public:
	EnemyManager() {};

	~EnemyManager() {};

	static EnemyManager& GetEnemyManager() { return *m_instance; }

	void Start() override;
	void Update(float deltaTime) override;
	void Draw() override;

	void SpawnDrones(int numberOfDrones, int time);
	void SpawnAsteroids(const char* filepath, int numberOfAsteroids, float minX, float maxX, float minY, float maxY, AsteroidsSize type);

	void OnContactEvent(Object* other) override;
	void OnEndContactEvent(Object* other) override;
private:
	static EnemyManager* m_instance;
	float m_Time = 0.0f;

};


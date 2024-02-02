#pragma once
#include "Engine/Core/Object.h"
#include "Engine/Core/GameWorld.h"

enum AsteroidsSize : int {
	SMALL = 32,
	MEDIUM = 64,
	LARGE = 96
};

enum class RusherPattern
{
	STAIR,
	LINE
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

	void SpawnWave();

	void SpawnDrones(int numberOfDrones, int time);
	void SpawnAsteroids(const char* filepath, int numberOfAsteroids, AsteroidsSize type);
	void SpawnAsteroidsChild(const char* filepath, int numberOfAsteroids, int minX, int maxX, int minY, int maxY, AsteroidsSize type);
	void SpawnRusher(RusherPattern pattern, int numberOfRushers);

	void OnContactEvent(Object* other) override;
	void OnEndContactEvent(Object* other) override;
private:
	static EnemyManager* m_instance;
	float m_Time = 0.0f;
	float m_TimerPerWave = 50.0f;
	int m_WaveCount = 0;
	float m_TimerWave = 0;

};


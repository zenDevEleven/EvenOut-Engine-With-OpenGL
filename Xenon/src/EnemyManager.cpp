#include "EnemyManager.h"
#include "EnemyDrone.h"
#include "Global.h"
#include "GameManager.h"

#include "Engine/LogSystem/Log.h"
#include <random>

#include "BigAsteroid.h"
#include "SmallAsteroid.h"
#include "MediumAsteroid.h"
#include "EnemyLoner.h"
#include "Engine/Core/InputCodes.h"
#include "EnemyRusher.h"

EnemyManager* EnemyManager::m_instance;

glm::vec2 FindRandomSpawnLocation() 
{
	std::random_device rd;
	std::uniform_real_distribution<double> distY(100, (double)Global::HEIGHT - 100);
	std::uniform_real_distribution<double> distX((double)Global::WIDTH + 150, (double)Global::WIDTH + 300);
	
	double valueY = distY(rd);
	double valueX = distX(rd);

	return glm::vec2(valueX, valueY);

}

void EnemyManager::Start()
{
	m_instance = this;
	float Width = (float)Global::WIDTH;
	float Height = (float)Global::HEIGHT;

	GameManager::GetManager().InstantiateObject<EnemyLoner>(FindRandomSpawnLocation());
	SpawnWave();
	
	/*SpawnAsteroids("Assets/Textures/SAster96.bmp", 1, Global::WIDTH + 300.0f, Global::WIDTH + 750.0f, 96.0f, Global::HEIGHT - 96.0f, AsteroidsSize::LARGE);
	SpawnAsteroids("Assets/Textures/MAster96.bmp", 1, Global::WIDTH + 400.0f, Global::WIDTH + 850.0f, 96.0f, Global::HEIGHT - 96.0f, AsteroidsSize::LARGE);
	SpawnAsteroids("Assets/Textures/SAster96.bmp", 1, Global::WIDTH + 400.0f, Global::WIDTH + 950.0f, 96.0f, Global::HEIGHT - 96.0f, AsteroidsSize::LARGE);
	SpawnAsteroids("Assets/Textures/MAster96.bmp", 2, Global::WIDTH + 600.0f, Global::WIDTH + 950.0f, 96.0f, Global::HEIGHT - 96.0f, AsteroidsSize::LARGE);
	SpawnAsteroids("Assets/Textures/MAster96.bmp", 1, Global::WIDTH + 100.0f, Global::WIDTH + 2050.0f, 96.0f, Global::HEIGHT - 96.0f, AsteroidsSize::LARGE);*/
}

void EnemyManager::Update(float deltaTime)
{
	m_TimerWave += deltaTime;
	m_Time += deltaTime;

	if (m_TimerWave >= m_TimerPerWave)
	{
		m_TimerWave = 0;
		m_WaveCount++;
		SpawnWave();
	}
}

void EnemyManager::Draw()
{
	
}


void EnemyManager::SpawnWave()
{
	switch (m_WaveCount)
	{
	case 0:
		SpawnDrones(8, m_Time);
		SpawnDrones(5, m_Time);
		SpawnDrones(2, m_Time);
		break;
	case 1:
		SpawnDrones(7, m_Time);
		SpawnAsteroids("Assets/Textures/SAster96.bmp", 1, AsteroidsSize::LARGE);
		SpawnAsteroids("Assets/Textures/SAster64.bmp", 1, AsteroidsSize::MEDIUM);
		break;
	case 2:
		SpawnDrones(3, m_Time);
		SpawnRusher(RusherPattern::LINE, 4);
		SpawnDrones(8, m_Time);
		SpawnRusher(RusherPattern::LINE, 2);
		break;
	case 3:
		SpawnAsteroids("Assets/Textures/MAster96.bmp", 2, AsteroidsSize::LARGE);
		GameManager::GetManager().InstantiateObject<EnemyLoner>(FindRandomSpawnLocation());
		GameManager::GetManager().InstantiateObject<EnemyLoner>(FindRandomSpawnLocation());
		SpawnAsteroids("Assets/Textures/SAster96.bmp", 2, AsteroidsSize::LARGE);
		break;
	case 4:
		GameManager::GetManager().InstantiateObject<EnemyLoner>(FindRandomSpawnLocation());
		SpawnRusher(RusherPattern::STAIR, 4);
		SpawnAsteroids("Assets/Textures/MAster96.bmp", 2, AsteroidsSize::LARGE);
		SpawnDrones(7, m_Time);
		break;
	case 5:
		GameManager::GetManager().InstantiateObject<EnemyLoner>(FindRandomSpawnLocation());
		SpawnAsteroids("Assets/Textures/SAster96.bmp", 2, AsteroidsSize::LARGE);
		SpawnAsteroids("Assets/Textures/MAster96.bmp", 1, AsteroidsSize::LARGE);
		SpawnDrones(7, m_Time);
		SpawnDrones(4, m_Time);
		break;
	case 6:
		m_WaveCount = 0;
		SpawnWave();
		break;
	}

}

void EnemyManager::SpawnDrones(int numberOfDrones, int time)
{
	glm::vec2 startPos = FindRandomSpawnLocation();
	float offsetX = 0;
	for (size_t i = 0; i < numberOfDrones; ++i)
	{
		GameManager::GetManager().InstantiateObject<EnemyDrone>(startPos.x + offsetX, startPos.y);

		offsetX += 32;
	}
}

void EnemyManager::SpawnRusher(RusherPattern pattern, int numberOfRushers)
{
	glm::vec2 tmpLocation = FindRandomSpawnLocation();

	float distanceBetweenEnemies = Global::HEIGHT / numberOfRushers;


	float yOffset = (64.0f - distanceBetweenEnemies) / 2;

	float xOffsetStairs = (32.0f - distanceBetweenEnemies) / 2;

	for (int i = 0; i < numberOfRushers; ++i)
	{
		switch (pattern)
		{
		case RusherPattern::STAIR:
			tmpLocation = glm::vec2(tmpLocation.x + (i * distanceBetweenEnemies - xOffsetStairs), i * distanceBetweenEnemies - yOffset);
			break;
		case RusherPattern::LINE:
			tmpLocation = glm::vec2(tmpLocation.x, i * distanceBetweenEnemies - yOffset);
			break;
		}
		GameManager::GetManager().InstantiateObject<EnemyRusher>(glm::vec2(tmpLocation));
	}
}

void EnemyManager::SpawnAsteroids(const char* filepath, int numberOfAsteroids, AsteroidsSize type)
{
	for (size_t i = 0; i < numberOfAsteroids; ++i)
	{
		glm::vec2 startPos = FindRandomSpawnLocation();

		switch (type)
		{

		case SMALL:
			GameManager::GetManager().InstantiateObject<SmallAsteroid>(filepath, startPos.x, startPos.y);
			break;
		case MEDIUM:
			GameManager::GetManager().InstantiateObject<MediumAsteroid>(filepath, startPos.x, startPos.y);
			break;
		case LARGE:
			GameManager::GetManager().InstantiateObject<BigAsteroid>(filepath, startPos.x, startPos.y);
			break;
		}
	}
}

void EnemyManager::SpawnAsteroidsChild(const char* filepath, int numberOfAsteroids, int minX, int maxX, int minY, int maxY, AsteroidsSize type)
{
	for (size_t i = 0; i < numberOfAsteroids; ++i)
	{
		int x = GameManager::GetManager().RandomNumber<int>(minX, maxX);
		int y = GameManager::GetManager().RandomNumber<int>(minY, maxY);

		switch (type)
		{

		case SMALL:
			GameManager::GetManager().InstantiateObject<SmallAsteroid>(filepath, x, y);
			break;
		case MEDIUM:
			GameManager::GetManager().InstantiateObject<MediumAsteroid>(filepath, x, y);
			break;		
		case LARGE:
			GameManager::GetManager().InstantiateObject<BigAsteroid>(filepath, x, y);
			break;
		}
	}
}

void EnemyManager::OnContactEvent(Object* other)
{
	
}

void EnemyManager::OnEndContactEvent(Object* other)
{
	
}

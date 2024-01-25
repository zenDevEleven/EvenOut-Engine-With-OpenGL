#include "MediumAsteroid.h"
#include "Engine/LogSystem/Log.h"
#include "Global.h"
#include "GameManager.h"
#include "EnemyManager.h"

using Frame = Engine::AnimatorComponent::AnimationFrame;
using Animation = Engine::AnimatorComponent::Animation;

MediumAsteroid::MediumAsteroid(const char* asteroidPath, float PositionX, float PositionY)
{
	AddTag("Asteroid");
	m_FilePath = asteroidPath;

	m_PositionX = PositionX;
	m_PositionY = PositionY;

	m_Transform = &AddComponent<Engine::TransformComponent>(glm::vec2(m_PositionX, m_PositionY));
	m_SpriteRender = &AddComponent<Engine::SpriteRenderer2D>(asteroidPath, 64, 0);
	m_Animator = &AddComponent<Engine::AnimatorComponent>(m_SpriteRender->m_SpriteTexture);
	m_Rigidbody = &AddComponent<Engine::Rigidbody2D>(Engine::Rigidbody2D::BodyType::Dynamic);
	m_Collider = &AddComponent<Engine::BoxCollider2DComponent>(glm::vec2(m_SpriteRender->m_SpriteTexture->t_PixelSize,
		m_SpriteRender->m_SpriteTexture->t_PixelSize), m_Rigidbody);

	m_Health = &AddComponent<Engine::HealthComponent>(100.0f);
	m_Health->setOnDieCallback(this, &MediumAsteroid::OnDie);
}

void MediumAsteroid::Start()
{
	__super::Start();

	m_VelocityX = GameManager::GetManager().RandomNumber<float>(50.0f, 100.0f);

	m_Animator->CreateAnimation(new Animation("AsteroidIdle", std::vector<Frame>{
		Frame(1, 1),
		Frame(3, 8)
	}));
}

void MediumAsteroid::Update(float deltaTime)
{
	__super::Update(deltaTime);
	m_Animator->PlayAnimationContiniousToFrame("AsteroidIdle", 0.06f, deltaTime, true);
	m_Transform->AddPos(-m_VelocityX * deltaTime, 0.0f);
}

void MediumAsteroid::OnDie()
{
	LOG_APP("im dead", Engine::LOG_ERROR);
	GameManager::GetManager().GetUIManager().AddPlayerScore(7500);

	std::string newPath = m_FilePath;

	std::string toReplace = "64";
	std::size_t pos = newPath.find(toReplace);

	if (pos == std::string::npos) return;

	newPath.replace(pos, toReplace.length(), "32");

	EnemyManager::GetEnemyManager().SpawnAsteroids(newPath.c_str(), GameManager::GetManager().RandomNumber<int>(1, 5),
		m_Transform->Position.x - 16.0f, m_Transform->Position.x + 16.0f, m_Transform->Position.y - 8.0f, m_Transform->Position.y + 8.0f, AsteroidsSize::SMALL);

	Destroy();
}

void MediumAsteroid::OnContactEvent(Object* other)
{
	__super::OnContactEvent(other);

	if (other->HasTag("Missile")) {
		m_Health->TakeDamage(50.0f);
	}
}

void MediumAsteroid::OnEndContactEvent(Object* other)
{
	__super::OnEndContactEvent(other);
}

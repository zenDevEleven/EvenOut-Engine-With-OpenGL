#include "SmallAsteroid.h"
#include "Engine/LogSystem/Log.h"
#include "Global.h"
#include "GameManager.h"
#include "EnemyManager.h"

using Frame = Engine::AnimatorComponent::AnimationFrame;
using Animation = Engine::AnimatorComponent::Animation;

SmallAsteroid::SmallAsteroid(const char* asteroidPath, float PositionX, float PositionY)
{
	AddTag("Asteroid");
	m_FilePath = asteroidPath;

	m_PositionX = PositionX;
	m_PositionY = PositionY;

	m_Transform = &AddComponent<Engine::TransformComponent>(glm::vec2(m_PositionX, m_PositionY));
	m_SpriteRender = &AddComponent<Engine::SpriteRenderer2D>(asteroidPath, 32, 0);
	m_Animator = &AddComponent<Engine::AnimatorComponent>(m_SpriteRender->m_SpriteTexture);
	m_Rigidbody = &AddComponent<Engine::Rigidbody2D>(Engine::Rigidbody2D::BodyType::Dynamic);
	m_Collider = &AddComponent<Engine::BoxCollider2DComponent>(glm::vec2(m_SpriteRender->m_SpriteTexture->t_PixelSize,
		m_SpriteRender->m_SpriteTexture->t_PixelSize), m_Rigidbody);

	m_Health = &AddComponent<Engine::HealthComponent>(100.0f);
	m_Health->setOnDieCallback(this, &SmallAsteroid::OnDie);
}

void SmallAsteroid::Start()
{
	__super::Start();

	m_VelocityY = GameManager::GetManager().RandomNumber<float>(10.0f, 40.0f);
	m_VelocityX = GameManager::GetManager().RandomNumber<float>(100.0f, 140.0f);

	m_Animator->CreateAnimation(new Animation("AsteroidIdle", std::vector<Frame>{
		Frame(1, 1),
			Frame(3, 8)
	}));
}

void SmallAsteroid::Update(float deltaTime)
{
	__super::Update(deltaTime);
	m_Animator->PlayAnimationContiniousToFrame("AsteroidIdle", 0.06f, deltaTime, true);
	m_Transform->AddPos(-m_VelocityX * deltaTime, m_VelocityY * deltaTime);
}

void SmallAsteroid::OnDie()
{
	LOG_APP("im dead", Engine::LOG_ERROR);
	GameManager::GetManager().GetUIManager().AddPlayerScore(5000);
	Destroy();
}

void SmallAsteroid::OnContactEvent(Object* other)
{
	__super::OnContactEvent(other);

	if (other->HasTag("Missile")) {
		m_Health->TakeDamage(50.0f);
	}
}

void SmallAsteroid::OnEndContactEvent(Object* other)
{
	__super::OnEndContactEvent(other);
}

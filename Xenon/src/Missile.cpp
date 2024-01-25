#include "Missile.h"
#include "DestructionParticle.h"
#include "GameManager.h"

using Frame = Engine::AnimatorComponent::AnimationFrame;

Missile::Missile(float missileSpeed, glm::vec2 spawnPos, MissileTier tier)
{
	m_MissileSpeed = missileSpeed;
	m_Tier = tier;

	AddTag("Missile");
	m_Transform = &AddComponent<Engine::TransformComponent>(spawnPos);

	m_SpriteRenderer = &AddComponent<Engine::SpriteRenderer2D>("Assets/Textures/missile.bmp", 16, 2, 0);
	m_Animator = &AddComponent<Engine::AnimatorComponent>(m_SpriteRenderer->m_SpriteTexture);
	m_Rigidbody = &AddComponent<Engine::Rigidbody2D>(Engine::Rigidbody2D::BodyType::Dynamic);
	m_BoxCollider = &AddComponent<Engine::BoxCollider2DComponent>(glm::vec2(m_SpriteRenderer->m_SpriteTexture->t_PixelSize * 2,
																			m_SpriteRenderer->m_SpriteTexture->t_PixelSize * 2), m_Rigidbody);
	m_SpriteRenderer->SetRotation(90.0f);
}

void Missile::Start()
{
	__super::Start();


	switch (m_Tier)
	{
	case MissileTier::TIER1:
		m_MissileDamage = 15.0f;
		m_Animator->SetStartFrame(Frame(1, 1));
		break;
	case MissileTier::TIER2:
		m_MissileDamage = 25.0f;
		m_Animator->SetStartFrame(Frame(1, 2));
		break;
	case MissileTier::TIER3:
		m_MissileDamage = 35.0f;
		m_Animator->SetStartFrame(Frame(1, 3));
		break;
	}
}

void Missile::Update(float deltaTime)
{
	__super::Update(deltaTime);
	m_Time += deltaTime;
	if (m_Time >= m_MissileLifeTime) {
		Destroy();
	}
	else {
		m_Transform->AddPos(m_MissileSpeed * deltaTime, 0);
	}
}

void Missile::OnContactEvent(Object* other)
{
	__super::OnContactEvent(other);

	if (!other->HasTag("Player") && !other->HasTag("Clone")) {
		GameManager::GetManager().InstantiateObject<DestructionParticle>(m_Transform->Position);
		Destroy();
	}

}

void Missile::OnEndContactEvent(Object* other)
{
	__super::OnEndContactEvent(other);
}

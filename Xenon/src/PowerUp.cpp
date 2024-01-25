#include "PowerUp.h"
#include "Player.h"

using Frame = Engine::AnimatorComponent::AnimationFrame;
using Animation = Engine::AnimatorComponent::Animation;

PowerUp::PowerUp(PowerUpType type, glm::vec2 position)
{
	m_Type = type;
	m_Position = position;

	m_Transform = &AddComponent<Engine::TransformComponent>(m_Position);

	switch (m_Type)
	{
	case PowerUpType::MISSILE_PU:
		m_SpriteRenderer = &AddComponent<Engine::SpriteRenderer2D>("Assets/Textures/PUMissil.bmp", 32, 1);
		break;
	case PowerUpType::LIFE_PU:
		m_SpriteRenderer = &AddComponent<Engine::SpriteRenderer2D>("Assets/Textures/PULife.bmp", 32, 1);
		break;
	case PowerUpType::SPEED_PU:
		m_SpriteRenderer = &AddComponent<Engine::SpriteRenderer2D>("Assets/Textures/PUSpeed.bmp", 32, 1);
		break;
	case PowerUpType::SHIEL_PU:
		m_SpriteRenderer = &AddComponent<Engine::SpriteRenderer2D>("Assets/Textures/PUShield.bmp", 32, 1);
		break;
	default:
		break;
	}


	m_Animator = &AddComponent<Engine::AnimatorComponent>(m_SpriteRenderer->m_SpriteTexture);
	m_Rigidbody = &AddComponent<Engine::Rigidbody2D>(Engine::Rigidbody2D::BodyType::Dynamic);
	m_Collider = &AddComponent<Engine::BoxCollider2DComponent>(glm::vec2(m_SpriteRenderer->m_SpriteTexture->t_PixelSize,
		m_SpriteRenderer->m_SpriteTexture->t_PixelSize), m_Rigidbody);
}

void PowerUp::Start()
{
	__super::Start();

	m_Animator->CreateAnimation(new Animation("PowerUpIdle", std::vector<Frame>{
		Frame(1, 1),
		Frame(2, 4)
	}));
}

void PowerUp::Update(float deltaTime)
{
	__super::Update(deltaTime);

	m_Animator->PlayAnimationContiniousToFrame("PowerUpIdle", 0.05f, deltaTime, true);
	m_Transform->AddPos(-30.0f * deltaTime, 0.0f);
}

void PowerUp::OnContactEvent(Object* other)
{
	if (other->HasTag("Player")) {
		switch (m_Type)
		{
		case PowerUpType::MISSILE_PU:
			Player* player = dynamic_cast<Player*>(other);
			player->UpgradeMissileTier();
			Destroy();
			break;
		}
	}
}

void PowerUp::OnEndContactEvent(Object* other)
{
	
}

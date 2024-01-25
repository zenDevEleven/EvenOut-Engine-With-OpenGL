#include "EnemyDrone.h"
using Frame = Engine::AnimatorComponent::AnimationFrame;
using Animation = Engine::AnimatorComponent::Animation;

EnemyDrone::EnemyDrone(float PositionX, float PositionY)
{
	m_PositionX = PositionX;
	m_PositionY = PositionY;

	AddTag("Enemy");
	m_Transform = &AddComponent<Engine::TransformComponent>(glm::vec2(m_PositionX, m_PositionY));
	m_SpriteRenderer = &AddComponent<Engine::SpriteRenderer2D>("Assets/Textures/drone.bmp", 32, 0);
	m_Animator = &AddComponent<Engine::AnimatorComponent>(m_SpriteRenderer->m_SpriteTexture);
	m_RigidBody = &AddComponent<Engine::Rigidbody2D>(Engine::Rigidbody2D::BodyType::Dynamic);
	m_Collider = &AddComponent<Engine::BoxCollider2DComponent>(glm::vec2(m_SpriteRenderer->m_SpriteTexture->t_PixelSize,
																m_SpriteRenderer->m_SpriteTexture->t_PixelSize), m_RigidBody);

	m_SpriteRenderer->SetRotation(90.0f);
}

void EnemyDrone::Start()
{
	__super::Start();

	m_Animator->CreateAnimation(new Animation("IdleEnemy", std::vector <Frame>{
			Frame(1, 1),
			Frame(9, 2)
	}));

	m_Animator->SetStartFrame(Frame(1, 1));
}

void EnemyDrone::Update(float deltaTime)
{
	__super::Update(deltaTime);

	m_Animator->PlayAnimationContiniousToFrame("IdleEnemy", 0.06, deltaTime, true);
	
	m_Transform->SetPosition(m_Transform->Position.x + (-m_VelocityX * deltaTime), m_PositionY + (std::sin(m_Transform->Position.x / 6) * m_YThreshold) * deltaTime);

}

void EnemyDrone::OnContactEvent(Object* other)
{
	__super::OnContactEvent(other);
	if (other->HasTag("Missile")) {
		LOG_APP("Destroyed!", Engine::LOG_ERROR);
		Destroy();
	}
}

void EnemyDrone::OnEndContactEvent(Object* other)
{
	__super::OnEndContactEvent(other);
}

#include "EnemyLoner.h"
#include "Global.h"
#include "GameManager.h"
using Frame = Engine::AnimatorComponent::AnimationFrame;
using Animation = Engine::AnimatorComponent::Animation;

EnemyLoner::EnemyLoner(glm::vec2 SpawnPosition)
{
	AddTag("Enemy");
	m_Transform = &AddComponent<Engine::TransformComponent>(SpawnPosition);
	m_SpriteRenderer = &AddComponent<Engine::SpriteRenderer2D>("Assets/Textures/LonerB.bmp", 64, 1);
	m_Animator = &AddComponent<Engine::AnimatorComponent>(m_SpriteRenderer->m_SpriteTexture);
	m_RigidBody = &AddComponent<Engine::Rigidbody2D>(Engine::Rigidbody2D::BodyType::Dynamic);
	m_Collider = &AddComponent<Engine::BoxCollider2DComponent>(glm::vec2(m_SpriteRenderer->m_SpriteTexture->t_PixelSize,
		m_SpriteRenderer->m_SpriteTexture->t_PixelSize), m_RigidBody);
	m_SpriteRenderer->SetRotation(90.0f);
}

void EnemyLoner::Start()
{
	__super::Start();

	m_Animator->CreateAnimation(new Animation("IdleEnemyLoner", std::vector <Frame>{
		Frame(1, 1),
		Frame(4, 4)
	}));

	m_Animator->SetStartFrame(Frame(1, 1));

	m_Timer = m_ShootInterval;
}

void EnemyLoner::Update(float deltaTime)
{
	__super::Update(deltaTime);

	m_Animator->PlayAnimationContiniousToFrame("IdleEnemyLoner", 0.06, deltaTime, true);

	if (m_Transform->Position.y <= 32 && !PatrolSideRight)
		PatrolSideRight = true;
	else if (m_Transform->Position.y >= Global::HEIGHT - 32 && PatrolSideRight)
		PatrolSideRight = false;

	if(!PatrolSideRight)
		m_Transform->AddPos(-m_VelocityX * deltaTime, -m_VelocityY * deltaTime);
	else
		m_Transform->AddPos(-m_VelocityX * deltaTime, m_VelocityY * deltaTime);

	Engine::TransformComponent playerTransform = Engine::GameEngine::GetEngine()->GetWorld().GetActorOfClass<Player>().GetComponent<Engine::TransformComponent>();
	if (m_Timer >= m_ShootInterval) {
		m_Timer = 0;
		glm::vec2 directionV = playerTransform.Position - m_Transform->Position;
		GameManager::GetManager().InstantiateObject<LonerBullet>(
			glm::vec2(m_Transform->Position.x - 16, m_Transform->Position.y + 16), 
			glm::vec2(directionV.x - 16, directionV.y + 16));
	}
	m_Timer+= deltaTime;
}

void EnemyLoner::OnContactEvent(Object* other)
{
	__super::OnContactEvent(other);
	if (other->HasTag("Missile")) {
		LOG_APP("Destroyed!", Engine::LOG_ERROR);
		Destroy();
	}
}

void EnemyLoner::OnEndContactEvent(Object* other)
{
	__super::OnEndContactEvent(other);
}


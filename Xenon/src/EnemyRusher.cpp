#include "EnemyRusher.h"
#include "GameManager.h"

using Frame = Engine::AnimatorComponent::AnimationFrame;
using Animation = Engine::AnimatorComponent::Animation;

EnemyRusher::EnemyRusher(glm::vec2 SpawnPos)
{
	AddTag("Enemy");
	m_Transform = &AddComponent<Engine::TransformComponent>(SpawnPos);
	m_SpriteRenderer = &AddComponent<Engine::SpriteRenderer2D>("Assets/Textures/rusher.bmp", glm::vec2(64.0f, 32.0f), 1);
	m_Animator = &AddComponent<Engine::AnimatorComponent>(m_SpriteRenderer->m_SpriteTexture);
	m_RigidBody = &AddComponent<Engine::Rigidbody2D>(Engine::Rigidbody2D::BodyType::Dynamic);
	m_Collider = &AddComponent<Engine::BoxCollider2DComponent>(glm::vec2(m_SpriteRenderer->m_SpriteTexture->t_PixelSize,
		m_SpriteRenderer->m_SpriteTexture->t_PixelSize), m_RigidBody);
	m_SpriteRenderer->SetRotation(90.0f);

	m_Health = &AddComponent<Engine::HealthComponent>(150.0f);
	m_Health->setOnDieCallback(this, &EnemyRusher::OnDie);
}

void EnemyRusher::Start()
{
	__super::Start();
	m_Animator->CreateAnimation(new Animation("IdleEnemyRusher", std::vector <Frame>{
		Frame(1, 1),
		Frame(6, 4)
	}));

	m_Animator->SetStartFrame(Frame(1, 1));
}

void EnemyRusher::Update(float deltaTime)
{
	__super::Update(deltaTime);
	m_Animator->PlayAnimationContiniousToFrame("IdleEnemyRusher", 0.05f, deltaTime, true);
	m_Transform->AddPos(-m_YVelocity * deltaTime, 0.0f);
}

void EnemyRusher::OnContactEvent(Object* other)
{
	__super::OnContactEvent(other);
	if (other->HasTag("Player")) {
		Player* player = dynamic_cast<Player*>(other);
		player->TakeDamage(m_Damage);
	}
}

void EnemyRusher::OnEndContactEvent(Object* other)
{
	__super::OnEndContactEvent(other);
}

void EnemyRusher::OnDie()
{
	GameManager::GetManager().GetUIManager().AddPlayerScore(7000);
	GameManager::GetManager().GetUIManager().SpawnScorePopUp(7000, { m_Transform->Position.x - 32.0f, m_Transform->Position.y + 16.0f });
	Destroy();
}


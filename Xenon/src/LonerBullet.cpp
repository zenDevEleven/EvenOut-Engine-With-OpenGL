#include "LonerBullet.h"
#include "Player.h"

using Frame = Engine::AnimatorComponent::AnimationFrame;

LonerBullet::LonerBullet(glm::vec2 spawnPos, glm::vec2 direction)
{
	AddTag("LonerBullet");
	m_Transform = &AddComponent<Engine::TransformComponent>(spawnPos);
	m_SpriteRenderer = &AddComponent<Engine::SpriteRenderer2D>("Assets/Textures/EnWeap6.bmp", 16, 2, 0);
	m_Animator = &AddComponent<Engine::AnimatorComponent>(m_SpriteRenderer->m_SpriteTexture);
	m_RigidBody = &AddComponent<Engine::Rigidbody2D>(Engine::Rigidbody2D::BodyType::Dynamic);
	m_Collider = &AddComponent<Engine::BoxCollider2DComponent>(glm::vec2(m_SpriteRenderer->m_SpriteTexture->t_PixelSize,
		m_SpriteRenderer->m_SpriteTexture->t_PixelSize), m_RigidBody);

	float vLength = sqrt(direction.x * direction.x + direction.y * direction.y);  // a scalar value
	m_UnitDirectionVector = { (direction.x / vLength), (direction.y / vLength)};
}

void LonerBullet::Start()
{
	__super::Start();

	m_Animator->CreateAnimation
	(new Engine::AnimatorComponent::Animation("BulletIdle", std::vector<Engine::AnimatorComponent::AnimationFrame> {
			Frame(1, 1),
			Frame(1, 8)
	}));

}

void LonerBullet::Update(float deltaTime)
{
	__super::Update(deltaTime);
	m_Animator->PlayAnimationContiniousToFrame("BulletIdle", 0.05f, deltaTime, true);
	m_Transform->AddPos(-m_UnitDirectionVector.x * -m_BulletSpeed * deltaTime, m_UnitDirectionVector.y * m_BulletSpeed * deltaTime);
}

void LonerBullet::OnContactEvent(Object* other)
{
	__super::OnContactEvent(other);
	if (other->HasTag("Player"))
	{
		Player* player = dynamic_cast<Player*>(other);
		player->TakeDamage(m_Damage);
		Destroy();
	}
}

void LonerBullet::OnEndContactEvent(Object* other)
{
	__super::OnEndContactEvent(other);
}

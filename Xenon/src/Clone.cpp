#include "Clone.h"
#include "Missile.h"
#include "GameManager.h"

using Frame = Engine::AnimatorComponent::AnimationFrame;

Clone::Clone(glm::vec3 ClonePosition, CloneType type)
{
	AddTag("Clone");
	m_SpawnedType = type;
	m_Transform = &AddComponent<Engine::TransformComponent>(ClonePosition);
	m_SpriteRenderer = &AddComponent<Engine::SpriteRenderer2D>("Assets/Textures/clone.bmp", 32, 1);
	m_Animator = &AddComponent<Engine::AnimatorComponent>(m_SpriteRenderer->m_SpriteTexture);
	m_RigidBody = &AddComponent<Engine::Rigidbody2D>(Engine::Rigidbody2D::BodyType::Dynamic);
	m_Collider = &AddComponent<Engine::BoxCollider2DComponent>(glm::vec2(m_SpriteRenderer->m_SpriteTexture->t_PixelSize,
		m_SpriteRenderer->m_SpriteTexture->t_PixelSize), m_RigidBody);

	if (type == CloneType::CloneCompanion)
	{
		m_Health = &AddComponent<Engine::HealthComponent>(100.0f);
		m_Health->setOnDieCallback(this, &Clone::OnDie);
	}

	m_SpriteRenderer->SetRotation(90.0f);
}

void Clone::Start()
{
	__super::Start();

	m_Animator->SetStartFrame(Frame(1, 1));

	m_Animator->CreateAnimation
	(new Engine::AnimatorComponent::Animation("CloneIdle", std::vector<Frame> {
		    Frame(1, 1),
			Frame(1, 2),
			Frame(1, 3),
			Frame(1, 4),
			Frame(2, 1),
			Frame(2, 2),
			Frame(2, 3),
			Frame(2, 4),
			Frame(3, 1),
			Frame(3, 2),
			Frame(3, 3),
			Frame(3, 4),
			Frame(4, 1),
			Frame(4, 2),
			Frame(4, 3),
			Frame(4, 4)
	}));
}

void Clone::SetClonePosition(glm::vec3 ClonePosition)
{
	m_Transform->SetPosition(ClonePosition.x, ClonePosition.y);
}

void Clone::ShootMissile(MissileTier tier)
{
	GameManager::GetManager().InstantiateObject<Missile>(700.0f, glm::vec2(m_Transform->Position.x, m_Transform->Position.y + 8), tier);
}


void Clone::Update(float deltaTime)
{
	__super::Update(deltaTime);

	m_Animator->PlayAnimation("CloneIdle", 0.07, deltaTime, true);

	if (m_SpawnedType == CloneType::ClonePowerUp) {
		m_Transform->AddPos(-30.0f * deltaTime, 0.0f);
	}
}

void Clone::OnContactEvent(Object* other)
{
	__super::OnContactEvent(other);
	if (other->HasTag("Player")) {
		if (m_SpawnedType == CloneType::ClonePowerUp) {
			Player* player = dynamic_cast<Player*>(other);
			player->SpawnCloneCompanion();
			Destroy();
		}
	}
}

void Clone::OnEndContactEvent(Object* other)
{
	__super::OnEndContactEvent(other);
}

void Clone::OnDie()
{


}

#include "Player.h"
#include "Missile.h"
#include "GameManager.h"
#include "Engine/LogSystem/Log.h"
#include "Engine/Core/InputCodes.h"
#include "Clone.h"

using Frame = Engine::AnimatorComponent::AnimationFrame;

Player::Player()
{
	AddTag("Player");
	m_Transform = &AddComponent<Engine::TransformComponent>(glm::vec2(300.0f, 400.0f));
	m_SpriteRenderer = &AddComponent<Engine::SpriteRenderer2D>("Assets/Textures/Ship1.bmp", 64, 1);
	m_Animator = &AddComponent<Engine::AnimatorComponent>(m_SpriteRenderer->m_SpriteTexture);
	m_RigidBody2d = &AddComponent<Engine::Rigidbody2D>(Engine::Rigidbody2D::BodyType::Dynamic);
	m_Collider = &AddComponent<Engine::BoxCollider2DComponent>(glm::vec2(m_SpriteRenderer->m_SpriteTexture->t_PixelSize,
		m_SpriteRenderer->m_SpriteTexture->t_PixelSize), m_RigidBody2d);

	m_Health = &AddComponent<Engine::HealthComponent>(100.0f);
	m_Health->setOnDieCallback(this, &Player::OnDie);

	m_SpriteRenderer->SetRotation(90.0f);

	m_MissileTier = MissileTier::TIER1;
}

void Player::Start()
{
	__super::Start();

	GameManager::GetManager().GetUIManager().SetLifePoints(lifePoints);

	m_Animator->SetStartFrame(Engine::AnimatorComponent::AnimationFrame(4, 1));

	m_Animator->CreateAnimation
	(new Engine::AnimatorComponent::Animation("SpaceshipRight", std::vector<Engine::AnimatorComponent::AnimationFrame> {
			Frame(5, 1),
			Frame(6, 1),
			Frame(7, 1)
	}));

	m_Animator->CreateAnimation
	(new Engine::AnimatorComponent::Animation("IdleFromRight", std::vector<Engine::AnimatorComponent::AnimationFrame> {
		Engine::AnimatorComponent::AnimationFrame(7, 1),
			Frame(6, 1),
			Frame(5, 1),
			Frame(4, 1)
	}));

	m_Animator->CreateAnimation
	(new Engine::AnimatorComponent::Animation("IdleFromLeft", std::vector<Engine::AnimatorComponent::AnimationFrame> {
		Engine::AnimatorComponent::AnimationFrame(1, 1),
			Frame(2, 1),
			Frame(3, 1),
			Frame(4, 1)
	}));

	m_Animator->CreateAnimation
	(new Engine::AnimatorComponent::Animation("SpaceshipLeft", std::vector<Engine::AnimatorComponent::AnimationFrame> {
			Frame(3, 1),
			Frame(2, 1),
			Frame(1, 1),
	}));
}

void Player::Update(float deltaTime)	
{
	__super::Update(deltaTime);

	m_CurrentTime += deltaTime;

	float XAxis = Engine::Input::GetGamepadAxis(ENGINE_INPUT_GAMEPAD_AXIS_LEFTX);
	float YAxis = Engine::Input::GetGamepadAxis(ENGINE_INPUT_GAMEPAD_AXIS_LEFTY);

	if (YAxis > 0) {
		m_Animator->PlayAnimation("SpaceshipRight", 0.06, deltaTime, false);
		lastY = YAxis;
	}
	else if (YAxis < 0) {
		m_Animator->PlayAnimation("SpaceshipLeft", 0.06, deltaTime, false);
		lastY = YAxis;
	}
	else if (YAxis == 0) {
		if (lastY == 1)
			m_Animator->PlayAnimation("IdleFromRight", 0.06, deltaTime, false);
		else
			m_Animator->PlayAnimation("IdleFromLeft", 0.06, deltaTime, false);
	}

	if (Engine::Input::IsGamepadButtonPressed(ENGINE_INPUT_GAMEPAD_BUTTON_NORTH, false)) {
		if (m_NexShotTime < m_CurrentTime) {
			m_NexShotTime = m_CurrentTime + m_FireRate;
			GameManager::GetManager().InstantiateObject<Missile>(700.0f, glm::vec2(m_Transform->Position.x + 16, m_Transform->Position.y + 16), m_MissileTier);
			if (m_Clone != nullptr) {
				m_Clone->ShootMissile(MissileTier::TIER1);
			}
		}
	}

	m_Transform->AddPos(XAxis * 150.0f * deltaTime, YAxis * 150.0f * deltaTime);

	if (m_Clone != nullptr) {
		m_Clone->SetClonePosition(glm::vec3(m_Transform->Position.x - 16, m_Transform->Position.y + 80, 0.0f));
	}
}



void Player::UpgradeMissileTier()
{
	switch (m_MissileTier)
	{
	case MissileTier::TIER1:
		m_MissileTier = MissileTier::TIER2;
		break;
	case MissileTier::TIER2:
		m_MissileTier = MissileTier::TIER3;
		break;
	case MissileTier::TIER3:
		m_MissileTier = MissileTier::TIER1;
		break;
	}

	LOG_APP("UPDATED MISSILE", Engine::LOG_ERROR);
}

void Player::OnDie()
{
	lifePoints--;
	if (lifePoints == 0) {
		LOG_APP("YOU LOST", Engine::LOG_ERROR);
	}
	else {
		m_Health->AddHealth(100);
		GameManager::GetManager().GetUIManager().SetLifePoints(lifePoints);
		GameManager::GetManager().GetUIManager().UpdatePlayerHealth(100);
	}
}

void Player::OnContactEvent(Object* other)
{
	__super::OnContactEvent(other);

	if (other->HasTag("Enemy")) 
		TakeDamage(10);
}

void Player::OnEndContactEvent(Object* other)
{
	__super::OnEndContactEvent(other);
}

void Player::SpawnCloneCompanion()
{
	m_Clone = GameManager::GetManager().InstantiateObject<Clone>(glm::vec3(m_Transform->Position.x + 128, m_Transform->Position.y, 0.0f), CloneType::CloneCompanion);
}

void Player::TakeDamage(float m_Damage)
{
	m_Health->TakeDamage(m_Damage);
	GameManager::GetManager().GetUIManager().UpdatePlayerHealth(m_Health->GetHealth());
}

#pragma once
#include "Engine/Core/Actor.h"
#include "Engine/Core/Components.h"
#include "Engine/Core/Component.h"
#include "Engine/Core/Input.h"
#include "Engine/Core/AudioSystem.h"
#include "Missile.h"
#include "Clone.h"

class Player : public Engine::Actor
{
public:
	Player();
	virtual ~Player() {}

	void Start() override;
	void Update(float deltaTime) override;

	void OnDie();

	void UpgradeMissileTier();

	void OnContactEvent(Object* other) override;
	void OnEndContactEvent(Object* other) override;

	void SpawnCloneCompanion();
	void TakeDamage(float m_Damage);

private:
	Engine::TransformComponent* m_Transform;
	Engine::SpriteRenderer2D* m_SpriteRenderer;
	Engine::AnimatorComponent* m_Animator;
 	Engine::Rigidbody2D* m_RigidBody2d;
 	Engine::BoxCollider2DComponent* m_Collider;
 	Engine::HealthComponent* m_Health;
	int lifePoints = 5;

	float m_FireRate = 0.2f;
	float m_NexShotTime = 0.0f;
	float m_CurrentTime = 0.0f;

	float lastY = -2;

	MissileTier m_MissileTier;

	Clone* m_Clone;
};


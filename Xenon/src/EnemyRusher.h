#pragma once
#include "Engine/Core/Actor.h"
#include "Engine/Core/Components.h"

class EnemyRusher : public Engine::Actor
{
public:
	EnemyRusher(glm::vec2 spawnPos);


	void Start() override;


	void Update(float deltaTime) override;


	void OnContactEvent(Object* other) override;


	void OnEndContactEvent(Object* other) override;

	void OnDie();

private:
	Engine::TransformComponent* m_Transform;
	Engine::SpriteRenderer2D* m_SpriteRenderer;
	Engine::AnimatorComponent* m_Animator;
	Engine::Rigidbody2D* m_RigidBody;
	Engine::BoxCollider2DComponent* m_Collider;
	Engine::HealthComponent* m_Health;

	float m_Damage = 20.0f;
	float m_YVelocity = 120.0f;
};


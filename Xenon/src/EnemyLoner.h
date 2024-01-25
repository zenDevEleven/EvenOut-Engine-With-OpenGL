#pragma once
#include "Engine/Core/Actor.h"
#include "Engine/Core/Components.h"
#include "LonerBullet.h"

class EnemyLoner : public Engine::Actor
{
public:
	EnemyLoner(glm::vec2 SpawnPosition);

	virtual ~EnemyLoner() {};
	void Start() override;


	void Update(float deltaTime) override;

	void OnContactEvent(Object* other) override;

	void OnEndContactEvent(Object* other) override;
private:
	float lastDeltaTime = 0;

	bool PatrolSideRight = true;

	float m_VelocityY = 35.0f;
	float m_VelocityX = 15.0f;

	float m_ShootInterval = 5.0f;

	float m_Timer = 0;

	Engine::TransformComponent* m_Transform;
	Engine::SpriteRenderer2D* m_SpriteRenderer;
	Engine::AnimatorComponent* m_Animator;
	Engine::Rigidbody2D* m_RigidBody;
	Engine::BoxCollider2DComponent* m_Collider;

};
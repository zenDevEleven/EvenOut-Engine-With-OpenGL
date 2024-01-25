#pragma once
#include "Engine/Core/Actor.h"
#include "Engine/Core/Components.h"

class LonerBullet : public Engine::Actor
{
public:
	LonerBullet() {}
	LonerBullet(glm::vec2 spawnPos, glm::vec2 direction);

	void Start() override;


	void Update(float deltaTime) override;


	void OnContactEvent(Object* other) override;


	void OnEndContactEvent(Object* other) override;

private:
	Engine::TransformComponent* m_Transform;
	Engine::SpriteRenderer2D* m_SpriteRenderer;
	Engine::AnimatorComponent* m_Animator;
	Engine::Rigidbody2D* m_RigidBody;
	Engine::BoxCollider2DComponent* m_Collider;

	float m_Damage = 15.0f;
	float m_BulletSpeed = 60.0f;

	glm::vec2 m_UnitDirectionVector;
};


#pragma once
#include "Engine/Core/Actor.h"
#include "Engine/Core/Components.h"

enum class MissileTier {
	TIER1,
	TIER2,
	TIER3
};


class Missile : public Engine::Actor
{
public:
	Missile(float missileSpeed, glm::vec2 spawnPos, MissileTier tier);
	virtual ~Missile() {};

	void Start() override;
	void Update(float deltaTime) override;

	void OnContactEvent(Object* other) override;


	void OnEndContactEvent(Object* other) override;

private:
	Engine::TransformComponent* m_Transform;
	Engine::SpriteRenderer2D* m_SpriteRenderer;
	Engine::AnimatorComponent* m_Animator;
	Engine::Rigidbody2D* m_Rigidbody;
	Engine::BoxCollider2DComponent* m_BoxCollider;

	float m_MissileSpeed;
	float m_MissileLifeTime = 0.8f;
	float m_Time = 0.0f;

	MissileTier m_Tier;

	float m_MissileDamage = 0.0f;

};


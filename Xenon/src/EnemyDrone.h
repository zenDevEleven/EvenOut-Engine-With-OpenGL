#pragma once
#include "Engine/Core/Actor.h"
#include "Engine/Core/Components.h"

class EnemyDrone : public Engine::Actor
{
public:
	EnemyDrone(float PositionX, float PositionY);

	virtual ~EnemyDrone() {};
	void Start() override;


	void Update(float deltaTime) override;

	void OnContactEvent(Object* other) override;


	void OnEndContactEvent(Object* other) override;
private:
	float m_PositionX;
	float m_PositionY;

	float lastDeltaTime = 0;

	float m_VelocityX = 20.0f;
	float m_VelocityY = 520.0f;

	float m_YThreshold = 2000.0f;

	Engine::TransformComponent* m_Transform;
	Engine::SpriteRenderer2D* m_SpriteRenderer;
	Engine::AnimatorComponent* m_Animator;
	Engine::Rigidbody2D* m_RigidBody;
	Engine::BoxCollider2DComponent* m_Collider;

};


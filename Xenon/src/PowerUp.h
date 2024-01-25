#pragma once
#include "Engine/Core/Actor.h"
#include "Engine/Core/Components.h"

enum class PowerUpType {
	MISSILE_PU,
	LIFE_PU,
	SPEED_PU,
	SHIEL_PU
};
class PowerUp : public Engine::Actor
{
public:
	PowerUp(PowerUpType type, glm::vec2 SpawnPos);

	void Start() override;


	void Update(float deltaTime) override;


	void OnContactEvent(Object* other) override;


	void OnEndContactEvent(Object* other) override;
private:
	Engine::TransformComponent* m_Transform;
	Engine::SpriteRenderer2D* m_SpriteRenderer;
	Engine::AnimatorComponent* m_Animator;
	Engine::Rigidbody2D* m_Rigidbody;
	Engine::BoxCollider2DComponent* m_Collider;

	PowerUpType m_Type;
	glm::vec2 m_Position;
};


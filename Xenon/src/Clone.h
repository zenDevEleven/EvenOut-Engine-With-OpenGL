#pragma once
#include "Engine/Core/Actor.h"
#include "Engine/Core/Components.h"
#include "Missile.h"

enum class CloneType {
	ClonePowerUp,
	CloneCompanion,
};

class Clone : public Engine::Actor
{
public:
	Clone(glm::vec3 ClonePosition, CloneType type);
	void Start() override;
	void Update(float deltaTime) override;
	void OnDie();

	void SetClonePosition(glm::vec3 ClonePosition);
	void ShootMissile(MissileTier tier);

	void OnContactEvent(Object* other) override;


	void OnEndContactEvent(Object* other) override;

private:
	Engine::SpriteRenderer2D* m_SpriteRenderer;
	Engine::TransformComponent* m_Transform;
	Engine::AnimatorComponent* m_Animator;
	Engine::BoxCollider2DComponent* m_Collider;
	Engine::Rigidbody2D* m_RigidBody;
	Engine::HealthComponent* m_Health;

	CloneType m_SpawnedType;

};


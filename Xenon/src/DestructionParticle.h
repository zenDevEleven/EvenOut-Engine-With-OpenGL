#pragma once
#include "Engine/Core/Actor.h"
#include "Engine/Core/Components.h"

class DestructionParticle : public Engine::Actor
{
public:
	DestructionParticle(glm::vec2 spawnPosition);

	void Start() override;


	void Update(float deltaTime) override;


	void OnContactEvent(Object* other) override;


	void OnEndContactEvent(Object* other) override;

private:
	Engine::TransformComponent* m_Transform;
	Engine::SpriteRenderer2D* m_SpriteRenderer;
	Engine::AnimatorComponent* m_Animator;

	glm::vec2 m_Position;

};


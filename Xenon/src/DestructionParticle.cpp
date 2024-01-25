#include "DestructionParticle.h"

using Frame = Engine::AnimatorComponent::AnimationFrame;
using Animation = Engine::AnimatorComponent::Animation;

DestructionParticle::DestructionParticle(glm::vec2 spawnPosition)
{
	m_Position = spawnPosition;

	m_Transform = &AddComponent<Engine::TransformComponent>(m_Position);
	m_SpriteRenderer = &AddComponent<Engine::SpriteRenderer2D>("Assets/Textures/explode16.bmp", 16, 1);
	m_Animator = &AddComponent<Engine::AnimatorComponent>(m_SpriteRenderer->m_SpriteTexture);

	m_SpriteRenderer->SetRotation(90.0f);
}

void DestructionParticle::Start()
{
	__super::Start();
	m_Animator->CreateAnimation(new Animation("IdleExplode", std::vector<Frame>{
		Frame(1, 1),
		Frame(2, 5)
	}));
}

void DestructionParticle::Update(float deltaTime)
{
	__super::Update(deltaTime);
	m_Animator->PlayAnimationContiniousToFrame("IdleExplode", 0.03f, deltaTime, false);

	if (!m_Animator->IsPlayingAnimation("IdleExplode")) {
		Destroy();
	}
}

void DestructionParticle::OnContactEvent(Object* other)
{
}

void DestructionParticle::OnEndContactEvent(Object* other)
{
	
}

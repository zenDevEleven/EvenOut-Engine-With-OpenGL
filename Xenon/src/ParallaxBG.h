#pragma once
#include "Engine/Core/Object.h"
#include "Engine/Core/Components.h"

class ParallaxBG : public Engine::Object
{
public:
	ParallaxBG() {}

	void Start() override;

	void Update(float deltaTime) override;


	void OnContactEvent(Object* other) override;


	void OnEndContactEvent(Object* other) override;

private:
	float m_ScollingSpeed = 30.0f;

	std::vector<Engine::SpriteRenderer2D*> m_SpriteRenderers;
	int m_SpriteRendererIndex = 0;

	Engine::TransformComponent* m_Transform;
};


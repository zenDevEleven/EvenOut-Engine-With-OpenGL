#include "Background.h"
#include "Global.h"

void Background::Start()
{
	LOG_APP("Loading background texture", Engine::LOG_INFO);
	m_Transform = AddComponent<Engine::TransformComponent>(glm::vec2(0.0f, 0.0f));
	m_SpriteRenderer = AddComponent<Engine::SpriteRenderer2D>("Assets/Textures/galaxy2.bmp", glm::vec2(640,  480), 0);
	m_SpriteRenderer.SetRotation(90.0f);
}

void Background::Update(float deltaTime)
{
}

void Background::OnContactEvent(Object* other)
{
}

void Background::OnEndContactEvent(Object* other)
{
	
}


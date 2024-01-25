#include "ParallaxBG.h"
#include "Global.h"

void ParallaxBG::Start()
{
	__super::Start();

	m_Transform = &AddComponent<Engine::TransformComponent>();

	m_SpriteRenderers.push_back(&AddComponent<Engine::SpriteRenderer2D>("Assets/Textures/rocksBg.bmp", glm::vec2(640, 1780), 0));
	m_SpriteRenderers.push_back(&AddComponent<Engine::SpriteRenderer2D>("Assets/Textures/rocks.bmp", glm::vec2(640, 1780), 0));

	m_Transform->SetPosition(Global::WIDTH + m_SpriteRenderers[m_SpriteRendererIndex]->m_SpriteTexture->GetHeightF(), 0);

	for (int i = 0; i < m_SpriteRenderers.size(); ++i)
	{
		m_SpriteRenderers[i]->SetRotation(90.0f);
		m_SpriteRenderers[i]->SetIsActive(false);
	}

	m_SpriteRenderers[m_SpriteRendererIndex]->SetIsActive(true);
}

void ParallaxBG::Update(float deltaTime)
{
	__super::Update(deltaTime);

	
	if (m_Transform->Position.x <= 0) {
		m_SpriteRenderers[m_SpriteRendererIndex]->SetIsActive(false);

		m_SpriteRendererIndex++;
		if (m_SpriteRendererIndex >= m_SpriteRenderers.size()) {
			m_SpriteRendererIndex = 0;
		}
		m_SpriteRenderers[m_SpriteRendererIndex]->SetIsActive(true);

		m_Transform->SetPosition(Global::WIDTH + m_SpriteRenderers[m_SpriteRendererIndex]->m_SpriteTexture->GetHeightF(), 0);
	}
	else
	{
		m_Transform->AddPos(-m_ScollingSpeed * deltaTime, 0);
	}
}

void ParallaxBG::OnContactEvent(Object* other)
{
	
}

void ParallaxBG::OnEndContactEvent(Object* other)
{
	
}

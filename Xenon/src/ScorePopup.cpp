#include "ScorePopup.h"

ScorePopup::ScorePopup(glm::vec2 SpawnPosition, int scoreToShow, UIManager& manager)
{
	m_Position = SpawnPosition;
	m_ScoreToDraw = scoreToShow;
	m_Text = &AddComponent<Engine::TextRendererComponent>("Assets/Textures/font8x8.bmp", manager.GetFontData(), std::to_string(scoreToShow), SpawnPosition);
}

void ScorePopup::Start()
{
	__super::Start();
}

void ScorePopup::Update(float deltaTime)
{
	__super::Update(deltaTime);
	m_Time += deltaTime;
	if (m_Time >= m_LifeTime)
		Destroy();
	else
		m_Text->AddPosition(glm::vec2(0.0f, m_Position.y * -m_VelocityY * deltaTime));
}

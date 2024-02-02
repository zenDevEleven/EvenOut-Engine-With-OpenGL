#pragma once
#include "Engine/Core/Components.h"
#include "Engine/Core/UIObject.h"
#include "UIManager.h"

class ScorePopup : public Engine::UIObject
{
public:
	ScorePopup() {}
	ScorePopup(glm::vec2 SpawnPosition, int scoreToShow, UIManager& manager);
	void Start() override;
	void Update(float deltaTime) override;

private:
	Engine::TextRendererComponent* m_Text;

	float m_VelocityY = 0.05f;
	float m_LifeTime = 1.0f;
	float m_Time = 0.0f;

	int m_ScoreToDraw = 0;
	glm::vec2 m_Position;
};


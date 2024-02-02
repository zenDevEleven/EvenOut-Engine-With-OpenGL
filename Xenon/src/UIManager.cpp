#include "UIManager.h"
#include "Global.h"

#include "Engine/Core/DataSaver.h"
#include "GameManager.h"
#include "ScorePopup.h"

UIManager::UIManager()
{
	PlayerRender = &AddComponent<Engine::TextRendererComponent>("Assets/Textures/font8x8.bmp", fontData, "player one", glm::vec2(5.0f, 4.0f));
	ScoreRender = &AddComponent<Engine::TextRendererComponent>("Assets/Textures/font16x16.bmp", fontData, scoreTxt, glm::vec2(5.0f, 12.0f));
	HighestScoreRender = &AddComponent<Engine::TextRendererComponent>("Assets/Textures/font8x8.bmp", fontData, "Highest Score:", glm::vec2((Global::WIDTH / 2) - (4 * 14), 4.0f));
	HighestScoreTextRender = &AddComponent<Engine::TextRendererComponent>("Assets/Textures/font8x8.bmp", fontData, highestScoreTxt, glm::vec2((Global::WIDTH / 2) - (4 * (strlen(highestScoreTxt.c_str()) - 1)), 12.0f));
	
	HealthbarBorder = &AddComponent<Engine::ImageRendererComponent>("Assets/Textures/HealthBar_Border.bmp", glm::vec2(5.0f, Global::HEIGHT - 16));
	HealthbarFill = &AddComponent<Engine::ImageRendererComponent>("Assets/Textures/HealthBar_Fill.bmp", glm::vec2(5.0f, Global::HEIGHT - 16));
	lifeShipImage = &AddComponent<Engine::ImageRendererComponent>("Assets/Textures/Ship1.bmp", glm::vec2(5.0f, Global::HEIGHT - 70));
}

void UIManager::Start()
{
	__super::Start();

	lifeShipImage->SetSize({ 64.0f, 64.0f });
	lifeShipImage->SetPosition({ (64.0f * 3), 64.0f });
	lifeShipImage->SetScale(.5f);

	if (Engine::DataSaver::RegistryValueExists<int>("HighScore")) {
		lastHighScore = Engine::DataSaver::GetKey<int>("HighScore");
		highestScoreTxt = std::to_string(lastHighScore);
	}
	else {
		lastHighScore = 0;
		highestScoreTxt = std::to_string(lastHighScore);
	}

	RenderTextScore(highestScoreTxt, HighestScoreTextRender);

}

void UIManager::Update(float deltaTime)
{
	__super::Update(deltaTime);

	if (playerScore > lastHighScore) {
		lastHighScore = playerScore;
		highestScoreTxt = std::to_string(lastHighScore);
		Engine::DataSaver::SaveInt("HighScore", lastHighScore);
		RenderTextScore(highestScoreTxt, HighestScoreTextRender);
	}
}

void UIManager::Draw()
{
	__super::Draw();
}

void UIManager::AddPlayerScore(int scoreToAdd)
{
	playerScore += scoreToAdd;
	scoreTxt = std::to_string(playerScore);

	RenderTextScore(scoreTxt, ScoreRender);
}

void UIManager::UpdatePlayerHealth(int newHealth)
{
	int texWidth = HealthbarFill->m_ImageTexture->GetWidth();
	int newXValue = (newHealth * texWidth) / 100;

	HealthbarFill->SetSize({ newXValue , HealthbarFill->m_ImageTexture->GetHeight() });
}

void UIManager::SpawnScorePopUp(int scoreToDraw, glm::vec2 Position)
{
	GameManager::GetManager().InstantiateObject<ScorePopup>(Position, scoreToDraw, *this);
}

void UIManager::RenderTextScore(std::string TextToRender, Engine::TextRendererComponent* component) {
	std::string tmpText = TextToRender;

	if (strlen(TextToRender.c_str()) != zerosInScore)
	{
		int zerosToAdd = zerosInScore - strlen(TextToRender.c_str());

		for (int i = 0; i < zerosToAdd; i++)
		{
			tmpText.insert(0, 1, '0');
		}
	}

	component->SetText(tmpText);

}

void UIManager::SetLifePoints(int lifePoints)
{
	playerLifePoints = lifePoints;

	lifeShipImage->SetFlexibility(true, lifePoints);
	lifeShipImage->SetTiling(32, Engine::ImageRendererComponent::TilingDirection::RIGHT);
}


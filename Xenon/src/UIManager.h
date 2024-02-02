#pragma once
#include "Engine/Core/UIObject.h"
#include "Engine/Core/Components.h"
#include "Engine/Core/Texture.h"
#include "Engine/LogSystem/Log.h"
#include <random>

class UIManager : public Engine::UIObject
{
public:
	UIManager();


	void Start() override;


	void Update(float deltaTime) override;


	void Draw() override;

	void AddPlayerScore(int newScore);
	void UpdatePlayerHealth(int newHealth);
	void SpawnScorePopUp(int scoreToDraw, glm::vec2 Position);
	void RenderTextScore(std::string TextToRender, Engine::TextRendererComponent* component);
	void SetLifePoints(int lifePoints);

	std::vector<char> GetFontData() { return fontData; }

private:
	int lastHighScore;
	int playerScore = 0;
	int zerosInScore = 10;
	
	int playerLifePoints;

	Engine::TextRendererComponent* ScoreRender;
	Engine::TextRendererComponent* PlayerRender;
	Engine::TextRendererComponent* HighestScoreRender;
	Engine::TextRendererComponent* HighestScoreTextRender;

	Engine::ImageRendererComponent* HealthbarBorder;
	Engine::ImageRendererComponent* HealthbarFill;

	Engine::ImageRendererComponent* lifeShipImage;
	
	std::string scoreTxt = "0000000000";
	std::string highestScoreTxt = "0000000000";


	std::vector<char> fontData = { ' ' , '!' , '"', '#', '$', '%', '&', ' ',
								   '(',  ')',  '*', '+', ',', '-', '.', '/',
								   '0',  '1',  '2', '3', '4', '5', '6', '7',
								   '8',  '9',  ':', ';', '<', '=', '>', '?',
								   '@',  'A',  'B', 'C', 'D', 'E', 'F', 'G',
								   'H',  'I',  'J', 'K', 'L', 'M', 'N', 'O',
								   'P',  'Q',  'R', 'S', 'T', 'U', 'V', 'W',
								   'X',  'Y',  'Z', '[', '~', ']', '^', '_',
								   ' ',  'a',  'b', 'c', 'd', 'e', 'f', 'g',
								   'h',  'i',  'j', 'k', 'l', 'm', 'n', 'o',
								   'p',  'q',  'r', 's', 't', 'u', 'v', 'w',
								   'x',  'y',  'z', '{', ' ', '}', ' ', ' ' };
};


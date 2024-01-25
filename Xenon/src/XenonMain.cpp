#include "Engine/Core/GameEngine.h"
#include "Engine/Core/GameWorld.h"
#include "GameManager.h"
#include "Global.h"

int main(int argc, char** argv) {
	Engine::GameEngine engine;

	engine.Initialize("EventOut Engine", Global::WIDTH, Global::HEIGHT);
	engine.GetWorld().CreateActor<GameManager>();
	engine.Run();
}
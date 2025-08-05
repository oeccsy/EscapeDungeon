#include "Game.h"
#include "Level/DungeonLevel.h"

Game::Game() { }

Game::~Game() { }

void Game::Run()
{
	engine.AddLevel(new DungeonLevel);
	engine.Run();
}
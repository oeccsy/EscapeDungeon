#include "Game.h"
#include "Level/ConnectLevel.h"

Game::Game() { }

Game::~Game() { }

void Game::Run()
{
	engine.AddLevel(new ConnectLevel());
	engine.Run();
}
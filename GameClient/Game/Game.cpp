#include "Game.h"
#include "Level/ConnectLevel.h"
#include "Level/DungeonLevel.h"

Game* Game::instance = nullptr;

Game::Game()
{
	instance = this;
}

Game::~Game() { }

void Game::LoadDungeonLevel()
{
	system("cls");
	engine.AddLevel(new DungeonLevel());
}

void Game::Run()
{
	engine.AddLevel(new ConnectLevel());
	engine.Run();
}

Game& Game::Get()
{
	return *instance;
}
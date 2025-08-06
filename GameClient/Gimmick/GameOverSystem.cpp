#include "GameOverSystem.h"
#include "Actor/Exit.h"
#include "Utils/Utils.h"

#include <iostream>

GameOverSystem::GameOverSystem() : playerCount(1) {}
GameOverSystem::GameOverSystem(int playerCount) : playerCount(playerCount) { }

void GameOverSystem::CheckGameOver()
{
	if (playerCount == Exit::escapeCount) GameOver();
}

void GameOverSystem::GameOver()
{
	Utils::SetConsolePosition(Vector2(100, 30));
	Utils::SetConsoleTextColor(Color::White);
	std::cout << "Game Over!";
}

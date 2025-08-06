#include "GameOverSystem.h"
#include "Actor/Player.h"
#include "Utils/Utils.h"

#include <iostream>

GameOverSystem::GameOverSystem() : playerCount(1) {}
GameOverSystem::GameOverSystem(int playerCount) : playerCount(playerCount) { }

void GameOverSystem::CheckGameOver()
{
	if (playerCount == Player::escapeCount + Player::deadCount) GameOver();
}

void GameOverSystem::GameOver()
{
	Utils::SetConsolePosition(Vector2(100, 30));
	Utils::SetConsoleTextColor(Color::White);
	std::cout << "Game Over!";
}

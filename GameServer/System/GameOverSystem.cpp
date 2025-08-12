#include "GameOverSystem.h"
#include "Actor/Player.h"
#include "Utils/Utils.h"
#include "Utils/Logs.h"

#include "Engine.h"

#include <iostream>

bool GameOverSystem::isGameOver = false;

void GameOverSystem::CheckGameOver()
{
	if (Player::playerCount == Player::escapeCount + Player::deadCount) GameOver();
}

void GameOverSystem::GameOver()
{
	isGameOver = true;
	Logs::Get().AddLog({ "==== 게임 종료 ====" });
	Engine::Get().Quit();
}

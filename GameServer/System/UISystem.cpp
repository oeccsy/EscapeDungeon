#include "UISystem.h"
#include "Actor/Player.h"
#include "Actor/Monster.h"
#include "Utils/Utils.h"

#include <iostream>

void UISystem::InitLogArea()
{
	Utils::SetConsoleTextColor(Color::White);

	Utils::SetConsolePosition(Vector2(90, 7));
	std::cout << "■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■\n";

	for (int i = 1; i <= 30; ++i)
	{
		Utils::SetConsolePosition(Vector2(90, 38 - i));
		std::cout << "■                                        ■" << '\n';
	}

	Utils::SetConsolePosition(Vector2(90, 38));
	std::cout << "■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■\n";
}

void UISystem::RenderPlayerCount(int playerCount)
{
	Utils::SetConsoleTextColor(Color::White);
	Utils::SetConsolePosition(Vector2(10, 30));
	std::cout << "플레이어 수 : " << playerCount;
}

void UISystem::RenderStaminaUI(std::vector<Player*> players, Monster* monster)
{
	Utils::SetConsoleTextColor(Color::White);
	Utils::SetConsolePosition(Vector2(90, 0));
	std::cout << "[플레이어 스태미나]";

	for (int i = 0; i < players.size(); ++i)
	{
		char playerStaminaText[20] = { };
		playerStaminaText[Player::MAX_STAMINA] = '\0';

		Player* player = players[i];

		for (int j = 0; j < Player::MAX_STAMINA; ++j)
		{
			playerStaminaText[j] = (j < player->GetStamina()) ? 'O' : ' ';
		}

		Utils::SetConsolePosition(Vector2(90, 1 + i));
		std::cout << "Player " << (i + 1) << " : " << playerStaminaText;
	}


	if (monster == nullptr) return;

	Utils::SetConsolePosition(Vector2(90, 5));
	std::cout << "[몬스터 스태미나]";
	
	char monsterStaminaText[20] = { };
	monsterStaminaText[Monster::MAX_STAMINA] = '\0';

	for (int i = 0; i < Monster::MAX_STAMINA; i++)
	{
		monsterStaminaText[i] = (i < monster->GetStamina()) ? 'X' : ' ';
	}

	Utils::SetConsolePosition(Vector2(90, 6));
	std::cout << "Monster : " << monsterStaminaText;
}

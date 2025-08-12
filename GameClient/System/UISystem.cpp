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

void UISystem::InitDesc()
{
	Utils::SetConsoleTextColor(Color::White);
	Utils::SetConsolePosition(Vector2(10, 10));
	std::cout << "[게임 방법] " << '\n';

	Utils::SetConsolePosition(Vector2(10, 11));
	std::cout << "이동 : 방향키(위, 아래, 왼쪽, 오른쪽)" << '\n';
	Utils::SetConsolePosition(Vector2(10, 12));
	std::cout << "게임 시작 : Enter" << '\n';


	Utils::SetConsolePosition(Vector2(10, 14));
	std::cout << "[플레이어 목표] " << '\n';
	Utils::SetConsolePosition(Vector2(10, 15));
	std::cout << "잠금장치를 해제하기		* T : 잠금장치" << '\n';
	Utils::SetConsolePosition(Vector2(10, 16));
	std::cout << "탈출구로 탈출하기		* E : 탈출구" << '\n';
	Utils::SetConsolePosition(Vector2(10, 17));
	std::cout << "몬스터 피하기			* M : 몬스터" << '\n';

	Utils::SetConsolePosition(Vector2(10, 19));
	std::cout << "[몬스터 목표] " << '\n';
	Utils::SetConsolePosition(Vector2(10, 20));
	std::cout << "플레이어 잡기			* P : 플레이어" << '\n';
}

void UISystem::RenderPlayerCount(int playerCount, int reqPlayerCount)
{
	Utils::SetConsoleTextColor(Color::White);
	Utils::SetConsolePosition(Vector2(10, 30));
	std::cout << "필요한 플레이어 수 : " << reqPlayerCount;
	Utils::SetConsolePosition(Vector2(10, 32));
	std::cout << "현재 플레이어 수 : " << playerCount;
}

void UISystem::RenderStaminaUI(Player* player)
{
	if (player == nullptr) return;

	Utils::SetConsoleTextColor(Color::White);
	Utils::SetConsolePosition(Vector2(90, 1));
	std::cout << "[플레이어 스태미나]";

	char playerStaminaText[20] = { };
	playerStaminaText[Player::MAX_STAMINA] = '\0';

	for (int i = 0; i < Player::MAX_STAMINA; ++i)
	{
		playerStaminaText[i] = (i < player->GetStamina()) ? 'O' : ' ';
	}

	Utils::SetConsolePosition(Vector2(90, 2));
	std::cout << "Player : " << playerStaminaText;
}

void UISystem::RenderStaminaUI(Monster* monster)
{
	if (monster == nullptr) return;

	Utils::SetConsoleTextColor(Color::White);
	Utils::SetConsolePosition(Vector2(90, 1));
	std::cout << "[몬스터 스태미나]";

	char monsterStaminaText[20] = { };
	monsterStaminaText[Monster::MAX_STAMINA] = '\0';

	for (int i = 0; i < Monster::MAX_STAMINA; ++i)
	{
		monsterStaminaText[i] = (i < monster->GetStamina()) ? 'X' : ' ';
	}

	Utils::SetConsolePosition(Vector2(90, 2));
	std::cout << "Monster : " << monsterStaminaText;
}
#pragma once

#include "Actor/Player.h"
#include "Actor/Monster.h"

#include <vector>

class UISystem
{
public:
	UISystem() = default;
	~UISystem() = default;

	void InitLogArea();
	void RenderStaminaUI(Player* players);
	void RenderStaminaUI(Monster* monster);
};
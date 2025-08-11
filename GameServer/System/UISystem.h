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
	void RenderStaminaUI(std::vector<Player*> players, Monster* monster);
};
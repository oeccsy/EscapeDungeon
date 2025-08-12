#pragma once

#include "Level/Level.h"
#include <vector>

class Task;
class Player;
class Exit;
class Monster;

class InteractionSystem
{
public:
	InteractionSystem() = default;
	~InteractionSystem() = default;

	void ProgressTask(std::vector<Task*>& tasks, std::vector<Player*>& players, float deltaTime);
	void CheckOpenExit(Level& level, int count);
	void EscapePlayer(std::vector<Exit*>& exits, std::vector<Player*>& players);
	void KillPlayer(Monster* monster, std::vector<Player*>& players);
};
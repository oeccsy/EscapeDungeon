#pragma once

#include "Level/Level.h"
#include <vector>

class Task;
class Player;
class Exit;

class ExitSystem
{
public:
	ExitSystem() = default;
	~ExitSystem() = default;

	void ProgressTask(std::vector<Task*>& tasks, std::vector<Player*>& players, float deltaTime);
	void CheckOpenExit(Level& level, int count);
	void EscapePlayer(std::vector<Exit*>& exits, std::vector<Player*>& players);

private:
	bool isExitExist = false;
};
#pragma once

#include "Level/Level.h"
#include "Actor/Task.h"
#include "Actor/Player.h"

class ExitSystem
{
public:
	ExitSystem() = default;
	~ExitSystem() = default;

	void ProgressTask(std::vector<Task*>& tasks, std::vector<Player*>& players, float deltaTime);
	void CreateExit(Level& level, int count);

private:
	

};
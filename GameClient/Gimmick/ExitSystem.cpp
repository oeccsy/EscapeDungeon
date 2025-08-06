#include "ExitSystem.h"

#include "Actor/Task.h"
#include "Actor/Player.h"
#include "Actor/Exit.h"

#include <vector>

void ExitSystem::ProgressTask(std::vector<Task*>& tasks, std::vector<Player*>& players, float deltaTime)
{
	for (auto task : tasks)
	{
		for (auto player : players)
		{
			if (task->Intersects(player))
			{
				task->ProgressTask(deltaTime);
			}
			
		}
	}
}

void ExitSystem::CheckOpenExit(Level& level, int count)
{
	if (Task::completedTaskCount < Task::REQUIRED_TASK_COUNT) return;
	if (isExitExist) return;

	level.AddActor(new Exit({ 30, 10 }));
	isExitExist = true;
}

void ExitSystem::EscapePlayer(std::vector<Exit*>& exits, std::vector<Player*>& players)
{
	for (auto exit : exits)
	{
		for (auto player : players)
		{
			if (exit->GetPosition() == player->GetPosition()) exit->Escape(player);
		}
	}
}

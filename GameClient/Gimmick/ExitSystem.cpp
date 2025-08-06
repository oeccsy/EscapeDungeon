#include "ExitSystem.h"

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

void ExitSystem::CreateExit(Level& level, int count)
{

}

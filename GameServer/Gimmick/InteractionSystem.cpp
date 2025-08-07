#include "InteractionSystem.h"

#include "Actor/Task.h"
#include "Actor/Player.h"
#include "Actor/Exit.h"
#include "Actor/Monster.h"

#include <vector>

void InteractionSystem::ProgressTask(std::vector<Task*>& tasks, std::vector<Player*>& players, float deltaTime)
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

void InteractionSystem::CheckOpenExit(Level& level, int count)
{
	if (Task::completedTaskCount < Task::REQUIRED_TASK_COUNT) return;
	if (isExitExist) return;

	level.AddActor(new Exit({ 30, 10 }));
	isExitExist = true;
}

void InteractionSystem::EscapePlayer(std::vector<Exit*>& exits, std::vector<Player*>& players)
{
	for (auto exit : exits)
	{
		for (auto player : players)
		{
			if (exit->GetPosition() == player->GetPosition()) player->Escape();
		}
	}
}

void InteractionSystem::KillPlayer(Monster* monster, std::vector<Player*>& players)
{
	if (monster == nullptr) return;
	
	for (auto player : players)
	{
		if (monster->Intersects(player->GetPosition())) monster->Kill(player);
	}
}

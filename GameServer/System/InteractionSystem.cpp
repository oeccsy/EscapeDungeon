#include "InteractionSystem.h"

#include "Actor/Task.h"
#include "Actor/Player.h"
#include "Actor/Exit.h"
#include "Actor/Monster.h"

#include "Utils/Logs.h"

#include "Networking/Command.h"
#include "Networking/Server.h"

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
	if (Exit::isExitOpen) return;

	Exit::isExitOpen = true;
	Logs::Get().AddLog({ "탈출구가 열렸습니다!" });
}

void InteractionSystem::EscapePlayer(std::vector<Exit*>& exits, std::vector<Player*>& players)
{
	for (auto exit : exits)
	{
		for (auto player : players)
		{
			if (exit->GetPosition() == player->GetPosition())
			{
				player->Escape();

				Command packet;
				packet.data[0] = 'e';
				packet.data[1] = player->GetActorID();

				Server::Get().writeQueue.push(packet);
			}
		}
	}
}

void InteractionSystem::KillPlayer(Monster* monster, std::vector<Player*>& players)
{
	if (monster == nullptr) return;
	
	for (auto player : players)
	{
		if (monster->Intersects(player->GetPosition()))
		{
			monster->Kill(player);

			Command packet;
			packet.data[0] = 'k';
			packet.data[1] = player->GetActorID();

			Server::Get().writeQueue.push(packet);
		}
	}
}

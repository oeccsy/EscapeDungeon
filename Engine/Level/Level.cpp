#include "Level.h"
#include "Actor/Actor.h"
#include "Utils/Utils.h"

#include <iostream>

Level::Level() {}

Level::~Level()
{
	for (Actor* actor : actors)
	{
		SafeDelete(actor);
	}

	actors.clear();
}

void Level::AddActor(Actor* newActor)
{
	addRequestedActors.emplace_back(newActor);
}

void Level::DestroyActor(Actor* destroyedActor)
{
	destroyRequestedActors.emplace_back(destroyedActor);
}

void Level::BeginPlay()
{
	for (Actor* const actor : actors)
	{
		if (!actor->isActive || actor->isExpired) continue;
		if (actor->HasBeganPlay()) continue;

		actor->BeginPlay();
	}
}

void Level::Tick(float deltaTime)
{
	for (Actor* const actor : actors)
	{
		if (!actor->isActive || actor->isExpired) continue;

		actor->Tick(deltaTime);
	}
}

void Level::Render()
{
	SortActorsBySortingOrder();

	for (Actor* const actor : actors)
	{
		if (!actor->isActive || actor->isExpired) continue;

		Actor* searchedActor = nullptr;
		
		for (Actor* const otherActor : actors)
		{
			if (actor == otherActor) continue;
			
			if (actor->GetPosition() == otherActor->GetPosition())
			{
				if (actor->sortingOrder < otherActor->sortingOrder)
				{
					searchedActor = otherActor;
					break;
				}
			}
		}

		if (searchedActor) continue;

		actor->Render();
	}
}

void Level::ProcessAddAndDestroyActors()
{
	for (auto iterator = actors.begin(); iterator != actors.end();)
	{
		if ((*iterator)->isExpired)
		{
			iterator = actors.erase(iterator);
			continue;
		}

		++iterator;
	}

	for (auto* actor : destroyRequestedActors)
	{
		Utils::SetConsolePosition(actor->position);

		for (int i = 0; i < actor->width; ++i)
		{
			std::cout << " ";
		}

		SafeDelete(actor);
	}

	destroyRequestedActors.clear();


	for (auto* const actor : addRequestedActors)
	{
		actors.emplace_back(actor);
		actor->SetOwner(this);
	}

	addRequestedActors.clear();
}

void Level::SortActorsBySortingOrder()
{
	for (int i = 0; i < (int)actors.size(); ++i)
	{
		for (int j = 0; j < (int)actors.size()-1; ++j)
		{
			if (actors[j]->sortingOrder > actors[j + 1]->sortingOrder)
			{
				std::swap(actors[j], actors[j + 1]);
			}
		}
	}
}

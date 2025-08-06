#include "Exit.h"

int Exit::escapeCount = 0;

Exit::Exit(const Vector2& position) : Actor("E", Color::White, position)
{
	SetSortingOrder(2);
}

void Exit::Escape(Actor* actor)
{
	actor->Destroy();
	++escapeCount;
}


#include "Exit.h"

bool Exit::isExitOpen = false;

Exit::Exit(const Vector2& position) : Actor("E", Color::White, position)
{
	SetSortingOrder(2);
}

void Exit::Render()
{
	if (Exit::isExitOpen)
	{
		strcpy_s(this->image, 2, "E");
	}
	else
	{
		strcpy_s(this->image, 2, "#");
	}

	super::Render();
}

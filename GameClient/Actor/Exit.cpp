#include "Exit.h"

Exit::Exit(const Vector2& position) : Actor("E", Color::White, position)
{
	SetSortingOrder(2);
}
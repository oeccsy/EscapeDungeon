#include "Road.h"

Road::Road(const Vector2& position) : Actor("#", Color::White, position)
{
	SetSortingOrder(0);
}

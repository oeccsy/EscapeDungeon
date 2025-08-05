#pragma once

#include "Math/Vector2.h"

class IMovable
{
public:
	virtual bool Movable(const Vector2& targetPos) = 0;
};
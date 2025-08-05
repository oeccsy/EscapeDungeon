#pragma once

#include "Actor/Actor.h"

class Road : public Actor
{
	RTTI_DECLARATIONS(Road, Actor)

public:
	Road(const Vector2& position);
};
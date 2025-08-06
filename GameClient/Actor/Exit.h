#pragma once

#include "Actor/Actor.h"

class Exit : public Actor
{
	RTTI_DECLARATIONS(Exit, Actor)

public:
	Exit(const Vector2& position);

	void Escape(Actor* actor);

public:
	static int escapeCount;
};
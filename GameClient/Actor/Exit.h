#pragma once

#include "Actor/Actor.h"

class Exit : public Actor
{
	RTTI_DECLARATIONS(Exit, Actor)

public:
	Exit(const Vector2& position);

	virtual void Render() override;

public:
	static bool isExitOpen;
};
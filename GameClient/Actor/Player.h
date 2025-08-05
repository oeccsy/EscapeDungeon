#pragma once

#include "Actor/Actor.h"

class IMovable;
class Player : public Actor
{
	RTTI_DECLARATIONS(Player, Actor)

public:
	Player(const Vector2& position, IMovable* movable);

	virtual void BeginPlay() override;
	virtual void Tick(float deltaTime) override;

private:
	IMovable* movableInterface = nullptr;
};
#pragma once

#include "Actor/Actor.h"
#include "Utils/Timer.h"

class IMovable;
class Player : public Actor
{
	RTTI_DECLARATIONS(Player, Actor)

public:
	Player(const Vector2& position, IMovable* movable);

	virtual void BeginPlay() override;
	virtual void Tick(float deltaTime) override;

	void Move();
	int GetStamina();

private:
	void AddStamina();

public:
	static const int MAX_STAMINA = 10;

private:
	int stamina;
	Timer staminaTimer;

	IMovable* movableInterface = nullptr;
};
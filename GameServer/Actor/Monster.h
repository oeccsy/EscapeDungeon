#pragma once

#include "Actor/Actor.h"
#include "Utils/Timer.h"

class IMovable;
class Monster : public Actor
{
	RTTI_DECLARATIONS(Monster, Actor)

public:
	Monster(const Vector2& position, IMovable* movable);

	virtual void BeginPlay() override;
	virtual void Tick(float deltaTime) override;

	void Move();
	void Kill(Actor* actor);
	int GetStamina();

private:
	void AddStamina();

public:
	static const int MAX_STAMINA = 5;

private:
	int stamina;
	Timer staminaTimer;

	IMovable* movableInterface = nullptr;
};
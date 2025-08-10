#pragma once

#include "Actor/Actor.h"
#include "Utils/Timer.h"
#include "Math/Vector2.h"

class IMovable;
class Monster : public Actor
{
	RTTI_DECLARATIONS(Monster, Actor)

public:
	Monster(const Vector2& position, IMovable* movable);

	virtual void Tick(float deltaTime) override;

	void Move(Vector2 dir);
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
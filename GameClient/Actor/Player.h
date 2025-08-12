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
	void Escape();
	void Die();

	int GetStamina();
	void SetStamina(int stamina);

	void SetOwner(bool isOwner);

private:
	void AddStamina();

public:
	static const int MAX_STAMINA = 10;
	static int playerCount;
	static int escapeCount;
	static int deadCount;

private:
	int stamina;
	Timer staminaTimer;

	IMovable* movableInterface = nullptr;

	bool isOwner = false;
};
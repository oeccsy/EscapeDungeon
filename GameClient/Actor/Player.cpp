#include "Player.h"
#include "Input.h"
#include "Interface/IMovable.h"

Player::Player(const Vector2& position, IMovable* movable) : Actor("P", Color::Red, position), movableInterface(movable)
{
	SetSortingOrder(3);
}

void Player::BeginPlay()
{
	super::BeginPlay();
}

void Player::Tick(float deltaTime)
{
	super::Tick(deltaTime);

	if (Input::Get().GetKeyDown(VK_ESCAPE))
	{
		QuitGame();
		return;
	}

	if (Input::Get().GetKeyDown(VK_RIGHT))
	{
		Vector2 targetPos = GetPosition() + Vector2(1, 0);
		bool movable = movableInterface->Movable(targetPos);

		if (movable)
		{
			SetPosition(targetPos);
		}
	}

	if (Input::Get().GetKeyDown(VK_LEFT))
	{
		Vector2 targetPos = GetPosition() + Vector2(-1, 0);
		bool movable = movableInterface->Movable(targetPos);

		if (movable)
		{
			SetPosition(targetPos);
		}
	}

	if (Input::Get().GetKeyDown(VK_UP))
	{
		Vector2 targetPos = GetPosition() + Vector2(0, -1);
		bool movable = movableInterface->Movable(targetPos);

		if (movable)
		{
			SetPosition(targetPos);
		}
	}

	if (Input::Get().GetKeyDown(VK_DOWN))
	{
		Vector2 targetPos = GetPosition() + Vector2(0, 1);
		bool movable = movableInterface->Movable(targetPos);

		if (movable)
		{
			SetPosition(targetPos);
		}
	}
}

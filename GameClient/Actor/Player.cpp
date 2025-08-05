#include "Player.h"
#include "Input.h"
#include "Interface/IMovable.h"

Player::Player(const Vector2& position, IMovable* movable) : Actor("P", Color::Red, position), movableInterface(movable)
{
	SetSortingOrder(3);

	stamina = 5;
	staminaTimer.SetTimer(0.5f);

	Delegate staminaEvent(this, [](void* instance) { static_cast<Player*>(instance)->AddStamina(); });
	staminaTimer.Register(staminaEvent);
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

	Move();
	staminaTimer.Tick(deltaTime);
}

void Player::Move()
{
	if (stamina <= 0) return;
	

	if (Input::Get().GetKeyDown(VK_RIGHT))
	{
		Vector2 targetPos = GetPosition() + Vector2(1, 0);
		bool movable = movableInterface->Movable(targetPos);

		if (movable)
		{
			SetPosition(targetPos);
			--stamina;
		}
	}

	if (Input::Get().GetKeyDown(VK_LEFT))
	{
		Vector2 targetPos = GetPosition() + Vector2(-1, 0);
		bool movable = movableInterface->Movable(targetPos);

		if (movable)
		{
			SetPosition(targetPos);
			--stamina;
		}
	}

	if (Input::Get().GetKeyDown(VK_UP))
	{
		Vector2 targetPos = GetPosition() + Vector2(0, -1);
		bool movable = movableInterface->Movable(targetPos);

		if (movable)
		{
			SetPosition(targetPos);
			--stamina;
		}
	}

	if (Input::Get().GetKeyDown(VK_DOWN))
	{
		Vector2 targetPos = GetPosition() + Vector2(0, 1);
		bool movable = movableInterface->Movable(targetPos);

		if (movable)
		{
			SetPosition(targetPos);
			--stamina;
		}
	}
}

int Player::GetStamina()
{
	return stamina;
}

void Player::AddStamina()
{
	if (stamina < MAX_STAMINA)
	{
		++stamina;
	}
}

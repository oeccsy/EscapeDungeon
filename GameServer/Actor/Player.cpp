#include "Player.h"
#include "Input.h"
#include "Interface/IMovable.h"

#include "Collider/BoxCollider.h"

int Player::escapeCount = 0;
int Player::deadCount = 0;

Player::Player(const Vector2& position, IMovable* movable) : Actor("P", Color::Red, position), movableInterface(movable)
{
	SetSortingOrder(3);

	stamina = 5;
	staminaTimer.SetTimer(0.5f);

	Delegate staminaEvent(this, [](void* instance) { static_cast<Player*>(instance)->AddStamina(); });
	staminaTimer.Register(staminaEvent);

	collider = new BoxCollider({ 0, 0 }, { 1, 1 }, this);
}

void Player::Tick(float deltaTime)
{
	super::Tick(deltaTime);

	staminaTimer.Tick(deltaTime);
}

void Player::Move(Vector2 dir)
{
	if (stamina <= 0) return;
	if (dir.x * dir.x + dir.y * dir.y > 1) return;
 
	Vector2 targetPos = GetPosition() + dir;
	bool movable = movableInterface->Movable(targetPos);

	if (movable == false) return;
	
	SetPosition(targetPos);
	--stamina;
}

void Player::Escape()
{
	++escapeCount;
	Destroy();
}

void Player::Die()
{
	++deadCount;
	Destroy();
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

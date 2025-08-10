#include "Monster.h"
#include "Input.h"
#include "Interface/IMovable.h"

#include "Math/Vector2.h"
#include "Collider/BoxCollider.h"
#include "Actor/Player.h"

Monster::Monster(const Vector2& position, IMovable* movable) : Actor("M", Color::Red, position), movableInterface(movable)
{
	SetSortingOrder(3);

	stamina = 5;
	staminaTimer.SetTimer(0.25f);

	Delegate staminaEvent(this, [](void* instance) { static_cast<Monster*>(instance)->AddStamina(); });
	staminaTimer.Register(staminaEvent);

	collider = new BoxCollider({ 0, 0 }, { 1, 1 }, this);
}

void Monster::Tick(float deltaTime)
{
	super::Tick(deltaTime);

	staminaTimer.Tick(deltaTime);
}

void Monster::Move(Vector2 dir)
{
	if (stamina <= 0) return;
	if (dir.x * dir.x + dir.y * dir.y > 1) return;

	Vector2 targetPos = GetPosition() + dir;
	bool movable = movableInterface->Movable(targetPos);

	if (movable == false) return;

	SetPosition(targetPos);
	--stamina;
}

void Monster::Kill(Actor* actor)
{
	if (actor == nullptr) return;

	Player* player = actor->As<Player>();

	if (player) player->Die();
}

int Monster::GetStamina()
{
	return stamina;
}

void Monster::AddStamina()
{
	if (stamina < MAX_STAMINA)
	{
		++stamina;
	}
}
